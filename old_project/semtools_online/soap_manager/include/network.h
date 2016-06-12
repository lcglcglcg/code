
#ifndef	__NETWORK_H__
#define	__NETWORK_H__

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

#define	NETWORK_SUCCESS 0
#define	NETWORK_ERROR -1
#define	NETWORK_TIMEOUT -2
#define	NETWORK_SIGNALEINTR -3

#define	NETWORK_MAX_FD 10000
#define	NETWORK_MAX_POLL 1024
#define	NETWORK_MAX_EVENT NETWORK_MAX_FD
#define	NETWORK_MAX_BACKLOG 1024

extern int network_fdmax(int fdmax);
extern int network_noblocking(int sockfd);
extern int network_check(int sockfd);
extern int network_not_time_wait(int sockfd);
extern int network_set_kernel_buffer(int sockfd, int send_size, int recv_size);
extern int network_get_kernel_buffer(int sockfd, int *send_size, int *recv_size);
extern int accept4(int sockfd, struct sockaddr *addr, socklen_t *addrlen, int flags);

extern int network_bind(char *address, int port);
extern int network_accept(int listenfd);
extern int network_connect(char *address, int port);
extern size_t network_send(int sockfd, void *buffer, size_t buffer_size);
extern size_t network_recv(int sockfd, void *buffer, size_t buffer_size);


extern int network_udp_bind(char *address, int port);
extern int network_udp_accept(int listenfd);
extern int network_udp_connect(char *address, int port);
extern size_t network_udp_send(int sockfd, void *buffer, size_t buffer_size);
extern size_t network_udp_recv(int sockfd, void *buffer, size_t buffer_size);


typedef struct network_epoll_t network_epoll_t;
typedef void(network_epoll_in)(int sockfd, void *arg);
typedef void(network_epoll_out)(int sockfd, void *arg);
typedef void(network_epoll_err)(int sockfd, void *arg);
typedef void(network_epoll_hup)(int sockfd, void *arg);
typedef void(network_epoll_pri)(int sockfd, void *arg);

struct network_epoll_t {

    int epfd;
    int timer;
    int error;
    void *arg;

    unsigned int event_pool_max;
    struct epoll_event *event_pool;
    struct epoll_event event_ctl;

    network_epoll_in *in;
    network_epoll_out *out;
    network_epoll_err *err;
    network_epoll_hup *hup;
    network_epoll_pri *pri;
};

extern int network_epoll_event_add(network_epoll_t *ep, int sockfd, int mask);
extern int network_epoll_event_mod(network_epoll_t *ep, int sockfd, int mask);
extern int network_epoll_event_del(network_epoll_t *ep, int sockfd, int mask);
extern int network_epoll(network_epoll_t *ep);
extern network_epoll_t *network_epoll_init(int event_max, int pool_max, int timer);


#endif// __NETWORK_H__
