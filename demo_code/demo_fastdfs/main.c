
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "fdfs_client.h"
#include "fdfs_global.h"
#include "base64.h"
#include "sockopt.h"
#include "logger.h"
#include "fdfs_http_shared.h"

int main()
{

    int result = fdfs_client_init("/etc/fdfs/client.conf");

    ConnectionInfo *pTrackerServer = tracker_get_connection();

    ConnectionInfo storageServer;
    char group_name[FDFS_GROUP_NAME_MAX_LEN + 1] = {0};
    int store_path_index = 0;
    if ((result=tracker_query_storage_store(pTrackerServer,
		    &storageServer, group_name, &store_path_index)) != 0)
    {
	fdfs_client_destroy();
	return result;
    }

    printf("group_name=%s ip_addr=%s port=%d\n",
	    group_name, 
	    storageServer.ip_addr,
	    storageServer.port);

    ConnectionInfo *pStorageServer;
    if ((pStorageServer=tracker_connect_server(&storageServer,
		    &result)) == NULL)
    {
	fdfs_client_destroy();
	return result;
    }

    char remote_filename[256];
    int i;
    for (i = 0; i < 1000; i++) {

#if 1
	char *local_filename = "/make/demo_fastdfs/main.c";
	result = storage_upload_by_filename(pTrackerServer,
		pStorageServer, store_path_index,
		local_filename, NULL,
		NULL, 0,
		group_name, remote_filename);
#else
	char *buffer = "abc";
	int size = 3;
	result = storage_upload_by_filebuff(pTrackerServer,
		pStorageServer, store_path_index,
		buffer, size, NULL,
		NULL, 0,
		group_name, remote_filename);
#endif
	if (result != 0) {

	    printf("upload error: %d, error info: %s\n", result, STRERROR(result));
	    tracker_disconnect_server_ex(pStorageServer, true);
	    fdfs_client_destroy();
	    return result;
	}

	printf("%s\n", remote_filename);
    }

    return 0;
}



