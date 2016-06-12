
#ifndef __SITE_BUILDER_FLUSH_H__
#define __SITE_BUILDER_FLUSH_H__

#include <nginx.h>
#include <ngx_core.h>
#include <ngx_http.h>

#include "site_builder_element.h"

#define SITE_BUILDER_FLUSH_HOME_PATH "/var/site_builder_flush"

#ifdef __cplusplus
extern "C" { 
#endif

    extern void site_builder_flush_init_base(ngx_pool_t *pool);

    extern int site_builder_flush_find_str(char *row_key, char *base, char *key, char **buffer, ngx_pool_t *pool);
    extern int site_builder_flush_find_list(char *row_key, char *base, char *key, ngx_queue_t *queue, ngx_pool_t *pool);

    extern int site_builder_flush_save_str(char *row_key, char *base, char *key, char *buffer, int buffer_size);
    extern int site_builder_flush_save_list(char *row_key, char *base, char *key, ngx_queue_t *queue, ngx_pool_t *pool);

#ifdef __cplusplus
}
#endif

#endif



