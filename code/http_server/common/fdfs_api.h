#ifndef _FDFS_API_H
#define _FDFS_API_H

int fdfs_upload_file(const char* conf_file_path, const char* upload_file, char* file_id);
int fdfs_upload_file_ex(const char* local_filename, char* file_id);
#endif