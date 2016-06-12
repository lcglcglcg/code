
#ifndef	__CALENDAR_H__
#define	__CALENDAR_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "lg_list.h"
#include "sem.h"

typedef	struct calendar_t calendar_t;
typedef	struct calendar_user_t calendar_user_t;
typedef	struct calendar_node_t calendar_node_t;

struct calendar_node_t {

    time_t date;
    calendar_node_t *next;
};

struct calendar_user_t {

    int user_id;
    pthread_mutex_t lock;
    calendar_user_t *next;
    calendar_node_t *head;
};

struct calendar_t {

    pthread_mutex_t lock;
    calendar_user_t *head;
};

extern char *strptime(const char *s, const char *format, struct tm *tm);
extern time_t calendar_string_to_time(char *date_str);
extern void calendar_time_to_string(time_t date_time, char *date_str);

extern calendar_t *calendar_init(void);
extern void calendar_free(calendar_t *calendar);
extern void calendar_node_add(calendar_user_t *user, time_t daet);
extern calendar_user_t *calendar_user_add(calendar_t *calendar, int user_id);
extern void calendar_add(calendar_t *calendar, int user_id, time_t date);

extern void calendar_save(calendar_t *calendar, char *path);
extern void calendar_load(calendar_t *calendar, char *path);

#endif






