#include <stdio.h>
#include "fdfs_api.h"
#include "make_log.h"

int main(int argc, char* argv[])
{
    char fileid[1024] = {0};
    fdfs_upload_file("/etc/fdfs/client.conf", argv[1], (char*)fileid);
    LOG("fdfs_client", "fileID==%s", fileid);
}
