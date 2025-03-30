#ifndef  _FILE_UPLOAD_H_
#define  _FILE_UPLOAD_H_

#define FILE_MAX_PATH 256
char* memstr(char* full_data, int full_data_len, char* substr);
int  file_upload_proc();   // 消息队列(循环体)
int get_file_context(char* begin, char* end, int len, char* filename);   // 处理http, 文本消息处理
int store_data(char* filename, char* fileid);

#endif