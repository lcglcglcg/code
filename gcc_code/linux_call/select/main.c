#include "command.h"

int main(int argc, char **argv)
{
    int i, n, maxi, maxfd, connfd, sockfd;
    int nready, client[FD_SETSIZE];
    fd_set rset, allset;
    char *buf = malloc(MAX);

    /*******************************************/
    /*捕获信号*/
    /*******************************************/

    signal(SIGPIPE, sig_usr);
    signal(SIGCHLD, sig_usr);

    /*******************************************/
    /*创建并初始化socket*/
    /*******************************************/

    int listenfd = Socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in	saddr, caddr;
    bzero(&saddr, sizeof(saddr));
    bzero(&caddr, sizeof(caddr));
    saddr.sin_family = AF_INET;
    saddr.sin_addr.s_addr = inet_addr("0.0.0.0");
    saddr.sin_port = htons(PORT);
    socklen_t len = sizeof(saddr);

    /*******************************************/
    /*监听*/
    /*******************************************/

    Bind(listenfd, (struct sockaddr *)&saddr, sizeof(saddr));
    Listen(listenfd, 100);

    /*******************************************/
    /*select*/
    /*******************************************/

    maxfd = listenfd;		
    for (i = 0; i < FD_SETSIZE; i++)
	client[i] = -1;	
    maxi = -1;			
    FD_ZERO(&allset);
    FD_SET(listenfd, &allset);

    /*******************************************/
    /* 设置要监控的集*/
    /*******************************************/

    while(1)
    {
	rset = allset;	
	nready = select(maxfd + 1, &rset, NULL, NULL, NULL);
	if (nready < 0)
	    printf("select error\n");

	/*******************************************/
	/*监听的socket加入到集*/
	/*******************************************/

	if (FD_ISSET(listenfd, &rset)) 
	{ 
	    len = sizeof(caddr);
	    connfd=Accept(listenfd,(struct sockaddr*)&caddr,&len);

	    for (i = 0; i < FD_SETSIZE; i++)
		if (client[i] < 0) 
		{

		    /*******************************************/
		    /* 添加新的socket到监控*/
		    /*******************************************/

		    client[i] = connfd; 
		    break;
		}
	    /*******************************************/
	    /* 是否超过监控数量*/
	    /*******************************************/
	    if (i == FD_SETSIZE) 
	    {
		fputs("too many clients\n", stderr);
		exit(1);
	    }

	    /*******************************************/
	    /*设置新的监控到集 */
	    /*******************************************/

	    FD_SET(connfd, &allset);	

	    /*******************************************/
	    /* for select */
	    /*******************************************/

	    if (connfd > maxfd)
		maxfd = connfd; 

	    /*******************************************/
	    /* max index in client[] array */
	    /*******************************************/

	    if (i > maxi)
		maxi = i;	

	    /*******************************************/
	    /* no more readable descriptors */
	    /*******************************************/

	    if (--nready == 0)
		continue;	
	}

	/*******************************************/
	/* check all clients for data */
	/*******************************************/

	for (i = 0; i <= maxi; i++) 
	{	
	    if ((sockfd = client[i]) < 0)
		continue;

	    /*******************************************/
	    /* connection closed by client */
	    /*******************************************/

	    if (FD_ISSET(sockfd, &rset)) 
	    {
		if ((n = Read(sockfd, buf, MAX)) == 0) 
		{
		    Close(sockfd);
		    FD_CLR(sockfd, &allset);
		    client[i] = -1;
		} else 
		{
		    printf("pid = %d ", getpid());
		    my_web(sockfd, buf);
		    Close(sockfd);
		    FD_CLR(sockfd, &allset);
		    client[i] = -1;
		}

		/*******************************************/
		/* no more readable descriptors */
		/*******************************************/

		if (--nready == 0)
		    break;	
	    }
	}
    }
}

void sig_usr(int sig)
{

    if(SIGCHLD == sig)
    {
	wait(NULL);
	printf("client exit\n");
    }
    else
    {
	printf("sig = %d", sig);
    }
}
