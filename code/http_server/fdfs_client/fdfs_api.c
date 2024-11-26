#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "fdfs_client.h"
#include "fastcommon/logger.h"
#include "common/make_log.h"

int fdfs_upload_file(const char* conf_filename, const char* local_filename, char* file_id)
{
	char group_name[FDFS_GROUP_NAME_MAX_LEN + 1];
	ConnectionInfo* pTrackerServer = NULL;
	int result = 0;
	int store_path_index = 0;
	ConnectionInfo storageServer = {0};
	// 通过客户端文件初始化数据
	if ((result=fdfs_client_init(conf_filename)) != 0)
	{
		return result;
	}

	// 链接追踪器
	pTrackerServer = tracker_get_connection();
	if (pTrackerServer == NULL)
	{
		printf("et tracker_get_connection fail\n");
		LOG("fdfs_client", " get tracker_get_connection fail ");
		fdfs_client_destroy();
		return errno != 0 ? errno : ECONNREFUSED;
	}

	*group_name = '\0';

	// 获取追踪器
	if ((result=tracker_query_storage_store(pTrackerServer, \
	                &storageServer, group_name, &store_path_index)) != 0)
	{
		fdfs_client_destroy();
		LOG("fdfs_client", "tracker_query_storage fail, error no: %d, error info: %s", \
			result, STRERROR(result));
		return result;
	}

	result = storage_upload_by_filename1(pTrackerServer, \
			&storageServer, store_path_index, \
			local_filename, NULL, \
			NULL, 0, group_name, file_id);
	if (result == 0)
	{
		LOG("fdfs_client", " %s", file_id);
	}
	else
	{
		LOG("fdfs_client", "upload file fail, error no: %d, error info: %s", result, STRERROR(result));
	}

	tracker_close_connection_ex(pTrackerServer, true);
	fdfs_client_destroy();

	return result;
}
