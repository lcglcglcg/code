
#include "ad_system_display_db.h"

// GetInfoImg 1001500000 1020004000
// GetInfoTxt 1001500000 1020004000
// GetPlace TI2OJ65M4AGD2H
// GetCode nanke
// GetCode jjxyz
// GetCode beijing

static ad_system_db_t *db = NULL;
void *ad_system_db_reload(void *p)
{

    ad_system_db_t *old_db = NULL;
    ad_system_db_t *new_db = NULL;

    for (sleep(60);;) {

	fprintf(stdout, "reload: %p ...\n", db);
	new_db = ad_system_db_element_create();
	if (new_db) {

	    old_db = db;
	    db = new_db;

	} else {

	    old_db = new_db;
	}

	sleep(60);
	// sleep(1200);
	ad_system_db_element_free(old_db);
	old_db = NULL;
    }

    pthread_exit(NULL);
}

int ad_system_db_packet_send_data(int sockfd, char *buffer, size_t size)
{

    if (write(sockfd, &size, sizeof(size_t)) <= 0)
	return -1;

    if (write(sockfd, buffer, size) <= 0)
	return -1;

    return sizeof(size_t) + size;
}

int ad_system_db_packet_send_not_data(int sockfd)
{

    size_t size = NOT_DATA_SIZE;
    if (write(sockfd, &size, sizeof(size_t)) <= 0)
	return -1;

    if (write(sockfd, NOT_DATA_STR, NOT_DATA_SIZE) <= 0)
	return -1;

    return sizeof(size_t) + size;
}

int ad_system_db_packet_send_list(int sockfd, lg_list_t *head)
{

    size_t size = 0;
    lg_list_t *p = NULL;
    lg_list_for_each(p, head) {

	ad_system_info_t *info = (ad_system_info_t *)p;
	size += info->size;
    }

    if (write(sockfd, &size, sizeof(size_t)) <= 0)
	return -1;

    lg_list_for_each(p, head) {

	ad_system_info_t *info = (ad_system_info_t *)p;
	if (write(sockfd, info->buffer, info->size) <= 0)
	    return -1;
    }

    return sizeof(size_t) + size;
}

int ad_system_db_packet(int sockfd, char *buffer)
{

    char *saveptr = NULL;
    char *str = strtok_r(buffer, " \r\n", &saveptr);
    if (!str || !(*str)) return ad_system_db_packet_send_not_data(sockfd);

    if (strcmp(str, "GetInfoImg") == 0) {

	char *region_code = strtok_r(NULL, " \r\n", &saveptr);
	char *disease_code = strtok_r(NULL, " \r\n", &saveptr);
	// char *img_size = strtok_r(NULL, " \r\n", &saveptr);
	if (region_code && disease_code) {

	    lg_list_t *head = ad_system_db_element_find_list(db->info_region_img, region_code, disease_code, NULL);
	    if (head) return ad_system_db_packet_send_list(sockfd, head);
	}

    } else if (strcmp(str, "GetInfoTxt") == 0) {

	char *region_code = strtok_r(NULL, " \r\n", &saveptr);
	char *disease_code = strtok_r(NULL, " \r\n", &saveptr);
	if (region_code && disease_code) {

	    lg_list_t *head = ad_system_db_element_find_list(db->info_region_txt, region_code, disease_code, NULL);
	    if (head) return ad_system_db_packet_send_list(sockfd, head);
	}

    } else if (strcmp(buffer, "GetCode") == 0) {


	char *key = strtok_r(NULL, " \r\n", &saveptr);
	char *value = NULL;
	int size = ad_system_db_element_find(db, &value, key, NULL);
	if (value && size) return ad_system_db_packet_send_data(sockfd, value, size);

    } else if (strcmp(buffer, "GetPlace") == 0) {

	char *key = strtok_r(NULL, " \r\n", &saveptr);
	char *value = NULL;
	int size = ad_system_db_element_find_data(db->dict_place, &value, key, NULL);
	if (value && size) return ad_system_db_packet_send_data(sockfd, value, size);
    }

    return ad_system_db_packet_send_not_data(sockfd);
}

int main()
{

    // if (fork() > 0) exit(0);

    lg_ocilib_init();
    db = ad_system_db_element_create();
    // return demo_print(db);

    pthread_t tid;
    pthread_create(&tid, NULL, ad_system_db_reload, NULL);
    int listenfd = ad_system_db_netwrok_init();
    fprintf(stdout, "runing ...\n");

    for (;;) {

	size_t sockfd = accept(listenfd, NULL, NULL);
	if (sockfd != -1) {

	    pthread_create(&tid, NULL, ad_system_db_netwrok, (void *)sockfd);
	}
    }

    return 0;
}



