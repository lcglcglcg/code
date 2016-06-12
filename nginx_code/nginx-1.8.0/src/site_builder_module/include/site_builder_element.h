
#ifndef __SITE_BUILDER_ELEMENT_H__
#define __SITE_BUILDER_ELEMENT_H__

#ifdef __cplusplus
extern "C" { 
#endif

#include <nginx.h>
#include <ngx_core.h>
#include <ngx_http.h>
#include "lg_rbtree.h"

    typedef struct {

	ngx_queue_t next;

	char *buffer;
	int buffer_size;

    } site_builder_element_str_t;

    typedef struct {

	lg_rbtree_node tree;

	char *key;
	int key_size;

	char *buffer;
	int buffer_size;

    } site_builder_element_node_t;

    typedef struct {

	ngx_queue_t next;
	char *name;

	lg_rbtree_head head;
	ngx_pool_t *pool;
	int total;

    } site_builder_element_head_t;

    // init
    extern void site_builder_element_init(site_builder_element_head_t *p, char *name, ngx_pool_t *pool);

    // add
    extern site_builder_element_node_t *site_builder_element_key_add(site_builder_element_head_t *p,
	    char *key, int key_size, 
	    char *buffer, int buffer_size);

    extern site_builder_element_node_t *site_builder_element_key_add_r(site_builder_element_head_t *p,
	    char *key, int key_size, 
	    char *buffer, int buffer_size);

    extern site_builder_element_node_t *site_builder_element_index_add(site_builder_element_head_t *p,
	    char *key, int key_size,
	    char *buffer, int buffer_size);

    extern site_builder_element_node_t *site_builder_element_index_add_r(site_builder_element_head_t *p,
	    char *key, int key_size,
	    char *buffer, int buffer_size);

    /* find */
    extern site_builder_element_node_t *site_builder_element_key_find(site_builder_element_head_t *p,  char *key);
    extern site_builder_element_node_t *site_builder_element_index_find(site_builder_element_head_t *p, int index);
    extern site_builder_element_node_t *site_builder_element_index_rand(site_builder_element_head_t *p);

#ifdef __cplusplus
}
#endif

#endif



