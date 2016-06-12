#include "command.h"

int Socket(int family, int type, int protocol)
{
    int n;

    if ( (n = socket(family, type, protocol)) < 0)
	err_sys("socket error");
    return n;
}

void Bind(int fd, const struct sockaddr *sa, socklen_t salen)
{
    if (bind(fd, sa, salen) < 0)
	err_sys("bind error");
}

void Listen(int fd, int backlog)
{
    char *p;
    if((p = getenv("LISTEN_LOG")) !=  NULL)
	backlog = atoi(p);

    if (listen(fd, backlog) < 0)
	err_sys("listen error");
}

int Accept(int fd, struct sockaddr *sa, socklen_t *salenptr)
{
    int n;

again:
    if ( (n = accept(fd, sa, salenptr)) < 0) {
	if ((errno == ECONNABORTED) || (errno == EINTR))
	    goto again;
	else
	    err_sys("accept error");
    }
    return n;
}

void Connect(int fd, const struct sockaddr *sa, socklen_t salen)
{
    if (connect(fd, sa, salen) < 0)
	err_sys("connect error");
}

int Open(const char *pathname, int flags)
{
    int fd = open(pathname, flags);
    if(fd < 0)
    {
	printf("open err\n");
    }
    return fd;
}

ssize_t Read(int fd, void *ptr, size_t nbytes)
{
    ssize_t n;

again:
    if ( (n = read(fd, ptr, nbytes)) == -1) {
	if (errno == EINTR)
	    goto again;
	else
	    return -1;
    }
    return n;
}

ssize_t Write(int fd, const void *ptr, size_t nbytes)
{
    ssize_t n;

again:
    if ( (n = write(fd, ptr, nbytes)) == -1) {
	if (errno == EINTR)
	    goto again;
	else
	    return -1;
    }
    return n;
}

void Close(int fd)
{
    if (close(fd) == -1)
	err_sys("close error");
}

void err_sys(const char *s)
{
    perror(s);
}
