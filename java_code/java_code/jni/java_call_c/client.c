#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>

char *utf8 = "UTF8中文测试";
char *gbk = "GBK中文测试";
int main(int argc, char **argv)
{

    int Code;
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr.sin_port = htons(8000);

    for (;;) {

	sleep(1);
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	Code = connect(sockfd, (struct sockaddr *)&addr, sizeof(struct sockaddr_in));

	if (Code != -1) {

	    Code = write(sockfd, utf8, strlen(utf8)); 
	    Code = write(sockfd, gbk, strlen(gbk)); 
	}

	close(sockfd);
    }

    return 0;
}

