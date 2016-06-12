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

#include "thread_pool.h"

//static pthread_cond_t lg_th_static_cond = PTHREAD_COND_INITIALIZER;
//static pthread_mutex_t lg_th_static_lock = PTHREAD_MUTEX_INITIALIZER;

thread_pool_node *thread_pool_add(thread_pool_t *pool, 
	thread_pool_node *node, 
	pthread_attr_t *attr, 
	thread_pool_call_function *function, 
	void *arg)
{

    if (!node) {

	if (!(node = malloc(sizeof(thread_pool_node))))
	    return NULL;
	bzero(node, sizeof(thread_pool_node));
    }

    node->sem = &pool->sem;
    node->cond = &pool->cond;
    node->mutex = &pool->mutex;
    node->queue = &pool->queue;

    node->error = 0;
    node->thread_status = 0;

    node->tid = 0;
    node->attr = pool->attr;
    node->function = pool->function;
    node->arg = pool->arg;

    if (attr) node->attr = attr;
    if (function) node->function = function;
    if (arg) node->arg = arg;

    //pipe2(pipefd, SOCK_NONBLOCK | SOCK_CLOEXEC);
    lg_list_add(&node->list, &pool->head);
    return node;
}

thread_pool_t *thread_pool_create(pthread_attr_t *attr,
	thread_pool_call_function *function, 
	void *arg)
{

    thread_pool_t *pool = NULL;
    if (!(pool = malloc(sizeof(thread_pool_t))))
	return NULL;

    thread_pool_node *pool_list = NULL;
    if (!(pool_list = malloc(sizeof(thread_pool_node) * THREAD_POLL_LEN))) {

	free(pool);
	return NULL;
    }

    bzero(pool, sizeof(thread_pool_t));
    bzero(pool_list, sizeof(thread_pool_node));

    sem_init(&pool->sem, 0, 0);
    pthread_cond_init(&pool->cond, NULL);
    pthread_mutex_init(&pool->mutex, NULL);

    pool->attr = attr;
    pool->function = function;
    pool->arg = arg;

    INIT_LIST_HEAD(&pool->head);

    int i;
    for (i = 0; i < THREAD_POLL_LEN; i++)
	thread_pool_add(pool, &pool_list[i], NULL, NULL, NULL);

    return pool;
}

void thread_pool_free(thread_pool_t *pool)
{

    //lg_list_del(&node->list);
    //sem_destroy(&node->sem);
    //lg_free(node);
}

void thread_pool_start(thread_pool_t *pool)
{

    lg_list_t *p = NULL;
    thread_pool_node *node = NULL;
    lg_list_for_each (p, &pool->head) {

	node = rb_entry(p, thread_pool_node, list);
	if (node && node->function)
	    node->error = pthread_create(&node->tid, 
		    node->attr,
		    node->function,
		    node);
    }
}

void thread_pool_stop(thread_pool_t *pool)
{

    lg_list_t *p = NULL;
    thread_pool_node *node = NULL;
    lg_list_for_each (p, &pool->head) {

	node = rb_entry(p, thread_pool_node, list);
	if (node && node->function)
	    node->error = pthread_cancel(node->tid);
    }
}

void thread_pool_join(thread_pool_t *pool)
{

    lg_list_t *p = NULL;
    thread_pool_node *node = NULL;
    lg_list_for_each (p, &pool->head) {

	node = rb_entry(p, thread_pool_node, list);
	if (node && node->function) {

	    node->error = pthread_join(node->tid, NULL);
	    fprintf(stdout, "join tid=%lu\n", node->tid);
	}
    }
}

void thread_pool_status(thread_pool_t *pool)
{

    int i = 0;
    lg_list_t *p = NULL;
    thread_pool_node *node = NULL;
    fprintf(stdout, "id, tid, address, status\n");
    lg_list_for_each (p, &pool->head) {

	node = rb_entry(p, thread_pool_node, list);
	fprintf(stdout, "%d, %ld,%p,%d\n", ++i, node->tid, node, node->thread_status);
    }
}






