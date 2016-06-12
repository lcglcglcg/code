
#ifndef	__SEMKEY_H__
#define	__SEMKEY_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <netdb.h>
#include "baidu_user.h"

typedef struct sem_key_user_t sem_key_user_t;
struct sem_key_user_t {

    int type;
    char str[256];
    sem_key_user_t *next;
};

extern sem_key_user_t *sem_key_init(char *path);
extern void sem_key_free(sem_key_user_t *head);
extern void sem_key_coding(char *buffer, int size);
extern void sem_key_decoding(char *buffer, int size);
extern int sem_key_packet(baidu_user_t *baidu_list, int sem_id, char *user_id);

#endif



