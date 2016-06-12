
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
    ConnectionInfo *pStorageServer;

    char group_name[FDFS_GROUP_NAME_MAX_LEN + 1];
    *group_name = '\0';
    char szPortPart[16];
    char file_id[128];


    char *local_filename = "/make/demo_fastdfs/main.c";

    int store_path_index = 0;

    if ((result=tracker_query_storage_store(pTrackerServer,
		    &storageServer, group_name, &store_path_index)) != 0)
    {
	fdfs_client_destroy();
	return result;
    }

    printf("group_name=%s, ip_addr=%s, port=%d\n",
	    group_name, storageServer.ip_addr,
	    storageServer.port);

    if ((pStorageServer=tracker_connect_server(&storageServer,
		    &result)) == NULL)
    {
	fdfs_client_destroy();
	return result;
    }

    const char *file_ext_name = fdfs_get_file_ext_name(local_filename);
    fprintf(stdout, "file_ext_name=%s\n", file_ext_name);
    *group_name = '\0';

    int upload_type = 2;
    char remote_filename[256];
    int64_t file_size;

    fprintf(stdout, "%d:%d:%d\n", FDFS_UPLOAD_BY_FILE, FDFS_UPLOAD_BY_BUFF, upload_type);

    if (upload_type == FDFS_UPLOAD_BY_FILE)
    {
	result = storage_upload_by_filename(pTrackerServer,
		pStorageServer, store_path_index,
		local_filename, file_ext_name,
		NULL, 0,
		group_name, remote_filename);

	printf("storage_upload_by_filename\n");
    }
    else if (upload_type == FDFS_UPLOAD_BY_BUFF)
    {
	char *file_content;
	if ((result=getFileContent(local_filename,
			&file_content, &file_size)) == 0)
	{
	    result = storage_upload_by_filebuff(pTrackerServer,
		    pStorageServer, store_path_index,
		    file_content, file_size, file_ext_name,
		    NULL, 0,
		    group_name, remote_filename);
	    free(file_content);
	}

	printf("storage_upload_by_filebuff\n");
    }

    if (result != 0)
    {
	printf("upload file fail, "
		"error no: %d, error info: %s\n",
		result, STRERROR(result));
	tracker_disconnect_server_ex(pStorageServer, true);
	fdfs_client_destroy();
	return result;
    }


    char file_url[256];
    sprintf(file_id, "%s/%s", group_name, remote_filename);

    fprintf(stdout, "file_id=%s\n", file_id);
    printf("group_name=%s, remote_filename=%s\n",
	    group_name, remote_filename);

    return 0;
}


