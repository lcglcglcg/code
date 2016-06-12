
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include "lg_rabbitmq.h"

static int lg_rabbitmq_get_rpc_reply(amqp_connection_state_t conn)
{
    amqp_rpc_reply_t r = amqp_get_rpc_reply(conn);
    if (r.reply_type != AMQP_RESPONSE_NORMAL)
	return -1;

    return 0;
}

amqp_connection_state_t lg_rabbitmq_conn(const char *host, const char *user, const char *pass)
{

    amqp_connection_state_t conn = amqp_new_connection();
    if (!conn) return NULL;

    amqp_socket_t *socket = amqp_tcp_socket_new(conn);
    if (!socket) return NULL;

    if (amqp_socket_open(socket, host, RABBITMQ_PORT))
	return NULL;

    amqp_rpc_reply_t r = amqp_login(conn, "/", 0, 131072, 0, AMQP_SASL_METHOD_PLAIN, user, pass);
    if (r.reply_type != AMQP_RESPONSE_NORMAL)
	return NULL;

    amqp_channel_open(conn, 1);
    if (lg_rabbitmq_get_rpc_reply(conn))
	return NULL;

    return conn;
}

void lg_rabbitmq_close(amqp_connection_state_t conn)
{
    amqp_channel_close(conn, 1, AMQP_REPLY_SUCCESS);
    amqp_connection_close(conn, AMQP_REPLY_SUCCESS);
    amqp_destroy_connection(conn);
}

int lg_rabbitmq_bind(amqp_connection_state_t conn ,const char *routingkey, const char *queuename)
{

    amqp_queue_declare(conn, 1, amqp_cstring_bytes(queuename), 0, 0, 0, 1, amqp_empty_table);
    if (lg_rabbitmq_get_rpc_reply(conn))
	return -1;

    amqp_queue_bind(conn, 1, 
	    amqp_cstring_bytes(queuename),
	    amqp_cstring_bytes("amq.direct"),
	    amqp_cstring_bytes(routingkey), 
	    amqp_empty_table);
    if (lg_rabbitmq_get_rpc_reply(conn))
	return -1;

    return 0;
}

int lg_rabbitmq_recv(amqp_connection_state_t conn ,const char *queuename, lg_rabbitmq_recv_function_t *call_function, void *arg)
{

    amqp_envelope_t envelope;
    amqp_queue_declare(conn, 1, amqp_cstring_bytes(queuename), 0, 0, 0, 1, amqp_empty_table);

    amqp_basic_consume(conn, 1, amqp_cstring_bytes(queuename), amqp_empty_bytes, 0, 0, 0, amqp_empty_table);
    if (lg_rabbitmq_get_rpc_reply(conn))
	return -1;

    for (;;) {

	amqp_maybe_release_buffers(conn);

	amqp_rpc_reply_t res = amqp_consume_message(conn, &envelope, NULL, 0);
	if (AMQP_RESPONSE_NORMAL == res.reply_type) {

	    call_function(
		    (char *)envelope.message.body.bytes,
		    (int)envelope.message.body.len, 
		    arg);

	    amqp_destroy_envelope(&envelope);
	}

	fprintf(stdout, "---->[%d][%d]\n", AMQP_RESPONSE_NORMAL, res.reply_type);
    }

    return 0;
}

int lg_rabbitmq_send(amqp_connection_state_t conn ,const char *queuename, char *buffer)
{

    return amqp_basic_publish(conn,
	    1,
	    amqp_empty_bytes,
	    amqp_cstring_bytes(queuename),
	    0,
	    0,
	    NULL,
	    amqp_cstring_bytes(buffer));
}


