

#ifndef __SITE_BUILDER_MODULE_H__
#define __SITE_BUILDER_MODULE_H__

#include <nginx.h>
#include <ngx_core.h>
#include <ngx_http.h>

#include "site_builder_element.h"
#include "site_builder_template.h"

#define SITE_BUILDER_KEY_SIZE 128

#ifdef __cplusplus
extern "C" { 
#endif

    typedef struct {

	ngx_int_t type;

	char *server_name;
	int server_name_len;

	char *location_name;
	int location_name_len;

	char *server_location_name;
	int server_location_name_len;

	/* Configuration */
	ngx_str_t template_id;
	ngx_str_t keyword_dict;
	ngx_str_t content_dict;
	ngx_str_t link_external_dict;

	/* template */
	site_builder_template_head_t template_index;
	site_builder_template_head_t template_content;
	site_builder_element_head_t affiliated_file;

	/* element */
	site_builder_element_head_t *keyword;
	site_builder_element_head_t *content;
	site_builder_element_head_t *link_external;

    } site_builder_loc_conf_t;

    extern char *site_builder_init(ngx_conf_t *cf, site_builder_loc_conf_t *loc_conf);
    extern ngx_int_t site_builder_global_init(ngx_cycle_t *cycle);
    extern int site_builder_filter(ngx_http_request_t *r, site_builder_loc_conf_t *loc_conf);

#ifdef __cplusplus
}
#endif

#endif




