#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <sys/epoll.h>
#define	lg_malloc(size)	malloc(size);

typedef struct
{
    char           	flag[4];
    unsigned int 	seq; 
    unsigned int 	timestamp;     
    unsigned int	msgid;
    unsigned int    	size;

}MESS_HEAD;//Online报文头

typedef struct {

    MESS_HEAD		head;
    char 		account[64];
    unsigned int 	hospital;
    char 		timestring[24];
    unsigned int 	status;
    unsigned int 	system;

}MESS_LOGIN;//Online登录报文

typedef struct
{
    MESS_HEAD       	head;
    unsigned int    	ret;
    char            	msn[64]; 
    char            	qq[64];
    char            	nickname[256];
    char            	nickname2[256];
    char            	faceurl[256];
    int             	style;                     
    char            	FTPAddr[64];    
    int             	FTPPort;                
    char            	FTPUser[24];    
    char            	FTPPsw[24];          
    char            	SvrUrl[64];         
    char            	workerid[16];  
    int             	workage;       
    int             	isShowDoctor;

}MESS_LOGIN_RET;//Online登录返回报文

typedef struct
{
    MESS_HEAD       	head;
    char 		account[64];
    unsigned int 	hospital;
    char 		timestring[24];

}MESS_LOGOUT;//Online登出报文

typedef struct
{
    MESS_HEAD       	head;
    unsigned int    	ret; 

}MESS_LOGOUT_RET;//Online登出报文返回

typedef struct {

    char		nickname[256];
    MESS_LOGIN 		login;
    MESS_LOGIN_RET	login_ret;
    MESS_LOGOUT 	logout;
    MESS_LOGOUT_RET	logoutsc;

}online_t;


online_t *online_init(void)
{

    online_t *this = NULL;
    MESS_LOGIN *login = NULL;
    MESS_LOGOUT *logout = NULL;

    this = lg_malloc(sizeof(online_t));
    bzero(this, sizeof(online_t));


    login = &this->login;
    logout = &this->logout;
    memcpy(this->nickname, "checkacc1", 256);


    memcpy(login->head.flag, "15TO", 4);
    login->head.msgid = 30000;
    login->head.seq = 0;
    login->head.size = 100;
    memcpy(login->account, "7639ba3a-fbc8-4c84-9a03-3d8be40604fe", 64);
    login->hospital = 3410;
    login->system = 1;

    memcpy(logout->head.flag, "15TO", 4);
    logout->head.msgid = 30001;
    logout->head.seq = 0;
    logout->head.size = 100;
    memcpy(logout->account, "7639ba3a-fbc8-4c84-9a03-3d8be40604fe", 64);
    logout->hospital = 3410;

    return this;
}

int online_err(int sockfd)
{

    int err;
    socklen_t len = sizeof(int);
    int retval = getsockopt(sockfd, SOL_SOCKET, SO_ERROR, &err, &len);

    if(retval == -1) {

	fprintf(stdout, "getsockopt:%s\n", strerror(errno));
	close(sockfd);
	return -1;

    } else if (err) {

	fprintf(stdout, "socket:%s\n", strerror(err));
	close(sockfd);
	return -1;
    }

    return 0;
}
int online_err_name(int sockfd)
{

    struct sockaddr_in addr;
    socklen_t len = sizeof(struct sockaddr_in);

    int retval = getsockname(sockfd, (struct sockaddr *)&addr, &len);

    if (retval == -1) {

	online_err(sockfd);
	return -1;
    }

    fprintf(stdout, "socket = %s:%d\n", inet_ntoa(addr.sin_addr), ntohs(addr.sin_port));
    //struct sockaddr_in recvline;
    //fprintf(stdout, "len=%d addr=%s port =%d\n", len,
    //inet_ntop(AF_INET, &addr.sin_addr.s_addr, &recvline, len),
    //ntohs(addr.sin_port)); 

    return 0;
}

int online_run(online_t *data)
{

    int retval;
    MESS_LOGIN *login = &data->login;
    MESS_LOGOUT *logout = &data->logout;
    MESS_LOGIN_RET *login_ret = &data->login_ret;

    struct sockaddr_in addr;
    addr.sin_addr.s_addr = inet_addr("119.161.216.9");
    //addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr.sin_port = htons(5112);
    addr.sin_family = AF_INET;

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    retval = fcntl(sockfd, F_SETFL, fcntl(sockfd, F_GETFD, 0) | O_NONBLOCK);

    int epfd = epoll_create(1);
    struct epoll_event events;

    struct epoll_event temp;
    temp.data.fd = sockfd;
    temp.events = EPOLLOUT | EPOLLERR; 
    epoll_ctl(epfd, EPOLL_CTL_ADD, sockfd, &temp);

    retval = connect(sockfd, (struct sockaddr *)&addr, sizeof(struct sockaddr_in));

    if (retval == -1) {

	switch(errno){

	    case EISCONN:
	    case EINPROGRESS:
		break;

	    default:
		fprintf(stdout, "connect:%s\n", strerror(errno));
		return -1;
	}
    }

    for(;;){

	retval = epoll_wait(epfd, &events, 1, 1000);

	if (retval == -1) {

	    fprintf(stdout, "epoll_wait:%s\n", strerror(errno));
	    return -1;

	} else if (retval == 0) {

	    fprintf(stdout, "TIME OUT\n");
	    return -1;
	}

	if (EPOLLIN & events.events) {

	    fprintf(stdout, "EPOLLIN\n");

	    retval = read(sockfd, login_ret, sizeof(MESS_LOGIN_RET));

	    if(retval == -1) {

		switch(errno){

		    case EINTR:
		    case EAGAIN:
			break;

		    default:
			fprintf(stdout, "read:%s\n", strerror(errno));
			return -1;
		}

	    } else if (retval == 0) {

		fprintf(stdout, "close\n");
		return -1;

	    }

	    fprintf(stdout, "%s:%s\n", data->nickname, login_ret->nickname);
	    close(sockfd);
	    return 0;

	} else if (EPOLLOUT & events.events) {

	    fprintf(stdout, "EPOLLOUT\n");

	    if (online_err_name(events.data.fd) == -1)
		return -1;

	    retval = write(sockfd, login, sizeof(MESS_LOGIN));

	    if(retval == -1) {

		switch(errno){

		    case EINTR:
		    case EAGAIN:
			break;

		    default:
			fprintf(stdout, "write:%s\n", strerror(errno));
			close(sockfd);
			return -1;
		}

	    } else if (retval == 0) {

		fprintf(stdout, "close\n");
		close(sockfd);
		return -1;

	    }

	    temp.data.fd = sockfd;
	    temp.events = EPOLLIN | EPOLLERR; 
	    epoll_ctl(epfd, EPOLL_CTL_MOD, sockfd, &temp);

	} else if (EPOLLERR & events.events) {

	    fprintf(stdout, "EPOLLERR\n");
	    online_err(events.data.fd);
	    close(sockfd);
	    return -1;
	}
    }

    return 0;
}

int main(int argc, char **argv)
{

    online_t *data = online_init();

    for(;;sleep(1))
	online_run(data);

    return 0;
}

