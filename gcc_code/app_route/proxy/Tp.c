/*
 * Tp.c
 *
 *  Created on: 2008-10-29
 *      Author: robeen && children
 */


#include "Tp.h"


int clinum=0;

//Set the max client number we could bear
int Max_cli=0;

//Set to adjust the condition of the network
//0-----Not busy now
//1-----busy now
int Busy_Option=1;

pthread_mutex_t conp_mutex=PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t conn_mutex=PTHREAD_MUTEX_INITIALIZER;

static void* Connection(void*);
//void* Connection(int);


int
main(int argc, char **argv)
{
    int connfd,sockfd;
    int n=0;

    socklen_t addrlen;
	pthread_t Clitid;

    if(argc!=3 && argc!=4)
		err_quit("user massage: ./Tproxy <port> <Max_CLient_Num> <Busy_Option>");
    if(argc==3)
    	fprintf(stdout,"Use default Busy Option,Working at Busy Mode.\n");
    Max_cli=atoi(argv[2]);
    Busy_Option=atoi(argv[3]);
    if(Busy_Option!=0 && Busy_Option!=1)
    	err_quit("You set a wrong Busy Option.");

    sockfd=tcp_listen(NULL,argv[1],&addrlen);
    fprintf(stdout,"listening on %s\n",argv[1]);

	for(;;){
		if(clinum<Max_cli){
			if(clinum>=Max_cli*0.8) n=(random()%4);
			if(n==0){
				connfd=accept(sockfd,NULL,0);
				if(connfd<0) {
					err_info("accept error.\n");
				    continue;
				}
				pthread_create(&Clitid,NULL,Connection,(void*)connfd);
			}
    	}
		else sleep(n);
    }

	return 0;
}


void*
Connection(void* arg)
{
	pthread_detach(pthread_self());
	int clifd,servfd;
	clifd=(int)(arg);
	struct sockaddr_in* servaddr;
	socklen_t servlen;
	servlen=sizeof(*servaddr);
	servaddr=malloc(servlen);
	memset(servaddr,0,servlen);

	//////////////////////////////////////////////////////////////////////////////////////////
	if ( (getsockopt(clifd,SOL_IP,SO_ORIGINAL_DST,servaddr,&servlen )) != 0 ){
		//perror("getsockopt SO_ORIGINAL_DST:");
		close(clifd);
		//return NULL;
		err_ret("Could not get original destination.");
	}
	//////////////////////////////////////////////////////////////////////////////////////////

    Connect_Serv(&servfd,servaddr);
    if(Busy_Option)
    	DataTransAlpha(clifd,servfd);
    else
    	Data_Trans(clifd,servfd);

    close(servfd);
    close(clifd);

    fprintf(stdout,"Client %d finish connection with the server!\n",clinum);

    /*pthread_mutex_lock(&conn_mutex);
	clinum--;
	pthread_mutex_unlock(&conn_mutex);*/
	free(servaddr);

    return NULL;
}











