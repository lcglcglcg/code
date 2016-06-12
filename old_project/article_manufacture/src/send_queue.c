
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <curl/curl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <libxml/parser.h>

#include "lg_rabbitmq.h"
#include "lg_thread_pool.h"
#include "lg_config_file.h"

const char *rabbitmq_host = "127.0.0.1";
const char *rabbitmq_user = "lcglcg";
const char *rabbitmq_pass = "123456";

char *buffer = 
"<task>"
"<task_id>001</task_id>"
"<task_queue>lcglcg</task_queue>"
"<task_content>"
"大家都知道，高血压患者应喝绿茶，但一些高血压患者和普通的茶，不想喝，但他还可以有更多的选择，例如，一个黑色的茶，不仅能降低血脂，还可以减少血栓的形成，与普通茶、效率和完整的，以下10点来介绍你的预防高血压的保健茶。"
"</task_content>"
"</task>";

void task_rabbitmq_send(char *queue_name, char *buffer)
{

    struct tm tm;
    time_t t = time(NULL);
    localtime_r(&t, &tm);

    int ret = -1;
    amqp_connection_state_t conn = lg_rabbitmq_conn(rabbitmq_host, rabbitmq_user, rabbitmq_pass);
    if (conn) {

	amqp_queue_declare(conn, 1, amqp_cstring_bytes(queue_name), 0, 0, 0, 1, amqp_empty_table);

	int i;
	for (i = 0; i < 1000000; i++) {
	    ret = amqp_basic_publish(conn,
		    1,
		    amqp_empty_bytes,
		    amqp_cstring_bytes(queue_name),
		    0,
		    0,
		    NULL,
		    amqp_cstring_bytes(buffer));
	}
	lg_rabbitmq_close(conn);
    }

    if (ret) {

	fprintf(stdout, "[%04d-%02d-%02d %02d:%02d:%02d][%s][%s]\n",
		tm.tm_year + 1900,
		tm.tm_mon + 1,
		tm.tm_mday,
		tm.tm_hour,
		tm.tm_min,
		tm.tm_sec,
		queue_name,
		amqp_error_string2(ret));

    } else {

	fprintf(stdout, "[%04d-%02d-%02d %02d:%02d:%02d][%s][success]\n",
		tm.tm_year + 1900,
		tm.tm_mon + 1,
		tm.tm_mday,
		tm.tm_hour,
		tm.tm_min,
		tm.tm_sec,
		queue_name);
    }

    fflush(stdout);
}

int main()
{

    task_rabbitmq_send("pseudo-original", buffer);
    // task_rabbitmq_send("demo1", buffer);
    return 0;
}


