
#ifndef	__ZOOSNET_REPORT_H__
#define	__ZOOSNET_REPORT_H__

#include "sem.h"

typedef	int (zoosnet_report_day_save_backcall)(char *date_str, int size, void *arg);
typedef struct {

    unsigned int baidu_ad;//创意ID
    unsigned int baidu_kw;//keyword_ID
    unsigned int baidu_net;//搜索引擎 or 网盟
    unsigned int baidu_type;//pc or mobile

}zoosnet_report_baidu_url_t;

typedef struct zoosnet_report_node_t zoosnet_report_node_t;
struct zoosnet_report_node_t {

    unsigned int keyword_ID;
    unsigned int mobile_mark;

    char search_str[256];
    char baidu_url_str[1024];

    unsigned int dialog_type;
    unsigned int reservation;

    zoosnet_report_node_t *next;
};

typedef struct zoosnet_report_head_t zoosnet_report_head_t;
struct zoosnet_report_head_t {

    char date_str[256];
    zoosnet_report_node_t *head;
    zoosnet_report_head_t *next;
};

typedef struct reservation_node_t reservation_node_t;
struct reservation_node_t {

    char str[256];
    reservation_node_t *next;
};

extern char *strptime(const char *s, const char *format, struct tm *tm);
extern zoosnet_report_head_t *zoosnet_report_load(
	char *path,
	char *reservation_str,
	char *reservation_col_str,
	char *not_reservation_str);
extern void zoosnet_report_time_get(
	zoosnet_report_head_t *head,
	time_t *start_time, 
	time_t *end_time);
extern void zoosnet_report_day_save(
	zoosnet_report_head_t *head, 
	int sem_id, 
	zoosnet_report_day_save_backcall *backcall);
extern zoosnet_report_head_t *zoosnet_report_day_load(
	int sem_id, 
	char *start_date, 
	char *end_date);
extern void zoosnet_report_print(zoosnet_report_head_t *head);
extern void zoosnet_report_free(zoosnet_report_head_t *head);

#endif


