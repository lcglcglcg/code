
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

	/* Configuration */
	ngx_str_t template_dir;
	ngx_str_t keyword_file;
	ngx_str_t content_file;
	ngx_str_t link_external_url_file;
	ngx_str_t link_external_keyword_file;
	ngx_str_t link_group_file;

	/* template */
	site_builder_template_list_t template_list;

	/* element */
	site_builder_element_head_t *affiliated_file;
	site_builder_element_head_t *keyword;
	site_builder_element_head_t *content;
	site_builder_element_head_t *link_external_url;
	site_builder_element_head_t *link_external_keyword;
	site_builder_element_head_t *link_group;

    } site_builder_loc_conf_t;

    extern char *site_builder_init(ngx_conf_t *cf, site_builder_loc_conf_t *loc_conf);
    extern void site_builder_init_global_value(ngx_cycle_t *cycle);

    extern int site_builder_filter(ngx_http_request_t *r, site_builder_loc_conf_t *loc_conf);

#ifdef __cplusplus
}
#endif

#endif




