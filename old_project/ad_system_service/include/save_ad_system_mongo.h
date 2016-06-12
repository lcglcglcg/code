
#ifndef	__AD_SYSTEM_MONGO_H__
#define	__AD_SYSTEM_MONGO_H__

#include "bson.h"
#include "mongoc.h"
#include "lg_list.h"

#define	MONGO_DB_NAME "adx"

#ifdef __cplusplus
extern "C" { 
#endif

    /* api */
    typedef struct {

	mongoc_client_t *client;
	mongoc_collection_t *collection;
	mongoc_cursor_t *cursor;
	const bson_t *doc;
	bson_error_t error;

    } ad_system_mongo_t;

    extern int ad_system_mongo_init(lg_list_t *config_file_head);
    extern int ad_system_mongo_conn(ad_system_mongo_t *p);
    extern void ad_system_mongo_free(ad_system_mongo_t *p);

    /* 业务逻辑 */

    /* report list */
    typedef struct {

	lg_list_t list;
	char report_id[64];
	char report_name[256];
	char report_type[128];
	char range_time[128];
	char create_time[128];
	char status[128];
	char path[1024];

    } ad_system_mongo_report_list_t;

    extern int ad_system_mongo_report_list_add(const char *account_id,
	    const char *report_id, 
	    const char *report_name, 
	    int report_type, 
	    int text_or_img, 
	    const char *time_range, 
	    const char *path);
    extern int ad_system_mongo_report_list_set(char *account_id, char *report_id, int status);
    extern int ad_system_mongo_report_list_get(char *account_id, int report_belong, lg_list_t *head);
    extern int ad_system_mongo_report_list_del(char *account_id, const char *report_id);

    /* stat trend load */
    extern int ad_system_mongo_this_day_load(lg_list_t *head, char *account_id, int display_style);
    extern int ad_system_mongo_last_day_load(lg_list_t *head, char *account_id, int display_style);
    extern int ad_system_mongo_seven_day_load(lg_list_t *head, char *account_id, int display_style);

    /* money trend load */
    typedef struct {

	lg_list_t list;
	char date[20];
	double cost;

    } ad_system_mongo_money_trend_t;
    extern int ad_system_mongo_money_trend_load(char *account_id, lg_list_t *year_head, lg_list_t *month_head);

    //后台->投放中心
    extern int ad_system_mongo_delivery_center_find(const char *key, char *account_id_str);

    //首页->曲线图
    extern int ad_system_mongo_stat_trend_seven_day_load(char *account_id, char *table, lg_list_t *head, int display_style);


    //标准报告
    extern int ad_system_mongo_report_standard_load(lg_list_t *head,
	    const char *table,
	    const char *account_id_str, 
	    const char *date_s_str, 
	    const char *date_e_str, 
	    int date_type,
	    int display_style,
	    int bidding_model);

    //综合报告
    extern int ad_system_mongo_report_complex_load(lg_list_t *head,
	    const char *table,
	    const char *account_id_str, 
	    const char *date_s_str, 
	    const char *date_e_str, 
	    int date_type,
	    int display_style,
	    int bidding_model);

    extern void ad_system_mongo_list_free(lg_list_t *head);

#ifdef __cplusplus
}
#endif

#endif




