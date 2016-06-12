
#ifndef	__LG_NETWORK_H__
#define	__LG_NETWORK_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>    
#include <sys/epoll.h>

#define	NETWORK_SUCCESS 0
#define	NETWORK_ERROR -1
#define	NETWORK_TIMEOUT -2
#define	NETWORK_SIGNALEINTR -3
#define NETWORK_EPOLL_FD_MAX 65535

#ifdef __cplusplus
extern "C" { 
#endif

    typedef struct lg_network_epoll_s lg_network_epoll_t;
    typedef void(lg_network_epoll_function_t)(lg_network_epoll_t* ep, int sockfd);
    struct lg_network_epoll_s {

	int epfd;
	int timer;
	struct epoll_event *event_pool;
	struct sockaddr_in *addr_pool;
	void *arg;

	lg_network_epoll_function_t *in;
	lg_network_epoll_function_t *out;
	lg_network_epoll_function_t *err;
	lg_network_epoll_function_t *hup;
	lg_network_epoll_function_t *pri;
    };

    extern lg_network_epoll_t *lg_network_epoll_init();
    extern int lg_network_epoll_wait(lg_network_epoll_t *ep);
    extern int lg_network_epoll_event_add(lg_network_epoll_t *ep, int sockfd);
    extern int lg_network_epoll_event_mod(lg_network_epoll_t *ep, int sockfd, int mask);
    extern int lg_network_epoll_event_del(lg_network_epoll_t *ep, int sockfd);
    extern void lg_network_epoll_set_function_in(lg_network_epoll_t *ep, lg_network_epoll_function_t *function);
    extern void lg_network_epoll_set_function_out(lg_network_epoll_t *ep, lg_network_epoll_function_t *function);

    extern int lg_network_epoll_addr_add(lg_network_epoll_t *ep, int sockfd, struct sockaddr_in addr);
    extern char *lg_network_epoll_addr_get(lg_network_epoll_t *ep, int sockfd, char *addr_str);

    extern int accept4(int sockfd, struct sockaddr *addr, socklen_t *addrlen, int flags);
    extern int lg_network_accept(lg_network_epoll_t *ep, int listenfd);
    extern int lg_network_connect(lg_network_epoll_t *ep, char *address, int port);
    extern ssize_t lg_network_send(int sockfd, void *buffer, size_t buffer_size);
    extern ssize_t lg_network_recv(int sockfd, void *buffer, size_t buffer_size);

    extern int lg_network_bind(char *address, int port);
    extern int lg_network_listen(int sockfd, int backlog);

    extern int lg_network_fdmax(int fdmax);
    extern int lg_network_noblocking(int sockfd);
    extern int lg_network_check(int sockfd);
    extern int lg_network_not_time_wait(int sockfd);
    extern int lg_network_set_kernel_buffer(int sockfd, int send_size, int recv_size);
    extern int lg_network_get_kernel_buffer(int sockfd, int *send_size, int *recv_size);

#ifdef __cplusplus
}
#endif

#endif// __NETWORK_H__



