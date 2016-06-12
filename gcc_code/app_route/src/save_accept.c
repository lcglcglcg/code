
#include "lg_network.h"
#include "lg_thread.h"

void *io_thread_demo(lg_thread_node_t *self)
{

    int sockfd = -1;
    char addr_str[20] = {0};
    char buffer[4096] = {0};
    lg_network_epoll_t *ep = NULL;
    ep = (lg_network_epoll_t *)self->arg;

    for(;;){

	sem_wait(self->sem);
	pthread_mutex_lock(self->mutex);
	lg_queue_dequeue(self->queue, (void *)&sockfd, sizeof(int));
	pthread_mutex_unlock(self->mutex);

	lg_network_epoll_addr_get(ep, sockfd, addr_str);
	fprintf(stdout, "[%s][recv][%d]\n", addr_str, sockfd);

	while(1){

	    memset(buffer, 0, 20);
	    ssize_t size = recv(sockfd, buffer, 10, 0);
	    if (size == 0 || size < 0)
		break;

	    fprintf(stdout, "%s", buffer);
	}

	send(sockfd, "ok", 2, 0);
	lg_network_epoll_event_del(ep, sockfd);
	close(sockfd);
    }

    return NULL;
}

void event_demo(lg_network_epoll_t* ep, int sockfd)
{

    fprintf(stdout, "[event_click][%d]\n", sockfd);

    if (sockfd == ep->listenfd) {

	struct sockaddr_in addr = {0};
	socklen_t addr_len = sizeof(struct sockaddr_in);

	int sockfd_client = accept4(sockfd, (struct sockaddr *)&addr, &addr_len, SOCK_NONBLOCK | SOCK_CLOEXEC);
	lg_network_epoll_addr_add(ep, sockfd_client, addr);
	lg_network_epoll_event_add(ep, sockfd_client);

    } else {

	pthread_mutex_lock(ep->mutex);
	lg_queue_enqueue(ep->queue, (void *)&sockfd, sizeof(int));
	sem_post(ep->sem);
	pthread_mutex_unlock(ep->mutex);
    }
}

void *ep_thread_demo(lg_thread_node_t *self)
{

    lg_network_epoll_t *ep = NULL;
    ep = (lg_network_epoll_t *)self->arg;
    ep->in = event_demo;

    ep->listenfd = lg_network_bind(NULL, 80);
    lg_network_epoll_event_add(ep, ep->listenfd);

    for(;;) lg_network_epoll(ep);
    return NULL;
}

int main(int argc, char *argv[])
{

    lg_network_epoll_t *ep = NULL;
    ep = lg_network_epoll_init(4000, -1);

    lg_thread_pool_t _th_pool;
    lg_thread_pool_t *th_pool = &_th_pool;
    th_pool = lg_thread_pool_init(th_pool, ep->queue, ep->mutex, ep->sem);

    lg_thread_node_add(th_pool, ep_thread_demo, ep);
    lg_thread_pool_add(100, th_pool, io_thread_demo, ep);

    lg_thread_pool_start(th_pool);
    lg_thread_pool_join(th_pool);

    return 0;
}


