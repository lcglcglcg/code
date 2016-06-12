
#include "lg_network.h"
#include "lg_thread.h"

#if 0
#define	EX_PORT 80
#define	IN_IP "192.168.10.150"
#define	IN_PORT 80
#else
#define	EX_PORT 3389
#define	IN_IP "119.161.216.21"
#define	IN_PORT 3389
#endif

lg_thread_pool_t *th_pool = NULL;
lg_network_epoll_t *external_ep = NULL;
lg_network_epoll_t *internal_ep = NULL;
int external_to_internal[40960] = {-1};
int internal_to_external[40960] = {-1};

lg_queue_t external_to_internal_queue;
pthread_mutex_t external_to_internal_mutex;
sem_t external_to_internal_sem;

lg_queue_t internal_to_external_queue;
pthread_mutex_t internal_to_external_mutex;
sem_t internal_to_external_sem;

void *external_to_internal_thread(lg_thread_node_t *self)
{

    int sockfd;
    char addr_str[20] = {0};
    char buffer[4096] = {0};

    for(;;){

	sem_wait(&external_to_internal_sem);
	pthread_mutex_lock(&external_to_internal_mutex);
	lg_queue_dequeue(&external_to_internal_queue, (void *)&sockfd, sizeof(int));
	pthread_mutex_unlock(&external_to_internal_mutex);

	lg_network_epoll_addr_get(external_ep, internal_to_external[sockfd], addr_str);
	fprintf(stdout, "[recv][%s][%d]\n", addr_str, internal_to_external[sockfd]);

	fprintf(stdout, "[external_to_internal_thread][%d]===>[%d]\n", 
		internal_to_external[sockfd], sockfd);

	while(1){

	    ssize_t size = recv(internal_to_external[sockfd], buffer, 4096, 0);
	    fprintf(stdout, "external_to_internal size=%zd\n", size);
	    if (size == 0) {

		close(sockfd);
		close(internal_to_external[sockfd]);
		external_to_internal[internal_to_external[sockfd]] = -1;
		internal_to_external[sockfd] = -1;
		break;

	    } else if (size < 0) {

		break;

	    } else {

		send(sockfd, buffer, size, 0);
	    }
	}
    }

    return NULL;
}

void *internal_to_external_thread(lg_thread_node_t *self)
{

    int sockfd;
    char buffer[4096] = {0};

    for(;;){

	sem_wait(&internal_to_external_sem);
	pthread_mutex_lock(&internal_to_external_mutex);
	lg_queue_dequeue(&internal_to_external_queue, (void *)&sockfd, sizeof(int));
	pthread_mutex_unlock(&internal_to_external_mutex);

	fprintf(stdout, "[internal_to_external_thread][%d]===>[%d]\n", 
		sockfd, internal_to_external[sockfd]);

	while(1){

	    ssize_t size = recv(sockfd, buffer, 4096, 0);
	    fprintf(stdout, "internal_to_external size=%zd\n", size);
	    if (size == 0) {

		close(sockfd);
		close(internal_to_external[sockfd]);
		external_to_internal[internal_to_external[sockfd]] = -1;
		internal_to_external[sockfd] = -1;
		break;

	    } else if (size < 0) {

		break;

	    } else {

		send(internal_to_external[sockfd], buffer, size, 0);
	    }
	}
    }
}

void internal_event_in(lg_network_epoll_t* ep, int sockfd)
{

    int code = lg_network_check(sockfd);
    fprintf(stdout, "[EPOLLIN]%d:%s\n", code, strerror(code));
    if (code != 0) {

	close(sockfd);
	close(internal_to_external[sockfd]);
	external_to_internal[internal_to_external[sockfd]] = -1;
	internal_to_external[sockfd] = -1;
	return;
    }

    pthread_mutex_lock(&internal_to_external_mutex);
    lg_queue_enqueue(&internal_to_external_queue, (void *)&sockfd, sizeof(int));
    sem_post(&internal_to_external_sem);
    pthread_mutex_unlock(&internal_to_external_mutex);
}

void internal_event_out(lg_network_epoll_t* ep, int sockfd)
{
    int code = lg_network_check(sockfd);
    fprintf(stdout, "[EPOLLOUT]%d:%s\n", code, strerror(code));
    if (code != 0) {

	close(sockfd);
	close(internal_to_external[sockfd]);
	external_to_internal[internal_to_external[sockfd]] = -1;
	internal_to_external[sockfd] = -1;
	return;
    }

    pthread_mutex_lock(&external_to_internal_mutex);
    lg_queue_enqueue(&external_to_internal_queue, (void *)&sockfd, sizeof(int));
    sem_post(&external_to_internal_sem);
    pthread_mutex_unlock(&external_to_internal_mutex);
}

void *internal_epwait_thread(lg_thread_node_t *self)
{

    internal_ep->in = internal_event_in;
    internal_ep->out = internal_event_out;
    for(;;) lg_network_epoll(internal_ep);
}

void *connect_thread(lg_thread_node_t *self)
{

    int external_sockfd = -1;
    int internal_sockfd = -1;

    for(;;){

	sem_wait(external_ep->sem);
	pthread_mutex_lock(external_ep->mutex);
	lg_queue_dequeue(external_ep->queue, (void *)&external_sockfd, sizeof(int));
	pthread_mutex_unlock(external_ep->mutex);

	internal_sockfd = external_to_internal[external_sockfd];
	if (internal_sockfd != -1 && lg_network_check(internal_sockfd) == 0) {

	    fprintf(stdout, "[connect_thread-2][%d]\n", external_sockfd);
	    pthread_mutex_lock(&external_to_internal_mutex);
	    lg_queue_enqueue(&external_to_internal_queue, (void *)&internal_sockfd, sizeof(int));
	    sem_post(&external_to_internal_sem);
	    pthread_mutex_unlock(&external_to_internal_mutex);

	} else {

	    fprintf(stdout, "[connect_thread-1][%d]\n", external_sockfd);
	    internal_sockfd = lg_network_connect(IN_IP, IN_PORT);
	    external_to_internal[external_sockfd] = internal_sockfd;
	    internal_to_external[internal_sockfd] = external_sockfd;
	    lg_network_epoll_event_add(internal_ep, internal_sockfd, EPOLLIN | EPOLLOUT | EPOLLET);
	}
    }
}

void external_event(lg_network_epoll_t* ep, int sockfd)
{

    fprintf(stdout, "[external_event][%d]\n", sockfd);

    if (sockfd == ep->listenfd) {

	struct sockaddr_in addr = {0};
	socklen_t addr_len = sizeof(struct sockaddr_in);

	int sockfd_client = accept4(sockfd, (struct sockaddr *)&addr, &addr_len, SOCK_NONBLOCK | SOCK_CLOEXEC);
	lg_network_epoll_addr_add(ep, sockfd_client, addr);
	lg_network_epoll_event_add(ep, sockfd_client, 0);

    } else {

	pthread_mutex_lock(ep->mutex);
	lg_queue_enqueue(ep->queue, (void *)&sockfd, sizeof(int));
	sem_post(ep->sem);
	pthread_mutex_unlock(ep->mutex);
    }
}

void *external_epwait_thread(lg_thread_node_t *self)
{

    external_ep->in = external_event;
    external_ep->listenfd = lg_network_bind(NULL, EX_PORT);
    lg_network_epoll_event_add(external_ep, external_ep->listenfd, 0);
    for(;;) lg_network_epoll(external_ep);
}

int main(int argc, char *argv[])
{

    if (lg_queue_init(&external_to_internal_queue, 40960) == NULL)
	return -1;

    if (pthread_mutex_init(&external_to_internal_mutex, NULL) != 0)
	return -1;

    if (sem_init(&external_to_internal_sem, 0, 0) != 0)
	return -1;

    if (lg_queue_init(&internal_to_external_queue, 40960) == NULL)
	return -1;

    if (pthread_mutex_init(&internal_to_external_mutex, NULL) != 0)
	return -1;

    if (sem_init(&internal_to_external_sem, 0, 0) != 0)
	return -1;

    if (!(external_ep = lg_network_epoll_init(4000, -1)))
	return -1;

    if (!(internal_ep = lg_network_epoll_init(4000, -1)))
	return -1;

    if (!(th_pool = lg_thread_pool_init(NULL, NULL, NULL, NULL)))
	return -1;

    lg_thread_node_add(th_pool, external_epwait_thread, NULL);
    lg_thread_pool_add(50, th_pool, connect_thread, NULL);

    lg_thread_node_add(th_pool, internal_epwait_thread, NULL);
    lg_thread_pool_add(100, th_pool, external_to_internal_thread, NULL);
    lg_thread_pool_add(100, th_pool, internal_to_external_thread, NULL);

    lg_thread_pool_start(th_pool);
    lg_thread_pool_join(th_pool);

    return 0;
}


