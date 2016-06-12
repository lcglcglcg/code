
#ifndef __SITE_BUILDER_FLUSH_H__
#define __SITE_BUILDER_FLUSH_H__

#include <nginx.h>
#include <ngx_core.h>
#include <ngx_http.h>

#include "site_builder_element.h"

#ifdef __cplusplus
extern "C" { 
#endif

    extern int site_builder_flush_init(ngx_pool_t *pool);
    extern int site_builder_flush_check();

    extern int site_builder_flush_load(const char *key, int key_size, char **buffer);
    extern int site_builder_flush_str(const char *key, int key_size, const char *buffer, int buffer_size);
    extern int site_builder_flush_list(const char *key, int key_size, ngx_queue_t *queue);
    extern int site_builder_flush_list_key_value(const char *key, int key_size, ngx_queue_t *queue);

#ifdef __cplusplus
}
#endif

#endif




