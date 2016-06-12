
#ifndef __SITE_BUILDER_TEMPLATE_H__
#define __SITE_BUILDER_TEMPLATE_H__

#include <nginx.h>
#include <ngx_core.h>
#include <ngx_http.h>

#include "site_builder_element.h"
#include "site_builder_dict.h"

#define SITE_BUILDER_TEMPLATE_TYPE_HTML			1
#define SITE_BUILDER_TEMPLATE_TYPE_TITLE		2
#define SITE_BUILDER_TEMPLATE_TYPE_KEYWORD		3
#define SITE_BUILDER_TEMPLATE_TYPE_CONTENT		4
#define SITE_BUILDER_TEMPLATE_TYPE_META			5

#define SITE_BUILDER_TEMPLATE_TYPE_DATE			6
#define SITE_BUILDER_TEMPLATE_TYPE_TIME			7
#define SITE_BUILDER_TEMPLATE_TYPE_COUNT		8

#define SITE_BUILDER_TEMPLATE_TYPE_LINK_INTERNAL	9
#define SITE_BUILDER_TEMPLATE_TYPE_LINK_EXTERNAL	10
#define SITE_BUILDER_TEMPLATE_TYPE_LINK_BARE		11
#define SITE_BUILDER_TEMPLATE_TYPE_LINK_GROUP		12

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

	char *template_name;
	ngx_queue_t head;

	int title_total;
	int keyword_total;
	int content_total;
	int meta_total;

	int date_total;
	int time_total;
	int count_total;

	int link_internal_total;
	int link_external_total;
	int link_bare_total;
	int link_group_total;

    } site_builder_template_head_t;

    extern int site_builder_template_load(char *template_id,
	    site_builder_template_head_t *template_index,
	    site_builder_template_head_t *template_content,
	    site_builder_element_head_t *affiliated_file,
	    ngx_pool_t *pool);

    extern void site_builder_template_print(site_builder_template_head_t *template);

#ifdef __cplusplus
}
#endif

#endif


