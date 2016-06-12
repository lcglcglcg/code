
#ifndef	__PACKET_H__
#define	__PACKET_H__

#include "json.h"
#include "lg_string.h"
#include "lg_network.h"

#define	DB_HOST "127.0.0.1"
#define	DB_USER "root"
#define	DB_PASS "lcglcg"
#define	DB_BASE "web_click"

extern void network_packet_click_parse(int sockfd, char *buffer, char *ip_addr);
extern void network_packet_manager_parse(int sockfd, char *buffer, char *click_ipaddr);
extern void network_packet_click_to_sql(char *url_str, char *link_str, char *cookie_str, char *addr_str);

#endif


