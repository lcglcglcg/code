
#ifndef __SITE_BUILDER_CACHE_H__
#define __SITE_BUILDER_CACHE_H__

#include <nginx.h>
#include <ngx_core.h>
#include <ngx_http.h>
#include "site_builder_element.h"

#define SITE_BUILDER_CACHE_KEY_SIZE 1024

#ifdef __cplusplus
extern "C" { 
#endif

    extern void site_builder_cache_init(ngx_pool_t *pool);

    /* find */
    extern site_builder_element_tree_t *site_builder_cache_find_str(
	    const char *base,
	    const char *server_location_name,
	    const char *key);

    extern int site_builder_cache_find_number(
	    const char *base,
	    const char *server_location_name,
	    const char *key);

    extern site_builder_element_list_t *site_builder_cache_find_list(
	    const char *base,
	    const char *server_location_name,
	    const char *key);

    extern site_builder_element_list_t *site_builder_cache_find_list_key_value(
	    const char *base,
	    const char *server_location_name,
	    const char *key);

    /* save */
    extern site_builder_element_tree_t *site_builder_cache_save_str(
	    const char *base,
	    const char *server_location_name,
	    const char *key,
	    const char *buffer, int buffer_size);

    extern site_builder_element_tree_t *site_builder_cache_save_str_r(
	    const char *base,
	    const char *server_location_name,
	    const char *key,
	    const char *buffer, int buffer_size);

    extern int site_builder_cache_save_number(
	    const char *base,
	    const char *server_location_name,
	    const char *key,
	    int number);

    extern site_builder_element_list_t *site_builder_cache_save_list(
	    const char *base,
	    const char *server_location_name,
	    const char *key,
	    ngx_queue_t *list_head);

    extern site_builder_element_list_t *site_builder_cache_save_list_key_value(
	    const char *base,
	    const char *server_location_name,
	    const char *key,
	    ngx_queue_t *list_head);

#ifdef __cplusplus
}
#endif

#endif


