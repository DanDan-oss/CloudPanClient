/**
 * @file reg_cgi.c
 * @brief  注册事件后CGI程序
 * @author Mike
 * @version 2.0
 * @date 2017年2月14日
 */

#include "common/make_log.h"  //日志头文件
#include "common/deal_mysql.h"
#include "common/cJSON.h"
#include "common/configure.h"
#include <fcgi_stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

//#include "../common/cJSON.h"

#define REG_LOG_MODULE       "fdfs_cgi_register"

int main()
{
    register_proc();
    return 0;
}

int  register_proc()
{
    char* contentLength = NULL;
    int length = 0;
    while (FCGI_Accept() >= 0)
    {
        char* contentLength = getenv("CONTENT_LENGTH");
        printf("Content-type: text/html\r\n\r\n");      // 返回消息

        if(contentLength != NULL)  length=atoi(contentLength);      // atoi()字符串转整型
        else length=0;   
        
        // 没有登陆用户信息
        if(length <= 0) { LOG(REG_LOG_MODULE,"len = 0, No data from standard input"); printf("No data from standard input.<p>\n"); continue; }

        // 获取登录用户信息
        char buffer[4*1024] = {0}; char* out = NULL;
        int result = fread(buffer, 1, length, stdin); //从标准输入(web服务器)读取内容
        //LOG(REG_LOG_MODULE, "buffer = %s",buffer );
        if(result == 0)  { LOG(REG_LOG_MODULE, "fread(buf, 1, len, stdin) err");  continue; }

        // 获取注册状态
        // ,返回前端注册情况， 002代表注册成功
        //返回前端注册情况， 004代表失败
        result=user_register(buffer);
        if(result == 0)         out="{\"code\":\"002\"}";   //out = return_status("002"); //util_cgi.h
        else if(result == -1)   out="{\"code\":\"004\"}";   //out = return_status("004"); //util_cgi.h
        else if(result == -2)   out="{\"code\":\"003\"}";   //out = return_status("003"); //util_cgi.h

        //给前端反馈信息
        if(out) { LOG(REG_LOG_MODULE, "res = %s", out); printf(out);  /*free(out); */ } 
    }

}

//注册用户，成功返回0，失败返回-1, 该用户已存在返回-2
/*
注册：
    成功：{"code":"002"}
    该用户已存在：{"code":"003"}
    失败：{"code":"004"}
*/
int user_register(char* reg_buf)
{
    int resualt;
    MYSQL* conn;
    char mysql_user[256] = {0};
    char mysql_pwd[256] = {0};
    char mysql_db[256] = {0};

    do
    {
        // 获取数据库账户密码
        resualt = get_mysql_info(mysql_user, mysql_pwd, mysql_db);
        if(resualt != 0 )
            break;
        //memcpy(mysql_user, "debian-sys-maint", sizeof("debian-sys-maint"));
        //memcpy(mysql_pwd, "M5Uya0LliUrmcSuU", sizeof("M5Uya0LliUrmcSuU"));
        //memcpy(mysql_db, "dfs", sizeof("dfs"));
        LOG(REG_LOG_MODULE, "mysql_user = %s, mysql_pwd = %s, mysql_db = %s", mysql_user, mysql_pwd, mysql_db);

        //获取注册用户的信息
        char user[128], nick_name[128], pwd[128], tel[128], email[128];
        resualt = get_reg_info(reg_buf, user, nick_name, pwd, tel, email);
        if(resualt != 0)  break;
        LOG(REG_LOG_MODULE, "user = %s, nick_name = %s, pwd = %s, tel = %s, email = %s", user, nick_name, pwd, tel, email);

        // connect the database
        conn = msql_conn(mysql_user, mysql_pwd, mysql_db);
        if(conn == NULL) { LOG(REG_LOG_MODULE, "msql_conn err");  resualt = -1; break; }

        //设置数据库编码，主要处理中文编码问题
        mysql_query(conn, "set names utf8");

        // 查看此用户是否存在
        //返回值： 0成功并保存记录集，1没有记录集，2有记录集但是没有保存，-1失败
        char sql_cmd[SQL_MAX_LEN] = {0};
        sprintf(sql_cmd, "select * from user where name = '%s'", user);
        resualt = process_result_one(conn, sql_cmd, NULL);
        if(resualt == 2) { LOG(REG_LOG_MODULE, "Registered user, user '%s' already exists", user); resualt = -2; break;  }  // 用户存在

        //当前时间戳
        struct timeval tv;
        struct tm* ptm;
        char time_str[128];
        gettimeofday(&tv, NULL);
        ptm = localtime(&tv.tv_sec);//把从1970-1-1零点零分到当前时间系统所偏移的秒数时间转换为本地时间
        //strftime() 函数根据区域设置格式化本地时间/日期，函数的功能将时间格式化，或者说格式化一个时间字符串
        strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", ptm);

        // 插入注册信息
        sprintf(sql_cmd, "insert into user (name, nickname, password, phone, createtime, email) values ('%s', '%s', '%s', '%s', '%s', '%s')", user, nick_name, pwd, tel, time_str ,email);
        resualt=mysql_query(conn, sql_cmd);
        if(resualt != 0) { LOG(REG_LOG_MODULE, "%s 用户数据插入失败：%s", sql_cmd, mysql_error(conn)); resualt = -1;  break; }

        LOG(REG_LOG_MODULE, "%s 用户数据插入成功!!", sql_cmd);
        resualt=0;
        break;
    } while (0);

    if(conn)  mysql_close(conn);
    return resualt;
}

int get_reg_info(char *reg_buf, char *user, char *nick_name, char *pwd, char *tel, char *email)
{
    int resualt;
    cJSON *child, *root;

    /*json数据如下
    {
        userName:xxxx,
        nickName:xxx,
        firstPwd:xxx,
        phone:xxx,
        email:xxx
    }
    */

    //解析json包
    //解析一个json字符串为cJSON对象
    do
    {
        root=cJSON_Parse(reg_buf);
        if(!root) { LOG(REG_LOG_MODULE, "cJSON_Parse err: %s", reg_buf);  resualt = -1;  break; }

        //返回指定字符串对应的json对象
        child=cJSON_GetObjectItem(root, "userName");
        if(!child) { LOG(REG_LOG_MODULE, "cJSON_GetObjectItem userName err: %s", reg_buf); resualt = -1;  break; }
        strcpy(user, child->valuestring);  // 拷贝内容
        //LOG(REG_LOG_MODULE, "child->valuestring = %s\n", child->valuestring);
        
        // 昵称
        child=cJSON_GetObjectItem(root, "nickName");
        if(!child) { LOG(REG_LOG_MODULE, "cJSON_GetObjectItem nickName err: %s", reg_buf); resualt = -1;  break; }
        strcpy(nick_name, child->valuestring); //拷贝内容

        // 密码
        child=cJSON_GetObjectItem(root, "firstPwd");
        if(!child) { LOG(REG_LOG_MODULE, "cJSON_GetObjectItem firstPwd err: %s", reg_buf); resualt = -1;  break; }
        strcpy(pwd, child->valuestring);

        //电话
        child=cJSON_GetObjectItem(root, "phone");
        if(!child) { LOG(REG_LOG_MODULE, "cJSON_GetObjectItem phone err: %s", reg_buf); resualt = -1;  break; }
        strcpy(tel, child->valuestring);

        // 邮箱
        child=cJSON_GetObjectItem(root, "email");
        if(!child) { LOG(REG_LOG_MODULE, "cJSON_GetObjectItem email err: %s", reg_buf); resualt = -1;  break; }
        strcpy(email, child->valuestring);
        
        resualt=0;
    } while (false);
    
    if(root)  cJSON_Delete(root);
    return resualt;

}