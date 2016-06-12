
#ifndef __LG_UPSTREAM_H__
#define __LG_UPSTREAM_H__


#include <nginx.h>
#include <ngx_core.h>
#include <ngx_http.h>

#ifdef __cplusplus
extern "C" { 
#endif

    extern void lg_ngx_network_subrequest_init(ngx_http_request_t *r,
	    ngx_http_event_handler_pt fun,
	    ngx_str_t *loc,
	    ngx_str_t *args);

#ifdef __cplusplus
}
#endif

#endif


