
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <hiredis/hiredis.h>

#include "lg_redis.h"
#include "lg_election.h"
#include "lg_rabbitmq.h"
#include "lg_thread_pool.h"
#include "lg_config_file.h"
#include "disorderly.h"

int task_content_create(task_t *task)
{

    int cut_size = 0;
    task->content_size = 0;
    lg_election_head_t *head = task->diseases_dict;

    for (;;) {

	lg_election_t *node = NULL;
	if (!(node = lg_election_find(head, rand() % head->total)))
	    return -1;

	memcpy(&task->content[task->content_size], node->buffer, node->size);
	task->content_size += node->size;

	if (cut_size > 700) {

	    memcpy(&task->content[task->content_size], ".\n", 2);
	    task->content_size += 2;
	    cut_size = 0;

	} else {

	    if (rand() % 2 && rand() % 2) {

		memcpy(&task->content[task->content_size], ",", 1);
		task->content_size++;
		cut_size++;
	    }
	}

	cut_size += node->size;

	if (task->content_size > 4096) {

	    memcpy(&task->content[task->content_size], ".\n", 2);
	    task->content_size += 2;
	    task->content[task->content_size] = 0;
	    return 0;
	}
    }

    return -1;
}

int task_run(redisContext *conn, task_t *task)
{

    for (;;) {

	redisReply *reply = redisCommand(conn, "llen %s", task->queue_name);
	if (reply->integer < 32) {

	    if (task_content_create(task) == 0) {

		redisReply *send_content = redisCommand(conn, "rpush %s %b", task->queue_name, task->content, task->content_size);
		if (send_content) freeReplyObject(send_content);
	    }

	    if (reply) freeReplyObject(reply);

	} else {

	    if (reply) freeReplyObject(reply);
	    return 0;
	}
    }

    return 0;
}

task_t *task_init(lg_list_t *config_file)
{

    lg_election_head_t *diseases_dict = NULL;
    if (!(diseases_dict = lg_election_load_file("dict.utf8.keyword")))
	return NULL;

    task_t *task = malloc(sizeof(task_t));
    if (!task) return NULL;

    task->host = lg_config_file_find(config_file, "host");
    task->user = lg_config_file_find(config_file, "user");
    task->pass = lg_config_file_find(config_file, "pass");
    task->diseases_dict = diseases_dict;
    return task;
}

int main()
{

    if (fork() > 0) exit(1);

    lg_list_t config_file;
    if (lg_config_file_load(&config_file, "article_manufacture.conf")) {

	fprintf(stdout, "article_manufacture.conf::err\n");
	return -1;
    }

    task_t *task = task_init(&config_file);
    if (!task) {

	fprintf(stdout, "task_init::err\n");
	return -1;
    }

    int i, diseases_code_total = sizeof(diseases_code_arr) / sizeof(int);
    for (;;) {

	redisContext *conn = redisConnect("127.0.0.1", 6379);
	if (conn) {

	    for (i = 0; i < diseases_code_total; i++) {

		task->diseases_code = diseases_code_arr[i];
		sprintf(task->routingkey, "%d", task->diseases_code);
		sprintf(task->queue_name, "%d", task->diseases_code);
		task_run(conn, task);
	    }

	    redisFree(conn);
	}

	sleep(1);
    }
}

