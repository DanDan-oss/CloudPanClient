#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <fcgi_stdio.h>
#include "file_upload.h"
#include "fastcommon/logger.h"
#include "common/make_log.h"

int  main_proc()
{
    int buf_size=0;
    char* contentLength = NULL;
    while (FCGI_Accept() >= 0)
    {
        printf("Content-Type:text\r\n\r\n");
        printf("clintip= %s\r\n", getenv("REMOTE_ADDR"));
        printf("request= %s\r\n", getenv("REQUEST_METHOD"));
        printf("query_string= %s\r\n", getenv("QUERY_STRING"));
        contentLength = getenv("CONTENT_LENGTH");

        // 读取消息,存放缓冲区
        if(!contentLength)
            continue;
        buf_size = strtol(contentLength, NULL, 10);     // 获取消息长度,转换成10进制
        if(buf_size <= 0)
        {
            LOG("fdfs_upload", "No data from standard input.<p>");
            continue;
        }
        int ch=0;
        char file_path[FILE_MAX_PATH]={0};
        char* file_data = (char*)malloc(buf_size);        // 根据post数据长度, 创建对应大小的数组
        char *begin, *end, *p;
        end = NULL;
        p = begin = file_data;
        for (size_t i = 0; i < buf_size; i++)
        {
            if( 0 > (ch = getchar()) )
            {
                LOG("fdfs_upload", "Error: Not enough bytes received on standard input<p>");
                break;
            }
            *p = ch;
            p++;
        }
        end = p;

        // 获取文件内容
        get_file_context(begin, end, buf_size, file_path);

        if(file_data)
        {
            free(file_data);
            file_data = NULL;
        }
    }
    return 0;
}

int get_file_context(char* begin, char* end, int len, char* filename)
{
    char* p = NULL;
    char boundary[FILE_MAX_PATH] = {0};     // png文件分界线字符串

    p = (char*)strstr(begin, "\r\n") + strlen("\r\n");   // 指针跳转到http 请求头
    strncpy(boundary, begin, p-begin-strlen("\r\n"));
    len -= (p-begin);
    LOG("fdfs_upload", "boundary: %s", boundary);

    begin = p;
    p = (char*)strstr(p, "\r\n") + + strlen("\r\n");      // 指针跳转到http 请求体
    len -= (p-begin);

    // 读取filename filename="xxxxx"
    char* first_ptr = (char*)strstr(begin, "filename=") + strlen("filename=") +1;  // 跳到第一个"号
    char* last_ptr = strchr(first_ptr, '"');            // 跳到第二"号
    strncpy(filename, first_ptr, last_ptr-first_ptr);
    LOG("fdfs_upload", "<br>filename: %s<br>", filename);

    // 读取 content-type
    begin = p;
    p = (char*)strstr(begin, "\r\n") + strlen("\r\n") + strlen("\r\n");       // 跳过两个\r\n
    len -= (p-begin);

    // 读取文件正文
    begin = p;
    p = memstr(begin, len, boundary);
    if(p)
        p -= strlen("\r\n"); // 减去\r\n长度
    else
        p = end - strlen("\r\n");

    // 将上传文件内容写入文件中
    int fd = open(filename, O_CREAT|O_WRONLY, 0664);
    write(fd, begin, p-begin);
    close(fd);
}

char* memstr(char* full_data, int full_data_len, char* substr)
{
    if (full_data == NULL || full_data_len <= 0 || substr == NULL)
    {
        return NULL;

    }

    if (*substr == '\0')
    {
        return NULL;

    }

    int sublen = strlen(substr);
    char* cur = full_data;
    int last_possible = full_data_len - sublen + 1;
    for (int i = 0; i < last_possible; i++)
    {
        if (*cur == *substr)
        {
            if (memcmp(cur, substr, sublen) == 0)
            {
                // found
                return cur;
            }

        }
        cur++;
    }
    return NULL;
}