
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <hiredis/hiredis.h>
#include <scws.h>
#include <iconv.h>

int demo_iconv(char *src, size_t src_len, char *dest, size_t dest_len)
{

    size_t size = dest_len;
    iconv_t cd = iconv_open("GBK", "UTF-8");
    if (cd == (iconv_t)(-1)) return -1;

    if (iconv(cd, &src, &src_len, &dest, &dest_len) == -1) {

	iconv_close(cd);
	return -1;
    }

    iconv_close(cd);
    return size - dest_len;
}

int demo_token(char *str, int len)
{

    short high, low;
    unsigned int code, i;
    for(i = 0; i < len; i++) {

	if(str[i] >= 0) {

	    return -1;

	} else {

	    high = (short)(str[i] + 256);
	    low = (short)(str[i + 1] + 256);
	    code = high * 256 + low;

	    if(!((code >= 0xB0A1 && code <= 0xF7FE) || (code >= 0x8140 && code <= 0xA0FE) || (code >= 0xAA40 && code <= 0xFEA0))) {

		return -1;
	    }
	}
    }

    return 0;
}

void demo_scws_parse(scws_t s, redisContext *conn, char *buffer, int size)
{

    char dest[4096];
    scws_res_t res, cur;
    scws_send_text(s, buffer, size);
    while ((res = cur = scws_get_result(s))) {

	while (cur != NULL) {

	    if (cur->len >= 6 && cur->len < 1024) {

		int dest_len = demo_iconv(buffer + cur->off, cur->len, dest, 4096);
		if (dest_len > 0) {

		    if (demo_token(dest, dest_len) == 0) {

			// fprintf(stdout, "%.*s\n", cur->len, buffer + cur->off);
			redisReply *reply = redisCommand(conn, "set %b 0", buffer + cur->off, cur->len);
			if (reply) freeReplyObject(reply);
		    }
		}
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


