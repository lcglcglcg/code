
#ifndef __MANAGER_SITE_BUILDER_MODULE_H__
#define __MANAGER_SITE_BUILDER_MODULE_H__

#include <nginx.h>
#include <ngx_core.h>
#include <ngx_http.h>
#include "lg_ngx_network.h"

#ifdef __cplusplus
extern "C" { 
#endif

    typedef struct {

	int type;

    } ngx_manager_site_builder_conf_t;

    typedef struct {

    } m_request_t;

    extern void manager_site_builder_packet(ngx_http_request_t *r, char *buffer, int size);

#ifdef __cplusplus
}
#endif

#endif




