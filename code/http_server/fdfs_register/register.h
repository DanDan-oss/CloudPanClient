#ifndef  _REGISTER_H_
#define  _REGISTER_H_

int  register_proc();
int user_register(char *reg_buf); //注册用户，成功返回0，失败返回-1, 该用户已存在返回-2
int get_reg_info(char *reg_buf, char *user, char *nick_name, char *pwd, char *tel, char *email); //解析用户注册信息的json包
#endif # _REGISTER_H_