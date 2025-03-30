/**
 * @file reg_cgi.c
 * @brief  注册事件后CGI程序
 * @author Mike
 * @version 2.0
 * @date 2017年2月14日
 */

#include "common/make_log.h"  //日志头文件
#include "common/deal_mysql.h"
#include <fcgi_stdio.h>

#define REG_LOG_MODULE       "fdfs_cgi"
#define REG_LOG_PROC         "register"

int  register_proc();
int user_register(char *reg_buf); //注册用户，成功返回0，失败返回-1, 该用户已存在返回-2

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
        if(contentLength == NULL)
            length = atoi(contentLength);   // 字符串转整型
        else
            length = 0;
        
        if(length <= 0)
        {
            // 没有登陆用户信息
            printf("No data from standard input.<p>\n");
            LOG(REG_LOG_MODULE, REG_LOG_PROC, "len = 0, No data from standard input\n");
            continue;
        }

        // 获取登录用户信息
        char buffer[4*1024] = {0};
        int result = fread(buffer, 1, length, stdin); //从标准输入(web服务器)读取内容
        char* out = NULL;

        if(result == 0)
        {
            LOG(REG_LOG_MODULE, REG_LOG_PROC, "fread(buf, 1, len, stdin) err\n");
            continue;
        }

        LOG(REG_LOG_MODULE, REG_LOG_PROC, "buf = %s\n", buffer);

        result = user_register( buffer );
        if(result == 0) // 注册成功
        {
            // 返回前端注册情况， 002代表成功
            print('{"code":"002"}');
        }else if(result == -1)
        {
            //返回前端注册情况， 004代表失败
            print('{"code":"004"}');
        }else if(result == -2)
        {
            print('{"code":"003"}');
        }

        if(out != NULL)
        {
            printf(out); //给前端反馈信息
            free(out);
        }
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
    int resualt = 0;
    MYSQL* conn = NULL;
    char mysql_user[256] = {0};
    char mysql_pwd[256] = {0};
    char mysql_db[256] = {0};

    do
    {
        // 获取数据库账户密码
        resualt = get_mysql_info(mysql_user, mysql_pwd, mysql_db);
        if(resualt != 0 )
            break;
        LOG(REG_LOG_MODULE, REG_LOG_PROC, "mysql_user = %s, mysql_pwd = %s, mysql_db = %s\n", mysql_user, mysql_pwd, mysql_db);

        //获取注册用户的信息
        char user[128];
        char nick_name[128];
        char pwd[128];
        char tel[128];
        char email[128];
        resualt = get_reg_info(reg_buf, user, nick_name, pwd, tel, email);
        if(resualt != 0)
            break;
        LOG(REG_LOG_MODULE, REG_LOG_PROC, "user = %s, nick_name = %s, pwd = %s, tel = %s, email = %s\n", user, nick_name, pwd, tel, email);

        //connect the database
        conn = mysql_conn(mysql_user, mysql_pwd, mysql_db);
        if(conn == NULL)
        {
            LOG(REG_LOG_MODULE, REG_LOG_PROC, "msql_conn err\n");
            resualt = -1;
            break;
        }
        //设置数据库编码，主要处理中文编码问题
        mysql_query(conn, "set names utf8");
        char sql_cmd[SQL_MAX_LEN] = {0};

        // 查看此用户是否存在
        sprintf(sql_cmd, "select * from user where name = '%s'", user);
        //返回值： 0成功并保存记录集，1没有记录集，2有记录集但是没有保存，-1失败
        resualt = process_result_one(conn, sql_cmd, NULL);
        if(resualt == 2)  // 用户存在
        {
            LOG(REG_LOG_MODULE, REG_LOG_PROC, "【%s】该用户已存在\n");
            resualt = -2;
            break;
        }

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
        if(mysql_query(conn, sql_cmd) != 0)
        {
            LOG(REG_LOG_MODULE, REG_LOG_PROC, "%s 用户数据插入失败：%s\n", sql_cmd, mysql_error(conn));
            resualt = -1;
            break;
        }
        LOG(REG_LOG_MODULE, REG_LOG_PROC, "%s 用户数据插入成功!!\n", sql_cmd);
        break;
    } while (0);

    if(conn)
        mysql_close(conn);
    return resualt;
}