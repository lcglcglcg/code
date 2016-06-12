
#include <stdio.h>
#include <windows.h>
#include "json.h"

struct in_addr network_host_to_addr(char *host)
{

    struct sockaddr_in addr = {0};
    struct hostent *phostent = NULL;

    if((phostent = gethostbyname(host)) == NULL) {

	return addr.sin_addr;
    }

    if (phostent->h_addr_list[0]) {

	memcpy(&addr.sin_addr.s_addr, phostent->h_addr_list[0], phostent->h_length);
    }

    fprintf(stdout, "network_host_to_addr: %s\n", inet_ntoa(addr.sin_addr));
    return addr.sin_addr;
}

SOCKET network_connect()
{

    int retval;
    struct sockaddr_in addr = {0};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(6001);
    //addr.sin_addr = network_host_to_addr("lcg.leechat.com");
    addr.sin_addr.s_addr = inet_addr("192.168.10.88");
    if (!addr.sin_addr.s_addr) {

	fprintf(stderr, "network_connect: error!\n");
	return FALSE;
    }

    //recv time out
    struct timeval time_out;
    time_out.tv_sec = 5;
    time_out.tv_usec = 0;

    SOCKET sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == INVALID_SOCKET) {

	fprintf(stderr, "socket: error!\n");
	return FALSE;
    }

    retval = setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (char *)&time_out, sizeof(time_out));
    if (retval == -1) {

	fprintf(stderr, "setsockopt: error!\n");
	closesocket(sockfd);
	return FALSE;
    }

    retval = connect(sockfd, (SOCKADDR*)&addr, sizeof(addr));
    if (retval == -1) {

	fprintf(stderr, "connect: error!\n");
	closesocket(sockfd);
	return FALSE;
    }

    return sockfd;
}

int network_send(SOCKET sockfd, void *buffer, int buffer_size)
{

    int size = send(sockfd, (char *)buffer, buffer_size, 0);
    if (size == -1 || size != buffer_size) {

	fprintf(stderr, "send: %d\n", size);
	closesocket(sockfd);
	return FALSE;
    }

    return size;
}

int network_recv(SOCKET sockfd, void *buffer, int buffer_size)
{

    int size = recv(sockfd, buffer, buffer_size, 0);
    if (size == -1) {

	fprintf(stderr, "recv: %d\n", size);
	closesocket(sockfd);
	return FALSE;
    }

    return size;
}

int main(int argc, char *argv[])
{

    WSADATA wsadata;
    if (WSAStartup(MAKEWORD(2, 2), &wsadata) != NO_ERROR) {

	fprintf(stderr, "WSAStartup: error!\n");
	return FALSE;
    }

    json_object *jroot = json_object_new_object();
    json_object_object_add(jroot, "packType", json_object_new_string("3"));
    json_object_object_add(jroot, "index", json_object_new_string("0"));
    json_object_object_add(jroot, "url", NULL);
    json_object_object_add(jroot, "link", NULL);
    json_object_object_add(jroot, "cookie", NULL);

    char *p = malloc(4096);
    SOCKET sockfd = network_connect();

    char *buffer = (char *)json_object_to_json_string(jroot);
    network_send(sockfd, buffer, strlen(buffer));
    json_object_put(jroot);

    buffer = p;
    memset(buffer, 0, 4096);
    network_recv(sockfd, buffer, 4096);
    closesocket(sockfd);

    jroot = json_tokener_parse(buffer);
    json_object_object_add(jroot, "cookie", json_object_new_string("cookie"));
    buffer = (char *)json_object_to_json_string(jroot);
    fprintf(stdout, "%s\n", buffer);

    sockfd = network_connect();
    network_send(sockfd, buffer, strlen(buffer));
    json_object_put(jroot);

    buffer = p;
    memset(buffer, 0, 4096);
    network_recv(sockfd, buffer, 4096);
    closesocket(sockfd);
    
    jroot = json_tokener_parse(buffer);
    json_object_object_add(jroot, "cookie", json_object_new_string("cookie"));
    buffer = (char *)json_object_to_json_string(jroot);
    fprintf(stdout, "%s\n", buffer);

    WSACleanup();
    return 0;
}

