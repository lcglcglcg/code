
#ifndef __LG_LEVELDB_H__
#define	__LG_LEVELDB_H__

#ifdef __cplusplus
extern "C" { 
#endif

#include <ngx_core.h>

    extern void *lg_leveldb_open(char *path);
    extern int lg_leveldb_set(void *p_db, char *key, char *value);
    extern int lg_leveldb_get(void *p_db, char *key, char **value, ngx_pool_t *pool);

#ifdef __cplusplus
}
#endif
#endif



