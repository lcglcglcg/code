#ifndef	_TP_H_
#define	_TP_H_

#include <limits.h>
#include <sys/stat.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <errno.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>
#include <sys/select.h>
#include <linux/netfilter_ipv4.h>
#include <netdb.h>
#include <time.h>
#include <stdarg.h>
#include <syslog.h>

//#define MAXCLI 1000
#define LISTENQ 10
#define MAXLINE 4096
#define ERR_MAXLINE 400


//Main part,handling connection
void Connect_Serv(int *,struct sockaddr_in*);
int tcp_connect(const char* , const char* );
int tcp_listen(const char *, const char *, socklen_t *);

//Responsable for Data transmission
//There is four version requiring test
//to see which one can work better
void Data_Trans(int,int);
void DataTransBeta(int,int);
void DataTransAlpha(int,int);
void DataTransTheta(int,int);

//Error handler.
void err_quit(const char *,...);
void err_sys(const char *,...);
void err_info(const char *,...);
void err_ret(const char *,...);
void err_log(int ,int ,const char *,va_list);

int writen(int,const void*,int);
//int readn(int,void*,int);


#endif



