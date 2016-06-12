/*
 * Data_Trans.c
 *
 *  Created on: 2008-11-1
 *      Author: robeen
 */

#include "Tp.h"
#include "clinum.h"


void Data_Trans(int clifd,int servfd)
{
	int maxfdp;
	int m=0,n=0;
	//struct timeval timeout;
	//timeout.tv_sec=3;
	//timeout.tv_usec=0;
	fd_set rset;
	FD_ZERO(&rset);
	int CliData=0,ServData=0;
	char cli_buf[MAXLINE];
	char serv_buf[MAXLINE];

	for(;;)
	{
        if( CliData==0 )
			FD_SET( clifd,&rset );
		if( ServData==0 )
			FD_SET( servfd,&rset );

		maxfdp=(clifd>=servfd?clifd:servfd )+1;
		if(select( maxfdp,&rset,NULL,NULL,NULL )<0){
			err_ret("select error.");
			//return;
		}

		if( FD_ISSET(clifd,&rset) ){
			n=read(clifd,cli_buf,MAXLINE);
			if( n<0 ) {
				err_ret("cannot read from client.");
				//return;
			}
			if( n==0 ){
				CliData=1;
				shutdown( servfd,SHUT_WR );
				shutdown( clifd,SHUT_RD );
			}
			if(writen( servfd,cli_buf,n )<0) perror("writen to server error.");
            //memset(cli_buf,0,MAXLINE);
		}

		if( FD_ISSET(servfd,&rset) ){
			m=read(servfd,serv_buf,MAXLINE);
			if( m<0 ){
				perror("cannot read from server.");
				return;
			}
			if( m==0 ){
				ServData=1;
                shutdown( clifd,SHUT_WR );
				shutdown( servfd,SHUT_RD);
				}
			if(writen( clifd,serv_buf,m )<0) perror("writen to client error.");
			memset(serv_buf,'\0',MAXLINE);
			}

         if( CliData==1 && ServData==1 ) {
			return;
		}
	}
}


