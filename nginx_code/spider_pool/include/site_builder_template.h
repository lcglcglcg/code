
#ifndef __SITE_BUILDER_TEMPLATE_H__
#define __SITE_BUILDER_TEMPLATE_H__

#include <nginx.h>
#include <ngx_core.h>
#include <ngx_http.h>
#include "site_builder_element.h"

#define SITE_BUILDER_TEMPLATE_TYPE_HTML			1
#define SITE_BUILDER_TEMPLATE_TYPE_TITLE		2
#define SITE_BUILDER_TEMPLATE_TYPE_KEYWORD		3
#define SITE_BUILDER_TEMPLATE_TYPE_CONTENT		4

#define SITE_BUILDER_TEMPLATE_TYPE_LINK_INTERNAL	8
#define SITE_BUILDER_TEMPLATE_TYPE_LINK_EXTERNAL	9
#define SITE_BUILDER_TEMPLATE_TYPE_LINK_BARE		11
#define SITE_BUILDER_TEMPLATE_TYPE_LINK_GROUP		101

#ifdef __cplusplus
extern "C" { 
#endif

    typedef struct {

	ngx_queue_t next;
	int type;
	char *buffer;
	int size;

    } site_builder_template_node_t;

    typedef struct {

	ngx_queue_t next;
	ngx_queue_t head;

	int template_id;
	char *template_name;

	int title_total;
	int keyword_total;
	int content_total;

	int link_internal_total;
	int link_external_total;
	int link_bare_total;
	int link_group_total;

    } site_builder_template_t;

    typedef struct {

	ngx_queue_t head;
	site_builder_element_head_t *affiliated_file_list;
	int template_total;

    } site_builder_template_list_t;

    extern int site_builder_template_dir_load(
	    char *path, 
	    int path_size,
	    site_builder_template_list_t *template_list,
	    site_builder_element_head_t *affiliated_file_list,
	    ngx_pool_t *pool);

#ifdef __cplusplus
}
#endif

#endif


