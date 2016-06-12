/*
 * DataTracsBeta.c
 *
 *  Created on: 2008-12-3
 *      Author: robeen
 */
#include "Tp.h"
#include "clinum.h"

void DataTransBeta(int clifd,int servfd)
{
	int maxfdp;
	int n=0,m=0;
	//struct timeval timeout;
	//timeout.tv_sec=3;
	//timeout.tv_usec=0;
	fd_set rset,wset;
	FD_ZERO(&rset);
	FD_ZERO(&wset);
	int CliData=0,ServData=0;
	int CliExit=0,ServExit=0;
	//flag set to figure out whether client or server close connection(send FIN)
	char cli_buf[MAXLINE];
	char serv_buf[MAXLINE];
	maxfdp=(clifd>=servfd?clifd:servfd )+1;

	for(;;)//loop while each side close the connection
	{
        if( CliExit==0 && CliData==0)
        	FD_SET( clifd,&rset);

        if( CliData>0 )
        	FD_SET( servfd,&wset );

        if( ServExit==0 && ServData==0)
        	FD_SET( servfd,&rset);

        if( ServData>0 )
        	FD_SET( clifd,&wset );

        n=select( maxfdp,&rset,&wset,NULL,NULL );
		if(n<0)	{
			perror("select error.");
			return;
		}

		if( FD_ISSET(clifd,&rset) ){
			n=read(clifd,cli_buf,MAXLINE);
			if( n<0 ) {
				if(errno != EWOULDBLOCK){
				   perror("cannot read from client.");
				   return;
				}
			}
			if( n==0 ) {
				CliExit=1;
				if((shutdown(clifd,SHUT_RD))<0)
					perror("shutdown client read error.");
				    shutdown(servfd,SHUT_WR);
			} else
			{
				CliData=n;
				//FD_SET(servfd,&wset);
			}
		}

		if( FD_ISSET(servfd,&wset) ){
			m=writen( servfd,cli_buf,CliData );
			if(m<0){
				perror("writen to server error.");
				return;
			} else {
				CliData=0;
				/*if(CliExit){
					if((shutdown(servfd,SHUT_WR))<0)
						perror("shutdown server write error.");
				}*/
			}

		}

            //memset(cli_buf,0,MAXLINE);


		if( FD_ISSET(servfd,&rset) ){
			n=read(servfd,serv_buf,MAXLINE);
			if( n<0 ){
				if(errno != EWOULDBLOCK){
				   perror("cannot read from server.");
				   return;
				}
				else continue;
			}
			if( n==0 ){
				ServExit=1;
				fprintf(stdout,"Find FIN from server!\n");

				if((shutdown(servfd,SHUT_RD))<0)
					perror("shutdown server read error.");
				    shutdown(clifd,SHUT_WR);
			} else
				ServData=n;
			    //FD_SET(clifd,&wset);
		}

		if( FD_ISSET(clifd,&wset) ){
			m=writen( clifd,serv_buf,ServData );
			if( m<0 ){
				perror("writen to client error.");
				return;
		    } else{
		    	ServData=0;
		    	/*if(ServExit){
		    		if((shutdown(clifd,SHUT_WR))<0)
		    			perror("shutdown client write error.");
		    	}*/
		    }
		}

         if( CliExit==1 && ServExit==1 ) return;


	}
}
