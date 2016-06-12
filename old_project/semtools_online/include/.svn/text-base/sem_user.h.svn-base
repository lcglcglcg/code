
#ifndef	__SEM_USER_H__
#define	__SEM_USER_H__

#include "sem.h"
#include "lg_list.h"

typedef	struct {

    int sem_id;
    char account[256];
    char password[256];
    int stop_marker;
    time_t last_time;

    lg_list_t node;

}sem_user_node_t;

typedef	struct {

    lg_list_t head;
    pthread_mutex_t mutex;

}sem_user_t;

typedef	void (sem_user_list_backcall)(sem_user_node_t *node, void *arg);

#endif


