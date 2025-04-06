#include "login.h"
#include "common/deal_mysql.h"
#include "common/cJSON.h"
#include "common/configure.h"
#include "common/make_log.h"
#include <stdio.h>
#include <stdlib.h>
#include <fcgi_stdio.h>

#define  LOGIN_LOG_MODULE       "fdfs_cgi_login"
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
        printf("Content-type: text/html\r\n\r\n");      // 返回消息

        if(contentLength != NULL)  length=atoi(contentLength);      // atoi()字符串转整型
        else length=0;   
        
        // 没有登陆用户信息
        if(length <= 0) { LOG(LOGIN_LOG_MODULE,"len = 0, No data from standard input"); printf("No data from standard input.<p>\n"); continue; }

        // 获取登录用户信息
        char buffer[4*1024] = {0}; char* out = NULL;
        int result = fread(buffer, 1, length, stdin); //从标准输入(web服务器)读取内容
        //LOG(LOGIN_LOG_MODULE, "buffer = %s",buffer );
        if(result == 0)  { LOG(LOGIN_LOG_MODULE, "fread(buf, 1, len, stdin) err");  continue; }

        //获取登陆用户的信息
        char user[512] = {0};
        char pass[512] = {0};

        result = get_login_info(buffer, user, pass);
        if(result == 0)         out="{\"code\":\"002\"}";
        else if(result == -1)   out="{\"code\":\"004\"}";
        else if(result == -2)   out="{\"code\":\"003\"}";

        LOG(LOGIN_LOG_MODULE, "user=%s pass=%s", user, pass);
        //给前端反馈信息
        if(out) { LOG(LOGIN_LOG_MODULE, "res = %s", out); printf(out);  /*free(out); */ } 
    }
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
        if(!root)  { LOG(LOGIN_LOG_MODULE, "cJSON_Parse err: %s", login_buf);  result = -1;  break; }

        // 返回指定字符串对应的json对象
        //用户
        cJSON *child=cJSON_GetObjectItem(root, "user");
        if(!child)  { LOG(LOGIN_LOG_MODULE, "cJSON_GetObjectItem user err: %s", login_buf);  result = -1;  break; }
        strcpy(user, child->valuestring);   //拷贝内容

        child=cJSON_GetObjectItem(root, "pwd");
        if(!child)  { LOG(LOGIN_LOG_MODULE, "cJSON_GetObjectItem pwd err: %s", login_buf);  result = -1;  break; }
        strcpy(pwd, child->valuestring);   //拷贝内容
        result = 0;
    } while (0);

    if(root)  cJSON_Delete(root);
    return result;
}
