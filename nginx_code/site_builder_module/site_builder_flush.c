
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include "site_builder_flush.h"

#define SITE_BUILDER_FLUSH_BUF_SIZE	4096
#define SITE_BUILDER_FLUSH_SET_HEAD	"3\nset\n"
#define SITE_BUILDER_FLUSH_SET_HEAD_LEN	6
#define SITE_BUILDER_FLUSH_GET_HEAD	"3\nget\n"
#define SITE_BUILDER_FLUSH_GET_HEAD_LEN	6

typedef struct {

    int head_size;
    int value_size;
    int value_len_size;
    int buffer_size;

    char *value;
    char *pos;

} site_builder_flush_get_value_info_t;

typedef struct {

    char *buffer;
    int size;

} site_builder_flush_value_t;

ngx_pool_t *site_builder_flush_pool = NULL;

/*************/
/* flush api */
/*************/
int site_builder_flush_network_check(int sockfd)
{

    int error = -1;
    socklen_t len = sizeof(int);
    if (getsockopt(sockfd, SOL_SOCKET, SO_ERROR, &error, &len) == -1)
	return -1;

    return error;
}

int site_builder_flush_network_connect()
{

    struct sockaddr_un addr;
    bzero(&addr, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strcpy(addr.sun_path, "/tmp/ssdb.sock");
    int sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sockfd < 0) return -1;

    if (connect(sockfd, (struct sockaddr *)&addr, sizeof(addr))) {

	close(sockfd);
	return -1;
    }

    if (site_builder_flush_network_check(sockfd)) {

	close(sockfd);
	return -1;
    }

    return sockfd;
}

int site_builder_flush_set(int sockfd, const char *key, int key_size, const char *value, int value_size)
{

    int size = 0;
    char buffer[32];

    if (write(sockfd, SITE_BUILDER_FLUSH_SET_HEAD, SITE_BUILDER_FLUSH_SET_HEAD_LEN) != SITE_BUILDER_FLUSH_SET_HEAD_LEN)
	return -1;

    size = sprintf(buffer, "%d\n", key_size);
    if (write(sockfd, buffer, size) != size)
	return -1;

    if (write(sockfd, key, key_size) != key_size)
	return -1;

    size = sprintf(buffer, "\n%d\n", value_size);
    if (write(sockfd, buffer, size) != size)
	return -1;

    if (write(sockfd, value, value_size) != value_size)
	return -1;

    if (write(sockfd, "\n\n", 2) != 2)
	return -1;

    return 0;
}

int site_builder_flush_get_info_parse(char *buffer, int size, site_builder_flush_get_value_info_t *info)
{

    if (size == 13 && strncmp(buffer, "9\nnot_found\n\n", 13) == 0)
	return -1;

    info->head_size = 5;
    if (strncmp(buffer, "2\nok\n", info->head_size))
	return -2;

    info->value_size = atoi(&buffer[info->head_size]);
    if (info->value_size <= 0)
	return -3;

    char *str = strstr(&buffer[info->head_size], "\n");
    info->value_len_size = str - &buffer[info->head_size];
    if (info->value_len_size <= 0 || info->value_len_size > 32)
	return -4;

    // head_size + value_len_size + \n + info->value_size + \n + \n
    info->buffer_size = info->head_size + info->value_len_size + 1 + info->value_size + 2;
    info->value = ngx_pcalloc(site_builder_flush_pool, info->value_size + 1);
    if (!info->value) 
	return -5;

    // fprintf(stdout, "size=%d\n", size);
    // fprintf(stdout, "value_size=%d\n", info->value_size);
    // fprintf(stdout, "value_len_size=%d\n", info->value_len_size);
    // fprintf(stdout, "buffer_size=%d\n", info->buffer_size);
    info->pos = info->value;
    return 0;
}

int site_builder_flush_get_value(int sockfd, site_builder_flush_value_t *value)
{

    int ret = -1;

    char *src_buffer = NULL;
    int src_buffer_size = 0;

    char buffer[SITE_BUILDER_FLUSH_BUF_SIZE];
    int buffer_size = 0;

    site_builder_flush_get_value_info_t info;
    memset(&info, 0, sizeof(site_builder_flush_get_value_info_t));

    for (;;) {

	int size = read(sockfd, buffer, SITE_BUILDER_FLUSH_BUF_SIZE);
	if (size <= 0) return -6;

	if (buffer_size == 0) {

	    ret = site_builder_flush_get_info_parse(buffer, size, &info);
	    if (ret) return ret;

	    src_buffer = buffer + info.head_size + info.value_len_size + 1;
	    src_buffer_size = size - info.head_size - info.value_len_size - 1;

	} else {

	    src_buffer = buffer;
	    src_buffer_size = size;
	}

	buffer_size += size;
	// fprintf(stdout, "==>%d::%d\n", info.buffer_size, buffer_size);
	// fprintf(stdout, "[%d][%.*s]\n", src_buffer_size, src_buffer_size, src_buffer);

	int copy_size = info.pos - info.value + src_buffer_size;
	if (copy_size > info.value_size) {

	    src_buffer_size = src_buffer_size - (copy_size - info.value_size);
	    // fprintf(stdout, "%d:%d\n", copy_size, src_buffer_size);
	}

	if (info.buffer_size == buffer_size) {

	    // fprintf(stdout, "[e_size][%d]\n", src_buffer_size);
	    memcpy(info.pos, src_buffer, src_buffer_size);
	    info.value[info.value_size] = 0;

	    value->buffer = info.value;
	    value->size = info.value_size;
	    // fprintf(stdout, "[value][%s]\n", info.value);
	    return 0;

	} else if (buffer_size > info.buffer_size) {

	    // fprintf(stdout, "err\n");
	    return -7;

	} else {

	    memcpy(info.pos, src_buffer, src_buffer_size);
	    info.pos += src_buffer_size;
	}
    }

    return 0;
}

int site_builder_flush_get(int sockfd, const char *key, int key_size, site_builder_flush_value_t *value)
{

    int size = 0;
    char buffer[32];

    if (write(sockfd, SITE_BUILDER_FLUSH_GET_HEAD, SITE_BUILDER_FLUSH_GET_HEAD_LEN) != SITE_BUILDER_FLUSH_GET_HEAD_LEN)
	return -8;

    size = sprintf(buffer, "%d\n", key_size);
    if (write(sockfd, buffer, size) != size)
	return -8;

    if (write(sockfd, key, key_size) != key_size)
	return -8;

    if (write(sockfd, "\n\n", 2) != 2)
	return -8;

    value->size = 0;
    value->buffer = NULL;
    return site_builder_flush_get_value(sockfd, value);
}

/**************/
/* flush call */
/**************/

int site_builder_flush_sockfd_r = -1;
int site_builder_flush_sockfd_w = -1;
int site_builder_flush_init(ngx_pool_t *pool)
{

    site_builder_flush_sockfd_r = site_builder_flush_network_connect();
    site_builder_flush_sockfd_w = site_builder_flush_network_connect();
    if (site_builder_flush_sockfd_r < 0 || site_builder_flush_sockfd_w < 0)
	return -1;

    site_builder_flush_pool = pool;
    return 0;
}

int site_builder_flush_check()
{

    int sockfd = site_builder_flush_network_connect();
    if (sockfd == -1) return -1;

    close(sockfd);
    return 0;
}

int site_builder_flush_get_conn_r()
{

    if (site_builder_flush_sockfd_r == -1 || site_builder_flush_network_check(site_builder_flush_sockfd_r)) {

	site_builder_flush_sockfd_r = site_builder_flush_network_connect();
	if (site_builder_flush_sockfd_r < 0)
	    return -1;
    }

    return site_builder_flush_sockfd_r;
}

int site_builder_flush_get_conn_w()
{

    if (site_builder_flush_sockfd_w == -1 || site_builder_flush_network_check(site_builder_flush_sockfd_w)) {

	site_builder_flush_sockfd_w = site_builder_flush_network_connect();
	if (site_builder_flush_sockfd_w < 0)
	    return -1;
    }

    return site_builder_flush_sockfd_w;
}

int site_builder_flush_load(const char *key, int key_size, char **buffer)
{
    site_builder_flush_value_t value;
    int sockfd = site_builder_flush_get_conn_r();
    if (sockfd < 0) return -1;

    int ret = site_builder_flush_get(sockfd, key, key_size, &value);
    // fprintf(stdout, "[find][%s][%d]\n", key, ret);
    if (ret) {

	if (ret == -1) return -1;

	close(sockfd);
	site_builder_flush_sockfd_r = -1;
	return -1;
    }
    // fprintf(stdout, "[find]site_builder_flush_load::ok\n");

    *buffer = value.buffer;
    return value.size;
}

int site_builder_flush_str(const char *key, int key_size, const char *buffer, int buffer_size)
{
    int sockfd = site_builder_flush_get_conn_w();
    if (sockfd < 0) return -1;

    // fprintf(stdout, "[save][%s][%s]\n", key, buffer);
    site_builder_flush_set(sockfd, key, key_size, buffer, buffer_size);
    // fprintf(stdout, "[save]site_builder_flush_save::ok\n");
    return 0;
}

int site_builder_flush_list(const char *key, int key_size, ngx_queue_t *queue)
{

    int sockfd = site_builder_flush_get_conn_w();
    if (sockfd < 0) return -1;

    // fprintf(stdout, "[save][%s][", key);

    int value_size = 0;
    ngx_queue_t *p = NULL;
    for (p = ngx_queue_head(queue);
	    p != ngx_queue_sentinel(queue);
	    p = ngx_queue_next(p)) {

	site_builder_element_str_t *node = (site_builder_element_str_t *)p;
	value_size += node->buffer_size + 1;
	// fprintf(stdout, "%s\n", node->buffer);
    }

    // fprintf(stdout, "]\n");

    int size = 0;
    char buffer[32];

    if (write(sockfd, SITE_BUILDER_FLUSH_SET_HEAD, SITE_BUILDER_FLUSH_SET_HEAD_LEN) != SITE_BUILDER_FLUSH_SET_HEAD_LEN)
	return -1;

    size = sprintf(buffer, "%d\n", key_size);
    if (write(sockfd, buffer, size) != size)
	return -1;

    if (write(sockfd, key, key_size) != key_size)
	return -1;

    size = sprintf(buffer, "\n%d\n", value_size);
    if (write(sockfd, buffer, size) != size)
	return -1;

    for (p = ngx_queue_head(queue);
	    p != ngx_queue_sentinel(queue);
	    p = ngx_queue_next(p)) {

	site_builder_element_str_t *node = (site_builder_element_str_t *)p;

	if (write(sockfd, node->buffer, node->buffer_size) != node->buffer_size)
	    return -1;

	if (write(sockfd, "\n", 1) != 1)
	    return -1;
    }

    if (write(sockfd, "\n\n", 2) != 2)
	return -1;

    return 0;
}

int site_builder_flush_list_key_value(const char *key, int key_size, ngx_queue_t *queue)
{

    int sockfd = site_builder_flush_get_conn_w();
    if (sockfd < 0) return -1;

    int value_size = 0;
    ngx_queue_t *p = NULL;
    for (p = ngx_queue_head(queue);
	    p != ngx_queue_sentinel(queue);
	    p = ngx_queue_next(p)) {

	site_builder_element_key_value_t *node = (site_builder_element_key_value_t *)p;
	value_size += (node->key_size + node->value_size + 2);
	// fprintf(stdout, "[save][%s][%s]\t\t\t\t[%s]\n", key, node->key, node->value);
    }

    int size = 0;
    char buffer[32];

    if (write(sockfd, SITE_BUILDER_FLUSH_SET_HEAD, SITE_BUILDER_FLUSH_SET_HEAD_LEN) != SITE_BUILDER_FLUSH_SET_HEAD_LEN)
	return -1;

    size = sprintf(buffer, "%d\n", key_size);
    if (write(sockfd, buffer, size) != size)
	return -1;

    if (write(sockfd, key, key_size) != key_size)
	return -1;

    size = sprintf(buffer, "\n%d\n", value_size);
    if (write(sockfd, buffer, size) != size)
	return -1;

    for (p = ngx_queue_head(queue);
	    p != ngx_queue_sentinel(queue);
	    p = ngx_queue_next(p)) {

	site_builder_element_key_value_t *node = (site_builder_element_key_value_t *)p;

	if (write(sockfd, node->key, node->key_size) != node->key_size)
	    return -1;

	if (write(sockfd, "\t", 1) != 1)
	    return -1;

	if (write(sockfd, node->value, node->value_size) != node->value_size)
	    return -1;

	if (write(sockfd, "\n", 1) != 1)
	    return -1;
    }

    if (write(sockfd, "\n\n", 2) != 2)
	return -1;

    return 0;
}



