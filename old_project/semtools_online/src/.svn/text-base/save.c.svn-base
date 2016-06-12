
#include "sem.h"
#include "soap.h"
#include "pack_manager.h"
#include "sem_key.h"

typedef struct {

    network_epoll_t *io_ep;
    network_epoll_t *listen_ep;

    calendar_t *baidu_calendar;
    calendar_t *zoosnet_calendar;
    baidu_user_t *baidu_list;
    task_list_t *task;

    thread_pool_t *pool;

    int key_user_id;
    int key_status;

}thread_arg_t;

void signal_function(int sig){}
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
    thread_pool_t *head = th_arg->pool;
    task_list_t *task = th_arg->task;
    calendar_t *baidu_calendar = th_arg->baidu_calendar;
    calendar_t *zoosnet_calendar = th_arg->zoosnet_calendar;

    while(1){

	//int id = 0;
	lg_list_t *p = NULL;
	thread_pool_node *node = NULL;
	lg_list_for_each (p, &head->head) {

	    node = rb_entry(p, thread_pool_node, list);
	    //    if (node->thread_status)
	    //	fprintf(stdout, "id=%d tid=%lu thread_status=%u\n",
	    //		id++, node->tid, node->thread_status);
	}

	//if (!id)fprintf(stdout, "not thread run!\n");
	sleep(5);

	calendar_save(baidu_calendar, "baidu_calendar");
	calendar_save(zoosnet_calendar, "zoosnet_calendar");
	task_list_save(task, TASK_LIST_PATH);
    }
}

int _main(int argc, char *argv[])
{

#if 0
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

    thread_arg.baidu_calendar = calendar_init();
    thread_arg.zoosnet_calendar = calendar_init();
    thread_arg.baidu_list = baidu_user_init();
    thread_arg.task = task_list_init();
    task_list_load(thread_arg.task, TASK_LIST_PATH);


    thread_pool_t *pool = thread_pool_create(NULL, io_processor_thread, &thread_arg);
    thread_arg.pool = pool;

    calendar_add(thread_arg.baidu_calendar, 332, calendar_string_to_time("2013-06-20"));
    calendar_add(thread_arg.baidu_calendar, 332, calendar_string_to_time("2013-06-21"));
    calendar_add(thread_arg.baidu_calendar, 332, calendar_string_to_time("2013-06-22"));
    calendar_add(thread_arg.baidu_calendar, 332, calendar_string_to_time("2013-06-23"));
    calendar_add(thread_arg.baidu_calendar, 332, calendar_string_to_time("2013-06-24"));
    calendar_add(thread_arg.baidu_calendar, 332, calendar_string_to_time("2013-06-25"));
    calendar_add(thread_arg.baidu_calendar, 332, calendar_string_to_time("2013-06-26"));
    calendar_add(thread_arg.baidu_calendar, 326, calendar_string_to_time("2013-06-20"));
    calendar_add(thread_arg.baidu_calendar, 326, calendar_string_to_time("2013-06-21"));
    calendar_add(thread_arg.baidu_calendar, 326, calendar_string_to_time("2013-06-22"));
    calendar_add(thread_arg.baidu_calendar, 326, calendar_string_to_time("2013-06-23"));
    calendar_add(thread_arg.baidu_calendar, 326, calendar_string_to_time("2013-06-24"));
    calendar_add(thread_arg.baidu_calendar, 326, calendar_string_to_time("2013-06-25"));
    calendar_add(thread_arg.baidu_calendar, 326, calendar_string_to_time("2013-06-26"));

    calendar_add(thread_arg.zoosnet_calendar, 146, calendar_string_to_time("2013-06-21"));
    calendar_add(thread_arg.zoosnet_calendar, 146, calendar_string_to_time("2013-06-22"));
    calendar_add(thread_arg.zoosnet_calendar, 146, calendar_string_to_time("2013-06-23"));
    calendar_add(thread_arg.zoosnet_calendar, 146, calendar_string_to_time("2013-06-24"));
    calendar_add(thread_arg.zoosnet_calendar, 146, calendar_string_to_time("2013-06-25"));

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

unsigned int string_to_number(char *buffer)
{

    int i;
    MD5_CTX ctx = {0};
    unsigned char md[16] = {0};

    MD5_Init(&ctx);
    MD5_Update(&ctx, buffer, strlen(buffer));
    MD5_Final(md, &ctx);

    unsigned int count = 0;
    for (i = 0; i < 16; i++)
	count = count * 10 + md[i];

    return count;
}

void baidu_list_key_del(sem_key_user_t *semkey, baidu_user_t *baidu_list, baidu_user_node_t *baidu_node)
{

    sem_key_user_t *key_node = NULL;
    for (key_node = semkey; key_node; key_node = key_node->next) {

	if (strcmp(key_node->str, baidu_node->account) == 0)
	    return;
    }

    baidu_user_del(baidu_list, baidu_node->sem_id, baidu_node->baidu_id);
}

int main(int argc, char *argv[])
{

    baidu_user_t *baidu_list = baidu_user_init();
    baidu_user_load(baidu_list, "baidu_user_list");

    for(;;sleep(5))
	sem_key_packet(baidu_list, 123456);
    return 0;

    sem_key_user_t *semkey = NULL;
    semkey = sem_key_init("/etc/semkey.dat");

    sem_key_user_t *key_node = NULL;
    baidu_user_node_t *baidu_node = NULL;
    for (key_node = semkey->next; key_node; key_node = key_node->next) {

	fprintf(stdout, "%s\n", key_node->str);
	int baidu_id = string_to_number(key_node->str);
	baidu_node = baidu_user_add(baidu_list, 111, baidu_id);
	strcpy(baidu_node->account, key_node->str);

#if 0
	if (strcmp(baidu_node->account, "sgszzx") == 0)
	    strcpy(baidu_node->password, "Esz3g=p3z");
	else if (strcmp(baidu_node->account, "newUfstoneTest004") == 0)
	    strcpy(baidu_node->password, "newUfstoneTest004");
	else if (strcmp(baidu_node->account, "newUfstoneTest003") == 0)
	    strcpy(baidu_node->password, "newUfstoneTest003");
	else if (strcmp(baidu_node->account, "newUfstoneTest002") == 0)
	    strcpy(baidu_node->password, "newUfstoneTest002");
#endif

    }

    for (baidu_node = baidu_list->head; baidu_node; baidu_node = baidu_node->next) {

	baidu_list_key_del(semkey, baidu_list, baidu_node);
    }

    baidu_user_domain_synchronous(baidu_list);
    baidu_user_save(baidu_list, "baidu_user_list");
    baidu_user_print(baidu_list);

    //void baidu_download(baidu_user_t *baidu_list);
    //baidu_download(baidu_list);

    return 0;
}

