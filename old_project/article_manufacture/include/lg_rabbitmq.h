

#ifndef __LG_LG_RABBITMQ_H__
#define	__LG_LG_RABBITMQ_H__

#include <amqp.h>
#include <amqp_tcp_socket.h>
#include <amqp_framing.h>

#define RABBITMQ_PORT 5672

#ifdef __cplusplus
extern "C" { 
#endif

    typedef int (lg_rabbitmq_recv_function_t)(char *buffer, int size, void *arg);

    extern amqp_connection_state_t lg_rabbitmq_conn(const char *host, const char *user, const char *pass);
    extern void lg_rabbitmq_close(amqp_connection_state_t conn);

    extern int lg_rabbitmq_recv(amqp_connection_state_t conn ,const char *queuename, lg_rabbitmq_recv_function_t *call_function, void *arg);
    extern int lg_rabbitmq_send(amqp_connection_state_t conn ,const char *queuename, char *buffer);

#ifdef __cplusplus
}
#endif

#endif


