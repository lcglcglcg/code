#include	"Tp.h"

int						/* Write "n" bytes to a descriptor. */
writen(int fd, const void *vptr, int n)
{
	int	nleft;
	int	nwritten;
	const char	*ptr;

	ptr = vptr;
	nleft = n;
	while (nleft > 0) {
		if ( (nwritten = write(fd, ptr, nleft)) <= 0) {
			if (nwritten < 0 && errno == EINTR)
				nwritten = 0;		/* and call write() again */
			else
				return(-1);			/* error */
		}

		nleft-= nwritten;
		ptr+= nwritten;
	}
	return(n);
}

