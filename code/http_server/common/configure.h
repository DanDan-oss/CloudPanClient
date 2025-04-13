#ifndef CONFIGURE_H
#define CONFIGURE_H


/* -------------------------------------------*/
/**
 * @brief  从配置文件中得到相对应的参数
 *
 * @param profile   配置文件路径
 * @param tile      配置文件title名称[title]
 * @param key       key
 * @param value    (out)  得到的value
 *
 * @returns
 *      0 succ, -1 fail
 */
/* -------------------------------------------*/

extern int get_config_vaule(const char *profile, char *title, char *key, char *value);

// 获取数据库用户名、用户密码、数据库标识等信息
extern int get_mysql_info(char *mysql_user, char *mysql_passwd, char *mysql_db);

// 获取redis IP、端口
extern int get_redis_info(char *redis_ip, char *redis_port);
#endif