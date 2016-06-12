
#include <string.h>
#include <stdlib.h>
#include <hiredis.h>
#include "manager_account_redis.h"

static char *redis_host = NULL;
static int redis_port = 0;
int manager_account_redis_init(lg_list_t *config_file_head)
{

    if (!(redis_host = lg_config_file_find(config_file_head, "redis_host")))
	return -1;;

    if (!(redis_port = lg_config_file_find_number(config_file_head, "redis_port")))
	return -1;;

    // fprintf(stdout, "redis_host=%s\n", redis_host);
    // fprintf(stdout, "redis_port=%d\n", redis_port);
    return 0;
}

redisContext *manager_account_redis_conn(char *db)
{
    redisContext *conn = redisConnect(redis_host, redis_port);
    if (conn == NULL || conn->err) {

	if (conn) redisFree(conn);
	return NULL;
    }

    if (db) {

	redisReply *reply = redisCommand(conn, "select %s", db);
	if (!reply || reply->type != REDIS_REPLY_STATUS) {

	    if (reply) freeReplyObject(reply);
	    redisFree(conn);
	    return NULL;
	}

	freeReplyObject(reply);
    }

    return conn;
}

int manager_account_redis_get_session(const char *session_id, size_t *account_id)
{

    if (!session_id) return -1;

    redisContext *conn = manager_account_redis_conn(REDIS_DB_SESSION);
    if (!conn) return -1;

    redisReply *reply = redisCommand(conn, "get %s", session_id);
    if (!reply || reply->type != REDIS_REPLY_STRING) {

	*account_id = 0;
	redisFree(conn);
	return -1;
    }

    if (!(*account_id = atoi(reply->str))) {

	freeReplyObject(reply);
	redisFree(conn);
	return -1;
    }

    freeReplyObject(reply);
    reply = redisCommand(conn, "EXPIRE %s %d", session_id, REDIS_SESSION_TIME_OUT);
    if (!reply || reply->type != REDIS_REPLY_INTEGER) {

	redisFree(conn);
	return -1;
    }

    freeReplyObject(reply);
    redisFree(conn);
    return 0;
}

int manager_account_redis_set_session(const char *session_id, size_t account_id)
{

    if (!session_id) return -1;

    redisContext *conn = manager_account_redis_conn(REDIS_DB_SESSION);
    if (!conn) return -1;

    redisReply *reply = redisCommand(conn, "set %s %d", session_id, account_id);
    if (!reply || reply->type != REDIS_REPLY_STATUS) {

	redisFree(conn);
	return -1;
    }

    freeReplyObject(reply);
    reply = redisCommand(conn, "EXPIRE %s %d", session_id, REDIS_SESSION_TIME_OUT);
    if (!reply || reply->type != REDIS_REPLY_INTEGER) {

	redisFree(conn);
	return -1;
    }

    freeReplyObject(reply);
    redisFree(conn);
    return 0;
}

int manager_account_redis_del_session(const char *session_id)
{

    if (!session_id) return -1;

    redisContext *conn = manager_account_redis_conn(REDIS_DB_SESSION);
    if (!conn) return -1;

    redisReply *reply = redisCommand(conn, "del %s", session_id);
    if (!reply || reply->type != REDIS_REPLY_STATUS) {

	redisFree(conn);
	return -1;
    }

    freeReplyObject(reply);
    redisFree(conn);
    return 0;
}

int manager_account_redis_get_var_code(const char *session_id, char *var_code)
{

    if (!session_id) return -1;

    redisContext *conn = manager_account_redis_conn(REDIS_DB_VAR_CODE);
    if (!conn) return -1;

    redisReply *reply = redisCommand(conn, "get %s", session_id);
    if (!reply) {

	redisFree(conn);
	return -1;
    }

    if (!reply->str) {

	redisFree(conn);
	return -1;
    }

    strcpy(var_code, reply->str);
    freeReplyObject(reply);

    reply = redisCommand(conn, "DEL %s", session_id);
    if (reply) freeReplyObject(reply);

    redisFree(conn);
    return 0;
}

int manager_account_redis_set_var_code(const char *session_id, const char *var_code)
{

    if (!session_id) return -1;

    redisContext *conn = manager_account_redis_conn(REDIS_DB_VAR_CODE);
    if (!conn) return -1;

    redisReply *reply = redisCommand(conn, "set %s %s", session_id, var_code);
    if (!reply || reply->type != REDIS_REPLY_STATUS) {

	redisFree(conn);
	return -1;
    }

    freeReplyObject(reply);
    reply = redisCommand(conn, "EXPIRE %s %d", session_id, REDIS_VAR_CODE_TIME_OUT);
    if (!reply || reply->type != REDIS_REPLY_INTEGER) {

	redisFree(conn);
	return -1;
    }

    freeReplyObject(reply);
    redisFree(conn);
    return 0;
}

int manager_account_redis_set_wangmeng_session(const char *session_id, const char *wangmeng_session)
{

    if (!session_id) return -1;

    redisContext *conn = manager_account_redis_conn(REDIS_DB_WANGMENG_SESSION);
    if (!conn) return -1;

    redisReply *reply = redisCommand(conn, "set %s %s", session_id, wangmeng_session);
    if (!reply || reply->type != REDIS_REPLY_STATUS) {

	redisFree(conn);
	return -1;
    }

    freeReplyObject(reply);
    reply = redisCommand(conn, "EXPIRE %s %d", session_id, REDIS_VAR_CODE_TIME_OUT);
    if (!reply || reply->type != REDIS_REPLY_INTEGER) {

	redisFree(conn);
	return -1;
    }

    freeReplyObject(reply);
    redisFree(conn);
    return 0;
}

int manager_account_redis_get_wangmeng_session(const char *session_id, char *wangmeng_session)
{

    if (!session_id) return -1;

    redisContext *conn = manager_account_redis_conn(REDIS_DB_WANGMENG_SESSION);
    if (!conn) return -1;

    redisReply *reply = redisCommand(conn, "get %s", session_id);
    if (!reply) {

	redisFree(conn);
	return -1;
    }

    if (!reply->str) {

	redisFree(conn);
	return -1;
    }

    strcpy(wangmeng_session , reply->str);
    freeReplyObject(reply);

    reply = redisCommand(conn, "DEL %s", session_id);
    if (reply) freeReplyObject(reply);

    redisFree(conn);
    return 0;
}


int manager_account_redis_set_ruanwen_session(const char *session_id, const char *ruanwen_session)
{

    if (!session_id) return -1;

    redisContext *conn = manager_account_redis_conn(REDIS_DB_RUANWEN_SESSION);
    if (!conn) return -1;

    redisReply *reply = redisCommand(conn, "set %s %s", session_id, ruanwen_session);
    if (!reply || reply->type != REDIS_REPLY_STATUS) {

	redisFree(conn);
	return -1;
    }

    freeReplyObject(reply);
    reply = redisCommand(conn, "EXPIRE %s %d", session_id, REDIS_VAR_CODE_TIME_OUT);
    if (!reply || reply->type != REDIS_REPLY_INTEGER) {

	redisFree(conn);
	return -1;
    }

    freeReplyObject(reply);
    redisFree(conn);
    return 0;
}

int manager_account_redis_get_ruanwen_session(const char *session_id, char *ruanwen_session)
{

    if (!session_id) return -1;

    redisContext *conn = manager_account_redis_conn(REDIS_DB_RUANWEN_SESSION);
    if (!conn) return -1;

    redisReply *reply = redisCommand(conn, "get %s", session_id);
    if (!reply) {

	redisFree(conn);
	return -1;
    }

    if (!reply->str) {

	redisFree(conn);
	return -1;
    }

    strcpy(ruanwen_session , reply->str);
    freeReplyObject(reply);

    reply = redisCommand(conn, "DEL %s", session_id);
    if (reply) freeReplyObject(reply);

    redisFree(conn);
    return 0;
}


