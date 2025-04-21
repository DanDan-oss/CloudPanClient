#ifndef _UTIL_CGI_H_
#define _UTIL_CGI_H_


#define FILE_NAME_LEN       (256)	//文件名字长度
#define TEMP_BUF_MAX_LEN    (512)	//临时缓冲区大小
#define FILE_URL_LEN        (512)   //文件所存放storage的host_name长度
#define HOST_NAME_LEN       (30)	//主机ip地址长度
#define USER_NAME_LEN       (128)	//用户名字长度
#define TOKEN_LEN           (128)	//登陆token长度
#define MD5_LEN             (256)   //文件md5长度
#define PWD_LEN             (256)	//密码长度
#define TIME_STRING_LEN     (25)    //时间戳长度
#define SUFFIX_LEN          (8)     //后缀名长度
#define PIC_NAME_LEN        (10)    //图片资源名字长度
#define PIC_URL_LEN         (256)   //图片资源url名字长度

int trim_space(char *inbuf);        // 去掉一个字符串两边的空白字符

char* memstr(char* full_data, int full_data_len, char* substr);

//通过文件名file_name， 得到文件后缀字符串, 保存在suffix 如果非法文件后缀,返回"null"
int get_file_suffix(const char *file_name, char *suffix);
#endif