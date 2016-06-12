
#ifndef	__TASK_LIST_H__
#define	__TASK_LIST_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "lg_list.h"

typedef struct {

    char url_str[2048];
    char link_str[2048];
    char time_str[128];
    lg_list_t node;

}task_list_node;

extern void task_list_add(char *url_str, char *link_str, char *delay_time);
extern int task_list_next(char *url_str, char *link_str, task_list_node *temp);
extern size_t task_list_get_buffer(char *buffer);
extern void task_list_free();
extern void task_list_print();

#endif


