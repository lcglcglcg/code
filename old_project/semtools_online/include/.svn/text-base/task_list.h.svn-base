
#ifndef	__TASK_LIST_H__
#define	__TASK_LIST_H__

#include "sem.h"
#include "lg_list.h"

typedef	struct task_list_t task_list_t;
typedef	struct task_list_user_t task_list_user_t;
typedef	struct task_list_node_t task_list_node_t;

struct task_list_node_t {

    int sem_id;
    int task_id;
    char task_name[256];
    //char create_ip[20];

    time_t create_time;
    time_t completion_time;
    int status;

    task_list_node_t *next;
};

struct task_list_user_t {

    int sem_id;
    int index_max;
    int count_max;

    pthread_mutex_t lock;
    task_list_node_t *head;
    task_list_user_t *next;
};

struct task_list_t {

    pthread_mutex_t lock;
    task_list_user_t *head;
};

extern task_list_t *task_list_init(void);
extern void task_list_free(task_list_t *task_list);
extern time_t task_list_string_to_time(char *date_str);
extern void task_list_time_to_string(time_t date_time, char *date_str);
extern task_list_node_t *task_list_node_find(task_list_user_t *user, int task_id);
extern task_list_node_t *task_list_node_add(task_list_user_t *user, char *task_name);
extern int task_list_node_del(task_list_user_t *user, int task_id);
extern task_list_user_t *task_list_user_find(task_list_t *task, int sem_id);
extern task_list_user_t *task_list_user_add(task_list_t *task_list, int sem_id);
extern void task_list_set_status(task_list_t *task, int sem_id, int task_id, int status);
extern int task_list_del(task_list_t *task, int sem_id, int task_id);
extern void task_list_save(task_list_t *task, char *path);
extern void task_list_load(task_list_t *task, char *path);

#endif


