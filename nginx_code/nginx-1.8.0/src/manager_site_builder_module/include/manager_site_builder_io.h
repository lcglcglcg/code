
#ifndef __MANAGER_SITE_BUILDER_IO_H__
#define __MANAGER_SITE_BUILDER_IO_H__

#include <nginx.h>
#include <ngx_core.h>
#include <ngx_http.h>

#ifdef __cplusplus
extern "C" { 
#endif

    extern int manager_site_builder_io_get_file(char *dir, char *name, char **buffer, ngx_pool_t *pool);
    extern int manager_site_builder_io_add_template_file(char *template_id, char *template_file_name, char *buffer, int size);
    extern int manager_site_builder_io_add_dict_file(char *dict_id, char *buffer, int size);

#ifdef __cplusplus
}
#endif

#endif



