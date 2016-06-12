
#include "soap_manager.h"
#include "pack_manager.h"

void signal_function(int sig){}

int md5_to_string(char *buffer, char *md5_buffer)
{

    int i;
    MD5_CTX ctx = {0};
    char temp[3] = {0};
    unsigned char md[16] = {0};

    MD5_Init(&ctx);
    MD5_Update(&ctx, buffer, strlen(buffer));//更新这块区域防止引用非法数据
    MD5_Final(md, &ctx); //ctx中的数据按照MD5算法生成16位的MD5码存放到md中

    for (i = 0; i < 16; i++) {

	sprintf(temp, "%02x", md[i]);
	strcat(md5_buffer, temp);
    }

    return 0;
}

void packet_baidu_get_call(
	int sem_id,
	int baidu_id,
	char *account,
	char *password,
	char *token,
	char *web_name,
	char *domain,
	char *company_name,
	char *contact_str,
	char *contact_tel,
	int stop_marker,
	void *arg)
{

    packet_baidu_get_data_t data = {0};

    data.sem_id = sem_id;
    data.baidu_id = baidu_id;
    if(account)strcpy(data.baidu_account, account);
    if(password)strcpy(data.baidu_password, password);
    if(token)strcpy(data.baidu_token, token);
    if(web_name)strcpy(data.baidu_web_name, web_name);
    if(domain)strcpy(data.baidu_domain, domain);
    if(company_name)strcpy(data.baidu_company_name, company_name);
    if(contact_str)strcpy(data.baidu_contact_str, contact_str);
    if(contact_tel)strcpy(data.baidu_contact_tel, contact_tel);

    if (stop_marker == 1 || stop_marker == 3)
	data.baidu_stop_marker = 0;
    else if (stop_marker == 0 || stop_marker == 2)
	data.baidu_stop_marker = -1;
    else
	data.baidu_stop_marker = -2;

    int sockfd = (int)arg;
    write(sockfd, &data, sizeof(data));

    fprintf(stdout, "sem_id=%d\n", sem_id);
    fprintf(stdout, "baidu_id=%d\n", baidu_id);
    fprintf(stdout, "account=%s\n", account);
    fprintf(stdout, "password=%s\n", password);
    fprintf(stdout, "token=%s\n", token);
    fprintf(stdout, "web_name=%s\n", web_name);
    fprintf(stdout, "domain=%s\n", domain);
    fprintf(stdout, "company_name=%s\n", company_name);
    fprintf(stdout, "contact_str=%s\n", contact_str);
    fprintf(stdout, "contact_tel=%s\n", contact_tel);
    fprintf(stdout, "stop_marker=%d\n", stop_marker);
    fprintf(stdout, "==================\n");
}

void packet_login(int sockfd, char *buffer, size_t size)
{

    packet_login_t *login = NULL;
    login = (packet_login_t *)buffer;
    fprintf(stdout, "packet_login:sem_account=%s sem_password=%s\n", login->sem_account, login->sem_password);

    packet_return_t pack_ret = {0};
    pack_ret.ret = soap_manager_login(
	    login->sem_account, 
	    login->sem_password, 
	    &pack_ret.sem_id, 
	    &pack_ret.code);

    fprintf(stdout, "packet_login:ret=%d sem_id=%d code=%d\n", pack_ret.ret, pack_ret.sem_id, pack_ret.code);
    write(sockfd, &pack_ret, sizeof(pack_ret));
}

void packet_sem_pass_update(int sockfd, char *buffer, size_t size)
{

    packet_sem_pass_update_t *sem_pass = NULL;
    sem_pass = (packet_sem_pass_update_t *)buffer;

    char md5_password[128] = {0};
    md5_to_string(sem_pass->sem_password, md5_password);

    packet_return_t pack_ret = {0};
    pack_ret.ret = soap_manager_pass_update(
	    sem_pass->sem_account, 
	    md5_password,
	    sem_pass->new_sem_password,
	    &pack_ret.code);
    write(sockfd, &pack_ret, sizeof(pack_ret));
}

void packet_baidu_add(int sockfd, char *buffer, size_t size)
{

    packet_baidu_add_t *baidu_add = NULL;
    baidu_add = (packet_baidu_add_t *)buffer;

    packet_return_t pack_ret = {0};
    pack_ret.ret = soap_manager_baidu_add(
	    baidu_add->sem_id,
	    baidu_add->baidu_account,
	    baidu_add->baidu_password,
	    baidu_add->baidu_domain,
	    *baidu_add->baidu_token?baidu_add->baidu_token:NULL,
	    baidu_add->baidu_web_name,
	    baidu_add->baidu_company_name,
	    *baidu_add->baidu_contact_str?baidu_add->baidu_contact_str:NULL,
	    *baidu_add->baidu_contact_tel?baidu_add->baidu_contact_tel:NULL,
	    &pack_ret.code);

    write(sockfd, &pack_ret, sizeof(pack_ret));
}

void packet_baidu_del(int sockfd, char *buffer, size_t size)
{

    packet_baidu_del_t *baidu_del = NULL;
    baidu_del = (packet_baidu_del_t *)buffer;

    packet_return_t pack_ret = {0};
    pack_ret.ret = soap_manager_baidu_del(baidu_del->sem_id, baidu_del->baidu_id, &pack_ret.code);
    write(sockfd, &pack_ret, sizeof(pack_ret));
}

void packet_baidu_mod(int sockfd, char *buffer, size_t size)
{

    packet_baidu_mod_t *baidu_mod = NULL;
    baidu_mod = (packet_baidu_mod_t *)buffer;

    packet_return_t pack_ret = {0};
    pack_ret.ret = soap_manager_baidu_update(
	    baidu_mod->sem_id,
	    baidu_mod->baidu_id,
	    baidu_mod->baidu_account,
	    baidu_mod->baidu_password,
	    baidu_mod->baidu_domain,
	    *baidu_mod->baidu_token?baidu_mod->baidu_token:NULL,
	    baidu_mod->baidu_web_name,
	    baidu_mod->baidu_company_name,
	    *baidu_mod->baidu_contact_str?baidu_mod->baidu_contact_str:NULL,
	    *baidu_mod->baidu_contact_tel?baidu_mod->baidu_contact_tel:NULL,
	    &pack_ret.code);
    write(sockfd, &pack_ret, sizeof(pack_ret));
}

void packet_baidu_get(int sockfd, char *buffer, size_t size)
{

    packet_baidu_get_t *baidu_get = NULL;
    baidu_get = (packet_baidu_get_t *)buffer;

    packet_return_t pack_ret = {0};
    pack_ret.ret = soap_manager_baidu_get(baidu_get->sem_id, packet_baidu_get_call, (void *)sockfd, &pack_ret.code);
    if (pack_ret.ret != 0)
	write(sockfd, &pack_ret, sizeof(pack_ret));
}

void packet_baidu_getall(int sockfd, char *buffer, size_t size)
{

    packet_return_t pack_ret = {0};
    pack_ret.ret = soap_manager_baidu_get_all(packet_baidu_get_call, (void *)sockfd, &pack_ret.code);
    if (pack_ret.ret != 0)
	write(sockfd, &pack_ret, sizeof(pack_ret));
}

void *packet_analysis(void *arg)
{ 

    int sockfd = (int)arg;
    char buffer[1024] = {0};
    size_t size = read(sockfd, buffer, 1024);
    int msid = *(int *)buffer;
    fprintf(stdout, "size=%zd msid:%d\n", size, msid);

    switch(msid){

	case 1000:
	    packet_login(sockfd, buffer, size);
	    break;

	case 1001:
	    packet_sem_pass_update(sockfd, buffer, size);
	    break;

	case 1002:
	    packet_baidu_add(sockfd, buffer, size);
	    break;

	case 1003:
	    packet_baidu_del(sockfd, buffer, size);
	    break;

	case 1004:
	    packet_baidu_mod(sockfd, buffer, size);
	    break;

	case 1005:
	    packet_baidu_get(sockfd, buffer, size);
	    break;

	case 1006:
	    packet_baidu_getall(sockfd, buffer, size);
	    break;
    }

    close(sockfd);
    pthread_exit(NULL);
}

void demo_call(
	int sem_id,
	int baidu_id,
	char *account,
	char *password,
	char *token,
	char *web_name,
	char *domain,
	char *company_name,
	char *contact_str,
	char *contact_tel,
	int stop_marker,
	void *arg)
{

    fprintf(stdout, "account:%s\n", account);
    fprintf(stdout, "password:%s\n", password);
    fprintf(stdout, "domain:%s\n", domain);
    fprintf(stdout, "===================================\n");
}

int main(int argc, char *argv[])
{
   
    int code = -1;
    return soap_manager_baidu_get_all(demo_call, NULL, &code);

#if 1
    pid_t pid = fork();
    if (pid < 0) {

	fprintf(stdout, "[fork() err!]\n");
	exit(0);

    } else if (pid > 0) {

	exit(0);
    }
#endif

    int flag = 1;
    struct sockaddr_in addr = {0};
    addr.sin_addr.s_addr = 0;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(8001);

    int listfd = socket(PF_INET, SOCK_STREAM, 0);
    setsockopt(listfd, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(int));
    bind(listfd, (struct sockaddr *)&addr, sizeof(addr));
    listen(listfd, 100);

    signal(SIGCHLD, signal_function);
    signal(SIGSEGV, signal_function);
    signal(SIGPIPE, signal_function);

    while(1){

	int sockfd  = -1;
	if ((sockfd = accept(listfd, NULL, NULL)) > 0) {

	    pthread_t tid;
	    pthread_create(&tid, NULL, packet_analysis, (void *)sockfd);
	    //packet_analysis(sockfd);
	}
    }

    return 0;
}


