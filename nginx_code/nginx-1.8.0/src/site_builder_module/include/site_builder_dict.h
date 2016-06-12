
#ifndef __SITE_BUILDER_DICT_H__
#define __SITE_BUILDER_DICT_H__

#ifdef __cplusplus
extern "C" { 
#endif

#include <nginx.h>
#include <ngx_core.h>
#include <ngx_http.h>
#include "site_builder_element.h"

#define SITE_BUILDER_DICT_BUF_SIZE 409600
#define SITE_BUILDER_DICT_HOME_PATH "/var/site_builder_dir"

    extern int site_builder_dict_file_to_buffer(char *path, char **buffer, ngx_pool_t *pool);
    extern int site_builder_dict_load_file(char *dict_id, site_builder_element_head_t *p, ngx_pool_t *pool);
    extern int site_builder_dict_load_file_cut(char *dict_id, site_builder_element_head_t *p, ngx_pool_t *pool);

    extern site_builder_element_head_t *site_builder_dict_keyword(char *dict_id, ngx_pool_t *pool);
    extern site_builder_element_head_t *site_builder_dict_content(char *dict_id, ngx_pool_t *pool);
    extern site_builder_element_head_t *site_builder_dict_link_external(char *dict_id, ngx_pool_t *pool);

#ifdef __cplusplus
}
#endif

#endif



