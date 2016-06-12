/*
 * DataTransAlpha.c
 *
 *  Created on: 2008-12-4
 *      Author: robeen
 */

#include "Tp.h"
#include "clinum.h"

void DataTransAlpha(int clifd,int servfd)
{
	int maxfdp,val;
	int n=0,m=0;
	//struct timeval timeout;
	//timeout.tv_sec=3;
	//timeout.tv_usec=0;
	fd_set rset,wset;
	//FD_ZERO(&error);
	int CliData=0,ServData=0;
	//to figure out whether there comes FIN/RST from client or server
	char cli_buf[MAXLINE];
	char serv_buf[MAXLINE];
	char *clifptr,*clibptr,*servfptr,*servbptr;
	clifptr=clibptr=cli_buf;
	servfptr=servbptr=serv_buf; //initial the ptr
	val=fcntl(clifd,F_GETFL,0);
	fcntl(clifd,F_SETFL,val | O_NONBLOCK);
	val=fcntl(servfd,F_GETFL,0);
	fcntl(servfd,F_SETFL,val | O_NONBLOCK); //set the socket as NONBLOCK
	maxfdp=(clifd>=servfd?clifd:servfd )+1;

    for(;;)//loop while each side close the connection
	{
    	FD_ZERO(&rset);
    	FD_ZERO(&wset);

    	if( CliData==0 && clifptr<&cli_buf[MAXLINE])
        	FD_SET( clifd,&rset);

        if( ServData==0 && servfptr<&serv_buf[MAXLINE])
        	FD_SET( servfd,&rset);

        if( clibptr!=clifptr )
        	FD_SET( clifd,&wset );

        if( servbptr!=servfptr )
        	FD_SET( servfd,&wset );

		n=select( maxfdp,&rset,&wset,NULL,NULL );
		if(n<0)	{
			err_ret("select error.");
		}

		if( FD_ISSET(clifd,&rset) ){
			n=read(clifd,clifptr,&cli_buf[MAXLINE]-clifptr);
			if( n<0 ) {
				fprintf(stderr,"Reading from client %d,error occurs,errno is %d\n",clinum,errno);
				if(errno==EWOULDBLOCK || errno==EAGAIN || errno==EINTR){
					continue;
				} else {
					err_ret("cannot read from client %d.",clinum);
					//return;
				}
			}
			else if( n==0 ) {
				fprintf(stdout,"client %d send FIN.\n",clinum);
				CliData=1;
				if(clibptr==clifptr){
					if(shutdown(clifd,SHUT_RD)<0)
						err_info("shutdown client %d read error.",clinum);
				}
			}
			else {
				fprintf(stderr,"Read %d bytes from client %d\n",n,clinum);
				clifptr=clifptr+n;
				FD_SET(servfd,&wset);
			}
		}

		if( FD_ISSET(servfd,&rset) ){
			n=read(servfd,servfptr,&serv_buf[MAXLINE]-servfptr);
			if( n<0 ){
				fprintf(stderr,"Reading from server %d,error occurs,errno is %d\n",clinum,errno);
				if(errno==EWOULDBLOCK || errno==EAGAIN || errno==EINTR){
					continue;
				} else {
					err_ret("cannot read from server %d.",clinum);
				    //return;
				}
			}
			else if( n==0 ){
                fprintf(stdout,"server %d send FIN!\n",clinum);
				ServData=1;
				if(servbptr==servfptr){
					if((shutdown(servfd,SHUT_RD))<0)
						err_info("shutdown server %d read error.",clinum);
				}
			}
			else {
				fprintf(stderr,"Read %d bytes from server %d\n",n,clinum);
				servfptr=servfptr+n;
				FD_SET(clifd,&wset);
			}
		}

		if( FD_ISSET(servfd,&wset) && ((n=clifptr-clibptr)>0) ){
			m=write( servfd,clibptr,n );
			if(m<0){
				err_ret("writen to server %d error,errno is %d\n.",clinum,errno);
				//return;
			} else{
				fprintf(stderr,"write %d bytes to server %d,errno = %d\n",m,clinum,errno);
				clibptr=clibptr+m;
				if(clibptr==clifptr) //if there is no data to handle,push both ptr back to the head
					clibptr=clifptr=cli_buf;
				if(ServData==1){
					if((shutdown(servfd,SHUT_WR))<0)
						err_info("shutdown server %d write error.",clinum);
				}
            }

		}

        if( FD_ISSET(clifd,&wset) && ((n=servfptr-servbptr)>0) ){
			m=write( clifd,servbptr,n );
			if(m<0){
			    err_ret("writen to client %d error.",clinum);
				//return;
			} else{
				fprintf(stderr,"write %d bytes to client %d,errno = %d\n",m,clinum,errno);
				servbptr=servbptr+m;
				if(servbptr==servfptr) //if there is no data to handle,push both ptr back to the head
					servbptr=servfptr=serv_buf;
				if(ServData==1){
					if((shutdown(clifd,SHUT_WR))<0)
						err_info("shutdown client %d write error.",clinum);
				}
			}

		}

        if( CliData==1 && ServData==1 ) return;
   }
}
