
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <hiredis/hiredis.h>
#include "lg_redis.h"
#include "lg_string.h"

int lg_redis_recv(lg_redis_recv_function_t *function, void *arg)
{
    redisContext *conn = redisConnect("127.0.0.1", 6379);
    if (!conn) {

	fprintf(stdout, "redis conn ::err\n");
	return -1; 
    }

    for (;;) {

	redisReply *reply = redisCommand(conn, "rpop article");
	if (reply) {

	    if (reply->type == REDIS_REPLY_STRING) {

		function(reply->str, arg);

	    } else {

		sleep(1);
	    }

	    freeReplyObject(reply);
	}
    }

    redisFree(conn);
    return 0;
}

int lg_redis_send(char *queue_name, char *buffer, size_t size)
{

    redisContext *conn = redisConnect("127.0.0.1", 6379);
    if (!conn) {

	fprintf(stdout, "redis conn ::err\n");
	return -1; 
    }

    redisReply *reply = redisCommand(conn, "rpush %s %b", queue_name, buffer, size);
    if (!reply || reply->type != 3) {

	return -1;
	redisFree(conn);
    }

    if (reply) freeReplyObject(reply);
    redisFree(conn);
    return 0;
}






