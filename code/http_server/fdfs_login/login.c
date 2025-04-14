#include "login.h"
#include "common/configure.h"
#include "common/make_log.h"
#include "common/cJSON.h"
#include "common/deal_mysql.h"
#include "common/cryptutil.h"
#include "common/redis_op.h"
#include <stdio.h>
#include <stdlib.h>
#include <fcgi_stdio.h>

#define  LOGIN_LOG_MODULE       "fdfs_cgi_login"
#define TOKEN_LEN       128
#define PWD_LEN             (256)	//密码长度
int main()
{
    login_proc();
    return 0;
}

int  login_proc()
{
    while (FCGI_Accept() >= 0)
    {
        char* contentLength = getenv("CONTENT_LENGTH");
        int length = 0;
        char token[TOKEN_LEN];
        printf("Content-type: text/html\r\n\r\n");      // 返回消息

        if(contentLength != NULL)  length=atoi(contentLength);      // atoi()字符串转整型
        else length=0;   
        
        // 没有登陆用户信息
        if(length <= 0) { LOG_ERROR(LOGIN_LOG_MODULE,"len = 0, No data from standard input"); printf("No data from standard input.<p>\n"); continue; }

        // 获取登录用户信息
        char buffer[4*1024] = {0}; char out[1024];
        int result = fread(buffer, 1, length, stdin); //从标准输入(web服务器)读取内容
        //LOG(LOGIN_LOG_MODULE, "buffer = %s",buffer );
        if(result == 0)  { LOG_ERROR(LOGIN_LOG_MODULE, "fread(buf, 1, len, stdin) err");  continue; }

        // 用户登录
        result=user_login_in(buffer, token);
        if(result == 0)         sprintf(out, "{\"code\":\"000\", \"token\":\"%s\"}", token);
        else if(result == -2)   sprintf(out, "{\"code\":\"001\", \"token\":\"user does not exist\"}");
        else if(result == -3)   sprintf(out, "{\"code\":\"001\", \"token\":\"the password is incorrect\"}");
        else                    sprintf(out, "{\"code\":\"001\", \"token\":\"faild\"}");
        //给前端反馈信息
        if(out) { LOG_INFO(LOGIN_LOG_MODULE, "res = %s", out); printf(out);  /*free(out); */ } 
    }
}

int user_login_in(char *reg_buf, char *token)
{
    // 获取登录用户信息
    char login_user[128]={0}, login_pwd[128]={0};
    int result = get_login_info(reg_buf, login_user, login_pwd);
    if(result != 0)  return result;
    LOG_INFO(LOGIN_LOG_MODULE, "login in user=%s pass=%s", login_user, login_pwd);

    // 检查登录用户,成功返回0，失败返回-1,用户不存在返回-2
    result = check_user_pwd(login_user, login_pwd);
    LOG_INFO(LOGIN_LOG_MODULE, "check_user_pwd result=%d", result);
    if(result != 0)   return result;

    //生成token字符串
    memset(token, 0, TOKEN_LEN);
    result=set_token(login_user, token);
    LOG_INFO(LOGIN_LOG_MODULE, "token = %s", token);
    return 0;
}

//解析用户登陆信息的json包login_buf
//用户名保存在user，密码保存在pwd
int get_login_info(char *login_buf, char *user, char *pwd)
{
    int result;
    cJSON *root;

    if(!login_buf || !user || !pwd )   return -1;

    do
    {
        //解析json包
        //解析一个json字符串为cJSON对象
        root = cJSON_Parse(login_buf);
        if(!root)  { LOG_ERROR(LOGIN_LOG_MODULE, "cJSON_Parse err: %s", login_buf);  result = -1;  break; }

        // 返回指定字符串对应的json对象
        //用户
        cJSON *child=cJSON_GetObjectItem(root, "user");
        if(!child)  { LOG_ERROR(LOGIN_LOG_MODULE, "cJSON_GetObjectItem user err: %s", login_buf);  result = -1;  break; }
        strcpy(user, child->valuestring);   //拷贝内容

        child=cJSON_GetObjectItem(root, "pwd");
        if(!child)  { LOG_ERROR(LOGIN_LOG_MODULE, "cJSON_GetObjectItem pwd err: %s", login_buf);  result = -1;  break; }
        strcpy(pwd, child->valuestring);   //拷贝内容
        result = 0;
    } while (0);

    if(root)  cJSON_Delete(root);
    return result;
}

/* -------------------------------------------*/
/**
 * @brief  判断用户登陆情况
 *
 * @param user 		用户名
 * @param pwd 		密码
 *
 * @returns
 *      成功: 0
 *      失败：-1
 */
 /* -------------------------------------------*/
int check_user_pwd( char *user, char *pwd)
{
    //获取登陆用户的信息
    int result;
    char sql_cmd[SQL_MAX_LEN]= {0};
    char mysql_user[256] = {0};
    char mysql_pwd[256] = {0};
    char mysql_db[256] = {0};
    MYSQL *conn;

    do
    {
        // 获取数据库账户密码
        result = get_mysql_info(mysql_user, mysql_pwd, mysql_db);
        if(result != 0 )  break;
        LOG_INFO(LOGIN_LOG_MODULE, "mysql_user = %s, mysql_pwd = %s, mysql_db = %s", mysql_user, mysql_pwd, mysql_db);

        // connect the database
        conn = msql_conn(mysql_user, mysql_pwd, mysql_db);
        if(conn == NULL) { LOG_ERROR(LOGIN_LOG_MODULE, "msql_conn err");  result = -1; break; }
        //设置数据库编码，主要处理中文编码问题
        mysql_query(conn, "set names utf8");

        //sql语句，查找某个用户对应的密码
        //返回值： 0成功并保存记录集，1没有记录集，2有记录集但是没有保存，-1失败
        char tmp[PWD_LEN] = {0}; //deal result
        sprintf(sql_cmd, "select password from user where name=\"%s\"", user);
        result = process_result_one(conn, sql_cmd, tmp);
        if(result == 1) { LOG_ERROR(LOGIN_LOG_MODULE, "login user '%s' does not exist", user); result = -2; break;  }  // 用户不存在
        else if(result != 0) { LOG_ERROR(LOGIN_LOG_MODULE, "login user '%s' select error", user); result = -1; break;  }  // 其它错误

        // 判断客户端发送的密码和数据库中的密码是否一致
        char md5_pass[16]; // 二进制MD5
        char hex_pass[33]; // 十六进制字符串

        MD5((unsigned char*)tmp, strlen(tmp), md5_pass);
        md5_to_hex(md5_pass, hex_pass);
        LOG_INFO(LOGIN_LOG_MODULE, "login password src=%s des=%s", pwd, hex_pass);
        if( 0==strcmp(hex_pass, pwd))   result=0;   else    result=-3;           // 密码错误
    
    } while (0);

    if(conn)    mysql_close(conn);
    return result;
}


/* -------------------------------------------*/
/**
 * @brief  生成token字符串, 保存redis数据库
 *
 * @param user 		用户名
 * @param token     生成的token字符串
 *
 * @returns
 *      成功: 0
 *      失败：-1
 */
 /* -------------------------------------------*/
 int set_token(char *user, char *token)
 {
     int result;
     redisContext *redis_conn=NULL;
     char redis_ip[30]={0}, redis_port[10]={0};

    do
    {
        // 读取redis配置信息
        result = get_redis_info(redis_ip, redis_port);
        if(result != 0 )  break;
        LOG_INFO(LOGIN_LOG_MODULE, "redis_ip = %s, redis_port = %s", redis_ip, redis_port);

        // 连接redis数据库
        redis_conn = rop_connectdb_nopwd(redis_ip, redis_port);
        if(redis_conn == NULL) { LOG_ERROR(LOGIN_LOG_MODULE, "redis connected error");  result = -1; break; }


        //产生4个1000以内的随机数
        int rand_num[4] = {0};
        srand((unsigned int)time(NULL));
        for (size_t i = 0; i < 4; ++i)  rand_num[i]=rand()%1000;        //随机数
        
        char tmp[1024] = {0};
        sprintf(tmp, "%s%d%d%d%d", user, rand_num[0], rand_num[1], rand_num[2], rand_num[3]);
        LOG_INFO(LOGIN_LOG_MODULE, "tmp = %s", tmp);

        // 加密
        char enc_tmp[1024*2] = {0};
        int enc_len = 0;
        
        result=desEncryptText((unsigned char*)tmp, strlen(tmp), (unsigned char*)enc_tmp, &enc_len);
        if(result != 0 )    { LOG_ERROR(LOGIN_LOG_MODULE, "desEncryptText error");  result = -1; break; }

        // to base64
        char base64[1024*3] = {0};
        base64_encode((const unsigned char *)enc_tmp, enc_len, base64); //base64编码
        LOG_INFO(LOGIN_LOG_MODULE, "base64 = %s", base64);

        // to md5
        MD5_CTX md5;
        unsigned char decrypt[16];
        md5Init(&md5);
        md5Update(&md5, (unsigned char*)base64, strlen(base64));
        md5Final(&md5, decrypt);

        char str[100] = { 0 };
        md5_to_hex(decrypt, token);

        // redis保存此字符串，用户名 : token, 有效时间为24小时
        result= rop_setex_string(redis_conn, user, 86400, token);
        //result = rop_setex_string(redis_conn, user, 30, token); //30秒

    } while (0);
    
    if(redis_conn)   rop_disconnect(redis_conn);
    return result;
 }