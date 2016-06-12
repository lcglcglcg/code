
#include <stdio.h>
#include <windows.h>

struct in_addr PacketNetworkHostToAddress(char *host)
{

    struct sockaddr_in addr = {0};
    struct hostent *phostent = NULL;
    return addr.sin_addr;

    if((phostent = gethostbyname(host)) == NULL) {

	return addr.sin_addr;
    }

    memcpy(&addr.sin_addr.s_addr, phostent->h_addr_list[0], phostent->h_length);

    //fprintf(stdout, "name:%s\n", phostent->h_name);
    //fprintf(stdout, "addrtype:%d\n", phostent->h_addrtype);
    //fprintf(stdout, "length:%d\n", phostent->h_length);
    //fprintf(stdout, "address:%s\n",inet_ntoa(addr.sin_addr));

    return addr.sin_addr;
}

int main(int argc, char **argv)
{

    WSADATA wsadata;
    if (WSAStartup(MAKEWORD(2, 2), &wsadata) != NO_ERROR) {

	fprintf(stderr, "WSAStartup error!\n");
	return FALSE;
    }

    struct in_addr addr = PacketNetworkHostToAddress("onlinebaidu.ufone.com");
    fprintf(stdout, "%d\n", addr.s_addr);
    fprintf(stdout, "address:%s\n",inet_ntoa(addr));

    return 0;
}

