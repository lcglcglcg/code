/*
 * err_quit.c
 *
 *  Created on: 2008-11-1
 *      Author: robeen
 */
#include "Tp.h"


//fetal error,which is not related to system call,
//require restart the program.Log the error to syslog.
void err_quit(const char *err_msg,...)
{
    va_list ap;
    va_start(ap,err_msg);
	err_log(0,LOG_ERR,err_msg,ap);
	va_end(ap);
    exit(1);
}

//fetal error,which is related to system call,
//require restart an log error
void err_sys(const char *err_msg,...)
{
	va_list ap;
	va_start(ap,err_msg);
	err_log(1,LOG_ERR,err_msg,ap);
	va_end(ap);
	exit(1);
}

//nonfetal error,Just print the error massage
//we also log it to syslog
void err_info(const char *err_msg,...)
{
	va_list ap;
	va_start(ap,err_msg);
	err_log(1,LOG_INFO,err_msg,ap);
	va_end(ap);
}

//nonfetal error ,for return
void err_ret(const char *err_msg,...)
{
	va_list ap;
	va_start(ap,err_msg);
	err_log(1,LOG_INFO,err_msg,ap);
	va_end(ap);
	return;
}


void err_log(int err_flag,int level,const char *err_msg,va_list ap)
{
	int errno_bak,n;
	char buf[ERR_MAXLINE+1];
	vsprintf(buf,err_msg,ap);
	errno_bak=errno;

	if(err_flag){
	n=strlen(buf);
	snprintf(buf+n,MAXLINE-n,":%s",strerror(errno_bak));
	strcat(buf,"\n");
	}
	syslog(level,buf);
	fputs(buf,stderr);
}


