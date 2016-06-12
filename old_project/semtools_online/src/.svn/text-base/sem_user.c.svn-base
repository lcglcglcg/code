
#include "sem_user.h"

sem_user_node_t *sem_user_find(sem_user_t *head, int sem_id, char *account)
{

    lg_list_t *p = NULL;
    sem_user_node_t *node = NULL;

    pthread_mutex_lock(&head->mutex);
    lg_list_for_each (p, &head->head) {

	node = rb_entry(p, sem_user_node_t, node);
	if (node->sem_id == sem_id 
		&& strcmp(node->account, account) == 0) {

	    pthread_mutex_unlock(&head->mutex);
	    return node;
	}
    }

    pthread_mutex_unlock(&head->mutex);
    return NULL;
}

void sem_user_add(sem_user_t *head, int sem_id, char *account)
{

    sem_user_node_t *node = NULL;
    if (sem_user_find(head, sem_id, account))
	return;

    if (!(node = malloc(sizeof(sem_user_t))))
	return;

    memset(node, 0, sizeof(sem_user_t));
    node->sem_id = sem_id;
    strcpy(node->account, account);
    
    pthread_mutex_lock(&head->mutex);
    lg_list_add(&node->node, &head->head);
    pthread_mutex_unlock(&head->mutex);
}

void sem_user_del(sem_user_t *head, int sem_id, char *account)
{

    sem_user_node_t *node = NULL;
    if (!(node = sem_user_find(head, sem_id, account)))
	return;

    pthread_mutex_lock(&head->mutex);
    lg_list_del(&node->node);
    pthread_mutex_unlock(&head->mutex);
    
    free(node);
}

void sem_user_list(sem_user_t *head, int sem_id, sem_user_list_backcall *backcall, void *arg)
{

    lg_list_t *p = NULL;
    sem_user_node_t *node = NULL;
    
    pthread_mutex_lock(&head->mutex);
    lg_list_for_each (p, &head->head) {

	node = rb_entry(p, sem_user_node_t, node);
	if (node->sem_id == sem_id)
	    if (backcall)
		backcall(node, arg);
    }
    
    pthread_mutex_unlock(&head->mutex);
}

sem_user_t *sem_user_init(void)
{

    sem_user_t *head = NULL;
    if (!(head = malloc(sizeof(sem_user_t))))
	return NULL;

    if (pthread_mutex_init(&head->mutex, NULL) != 0) {

	free(head);
	return NULL;
    }

    INIT_LIST_HEAD(&head->head);
    return head;
}

void sem_user_free(sem_user_t *head)
{

    lg_list_t *p = (&head->head)->next;
    lg_list_t *root = &head->head;
    lg_list_t *p_free = NULL;
    sem_user_node_t *node = NULL;
    
    pthread_mutex_lock(&head->mutex);

    while(p != root){

	p_free = p;
	p = p->next;
	node = rb_entry(p_free, sem_user_node_t, node);
	lg_list_del(&node->node);
	free(node);
    }
    
    pthread_mutex_unlock(&head->mutex);

    pthread_mutex_destroy(&head->mutex);
    free(head);
}

void sem_user_print(sem_user_t *head)
{

    lg_list_t *p = NULL;
    sem_user_node_t *node = NULL;
    lg_list_for_each (p, &head->head) {

	node = rb_entry(p, sem_user_node_t, node);
	fprintf(stdout, "semid:%u account:%s\n", node->sem_id, node->account);
    }
}

