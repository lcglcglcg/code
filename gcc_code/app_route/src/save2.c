
#include "lg_network.h"
#include "lg_thread.h"

#if 10
#define	IP "119.161.216.21"
#define	PORT 3389
#else
#define	IP "192.168.10.150"
#define	PORT 80
#endif

int sockfd_data[1024] = {0};
int sockfd_status[1024] = {0};
lg_network_epoll_t *ep = NULL;

void lg_proxy_close(int sockfd1, int sockfd2)
{

    if (sockfd1 > 0) {

	sockfd_data[sockfd1] = 0;
	sockfd_status[sockfd1] = 0;
	close(sockfd1);
    }

    if (sockfd2 > 0) {

	sockfd_data[sockfd2] = 0;
	sockfd_status[sockfd2] = 0;
	close(sockfd2);
    }
}

void lg_proxy_route(int in_sockfd, int out_sockfd)
{

    ssize_t size;
    char buffer[1024];
    //fprintf(stdout, "[%d]==>[%d]\n", in_sockfd, out_sockfd);

    while(1){

	size = recv(in_sockfd, buffer, 1024, 0);
	if (size == 0) {

	    return lg_proxy_close(in_sockfd, out_sockfd);

	} else if (size < 0) {

	    return;
	}

	send(out_sockfd, buffer, size, 0);
    }
}

void lg_proxy_event_in(lg_network_epoll_t* ep, int sockfd)
{

    if (sockfd == ep->listenfd) {

	int external_sockfd = accept4(sockfd, NULL, NULL, SOCK_NONBLOCK | SOCK_CLOEXEC);
	if (external_sockfd <= 0) {

	    fprintf(stdout, "error accept\n");
	    return;
	}

	fprintf(stdout, "new accept = %d\n", external_sockfd);

	int internal_sockfd = lg_network_connect(IP, PORT);
	if (internal_sockfd <= 0) {

	    fprintf(stdout, "error connect\n");
	    return;
	}

	fprintf(stdout, "new connect = %d\n", internal_sockfd);

	sockfd_status[external_sockfd] = 1;
	sockfd_status[internal_sockfd] = 2;
	sockfd_data[internal_sockfd] = external_sockfd;

	lg_network_epoll_event_add(ep, internal_sockfd, EPOLLOUT);
	lg_network_epoll_event_add(ep, external_sockfd, EPOLLIN);
	return;
    }

    int out_sockfd = 0;
    if ((out_sockfd = sockfd_data[sockfd])) {

	fprintf(stdout, "[EPOLLIN][%d]\n", sockfd);

	if (lg_network_check(sockfd) != 0) {

	    fprintf(stdout, "error EPOLLIN:%d\n", sockfd_status[sockfd]);
	    fprintf(stdout, "error EPOLLIN:%s\n", strerror(errno));
	    return lg_proxy_close(sockfd, out_sockfd);
	}

	lg_proxy_route(sockfd, out_sockfd);
    }
}

void lg_proxy_event_out(lg_network_epoll_t* ep, int sockfd)
{

    fprintf(stdout, "[EPOLLOUT][%d]\n", sockfd);

    int internal_sockfd = sockfd;
    int external_sockfd = sockfd_data[sockfd];
    if (lg_network_check(internal_sockfd) != 0) {

	fprintf(stdout, "error lg_proxy_event_out\n");
	return lg_proxy_close(external_sockfd, internal_sockfd);
    }

    sockfd_data[external_sockfd] = internal_sockfd;
    lg_network_epoll_event_mod(ep, internal_sockfd, EPOLLIN);
    lg_proxy_route(external_sockfd, internal_sockfd);
}

int main(int argc, char *argv[])
{

    memset(sockfd_data, 0, sizeof(sockfd_data));

    if (!(ep = lg_network_epoll_init(1000, -1)))
	return -1;

    ep->listenfd = lg_network_bind(NULL, PORT);
    lg_network_epoll_event_add(ep, ep->listenfd, 0);

    ep->in = lg_proxy_event_in;
    ep->out = lg_proxy_event_out;

    for(;;) lg_network_epoll(ep);
    return 0;
}


