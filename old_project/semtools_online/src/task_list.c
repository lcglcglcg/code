
#include "task_list.h"

time_t task_list_string_to_time(char *date_str)
{

    struct tm date_tm;
    memset(&date_tm, 0, sizeof(struct tm));
    if (!strptime(date_str, "%Y-%m-%d", &date_tm))
	return 0;

    return (mktime(&date_tm) + 86400);
}

void task_list_time_to_string(time_t date_time, char *date_str)
{

    struct tm date_tm;
    localtime_r(&date_time, &date_tm);
    sprintf(date_str, "%04d-%02d-%02d %02d:%02d:%02d",
	    date_tm.tm_year + 1900,
	    date_tm.tm_mon + 1,
	    date_tm.tm_mday,
	    date_tm.tm_hour,
	    date_tm.tm_min,
	    date_tm.tm_sec);
}

task_list_node_t *task_list_node_find(task_list_user_t *user, int task_id)
{

    task_list_node_t *node = NULL;
    pthread_mutex_lock(&user->lock);

    for (node = user->head; node; node = node->next) {

	if (node->task_id == task_id) {

	    pthread_mutex_unlock(&user->lock);
	    return node;
	}
    }

    pthread_mutex_unlock(&user->lock);
    return NULL;
}

task_list_node_t *task_list_node_add(task_list_user_t *user, char *task_name)
{

    task_list_node_t *node = NULL;
    if (!(node = malloc(sizeof(task_list_node_t))))
	return NULL;

    node->sem_id = user->sem_id;
    node->task_id = user->index_max++;
    strcpy(node->task_name, task_name);
    node->create_time = time(NULL);
    node->completion_time = 0;
    node->status = -1;
    user->count_max++;

    pthread_mutex_lock(&user->lock);
    node->next = user->head;
    user->head = node;
    pthread_mutex_unlock(&user->lock);

    return node;
}

int task_list_node_del(task_list_user_t *user, int task_id)
{

    task_list_node_t *node = user->head;
    task_list_node_t *prev = NULL;
    if (node == NULL)return -1;

    pthread_mutex_lock(&user->lock);

    if (node && node->task_id == task_id) {

	user->head = node->next;
	user->count_max--;
	pthread_mutex_unlock(&user->lock);
	free(node);
	return 0;
    }

    while(node){

	prev = node;
	node = node->next;

	if (node && node->task_id == task_id) {

	    prev->next = node->next;
	    user->count_max--;
	    pthread_mutex_unlock(&user->lock);
	    free(node);
	    return 0;
	}
    }

    pthread_mutex_unlock(&user->lock);
    return -1;
}

task_list_user_t *task_list_user_find(task_list_t *task, int sem_id)
{

    task_list_user_t *user = NULL;
    pthread_mutex_lock(&task->lock);

    for (user = task->head; user; user = user->next) {

	if (user->sem_id == sem_id) {

	    pthread_mutex_unlock(&task->lock);
	    return user;
	}
    }

    pthread_mutex_unlock(&task->lock);
    return NULL;
}

task_list_user_t *task_list_user_add(task_list_t *task_list, int sem_id)
{

    task_list_user_t *user = NULL;
    if ((user = task_list_user_find(task_list, sem_id)))
	return user;

    if (!(user = malloc(sizeof(task_list_user_t))))
	return NULL;

    user->head = NULL;
    user->sem_id = sem_id;
    user->index_max = 0;
    user->count_max = 0;

    if (pthread_mutex_init(&user->lock, NULL) != 0) {

	free(user);
	return NULL;
    }

    pthread_mutex_lock(&task_list->lock);
    user->next = task_list->head;
    task_list->head = user;
    pthread_mutex_unlock(&task_list->lock);

    return user;
}

void task_list_set_status(task_list_t *task, int sem_id, int task_id, int status)
{

    task_list_node_t *node = NULL;
    task_list_user_t *user = NULL;

    if (!(user = task_list_user_find(task, sem_id)))
	return;

    if (!(node = task_list_node_find(user, task_id)))
	return;

    node->status = status;
    node->completion_time = time(NULL);
}

int task_list_del(task_list_t *task, int sem_id, int task_id)
{

    task_list_user_t *user = NULL;
    if (!(user = task_list_user_find(task, sem_id)))
	return -1;

    return task_list_node_del(user, task_id);
}

void task_list_order(task_list_user_t *user)
{

    task_list_node_t *node = NULL;
    task_list_node_t *prev = NULL;
    task_list_node_t *next = NULL;
    node = user->head;

    while(node){

	next = node->next;
	node->next = prev;
	prev = node;
	node = next;
    }

    user->head = prev;
}

void task_list_load(task_list_t *task, char *path)
{

    int fd = -1;
    if ((fd = open(path, O_RDONLY)) < 0)
	return;

    task_list_user_t *user = NULL;
    task_list_node_t *node = NULL;
    task_list_node_t task_buffer;

    while (read(fd, &task_buffer, sizeof(task_list_node_t)) > 0) {

	node = malloc(sizeof(task_list_node_t));
	memcpy(node, &task_buffer, sizeof(task_list_node_t));

	user = task_list_user_add(task, node->sem_id);
	if (user->index_max < node->task_id)
	    user->index_max = node->task_id + 1;
	user->count_max++;

	node->next = user->head;
	user->head = node;
    }

    for (user = task->head; user; user = user->next)
	task_list_order(user);
}

task_list_t *task_list_init(void)
{

    task_list_t *task_list = NULL;
    if (!(task_list = malloc(sizeof(task_list_t))))
	return NULL;

    if (pthread_mutex_init(&task_list->lock, NULL) != 0) {

	free(task_list);
	return NULL;
    }

    task_list->head = NULL;
    return task_list;
}

void task_list_save(task_list_t *task_list, char *path)
{

    int fd = -1;
    if ((fd = open(path, O_WRONLY | O_TRUNC | O_CREAT, 0755)) == -1)
	return;

    task_list_node_t *node = NULL;
    task_list_user_t *user = NULL;

    pthread_mutex_lock(&task_list->lock);
    for (user = task_list->head; user; user = user->next) {

	pthread_mutex_lock(&user->lock);
	for (node = user->head; node; node = node->next) {

	    if (write(fd, node, sizeof(task_list_node_t)) == -1) {

		close(fd);
		return;
	    }
	}

	pthread_mutex_unlock(&user->lock);
    }

    pthread_mutex_unlock(&task_list->lock);
    close(fd);
}

void task_list_free(task_list_t *task_list)
{

    task_list_user_t *user = task_list->head;
    task_list_user_t *user_free = NULL;
    task_list_node_t *node = NULL;
    task_list_node_t *node_free = NULL;

    while(user){

	user_free = user;
	user = user->next;
	node = user_free->head;

	while(node){

	    node_free = node;
	    node = node->next;
	    free(node_free);
	}

	pthread_mutex_destroy(&user_free->lock);
	free(user_free);
    }

    pthread_mutex_destroy(&task_list->lock);
    free(task_list);
}

void task_list_print(task_list_t *task_list)
{

    task_list_node_t *node = NULL;
    task_list_user_t *user = NULL;

    for (user = task_list->head; user; user = user->next) {

	fprintf(stdout, "[sem_id=%d][index_max=%d][count_max=%d]\n",
		user->sem_id,
		user->index_max,
		user->count_max);

	for (node = user->head; node; node = node->next) {

	    fprintf(stdout, "[sem_id=%d][task_id=%d][task_name=%s][status=%d]\n",
		    node->sem_id,
		    node->task_id,
		    node->task_name,
		    node->status);
	}
    }
}




