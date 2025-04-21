#ifndef  _FILE_UPLOAD_H_
#define  _FILE_UPLOAD_H_

int  file_upload_proc();   // 消息队列(循环体)

void read_configuer();      // 读取mysql配置信息
int recv_save_file(long len, char *user, char *filename, char *md5, long *p_size);
int upload_to_dstorage(char *filename, char *fileid);
int store_fileinfo_to_mysql(char *user, char *filename, char *md5, long size, char *fileid, char *fdfs_file_url);

#endif