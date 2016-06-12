#include	"Tp.h"

int
tcp_connect(const char *host, const char *serv)
{
	int n,sockfd;
	struct addrinfo	hints, *res, *ressave;

	memset(&hints, 0,sizeof(struct addrinfo));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;

	if ( (n = getaddrinfo(host, serv, &hints, &res)) != 0)
		fprintf(stderr,"tcp_connect error for %s, %s: %s",
				host, serv, gai_strerror(n));

	ressave = res;

	do {
		sockfd = socket(res->ai_family, res->ai_socktype,
				res->ai_protocol);
		if (sockfd < 0)
			continue;	/* ignore this one */

		int on = 1;
	    if(setsockopt(sockfd,SOL_SOCKET,SO_KEEPALIVE,
	    		(const char *) &on,sizeof(on))== -1)
		   err_info("setsockopt(...,SO_KEEPALIVE,...)");

		if (connect(sockfd, res->ai_addr, res->ai_addrlen) == 0)
			break;		/* success */

		close(sockfd);	/* ignore this one */
	} while ( (res = res->ai_next) != NULL);

	if (res == NULL)	/* errno set from final connect() */
		fprintf(stderr,"tcp_connect error for %s, %s", host, serv);

	freeaddrinfo(ressave);

	return(sockfd);
}

int
tcp_listen(const char *host, const char *serv, socklen_t *addrlenp)
{
	int listenfd, n;
	const int	on = 1;
	struct addrinfo	hints, *res, *ressave;

	memset(&hints,0, sizeof(struct addrinfo));
	hints.ai_flags = AI_PASSIVE;
	//hints.ai_family = AF_UNSPEC;
	hints.ai_family=AF_INET;
	hints.ai_socktype = SOCK_STREAM;

	if ( (n = getaddrinfo(host, serv, &hints, &res)) != 0)
		err_sys("tcp_listen error ");
	ressave = res;

	do {
		listenfd = socket(res->ai_family, res->ai_socktype,
				res->ai_protocol);
		if (listenfd < 0)
			continue;		/* error, try next one */
		setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
		if (bind(listenfd, res->ai_addr, res->ai_addrlen) == 0)
			break;			/* success */

		close(listenfd);
		/* bind error, close and try next one */
	} while ( (res = res->ai_next) != NULL);

	if (res == NULL)
		/* errno from final socket() or bind() */
		err_sys("tcp_listen error" );

	listen(listenfd, LISTENQ);

	if (addrlenp)
		*addrlenp = res->ai_addrlen;
	/* return size of protocol address */

	freeaddrinfo(ressave);

	return(listenfd);
}



