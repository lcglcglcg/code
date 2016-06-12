

#ifndef __LG_LG_REDIS_H__
#define	__LG_LG_REDIS_H__

#ifdef __cplusplus
extern "C" { 
#endif

    typedef int (lg_redis_recv_function_t)(char *buffer, void *arg);
    extern int lg_redis_recv(lg_redis_recv_function_t *function, void *arg);
    extern int lg_redis_send(char *queue_name, char *buffer, size_t size);

#ifdef __cplusplus
}
#endif

#endif


