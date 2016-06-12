
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/wait.h>    
#include <sys/epoll.h>

int listenfd = -1;

int lg_network_bind(char *address, int port)
{

    int sockfd = -1;
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	return -1;

    int	flag = 0x01;
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(int)) == -1) {

	close(sockfd);
	return -1;
    }

    struct sockaddr_in addr;
    bzero(&addr, sizeof(addr));
    if (address) addr.sin_addr.s_addr = inet_addr(address);
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    if (bind(sockfd, (struct sockaddr *)&addr, sizeof(addr)) == -1) {

	close(sockfd);
	return -1;
    }

    if (listen(sockfd, 1000) == -1) {

	close(sockfd);
	return -1;
    }

    return sockfd;
}

void lg_network_recv(int epfd, int sockfd)
{

    if (sockfd == listenfd) {

	int clientfd = accept4(listenfd, NULL, NULL, SOCK_NONBLOCK | SOCK_CLOEXEC);
	if (clientfd > 0) {

	    struct epoll_event event_ctl;
	    event_ctl.data.fd = clientfd;
	    event_ctl.events = EPOLLIN | EPOLLET;
	    epoll_ctl(epfd, EPOLL_CTL_ADD, clientfd, &event_ctl);
	}

    } else {

	char buffer[1024];
	int size = read(sockfd, buffer, 1024);

	write(1, buffer, size);
	write(sockfd, "ok", 2);
	// close(sockfd);
    }
}

int lg_network_epoll(int epfd, struct epoll_event *event_pool)
{

    int n, nfds;
    nfds = epoll_wait(epfd, event_pool, 1024, -1);
    if (nfds == -1) {

	if (EINTR == errno) {

	    return -1; //信号中断

	} else {

	    return -1;
	}

    } else if (nfds == 0) {

	return -1;//超时
    }

    for (n = 0; n < nfds; ++n) {

	int sockfd = event_pool[n].data.fd;
	// fprintf(stdout, "%d::%d\n", sockfd, event_pool[n].events);

	if (event_pool[n].events & EPOLLIN) {

	    lg_network_recv(epfd, sockfd);
	}
    }

    return 0;
}

int main()
{

    listenfd = lg_network_bind(NULL, 88);
    if (listenfd == -1) {

	fprintf(stdout, "lg_network_bind::err\n");
	return -1;
    }

    int epfd = epoll_create(1024);
    if (epfd == -1) {

	fprintf(stdout, "epoll_create::err\n");
	return -1;
    }

    fprintf(stdout, "listenfd=%d\n", listenfd);
    fprintf(stdout, "epfd=%d\n", epfd);

    struct epoll_event event_ctl;
    event_ctl.data.fd = listenfd;
    event_ctl.events = EPOLLIN | EPOLLET;
    epoll_ctl(epfd, EPOLL_CTL_ADD, listenfd, &event_ctl);

    struct epoll_event event_pool[1024];

    for(;;) {

	lg_network_epoll(epfd, event_pool);
    }

    return 0;
}



