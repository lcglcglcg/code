
#ifndef __MANAGER_ACCOUNT_MODULE_H__
#define __MANAGER_ACCOUNT_MODULE_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <nginx.h>
#include <ngx_core.h>
#include <ngx_http.h>

#include "lg_list.h"
#include "lg_rbtree.h"

#include "manager_account_json.h"

#ifdef __cplusplus
extern "C" { 
#endif

    typedef struct {

	lg_list_t config_file;
	ngx_str_t config_file_path;

    } manager_account_module_conf_t;

    typedef struct {

	char *session_id;
	size_t account_id;
	manager_account_json_t json;

	char wangmeng_session[64];
	char ruanwen_session[64];

    } manager_account_module_request_t;

    extern void manager_account_packet_parse(ngx_http_request_t *r, 
	    // ngx_http_core_srv_conf_t *srv_conf,
	    // manager_account_module_conf_t *loc_conf,
	    // ngx_http_core_loc_conf_t *core_conf,
	    manager_account_module_request_t *m_request);
    extern int manager_account_packet_var_code(ngx_http_request_t *r);

#ifdef __cplusplus
}
#endif

#endif




