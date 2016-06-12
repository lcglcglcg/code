
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include "License.h"

#ifdef	WIN32
#include <windows.h>
#else
#include <fcntl.h>
#include <errno.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#define	SOCKET int
#define	closesocket(sockfd) close(sockfd)
#endif

#define HOST "auth.15talk.haomeit.com"
#define	PORT 1234

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

void License_packet_key_coding(char *buffer, int size)
{

    int i, key;
    for (i = 0; i < size; i++) {

	if (i % 2)
	    buffer[i] = ~buffer[i];

	if (!(i % 3))
	    key = ((i + 1) * 5);
	else if (!(i % 4))
	    key = ((i + 2) / 3);
	else if (i % 5)
	    key = ((i + 3) * 4);
	else
	    key = ((i + 4) / 2);

	if (key % 2)
	    buffer[i] = (buffer[i] + key);
	else
	    buffer[i] = (buffer[i] - key);
    }
}

void License_packet_key_decoding(char *buffer, int size)
{

    int i, key;
    for (i = 0; i < size; i++) {

	if (!(i % 3)) 
	    key = ((i + 1) * 5);
	else if (!(i % 4))
	    key = ((i + 2) / 3);
	else if (i % 5)
	    key = ((i + 3) * 4);
	else
	    key = ((i + 4) / 2);

	if (key % 2)
	    buffer[i] = (buffer[i] - key);
	else
	    buffer[i] = (buffer[i] + key);

	if (i % 2)
	    buffer[i] = ~buffer[i];
    }
}

int License_packet_key_check(const char *licence_path)
{

    srand(time(NULL));
    char rand_str[128] = {0};
    sprintf(rand_str, "%u", rand());

    char cpu_info[4096] = {0};
    if (get_License_drivers_cpuinfo(cpu_info))
	return -1;

    char mac_addr[128] = {0};
    if (get_License_drivers_mac_addr(mac_addr))
	return -2;

    char licence_file[4096] = {0};
    if (!get_License_drivers_licence(licence_path, licence_file))
	return -1;

    char buffer[4096] = {0};
    int size = sprintf(buffer, "%s\n%s\n%s\n%s\n", rand_str, mac_addr, cpu_info, licence_file);
    fprintf(stdout, "[%s]\n", buffer);
    License_packet_key_coding(buffer, size);

    SOCKET sockfd = 0;
    if (!(sockfd = network_connect(HOST, PORT)))
	return -1;

    if (send(sockfd, buffer, size, 0) != size) {

	closesocket(sockfd);
	return -2;
    }

    if ((size = recv(sockfd, buffer, 4096, 0)) == -1) {

	closesocket(sockfd);
	return -3;
    }

    buffer[size] = 0;
    License_packet_key_decoding(buffer, size);
    if (strcmp(rand_str, buffer) == 0) {

	closesocket(sockfd);
	return 0;
    }

    closesocket(sockfd);
    return -4;
}


