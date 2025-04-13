#ifndef  _LOGIN_H
#define  _LOGIN_H

int  login_proc();
int get_login_info(char *login_buf, char *user, char *pwd);
int user_login_in(char *reg_buf, char *token);
int check_user_pwd( char *user, char *pwd);
int set_token(char *user, char *token);
#endif // _LOGIN_H