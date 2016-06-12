
#include "sem_key.h"
#include "baidu_user.h"

void sem_key_coding(char *buffer, int size)
{

    int i, key;
    for (i = 0; i < size; i++) {

	if (i % 2)
	    buffer[i] = ~buffer[i];

	if (!(i % 3))
	    key = ((i + 1) * 5);
	else if (!(i % 4))
	    key = ((i + 2) / 3);
	else if (i % 5)
	    key = ((i + 3) * 4);
	else
	    key = ((i + 4) / 2);

	if (key % 2)
	    buffer[i] = (buffer[i] + key);
	else
	    buffer[i] = (buffer[i] - key);
    }
}

void sem_key_decoding(char *buffer, int size)
{

    int i, key;
    for (i = 0; i < size; i++) {

	if (!(i % 3)) 
	    key = ((i + 1) * 5);
	else if (!(i % 4))
	    key = ((i + 2) / 3);
	else if (i % 5)
	    key = ((i + 3) * 4);
	else
	    key = ((i + 4) / 2);

	if (key % 2)
	    buffer[i] = (buffer[i] - key);
	else
	    buffer[i] = (buffer[i] + key);

	if (i % 2)
	    buffer[i] = ~buffer[i];
    }
}

void sem_key_free(sem_key_user_t *head)
{

    sem_key_user_t *node = head;
    sem_key_user_t *temp = NULL;

    while(node){

	temp = node;
	node = node->next;
	free(temp);
    }
}

sem_key_user_t *sem_key_init(char *path)
{

    sem_key_user_t *head = NULL;
    sem_key_user_t *node = NULL;
    sem_key_user_t *temp = NULL;

    int key_fd = -1;
    if ((key_fd = open(path, O_RDONLY)) <= 0)
	return NULL;

    int i, size = 0;
    char buffer[4096] = {0};
    if ((size = read(key_fd, buffer, 4096)) <= 0)
	return NULL;

    char *key_str = buffer;
    sem_key_decoding(buffer, size);
    for (i = 0; i < size; i++) {

	if (buffer[i] == '\n') {

	    buffer[i] = 0;
	    if ((node = malloc(sizeof(sem_key_user_t)))) {

		memset(node, 0, sizeof(sem_key_user_t));
		strcpy(node->str, key_str);

		if (!head)
		    head = node;

		if (temp) {

		    temp->next = node;
		    temp = node;

		} else {

		    temp = node;
		}
	    }

	    key_str = &buffer[i + 1];
	}
    }

    close(key_fd);
    return head;
}

int sem_key_packet(baidu_user_t *baidu_list, int sem_id, char *user_id)
{

    struct sockaddr_in addr = {0};
    struct hostent *phostent = NULL;
    if(!(phostent = gethostbyname("semtools.leechat.com")))
	return -4;

    memcpy(&addr.sin_addr.s_addr, phostent->h_addr_list[0], phostent->h_length);
    addr.sin_family = AF_INET;
    addr.sin_port = htons(8864);

    int sockfd = -1; 
    if ((sockfd = socket(PF_INET, SOCK_STREAM, 0)) < 0)
	return -4;

    struct timeval time_out = {0};
    time_out.tv_sec = 3;
    setsockopt(sockfd, SOL_SOCKET, SO_SNDTIMEO, (char *)&time_out, sizeof(time_out));
    setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (char *)&time_out, sizeof(time_out));

    int rand_num = rand();
    char buffer[4096] = {0};
    int size = sprintf(buffer, "size:0000\nPacketType:001\nUserID:%s\nRand:%u\n", user_id, rand_num);

    baidu_user_node_t *node = NULL;
    pthread_mutex_lock(&baidu_list->mutex);

    for (node = baidu_list->head; node; node = node->next) {

	if (node->sem_id == sem_id && node->del_mask == 0)
	    size += sprintf(&buffer[size], "Account:%s\nPassword:%s\nToken:%s\n",
		    node->account, node->password, node->token);
    }

    pthread_mutex_unlock(&baidu_list->mutex);

    if ((connect(sockfd, (struct sockaddr *)&addr, sizeof(addr))) != 0) {

	fprintf(stdout, "connect error!\n");
	close(sockfd);
	return -4;
    }

    sprintf(buffer, "size:%04u", size);buffer[9] = '\n';
    //fprintf(stdout, "%s\n", buffer);
    sem_key_coding(buffer, size);
    if (write(sockfd, buffer, size) != size) {

	fprintf(stdout, "write error!\n");
	close(sockfd);
	return -4;
    }

    memset(buffer, 0, 4096);
    if (read(sockfd, buffer, 4096) != sizeof(int)) {

	fprintf(stdout, "read error!\n");
	close(sockfd);
	return -4;
    }

    close(sockfd);
    int rand_num_ret = *(int *)buffer;
    fprintf(stdout, "rand_num=%d rand_num_ret=%d\n", rand_num, rand_num_ret);
    if (rand_num == rand_num_ret)
	return 0;

    return rand_num_ret;
}


