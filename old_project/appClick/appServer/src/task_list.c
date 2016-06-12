
#include "json.h"
#include "task_list.h"

lg_list_t task_list_head = {&task_list_head, &task_list_head};
pthread_mutex_t task_list_mutex = PTHREAD_MUTEX_INITIALIZER;

task_list_node *task_list_find(char *url_str, char *link_str)
{

    lg_list_t *p = NULL;
    task_list_node *node = NULL;

    lg_list_for_each(p, &task_list_head) {

	node = rb_entry(p, task_list_node, node);
	if (strcmp(node->url_str, url_str) == 0 
		&& strcmp(node->link_str, link_str) == 0) {

	    return node;
	}
    }

    return NULL;
}

void task_list_add(char *url_str, char *link_str, char *time_str)
{

    if (!url_str || !link_str || !time_str)
	return;

    pthread_mutex_lock(&task_list_mutex);

    if (task_list_find(url_str, link_str)) {

	pthread_mutex_unlock(&task_list_mutex);
	return;
    }

    task_list_node *node = NULL;
    if (!(node = malloc(sizeof(task_list_node)))) {

	pthread_mutex_unlock(&task_list_mutex);
	return;
    }

    strcpy(node->url_str, url_str);
    strcpy(node->link_str, link_str);
    strcpy(node->time_str, time_str);
    lg_list_add(&task_list_head, &node->node);

    pthread_mutex_unlock(&task_list_mutex);
}

int task_list_next(char *url_str, char *link_str, task_list_node *temp)
{

    lg_list_t *p = NULL;
    task_list_node *node = NULL;

    pthread_mutex_lock(&task_list_mutex);

    if (lg_list_empty(&task_list_head)) {

	pthread_mutex_unlock(&task_list_mutex);
	return -2;
    }

    if (url_str == NULL || link_str == NULL) {

	node = rb_entry((&task_list_head)->next, task_list_node, node);
	memcpy(temp, node, sizeof(task_list_node));
	pthread_mutex_unlock(&task_list_mutex);
	return 0;

    }

    lg_list_for_each(p, &task_list_head) {

	node = rb_entry(p, task_list_node, node);
	if (strcmp(node->url_str, url_str) == 0 
		&& strcmp(node->link_str, link_str) == 0) {

	    if (p->next == &task_list_head) p = p->next;
	    node = rb_entry(p->next, task_list_node, node);
	    memcpy(temp, node, sizeof(task_list_node));
	    pthread_mutex_unlock(&task_list_mutex);
	    return 0;
	}
    }

    pthread_mutex_unlock(&task_list_mutex);
    return -1;
}

size_t task_list_get_buffer(char *buffer)
{

    lg_list_t *p = NULL;
    task_list_node *node = NULL;
    json_object *j_data = json_object_new_array();
    
    pthread_mutex_lock(&task_list_mutex);

    lg_list_for_each(p, &task_list_head) {

	node = rb_entry(p, task_list_node, node);

	json_object *j_temp = json_object_new_object();
	json_object_object_add(j_temp, "url", json_object_new_string(node->url_str));
	json_object_object_add(j_temp, "link", json_object_new_string(node->link_str));
	json_object_object_add(j_temp, "time", json_object_new_string(node->time_str));
	json_object_array_add(j_data, j_temp);
    }
    
    pthread_mutex_unlock(&task_list_mutex);

    json_object *j_root = json_object_new_object();
    json_object_object_add(j_root, "data", j_data);

    char *jbuffer = (char *)json_object_to_json_string(j_root);
    size_t size = strlen(jbuffer);
    memcpy(buffer, jbuffer, size);

    json_object_put(j_root);
    return size;
}

void task_list_free()
{

    task_list_node *node = NULL;
    lg_list_t *p = (&task_list_head)->next;

    pthread_mutex_lock(&task_list_mutex);

    while(p != &task_list_head) {

	node = rb_entry(p, task_list_node, node);
	p = p->next;
	free(node);
    }

    lg_list_init(&task_list_head);
    pthread_mutex_unlock(&task_list_mutex);
}

void task_list_print()
{

    lg_list_t *p = NULL;
    task_list_node *node = NULL;

    lg_list_for_each(p, &task_list_head) {

	node = rb_entry(p, task_list_node, node);
	fprintf(stdout, "[%s][%s]\n", node->url_str, node->link_str);
    }
}


