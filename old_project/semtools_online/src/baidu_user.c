
#include "baidu_user.h"

void baidu_user_time_to_string(time_t date_time, char *date_str)
{

    struct tm date_tm;
    gmtime_r(&date_time, &date_tm);
    sprintf(date_str, "%04d-%02d-%02d %02d:%02d:%02d",
	    date_tm.tm_year + 1900,
	    date_tm.tm_mon + 1,
	    date_tm.tm_mday,
	    date_tm.tm_hour,
	    date_tm.tm_min,
	    date_tm.tm_sec);
}

baidu_user_node_t *_baidu_user_find(baidu_user_t *baidu_list, int sem_id, int baidu_id)
{

    baidu_user_node_t *node = NULL;
    pthread_mutex_lock(&baidu_list->mutex);

    for (node = baidu_list->head; node; node = node->next) {

	if (node->sem_id == sem_id && node->baidu_id == baidu_id) {

	    pthread_mutex_unlock(&baidu_list->mutex);
	    return node;
	}
    }

    pthread_mutex_unlock(&baidu_list->mutex);
    return NULL;
}

baidu_user_node_t *baidu_user_find(baidu_user_t *baidu_list, int sem_id, int baidu_id)
{

    baidu_user_node_t *node = NULL;
    pthread_mutex_lock(&baidu_list->mutex);

    for (node = baidu_list->head; node; node = node->next) {

	if (node->sem_id == sem_id && node->baidu_id == baidu_id && node->del_mask == 0) {

	    pthread_mutex_unlock(&baidu_list->mutex);
	    return node;
	}
    }

    pthread_mutex_unlock(&baidu_list->mutex);
    return NULL;
}

baidu_user_node_t *baidu_user_add(baidu_user_t *baidu_list, int sem_id, int baidu_id)
{

    baidu_user_node_t *node = NULL;
    if ((node = _baidu_user_find(baidu_list, sem_id, baidu_id))) {

	node->sem_id = sem_id;
	node->baidu_id = baidu_id;
	node->last_time = time(NULL);
	node->del_mask = 0;
	node->token_type = 0;
	strcpy(node->token, "da2590c34de0e439d7ab0c783a85e223");
	if (!*node->password)node->status = 8202;
	return node;
    }

    if (!(node = malloc(sizeof(baidu_user_node_t))))
	return NULL;

    memset(node, 0, sizeof(baidu_user_node_t));
    node->sem_id = sem_id;
    node->baidu_id = baidu_id;
    node->last_time = time(NULL);
    node->del_mask = 0;
    node->token_type = 0;
    strcpy(node->token, "da2590c34de0e439d7ab0c783a85e223");
    node->status = 8202;

    pthread_mutex_lock(&baidu_list->mutex);
    node->next = baidu_list->head;
    baidu_list->head = node;
    pthread_mutex_unlock(&baidu_list->mutex);

    return node;
}

void baidu_user_del(baidu_user_t *baidu_list, int sem_id, int baidu_id)
{

    baidu_user_node_t *node = NULL;

    pthread_mutex_lock(&baidu_list->mutex);

    for (node = baidu_list->head; node; node = node->next) {

	if (node && node->sem_id == sem_id && node->baidu_id == baidu_id) {

	    node->del_mask = -1;
	    pthread_mutex_unlock(&baidu_list->mutex);
	    return;
	}
    }

    pthread_mutex_unlock(&baidu_list->mutex);
}

baidu_user_node_t *_baidu_user_semlist(baidu_user_node_t *head, baidu_user_node_t *node)
{

    baidu_user_node_t *temp = NULL;
    if (!(temp = malloc(sizeof(baidu_user_node_t))))
	return head;

    memcpy(temp, node, sizeof(baidu_user_node_t));
    temp->next = head;
    return temp;
}

baidu_user_node_t *baidu_user_semlist(baidu_user_t *baidu_list, int sem_id)
{

    baidu_user_node_t *node = NULL;
    baidu_user_node_t *temp_head = NULL;

    pthread_mutex_lock(&baidu_list->mutex);
    for (node = baidu_list->head; node; node = node->next) {

	if (node->sem_id == sem_id && node->del_mask == 0) {

	    temp_head = _baidu_user_semlist(temp_head, node);
	}
    }

    pthread_mutex_unlock(&baidu_list->mutex);

    return temp_head;
}

void baidu_user_semlist_free(baidu_user_node_t *head)
{

    baidu_user_node_t *node = head;
    baidu_user_node_t *node_free = NULL;

    while(node){

	node_free = node;
	node = node->next;
	free(node_free);
    }
}

void baidu_user_del_mask_init(baidu_user_t *baidu_list, int sem_id)
{

    baidu_user_node_t *node = NULL;
    pthread_mutex_lock(&baidu_list->mutex);

    for (node = baidu_list->head; node; node = node->next) {

	if (node->sem_id == sem_id) {

	    node->del_mask = -1;
	}
    }

    pthread_mutex_unlock(&baidu_list->mutex);
}

typedef	void(soap_baidu_account_info_call)(char **domain, int domain_size, void *arg);
int soap_baidu_get_account_info(
	char *account, 
	char *password, 
	char *token, 
	soap_baidu_account_info_call *function_call,
	void *arg);

void baidu_user_get_sem_count(baidu_user_t *baidu_list, int sem_id, int *baidu_count, int *baidu_stopmarker_count)
{

    *baidu_count = 0;
    *baidu_stopmarker_count = 0;

    baidu_user_node_t *node = NULL;
    pthread_mutex_lock(&baidu_list->mutex);

    for (node = baidu_list->head; node; node = node->next) {

	if (node->sem_id == sem_id) {

	    *baidu_count = *baidu_count + 1;

	    if (node->stop_marker != 0)
		*baidu_stopmarker_count = *baidu_stopmarker_count + 1;
	}
    }

    pthread_mutex_unlock(&baidu_list->mutex);
}

void baidu_user_domain_synchronous_call(char **domain, int domain_size, void *arg)
{

    int i;
    char *domain_buffer = arg;
    for (i = 0; i < domain_size; i++) {

	if (i == 0) {

	    strcpy(domain_buffer, domain[i]);

	} else {

	    strcat(domain_buffer, ",");
	    strcat(domain_buffer, domain[i]);
	}
    }
}

void baidu_user_domain_synchronous(baidu_user_t *baidu_list, int sem_id)
{

    baidu_user_node_t *node = NULL;
    pthread_mutex_lock(&baidu_list->mutex);

    for (node = baidu_list->head; node; node = node->next) {

	if (node->sem_id == sem_id && node->del_mask == 0 && *node->password) {

	    soap_baidu_get_account_info(
		    node->account,
		    node->password,
		    "da2590c34de0e439d7ab0c783a85e223",
		    baidu_user_domain_synchronous_call,
		    node->domain);
	}
    }

    pthread_mutex_unlock(&baidu_list->mutex);
}

baidu_user_t *baidu_user_init(void)
{

    baidu_user_t *baidu_list = NULL;
    if (!(baidu_list = malloc(sizeof(baidu_user_t))))
	return NULL;

    if (pthread_mutex_init(&baidu_list->mutex, NULL) != 0) {

	free(baidu_list);
	return NULL;
    }

    baidu_list->head = NULL;
    return baidu_list;
}

int baidu_user_check_time(baidu_user_node_t *node)
{

    time_t system_time = 0;
    if ((system_time = time(NULL) == -1))
	return -1;

    return (system_time - node->check_time);
}

void baidu_user_free(baidu_user_t *baidu_list)
{

    baidu_user_node_t *node = baidu_list->head;
    baidu_user_node_t *node_free = NULL;
    baidu_list->head = NULL;

    while(node){

	node_free = node;
	node = node->next;
	free(node_free);
    }

    pthread_mutex_destroy(&baidu_list->mutex);
    free(baidu_list);
}

void baidu_user_load(baidu_user_t *baidu_list, char *path)
{

    int fd = -1;
    if ((fd = open(path, O_RDONLY)) < 0)
	return;

    baidu_user_node_t buffer;
    baidu_user_node_t *node = NULL;
    baidu_user_node_t *head = NULL;

    while (read(fd, &buffer, sizeof(baidu_user_node_t)) > 0) {

	if (!(node = malloc(sizeof(baidu_user_node_t)))) {

	    close(fd);
	    return;
	}

	memcpy(node, &buffer, sizeof(baidu_user_node_t));
	node->next = head;
	head = node;
    }

    baidu_list->head = head;
}

void baidu_user_save(baidu_user_t *baidu_list, char *path)
{

    int fd = -1;
    if ((fd = open(path, O_WRONLY | O_TRUNC | O_CREAT, 0755)) == -1)
	return;

    baidu_user_node_t *node = NULL;
    pthread_mutex_lock(&baidu_list->mutex);

    for (node = baidu_list->head; node; node = node->next) {

	if (write(fd, node, sizeof(baidu_user_node_t)) == -1) {

	    close(fd);
	    return;
	}
    }

    pthread_mutex_unlock(&baidu_list->mutex);
}

void baidu_user_print(baidu_user_t *baidu_list)
{

    baidu_user_node_t *node = NULL;
    for (node = baidu_list->head; node; node = node->next)
	fprintf(stdout, "[%u][%s][%s][%d]\n",
		node->sem_id, 
		node->account,
		node->domain,
		node->status);

    fprintf(stdout, "==============\n");
}



