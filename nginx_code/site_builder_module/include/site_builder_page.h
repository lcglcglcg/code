
#ifndef __LOST_SPIDER_PAGE_H__
#define __LOST_SPIDER_PAGE_H__

#include <nginx.h>
#include <ngx_core.h>
#include <ngx_http.h>

#include "lg_ngx_network.h"
#include "site_builder_element.h"
#include "site_builder_template.h"
#include "site_builder_module.h"
#include "site_builder_uri.h"

#ifdef __cplusplus
extern "C" { 
#endif

    typedef struct {

	site_builder_uri_t uri;
	lg_ngx_network_t network;
	site_builder_template_t *template;

	const char *title_str;
	int title_size;

	const char *keyword_str;
	int keyword_size;

	ngx_queue_t content;
	ngx_queue_t content_rand;
	ngx_queue_t meta;

	const char *date_str;
	int date_str_size;

	const char *time_str;
	int time_str_size;

	ngx_queue_t link_internal;
	ngx_queue_t link_external;
	ngx_queue_t link_subdomain;
	ngx_queue_t link_bare;
	ngx_queue_t link_group_all;

	ngx_queue_t link_save_internal;
	ngx_queue_t link_save_external;
	ngx_queue_t link_save_subdomain;
	ngx_queue_t link_save_bare;
	ngx_queue_t link_save_group_all;

	ngx_queue_t link_group[SITE_BUILDER_TEMPLATE_GROUP_COUNT];
	ngx_queue_t link_save_group[SITE_BUILDER_TEMPLATE_GROUP_COUNT];

    } site_builder_page_info_t;

    extern int site_builder_page_load(ngx_http_request_t *r,
	    site_builder_loc_conf_t *loc_conf, 
	    site_builder_page_info_t *page_info);

    extern int site_builder_page_display(ngx_http_request_t *r, site_builder_loc_conf_t *loc_conf, site_builder_page_info_t *page_info);

#ifdef __cplusplus
}
#endif

#endif


