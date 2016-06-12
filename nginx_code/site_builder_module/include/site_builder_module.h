

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

	ngx_queue_t next;
	char *link_group_name;
	int index;

    } link_group_config_t;

    typedef struct {

	ngx_int_t type;

	char *server_name;
	int server_name_len;

	char *location_name;
	int location_name_len;

	char *server_location_name;
	int server_location_name_len;

	/* Configuration */
	ngx_str_t template_dir;
	ngx_str_t keyword_file;
	ngx_str_t content_file;
	ngx_str_t link_external_file;
	ngx_str_t mate_file;
	ngx_str_t link_subdomain_file;

	ngx_str_t link_group[SITE_BUILDER_TEMPLATE_GROUP_COUNT];
	ngx_queue_t link_group_config;

	/* template */
	site_builder_template_list_t template_list;

	/* element */
	site_builder_element_head_tree_t affiliated_file;
	site_builder_element_head_tree_t keyword;
	site_builder_element_head_tree_t content;
	site_builder_element_head_tree_t link_external;
	site_builder_element_head_tree_t link_bare;
	site_builder_element_head_tree_t meta;
	site_builder_element_head_tree_t link_subdomain;

	/* global element */
	ngx_queue_t *global_keyword_list;
	int *global_keyword_list_count;

	ngx_queue_t *global_content_list;
	int *global_content_list_count;

	/* ssdb conn */
	// lg_ssdb_conn_t *ssdb_conn;

    } site_builder_loc_conf_t;

    extern char *site_builder_init(ngx_conf_t *cf, site_builder_loc_conf_t *loc_conf);
    extern ngx_int_t site_builder_global_init(ngx_cycle_t *cycle);
    extern int site_builder_filter(ngx_http_request_t *r, site_builder_loc_conf_t *loc_conf);

#ifdef __cplusplus
}
#endif

#endif




