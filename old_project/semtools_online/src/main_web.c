
#include "sem.h"
#include "soap.h"
#include "pack_manager.h"

typedef struct {

    network_epoll_t *io_ep;
    network_epoll_t *listen_ep;

    calendar_t *baidu_calendar;
    calendar_t *zoosnet_calendar;
    baidu_user_t *baidu_list;
    task_list_t *task;

    thread_pool_t *pool;

}thread_arg_t;

void signal_function(int sig){}
extern void baidu_download(baidu_user_t *baidu_list, calendar_t *calendar);
extern void semtools_packet_analysis(
	task_list_t *task,
	calendar_t *baidu_calendar,
	calendar_t *zoosnet_calendar,
	baidu_user_t *baidu_user_list,
	network_epoll_t *io_event, 
	int sockfd, 
	char *buffer, 
	int size);

int get_sockfd_dequeue(lg_queue_t *socket_queue, pthread_mutex_t *mutex)
{

    int sockfd = -1;
    pthread_mutex_lock(mutex);
    if (lg_queue_dequeue(socket_queue,
		&sockfd,
		sizeof(int)) == -1)
	sockfd = -1;

    pthread_mutex_unlock(mutex);
    return sockfd;
}

void *io_processor_thread(void *arg)
{

    thread_pool_node *this = arg;
    thread_arg_t *th_arg = this->arg;
    network_epoll_t *io_ep = th_arg->io_ep;

    calendar_t *baidu_calendar = th_arg->baidu_calendar;
    calendar_t *zoosnet_calendar = th_arg->zoosnet_calendar;
    baidu_user_t *baidu_list = th_arg->baidu_list;
    task_list_t *task = th_arg->task;

    char buffer[2048];

    while(1){

	sem_wait(this->sem);
	this->thread_status = 1;

	int sockfd = -1;
	if ((sockfd = get_sockfd_dequeue(this->queue, this->mutex)) == -1)
	    continue;
	fprintf(stdout, "[io_processor_thread][%d]\n", sockfd);

	int size = -1;
	if ((size = network_recv(sockfd, buffer, 2048)) > 0)
	    semtools_packet_analysis(
		    task,
		    baidu_calendar,
		    zoosnet_calendar,
		    baidu_list, 
		    io_ep, 
		    sockfd, 
		    buffer, 
		    size);

	this->thread_status = 0;
    }
}

static void io_event(int sockfd, void *arg)
{

    thread_pool_node *this = arg;
    fprintf(stdout, "[io_event][%d]\n", sockfd);

    if (lg_queue_enqueue(this->queue, &sockfd, sizeof(int)) == 0)
	sem_post(this->sem);
}

void *io_thread(void *arg)
{

    thread_pool_node *this = arg;
    thread_arg_t *th_arg = this->arg;
    network_epoll_t *io_ep = th_arg->io_ep;

    io_ep->arg = arg;
    io_ep->in = io_event;
    for(;;)network_epoll(io_ep);
}

static void listen_event(int listenfd, void *arg)
{

    thread_pool_node *this = arg;
    thread_arg_t *th_arg = this->arg;
    network_epoll_t *io_ep = th_arg->io_ep;

    int sockfd = network_accept(listenfd);
    if (sockfd > 0) {

	fprintf(stdout, "[listen_event][%d]\n", sockfd);
	network_epoll_event_add(
		io_ep, 
		sockfd, 
		EPOLLET | EPOLLIN);
    }
}

void *listen_thread(void *arg)
{

    thread_pool_node *this = arg;
    thread_arg_t *th_arg = this->arg;
    network_epoll_t *listen_ep = th_arg->listen_ep;

    listen_ep->arg = arg;
    listen_ep->in = listen_event;
    for(;;)network_epoll(listen_ep);
}

void *check_run_status_thread(void *arg)
{

    thread_pool_node *this = arg;
    thread_arg_t *th_arg = this->arg;
    //thread_pool_t *head = th_arg->pool;
    task_list_t *task = th_arg->task;
    baidu_user_t *baidu_list = th_arg->baidu_list;
    calendar_t *baidu_calendar = th_arg->baidu_calendar;
    calendar_t *zoosnet_calendar = th_arg->zoosnet_calendar;


    while(1){

#if 0
	int id = 0;
	lg_list_t *p = NULL;
	thread_pool_node *node = NULL;
	lg_list_for_each (p, &head->head) {

	    node = rb_entry(p, thread_pool_node, list);
	    if (node->thread_status)
	    fprintf(stdout, "id=%d tid=%lu thread_status=%u\n",
	   id++, node->tid, node->thread_status);
	}

	if (!id)fprintf(stdout, "not thread run!\n");
#endif

	time_t day_time = time(NULL);
	struct tm day_tm = {0};
	if (localtime_r(&day_time, &day_tm))
	    if (day_tm.tm_hour == 14)
		baidu_download(baidu_list, baidu_calendar);

	task_list_save(task, TASK_LIST_PATH);
	baidu_user_save(baidu_list, BAIDU_USER_LIST_PATH);
	calendar_save(baidu_calendar, CALENDAR_BAIDU_PATH);
	calendar_save(zoosnet_calendar, CALENDAR_ZOOSNET_PATH);
	sleep(3600);
    }
}

int main(int argc, char *argv[])
{

#if 1
    pid_t pid = fork();
    if (pid < 0) {

	fprintf(stdout, "[fork() err!]\n");
	exit(0);

    } else if (pid > 0) {

	exit(0);
    }
#endif

    thread_arg_t thread_arg;
    thread_arg.io_ep = network_epoll_init(
	    NETWORK_MAX_EVENT, 
	    NETWORK_MAX_POLL, 
	    -1);

    thread_arg.listen_ep = network_epoll_init(
	    1, 
	    1, 
	    -1);

    thread_arg.task = task_list_init();
    thread_arg.baidu_list = baidu_user_init();
    thread_arg.baidu_calendar = calendar_init();
    thread_arg.zoosnet_calendar = calendar_init();

    task_list_load(thread_arg.task, TASK_LIST_PATH);
    baidu_user_load(thread_arg.baidu_list, BAIDU_USER_LIST_PATH);
    calendar_load(thread_arg.baidu_calendar, CALENDAR_BAIDU_PATH);
    calendar_load(thread_arg.zoosnet_calendar, CALENDAR_ZOOSNET_PATH);

    baidu_user_print(thread_arg.baidu_list);

    thread_pool_t *pool = thread_pool_create(NULL, io_processor_thread, &thread_arg);
    thread_arg.pool = pool;

    network_fdmax(NETWORK_MAX_FD);
    int sockfd = network_bind(NULL, 8000);
    network_epoll_event_add(thread_arg.listen_ep, sockfd, 0);
    lg_queue_create(&pool->queue, NETWORK_MAX_POLL * sizeof(int));

    thread_pool_add(pool,
	    NULL,
	    NULL,
	    io_thread,
	    &thread_arg);

    thread_pool_add(pool,
	    NULL,
	    NULL,
	    listen_thread,
	    &thread_arg);

    thread_pool_add(pool,
	    NULL,
	    NULL,
	    check_run_status_thread,
	    &thread_arg);

    signal(SIGSEGV, signal_function);
    signal(SIGPIPE, signal_function);

    thread_pool_start(pool);
    thread_pool_join(pool);

    return 0;
}



