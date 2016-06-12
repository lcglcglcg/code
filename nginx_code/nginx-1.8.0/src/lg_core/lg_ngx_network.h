
#ifndef __LG_NGX_NETWORK_H__
#define __LG_NGX_NETWORK_H__

#include <nginx.h>
#include <ngx_core.h>
#include <ngx_http.h>

#ifdef __cplusplus
extern "C" { 
#endif

    typedef struct {

	ngx_chain_t *head;
	ngx_chain_t *next;
	ngx_str_t content_type;
	size_t content_length;

    } lg_ngx_network_t;

    extern ngx_chain_t *lg_ngx_network_buffer_to_chain_r(ngx_http_request_t *r, const char *p_buffer, size_t size);
    extern ngx_chain_t *lg_ngx_network_buffer_to_chain(ngx_http_request_t *r, const char *p_buffer, size_t size);

    extern int lg_ngx_network_buffer_add(ngx_http_request_t *r, lg_ngx_network_t *network, const char *p_buffer, size_t size);

    extern int lg_ngx_network_send(ngx_http_request_t *r, lg_ngx_network_t *p);
    extern void lg_ngx_network_post_send(ngx_http_request_t *r, lg_ngx_network_t *p);

    extern int lg_ngx_network_buffer_send(ngx_http_request_t *r, lg_ngx_network_t *network, const char *p_buffer, size_t size);
    extern void lg_ngx_network_buffer_post_send(ngx_http_request_t *r, lg_ngx_network_t *network, const char *p_buffer, size_t size);

    extern int lg_ngx_network_not_found(ngx_http_request_t *r);

    extern char *lg_ngx_network_get_user_agent(ngx_http_request_t *r);
    extern char *lg_ngx_network_get_ipaddr(ngx_http_request_t *r);
    extern char *lg_ngx_network_get_forwarded_ipaddr(ngx_http_request_t *r);

    extern int lg_ngx_network_set_cookie(ngx_http_request_t *r, char *cookie, size_t cookie_size);
    extern char *lg_ngx_network_get_cookie(ngx_http_request_t *r, char *cookie);

    extern int lg_ngx_network_add_session(ngx_http_request_t *r, char *key, char *session_id);
    extern int lg_ngx_network_get_session(ngx_http_request_t *r, char *key, char **session_id);

    extern ngx_str_t *lg_ngx_network_get_post_buffer(ngx_http_request_t *r);

    extern void lg_ngx_network_recv_print(char *buffer);
    extern void lg_ngx_network_send_print(ngx_chain_t *buffer);

#ifdef __cplusplus
}
#endif

#endif


