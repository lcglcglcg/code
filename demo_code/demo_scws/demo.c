
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <hiredis/hiredis.h>
#include <scws.h>

void demo_scws_parse(scws_t s, redisContext *conn, char *buffer, int size)
{

    scws_res_t res, cur;
    scws_send_text(s, buffer, size);
    while ((res = cur = scws_get_result(s))) {

	while (cur != NULL) {

	    if (cur->len >= 6) {

		// fprintf(stdout, "%.*s\n", cur->len, buffer + cur->off);
		redisReply *reply = redisCommand(conn, "set %b 0", buffer + cur->off, cur->len);
		if (reply) freeReplyObject(reply);
	    }

	    cur = cur->next;
	}

	scws_free_result(res);
    }
}

void demo_load_file(scws_t s, redisContext *conn, char *path)
{

    char buffer[40960];
    FILE *fp = fopen(path, "r");
    while(fgets(buffer, 40960, fp)) {

	demo_scws_parse(s, conn, buffer, strlen(buffer));
    }
}

int demo_scan_dir(scws_t s, redisContext *conn, char *path)
{

    DIR *dir = NULL;
    struct dirent *dir_info = NULL;
    char _path[1024];

    if (!(dir = opendir(path)))
	return -1;

    while((dir_info = readdir(dir))) {

	if((strcmp(dir_info->d_name,".") == 0))
	    continue;

	if ((strcmp(dir_info->d_name,"..")==0))
	    continue;

	sprintf(_path, "%s/%s", path, dir_info->d_name);
	if (dir_info->d_type == 4) { //dir

	    demo_scan_dir(s, conn, _path);

	} else if (dir_info->d_type == 8) { //file

	    demo_load_file(s, conn, _path);
	}
    }

    closedir(dir);
    return 0;
}

int main()
{

    scws_t s = scws_new();
    scws_set_charset(s, "utf-8");
    scws_set_dict(s, "dict.utf8.xdb", SCWS_XDICT_XDB);

    redisContext *conn = redisConnect("127.0.0.1", 6379);

    demo_scan_dir(s, conn, "/back/make/demo_scws/keyword");

    scws_free(s);
    redisFree(conn);
    return 0;
}



