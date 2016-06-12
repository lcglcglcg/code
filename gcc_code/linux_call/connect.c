#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <asm/ioctls.h>
#include <errno.h>
#include <sys/select.h>
#include <fcntl.h>
#include <sys/epoll.h>

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
    MESS_HEAD       head;
    unsigned int    ret;
    char            msn[64]; 
    char            qq[64];
    char            nickname[256];
    char            nickname2[256];
    char            faceurl[256];
    int             style;                     
    char            FTPAddr[64];    
    int             FTPPort;                
    char            FTPUser[24];    
    char            FTPPsw[24];          
    char            SvrUrl[64];         
    char            workerid[16];  
    int             workage;       
    int             isShowDoctor;

}MESS_SC_LOGIN;//Online登录返回报文

typedef struct
{
    MESS_HEAD       head;
    char 		account[64];
    unsigned int 	hospital;
    char 		timestring[24];

}MESS_CS_LOGOUT;//Online登出报文

typedef struct
{
    MESS_HEAD       head;
    unsigned int    ret; 

}MESS_SC_LOGOUT;//Online登出报文返回

typedef struct {

    char		nickname[256];
    MESS_LOGIN		login;
    MESS_SC_LOGIN	loginsc;
    MESS_CS_LOGOUT 	logout;
    MESS_SC_LOGOUT	logoutsc;

}online_check_t;

online_check_t *online_check_init(void)
{

    online_check_t *this = malloc(sizeof(online_check_t));
    bzero(this, sizeof(online_check_t));
    memcpy(this->nickname, "checkacc1", 256);
    memcpy(this->nickname, "checkacc2", 256);

    MESS_LOGIN *login = &this->login;
    MESS_CS_LOGOUT *logout = &this->logout;

    memcpy(login->head.flag, "15TO", 4);
    login->head.msgid = 30000;
    login->head.seq = 0;
    login->head.timestamp = time(NULL);
    login->head.size = 100;
    memcpy(login->account, "7639ba3a-fbc8-4c84-9a03-3d8be40604fe", 64);
    login->hospital = 3410;
    login->system = 1;

    memcpy(logout->head.flag, "15TO", 4);
    logout->head.msgid = 30001;
    logout->head.seq = 0;
    logout->head.timestamp = time(NULL);
    logout->head.size = 100;
    memcpy(logout->account, "7639ba3a-fbc8-4c84-9a03-3d8be40604fe", 64);
    logout->hospital = 3410;

    return this;
}

int online_check_run(online_check_t *data)
{


    int retval, size, n;
    MESS_LOGIN *login = &data->login;
    MESS_SC_LOGIN *loginsc = &data->loginsc;
    MESS_CS_LOGOUT *logout = &data->logout;

    struct sockaddr_in addr;
    addr.sin_family=AF_INET; 
    addr.sin_port = htons(5112);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    //addr.sin_addr.s_addr = inet_addr("119.161.216.8");

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    int flags = fcntl(sockfd, F_GETFL, 0);
    fcntl(sockfd, F_SETFL, flags | O_NONBLOCK);

    int epfd = epoll_create(2);
    struct epoll_event events[2];

    struct epoll_event temp;
    temp.data.fd = sockfd;
    temp.events = EPOLLOUT | EPOLLET | EPOLLERR;
    epoll_ctl(epfd, EPOLL_CTL_ADD, sockfd, &temp);

    retval = connect(sockfd, (struct sockaddr *)&addr, sizeof(addr));

    if (retval == -1) {

	switch (errno) {

	    case EISCONN:
	    case EINPROGRESS:
		break;

	    default:
		perror("connect");
		return -1;
	} 

    } else if (retval == 0) {

	printf("SUCCESS\n");
	temp.data.fd = sockfd;
	temp.events = EPOLLOUT | EPOLLIN | EPOLLERR;
	epoll_ctl(epfd, EPOLL_CTL_MOD, sockfd, &temp);
    }

    for(;;) {

	retval = epoll_wait(epfd, events, 2, 1000);

	if (retval == -1) {

	    perror("epoll_wait");
	    close(sockfd);
	    return -1;

	} else if (retval == 0) {

	    printf("timeout\n");
	    close(sockfd);
	    return -1;
	}

	for(n = 0; n < retval; ++n) {

	    if (events[n].events & EPOLLIN) {

		printf("recv\n");
		size = read(sockfd, loginsc, sizeof(MESS_SC_LOGIN));

		if (size == -1) {

		    switch (errno) {

			case EINTR:
			case EAGAIN:
			    break;

			default:
			    perror("read");
			    close(sockfd);
			    return -1;
		    }

		} else if (size == 0) {

		    printf("read close\n");
		    close(sockfd);
		    return -1;
		}

		printf("%s:%s\n", loginsc->nickname, data->nickname);
		if (strcmp(loginsc->nickname, data->nickname) == 0) {

		    size = write(sockfd, logout, sizeof(MESS_CS_LOGOUT));
		    close(sockfd);
		    return 0;
		}

	    } else if (events[n].events & EPOLLOUT) {

		printf("send\n");

		int error = 0;
		socklen_t len = sizeof(int);

		if ((getsockopt(sockfd, SOL_SOCKET, SO_ERROR, &error, &len)) == 0) {

		    if(error == 0) {

			size = write(sockfd, login, sizeof(MESS_LOGIN)); 

			if (size == -1) {

			    switch (errno) {

				case EINTR:
				case EAGAIN:
				    break;

				default:
				    perror("write");
				    close(sockfd);
				    return -1;
			    }

			} else if (size == sizeof(MESS_LOGIN)) {

			    temp.data.fd = sockfd;
			    temp.events = EPOLLIN | EPOLLET | EPOLLERR;
			    temp.events = EPOLLIN | EPOLLERR;
			    epoll_ctl(epfd, EPOLL_CTL_MOD, sockfd, &temp);
			}
		    }
		}

	    } else if (events[n].events & EPOLLERR) {

		perror("EPOLLERR");
		//return -1;
	    }
	}
    }
}

int main(int argc, char** argv)
{

    online_check_t *data = online_check_init();
    online_check_run(data);
    return 0;
}


