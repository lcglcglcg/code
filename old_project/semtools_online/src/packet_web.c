
#include "sem.h"
#include "soap.h"
#include "pack_manager.h"

#include "baidu_report.h"
#include "zoosnet_report.h"
#include "complex_report.h"

#include "sem_user.h"
#include "baidu_user.h"
#include "task_list.h"

int soap_manager_connect(void)
{

    struct sockaddr_in addr = {0};
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr.sin_family = AF_INET;
    addr.sin_port = htons(8001);
    int sockfd = socket(PF_INET, SOCK_STREAM, 0);
    fprintf(stdout, "soap_manager_connect=%d\n", sockfd);
    if (connect(sockfd, (struct sockaddr *)&addr, sizeof(addr)) == 0)
	return sockfd;

    close(sockfd);
    return -1;
}

int soap_manager_synchronous(baidu_user_t *baidu_list,int sem_id)
{

    size_t size = -1;
    int soap_sockfd = -1;
    if ((soap_sockfd = soap_manager_connect()) == -1)
	return -1;

    packet_baidu_get_t soap_baidu_get = {0};
    soap_baidu_get.msid = PACKET_SOAP_MANAGER_BAIDU_GET;
    soap_baidu_get.sem_id = sem_id;
    if ((size = write(soap_sockfd, &soap_baidu_get, sizeof(soap_baidu_get))) < 0) {

	close(soap_sockfd);
	return -1;
    }

    packet_baidu_get_data_t pack_buffer;
    while(read(soap_sockfd, &pack_buffer, sizeof(pack_buffer)) > 0){

	baidu_user_node_t *baidu_node = NULL;
	if ((baidu_node = baidu_user_add(baidu_list, soap_baidu_get.sem_id, pack_buffer.baidu_id))) {

	    if (strcmp(baidu_node->password, pack_buffer.baidu_password))
		baidu_node->status = 0;

	    strcpy(baidu_node->account, pack_buffer.baidu_account);
	    strcpy(baidu_node->password, pack_buffer.baidu_password);
	    strcpy(baidu_node->domain, pack_buffer.baidu_domain);
	    strcpy(baidu_node->token, pack_buffer.baidu_token);
	    strcpy(baidu_node->web_name, pack_buffer.baidu_web_name);
	    strcpy(baidu_node->company_name, pack_buffer.baidu_company_name);
	    strcpy(baidu_node->contact_str, pack_buffer.baidu_contact_str);
	    strcpy(baidu_node->contact_tel, pack_buffer.baidu_contact_tel);
	    baidu_node->stop_marker = pack_buffer.baidu_stop_marker;
	    baidu_node->del_mask = 0;
	}
    }

    close(soap_sockfd);
    return 0;
}

void soap_manager_error(int sockfd, int code)
{

    char buffer[128];
    size_t size = sprintf(buffer, "{\"code\":%d}", code);
    network_send(sockfd, buffer, size);
}

void packet_login(int sockfd,
	baidu_user_t *baidu_list,
	char *account, char *password)
{

    size_t size = -1;
    int soap_sockfd = -1;
    if ((soap_sockfd = soap_manager_connect()) == -1)
	return soap_manager_error(sockfd, -4);
    fprintf(stdout, "soap_sockfd=%d\n", soap_sockfd);

    packet_login_t login = {0};
    login.msid = PACKET_SOAP_MANAGER_LOGIN;
    strcpy(login.sem_account, account);
    strcpy(login.sem_password, password);
    if ((size = write(soap_sockfd, &login, sizeof(packet_login_t))) < 0) {

	close(soap_sockfd);
	return soap_manager_error(sockfd, -4);
    }

    packet_return_t pack_ret = {0};
    if ((size = read(soap_sockfd, &pack_ret, sizeof(packet_return_t))) < 0) {

	close(soap_sockfd);
	return soap_manager_error(sockfd, -4);
    }

    fprintf(stdout, "ret=%d sem_id=%d code=%d\n",
	    pack_ret.ret,
	    pack_ret.sem_id,
	    pack_ret.code);
    close(soap_sockfd);
    int sem_id = pack_ret.sem_id;

    if (pack_ret.ret != 0)
	return soap_manager_error(sockfd, -4);

    if (pack_ret.code != 0)
	return soap_manager_error(sockfd, pack_ret.code);

    baidu_user_del_mask_init(baidu_list, pack_ret.sem_id);
    if ((soap_sockfd = soap_manager_connect()) == -1)
	return soap_manager_error(sockfd, -4);

    packet_baidu_get_t soap_baidu_get = {0};
    soap_baidu_get.msid = PACKET_SOAP_MANAGER_BAIDU_GET;
    soap_baidu_get.sem_id = pack_ret.sem_id;
    size = write(soap_sockfd, &soap_baidu_get, sizeof(soap_baidu_get));

    packet_baidu_get_data_t pack_buffer;
    while(read(soap_sockfd, &pack_buffer, sizeof(pack_buffer)) > 0){

	baidu_user_node_t *baidu_node = NULL;
	if ((baidu_node = baidu_user_add(baidu_list, soap_baidu_get.sem_id, pack_buffer.baidu_id))) {

	    strcpy(baidu_node->account, pack_buffer.baidu_account);
	    strcpy(baidu_node->password, pack_buffer.baidu_password);
	    strcpy(baidu_node->domain, pack_buffer.baidu_domain);
	    strcpy(baidu_node->token, pack_buffer.baidu_token);
	    strcpy(baidu_node->web_name, pack_buffer.baidu_web_name);
	    strcpy(baidu_node->company_name, pack_buffer.baidu_company_name);
	    strcpy(baidu_node->contact_str, pack_buffer.baidu_contact_str);
	    strcpy(baidu_node->contact_tel, pack_buffer.baidu_contact_tel);
	    baidu_node->stop_marker = pack_buffer.baidu_stop_marker;
	    baidu_node->del_mask = 0;
	}
    }

    int baidu_count = 0, baidu_stopmarker_count = 0;
    baidu_user_get_sem_count(baidu_list, pack_buffer.sem_id, &baidu_count, &baidu_stopmarker_count);
    fprintf(stdout, "baidu_count=%d baidu_stopmarker_count=%d\n", baidu_count, baidu_stopmarker_count);
    
    if (soap_manager_synchronous(baidu_list, sem_id) == -1)
	return soap_manager_error(sockfd, -4);

    char buffer[1024] = {0};
    size = sprintf(buffer, "%s{\"code\":0,\"sem_id\":%u, \"baidu_count\":%u, \"baidu_stopmarker_count\":%u}",
	    HTTP_GET_RETURN, sem_id, baidu_count, baidu_stopmarker_count);
    network_send(sockfd, buffer, size);
}

void packet_sem_pass_update(int sockfd, int sem_id, char *account, char *password, char *new_password)
{

    size_t size = -1;
    int soap_sockfd = -1;
    if ((soap_sockfd = soap_manager_connect()) == -1)
	return soap_manager_error(sockfd, -4);

    packet_sem_pass_update_t sem_pass = {0};
    sem_pass.msid = PACKET_SOAP_MANAGER_SEM_PASS;
    strcpy(sem_pass.sem_account, account);
    strcpy(sem_pass.sem_password, password);
    strcpy(sem_pass.new_sem_password, new_password);
    if ((size = write(soap_sockfd, &sem_pass, sizeof(sem_pass))) < 0) {

	close(soap_sockfd);
	return soap_manager_error(sockfd, -4);
    }

    packet_return_t pack_ret = {0};
    if ((size = read(soap_sockfd, &pack_ret, sizeof(packet_return_t))) < 0) {

	close(soap_sockfd);
	return soap_manager_error(sockfd, -4);
    }

    fprintf(stdout, "ret=%d code=%d\n",	pack_ret.ret, pack_ret.code);
    close(soap_sockfd);

    if (pack_ret.ret != 0 || pack_ret.code != 0)
	return soap_manager_error(sockfd, -4);

    char buffer[128] = {0};
    size = sprintf(buffer, "{\"code\":0}");
    network_send(sockfd, buffer, size);
}

int _packet_task_list_get_count(
	task_list_node_t *node,
	char *find_word,
	time_t s_time,
	time_t e_time)
{

    if (find_word)
	if (!strstr(node->task_name, find_word))
	    return -1;

    if (s_time && e_time) {

	if (node->create_time > s_time && node->create_time < e_time)
	    return 0;
	else
	    return  -1;
    }

    return 0;
}

int packet_task_list_get_count(
	task_list_user_t *user,
	char *find_word,
	time_t s_time,
	time_t e_time)
{

    int max = 0;
    task_list_node_t *node = NULL;
    pthread_mutex_lock(&user->lock);

    for (node = user->head; node; node = node->next) {

	if (_packet_task_list_get_count(node, find_word, s_time, e_time) == 0)
	    max++;
    }

    pthread_mutex_unlock(&user->lock);

    return max;
}

int packet_task_list_node_get(
	int sockfd, 
	char *buffer, 
	task_list_node_t *node, 
	char *find_word, 
	time_t s_time, 
	time_t e_time,
	int index,
	int *count,
	int index_start,
	int index_count)
{

    if (find_word)
	if (!strstr(node->task_name, find_word))
	    return -1;

    if (s_time && e_time) {

	if (node->create_time > s_time && node->create_time < e_time) {

	} else {

	    return  -1;
	}
    }

    char create_time[20] = {0};
    char completion_time[20] = {0};
    task_list_time_to_string(node->create_time, create_time);
    if (node->completion_time)
	task_list_time_to_string(node->completion_time, completion_time);
    else
	strcpy(completion_time, "--");

    size_t size;
    if (index >= index_start && *count < index_count) {

	*count = *count + 1;

	if (index_start == index)
	    size = sprintf(buffer,
		    "{\"task_id\":%u, \"task_name\":\"%s\", \"create_time\":\"%s\", \"completion_time\":\"%s\", \"status\":%d}",
		    node->task_id, node->task_name, create_time, completion_time, node->status);
	else
	    size = sprintf(buffer,
		    ",{\"task_id\":%u, \"task_name\":\"%s\", \"create_time\":\"%s\", \"completion_time\":\"%s\", \"status\":%d}",
		    node->task_id, node->task_name, create_time, completion_time, node->status);

	network_send(sockfd, buffer, size);
    }

    return 0;
}

void packet_task_list_get(
	int sockfd, 
	task_list_t *task, 
	int sem_id, 
	char *find_word, 
	char *start_time_str, 
	char *end_time_str,
	int index_start,
	int index_count)
{

    size_t size;
    char buffer[1024];

    task_list_user_t *user = NULL;
    if (!(user = task_list_user_find(task, sem_id))) {

	size = sprintf(buffer, "{\"task_count\":0, \"list\":[]}");
	network_send(sockfd, buffer, size);
	return;
    }

    struct tm date_tm;
    time_t s_time = 0, e_time = 0;

    memset(&date_tm, 0, sizeof(struct tm));
    if (strptime(start_time_str, "%Y-%m-%d", &date_tm))
	s_time = mktime(&date_tm);

    memset(&date_tm, 0, sizeof(struct tm));
    if (strptime(end_time_str, "%Y-%m-%d", &date_tm))
	e_time = mktime(&date_tm) + 86400;


    if (find_word && !*find_word)
	find_word = NULL;

    int max = packet_task_list_get_count(user, find_word, s_time, e_time);
    size = sprintf(buffer, "{\"task_count\":%u, \"list\":[", max);
    network_send(sockfd, buffer, size);

    task_list_node_t *node = NULL;
    pthread_mutex_lock(&user->lock);

    int index = 0, count = 0;
    for (node = user->head; node; node = node->next) {

	if (packet_task_list_node_get(
		    sockfd, 
		    buffer, 
		    node, 
		    find_word, 
		    s_time, 
		    e_time, 
		    index,
		    &count,
		    index_start,
		    index_count) == 0) {

	    index++;
	}
    }

    pthread_mutex_unlock(&user->lock);

    size = sprintf(buffer, "]}");
    network_send(sockfd, buffer, size);
}

void packet_calendar_baidu_add(int sockfd, calendar_t *baidu_calendar, int baidu_id, char *start_time_str, char *end_time_str)
{

    calendar_user_t *user = NULL;
    if (!(user = calendar_user_add(baidu_calendar, baidu_id)))
	return;

    time_t date_time;
    struct tm date_tm = {0};

    strptime(start_time_str, "%Y%m%d", &date_tm);
    time_t start_time = mktime(&date_tm) + 86400;

    strptime(end_time_str, "%Y%m%d", &date_tm);
    time_t end_time = mktime(&date_tm) + 86400;

    for (date_time = start_time; date_time <= end_time; date_time += 86400) {

	gmtime_r(&date_time, &date_tm);
	fprintf(stdout, "id:%u date:%04d-%02d-%02d\n",
		user->user_id,
		date_tm.tm_year + 1900,
		date_tm.tm_mon + 1,
		date_tm.tm_mday);

	calendar_node_add(user, date_time);
    }
}

void _packet_calendar_baidu_get(int sockfd, calendar_t *baidu_calendar, int baidu_id, char *baidu_account)
{

    char buffer[256];
    struct tm date_tm;
    calendar_user_t *user = NULL;
    calendar_node_t *node = NULL;

    pthread_mutex_lock(&baidu_calendar->lock);
    for (user = baidu_calendar->head; user; user = user->next) {

	fprintf(stdout, "user_id=%d baidu_id=%d\n", user->user_id, baidu_id);
	if (user->user_id == baidu_id) {

	    pthread_mutex_lock(&user->lock);
	    for (node = user->head; node; node = node->next) {

		gmtime_r(&node->date, &date_tm);
		int size = sprintf(buffer, "<day name='%s'>%04d-%02d-%02d</day>\n",
			baidu_account,
			date_tm.tm_year + 1900,
			date_tm.tm_mon + 1,
			date_tm.tm_mday);

		network_send(sockfd, buffer, size);
	    }

	    pthread_mutex_unlock(&user->lock);
	}
    }

    pthread_mutex_unlock(&baidu_calendar->lock);
}

void packet_calendar_baidu_get(int sockfd, 
	baidu_user_t *baidu_list,
	calendar_t *baidu_calendar,
	int sem_id)
{

    fprintf(stdout, "packet_calendar_baidu_get\n");
    fprintf(stdout, "baidu_list->head=%p\n", baidu_list->head);

    baidu_user_node_t *node = NULL;
    pthread_mutex_lock(&baidu_list->mutex);

    for (node = baidu_list->head; node; node = node->next) {

	if (node->sem_id == sem_id) {

	    fprintf(stdout, "sem_id=%d baidu_id=%d\n", node->sem_id, node->baidu_id);
	    _packet_calendar_baidu_get(sockfd, baidu_calendar, node->baidu_id, node->account);
	}
    }

    fprintf(stdout, "packet_calendar_baidu_get exit\n");

    pthread_mutex_unlock(&baidu_list->mutex);
}

void packet_calendar_zoosnet_get(int sockfd, calendar_t *zoosnet_calendar, int sem_id)
{

    char buffer[128];
    struct tm date_tm;
    calendar_user_t *user = NULL;
    calendar_node_t *node = NULL;

    pthread_mutex_lock(&zoosnet_calendar->lock);
    for (user = zoosnet_calendar->head; user; user = user->next) {

	if (user->user_id == sem_id) {

	    pthread_mutex_lock(&user->lock);
	    for (node = user->head; node; node = node->next) {

		gmtime_r(&node->date, &date_tm);
		int size = sprintf(buffer, "<day>%04d-%02d-%02d</day>",
			date_tm.tm_year + 1900,
			date_tm.tm_mon + 1,
			date_tm.tm_mday);

		network_send(sockfd, buffer, size);
	    }

	    pthread_mutex_unlock(&user->lock);
	}
    }

    pthread_mutex_unlock(&zoosnet_calendar->lock);
}

void packet_baidu_user_add_check_call(char **domain, int domain_size, void *arg)
{

    int i;
    char *domain_buffer = arg;
    for (i = 0; i < domain_size; i++) {

	if (i == 0) {

	    strcpy(domain_buffer, domain[i]);

	} else {

	    strcat(domain_buffer, ",");
	    strcat(domain_buffer, domain[i]);
	}
    }
}

void packet_baidu_user_add_check(int sockfd, char *account, char *password, char *token)
{

    int size;
    char domain[2048] = {0};
    char buffer[2048] = {0};

    if (token && !*token)token = "da2590c34de0e439d7ab0c783a85e223";
    int code = soap_baidu_get_account_info(
	    account,
	    password,
	    token,
	    packet_baidu_user_add_check_call,
	    domain);

    size = sprintf(buffer, "{\"code\":%d, \"baidu_domain\":\"%s\"}", code, domain);
    network_send(sockfd, buffer, size);
}

void packet_baidu_user_mod_check(int sockfd, baidu_user_t *baidu_list, int sem_id, int baidu_id, char *password, char *token)
{

    int size;
    char buffer[2048] = {0};

    if (password && !*password) password = NULL;
    if (token && !*token) token = "da2590c34de0e439d7ab0c783a85e223";

    baidu_user_node_t *baidu_node = NULL;
    if (!(baidu_node = baidu_user_find(baidu_list, sem_id, baidu_id))) {

	size = sprintf(buffer, "{\"code\":-1}");
	network_send(sockfd, buffer, size);
	return;
    }

    if (time(NULL) - baidu_node->check_time < 180) {

	size = sprintf(buffer, "{\"code\":-2}");
	network_send(sockfd, buffer, size);
	return;
    }

    baidu_node->check_time = time(NULL);

    int code = soap_baidu_get_account_info(
	    baidu_node->account,
	    password?password:baidu_node->password,
	    token,
	    NULL,
	    NULL);

    size = sprintf(buffer, "{\"code\":%d}", code);
    network_send(sockfd, buffer, size);
}

void packet_baidu_user_save(
	int sockfd, 
	int sem_id, 
	int baidu_id, 
	char *password, 
	char *token,
	baidu_user_t *baidu_list)
{

    size_t size;
    char buffer[64];

    baidu_user_node_t *baidu_node = NULL;
    if (!(baidu_node = baidu_user_find(baidu_list, sem_id, baidu_id))) {

	size = sprintf(buffer, "{\"code\":-1}");
	network_send(sockfd, buffer, size);
	return;
    }

    if (password && !*password) password = NULL;
    if (token && !*token) token = NULL;

    int code = soap_baidu_get_account_info(
	    baidu_node->account,
	    password?password:baidu_node->password,
	    token?token:baidu_node->token,
	    NULL,
	    NULL);

    if (code != 0) {

	size = sprintf(buffer, "{\"code\":%d}", code);
	network_send(sockfd, buffer, size);
	return;
    }

    if (password)strcpy(baidu_node->password, password);
    if (token)strcpy(baidu_node->token, token);

    size = sprintf(buffer, "{\"code\":0}");
    network_send(sockfd, buffer, size);
}

void packet_baidu_user_check(
	int sockfd, 
	int sem_id, 
	int baidu_id, 
	char *account,
	char *password, 
	char *token,
	baidu_user_t *baidu_list)
{

    if (baidu_id)
	packet_baidu_user_mod_check(sockfd, baidu_list, sem_id, baidu_id, password, token);
    else
	packet_baidu_user_add_check(sockfd, account, password, token);
}

void packet_baidu_user_add(
	int sockfd, 
	int sem_id,
	char *account,
	char *password,
	char *web_name,
	char *domain,
	char *company_name,
	char *contact_str,
	char *contact_tel,
	char *token,
	baidu_user_t *baidu_list)
{

    if (domain && !*domain)
	return soap_manager_error(sockfd, -1);

    size_t size = -1;
    int soap_sockfd = -1;
    if ((soap_sockfd = soap_manager_connect()) == -1)
	return soap_manager_error(sockfd, -4);

    packet_baidu_add_t baidu_add = {0};
    baidu_add.msid = PACKET_SOAP_MANAGER_BAIDU_ADD;
    baidu_add.sem_id = sem_id;
    strcpy(baidu_add.baidu_account, account);
    strcpy(baidu_add.baidu_password, password);
    strcpy(baidu_add.baidu_domain, domain);
    if (token && !*token) token = "da2590c34de0e439d7ab0c783a85e223";
    strcpy(baidu_add.baidu_token, token);
    strcpy(baidu_add.baidu_web_name, web_name);
    strcpy(baidu_add.baidu_company_name, company_name);
    strcpy(baidu_add.baidu_contact_str, contact_str);
    strcpy(baidu_add.baidu_contact_tel, contact_tel);
    if ((size = write(soap_sockfd, &baidu_add, sizeof(baidu_add))) < 0) {

	close(soap_sockfd);
	return soap_manager_error(sockfd, -4);
    }

    packet_return_t pack_ret = {0};
    if ((size = read(soap_sockfd, &pack_ret, sizeof(packet_return_t))) < 0) {

	close(soap_sockfd);
	return soap_manager_error(sockfd, -4);
    }

    close(soap_sockfd);
    fprintf(stdout, "ret=%d code=%d\n",	pack_ret.ret, pack_ret.code);

    if (pack_ret.ret != 0 || pack_ret.code != 0)
	return soap_manager_error(sockfd, -5);

    if (soap_manager_synchronous(baidu_list, sem_id) == -1)
	return soap_manager_error(sockfd, -4);

    char buffer[128] = {0};
    size = sprintf(buffer, "{\"code\":0}");
    network_send(sockfd, buffer, size);
}

void packet_baidu_user_mod(
	int sockfd,
	int sem_id,
	int baidu_id,
	char *password,
	char *web_name,
	char *company_name,
	char *contact_str,
	char *contact_tel,
	char *token,
	baidu_user_t *baidu_list)
{

    size_t size = -1;
    char buffer[64] = {0};
    baidu_user_node_t *baidu_node = NULL;
    if (!(baidu_node = baidu_user_find(baidu_list, sem_id, baidu_id))) {

	size = sprintf(buffer, "{\"code\":-1}");
	network_send(sockfd, buffer, size);
	return;
    }

    int soap_sockfd = -1;
    if ((soap_sockfd = soap_manager_connect()) == -1)
	return soap_manager_error(sockfd, -4);

    packet_baidu_mod_t baidu_mod = {0};
    baidu_mod.msid = PACKET_SOAP_MANAGER_BAIDU_MOD;
    baidu_mod.sem_id = sem_id;
    baidu_mod.baidu_id = baidu_id;
    strcpy(baidu_mod.baidu_account, baidu_node->account);
    if (password && !*password)password = baidu_node->password;
    strcpy(baidu_mod.baidu_password, password);
    strcpy(baidu_mod.baidu_domain, baidu_node->domain);
    if (token && !*token) token = "da2590c34de0e439d7ab0c783a85e223";
    strcpy(baidu_mod.baidu_token, token);
    strcpy(baidu_mod.baidu_web_name, web_name);
    strcpy(baidu_mod.baidu_company_name, company_name);
    strcpy(baidu_mod.baidu_contact_str, contact_str);
    strcpy(baidu_mod.baidu_contact_tel, contact_tel);
    if ((size = write(soap_sockfd, &baidu_mod, sizeof(baidu_mod))) < 0) {

	close(soap_sockfd);
	return soap_manager_error(sockfd, -4);
    }

    packet_return_t pack_ret = {0};
    if ((size = read(soap_sockfd, &pack_ret, sizeof(packet_return_t))) < 0) {

	close(soap_sockfd);
	return soap_manager_error(sockfd, -4);
    }

    fprintf(stdout, "ret=%d code=%d\n",	pack_ret.ret, pack_ret.code);
    close(soap_sockfd);

    if (pack_ret.ret != 0 || pack_ret.code != 0)
	return soap_manager_error(sockfd, -4);

    strcpy(baidu_node->password, password);
    strcpy(baidu_node->web_name, web_name);
    strcpy(baidu_node->company_name, company_name);
    strcpy(baidu_node->contact_str, contact_str);
    strcpy(baidu_node->contact_tel, contact_tel);
    strcpy(baidu_node->token, token);

    size = sprintf(buffer, "{\"code\":0}");
    network_send(sockfd, buffer, size);
}

void packet_baidu_user_del(int sockfd, baidu_user_t *baidu_list, int sem_id, int baidu_id)
{

    size_t size = -1;
    int soap_sockfd = -1;
    if ((soap_sockfd = soap_manager_connect()) == -1)
	return soap_manager_error(sockfd, -4);

    packet_baidu_del_t baidu_del = {0};
    baidu_del.msid = PACKET_SOAP_MANAGER_BAIDU_DEL;
    baidu_del.sem_id = sem_id;
    baidu_del.baidu_id = baidu_id;
    if ((size = write(soap_sockfd, &baidu_del, sizeof(baidu_del))) < 0) {

	close(soap_sockfd);
	return soap_manager_error(sockfd, -4);
    }

    packet_return_t pack_ret = {0};
    if ((size = read(soap_sockfd, &pack_ret, sizeof(packet_return_t))) < 0) {

	close(soap_sockfd);
	return soap_manager_error(sockfd, -4);
    }

    fprintf(stdout, "ret=%d code=%d\n",	pack_ret.ret, pack_ret.code);
    close(soap_sockfd);

    if (pack_ret.ret != 0 || pack_ret.code != 0)
	return soap_manager_error(sockfd, -4);

    baidu_user_del(baidu_list, sem_id, baidu_id);

    char buffer[128] = {0};
    size = sprintf(buffer, "{\"code\":0}");
    network_send(sockfd, buffer, size);
}

void packet_baidu_user_get(int sockfd, baidu_user_t *baidu_list, int sem_id)
{

    char buffer[1024];
    int size = sprintf(buffer, "{\"list\":[");
    network_send(sockfd, buffer, size);

    baidu_user_node_t *node = NULL;
    pthread_mutex_lock(&baidu_list->mutex);

    int format_mask = 0;
    for (node = baidu_list->head; node; node = node->next) {

	if (node->sem_id == sem_id && node->del_mask == 0) {

	    if (format_mask)
		network_send(sockfd, ",", 1);
	    format_mask = 1;

	    size = sprintf(buffer, "{\"baidu_id\":%u,", node->baidu_id);
	    network_send(sockfd, buffer, size);

	    size = sprintf(buffer, "\"baidu_account\":\"%s\",", node->account);
	    network_send(sockfd, buffer, size);

	    if (strcmp(node->token, "da2590c34de0e439d7ab0c783a85e223") == 0)
		size = sprintf(buffer, "\"baidu_token\":\"\",");
	    else
		size = sprintf(buffer, "\"baidu_token\":\"%s\",", node->token);
	    network_send(sockfd, buffer, size);

	    size = sprintf(buffer, "\"baidu_web_name\":\"%s\",", node->web_name);
	    network_send(sockfd, buffer, size);

	    size = sprintf(buffer, "\"baidu_domain\":\"%s\",", node->domain);
	    network_send(sockfd, buffer, size);

	    size = sprintf(buffer, "\"baidu_company_name\":\"%s\",", node->company_name);
	    network_send(sockfd, buffer, size);

	    size = sprintf(buffer, "\"baidu_contact_str\":\"%s\",", node->contact_str);
	    network_send(sockfd, buffer, size);

	    size = sprintf(buffer, "\"baidu_contact_tel\":\"%s\",", node->contact_tel);
	    network_send(sockfd, buffer, size);

	    char last_time[20];
	    baidu_user_time_to_string(node->last_time, last_time);
	    size = sprintf(buffer, "\"baidu_last_time\":\"%s\",", last_time);
	    network_send(sockfd, buffer, size);

	    size = sprintf(buffer, "\"baidu_stop_marker\":\"%d\",", node->stop_marker);
	    network_send(sockfd, buffer, size);

	    size = sprintf(buffer, "\"baidu_status\":%d}", node->status);
	    network_send(sockfd, buffer, size);
	}
    }

    size = sprintf(buffer, "]}");
    network_send(sockfd, buffer, size);
    pthread_mutex_unlock(&baidu_list->mutex);
}

void packet_baidu_user_status(int sockfd, baidu_user_t *baidu_list, int sem_id)
{

    char buffer[512];
    baidu_user_node_t *node = NULL;
    pthread_mutex_lock(&baidu_list->mutex);

    int status_count = 0;
    for (node = baidu_list->head; node; node = node->next) {

	if (node->sem_id == sem_id && node->status > 0)
	    status_count++;
    }

    pthread_mutex_unlock(&baidu_list->mutex);

    int size = sprintf(buffer, "%s%d", HTTP_GET_RETURN, status_count);
    network_send(sockfd, buffer, size);
}

void zoosnet_calendar_save(calendar_t *zoosnet_calendar, int sem_id, time_t start_time, time_t end_time)
{

    calendar_user_t *user = NULL;
    if (!(user = calendar_user_add(zoosnet_calendar, sem_id)))
	return;

    time_t date_time;
    struct tm date_tm = {0};
    for (date_time = start_time; date_time <= end_time; date_time += 86400) {

	calendar_node_add(user, date_time);

	char date_str[20];
	gmtime_r(&date_time, &date_tm);
	sprintf(date_str, "%04d-%02d-%02d",
		date_tm.tm_year + 1900,
		date_tm.tm_mon + 1,
		date_tm.tm_mday);

	fprintf(stdout, "zoosnet_calendar_save:%s\n", date_str);
    }
}

int _semtools_merger_day(
	int sem_id,
	task_list_node_t *task_node,
	char *baidu_account,
	char *baidu_domain,
	time_t start_time, 
	time_t end_time,
	zoosnet_report_head_t *zoosnet_report)
{

    char path[1024];
    char date_str[20];

    time_t date_time;
    for (date_time = start_time; date_time <= end_time; date_time+= 86400) {

	struct tm date_tm = {0};
	gmtime_r(&date_time, &date_tm);

	sprintf(date_str, "%04d-%02d-%02d",
		date_tm.tm_year + 1900,
		date_tm.tm_mon + 1,
		date_tm.tm_mday);

	sprintf(path, "%s/%s/baidu_pair_%04d_%02d_%02d.dat",
		BAIDU_REPORT_PATH,
		baidu_account,
		date_tm.tm_year + 1900,
		date_tm.tm_mon + 1,
		date_tm.tm_mday);

	baidu_pair_report_node_t *pair_report = NULL;
	pair_report = baidu_pair_report_day_load(NULL, path);

	zoosnet_report_head_t *zoosnet_temp = NULL;
	zoosnet_report_head_t *zoosnet_node = NULL;
	for (zoosnet_node = zoosnet_report; zoosnet_node; zoosnet_node = zoosnet_node->next) {

	    if (strcmp(date_str, zoosnet_node->date_str) == 0) {

		zoosnet_temp = zoosnet_node;
		break;
	    }
	}

	if (pair_report) {

	    complex_report_t *complex_report = complex_report_pair_load(NULL, pair_report, NULL, NULL);
	    if (zoosnet_temp)
		complex_report_zoosnet_day_load(complex_report, zoosnet_temp->head, baidu_domain, 0);
	    complex_report_calculate(complex_report);

	    if (zoosnet_temp)
		sprintf(path, "%s/%u/%u/%s/分日报告%s.xls", 
			DOWNLOAD_PATH, 
			sem_id,
			task_node->task_id,
			baidu_account, 
			date_str);
	    else
		sprintf(path, "%s/%u/%u/%s/分日报告_无商务通数据%s.xls", 
			DOWNLOAD_PATH, 
			sem_id,
			task_node->task_id,
			baidu_account, 
			date_str);

	    complex_report_to_xls(complex_report->all_head, path);

	    baidu_pair_report_free(pair_report);
	    complex_report_free(complex_report);
	}
    }

    return 0;
}

int _semtools_merger(
	int sem_id,
	task_list_node_t *task_node,
	char *baidu_account,
	char *baidu_domain,
	int task_mode,
	time_t start_time, 
	time_t end_time,
	zoosnet_report_head_t *zoosnet_report)
{

    char path[1024];
    sprintf(path, "%s/%u/%u/%s", DOWNLOAD_PATH, sem_id, task_node->task_id, baidu_account);
    if (access(path, 0)) {

	fprintf(stdout, "[mkdir][%s]\n", path);
	if (mkdir(path, 0755) == -1)
	    return -1;
    }

    time_t date_time = 0;
    baidu_pair_report_node_t *pair_report = NULL;
    baidu_search_report_node_t *search_report = NULL;
    char start_time_str[20] = {0}, end_time_str[20] = {0};

    int search_mask = 0;
    if (task_mode == 1)
	search_mask = 1;
    else if (task_mode == 2)
	search_mask = 0;
    else if (task_mode == 3)
	return _semtools_merger_day(sem_id, task_node, baidu_account, baidu_domain, start_time, end_time, zoosnet_report);
    else
	return -1;

    for (date_time = start_time; date_time <= end_time; date_time+= 86400) {

	struct tm date_tm = {0};
	gmtime_r(&date_time, &date_tm);

	sprintf(path, "%s/%s/baidu_pair_%04d_%02d_%02d.dat",
		BAIDU_REPORT_PATH,
		baidu_account,
		date_tm.tm_year + 1900,
		date_tm.tm_mon + 1,
		date_tm.tm_mday);
	pair_report = baidu_pair_report_day_load(pair_report, path);
	fprintf(stdout, "pair_report=%p path=%s\n", pair_report, path);

	if (!start_time_str[0] && pair_report)
	    sprintf(start_time_str, "%04d-%02d-%02d",
		    date_tm.tm_year + 1900,
		    date_tm.tm_mon + 1,
		    date_tm.tm_mday);

	if (pair_report)
	    sprintf(end_time_str, "%04d-%02d-%02d",
		    date_tm.tm_year + 1900,
		    date_tm.tm_mon + 1,
		    date_tm.tm_mday);

	if (search_mask) {

	    sprintf(path, "%s/%s/baidu_search_%04d_%02d_%02d.dat",
		    BAIDU_REPORT_PATH,
		    baidu_account,
		    date_tm.tm_year + 1900,
		    date_tm.tm_mon + 1,
		    date_tm.tm_mday);
	    search_report = baidu_search_report_day_load(search_report, path);
	    fprintf(stdout, "search_report=%p path=%s\n", search_report, path);
	}
    }

    if (pair_report == NULL)
	return -1;

    complex_report_t *complex_report = NULL;
    complex_report = complex_report_pair_load(complex_report, pair_report, NULL, NULL);
    baidu_pair_report_free(pair_report);

    if (search_mask) {

	complex_report_search_load(complex_report, search_report, NULL, NULL);
	baidu_search_report_free(search_report);
    }

    complex_report_zoosnet_load(complex_report, zoosnet_report, baidu_domain, search_mask);
    complex_report_calculate(complex_report);

    if (search_mask) {

	sprintf(path, "%s/%u/%u/%s/汇总报告_有搜索詞_默认_%s至%s.xls", 
		DOWNLOAD_PATH, 
		sem_id,
		task_node->task_id,
		baidu_account, 
		start_time_str, 
		end_time_str);
	complex_report_to_xls(complex_report->all_head, path);

    } else {

	sprintf(path, "%s/%u/%u/%s/汇总报告_无搜索詞_默认_%s至%s.xls",
		DOWNLOAD_PATH, 
		sem_id,
		task_node->task_id,
		baidu_account, 
		start_time_str, 
		end_time_str);
	complex_report_to_xls(complex_report->all_head, path);

    }

    complex_report_free(complex_report);
    return 0;
}

void semtools_merger(
	int sem_id,
	time_t time_s,
	time_t time_e,
	int task_mode,
	task_list_node_t *task_node,
	baidu_user_t *baidu_user_list,
	zoosnet_report_head_t *zoosnet_report)
{

    char path[1024];
    sprintf(path, "%s/%u", DOWNLOAD_PATH, sem_id);
    if (access(path, 0)) {

	fprintf(stdout, "[mkdir][%s]\n", path);
	if (mkdir(path, 0755) == -1)
	    return;
    }

    sprintf(path, "%s/%u/%u", DOWNLOAD_PATH, sem_id, task_node->task_id);
    if (access(path, 0)) {

	fprintf(stdout, "[mkdir][%s]\n", path);
	if (mkdir(path, 0755) == -1)
	    return;
    }

    baidu_user_node_t *temp_head = NULL;
    if (!(temp_head = baidu_user_semlist(baidu_user_list, sem_id)))
	return;

    int report_status = 0;
    baidu_user_node_t *node = NULL;
    for (node = temp_head; node; node = node->next) {

	fprintf(stdout, "[%s]\n", node->account);

	if (_semtools_merger(
		    sem_id,
		    task_node,
		    node->account,
		    node->domain,
		    task_mode,
		    time_s,
		    time_e,
		    zoosnet_report) == 0) {

	    report_status = 1;

	} else {

	    sprintf(path, "%s/%u/%u/%s", DOWNLOAD_PATH, sem_id, task_node->task_id, node->account);
	    fprintf(stdout, "[rmdir][%s]\n", path);
	    rmdir(path);
	}
    }

    baidu_user_semlist_free(temp_head);

    if (report_status) {

	sprintf(path, "7za a -tzip %s/%u/Semtools_Report_%u.zip %s/%u/%u/*",
		DOWNLOAD_PATH, sem_id, task_node->task_id,
		DOWNLOAD_PATH, sem_id, task_node->task_id);

	system(path);
	task_node->completion_time = time(NULL);
	task_node->status = 0;

    } else {

	task_node->completion_time = time(NULL);
	task_node->status = -3;
    }
}

void semtools_run(
	int sem_id,
	char *task_name,
	char *zoosnet_path,
	char *reservation_str,
	char *reservation_col_str,
	char *not_reservation_str,
	int task_mode,
	task_list_t *task,
	baidu_user_t *baidu_user_list,
	calendar_t *zoosnet_calendar)
{


    task_list_user_t *task_user = NULL;
    if (!(task_user = task_list_user_add(task, sem_id)))
	return;

    task_list_node_t *task_node = NULL;
    if (!(task_node = task_list_node_add(task_user, task_name)))
	return;

    char reservation_col[128] = {0};
    if (reservation_col_str) {

	if (strcmp(reservation_col_str, "2") == 0)
	    strcpy(reservation_col, "名称");
	else if (strcmp(reservation_col_str, "3") == 0)
	    strcpy(reservation_col, "备注");
	else if (strcmp(reservation_col_str, "4") == 0)
	    strcpy(reservation_col, "客人类别");
    }

    fprintf(stdout, "==[%s]==\n", reservation_col);

    char path[1024] = {0};
    sprintf(path, "%s/%s", UPLOADS_PATH, zoosnet_path);
    zoosnet_report_head_t *zoosnet_report = NULL;
    zoosnet_report = zoosnet_report_load(
	    path, 
	    reservation_str,
	    reservation_col,
	    not_reservation_str);

    if (!zoosnet_report) {

	task_node->completion_time = time(NULL);
	task_node->status = -2;
	return;
    }

    time_t start_time = 0, end_time = 0;
    zoosnet_report_time_get(zoosnet_report, &start_time, &end_time);
    zoosnet_report_day_save(zoosnet_report, sem_id, NULL);
    zoosnet_calendar_save(zoosnet_calendar, sem_id, start_time, end_time);
    semtools_merger(
	    sem_id,
	    start_time,
	    end_time,
	    task_mode,
	    task_node,
	    baidu_user_list, 
	    zoosnet_report);
    zoosnet_report_free(zoosnet_report);
}

void semtools_day_run(
	int sem_id,
	char *task_name,
	char *start_time_str,
	char *end_time_str,
	int task_mode,
	task_list_t *task,
	baidu_user_t *baidu_user_list)
{

    task_list_user_t *task_user = NULL;
    if (!(task_user = task_list_user_add(task, sem_id)))
	return;

    task_list_node_t *task_node = NULL;
    if (!(task_node = task_list_node_add(task_user, task_name)))
	return;

    struct tm start_date_tm = {0}, end_date_tm = {0};
    strptime(start_time_str, "%Y-%m-%d", &start_date_tm);
    strptime(end_time_str, "%Y-%m-%d", &end_date_tm);

    time_t start_time = 0, end_time = 0;
    start_time = mktime(&start_date_tm) + 86400;
    end_time = mktime(&end_date_tm) + 86400;

    zoosnet_report_head_t *zoosnet_report = NULL;
    zoosnet_report = zoosnet_report_day_load(sem_id, start_time_str, end_time_str);
    if (!zoosnet_report) {

	//task_node->completion_time = time(NULL);
	//task_node->status = -2;
    }

    semtools_merger(
	    sem_id,
	    start_time,
	    end_time,
	    task_mode,
	    task_node,
	    baidu_user_list, 
	    zoosnet_report);
    zoosnet_report_free(zoosnet_report);
}

void semtools_packet_return(network_epoll_t *io_event, int sockfd, int code)
{

    char buffer[256] = {0};
    int size = sprintf(buffer, "%s{\"code\":%d}", HTTP_GET_RETURN, code);
    network_send(sockfd, buffer, size);
    network_epoll_event_del(io_event, sockfd, 0);
    close(sockfd);
}

void semtools_packet_analysis(
	task_list_t *task,
	calendar_t *baidu_calendar,
	calendar_t *zoosnet_calendar,
	baidu_user_t *baidu_user_list,
	network_epoll_t *io_event, 
	int sockfd, 
	char *buffer, 
	int size)
{

    int i, j;
    char *str[10] = {NULL};
    //fprintf(stdout, "%s\n", buffer);
    if (size < 10 || strncmp(buffer, "GET /", 5) != 0) {

	return semtools_packet_return(io_event, sockfd, -4);
    }

    str[0] = &buffer[5];
    for (i = 6; i < size; i++) {

	if (buffer[i] == '&')
	    break;

	if (i >= size)
	    return semtools_packet_return(io_event, sockfd, -4);
    }

    for (j = 1; i < size; i++) {

	if (j > 20) {

	    return semtools_packet_return(io_event, sockfd, -4);

	} else if (buffer[i] == '&') {

	    buffer[i] = 0;
	    str[j] = &buffer[i + 1];
	    for (i++; i < size; i++) {

		if (buffer[i] == '&') {

		    buffer[i] = 0;
		    break;
		}
	    }

	    if (i >= size) {

		return semtools_packet_return(io_event, sockfd, -4);
	    }

	    int str_len = atoi(str[j]);
	    if (str_len >= (size - i))
		return semtools_packet_return(io_event, sockfd, -4);

	    str[j] = &buffer[i + 1];
	    i += str_len;
	    j++;

	} else if (buffer[i] == ' ') {

	    buffer[i] = 0;
	    break;

	} else {

	    return semtools_packet_return(io_event, sockfd, -4);
	}
    }

    for (i = 0; i < j; i++)
	fprintf(stdout, "[arg:%d][%s]\n", i, str[i]);

    char *pack_type = str[0];
    if (strcmp(pack_type, "login") == 0 && j == 3) {

	char *sem_account = str[1];
	char *sem_password = str[2];
	packet_login(sockfd, baidu_user_list, sem_account, sem_password);

	network_epoll_event_del(io_event, sockfd, 0);
	close(sockfd);

    } else if ((strcmp(pack_type, "sem/pass") == 0)) {

	int sem_id = atoll(str[1]);
	char *account = str[2];
	char *password = str[3];
	char *new_password = str[4];
	packet_sem_pass_update(sockfd, sem_id, account, password, new_password);

	network_epoll_event_del(io_event, sockfd, 0);
	close(sockfd);

    } else if ((strcmp(pack_type, "task/file/add") == 0) && j == 8) {

	int sem_id = atoll(str[1]);
	char *task_name = str[2];
	char *zoosnet_path = str[3];
	char *reservation_str = str[4];	
	char *reservation_col_str = str[5];
	char *not_reservation_str = str[6];
	int task_mode = atoi(str[7]);

	semtools_packet_return(io_event, sockfd, 0);

	if (task_mode == 1 || task_mode == 2 || task_mode == 3)
	    semtools_run(
		    sem_id,
		    task_name,
		    zoosnet_path,
		    reservation_str,
		    reservation_col_str,
		    not_reservation_str,
		    task_mode,
		    task,
		    baidu_user_list,
		    zoosnet_calendar);

	network_epoll_event_del(io_event, sockfd, 0);
	close(sockfd);

    } else if ((strcmp(pack_type, "task/calendar/add") == 0) && j == 6) {

	int sem_id = atoll(str[1]);
	char *task_name = str[2];
	char *start_time_str = str[3];
	char *end_time_str = str[4];
	int task_mode = atoi(str[5]);

	semtools_packet_return(io_event, sockfd, 0);

	if (task_mode == 1 || task_mode == 2 || task_mode == 3)
	    semtools_day_run(
		    sem_id,
		    task_name,
		    start_time_str,
		    end_time_str,
		    task_mode,
		    task,
		    baidu_user_list);

	fprintf(stdout, "task/calendar/add\n");

	network_epoll_event_del(io_event, sockfd, 0);
	close(sockfd);

    } else if ((strcmp(pack_type, "task/get") == 0)) {

	int sem_id = atoll(str[1]);
	char *find_word = str[2];
	char *start_time_str = str[3];
	char *end_time_str = str[4];
	int index_start = atoll(str[5]);
	int index_count  = atoll(str[6]);
	packet_task_list_get(
		sockfd, 
		task, 
		sem_id, 
		find_word, 
		start_time_str, 
		end_time_str,
		index_start,
		index_count);

	network_epoll_event_del(io_event, sockfd, 0);
	close(sockfd);

    } else if ((strcmp(pack_type, "task/del") == 0)) {

	int sem_id = atoll(str[1]);
	int task_id = atoll(str[2]);
	int code = task_list_del(task, sem_id, task_id);
	semtools_packet_return(io_event, sockfd, code);

	network_epoll_event_del(io_event, sockfd, 0);
	close(sockfd);

    } else if ((strcmp(pack_type, "calendar/baidu/add") == 0)) {

	int baidu_id = atoll(str[1]);
	char *start_time_str = str[2];
	char *end_time_str = str[3];
	packet_calendar_baidu_add(sockfd, baidu_calendar, baidu_id, start_time_str, end_time_str);

	network_epoll_event_del(io_event, sockfd, 0);
	close(sockfd);

    } else if ((strcmp(pack_type, "calendar/baidu/get") == 0)) {

	int sem_id = atoll(str[1]);
	packet_calendar_baidu_get(sockfd, baidu_user_list, baidu_calendar, sem_id);

	network_epoll_event_del(io_event, sockfd, 0);
	close(sockfd);

    } else if ((strcmp(pack_type, "calendar/zoosnet/get") == 0)) {

	int sem_id = atoll(str[1]);
	packet_calendar_zoosnet_get(sockfd, zoosnet_calendar, sem_id);

	network_epoll_event_del(io_event, sockfd, 0);
	close(sockfd);

    } else if ((strcmp(pack_type, "baidu/user/save") == 0)) {

	int sem_id = atoll(str[1]);
	int baidu_id = atoll(str[2]);
	char *baidu_password = str[3];
	char *baidu_token = str[4];
	packet_baidu_user_save(
		sockfd, 
		sem_id, 
		baidu_id, 
		baidu_password, 
		baidu_token,
		baidu_user_list);

	network_epoll_event_del(io_event, sockfd, 0);
	close(sockfd);

    } else if ((strcmp(pack_type, "baidu/user/check") == 0) && j == 6) {

	int sem_id = atoll(str[1]);
	int baidu_id = atoll(str[2]);
	char *baidu_account = str[3];
	char *baidu_password = str[4];
	char *baidu_token = str[5];
	packet_baidu_user_check(
		sockfd, 
		sem_id, 
		baidu_id, 
		baidu_account, 
		baidu_password, 
		baidu_token,
		baidu_user_list);

	network_epoll_event_del(io_event, sockfd, 0);
	close(sockfd);

    } else if ((strcmp(pack_type, "baidu/user/add") == 0)) {

	int sem_id = atoll(str[1]);
	char *account = str[2];
	char *password = str[3];
	char *web_name = str[4];
	char *domain = str[5];
	char *company_name = str[6];
	char *contact_str = str[7];
	char *contact_tel = str[8];
	char *token = str[9];

	packet_baidu_user_add(
		sockfd, 
		sem_id,
		account,
		password,
		web_name,
		domain,
		company_name,
		contact_str,
		contact_tel,
		token,
		baidu_user_list);

	network_epoll_event_del(io_event, sockfd, 0);
	close(sockfd);

    } else if ((strcmp(pack_type, "baidu/user/mod") == 0)) {

	int sem_id = atoll(str[1]);
	int baidu_id = atoll(str[2]);
	char *password = str[3];
	char *web_name = str[4];
	char *company_name = str[5];
	char *contact_str = str[6];
	char *contact_tel= str[7];
	char *token = str[8];
	packet_baidu_user_mod(
		sockfd,
		sem_id,
		baidu_id,
		password,
		web_name,
		company_name,
		contact_str,
		contact_tel,
		token,
		baidu_user_list);

	network_epoll_event_del(io_event, sockfd, 0);
	close(sockfd);

    } else if ((strcmp(pack_type, "baidu/user/del") == 0)) {

	int sem_id = atoll(str[1]);
	int baidu_id = atoll(str[2]);
	packet_baidu_user_del(sockfd, baidu_user_list, sem_id, baidu_id);

	network_epoll_event_del(io_event, sockfd, 0);
	close(sockfd);

    } else if ((strcmp(pack_type, "baidu/user/get") == 0)) {

	int sem_id = atoll(str[1]);
	packet_baidu_user_get(sockfd, baidu_user_list, sem_id);

	network_epoll_event_del(io_event, sockfd, 0);
	close(sockfd);

    } else if ((strcmp(pack_type, "baidu/user/status") == 0)) {

	int sem_id = atoll(str[1]);
	packet_baidu_user_status(sockfd, baidu_user_list, sem_id);

	network_epoll_event_del(io_event, sockfd, 0);
	close(sockfd);

    } else {

	network_epoll_event_del(io_event, sockfd, 0);
	close(sockfd);
    }
}



