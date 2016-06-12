
#ifndef __LG_THREAD_POOL_H__
#define	__LG_THREAD_POOL_H__

#include <pthread.h>
#include <semaphore.h>
#include "lg_list.h"

#define	LG_THREAD_POOL_IDLE	0
#define	LG_THREAD_POOL_RUN	1
#define	LG_THREAD_POOL_FINISH	2

#ifdef __cplusplus
extern "C" { 
#endif

    typedef struct lg_thread_pool_s lg_thread_pool_t;
    typedef struct lg_thread_node_s lg_thread_node_t;
    typedef void *(lg_thread_function_t)(lg_thread_node_t *thread_info);

    struct lg_thread_node_s {

	lg_list_t next;
	pthread_t tid;
	pthread_attr_t *attr;
	lg_thread_function_t *function;
	lg_thread_pool_t *pool;

	void *arg;
	int status;
	int errcode;
    };

    struct lg_thread_pool_s {

	sem_t sem;
	pthread_mutex_t mutex;
	pthread_cond_t cond;
	lg_list_t head;

    };

    extern lg_thread_pool_t *lg_thread_pool_create(
	    int thread_pool_max,
	    lg_thread_function_t *function,
	    pthread_attr_t *attr);

    extern int lg_thread_pool_start(lg_thread_pool_t *pool);
    extern int lg_thread_pool_stop(lg_thread_pool_t *pool);
    extern void lg_thread_pool_join(lg_thread_pool_t *pool);

#ifdef __cplusplus
}
#endif

#endif


