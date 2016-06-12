
#include "lg_network.h"
#include "lg_thread.h"

typedef struct {

    int epfd;
    int listenfd;

    int events;
    void (*callback)(int epfd, int sockfd, uint32_t events, void *arg);

}lg_epoll_t;

int net_listen(int prot)
{

    int on = 1;
    struct sockaddr_in addr;
    bzero(&addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(80);

    int listenfd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
    bind(listenfd, (struct sockaddr *)&addr, sizeof(struct sockaddr_in));
    listen(listenfd, 1000);
    return listenfd;
}

void net_epoll_event_add(int epfd, int sockfd, int mask)
{

    struct epoll_event temp;
    temp.events = EPOLLIN;
    epoll_ctl(epfd, EPOLL_CTL_ADD, sockfd, &temp);

}

void net_epoll_event_loop(int epfd, int size)
{

    int i, nfds;
    struct epoll_event ev[size];

    nfds = epoll_wait(epfd, ev, size, -1);
    for (i = 0; i < nfds; ++i) {

    }
}

int main(int argc, char *argv[])
{

    int epfd = epoll_create(4096);
    int listenfd = net_listen(80);

    net_epoll_event_add(epfd, listenfd, EPOLLIN);

    return 0;
}



