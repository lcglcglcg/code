
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "control_network.h"

struct in_addr network_host_to_addr(const char *host)
{

    struct sockaddr_in addr = {0};
    struct hostent *phostent = NULL;

    if((phostent = gethostbyname(host)) == NULL) {

	return addr.sin_addr;
    }

    if (phostent->h_addr_list[0]) {

	memcpy(&addr.sin_addr.s_addr, phostent->h_addr_list[0], phostent->h_length);
    }

    return addr.sin_addr;
}

int WSAStartupStatus = 0;
SOCKET network_connect(const char *host, int port)
{

#ifdef WIN32
    if (WSAStartupStatus == 0) {

	WSADATA wsadata;
	WSAStartup(MAKEWORD(2, 2), &wsadata);
	WSAStartupStatus = 1;
    }
#endif

    struct sockaddr_in addr = {0};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr = network_host_to_addr(host);
    if (!addr.sin_addr.s_addr)
	return 0;

    SOCKET sockfd = socket(AF_INET, SOCK_STREAM, 0);
#ifdef WIN32
    if (sockfd == INVALID_SOCKET)
	return 0;
#else
    if (sockfd < 0)
	return 0;
#endif

    struct timeval time_out;
    time_out.tv_sec = 30;
    time_out.tv_usec = 0;
    if (setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (char *)&time_out, sizeof(time_out)) == -1) {

	closesocket(sockfd);
	return 0;
    }

    if (connect(sockfd, (const struct sockaddr *)&addr, sizeof(addr)) == -1) {

	closesocket(sockfd);
	return 0;
    }

    return sockfd;
}



