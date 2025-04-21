#include "file_upload.h"
#include "fastcommon/logger.h"
#include "common/make_log.h"
#include "common/deal_mysql.h"
#include "common/fdfs_api.h"
#include "common/util_cgi.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <fcgi_stdio.h>
#include <string.h>

#define UPLOAD_LOG_MODULE "fdfs_upload"

//mysql 数据库配置信息 用户名， 密码， 数据库名称
static char mysql_user[128] = {0};
static char mysql_pwd[128] = {0};
static char mysql_db[128] = {0};

//redis 服务器ip、端口
//static char redis_ip[30] = {0};
//static char redis_port[10] = {0};

void read_configuer()
{
    // 获取数据库账户密码
    int result = get_mysql_info(mysql_user, mysql_pwd, mysql_db);
    if(result != 0 )  return;
    LOG_INFO(UPLOAD_LOG_MODULE, "mysql_user = %s, mysql_pwd = %s, mysql_db = %s", mysql_user, mysql_pwd, mysql_db);

    /*
    // 读取redis配置信息
    result = get_redis_info(redis_ip, redis_port);
    if(result != 0 )  return;
    LOG_INFO(UPLOAD_LOG_MODULE, "redis_ip = %s, redis_port = %s", redis_ip, redis_port);
    */
}

/* -------------------------------------------*/
/**
 * @brief  解析上传的post数据 保存到本地临时路径
 *         同时得到文件上传者、文件名称、文件大小
 *
 * @param len       (in)    post数据的长度
 * @param user      (out)   文件上传者
 * @param file_name (out)   文件的文件名
 * @param md5       (out)   文件的MD5码
 * @param p_size    (out)   文件大小
 *
 * @returns
 *          0 succ, -1 fail
 */
/* -------------------------------------------*/
int recv_save_file(long len, char *user, char *filename, char *md5, long *p_size)
{
    int result = 0;
    char* file_buffer = NULL;
    char* begin = NULL;
    char *p;

    char content_text[TEMP_BUF_MAX_LEN] = {0};      // 文件头部信息
    char boundary[TEMP_BUF_MAX_LEN] = {0};          // 分界线信息

    //==========> 申请存放文件的 内存 <===========
    do
    {
        file_buffer = (char*)malloc(len);
        if(NULL == file_buffer)   {  LOG_ERROR(UPLOAD_LOG_MODULE, "malloc error! file size is to big!!!!");  result = -1;  break; }
        int ret2 = fread(file_buffer, 1, len, stdin);       // 从标准输入(web服务器)读取内容
        if(0 == ret2)   { LOG_ERROR(UPLOAD_LOG_MODULE, "fread(file_buf, 1, len, stdin) err"); result = -1; break; }
        // ===========> 开始处理前端发送过来的post数据格式 <============
        p = begin = file_buffer;        // 内存起点

        /*
        ------WebKitFormBoundary88asdgewtgewx\r\n
        Content-Disposition: form-data; user="mike"; filename="xxx.jpg"; md5="xxxx"; size=10240\r\n
        Content-Type: application/octet-stream\r\n
        \r\n
        真正的文件内容\r\n
        ------WebKitFormBoundary88asdgewtgewx
        */

        // get boundary 得到分界线, ------WebKitFormBoundary88asdgewtgewx
        p = strstr(begin, strlen("\r\n"));
        if(NULL == p)   { LOG_ERROR(UPLOAD_LOG_MODULE, "wrong no boundary!"); result = -1; break; }

        
        
        // 拷贝分界线
        strncpy(boundary, begin, p-begin);
        boundary[p-begin] = '\0';               //字符串结束符
        LOG_INFO(UPLOAD_LOG_MODULE, "boundary: [%s]", boundary);

        p += strlen("\r\n");    // 指针跳转到http 请求体
        len -= (p-begin);       // 已经处理了p-begin的长度
        begin = p;              // get content text head

        // Content-Disposition: form-data; user="mike"; filename="xxx.jpg"; md5="xxxx"; size=10240\r\n
        p = strstr(begin, strlen("\r\n"));
        if(NULL == p)   { LOG_ERROR(UPLOAD_LOG_MODULE, "ERROR: get context text error, no filename?"); result = -1; break; }
        strncpy(content_text, begin, p-begin); content_text[p-begin] = '\0';
        LOG_INFO(UPLOAD_LOG_MODULE, "content_text: [%s]", content_text);
        p += strlen("\r\n"); len -= (p-begin);

        char* first_ptr =NULL, *last_ptr=NULL;
        //========================================获取文件上传者
        //Content-Disposition: form-data; user="mike"; filename="xxx.jpg"; md5="xxxx"; size=10240\r\n
        first_ptr = (char*)strstr(begin, "user=") + strlen("user=") +1;  // 跳到第一个"号
        last_ptr = strchr(first_ptr, '"');            // 跳到第二"号
        strncpy(user, first_ptr, last_ptr-first_ptr);      // 拷贝用户名
        user[last_ptr-first_ptr] = '\0';  trim_space(user);       // 去掉字符串两边的空白字符
        begin=last_ptr+1;

        //========================================获取文件名字
        // filename="xxx.jpg"; md5="xxxx"; size=10240\r\n
        first_ptr = (char*)strstr(begin, "filename=") + strlen("filename=") +1;  // 跳到第一个"号
        last_ptr = strchr(first_ptr, '"');                // 跳到第二"号
        strncpy(filename, first_ptr, last_ptr-first_ptr);        //拷贝文件名
        filename[last_ptr-first_ptr] = '\0'; trim_space(filename);   // 去掉字符串两边的空白字符
        begin=last_ptr+1;

        //========================================获取文件MD5码
        //md5="xxxx"; size=10240\r\n
        first_ptr = (char*)strstr(begin, "md5=") + strlen("md5=") +1;  // 跳到第一个"号
        last_ptr = strchr(first_ptr, '"');                // 跳到第二"号
        strncpy(md5, first_ptr, last_ptr-first_ptr);            // 拷贝文件MD5值
        md5[last_ptr-first_ptr] = '\0'; trim_space(md5);        // 去掉字符串两边的空白字符
        begin=last_ptr+1;

        //========================================获取文件大小
        // size=10240\r\n
        char tmp[256] = {0};
        first_ptr = (char*)strstr(begin, "size=") + strlen("size="); 
        last_ptr = strchr(first_ptr, '\r\n');
        strncpy(tmp, first_ptr, last_ptr-first_ptr);            // 拷贝文件大小
        tmp[last_ptr-first_ptr] = '\0'; *p_size = strtol(tmp, NULL, 10); //字符串转long

        // 拷贝文件内容
        begin = p; 
        p = strstr(begin, "\r\n"); p += strlen("\r\n\r\n"); len -= (p-begin); begin = p;
        p = memstr(begin, len, boundary);       // 找到文件结尾
        if(NULL == p)   { LOG_ERROR(UPLOAD_LOG_MODULE, "memstr(begin, len, boundary) error"); result = -1; break; }
        p = p-strlen("\r\n");
        // begin---> file_len = (p-begin)
        // =====> 此时begin-->p两个指针的区间就是post的文件二进制数据
        // ======>将数据写入文件中,其中文件名也是从post数据解析得来  <===========
        int fd = open(filename, O_CREAT|O_WRONLY, 0644);
        if(fd <0)   { LOG_ERROR(UPLOAD_LOG_MODULE, "open %s error", filename); result = -1; break; }
        // 写入文件
        // ftruncate会将参数fd指定的文件大小改为参数length指定的大小
        ftruncate(fd, (int)(p-begin)); write(fd, begin, (int)(p-begin)); close(fd);
        result = 0;
    } while (0);
    
    if(file_buffer)   free(file_buffer);
    return result;
}

/* -------------------------------------------*/
/**
 * @brief  将一个本地文件上传到 后台分布式文件系统中
 *
 * @param filename  (in) 本地文件的路径
 * @param fileid    (out)得到上传之后的文件ID路径
 *
 * @returns
 *      0 succ, -1 fail
 */
/* -------------------------------------------*/
int upload_to_dstorage(char *filename, char *fileid)
{
    int result = 0;

    // 读取fdfs client 配置文件的路径
    char fdfs_cli_conf_path[256] = {0};
    result = get_dfs_cli_path(fdfs_cli_conf_path);
    if(result != 0 )  return -1;

    result = fdfs_upload_file(fdfs_cli_conf_path, filename, fileid);
    if(result != 0 || strlen(fileid) == 0 )   { LOG_ERROR(UPLOAD_LOG_MODULE, "fdfs_upload_file error:%d", result); return -1; }
    LOG_INFO(UPLOAD_LOG_MODULE,  "get [%s] succ!\n", fileid);
    return result;
}

int store_fileinfo_to_mysql(char *user, char *filename, char *md5, long size, char *fileid, char *fdfs_file_url)
{
    int result = 0;
    MYSQL* conn = NULL;
    char suffix[SUFFIX_LEN];
    char sql_cmd[SQL_MAX_LEN] = {0};

    time_t now;
    char create_time[TIME_STRING_LEN];

    do
    {
        conn = msql_conn(mysql_user, mysql_pwd, mysql_db);
        if(!conn)  { LOG_ERROR(UPLOAD_LOG_MODULE, "msql_conn connect err"); result = -1; break; }

        //设置数据库编码
        mysql_query(conn, "set names utf8");

        //得到文件后缀字符串 如果非法文件后缀,返回"null"
        get_file_suffix(filename, suffix);      // mp4, jpg, png

        //sql 语句
        /*
            -- =============================================== 文件信息表
            -- md5 文件md5
            -- file_id 文件id
            -- url 文件url
            -- size 文件大小, 以字节为单位
            -- type 文件类型： png, zip, mp4……
            -- count 文件引用计数， 默认为1， 每增加一个用户拥有此文件，此计数器+1
        */
        sprintf(sql_cmd, "insert into file_info (md5, file_id, url, size, type, count) values ('%s', '%s', '%s', '%ld', '%s', %d)", md5, fileid, fdfs_file_url, size, suffix, 1);
        result = mysql_query(conn, sql_cmd);
        if(0 != result)   { LOG_ERROR(UPLOAD_LOG_MODULE, "%s 插入失败: %s", sql_cmd, mysql_error(conn)); result =-1; break;}
        LOG_INFO(UPLOAD_LOG_MODULE, "%s 文件信息插入成功", sql_cmd);

        // 获取当前时间
        now = time(NULL);
        strftime(create_time, TIME_STRING_LEN-1, "%Y-%m-%d %H:%M:%S", localtime(&now));

        //sql语句
        /*
        -- =============================================== 用户文件列表
        -- user 文件所属用户
        -- md5 文件md5
        -- createtime 文件创建时间
        -- filename 文件名字
        -- shared_status 共享状态, 0为没有共享， 1为共享
        -- pv 文件下载量，默认值为0，下载一次加1
        */
        sprintf(sql_cmd, "insert into user_file_list(user, md5, createtime, filename, shared_status, pv) values ('%s', '%s', '%s', '%s', %d, %d)", user, md5, create_time, filename, 0, 0);
        result = mysql_query(conn, sql_cmd);
        if(0 != result)   { LOG_ERROR(UPLOAD_LOG_MODULE, "%s 插入失败: %s", sql_cmd, mysql_error(conn)); result =-1; break; }

        // 查询用户文件数量
        char tmp[512] = {0};
        int count = 0;
        sprintf(sql_cmd, "select count from user_file_count where user = '%s'", user);
        //返回值： 0成功并保存记录集，1没有记录集，2有记录集但是没有保存，-1失败
        result = process_result_one(conn, sql_cmd, tmp); //执行sql语句
        if(result == 1)   { sprintf(sql_cmd, " insert into user_file_count (user, count) values('%s', %d)", user, 1); }
        else if(result == 0)   { count = atoi(tmp); sprintf(sql_cmd, "update user_file_count set count = %d where user = '%s'", count+1, user); }

        result = mysql_query(conn, sql_cmd);
        if(0 != result)   { LOG_ERROR(UPLOAD_LOG_MODULE, "%s 操作失败: %s", sql_cmd, mysql_error(conn)); result =-1; break; }
        result = 0;
    } while (0);
    
    // 连接 mysql 数据库
    if(conn)  mysql_close(conn);
    return result;
}

int  file_upload_proc()
{
    int result = 0;
    char filename[FILE_NAME_LEN] = {0};     // 文件名
    char user[USER_NAME_LEN] = {0};         // 文件上传者
    char md5[MD5_LEN] = {0};                // 文件md5码
    int size=0;                             // 文件大小
    char fileid[TEMP_BUF_MAX_LEN] = {0};    // 文件上传到fastDFS后的文件id

    char fdfs_file_url[FILE_URL_LEN] = {0}; //文件所存放storage的host_name
    char* contentLength = NULL;

    read_configuer();       // 读取数据库配置信息

    while (FCGI_Accept() >= 0)
    {
        printf("Content-Type:text\r\n\r\n");
        contentLength = getenv("CONTENT_LENGTH");
        long length=0;

        
        // 读取消息,存放缓冲区
        if(contentLength != NULL)  length=strtol(contentLength, NULL, 10);      // 获取消息长度,转换成10进制
        else length=0;   
    
        if(length <= 0)   {
            printf("No data from standard input.<p>\n");
            LOG_ERROR(UPLOAD_LOG_MODULE, "len = 0, No data from standard input");
            continue;
        }
        
        do
        {
            //===============> 得到上传文件  <============
            //result = recv_save_file(length, user, filename, md5, &size);
            result = get_file_context(length, filename, &size);
            if(result <0)   { result =-1; break; }
            LOG_INFO(UPLOAD_LOG_MODULE, "%s成功上传[%s, 大小：%ld, md5码：%s]到服务器", user, filename, size, md5);

            //===============> 将该文件存入fastDFS中,并得到文件的file_id <============
            result = upload_to_dstorage(filename, fileid);
            if(0 != result)   { result =-1; break; }

            //===============> 将该文件的FastDFS相关信息存入mysql中 <======
            result = store_fileinfo_to_mysql(user, filename, md5, size, fileid, fdfs_file_url);
            if(0 != result)   { result =-1; break; }
            
            //================> 删除本地临时存放的上传文件 <===============
            unlink(filename);
        } while (0);
        
        memset(filename, 0, FILE_NAME_LEN);
        memset(user, 0, USER_NAME_LEN);
        memset(md5, 0, MD5_LEN);
        memset(fileid, 0, TEMP_BUF_MAX_LEN);
        memset(fdfs_file_url, 0, FILE_URL_LEN);
    
        // 给前端返回，上传情况
        /*
           上传文件：
           成功：{"code":"008"}
           失败：{"code":"009"}
        */
        //char *out = NULL;
        char out[1024] = {0};     
        if(result == 0)  { sprintf(out, "{\"code\":\"008\"\0"); /*out = return_status("008");*/ }
        else             { sprintf(out, "{\"code\":\"009\"\0"); /*out = return_status("009");*/ }
        //if(out)          { printf(out);  free(out); }
        printf(out);
    }
}

int get_file_context(long len, char *filename, long *p_size)
{
    char boundary[TEMP_BUF_MAX_LEN] = {0};     // png文件分界线字符串
    int ch=0;
    char* file_data = (char*)malloc(len);        // 根据post数据长度, 创建对应大小的数组
    char *begin, *end, *p;
    end = NULL;
    p = begin = file_data;
    for (size_t i = 0; i < len; i++)
    {
        if( 0 > (ch = getchar()) )
        {
            printf("Error: Not enough bytes received on standard input<p>\n");
            break;
        }
        *p = ch;
        p++;
    }
    end = p;


    p = (char*)strstr(begin, "\r\n") + strlen("\r\n");   // 指针跳转到http 请求头
    strncpy(boundary, begin, p-begin-strlen("\r\n"));
    len -= (p-begin);
    LOG_INFO("fdfs_upload", "boundary: %s", boundary);
    printf("boundary: %s", boundary);

    begin = p;
    p = (char*)strstr(p, "\r\n") + + strlen("\r\n");      // 指针跳转到http 请求体
    len -= (p-begin);

    // 读取filename filename="xxxxx"
    char* first_ptr = (char*)strstr(begin, "filename=") + strlen("filename=") +1;  // 跳到第一个"号
    char* last_ptr = strchr(first_ptr, '"');            // 跳到第二"号
    strncpy(filename, first_ptr, last_ptr-first_ptr);
    LOG_INFO("fdfs_upload", "<br>filename: %s<br>", filename);
    printf("<br>filename: %s<br>", filename);

    // 读取 content-type
    begin = p;
    p = (char*)strstr(begin, "\r\n") + strlen("\r\n") + strlen("\r\n");       // 跳过两个\r\n
    len -= (p-begin);

    // 读取文件正文
    begin = p;
    p = memstr(begin, len, boundary);
    if(p)
        p -= strlen("\r\n"); // 减去\r\n长度
    else
        p = end - strlen("\r\n");

    // 将上传文件内容写入文件中
    int fd = open(filename, O_CREAT|O_WRONLY, 0664);
    write(fd, begin, p-begin);
    close(fd);
    *p_size = p-begin;
}