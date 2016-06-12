
#include "ad_system_display_db.h"

int ad_system_db_netwrok_init()
{

    struct sockaddr_un addr;
    memset(&addr, 0, sizeof(struct sockaddr_un));
    addr.sun_family = AF_UNIX;
    strcpy(addr.sun_path, SOCK_PATH);
    unlink(SOCK_PATH);

    int listenfd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (listenfd < 0)
	return -1;

    if (bind(listenfd, (struct sockaddr *)&addr, sizeof(struct sockaddr_un)))
	return -1;

    if (listen(listenfd, 1000))
	return -1;

    if (chmod(SOCK_PATH, S_ISUID
		| S_ISGID
		| S_ISVTX
		| S_IRUSR
		| S_IWUSR
		| S_IXUSR
		| S_IRGRP
		| S_IWGRP
		| S_IXGRP
		| S_IROTH
		| S_IWOTH
		| S_IXOTH))
	return -1;

    return listenfd;
}

void *ad_system_db_netwrok(void *arg)
{

    char buffer[128];
    int sockfd = (size_t)arg;
    pthread_detach(pthread_self());

    while(1) {

	size_t size = read(sockfd, buffer, 128);
	if (size <= 0) break;

	buffer[size] = 0;
	size = ad_system_db_packet(sockfd, buffer);
	if (size <= 0) break;
    }

    close(sockfd);
    pthread_exit(NULL);
}



