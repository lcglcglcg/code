#ifndef	_COMMAND
#define	_COMMAND

/*default*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*socket*/
#include <sys/types.h>
#include <sys/socket.h>
#include <net/if.h>

/*struct sockaddr_in*/
#include <netinet/in.h>

/*htons*/
#include <arpa/inet.h>

/*fork*/
#include <unistd.h>

/*wait*/
#include <sys/wait.h>

/*errno*/
#include <errno.h>
#include <linux/errno.h>

/*signal*/
#include <signal.h>

/*open*/
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

/*epoll*/
#include <sys/epoll.h>

#define MAX 	1024
#define PATH 	128
#define IP	"127.0.0.1"
#define PORT 	8000
#define LEN	20	

/*main.c*/
void sig_usr(int sig);

/*web.c*/
void my_web(int sockfd, char *buf);
int path_web(char *buf, char *path);
int down_web(char *buf, char *path, int cfd);

/*error.c*/
int Socket(int family, int type, int protocol);
void Bind(int fd, const struct sockaddr *sa, socklen_t salen);
void Listen(int fd, int backlog);
int Accept(int fd, struct sockaddr *sa, socklen_t *salenptr);
void Connect(int fd, const struct sockaddr *sa, socklen_t salen);
int Open(const char *pathname, int flags);
ssize_t Read(int fd, void *ptr, size_t nbytes);
ssize_t Write(int fd, const void *ptr, size_t nbytes);
void Close(int fd);
void err_sys(const char *s);
void display(char *dest, int mode);

#endif
