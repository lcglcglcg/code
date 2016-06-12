
#include <ctype.h>
#include "lg_string.h"
#include "ad_system_network.h"

typedef struct {

    int http_type;//1get 2post
    char Host[256];
    char X_Real_IP[128];
    char X_Forwarded_For[128];
    char Cookie[1024];
    size_t Content_Length;

    int sockfd;
    char *buffer;
    size_t size;

} ad_system_network_http_t;

void ad_system_network_addr(char *buffer, char *addr)
{

    char *str = NULL;
    if ((str = strstr(buffer, "X-Forwarded-For: "))) {

	for (str += 17; isalnum(*str) || *str == '.'; str++) {

	    *addr++ = *str;
	}   

	return;
    }
}

int ad_system_network_http_value(char *value, char *str)
{

    if (*str == ' ')str++;
    strcpy(value, str);
    return 0;
}

int ad_system_network_http_post_parse(ad_system_network_http_t *http, char *buffer, size_t size)
{

    int i;
    for (i = 0; i < http->Content_Length; i++) {

	if (read(http->sockfd, &buffer[i], 1) <= 0) {

	    close(http->sockfd);
	    pthread_exit(NULL);
	}
    }

    buffer[i] = 0;
    http->buffer = buffer;
    http->size = http->Content_Length;
    return 0;
}

int ad_system_network_http_parse(ad_system_network_http_t *http, char *buffer, size_t size)
{

    if (strncmp(buffer, "GET", 3) == 0)
	http->http_type = 1;
    else if (strncmp(buffer, "POST", 4) == 0)
	http->http_type = 2;
    else
	return -1;

    char *str = NULL;
    char *save = NULL;
    char *p_buf = buffer;
    while((str = strtok_r(p_buf, "\r\n", &save))) {

	if (strncmp(str, "Host:", 5) == 0) {
	    ad_system_network_http_value(http->Host, str + 5);

	} else if (strncmp(str, "X-Real-IP:", 10) == 0) {
	    ad_system_network_http_value(http->X_Real_IP, str + 10);

	} else if (strncmp(str, "X-Forwarded-For:", 16) == 0) {
	    ad_system_network_http_value(http->X_Forwarded_For, str + 16);

	} else if (strncmp(str, "Cookie:", 7) == 0) {
	    ad_system_network_http_value(http->Cookie, str + 7);

	} else if (strncmp(str, "Content-Length:", 15) == 0) {
	    http->Content_Length = atoi(str + 15);
	}

	p_buf = NULL;
    }

    if (http->http_type == 1) {

	if (buffer[3] == ' ' && buffer[4] == '/')
	    http->buffer = &buffer[4];
	else 
	    return -1;

	if ((str = strstr(buffer, "HTTP/")))
	    *str = 0;
	else 
	    return -1;

	http->size = strlen(http->buffer);

    } else {

	if (ad_system_network_http_post_parse(http, buffer, size))
	    return -1;
    }

#if 0
    fprintf(stdout, "http_type=%d\n", http->http_type);
    fprintf(stdout, "Host=%s\n", http->Host);
    fprintf(stdout, "X_Real_IP=%s\n", http->X_Real_IP);
    fprintf(stdout, "X_Forwarded_For=%s\n", http->X_Forwarded_For);
    fprintf(stdout, "Cookie=%s\n", http->Cookie);
    fprintf(stdout, "Content_Length=%zd\n", http->Content_Length);
    fprintf(stdout, "buffer=%s\n", http->buffer);
#endif
    lg_string_url_decode_r(http->buffer, http->size);
    return 0;
}

void *ad_system_network_thread(void *arg)
{

    pthread_detach(pthread_self());

    ad_system_network_sockfd_t temp;
    temp.arg = arg;
    int sockfd = temp.sockfd;

    size_t size = 0;
    char buffer[200000] = {0};

    for (size = 0; ; size++){

	if (read(sockfd, &buffer[size], 1) <= 0) {

	    close(sockfd);
	    pthread_exit(NULL);
	}

	if (size > 4) {

	    if (buffer[size - 3] == '\r' 
		    && buffer[size - 2] == '\n' 
		    && buffer[size - 1] == '\r' 
		    && buffer[size] == '\n') {
		break;
	    }
	}
    }

    ad_system_network_http_t http;
    memset(&http, 0, sizeof(ad_system_network_http_t));
    http.sockfd = sockfd;

    if (ad_system_network_http_parse(&http, buffer, size))
	close(sockfd);
    else
	ad_system_packet_parse(sockfd, http.buffer, http.Cookie, http.X_Forwarded_For);

    pthread_exit(NULL);
}

int ad_system_network_run()
{


    int listenfd = -1;
    if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) == NETWORK_ERROR)
	return NETWORK_ERROR;

    int	flag = 0x01;
    if (setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(int)) == NETWORK_ERROR) {

	close(listenfd);
	return NETWORK_ERROR;
    }

    struct sockaddr_in addr;
    bzero(&addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(5001);
    if (bind(listenfd, (struct sockaddr *)&addr, sizeof(addr)) == NETWORK_ERROR) {

	close(listenfd);
	return NETWORK_ERROR;
    }

    if (listen(listenfd, 1000) == NETWORK_ERROR) {

	close(listenfd);
	return NETWORK_ERROR;
    }

    while(1){

	ad_system_network_sockfd_t temp = {0};
	if ((temp.sockfd = accept(listenfd, NULL, NULL)) > 0)
	    //if ((temp.sockfd = accept4(listenfd, NULL, NULL, SOCK_NONBLOCK)) > 0)
	{
	    pthread_t tid;
	    pthread_create(&tid, NULL, ad_system_network_thread, temp.arg);
	}
    }

    return 0;
}




