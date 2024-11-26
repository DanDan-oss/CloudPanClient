#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "common/make_log.h"

void process_callback(const char* file_name);

int main(int argc, char** argv)
{
    int pipe_fd[2] = {0};
    pid_t ret_pid = 0;
    char fileid[1024] = {0};


    if(-1 == pipe(pipe_fd))
    {
        LOG("fdfs_client", "创建匿名管道失败");
        return 0;
    }

    ret_pid = fork();
    if(0 == ret_pid)
    {
        process_callback(argv[1]);
        exit(0);
    }
    ret_pid = wait(NULL);
    close(pipe_fd[1]);
    read(pipe_fd[1], fileid, sizeof(fileid));
    
}

void process_callback(const char* file_name)
{
    close(pipe_fd[0]);
    execlp("fdfs_upload_file", "fdfs_upload_file", "/etc/fdfs/client.conf %s", file_name);
    write(pipe_fd[1], " ", 1);
}