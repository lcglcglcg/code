#include "command.h"
#include "error.h"

void my_web(int sockfd, char *buf)
{
    char path[PATH];
    bzero(path, PATH);
    path_web(buf, path);
    down_web(buf, path, sockfd);
    printf("path = %s\n", path);
}

int path_web(char *buf, char *path)
{
    /**********************************************/
    /*home*/
    /**********************************************/

    if(buf[4] == '/' && buf[6] == 'H')
    {
	strcpy(path, "home.html");
	return 0;
    }

    /**********************************************/
    /*path*/
    /**********************************************/

    int i, j;
    for(i = 5, j = 0; buf[i] != ' '; i++)
	path[j++] = buf[i];
    path[j] = 0;
    return 1;
}

int down_web(char *buf, char *path, int sockfd)
{
    int n;
    char path_[PATH] = {"./www/"};
    strcat(path_, path);
    int fd = open(path_, O_RDWR);
    if(fd < 0)
	return 0;
    bzero(buf, MAX);
    while(1)
    {
	if((n = Read(fd, buf, MAX)) == 0)
	    break;
	Write(sockfd, buf, n);
    }
    return 0;
}
