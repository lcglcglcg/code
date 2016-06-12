
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
#include <pthread.h>
#include <semaphore.h>
#include "lg_thread.h"
#include "lg_queue.h"
#include "packet.h"

void network_packet_click_parse(int sockfd, char *buffer, char *addr)
{

    fprintf(stdout, "%s\n", buffer);
}

void *io_thread_click(lg_thread_node_t *self)
{

    int sockfd = -1;
    char addr_str[20] = {0};
    char buffer[4096] = {0};
    lg_network_epoll_t *ep = NULL;
    ep = (lg_network_epoll_t *)self->arg;

    while(1){

	sem_wait(self->sem);
	pthread_mutex_lock(self->mutex);
	lg_queue_dequeue(self->queue, (void *)&sockfd, sizeof(int));
	pthread_mutex_unlock(self->mutex);

	struct in_addr addr = ep->addr_pool[sockfd].sin_addr;
	inet_ntop(AF_INET, &addr, addr_str, 16);

	if (read(sockfd, buffer, 4096) > 0)
	    network_packet_click_parse(sockfd, buffer, addr_str);

	lg_network_epoll_event_del(ep, sockfd, EPOLLIN);
	close(sockfd);
    }

    return NULL;
}

void event_click(int sockfd, lg_network_epoll_t* ep)
{

    if (sockfd == ep->listenfd) {

	struct sockaddr_in addr = {0};
	socklen_t addr_len = sizeof(struct sockaddr_in);

	//int sockfd_client = accept4(sockfd, (struct sockaddr *)&addr, &addr_len, SOCK_NONBLOCK | SOCK_CLOEXEC);
	int sockfd_client = accept(sockfd, (struct sockaddr *)&addr, &addr_len);
	lg_network_epoll_event_add(ep, sockfd_client, addr, EPOLLIN);

    } else {

	pthread_mutex_lock(ep->mutex);
	lg_queue_enqueue(ep->queue, (void *)&sockfd, sizeof(int));
	sem_post(ep->sem);
	pthread_mutex_unlock(ep->mutex);
    }
}

void *ep_thread_click(lg_thread_node_t *self)
{

    lg_network_epoll_t *ep = NULL;
    ep = (lg_network_epoll_t *)self->arg;
    ep->in = event_click;

    struct sockaddr_in addr = {0};
    ep->listenfd = lg_network_bind(NULL, 5001);
    lg_network_epoll_event_add(ep, ep->listenfd, addr, EPOLLIN);

    for(;;) lg_network_epoll(ep);
    return NULL;
}

int main(int argc, char *argv[])
{

    lg_thread_pool_t *th_pool = NULL;
    th_pool = lg_thread_pool_init(NULL, NULL, NULL, NULL, NULL);

    lg_network_epoll_t *ep_click = NULL;
    ep_click = lg_network_epoll_init(1000, -1);
    lg_thread_pool_external_init(th_pool, ep_click);
    lg_thread_node_add(th_pool, ep_thread_click, ep_click);
    lg_thread_pool_add(20, th_pool, io_thread_click, ep_click);

    lg_thread_pool_start(th_pool);
    lg_thread_pool_join(th_pool);

    return 0;
}

