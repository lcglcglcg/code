
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

    char *key_user_id;
    int key_status;

}thread_arg_t;

void signal_function(int sig){}
extern void baidu_download(baidu_user_t *baidu_list, calendar_t *calendar);
extern void baidu_download_sem(baidu_user_t *baidu_list, calendar_t *calendar, int sem_id);
extern void semtools_packet_analysis(
	task_list_t *task,
	calendar_t *baidu_calendar,
	calendar_t *zoosnet_calendar,
	baidu_user_t *baidu_user_list,
	network_epoll_t *io_event, 
	int sockfd, 
	char *buffer, 
	int size);

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

char *baidu_list_key_init(baidu_user_t *baidu_list, int sem_id)
{

    sem_key_user_t *semkey = NULL;
    if (!(semkey = sem_key_init("/etc/semkey.dat")))
	return NULL;

    char *key_user_id = NULL;
    if (!(key_user_id = malloc(4096))) {

	sem_key_free(semkey);
	return NULL;
    }

    if (!*semkey->str) {

	free(key_user_id);
	sem_key_free(semkey);
	return NULL;
    }

    strcpy(key_user_id, semkey->str);
    sem_key_user_t *key_node = NULL;
    baidu_user_node_t *baidu_node = NULL;
    for (key_node = semkey->next; key_node; key_node = key_node->next) {

	int baidu_id = string_to_number(key_node->str);
	baidu_node = baidu_user_add(baidu_list, sem_id, baidu_id);
	strcpy(baidu_node->account, key_node->str);
	strcat(key_user_id, key_node->str);

	fprintf(stdout, "%s:%u\n", key_node->str, baidu_id);
    }

    for (baidu_node = baidu_list->head; baidu_node; baidu_node = baidu_node->next)
	baidu_list_key_del(semkey, baidu_list, baidu_node);

    baidu_user_domain_synchronous(baidu_list, sem_id);
    return key_user_id;
}

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
	if ((size = network_recv(sockfd, buffer, 2048)) > 0) {

	    if (th_arg->key_status == 0) {

		semtools_packet_analysis(
			task,
			baidu_calendar,
			zoosnet_calendar,
			baidu_list, 
			io_ep, 
			sockfd, 
			buffer, 
			size);
	    } else {

		int code = -4;
		if (th_arg->key_status == -1)
		    code = 200;
		else if (th_arg->key_status == -2)
		    code = 201;
		else if (th_arg->key_status == -3)
		    code = 202;

		char buffer[128] = {0};
		size = sprintf(buffer, "%s{\"code\":%d}", HTTP_GET_RETURN, code);
		network_send(sockfd, buffer, size);
		close(sockfd);
	    }

	    this->thread_status = 0;
	}
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

	/* key baidu user */
	//th_arg->key_status = 0;
	th_arg->key_status = sem_key_packet(baidu_list, 111, th_arg->key_user_id);
	fprintf(stdout, "key_status=%d\n", th_arg->key_status);
	/* end key baidu user */

	time_t day_time = time(NULL);
	struct tm day_tm = {0};
	if (localtime_r(&day_time, &day_tm))
	    if (day_tm.tm_hour == 14)
		baidu_download_sem(baidu_list, baidu_calendar, 111);

	task_list_save(task, TASK_LIST_PATH);
	baidu_user_save(baidu_list, BAIDU_USER_LIST_PATH);
	calendar_save(baidu_calendar, CALENDAR_BAIDU_PATH);
	calendar_save(zoosnet_calendar, CALENDAR_ZOOSNET_PATH);

	sleep(10);
    }
}

int main(int argc, char *argv[])
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

    thread_arg_t thread_arg = {0};
    thread_arg.io_ep = network_epoll_init(NETWORK_MAX_EVENT, NETWORK_MAX_POLL, -1);
    thread_arg.listen_ep = network_epoll_init(1, 1, -1);

    thread_arg.task = task_list_init();
    thread_arg.baidu_list = baidu_user_init();
    thread_arg.baidu_calendar = calendar_init();
    thread_arg.zoosnet_calendar = calendar_init();

    task_list_load(thread_arg.task, TASK_LIST_PATH);
    baidu_user_load(thread_arg.baidu_list, BAIDU_USER_LIST_PATH);
    calendar_load(thread_arg.baidu_calendar, CALENDAR_BAIDU_PATH);
    calendar_load(thread_arg.zoosnet_calendar, CALENDAR_ZOOSNET_PATH);

    thread_arg.key_user_id = baidu_list_key_init(thread_arg.baidu_list, 111);
    baidu_user_save(thread_arg.baidu_list, BAIDU_USER_LIST_PATH);
    baidu_user_print(thread_arg.baidu_list);

    //    baidu_download_sem(thread_arg.baidu_list, thread_arg.baidu_calendar, 111);
    //    calendar_save(thread_arg.baidu_calendar, CALENDAR_BAIDU_PATH);
    //    return 0;

    thread_pool_t *pool = thread_pool_create(NULL, io_processor_thread, &thread_arg);
    thread_arg.pool = pool;

    network_fdmax(NETWORK_MAX_FD);
    int sockfd = network_bind(NULL, 9000);
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

int main_key_demo(void)
{


    baidu_user_t *baidu_list = baidu_user_init();
    baidu_user_load(baidu_list, BAIDU_USER_LIST_PATH);

    char *key_user_id = baidu_list_key_init(baidu_list, 111);
    fprintf(stdout, "%s\n", key_user_id);
    baidu_user_save(baidu_list, BAIDU_USER_LIST_PATH);

    while(1){

	int key_status = sem_key_packet(baidu_list, 111, key_user_id);
	fprintf(stdout, "key_status=%d\n", key_status);
	sleep(10);
    }

    return 0;
}

void demo_day(calendar_t *calendar, int baidu_id)
{

    time_t date_time = 0;
    struct tm date_tm = {0};
    strptime("2013-06-20", "%Y-%m-%d", &date_tm);
    date_time = mktime(&date_tm) + 86400;

    int day;
    char date[20];
    for (day = 0; day < 7; day++, date_time += 86400) {

	gmtime_r(&date_time, &date_tm);
	sprintf(date, "%04d-%02d-%02d",
		date_tm.tm_year + 1900,
		date_tm.tm_mon + 1,
		date_tm.tm_mday);

	calendar_user_t *user = NULL;
	if ((user = calendar_user_add(calendar, baidu_id)))
	    calendar_node_add(user, date_time);

	fprintf(stdout, "%s\n", date);
    }

    calendar_save(calendar, CALENDAR_BAIDU_PATH);
}



