
#ifndef	__AD_SYSTEM_NETWORK_H__
#define	__AD_SYSTEM_NETWORK_H__

#ifdef __cplusplus
extern "C" { 
#endif

#include "json.h"
#include "lg_thread.h"
#include "lg_network.h"

    typedef union {

	int sockfd;
	void *arg;

    } ad_system_network_sockfd_t;

    extern int ad_system_network_run();
    extern void ad_system_packet_parse(int sockfd, char *buffer, char *cookies, char *addr);

#ifdef __cplusplus
}
#endif

#endif


