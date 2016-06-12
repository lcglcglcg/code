
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <stdint.h>
#include <amqp_tcp_socket.h>
#include <amqp.h>
#include <amqp_framing.h>

#include <assert.h>
#include "utils.h"

char *host = "127.0.0.1";
char *user = "lcglcg";
char *pass = "123456";

int main(int argc, char **argv)
{

    char *queuename = argv[1];
    amqp_connection_state_t conn = amqp_new_connection();
    amqp_socket_t *socket = amqp_tcp_socket_new(conn);
    if (!socket) die("creating TCP socket");

    int status = amqp_socket_open(socket, host, 5672);
    if (status) die("opening TCP socket");

    die_on_amqp_error(amqp_login(conn, "/", 0, 131072, 0, AMQP_SASL_METHOD_PLAIN, user, pass), "Logging in");
    amqp_channel_open(conn, 1);
    die_on_amqp_error(amqp_get_rpc_reply(conn), "Opening channel");

    amqp_basic_consume(conn, 1, amqp_cstring_bytes(queuename), amqp_empty_bytes, 0, 0, 0, amqp_empty_table);
    die_on_amqp_error(amqp_get_rpc_reply(conn), "Consuming");


    int n = 0;
    for (;;) {

	amqp_maybe_release_buffers(conn);

	amqp_envelope_t envelope;
	amqp_rpc_reply_t res = amqp_consume_message(conn, &envelope, NULL, 0);
	if (AMQP_RESPONSE_NORMAL != res.reply_type)
	    return -1;

#if 1
	fprintf(stdout, "[%d]%.*s\n", ++n,
		(int)envelope.message.body.len, 
		(char *)envelope.message.body.bytes);
#endif

	amqp_destroy_envelope(&envelope);
    }

    //end loop

    amqp_channel_close(conn, 1, AMQP_REPLY_SUCCESS);
    amqp_connection_close(conn, AMQP_REPLY_SUCCESS);
    amqp_destroy_connection(conn);
    return 0;
}



