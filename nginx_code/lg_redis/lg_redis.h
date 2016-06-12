
#ifndef	__LG_REDIS_H__
#define	__LG_REDIS_H__

#ifdef __cplusplus
extern "C" { 
#endif

    typedef void *(lg_redis_malloc)(void *pool, size_t size);
    typedef void (lg_redis_free)(void *pool, void *p);

    typedef struct {

	int sockfd;
	char host[16];
	int port;

	void *memory_pool;
	lg_redis_malloc *malloc;
	lg_redis_free *free;

    } lg_redis_conn_t;

    typedef struct {

	char *str;
	int size;

    } lg_redis_value_t;

    typedef struct {

	lg_redis_value_t *key;
	lg_redis_value_t *value;

    } lg_redis_key_value_t;


    extern int lg_redis_init(lg_redis_conn_t *conn, char *host, int port);
    extern void lg_redis_close(lg_redis_conn_t *conn);

    extern int lg_redis_select_base(lg_redis_conn_t *conn, const char *base);

    extern int lg_redis_add_key_value(lg_redis_conn_t *conn, const char *key, int key_size, const char *value, int value_size);
    extern int lg_redis_add_key_number(lg_redis_conn_t *conn, const char *key, int key_size, int number);
    extern lg_redis_value_t *lg_redis_get_value(lg_redis_conn_t *conn, const char *key, int key_size);

    extern lg_redis_value_t *lg_redis_rand_key(lg_redis_conn_t *conn);
    extern lg_redis_key_value_t *lg_redis_rand_key_value(lg_redis_conn_t *conn);
    extern lg_redis_value_t *lg_redis_rand_value(lg_redis_conn_t *conn);

    extern int lg_redis_add_list_key_value(lg_redis_conn_t *conn, const char *key, int key_size, const char *value, int value_size);

#ifdef __cplusplus
}
#endif

#endif


