
#ifndef	__LG_THREAD_POOL_H__
#define	__LG_THREAD_POOL_H__

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
#include "lg_list.h"
#include "lg_queue.h"

#define	THREAD_POLL_LEN 10

typedef	struct thread_pool_t thread_pool_t;
typedef	struct thread_pool_node thread_pool_node;
typedef void *(thread_pool_call_function)(void *arg);

struct thread_pool_node {

    sem_t *sem;
    pthread_cond_t *cond;
    pthread_mutex_t *mutex;
    lg_queue_t *queue;

    int error;
    int thread_status;
    //thread_pool_t *pool;

    pthread_t tid;
    pthread_attr_t *attr;
    thread_pool_call_function *function;
    void *arg;

    lg_list_t list;
};

struct thread_pool_t {

    sem_t sem;
    pthread_cond_t cond;
    pthread_mutex_t mutex;
    lg_queue_t queue;

    pthread_attr_t *attr;
    thread_pool_call_function *function;
    void *arg;

    lg_list_t head;
};

extern int pipe2(int pipefd[2], int flags);
extern thread_pool_node *thread_pool_add(thread_pool_t *pool,
	thread_pool_node *node, 
	pthread_attr_t *attr, 
	thread_pool_call_function *function,
	void *arg);
extern thread_pool_t *thread_pool_create(pthread_attr_t *attr, 
	thread_pool_call_function *function, 
	void *arg);
extern void thread_pool_free(thread_pool_t *pool);
extern void thread_pool_start(thread_pool_t *pool);
extern void thread_pool_stop(thread_pool_t *pool);
extern void thread_pool_join(thread_pool_t *pool);
extern void thread_pool_status(thread_pool_t *pool);

#endif// __LG_THREAD_POOL_H__


