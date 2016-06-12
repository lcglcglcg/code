
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>
#include <uuid/uuid.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "ad_system_packet.h"
#include "ad_system_upload_image.h"
#include "gd.h"

typedef struct {

    char boundary[128];
    size_t boundary_size;
    size_t Content_Length;
    size_t max_size;
    char file_name[2048];

    char *buffer;
    size_t buffer_size;
    int width;
    int height;

} ad_system_upload_image_http_t;

typedef union {

    int sockfd;
    void *arg;

} ad_system_upload_image_sockfd_t;

char *ad_system_upload_image_http_value(char *buffer, char *value, int value_size, char *delim, int delim_size)
{

    int size = 0;
    char *str = NULL;
    for (str = buffer, size = 0; *str; size++) {

	if (strncmp(str, delim, delim_size) == 0) {

	    *value = 0;
	    return value;
	}

	if (size >= value_size)
	    return NULL;

	*value++ = *str++;
    }

    return NULL;
}

int ad_system_upload_image_http_parse(char *buffer, size_t buffer_size, ad_system_upload_image_http_t *http)
{

    char *str = NULL;
    if (!(str = strstr(buffer, "Content-Type:")))
	return -1;

    char Content_Type[4096] = {0};
    if (!ad_system_upload_image_http_value(str, Content_Type, 4096, "\r\n", 2))
	return -1;

    if (!(str = (strstr(Content_Type, "boundary="))))
	return -1;

    if (strlen(str) > 64)
	return -1;

    http->boundary_size = sprintf(http->boundary, "--%s", &str[9]);

    if (!(str = strstr(buffer, "Content-Length:")))
	return -1;

    char Content_Length[4096] = {0};
    if (!ad_system_upload_image_http_value(str, Content_Length, 4096, "\r\n", 2))
	return -1;

    if (!(http->Content_Length = atoi(&Content_Length[15])))
	return -1;

    if (!(str = strstr(buffer, "\r\n\r\n")))
	return -1;

    size_t http_head_size = str - buffer + 4;
    http->max_size = http->Content_Length + http_head_size;

    return 0;
}

int ad_system_upload_image_buffer_file_name_parse(char *file_name, ad_system_upload_image_http_t *http)
{

    int i, j;
    for (i = 0, j = 0; file_name[i]; i++) {

	if (file_name[i] == '\r' && file_name[i + 1] == '\n')
	    return 0;

	http->file_name[j++] = file_name[i];
    }

    return -1;
}

int ad_system_upload_image_buffer_group_id_parse(char *group_id_buffer, char *group_id)
{

    int i, j;
    for (i = 0, j = 0; group_id_buffer[i]; i++) {

	if (group_id_buffer[i] == '\r' && group_id_buffer[i + 1] == '\n') {

	    group_id[j] = 0;
	    return 0;
	}

	group_id[j++] = group_id_buffer[i];
    }

    return -1;
}

int ad_system_upload_image_buffer_parse(char *buffer, ad_system_upload_image_http_t *http)
{

    size_t http_head_size = http->max_size - http->Content_Length;
    if (strncmp(&buffer[http_head_size], http->boundary, http->boundary_size))
	return -1;

#if 0
    char *group_id = NULL;
    if (!(group_id = strstr(&buffer[http_head_size], "Content-Disposition: form-data; name=\"GROUP_ID\"\r\n\r\n")))
	return -1;

    if (ad_system_upload_image_buffer_group_id_parse(group_id + 51, http) != 0)
	return -1;
#endif

    char *file_name = NULL;
    if (!(file_name = strstr(&buffer[http_head_size], "Content-Disposition: form-data; name=\"Filename\"\r\n\r\n")))
	return -1;

    if (ad_system_upload_image_buffer_file_name_parse(file_name + 51, http) != 0)
	return -1;

    if (!(http->buffer = strstr(&buffer[http_head_size], "Content-Type: application/octet-stream\r\n\r\n")))
	return -1;

    http->buffer += 42;
    http->buffer_size = http->max_size - (http->buffer - buffer) - 153;

    if (strncmp(&http->buffer[http->buffer_size + 2], http->boundary, http->boundary_size))
	return -1;

#if 0
    //fprintf(stdout, "%s\n", buffer);
    //write(1, buffer, http->max_size);
    //fprintf(stdout, "boundary=%s\n", http->boundary);
    fprintf(stdout, "Content-Length=%zd\n", http->Content_Length);
    fprintf(stdout, "file_name=%s\n", http->file_name);
    fprintf(stdout, "max-size=%zd\n", http->max_size);
    fprintf(stdout, "buffer-size=%zd\n", http->buffer_size);
    //fprintf(stdout, "%s\n", &http->buffer[http->buffer_size]);
    fprintf(stdout, "group_id=[%s]\n", http->group_id);
#endif
    return 0;
}

int ad_system_upload_image_read(int sockfd, char *buffer, ad_system_upload_image_http_t *http)
{

    size_t size = 0;
    size_t buffer_size = 0;

    for(;;){

	size = read(sockfd, &buffer[buffer_size], 4096);
	if (size <= 0)
	    return -1;

	if (buffer_size == 0) {

	    if (ad_system_upload_image_http_parse(buffer, size, http) != 0)
		return -1;
	}

	buffer_size += size;
	if (buffer_size > http->max_size)
	    return -1;

	if (http->max_size == buffer_size)
	    return 0;
    }

    return -1;
}

int ad_system_upload_image_write(int sockfd, ad_system_upload_image_http_t *http)
{

    int i, j;
    char key[64] = {0};
    char path[1024] = {0};
    char buffer[2048] = {0};

    uuid_t uuid;
    uuid_generate(uuid);

    for (i = 0, j = 0; i < 16; i++, j++, j++)
	sprintf(&key[j], "%02x", uuid[i]);

    for (i = 0, j = 0; i < 32; i++) {

	path[j++] = '/';
	path[j++] = key[i];
	sprintf(buffer, "%s%s", UPLOAD_IMG_FILE_PATH, path);

	if (access(buffer, 0))
	    if (mkdir(buffer, S_IRWXU | S_IRWXG | S_IRWXO) != 0)
		return -1;
    }

    sprintf(&path[64], "/temp.jpg");//, http->file_name);
    sprintf(buffer, "%s%s", UPLOAD_IMG_FILE_PATH, path);

    FILE *fp = NULL;
    if (!(fp = fopen(buffer, "wb"))) return -1;
    size_t size = fwrite(http->buffer, 1, http->buffer_size, fp);
    fclose(fp);
    if (size != http->buffer_size)
	return -1;

    if (!ad_system_image_list_add(
		key,
		http->file_name,
		path,
		http->width,
		http->height,
		http->buffer_size))
	return -1;

    size = sprintf(buffer, 
	    "{\"CODE\":\"0\","
	    "\"CREATIVE_IMG_ID\":\"%s\","
	    "\"FILE_NAME\":\"%s\","
	    "\"CREATIVE_IMG_SIZE\":\"%uX%u\","
	    "\"CREATIVE_IMG_SIZE_IMGLENGTH\":\"%zd\","
	    "\"CREATIVE_FILE_PATH\":\"%s\"}",

	    key,
	    http->file_name,
	    http->width,
	    http->height,
	    http->buffer_size,
	    path);

    write(sockfd, buffer, size);
    return 0;
}

int ad_system_upload_image_size_check(ad_system_upload_image_http_t *http)
{

    //760x90 728X90 580X90 468X60 960X60 460X60 640X60 960X90 
    //250x250 200X200 336X280 300X250 360X300 160x600 728X90 120X600 120X240

    if (http->width == 760 && http->height == 90)
	return 0;

    else if (http->width == 728 && http->height == 90)
	return 0;

    else if (http->width == 580 && http->height == 90)
	return 0;
    else if (http->width == 468 && http->height == 60)
	return 0;

    else if (http->width == 960 && http->height == 60)
	return 0;

    else if (http->width == 460 && http->height == 60)
	return 0;

    else if (http->width == 640 && http->height == 60)
	return 0;

    else if (http->width == 960 && http->height == 90)
	return 0;

    else if (http->width == 250 && http->height == 250)
	return 0;

    else if (http->width == 200 && http->height == 200)
	return 0;

    else if (http->width == 336 && http->height == 280)
	return 0;

    else if (http->width == 300 && http->height == 250)
	return 0;

    else if (http->width == 360 && http->height == 300)
	return 0;

    else if (http->width == 160 && http->height == 600)
	return 0;

    else if (http->width == 728 && http->height == 90)
	return 0;

    else if (http->width == 120 && http->height == 600)
	return 0;

    else if (http->width == 120 && http->height == 240)
	return 0;

    return -1;
}

int ad_system_upload_image_check(ad_system_upload_image_http_t *http)
{

    /*************************
      BMP 0x4D42,
      JPG 0xD8FF,
      PNG[4]={0x5089,0x474E,0x0A0D,0x0A1A},
      GIF[3]={0x4947,0x3846,0x6139};
     *************************/

    unsigned short img_type = *(unsigned short *)http->buffer;

    if (img_type == 0xd8ff) {

	gdImagePtr im = NULL;
	if ((im = gdImageCreateFromJpegPtr(http->buffer_size, http->buffer))) {

	    http->height = im->sy;
	    http->width = im->sx;
	    //fprintf(stdout, "JPG width=%u height=%u\n", http->width, http->height);
	    gdImageDestroy(im);
	    return 0;
	}

    } else if ((strncmp(http->buffer, "GIF87a", 6) == 0) || (strncmp(http->buffer, "GIF89a", 6) == 0)) {

	gdImagePtr im = NULL;
	if ((im = gdImageCreateFromGifPtr(http->buffer_size, http->buffer))) {

	    http->height = im->sy;
	    http->width = im->sx;
	    //fprintf(stdout, "GIF width=%u height=%u\n", http->width, http->height);
	    gdImageDestroy(im);
	    return 0;
	}
    }

    //fprintf(stdout, "IMG ERR\n");
    return -1;
}

void *ad_system_upload_image_thread(void *arg)
{

    pthread_detach(pthread_self());

    ad_system_upload_image_sockfd_t temp;
    temp.arg = arg;
    int sockfd = temp.sockfd;

    char buffer[1048576] = {0};
    ad_system_upload_image_http_t http;
    memset(&http, 0, sizeof(ad_system_upload_image_http_t));

    write(sockfd, HTTP_HEAD, strlen(HTTP_HEAD));
    if (ad_system_upload_image_read(sockfd, buffer, &http) == 0) {

	if (ad_system_upload_image_buffer_parse(buffer, &http) == 0) {

	    if (ad_system_upload_image_check(&http) == 0) {

		if (ad_system_upload_image_size_check(&http) == 0) {

		    if (ad_system_upload_image_write(sockfd, &http) == 0) {

			close(sockfd);
			pthread_exit(NULL);
		    }

		} else {

		    size_t size = sprintf(buffer, "{\"CODE\":\"%u\"}", PACKET_CODE_IMG_UPLOAD_FILE_SIZE_ERR);
		    write(sockfd, buffer, size);

		    close(sockfd);
		    pthread_exit(NULL);
		}
	    }
	}
    }

    size_t size = sprintf(buffer, "{\"CODE\":\"%u\"}", PACKET_CODE_IMG_UPLOAD_FILE_ERR);
    write(sockfd, buffer, size);

    close(sockfd);
    pthread_exit(NULL);
}

int ad_system_upload_image_run()
{

    int listenfd = -1;
    if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	return -1;

    int	flag = 0x01;
    if (setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(int)) == -1) {

	close(listenfd);
	return -1;
    }

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(5003);
    if (bind(listenfd, (struct sockaddr *)&addr, sizeof(addr)) == -1) {

	close(listenfd);
	return -1;
    }

    if (listen(listenfd, 1000) == -1) {

	close(listenfd);
	return -1;
    }

    for (;;) {

	ad_system_upload_image_sockfd_t temp;
	if ((temp.sockfd = accept(listenfd, NULL, NULL)) > 0) {

	    pthread_t tid;
	    pthread_create(&tid, NULL, ad_system_upload_image_thread, temp.arg);
	}
    }

    return 0;
}



