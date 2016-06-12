
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include "control_deivers.h"
#include "control_network.h"

#define HOST "auth.15talk.haomeit.com"
#define	PORT 1234

void control_key_coding(char *buffer, int size)
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

void control_key_decoding(char *buffer, int size)
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

int control_packet_baidu_check()
{

    SOCKET sockfd = 0;
    if (!(sockfd = network_connect("www.baidu.com", 80)))
	return -1;

    char buffer[4096];
    int size = sprintf(buffer, "GET / HTTP/1.1\r\n"
	    "Host: www.baidu.com\r\n"
	    "User-Agent: Mozilla/5.0\r\n"
	    "Connection: keep-alive\r\n"
	    "Cache-Control: max-age=0\r\n\r\n");

    if (send(sockfd, buffer, size, 0) != size) {

	closesocket(sockfd);
	return -1;
    }

    if ((size = recv(sockfd, buffer, 4096, 0)) == -1) {

	closesocket(sockfd);
	return -1;
    }

    closesocket(sockfd);
    return 0;
}

int control_packet_key_check(const char *licence_path)
{

    srand(time(NULL));
    char rand_str[128] = {0};
    sprintf(rand_str, "%u", rand());

    char mac_addr[128] = {0};
    get_deivers_mac_addr(mac_addr);

    char cpu_id[128] = {0};
    get_deivers_cpuid(cpu_id);

    char disk_id[128] = {0};
    get_deivers_disk(disk_id);

    char licence_str[4096] = {0};
    if (!get_deivers_licence(licence_path, licence_str))
	return -1;

    char buffer[4096] = {0};
    int size = sprintf(buffer, "%s\n%s\n%s\n%s\n%s\n", rand_str, mac_addr, cpu_id, disk_id, licence_str);
    //fprintf(stdout, "[License send][%s]\n", buffer);
    control_key_coding(buffer, size);

    SOCKET sockfd = 0;
    if (!(sockfd = network_connect(HOST, PORT)))
	return -1;

    if (send(sockfd, buffer, size, 0) != size) {

	//fprintf(stdout, "License send err\n");
	closesocket(sockfd);
	return -2;
    }

    if ((size = recv(sockfd, buffer, 4096, 0)) == -1) {

	//fprintf(stdout, "License recv err\n");
	closesocket(sockfd);
	return -3;
    }

    buffer[size] = 0;
    control_key_decoding(buffer, size);
    if (strcmp(rand_str, buffer) == 0) {

	closesocket(sockfd);
	return 0;
    }

    //fprintf(stdout, "[License recv][%s]\n", buffer);
    closesocket(sockfd);
    return -4;
}

