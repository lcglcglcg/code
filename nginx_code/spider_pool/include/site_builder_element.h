
#ifndef __SITE_BUILDER_ELEMENT_H__
#define __SITE_BUILDER_ELEMENT_H__

#include <nginx.h>
#include <ngx_core.h>
#include <ngx_http.h>
#include "lg_rbtree.h"

#define SITE_BUILDER_BUF_SIZE	409600
#define SITE_BUILDER_BUF_MIN	5

#ifdef __cplusplus
extern "C" { 
#endif

    typedef struct {

	lg_rbtree_node tree;

	const char *key;
	int key_size;

	const char *buffer;
	int buffer_size;

    } site_builder_element_t;

    typedef struct {

	ngx_queue_t next;
	char *name;

	lg_rbtree_head head;
	ngx_pool_t *pool;
	int total;

    } site_builder_element_head_t;

    // init
    extern void site_builder_element_init(site_builder_element_head_t *p, ngx_pool_t *pool);

    // add
    extern site_builder_element_t *site_builder_element_key_add(site_builder_element_head_t *p,
	    const char *key, int key_size, 
	    const char *buffer, int buffer_size);

    extern site_builder_element_t *site_builder_element_key_add_r(site_builder_element_head_t *p,
	    const char *key, int key_size, 
	    const char *buffer, int buffer_size);

    extern site_builder_element_t *site_builder_element_index_add(site_builder_element_head_t *p,
	    const char *buffer, int buffer_size);

    extern site_builder_element_t *site_builder_element_index_add_r(site_builder_element_head_t *p,
	    const char *buffer, int buffer_size);

    /* find */
    extern site_builder_element_t *site_builder_element_key_find(site_builder_element_head_t *p, const char *key);
    extern site_builder_element_t *site_builder_element_index_find(site_builder_element_head_t *p, int index);
    extern site_builder_element_t *site_builder_element_index_rand(site_builder_element_head_t *p);

    // load fiel to element
    extern int site_builder_element_buffer_to_file(const char *path, const char *buffer, int buffer_size);
    extern int site_builder_element_file_to_buffer(const char *path, char **buffer, ngx_pool_t *pool);

    extern int site_builder_element_load_file(const char *file_path, site_builder_element_head_t *p, ngx_pool_t *pool);
    extern int site_builder_element_load_link_external_file(const char *file_path, site_builder_element_head_t *p, ngx_pool_t *pool);

#ifdef __cplusplus
}
#endif

#endif



