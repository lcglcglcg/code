
/*
 * pthread_create()创建线程
 * pthread_exit()终止当前线程
 * pthread_cancel()中断另外一个线程的运行
 * pthread_join()阻塞当前的线程,直到另外一个线程运行结束
 * pthread_attr_init()初始化线程的属性
 * pthread_attr_setdetachstate()设置脱离状态的属性(决定这个线程在终止时是否可以被结合)
 * pthread_attr_getdetachstate()获取脱离状态的属性
 * pthread_attr_destroy()删除线程的属性
 * pthread_kill()向线程发送一个信号
 * pthread_mutex_init()初始化互斥锁
 * pthread_mutex_destroy()删除互斥锁
 * pthread_mutex_lock()占有互斥锁(阻塞操作)
 * pthread_mutex_trylock()试图占有互斥锁(不阻塞)即当互斥锁空闲时将占有该锁否则立即返回
 * pthread_mutex_unlock()释放互斥锁
 * pthread_cond_init()初始化条件变量
 * pthread_cond_destroy()销毁条件变量
 * pthread_cond_signal()唤醒第一个调用pthread_cond_wait()而进入睡眠的线程
 * pthread_cond_wait()等待条件变量的特殊条件发生
 * pthread_key_create()分配用于标识进程中线程特定数据的键
 * pthread_setspecific()为指定线程特定数据键设置线程特定绑定
 * pthread_getspecific()获取调用线程的键绑定,并将该绑定存储在value指向的位置
 * pthread_key_delete()销毁现有线程特定数据
 * pthread_equal()对两个线程的线程标识号进行比较
 * pthread_detach()分离线程
 * pthread_self()查询线程自身线程标识号
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "lg_thread_pool.h"

//pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

lg_thread_pool_t *lg_thread_pool_create(
	int thread_pool_max,
	lg_thread_function_t *function,
	pthread_attr_t *attr)
{

    lg_thread_pool_t *pool = NULL;
    if (!(pool = (lg_thread_pool_t *)malloc(sizeof(lg_thread_pool_t))))
	return NULL;

    sem_init(&pool->sem, 0, 0);
    pthread_mutex_init(&pool->mutex, NULL);
    pthread_cond_init(&pool->cond, NULL);
    lg_list_init(&pool->head);

    int i;
    for (i = 0; i < thread_pool_max; i++) {

	lg_thread_node_t *node = NULL;
	if (!(node = (lg_thread_node_t *)malloc(sizeof(lg_thread_node_t))))
	    return NULL;

	memset(node, 0, sizeof(lg_thread_node_t));
	lg_list_add(&pool->head, &node->next);
	node->function = function;
	node->attr = attr;
    }

    srand(time(NULL));
    return pool;
}

int lg_thread_pool_start(lg_thread_pool_t *pool)
{

    lg_list_t *p = NULL;
    lg_list_for_each (p, &pool->head) {

	lg_thread_node_t *node = (lg_thread_node_t *)p;
	node->errcode = pthread_create(&node->tid,
		node->attr,
		(void *(*)(void *))node->function,
		node);

	if (node->errcode)
	    return node->errcode;
    }

    return 0;
}

int lg_thread_pool_stop(lg_thread_pool_t *pool)
{

    lg_list_t *p = NULL;
    lg_list_for_each (p, &pool->head) {

	lg_thread_node_t *node = (lg_thread_node_t *)p;
	node->errcode = pthread_cancel(node->tid);

	if (node->errcode)
	    return node->errcode;
    }

    return 0;
}

void lg_thread_pool_join(lg_thread_pool_t *pool)
{

    lg_list_t *p = NULL;
    lg_list_for_each (p, &pool->head) {

	lg_thread_node_t *node = (lg_thread_node_t *)p;
	node->errcode = pthread_join(node->tid, NULL);
    }
}



