
#include "network.h"

int network_fdmax(int fdmax)
{

    struct rlimit rt;
    bzero(&rt, sizeof(struct rlimit));
    rt.rlim_max = rt.rlim_cur = fdmax;
    return setrlimit(RLIMIT_NOFILE, &rt);
}

int network_noblocking(int sockfd)
{

    int opts = -1;
    if ((opts = fcntl(sockfd, F_GETFD, 0)) == NETWORK_ERROR)
	return NETWORK_ERROR;

    if ((fcntl(sockfd, F_SETFL, opts | O_NONBLOCK)) == NETWORK_ERROR)
	return NETWORK_ERROR;

    return NETWORK_SUCCESS;
}

int network_check(int sockfd)
{

    int error = -1;
    socklen_t len = sizeof(int);
    if (getsockopt(sockfd, SOL_SOCKET, SO_ERROR, &error, &len) == NETWORK_ERROR)
	return NETWORK_ERROR;

    return error ? error : NETWORK_SUCCESS;
}

int network_not_time_wait(int sockfd)
{

    struct linger opt;
    opt.l_onoff = 0x01;
    opt.l_linger = 0x00;
    if (setsockopt(sockfd, SOL_SOCKET, SO_LINGER, &opt, sizeof(struct linger)) == NETWORK_ERROR) {

	fprintf(stdout, "[setsockopt SO_LINGER][%s]\n", strerror(errno));
	return NETWORK_ERROR;
    }

    return NETWORK_SUCCESS;
}

int network_set_kernel_buffer(int sockfd, int send_size, int recv_size)
{

    int size;

    if ((size = send_size)) {
	if (setsockopt(sockfd, SOL_SOCKET, SO_SNDBUF, &size, sizeof(int)) == NETWORK_ERROR) {

	    fprintf(stdout, "[setsockopt SO_SNDBUF][%s]\n", strerror(errno));
	    return NETWORK_ERROR;
	}
    }

    if ((size = recv_size)) {
	if (setsockopt(sockfd, SOL_SOCKET, SO_RCVBUF, &size, sizeof(int)) == NETWORK_ERROR) {

	    fprintf(stdout, "[setsockopt SO_RCVBUF][%s]\n", strerror(errno));
	    return NETWORK_ERROR;
	}
    }

    return NETWORK_SUCCESS;
}

int network_get_kernel_buffer(int sockfd, int *send_size, int *recv_size)
{

    socklen_t len = sizeof(int);
    if (getsockopt(sockfd, SOL_SOCKET, SO_SNDBUF, send_size, &len) == NETWORK_ERROR)
	return NETWORK_ERROR;

    if (getsockopt(sockfd, SOL_SOCKET, SO_RCVBUF, recv_size, &len) == NETWORK_ERROR)
	return NETWORK_ERROR;

    return NETWORK_SUCCESS;
}

int network_bind(char *address, int port)
{

    int sockfd = -1;
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == NETWORK_ERROR) {

	fprintf(stdout, "[socket][%s]\n", strerror(errno));
	return NETWORK_ERROR;
    }

    int	flag = 0x01;
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(int)) == NETWORK_ERROR) {

	fprintf(stdout, "[setsockopt SO_REUSEADDR][%s]\n", strerror(errno));
	close(sockfd);
	return NETWORK_ERROR;
    }

    if (network_noblocking(sockfd) == NETWORK_ERROR) {

	fprintf(stdout, "[network_noblocking][%s]\n", strerror(errno));
	close(sockfd);
	return NETWORK_ERROR;
    }

    struct sockaddr_in addr;
    bzero(&addr, sizeof(addr));
    if (address)addr.sin_addr.s_addr = inet_addr(address);
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    if (bind(sockfd, (struct sockaddr *)&addr, sizeof(addr)) == NETWORK_ERROR) {

	fprintf(stdout, "[bind][%s]\n", strerror(errno));
	close(sockfd);
	return NETWORK_ERROR;
    }

    if (listen(sockfd, NETWORK_MAX_BACKLOG) == NETWORK_ERROR) {

	fprintf(stdout, "[listen][%s]\n", strerror(errno));
	close(sockfd);
	return NETWORK_ERROR;
    }

    return sockfd;
}
int network_accept(int listenfd)
{

    //int sockfd = accept(listenfd, NULL, NULL);
    int sockfd = accept4(listenfd, NULL, NULL, SOCK_NONBLOCK | SOCK_CLOEXEC);
    if (sockfd == NETWORK_ERROR) {

	switch (errno) {

	    case EAGAIN:
	    case EINTR:
	    case EPERM:
	    case EMFILE: 
		return NETWORK_SUCCESS;

	    case EBADF:
	    case EFAULT:
	    case EINVAL:
	    case ENFILE:
	    case ENOMEM:
	    default:
		return NETWORK_ERROR;
	}
    }

    //network_noblocking(sockfd);
    return sockfd;
}

int network_connect(char *address, int port)
{
    return NETWORK_SUCCESS;
}

size_t network_send(int sockfd, void *buffer, size_t buffer_size)
{

    size_t size = write(sockfd, buffer, buffer_size);
    if (size == NETWORK_ERROR) {

	if (errno == EINTR || errno == EAGAIN)
	    return NETWORK_SUCCESS;

	if (network_check(sockfd) == NETWORK_SUCCESS)
	    return NETWORK_SUCCESS;

	//close(sockfd);    
	return NETWORK_ERROR;

    } else if (size == 0) {

	//close(sockfd);    
	return NETWORK_ERROR;
    }

    return size;
}

size_t network_recv(int sockfd, void *buffer, size_t buffer_size)
{

    size_t size = read(sockfd, buffer, buffer_size);
    if (size == NETWORK_ERROR) {

	if (errno == EINTR || errno == EAGAIN)
	    return NETWORK_SUCCESS;

	if (network_check(sockfd) == NETWORK_SUCCESS)
	    return NETWORK_SUCCESS;

	//close(sockfd);    
	return NETWORK_ERROR;

    } else if (size == 0) {

	//close(sockfd);    
	return NETWORK_ERROR;
    }

    return size;
}

int network_epoll_event_add(network_epoll_t *ep, int sockfd, int mask)
{

    ep->event_ctl.data.fd = sockfd;
    ep->event_ctl.events = mask ? mask : EPOLLIN;
    return epoll_ctl(ep->epfd, EPOLL_CTL_ADD, sockfd, &ep->event_ctl);
}

int network_epoll_event_mod(network_epoll_t *ep, int sockfd, int mask)
{

    ep->event_ctl.data.fd = sockfd;
    ep->event_ctl.events = mask ? mask : EPOLLIN;
    return epoll_ctl(ep->epfd, EPOLL_CTL_MOD, sockfd, &ep->event_ctl);
}

int network_epoll_event_del(network_epoll_t *ep, int sockfd, int mask)
{

    ep->event_ctl.data.fd = sockfd;
    ep->event_ctl.events = mask ? mask : EPOLLIN;
    return epoll_ctl(ep->epfd, EPOLL_CTL_DEL, sockfd, &ep->event_ctl);
}

int network_epoll(network_epoll_t *ep)
{

    int nfds = epoll_wait(ep->epfd, ep->event_pool, ep->event_pool_max, ep->timer);
    if (nfds == NETWORK_ERROR) {

	if (EINTR == errno) {

	    ep->error = errno;
	    return NETWORK_SIGNALEINTR;//信号中断

	} else {

	    ep->error = errno;
	    return NETWORK_ERROR;
	}

    } else if (nfds == 0) {

	return NETWORK_TIMEOUT;//超时
    }

    int n;
    for (n = 0; n < nfds; ++n) {

	if (ep->event_pool[n].events & EPOLLIN) {

	    int sockfd = ep->event_pool[n].data.fd;
	    if (ep->in)ep->in(sockfd, ep->arg);

	} else if (ep->event_pool[n].events & EPOLLOUT) {

	    int sockfd = ep->event_pool[n].data.fd;
	    if (ep->out)ep->out(sockfd, ep->arg);

	} else if (ep->event_pool[n].events & EPOLLERR) {

	    int sockfd = ep->event_pool[n].data.fd;
	    if (ep->err)ep->err(sockfd, ep->arg);

	    network_epoll_event_del(ep, sockfd, 0);
	    close(sockfd);

	} else if (ep->event_pool[n].events & EPOLLHUP) {

	    int sockfd = ep->event_pool[n].data.fd;
	    if (ep->hup)ep->hup(sockfd, ep->arg);

	    network_epoll_event_del(ep, sockfd, 0);
	    close(sockfd);

	} else if (ep->event_pool[n].events & EPOLLPRI) {

	    int sockfd = ep->event_pool[n].data.fd;
	    if (ep->pri)ep->pri(sockfd, ep->arg);

	    network_epoll_event_del(ep, sockfd, 0);
	    close(sockfd);
	}
    }

    return NETWORK_SUCCESS;
}

network_epoll_t *network_epoll_init(int event_max, int pool_max, int timer)
{

    network_epoll_t *ep = NULL;
    if (!(ep = malloc(sizeof(network_epoll_t)))) {

	fprintf(stdout, "[malloc][%s]\n", strerror(errno));
	return NULL;
    }

    struct epoll_event *event_pool = NULL;
    if (!(event_pool = malloc((sizeof(struct epoll_event)) * pool_max))) {

	fprintf(stdout, "[malloc][%s]\n", strerror(errno));
	free(ep);
	return NULL;
    }

    bzero(ep, sizeof(network_epoll_t));
    bzero(event_pool, ((sizeof(struct epoll_event)) * pool_max));
    ep->timer = timer;
    ep->event_pool = event_pool;
    ep->event_pool_max = pool_max;
    ep->epfd = epoll_create(event_max);

    if (ep->epfd == NETWORK_ERROR) {

	fprintf(stdout, "[epoll_create][%s]\n", strerror(errno));
	free(ep);
	free(event_pool);
	return NULL;
    }

    return ep;
}




