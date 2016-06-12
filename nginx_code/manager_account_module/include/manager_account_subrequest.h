
#ifndef __MANAGER_ACCOUNT_SUBREQUEST_H__
#define __MANAGER_ACCOUNT_SUBREQUEST_H__

#include <nginx.h>
#include <ngx_core.h>
#include <ngx_http.h>

#ifdef __cplusplus
extern "C" { 
#endif

    extern void manager_account_subrequest_init(ngx_http_request_t *r, ngx_str_t *loc, char *buffer, int size);

#ifdef __cplusplus
}
#endif

#endif




