
#ifndef _NETWORK_H__
#define	_NETWORK_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <windows.h>
#include "json.h"

#define	CLICK_SERVER_HOST "lcg.leechat.com"
#define	CLICK_SERVER_PORT 6001

    typedef struct {

	wchar_t url_str[2048];
	wchar_t link_str[2048];
	wchar_t cookie_str[2048];
	int delay_time;

    }task_info_t;

    extern SOCKET network_connect(char *ip_addr, int port);
    extern int network_packet_click_send(SOCKET sockfd, task_info_t *task_info);
    extern int network_packet_click_recv(SOCKET sockfd, task_info_t *task_info);

#ifdef __cplusplus
}
#endif

#endif
