
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <curl/curl.h>

curl_socket_t lg_curl_set_socket(void *clientp, curlsocktype purpose, struct curl_sockaddr *addr)
{

    int sockfd = socket(addr->family, addr->socktype, addr->protocol);
    struct linger opt;
    opt.l_onoff = 0x01;
    opt.l_linger = 0x00;
    setsockopt(sockfd, SOL_SOCKET, SO_LINGER, &opt, sizeof(struct linger));
    return sockfd;
}

int lg_curl_recv(char *buffer, int size, int nmemb, char *recv_buffer)
{

    int _size = size * nmemb;
    memcpy(recv_buffer, buffer, _size);
    return _size;
}

int lg_curl_post(char *url, char *send_buffer, char *recv_buffer)
{

    CURL *curl = curl_easy_init();
    if(curl == NULL)
	return -1;

    curl_easy_setopt(curl, CURLOPT_POST, 1);
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_OPENSOCKETFUNCTION, lg_curl_set_socket);

    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, send_buffer);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, lg_curl_recv);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, recv_buffer);

    CURLcode ret = curl_easy_perform(curl);
    curl_easy_cleanup(curl);
    return ret;
}


