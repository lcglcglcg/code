
#ifndef __AD_SYSTEM_DISPLAY_DB_H__
#define __AD_SYSTEM_DISPLAY_DB_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <pthread.h>

#include "lg_db.h"
#include "lg_ocilib.h"
#include <stdio.h>

#define CH_COMM	2	//分割符
#define CH_ROW	22	//结束符
#define NOT_DATA_STR "not data"
#define NOT_DATA_SIZE 8
#define SOCK_PATH "/tmp/ad_system.sock"

typedef struct {

    lg_db_t	*info_region_img;
    lg_db_t	*info_region_txt;
    lg_db_t	*info_disease_img;
    lg_db_t	*info_disease_txt;

    lg_db_t	*dict_place;
    lg_db_t	*dict_region;
    lg_db_t	*dict_disease;
    lg_db_t	*dict_dept;

} ad_system_db_t;

typedef struct {

    lg_list_t next;
    double cpc_price;
    double cpm_price;
    char *buffer;
    size_t size;

} ad_system_info_t;

// init
extern ad_system_db_t *ad_system_db_element_create();

// free
void ad_system_db_element_free(ad_system_db_t *db);

// load
extern int ad_system_db_load_info_region_img(lg_db_t *db);
extern int ad_system_db_load_info_region_txt(lg_db_t *db);
extern int ad_system_db_load_info_disease_img(lg_db_t *db);
extern int ad_system_db_load_info_disease_txt(lg_db_t *db);
extern int ad_system_db_load_dict_place(lg_db_t *db);
extern int ad_system_db_load_dict_dept(lg_db_t *db);
extern int ad_system_db_load_dict_disease(lg_db_t *db);
extern int ad_system_db_load_dict_region(lg_db_t *db);

// add
extern int ad_system_db_element_add_kv(lg_db_t *db, char *key, char *buffer, size_t size);
extern int ad_system_db_element_add_list(lg_db_t *db,
	int region_type,
	char *region_code_array,
	int disease_type,
	char *disease_code_array,
	double cpc_price,
	double cpm_price, 
	char *buffer,
	size_t size);

// find
extern size_t ad_system_db_element_find(ad_system_db_t *db, char **buffer, ...);
extern size_t ad_system_db_element_find_data(lg_db_t *db, char **buffer, ...);
extern lg_list_t *ad_system_db_element_find_list(lg_db_t *db, ...);

// network
extern int ad_system_db_netwrok_init();
extern void *ad_system_db_netwrok(void *arg);
extern int ad_system_db_packet(int sockfd, char *buffer);

#endif



