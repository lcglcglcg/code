
#ifndef	__LG_NETWORK_H__
#define	__LG_NETWORK_H__

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
#include <semaphore.h>
#include <pthread.h>

#define	NETWORK_SUCCESS 0
#define	NETWORK_ERROR -1
#define	NETWORK_TIMEOUT -2
#define	NETWORK_SIGNALEINTR -3

#ifdef __cplusplus
extern "C" { 
#endif

    extern int lg_network_fdmax(int fdmax);
    extern int lg_network_noblocking(int sockfd);
    extern int lg_network_check(int sockfd);
    extern int lg_network_not_time_wait(int sockfd);
    extern int lg_network_set_kernel_buffer(int sockfd, int send_size, int recv_size);
    extern int lg_network_get_kernel_buffer(int sockfd, int *send_size, int *recv_size);
    extern int accept4(int sockfd, struct sockaddr *addr, socklen_t *addrlen, int flags);

    extern int lg_network_bind(char *address, int port);
    extern int lg_network_accept(int listenfd);
    extern int lg_network_connect(char *address, int port);
    extern ssize_t lg_network_send(int sockfd, void *buffer, size_t buffer_size);
    extern ssize_t lg_network_recv(int sockfd, void *buffer, size_t buffer_size);

    typedef struct lg_network_epoll_t lg_network_epoll_t;
    typedef void(lg_network_epoll_in)(lg_network_epoll_t* ep, int sockfd);
    typedef void(lg_network_epoll_out)(lg_network_epoll_t* ep, int sockfd);
    typedef void(lg_network_epoll_err)(lg_network_epoll_t* ep, int sockfd);
    typedef void(lg_network_epoll_hup)(lg_network_epoll_t* ep, int sockfd);
    typedef void(lg_network_epoll_pri)(lg_network_epoll_t* ep, int sockfd);

    struct lg_network_epoll_t {

	lg_network_epoll_in *in;
	lg_network_epoll_out *out;
	lg_network_epoll_err *err;
	lg_network_epoll_hup *hup;
	lg_network_epoll_pri *pri;

	int epfd;
	int timer;
	int event_max;
	struct epoll_event *event_pool;
	struct sockaddr_in *addr_pool;
	struct epoll_event event_ctl;
	int listenfd;
	void *arg;

	lg_queue_t _queue;
	lg_queue_t *queue;
	pthread_mutex_t _mutex;
	pthread_mutex_t *mutex;
	sem_t _sem;
	sem_t *sem;
    };

    extern int lg_network_epoll(lg_network_epoll_t *ep);
    extern int lg_network_epoll_event_add(lg_network_epoll_t *ep, int sockfd, int mask);
    extern int lg_network_epoll_event_mod(lg_network_epoll_t *ep, int sockfd, int mask);
    extern int lg_network_epoll_event_del(lg_network_epoll_t *ep, int sockfd, int mask);

    extern void lg_network_epoll_addr_add(lg_network_epoll_t *ep, int sockfd, struct sockaddr_in addr);
    extern void lg_network_epoll_addr_get(lg_network_epoll_t *ep, int sockfd, char *addr_str);

    extern int lg_network_epoll_external(lg_network_epoll_t *ep);
    extern lg_network_epoll_t *lg_network_epoll_init(int event_max, int timer);

#ifdef __cplusplus
}
#endif

#endif// __NETWORK_H__
