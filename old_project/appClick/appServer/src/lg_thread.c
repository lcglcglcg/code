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

#include "lg_thread.h"

//static pthread_cond_t lg_th_static_cond = PTHREAD_COND_INITIALIZER;
//static pthread_mutex_t lg_th_static_lock = PTHREAD_MUTEX_INITIALIZER;

lg_thread_pool_t *lg_thread_pool_init(
	lg_thread_pool_t *pool,
	pthread_cond_t *cond,
	pthread_mutex_t *mutex,
	lg_queue_t *queue,
	sem_t *sem)
{

    if (pool == NULL) {

	if (!(pool = malloc(sizeof(lg_thread_pool_t)))) {

	    return NULL;
	}
    }

    pool->cond = cond;
    pool->mutex = mutex;
    pool->queue = queue;
    pool->sem = sem;
    lg_list_init(&pool->head);
    return pool;
}

lg_thread_node_t *lg_thread_node_add(
	lg_thread_pool_t *pool,
	lg_thread_function *function,
	void *arg)
{

    lg_thread_node_t *node = NULL;
    if (!(node = (lg_thread_node_t *)malloc(sizeof(lg_thread_node_t))))
	return NULL;

    node->tid = 0;
    node->attr = NULL;
    node->function = function;
    node->arg = arg;

    node->pool = pool;
    node->cond = pool->cond;
    node->mutex = pool->mutex;
    node->queue = pool->queue;
    node->sem = pool->sem;

    lg_list_add(&pool->head, &node->node);
    return node;
}

int lg_thread_pool_add(int max_pool,
	lg_thread_pool_t *pool,
	lg_thread_function *function,
	void *arg)
{

    lg_thread_node_t *list_node = NULL;
    if (!(list_node = malloc(sizeof(lg_thread_node_t) * max_pool)))
	return -1;

    int i;
    for (i = 0; i < max_pool; i++) {

	list_node[i].tid = 0;
	list_node[i].attr = NULL;
	list_node[i].function = function;
	list_node[i].arg = arg;

	list_node[i].pool = pool;
	list_node[i].cond = pool->cond;
	list_node[i].mutex = pool->mutex;
	list_node[i].queue = pool->queue;
	list_node[i].sem = pool->sem;

	lg_list_add(&pool->head, &list_node[i].node);
    }

    return 0;
}

void lg_thread_pool_start(lg_thread_pool_t *pool)
{

    lg_list_t *p = NULL;
    lg_thread_node_t *node = NULL;
    lg_list_for_each (p, &pool->head) {

	node = rb_entry(p, lg_thread_node_t, node);
	if (node && node->function)
	    pthread_create(&node->tid,
		    node->attr,
		    (lg_thread_function_ *)node->function,
		    node);
    }
}

void lg_thread_pool_stop(lg_thread_pool_t *pool)
{

    lg_list_t *p = NULL;
    lg_thread_node_t *node = NULL;
    lg_list_for_each (p, &pool->head) {

	node = rb_entry(p, lg_thread_node_t, node);
	if (node && node->function)
	    pthread_cancel(node->tid);
    }
}

void lg_thread_pool_join(lg_thread_pool_t *pool)
{

    lg_list_t *p = NULL;
    lg_thread_node_t *node = NULL;
    lg_list_for_each (p, &pool->head) {

	node = rb_entry(p, lg_thread_node_t, node);
	if (node && node->function)
	    pthread_join(node->tid, NULL);
    }
}

void lg_thread_pool_free(lg_thread_pool_t *pool)
{

}

int lg_thread_pool_external_init(lg_thread_pool_t *pool, lg_network_epoll_t *ep)
{

    if (!(ep->sem = malloc(sizeof(sem_t))))
	return -1;

    if (!(ep->queue = malloc(sizeof(lg_queue_t)))) {

	free(ep->sem);
	return -1;
    }

    if (!(ep->mutex = malloc(sizeof(pthread_mutex_t)))) {

	free(ep->sem);
	free(ep->queue);
	return -1;
    }

    if (lg_queue_init(ep->queue, 40960) == NULL) {

	free(ep->sem);
	free(ep->queue);
	free(ep->mutex);
	return -1;
    }

    if (pthread_mutex_init(ep->mutex, NULL) != 0) {


	lg_queue_free(ep->queue);
	free(ep->sem);
	free(ep->queue);
	free(ep->mutex);
	return -1;
    }

    if (sem_init(ep->sem, 0, 0) != 0) {

	pthread_mutex_destroy(ep->mutex);
	lg_queue_free(ep->queue);
	free(ep->sem);
	free(ep->queue);
	free(ep->mutex);
	return -1;
    }

    pool->mutex = ep->mutex;
    pool->queue = ep->queue;
    pool->sem = ep->sem;

    return 0;
}

