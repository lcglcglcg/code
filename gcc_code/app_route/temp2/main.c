
#include "tcp.h"


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

int main(int argc, char *argv[])
{
     struct epoll_event ev;

    int epfd1 = epoll_create(4096);
    int epfd2 = epoll_create(4096);
    int listenfd = net_listen(80);


    return 0;
}
