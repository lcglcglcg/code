
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

#define CH_COMM	2	//分割符
#define CH_ROW	22	//结束符
#define SOCK_PATH "/tmp/ad_system.sock"

void info_print(char *buffer, size_t size)
{

    size_t i;
    char *str = buffer;
    for (i = 0; i < size; i++) {

	if (buffer[i] == CH_COMM) {

	    buffer[i] = 0;
	    fprintf(stdout, "[%s]\n", str);
	    str = &buffer[i + 1];

	} else if (buffer[i] == CH_ROW) {

	    buffer[i] = 0;
	    fprintf(stdout, "[%s]\n", str);
	    fprintf(stdout, "============================\n");
	    str = &buffer[i + 1];
	}
    }

    if (str == buffer)
	fprintf(stdout, "[%s]\n", buffer);
}

int info_command(char *str, char *buffer)
{
    struct sockaddr_un addr;
    memset(&addr, 0, sizeof(struct sockaddr_un));
    addr.sun_family = AF_UNIX;
    strcpy(addr.sun_path, SOCK_PATH);

    int sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (connect(sockfd, (struct sockaddr *)&addr, sizeof(struct sockaddr_un)) == -1) 
	return -1;

    write(sockfd, str, strlen(str));

    size_t i, size = 0;
    read(sockfd, &size, sizeof(size_t));
    for (i = 0; i < size; i++)
	read(sockfd, &buffer[i], 1);

    buffer[size] = 0;
    info_print(buffer, size);
    close(sockfd);
    return 0;
}

int main()
{

    char buffer[4096];
    while(printf("> "), fgets(buffer, 4096, stdin), !feof(stdin))
	info_command(buffer, buffer);

    return 0;
}





