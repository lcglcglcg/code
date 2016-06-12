
#ifndef	__BAIDU_USER_H__
#define	__BAIDU_USER_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

typedef	struct baidu_user_node_t baidu_user_node_t;
struct baidu_user_node_t {

    int sem_id;
    int baidu_id;
    char account[128];
    char password[128];
    char web_name[256];
    char domain[1024];

    char company_name[256];
    char contact_str[256];
    char contact_tel[256];

    char token[64];
    int token_type;

    time_t last_time;
    time_t check_time;
    int stop_marker;
    int del_mask;
    int status;

    baidu_user_node_t *next;
};

typedef	struct {

    pthread_mutex_t mutex;
    baidu_user_node_t *head;

}baidu_user_t;

extern baidu_user_t *baidu_user_init(void);
extern void baidu_user_free(baidu_user_t *head);
extern void baidu_user_time_to_string(time_t date_time, char *date_str);

extern baidu_user_node_t *baidu_user_add(baidu_user_t *baidu_list, int sem_id, int baidu_id);
extern void baidu_user_del(baidu_user_t *head, int sem_id, int baidu_id);
extern baidu_user_node_t *baidu_user_find(baidu_user_t *head, int sem_id, int baidu_id);
extern int baidu_user_check_time(baidu_user_node_t *node);
extern void baidu_user_print(baidu_user_t *baidu_list);

extern baidu_user_node_t *baidu_user_semlist(baidu_user_t *baidu_list, int sem_id);
extern void baidu_user_semlist_free(baidu_user_node_t *head);

extern void baidu_user_del_mask_init(baidu_user_t *baidu_list, int sem_id);
extern void baidu_user_get_sem_count(baidu_user_t *baidu_list, int sem_id, int *baidu_count, int *baidu_stopmarker_count);
extern void baidu_user_domain_synchronous(baidu_user_t *baidu_list, int sem_id);

extern void baidu_user_load(baidu_user_t *baidu_list, char *path);
extern void baidu_user_save(baidu_user_t *baidu_list, char *path);

#endif


