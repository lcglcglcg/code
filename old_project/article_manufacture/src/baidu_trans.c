
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <curl/curl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <libxml/parser.h>

#include "lg_json.h"
#include "lg_string.h"
#include "lg_redis.h"
// #include "lg_rabbitmq.h"
#include "lg_thread_pool.h"
#include "lg_config_file.h"
#include "lg_filter_keyword.h"

#define BAIDU_TRANS_POST_URL "http://fanyi.baidu.com/v2transapi"
#define BAIDU_TRANS_KEY_SIZE 512
#define BAIDU_TRANS_BUF_SIZE 10000000

typedef struct {

    char *buffer;
    size_t size;

} lg_str_t;

typedef struct {

    char *language;
    char queue_name[BAIDU_TRANS_KEY_SIZE];
    char buffer[BAIDU_TRANS_BUF_SIZE];
    char temp[BAIDU_TRANS_BUF_SIZE];

    char *err;

} task_t;

char *baidu_trans_language_list[] = {

    "en",
    "ara",
    "est",
    "bul",
    "pl",
    "dan",
    "de",
    "ru",
    "fra",
    "fin",
    "kor",
    "nl",
    "cs",
    "rom",
    "pt",
    "jp",
    "swe",
    "slo",
    "th",
    "spa",
    "it",
    NULL
};

const char *local_addr = NULL;
const char *rabbitmq_host = NULL;
const char *rabbitmq_user = NULL;
const char *rabbitmq_pass = NULL;
lg_list_t filter_keyword_head;

int lg_network_not_time_wait(int sockfd)
{

    struct linger opt;
    opt.l_onoff = 0x01;
    opt.l_linger = 0x00;
    return setsockopt(sockfd, SOL_SOCKET, SO_LINGER, &opt, sizeof(struct linger));
}

curl_socket_t baidu_trans_set_socket(void *clientp, curlsocktype purpose, struct curl_sockaddr *addr)
{

    int sockfd = socket(addr->family, addr->socktype, addr->protocol);
    if (sockfd < 0)  {

	return sockfd;
    }

    // lg_network_not_time_wait(sockfd);
    struct sockaddr_in addr_loacl;
    bzero(&addr_loacl, sizeof(struct sockaddr_in));
    addr_loacl.sin_family = AF_INET;
    inet_aton(local_addr, &addr_loacl.sin_addr);
    bind(sockfd, (struct sockaddr *)&addr_loacl, sizeof(struct sockaddr_in));
    return sockfd;
}

int baidu_trans_recv(char *buffer, int size, int nmemb, lg_str_t *dest)
{

    int _size = size * nmemb;
    memcpy(dest->buffer + dest->size, buffer, _size);
    dest->size += _size;
    dest->buffer[dest->size] = 0;
    return _size;
}

int baidu_trans_post(char *send_buffer, lg_str_t *recv_buffer)
{

    CURL *curl = curl_easy_init();
    if(curl == NULL)
	return -1;

    curl_easy_setopt(curl, CURLOPT_POST, 1);
    curl_easy_setopt(curl, CURLOPT_URL, BAIDU_TRANS_POST_URL);
    curl_easy_setopt(curl, CURLOPT_OPENSOCKETFUNCTION, baidu_trans_set_socket);

    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, send_buffer);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, baidu_trans_recv);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, recv_buffer);

    CURLcode ret = curl_easy_perform(curl);
    curl_easy_cleanup(curl);
    return ret;
}

char *baidu_trans_run(task_t *task, char *language_from, char *language_to)
{

    task->temp[0] = 0;
    lg_str_t dest = {task->temp, 0};

    task->buffer[5000] = 0;
    lg_string_url_encode(task->temp, task->buffer, strlen(task->buffer));

    sprintf(task->buffer, "from=%s&to=%s&transtype=trans&simple_means_flag=3&query=%s", language_from, language_to, task->temp);
    if (baidu_trans_post(task->buffer, &dest) == -1)
	return NULL;

    lg_json_t *root_obj = lg_json_parse(task->temp);
    lg_json_t *trans_result_obj = lg_json_get_object(root_obj, "trans_result");
    lg_json_t *data_obj = lg_json_get_object(trans_result_obj, "data");

    int arr_length = json_object_get_array_length(data_obj);
    if (!arr_length) return NULL;

    int i, size = 0;
    for (i = 0; i < arr_length; i++) {

	lg_json_t *data_array_obj = lg_json_get_array(data_obj, i);
	lg_json_t *dst_obj = lg_json_get_object(data_array_obj, "dst");

	const char *buffer = lg_json_value_to_string(dst_obj);
	if (!buffer) {

	    lg_json_free(root_obj);
	    return NULL;
	}

	int temp_size = strlen(buffer);
	memcpy(&task->buffer[size], buffer, temp_size);
	size += temp_size;
	task->buffer[size++] = '\n';
    }

    if (size == 0) {

	lg_json_free(root_obj);
	return NULL;
    }

    task->buffer[size] = 0;
    lg_json_free(root_obj);
    return task->buffer;
}

void task_rabbitmq_send(char *queue_name, char *buffer, char *temp)
{

    struct tm tm;
    time_t t = time(NULL);
    localtime_r(&t, &tm);

#if 0
    int ret = -1;
    amqp_connection_state_t conn = lg_rabbitmq_conn(rabbitmq_host, rabbitmq_user, rabbitmq_pass);
    if (conn) {

	amqp_queue_declare(conn, 1, amqp_cstring_bytes(queue_name), 0, 0, 0, 1, amqp_empty_table);
	ret = amqp_basic_publish(conn,
		1,
		amqp_empty_bytes,
		amqp_cstring_bytes(queue_name),
		0,
		0,
		NULL,
		amqp_cstring_bytes(buffer));

	lg_rabbitmq_close(conn);
	// amqp_error_string2(ret));
    }
#else
    
    lg_list_t *filter_keyword = NULL;
    lg_list_for_each (filter_keyword, &filter_keyword_head) {

	lg_string_remove_keyword(buffer, filter_keyword->str, filter_keyword->size);
    }
    
    int size = strlen(buffer);
    size = lg_string_url_encode(temp, buffer, size);
    int ret = lg_redis_send(queue_name, temp, size);
#endif

    if (ret) {

	fprintf(stdout, "[%04d-%02d-%02d %02d:%02d:%02d][%s][%s]\n",
		tm.tm_year + 1900,
		tm.tm_mon + 1,
		tm.tm_mday,
		tm.tm_hour,
		tm.tm_min,
		tm.tm_sec,
		queue_name,
		"insert redis:: err");
    } else {

	fprintf(stdout, "[%04d-%02d-%02d %02d:%02d:%02d][%s][success]\n",
		tm.tm_year + 1900,
		tm.tm_mon + 1,
		tm.tm_mday,
		tm.tm_hour,
		tm.tm_min,
		tm.tm_sec,
		queue_name);
    }

    fflush(stdout);
}

int baidu_trans(task_t *task)
{

#if 1
    for (;;) {

	if (!baidu_trans_run(task, "zh", task->language)) {

	    if (strncmp(task->temp, "{\"error\":9,", 11) == 0)
		continue;

	    fprintf(stdout, "-->%s\n", task->temp);
	    return -1;
	}

	if (!baidu_trans_run(task, task->language, "zh")) {

	    if (strncmp(task->temp, "{\"error\":9,", 11) == 0)
		continue;

	    fprintf(stdout, "-->%s\n", task->temp);
	    return -1;
	}

	break;
    }
#endif

    task_rabbitmq_send(task->queue_name, task->buffer, task->temp);
    return 0;
}

void *task_thread(lg_thread_node_t *thread_info)
{

    for (;;) {

	if (thread_info->status == LG_THREAD_POOL_RUN) {

	    baidu_trans(thread_info->arg);
	    thread_info->status = LG_THREAD_POOL_FINISH;
	}

	usleep(1000);
    }

    pthread_exit(NULL);
}

void task_buffer_copy(lg_thread_pool_t *pool, char *queue_name, char *buffer)
{

    lg_list_t *p = NULL;
    lg_list_for_each (p, &pool->head) {

	lg_thread_node_t *node = (lg_thread_node_t *)p;
	task_t *task = node->arg;

	lg_list_t *filter_keyword = NULL;
	lg_list_for_each (filter_keyword, &filter_keyword_head) {

	    lg_string_remove_keyword(buffer, filter_keyword->str, filter_keyword->size);
	}

	strcpy(task->queue_name, queue_name);
	strcpy(task->buffer, buffer);
	node->status = LG_THREAD_POOL_RUN;
    }
}

void task_wait(lg_thread_pool_t *pool)
{

    lg_list_t *p = NULL;
    lg_list_for_each (p, &pool->head) {

	lg_thread_node_t *node = (lg_thread_node_t *)p;

	for (;;) {

	    if (node->status == LG_THREAD_POOL_FINISH) {

		node->status = LG_THREAD_POOL_IDLE;
		break;
	    }

	    usleep(1000);
	}
    }
}

int task_rabbitmq_recv_function_print(int ret, char *buffer)
{
    struct tm tm;
    time_t t = time(NULL);
    localtime_r(&t, &tm);
    fprintf(stdout, "[%04d-%02d-%02d %02d:%02d:%02d][%d][%s]\n",
	    tm.tm_year + 1900,
	    tm.tm_mon + 1,
	    tm.tm_mday,
	    tm.tm_hour,
	    tm.tm_min,
	    tm.tm_sec,
	    ret,
	    buffer);

    fflush(stdout);
    return 0;
}

int task_rabbitmq_recv_function(char *buffer, void *arg)
{

    fprintf(stdout, "[recv-mess]%s\n", buffer);
    int size = strlen(buffer);
    lg_string_url_decode(buffer, buffer, size);

    xmlDocPtr doc = xmlParseMemory(buffer, size);
    if(doc == NULL) return task_rabbitmq_recv_function_print(-1, buffer);

    xmlNodePtr cur = xmlDocGetRootElement(doc);
    if(cur == NULL) {

	xmlFreeDoc(doc);
	return task_rabbitmq_recv_function_print(-2, buffer);
    }

    char *task_queue = NULL;
    char *task_content = NULL;
    for (cur = cur->children; cur != NULL; cur = cur->next) {

	if (strcmp((char *)cur->name, "task_queue") == 0) {

	    task_queue = (char *)xmlNodeGetContent(cur);

	} else if (strcmp((char *)cur->name, "task_content") == 0) {

	    task_content = (char *)xmlNodeGetContent(cur);
	}
    }

    if (!task_queue || !task_content) {

	xmlFreeDoc(doc);
	return task_rabbitmq_recv_function_print(-3, buffer);
    }

    task_buffer_copy(arg, task_queue, task_content);
    task_wait(arg);

    xmlFreeDoc(doc);
    return 0;
}

int task_rabbitmq_recv(lg_thread_pool_t *pool)
{

    fprintf(stdout, "starting...\n");
#if 0
    amqp_connection_state_t recv_conn = lg_rabbitmq_conn(rabbitmq_host, rabbitmq_user, rabbitmq_pass);
    if (!recv_conn) {

	fprintf(stderr, "rabbitmq conn::err\n");
	exit(1);
    }

    return lg_rabbitmq_recv(recv_conn, "pseudo-original", task_rabbitmq_recv_function, pool);
#else

    return lg_redis_recv(task_rabbitmq_recv_function, pool);
#endif
}

int main(int argc, char **argv)
{

    if (fork() > 0) exit(1);

    local_addr = argv[1];
    curl_global_init(CURL_GLOBAL_ALL);

    if (lg_filter_keyword_load(&filter_keyword_head, "dict.utf8.filter_keyword")) {

	fprintf(stdout, "dict.utf8.filter_keyword::err\n");
	return -1;
    }

    lg_list_t config_file;
    if (lg_config_file_load(&config_file, "article_manufacture.conf")) {

	fprintf(stdout, "article_manufacture.conf::err\n");
	return -1;
    }

    rabbitmq_host = lg_config_file_find(&config_file, "host");
    rabbitmq_user = lg_config_file_find(&config_file, "user");
    rabbitmq_pass = lg_config_file_find(&config_file, "pass");
    if (!rabbitmq_host
	    || !rabbitmq_user
	    || !rabbitmq_pass
       ) {

	fprintf(stdout, "article_manufacture.conf::err\n");
	return -1;
    }

    int language_total = sizeof(baidu_trans_language_list) / sizeof(char *) - 1;
    lg_thread_pool_t *pool = lg_thread_pool_create(
	    language_total,
	    task_thread,
	    NULL);

    lg_list_t *p = NULL;
    char **language = baidu_trans_language_list;
    lg_list_for_each (p, &pool->head) {

	lg_thread_node_t *node = (lg_thread_node_t *)p;

	task_t *task = malloc(sizeof(task_t));
	if (!task) {

	    fprintf(stdout, "task_init::err\n");
	    return -1;
	}

	node->arg = task;
	task->language = *language++;
    }

    lg_thread_pool_start(pool);
    task_rabbitmq_recv(pool);

    // lg_thread_pool_join(pool);
    return 0;
}


