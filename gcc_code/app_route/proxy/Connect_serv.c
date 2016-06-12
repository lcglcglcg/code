/*
 * Connect_serv.c
 *
 *  Created on: 2008-11-10
 *      Author: robeen
 */
#include "Tp.h"
#include "clinum.h"
void
Connect_Serv(int *sockfd,struct sockaddr_in* servaddr)
{
	int threadID;
	char host[128];
	char *serve;
	//socklen_t servlen;
	threadID=pthread_self();
	fprintf(stdout,"thread %d is created!\n",threadID);
    serve=malloc(32);
	inet_ntop(AF_INET,&(servaddr->sin_addr),host, sizeof(host));
	sprintf(serve, "%d", (ntohs(servaddr->sin_port)));
	*sockfd=tcp_connect(host,serve);

	pthread_mutex_lock(&conp_mutex);
    clinum++;
	pthread_mutex_unlock(&conp_mutex);

    fprintf(stdout,"Client %d is connect to the server %s\n",clinum,host);
	free(serve);
	return;
}

