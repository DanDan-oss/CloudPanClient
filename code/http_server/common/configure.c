#include "configure.h"
#include "cJSON.h"
#include "make_log.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


#define CONFIG_PATH    "./conf/conf.json" //配置文件路径
#define CFG_LOG_MODULE "fdfs_cgi_configure"

/* -------------------------------------------*/
/**
 * @brief  从配置文件中得到相对应的参数
 *
 * @param profile   配置文件路径
 * @param title      配置文件title名称[title]
 * @param key       key
 * @param value    (out)  得到的value
 *
 * @returns
 *      0 succ, -1 fail
 */
/* -------------------------------------------*/
int get_config_vaule(const char *profile, char *title, char *key, char *value)
{
    int resualt;
    char *buffer = NULL;
    FILE *fp = NULL;
    cJSON *root= NULL;
    char path[256];

    // 处理异常空指针
    if(!profile || !title || !key || !value)  return -1;
    do
    {
        // 只读形式打开文件
        fp=fopen(profile, "rb");
        if(!fp)  {  getcwd(path, 256); perror("fopen"); LOG(CFG_LOG_MODULE, "fopen err: %s/%s", path, profile); resualt=-1; break; }

        fseek(fp, 0, SEEK_END); // 移动到文件末尾
        long fsize=ftell(fp);   // 获取文件大小
        fseek(fp, 0, SEEK_SET); // 移动到文件开头

        buffer=(char*)calloc(1, fsize+1);   //动态分配空间
        if(!buffer)   { perror("calloc"); LOG(CFG_LOG_MODULE, "calloc err"); resualt=-1; break; }

        // 读取文件内容
        fread(buffer, 1, fsize, fp);

        //解析一个json字符串为cJSON对象
        cJSON *root=cJSON_Parse(buffer);
        if(!root)  { LOG(CFG_LOG_MODULE, "cJSON root err"); resualt=-1; break; }

        cJSON *father=cJSON_GetObjectItem(root, title);
        if(!father)  { LOG(CFG_LOG_MODULE, "cJSON father err"); resualt=-1; break; }

        cJSON *son=cJSON_GetObjectItem(father, key);
        if(!son)  { LOG(CFG_LOG_MODULE, "cJSON son err"); resualt=-1; break; }

        //LOG(CFG_LOG_MODULE, "son->valuestring = %s\n", son->valuestring);
        strcpy(value, son->valuestring);
    } while (0);
    
    if(root) cJSON_Delete(root);
    if(buffer)  free(buffer);
    if(fp)  fclose(fp);
}

int get_mysql_info(char *mysql_user, char *mysql_passwd, char *mysql_db)
{
    if(-1 == get_config_vaule(CONFIG_PATH, "mysql", "user", mysql_user))  { LOG(CFG_LOG_MODULE, "mysql_user err"); return -1; }
    if(-1 == get_config_vaule(CONFIG_PATH, "mysql", "password", mysql_passwd))  { LOG(CFG_LOG_MODULE, "mysql_passwd err"); return -1; }
    if(-1 == get_config_vaule(CONFIG_PATH, "mysql", "database", mysql_db)) { LOG(CFG_LOG_MODULE, "mysql_db err"); return -1; }
    LOG(CFG_LOG_MODULE, "mysql_user=%s mysql_passwd=%s mysql_db=%s", mysql_user, mysql_passwd, mysql_db);
    return 0;
}

int get_redis_info(char *redis_ip, char *redis_port)
{
    if(-1 == get_config_vaule(CONFIG_PATH, "redis", "ip", redis_ip))  { LOG(CFG_LOG_MODULE, "redis_ip err"); return -1; }
    if(-1 == get_config_vaule(CONFIG_PATH, "redis", "port", redis_port))  { LOG(CFG_LOG_MODULE, "redis_port err"); return -1; }
    LOG(CFG_LOG_MODULE, "redis_ip=%s redis_port=%s", redis_ip, redis_port);
    return 0;
}