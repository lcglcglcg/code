
#include "lg_network.h"
#include "lg_thread.h"

void demo_in(lg_network_epoll_t* ep, int sockfd)
{

    int code = lg_network_check(sockfd);
    fprintf(stdout, "IN:%d:%s\n", code, strerror(code));
}

void demo_out(lg_network_epoll_t* ep, int sockfd)
{

    int code = lg_network_check(sockfd);
    fprintf(stdout, "OUT:%d:%s\n", code, strerror(code));
}

int main(int argc, char *argv[])
{

    lg_network_epoll_t *ep = lg_network_epoll_init(4000, -1);
    ep->in = demo_in;
    ep->out = demo_out;

    int sockfd = lg_network_connect("192.168.10.150", 8888);
    lg_network_epoll_event_add(ep, sockfd, EPOLLIN | EPOLLOUT | EPOLLET);
    for(;;) lg_network_epoll(ep);

    return 0;
}


