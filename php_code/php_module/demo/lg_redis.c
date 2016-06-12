
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "lg_redis.h"

lg_redis_t *lg_redis_connect(lg_redis_t *redis, char *host, int port)
{
    redisContext *c = redisConnect(host, port);
    if (c == NULL || c->err) {

	if (c) redisFree(c);
	return NULL;
    }

    redis->conn = c;
    lg_list_init(&redis->key_head);
    lg_list_init(&redis->key_value_head);
    return redis;
}

void lg_redis_close(lg_redis_t *redis)
{

    lg_list_t *head = &redis->key_head;
    lg_list_t *p = head->next;
    while(p != head) {

	lg_redis_key_t *node = (lg_redis_key_t *)p;
	freeReplyObject(node->key);

	p = p->next;
	free(node);
    }

    head = &redis->key_value_head;
    p = head->next;
    while(p != head) {

	lg_redis_key_value_t *node = (lg_redis_key_value_t *)p;
	freeReplyObject(node->key);
	freeReplyObject(node->value);

	p = p->next;
	free(node);
    }

    redisFree(redis->conn);
}

int lg_redis_ping(lg_redis_t *p)
{

    redisReply *reply = redisCommand(p->conn, "PING");
    if (!reply) return -1;

    if (strcmp(reply->str, "PONG") != 0) {

	freeReplyObject(reply);
	return -1;
    }

    freeReplyObject(reply);
    return 0;
}

int lg_redis_select_base(lg_redis_t *p, char *base)
{

    redisReply *r = redisCommand(p->conn, "select %s", base);
    if (r->type == REDIS_REPLY_STATUS && strcasecmp(r->str, "OK") == 0) {

	freeReplyObject(r);
	return 0;
    }

    freeReplyObject(r);
    return -1;
}

lg_redis_key_value_t *lg_redis_rand_key_value(lg_redis_t *p)
{

    lg_redis_key_value_t *node = malloc(sizeof(lg_redis_key_value_t));
    if (!node) return NULL;

    node->key = redisCommand(p->conn, "RANDOMKEY");
    if (!node->key) {

	free(node);
	return NULL;
    }

    node->value = redisCommand(p->conn, "get %s", node->key->str);
    if (!node->value) {

	freeReplyObject(node->key);
	free(node);
	return NULL;
    }

    lg_list_add(&p->key_value_head, &node->next);
    return node;
}

redisReply *lg_redis_rand_value(lg_redis_t *p)
{

    lg_redis_key_value_t *reply = lg_redis_rand_key_value(p);
    if (!reply) return NULL;
    return reply->value;
}

redisReply *lg_redis_get_key_all(lg_redis_t *p)
{

    lg_redis_key_t *node = malloc(sizeof(lg_redis_key_t));
    if (!node) return NULL;

    node->key = redisCommand(p->conn, "keys *");
    if (!node->key) {

	free(node);
	return NULL;
    }

    if (node->key->type != REDIS_REPLY_ARRAY) {

	freeReplyObject(node->key);
	free(node);
	return NULL;
    }

    //size_t i;
    //for (i = 0; i < node->key->elements; i++)
    //fprintf(stdout, "%s\n", node->key->element[i]->str);

    lg_list_add(&p->key_head, &node->next);
    return node->key;
}

redisReply *lg_redis_get_value(lg_redis_t *p, const char *key)
{

    lg_redis_key_t *node = malloc(sizeof(lg_redis_key_t));
    if (!node) return NULL;

    node->key = redisCommand(p->conn, "get %s", key);
    if (!node->key) {

	free(node);
	return NULL;
    }

    if (node->key->type != REDIS_REPLY_STRING) {

	freeReplyObject(node->key);
	free(node);
	return NULL;
    }

    lg_list_add(&p->key_head, &node->next);
    return node->key;
}

void lg_redis_add_key_value(lg_redis_t *p, const char *key, const char *buffer, size_t size)
{
    redisReply *r = redisCommand(p->conn, "set %s %b", key, buffer, size);
    if (r) freeReplyObject(r);
}

redisReply *lg_redis_get_list(lg_redis_t *p, char *key)
{

    lg_redis_key_t *node = malloc(sizeof(lg_redis_key_t));
    if (!node) return NULL;

    node->key = redisCommand(p->conn, "lrange %s 0 -1", key);
    if (!node->key) {

	free(node);
	return NULL;
    }

    if (node->key->type != REDIS_REPLY_ARRAY) {

	freeReplyObject(node->key);
	free(node);
	return NULL;
    }

    if (node->key->elements == 0) {

	freeReplyObject(node->key);
	free(node);
	return NULL;
    }

    lg_list_add(&p->key_head, &node->next);
    return node->key;
}

void lg_redis_add_key_list(lg_redis_t *p, const char *key, const char *buffer, size_t size)
{
    redisReply *r = redisCommand(p->conn, "rpush %s %b", key, buffer, size);
    if (r) freeReplyObject(r);
}

void lg_redis_del_key(lg_redis_t *p, const char *key)
{
    redisReply *r = redisCommand(p->conn, "del %s", key);
    if (r) freeReplyObject(r);
}


