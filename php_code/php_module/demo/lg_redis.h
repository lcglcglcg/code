
#ifndef __LG_REDIS_H__
#define __LG_REDIS_H__

#include "hiredis.h"
#include "lg_list.h"

#ifdef __cplusplus
extern "C" { 
#endif

    typedef struct {

	lg_list_t next;
	redisReply *key;

    } lg_redis_key_t;

    typedef struct {

	lg_list_t next;
	redisReply *key;
	redisReply *value;

    } lg_redis_key_value_t;

    typedef struct {

	redisContext *conn;
	lg_list_t key_head;
	lg_list_t key_value_head;

    } lg_redis_t;

    extern lg_redis_t *lg_redis_connect(lg_redis_t *redis, char *host, int port);
    extern void lg_redis_close(lg_redis_t *redis);

    extern int lg_redis_ping(lg_redis_t *p);
    extern int lg_redis_select_base(lg_redis_t *p, char *base);

    extern lg_redis_key_value_t *lg_redis_rand_key_value(lg_redis_t *p);
    extern redisReply *lg_redis_rand_value(lg_redis_t *p);

    extern redisReply *lg_redis_get_key_all(lg_redis_t *p);
    extern redisReply *lg_redis_get_value(lg_redis_t *p, const char *key);
    extern void lg_redis_add_key_value(lg_redis_t *p, const char *key, const char *buffer, size_t size);

    extern redisReply *lg_redis_get_list(lg_redis_t *p, char *key);
    extern void lg_redis_add_key_list(lg_redis_t *p, const char *key, const char *buffer, size_t size);

    extern void lg_redis_del_key(lg_redis_t *p, const char *key);

#ifdef __cplusplus
}
#endif

#endif


