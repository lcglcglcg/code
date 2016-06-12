/* include tcp_connect */
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


