
#include "lg_network.h"

lg_network_epoll_t *lg_network_epoll_init()
{

    if (lg_network_fdmax(NETWORK_EPOLL_FD_MAX))
	return NULL;

    lg_network_epoll_t *ep = NULL;
    if (!(ep = (lg_network_epoll_t *)malloc(sizeof(lg_network_epoll_t))))
	return NULL;

    struct epoll_event *event_pool = NULL;
    if (!(event_pool = (struct epoll_event *)malloc((sizeof(struct epoll_event)) * NETWORK_EPOLL_FD_MAX))) {

	free(ep);
	return NULL;
    }

    struct sockaddr_in *addr_pool = NULL;
    if (!(addr_pool = (struct sockaddr_in *)malloc((sizeof(struct sockaddr_in)) * NETWORK_EPOLL_FD_MAX))) {

	free(ep);
	free(event_pool);
	return NULL;
    }

    bzero(ep, sizeof(lg_network_epoll_t));
    bzero(event_pool, ((sizeof(struct epoll_event)) * NETWORK_EPOLL_FD_MAX));
    bzero(addr_pool, ((sizeof(struct sockaddr_in)) * NETWORK_EPOLL_FD_MAX));

    ep->epfd = epoll_create(NETWORK_EPOLL_FD_MAX);
    if (ep->epfd == NETWORK_ERROR) {

	free(ep);
	free(event_pool);
	free(addr_pool);
	return NULL;
    }

    ep->timer = 0;
    ep->event_pool = event_pool;
    ep->addr_pool = addr_pool;
    ep->arg = NULL;
    return ep;
}

int lg_network_epoll_wait(lg_network_epoll_t *ep)
{

    int n, nfds;
    nfds = epoll_wait(ep->epfd, ep->event_pool, NETWORK_EPOLL_FD_MAX, ep->timer);
    if (nfds == NETWORK_ERROR) {

	if (EINTR == errno) {

	    return NETWORK_SIGNALEINTR;//信号中断

	} else {

	    return NETWORK_ERROR;
	}

    } else if (nfds == 0) {

	return NETWORK_TIMEOUT;//超时
    }

    for (n = 0; n < nfds; ++n) {

	if (ep->event_pool[n].events & EPOLLIN) {

	    int sockfd = ep->event_pool[n].data.fd;
	    if (ep->in)ep->in(ep, sockfd);

	} else if (ep->event_pool[n].events & EPOLLOUT) {

	    int sockfd = ep->event_pool[n].data.fd;
	    if (ep->out)ep->out(ep, sockfd);

	} else if (ep->event_pool[n].events & EPOLLERR) {

	    int sockfd = ep->event_pool[n].data.fd;
	    if (ep->err)ep->err(ep, sockfd);
	    close(sockfd);

	} else if (ep->event_pool[n].events & EPOLLHUP) {

	    int sockfd = ep->event_pool[n].data.fd;
	    if (ep->hup)ep->hup(ep, ep->event_pool[n].data.fd);
	    close(sockfd);

	} else if (ep->event_pool[n].events & EPOLLPRI) {

	    int sockfd = ep->event_pool[n].data.fd;
	    if (ep->pri)ep->pri(ep, sockfd);
	    close(sockfd);
	}
    }

    return NETWORK_SUCCESS;
}

int lg_network_epoll_event_add(lg_network_epoll_t *ep, int sockfd)
{

    if (sockfd <= 0) return -1;
    struct epoll_event event_ctl;
    bzero(&event_ctl, sizeof(struct epoll_event));

    event_ctl.data.fd = sockfd;
    event_ctl.events = EPOLLIN | EPOLLOUT | EPOLLET;
    return epoll_ctl(ep->epfd, EPOLL_CTL_ADD, sockfd, &event_ctl);
}

int lg_network_epoll_event_mod(lg_network_epoll_t *ep, int sockfd, int mask)
{

    if (sockfd <= 0) return -1;
    struct epoll_event event_ctl;
    bzero(&event_ctl, sizeof(struct epoll_event));

    event_ctl.data.fd = sockfd;
    event_ctl.events = mask;
    return epoll_ctl(ep->epfd, EPOLL_CTL_MOD, sockfd, &event_ctl);
}

int lg_network_epoll_event_del(lg_network_epoll_t *ep, int sockfd)
{

    if (sockfd <= 0) return -1;
    struct epoll_event event_ctl;
    bzero(&event_ctl, sizeof(struct epoll_event));

    event_ctl.data.fd = sockfd;
    event_ctl.events = EPOLLIN | EPOLLOUT | EPOLLET;
    return epoll_ctl(ep->epfd, EPOLL_CTL_DEL, sockfd, &event_ctl);
}

void lg_network_epoll_set_function_in(lg_network_epoll_t *ep, lg_network_epoll_function_t *function)
{
    ep->in = function;
}

void lg_network_epoll_set_function_out(lg_network_epoll_t *ep, lg_network_epoll_function_t *function)
{
    ep->out = function;
}

int lg_network_epoll_addr_add(lg_network_epoll_t *ep, int sockfd, struct sockaddr_in addr)
{

    if (sockfd > 0 && sockfd < NETWORK_EPOLL_FD_MAX) {

	ep->addr_pool[sockfd] = addr;
	return NETWORK_SUCCESS;
    }

    return NETWORK_ERROR;
}

char *lg_network_epoll_addr_get(lg_network_epoll_t *ep, int sockfd, char *addr_str)
{
    if (sockfd > 0 && sockfd < NETWORK_EPOLL_FD_MAX) {

	struct in_addr addr = ep->addr_pool[sockfd].sin_addr;
	inet_ntop(AF_INET, &addr, addr_str, 16);
	return addr_str;
    }

    return NULL;
}

int lg_network_accept(lg_network_epoll_t *ep, int listenfd)
{

    struct sockaddr_in addr;
    socklen_t addr_len = sizeof(struct sockaddr_in);
    int sockfd = accept4(listenfd, (struct sockaddr *)&addr, &addr_len, SOCK_NONBLOCK | SOCK_CLOEXEC);
    if (sockfd == NETWORK_ERROR) {

	switch (errno) {

	    case EAGAIN:
	    case EINTR:
	    case EPERM:
	    case EMFILE: 
		//return NETWORK_SUCCESS;

	    case EBADF:
	    case EFAULT:
	    case EINVAL:
	    case ENFILE:
	    case ENOMEM:
	    default:
		return NETWORK_ERROR;
	}
    }

    if (lg_network_not_time_wait(sockfd)) {

	close(sockfd);
	return NETWORK_ERROR;
    }

    if (ep) {

	lg_network_epoll_event_add(ep, sockfd);
	lg_network_epoll_addr_add(ep, sockfd, addr);
    }

    return sockfd;
}

int lg_network_connect(lg_network_epoll_t *ep, char *address, int port)
{

    if (ep) {
	if (!ep->in || !ep->out)
	    return NETWORK_ERROR;
    }

    struct sockaddr_in addr;
    bzero(&addr, sizeof(addr));

    addr.sin_addr.s_addr = inet_addr(address);
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);

    int sockfd = -1;
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == NETWORK_ERROR)
	return NETWORK_ERROR;

    if (lg_network_noblocking(sockfd) == NETWORK_ERROR) {

	close(sockfd);
	return NETWORK_ERROR;
    }

    if (lg_network_not_time_wait(sockfd)) {

	close(sockfd);
	return NETWORK_ERROR;
    }

    if (connect(sockfd, (struct sockaddr *)&addr, sizeof(addr)) == 0) {

	if (ep) lg_network_epoll_event_add(ep, sockfd);
	return sockfd;
    }

    if (errno == EINPROGRESS) {

	if (ep) lg_network_epoll_event_add(ep, sockfd);
	return sockfd;
    }

    close(sockfd);
    return NETWORK_ERROR;
}

ssize_t lg_network_send(int sockfd, void *buffer, size_t buffer_size)
{

    ssize_t size = write(sockfd, buffer, buffer_size);
    if (size == NETWORK_ERROR) {

	if (errno == EINTR || errno == EAGAIN)
	    return NETWORK_SUCCESS;

	if (lg_network_check(sockfd) == NETWORK_SUCCESS)
	    return NETWORK_SUCCESS;

	close(sockfd);
	return NETWORK_ERROR;

    } else if (size == 0) {

	close(sockfd);
	return NETWORK_ERROR;
    }

    return size;
}

ssize_t lg_network_recv(int sockfd, void *buffer, size_t buffer_size)
{

    ssize_t size = read(sockfd, buffer, buffer_size);
    if (size == NETWORK_ERROR) {

	if (errno == EINTR || errno == EAGAIN)
	    return NETWORK_SUCCESS;

	if (lg_network_check(sockfd) == NETWORK_SUCCESS)
	    return NETWORK_SUCCESS;

	close(sockfd);
	return NETWORK_ERROR;

    } else if (size == 0) {

	close(sockfd);
	return NETWORK_ERROR;
    }

    return size;
}

int lg_network_fdmax(int fdmax)
{

    struct rlimit rt;
    bzero(&rt, sizeof(struct rlimit));
    rt.rlim_max = rt.rlim_cur = fdmax;
    return setrlimit(RLIMIT_NOFILE, &rt);
}

int lg_network_noblocking(int sockfd)
{

    int opts = -1;
    if ((opts = fcntl(sockfd, F_GETFD, 0)) == NETWORK_ERROR)
	return NETWORK_ERROR;

    if ((fcntl(sockfd, F_SETFL, opts | O_NONBLOCK)) == NETWORK_ERROR)
	return NETWORK_ERROR;

    return NETWORK_SUCCESS;
}

int lg_network_check(int sockfd)
{

    int error = -1;
    socklen_t len = sizeof(int);
    if (getsockopt(sockfd, SOL_SOCKET, SO_ERROR, &error, &len) == NETWORK_ERROR)
	return NETWORK_ERROR;

    return error;
}

int lg_network_not_time_wait(int sockfd)
{

    struct linger opt;
    opt.l_onoff = 0x01;
    opt.l_linger = 0x00;
    return setsockopt(sockfd, SOL_SOCKET, SO_LINGER, &opt, sizeof(struct linger));
}

int lg_network_set_kernel_buffer(int sockfd, int send_size, int recv_size)
{

    int size;
    if ((size = send_size))
	if (setsockopt(sockfd, SOL_SOCKET, SO_SNDBUF, &size, sizeof(int)) == NETWORK_ERROR)
	    return NETWORK_ERROR;

    if ((size = recv_size))
	if (setsockopt(sockfd, SOL_SOCKET, SO_RCVBUF, &size, sizeof(int)) == NETWORK_ERROR)
	    return NETWORK_ERROR;

    return NETWORK_SUCCESS;
}

int lg_network_get_kernel_buffer(int sockfd, int *send_size, int *recv_size)
{

    socklen_t len = sizeof(int);
    if (getsockopt(sockfd, SOL_SOCKET, SO_SNDBUF, send_size, &len) == NETWORK_ERROR)
	return NETWORK_ERROR;

    if (getsockopt(sockfd, SOL_SOCKET, SO_RCVBUF, recv_size, &len) == NETWORK_ERROR)
	return NETWORK_ERROR;

    return NETWORK_SUCCESS;
}

int lg_network_bind(char *address, int port)
{

    int sockfd = -1;
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == NETWORK_ERROR)
	return NETWORK_ERROR;

    int	flag = 0x01;
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(int)) == NETWORK_ERROR) {

	close(sockfd);
	return NETWORK_ERROR;
    }

    if (lg_network_noblocking(sockfd) == NETWORK_ERROR) {

	close(sockfd);
	return NETWORK_ERROR;
    }

    struct sockaddr_in addr;
    bzero(&addr, sizeof(addr));
    if (address)addr.sin_addr.s_addr = inet_addr(address);
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    if (bind(sockfd, (struct sockaddr *)&addr, sizeof(addr)) == NETWORK_ERROR) {

	close(sockfd);
	return NETWORK_ERROR;
    }

    return sockfd;
}

int lg_network_listen(int sockfd, int backlog)
{
    if (listen(sockfd, backlog) == NETWORK_ERROR) {

	close(sockfd);
	return NETWORK_ERROR;
    }

    return sockfd;
}


