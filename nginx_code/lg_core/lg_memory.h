
#ifndef __LG_MEMORY_H__
#define	__LG_MEMORY_H__

#include <pthread.h>

#ifdef __cplusplus
extern "C" { 
#endif

    extern void *lg_memory_share_malloc(int size);
    extern pthread_mutex_t *lg_memory_share_mutex_init();

#ifdef __cplusplus
}
#endif
#endif//__LG_MEMORY_H__



