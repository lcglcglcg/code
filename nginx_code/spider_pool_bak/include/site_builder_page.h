
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

	ngx_queue_t next;

	const char *str;
	int str_size;

    } site_builder_page_str_t;

    typedef struct {

	ngx_queue_t next;

	const char *keyword;
	int keyword_size;

	const char *link;
	int link_size;

    } site_builder_page_link_t;

    typedef struct {

	site_builder_uri_t uri;
	lg_ngx_network_t network;
	site_builder_template_t *template;

	const char *title_str;
	int title_size;

	const char *keyword_str;
	int keyword_size;

	ngx_queue_t content;

	ngx_queue_t link_internal;
	ngx_queue_t link_external;
	ngx_queue_t link_bare;
	ngx_queue_t link_group;

    } site_builder_page_info_t;

    extern int site_builder_page_load(ngx_http_request_t *r,
	    site_builder_loc_conf_t *loc_conf, 
	    site_builder_page_info_t *page_info);

    extern int site_builder_page_display(ngx_http_request_t *r, site_builder_loc_conf_t *loc_conf, site_builder_page_info_t *page_info);

#ifdef __cplusplus
}
#endif

#endif


