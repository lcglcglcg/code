
#ifndef __SITE_BUILDER_TEMPLATE_H__
#define __SITE_BUILDER_TEMPLATE_H__

#include <nginx.h>
#include <ngx_core.h>
#include <ngx_http.h>
#include "site_builder_element.h"

#define SITE_BUILDER_TEMPLATE_GROUP_COUNT		10

#define SITE_BUILDER_TEMPLATE_TYPE_HTML			1
#define SITE_BUILDER_TEMPLATE_TYPE_TITLE		2
#define SITE_BUILDER_TEMPLATE_TYPE_KEYWORD		3
#define SITE_BUILDER_TEMPLATE_TYPE_CONTENT		4
#define SITE_BUILDER_TEMPLATE_TYPE_CONTENT_META		5
#define SITE_BUILDER_TEMPLATE_TYPE_CONTENT_RAND		6
#define SITE_BUILDER_TEMPLATE_TYPE_META			7

#define SITE_BUILDER_TEMPLATE_TYPE_DATE			8
#define SITE_BUILDER_TEMPLATE_TYPE_TIME			9

#define SITE_BUILDER_TEMPLATE_TYPE_LINK_INTERNAL	10
#define SITE_BUILDER_TEMPLATE_TYPE_LINK_EXTERNAL	11
#define SITE_BUILDER_TEMPLATE_TYPE_LINK_SUBDOMAIN	12
#define SITE_BUILDER_TEMPLATE_TYPE_LINK_BARE		13

#define SITE_BUILDER_TEMPLATE_TYPE_LINK_SAVE_INTERNAL	14
#define SITE_BUILDER_TEMPLATE_TYPE_LINK_SAVE_EXTERNAL	15
#define SITE_BUILDER_TEMPLATE_TYPE_LINK_SAVE_SUBDOMAIN	16
#define SITE_BUILDER_TEMPLATE_TYPE_LINK_SAVE_BARE	17

#define SITE_BUILDER_TEMPLATE_TYPE_LINK_GROUP_ALL	1000
#define SITE_BUILDER_TEMPLATE_TYPE_LINK_GROUP0		100
#define SITE_BUILDER_TEMPLATE_TYPE_LINK_GROUP1		101
#define SITE_BUILDER_TEMPLATE_TYPE_LINK_GROUP2		102
#define SITE_BUILDER_TEMPLATE_TYPE_LINK_GROUP3		103
#define SITE_BUILDER_TEMPLATE_TYPE_LINK_GROUP4		104
#define SITE_BUILDER_TEMPLATE_TYPE_LINK_GROUP5		105
#define SITE_BUILDER_TEMPLATE_TYPE_LINK_GROUP6		106
#define SITE_BUILDER_TEMPLATE_TYPE_LINK_GROUP7		107
#define SITE_BUILDER_TEMPLATE_TYPE_LINK_GROUP8		108
#define SITE_BUILDER_TEMPLATE_TYPE_LINK_GROUP9		109

#define SITE_BUILDER_TEMPLATE_TYPE_LINK_SAVE_GROUP_ALL	1100
#define SITE_BUILDER_TEMPLATE_TYPE_LINK_SAVE_GROUP0	110
#define SITE_BUILDER_TEMPLATE_TYPE_LINK_SAVE_GROUP1	111
#define SITE_BUILDER_TEMPLATE_TYPE_LINK_SAVE_GROUP2	112
#define SITE_BUILDER_TEMPLATE_TYPE_LINK_SAVE_GROUP3	113
#define SITE_BUILDER_TEMPLATE_TYPE_LINK_SAVE_GROUP4	114
#define SITE_BUILDER_TEMPLATE_TYPE_LINK_SAVE_GROUP5	115
#define SITE_BUILDER_TEMPLATE_TYPE_LINK_SAVE_GROUP6	116
#define SITE_BUILDER_TEMPLATE_TYPE_LINK_SAVE_GROUP7	117
#define SITE_BUILDER_TEMPLATE_TYPE_LINK_SAVE_GROUP8	118
#define SITE_BUILDER_TEMPLATE_TYPE_LINK_SAVE_GROUP9	119

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
	int content_meta_total;
	int content_rand_total;
	int meta_total;

	int date_total;
	int time_total;

	int link_internal_total;
	int link_external_total;
	int link_subdomain_total;
	int link_bare_total;
	int link_group_all_total;
	int link_group_total[SITE_BUILDER_TEMPLATE_GROUP_COUNT];

	int link_save_internal_total;
	int link_save_external_total;
	int link_save_subdomain_total;
	int link_save_bare_total;
	int link_save_group_all_total;
	int link_save_group_total[SITE_BUILDER_TEMPLATE_GROUP_COUNT];

    } site_builder_template_t;

    typedef struct {

	ngx_queue_t head;
	site_builder_element_head_tree_t *affiliated_file_list;
	int template_total;

    } site_builder_template_list_t;

    extern int site_builder_template_dir_load(
	    char *path, 
	    int path_size,
	    site_builder_template_list_t *template_list,
	    site_builder_element_head_tree_t *affiliated_file_list,
	    ngx_pool_t *pool);

#ifdef __cplusplus
}
#endif

#endif


