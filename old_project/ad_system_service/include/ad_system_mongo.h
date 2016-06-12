
#ifndef	__AD_SYSTEM_MONGO_H__
#define	__AD_SYSTEM_MONGO_H__

#include "bson.h"
#include "mongoc.h"
#include "lg_list.h"

#if 0

#define	MONGO_DB_NAME "adx"
#define MONGO_REPORT_LIST "report_list_58"
#define MONGO_REPORT_STANDARD "report_standard_58"
#define MONGO_REPORT_REGION "report_region_58"
#define MONGO_DELIVERY_CENTER_LIST "delivery_center_list"

#define	ACCOUNT_DAY_TEMP "account_day_temp_58"
#define	ACCOUNT_YESTERDAY_TEMP "account_yesterday_temp_58"
#define	ACCOUNT_SEVENDAY_TEMP "account_sevenday_temp_58"
#define	ACCOUNT_SEVENDAY_DAILY_TEMP "account_sevenday_daily_temp_58"

#define CAMPAIGN_DAY_TEMP "campaign_day_temp_58"
#define CAMPAIGN_YESTERDAY_TEMP "campaign_yesterday_temp_58"
#define CAMPAIGN_SEVENDAY_TEMP "campaign_sevenday_temp_58"

#define GROUP_DAY_TEMP "group_day_temp_58"
#define GROUP_YESTERDAY_TEMP "group_yesterday_temp_58"
#define GROUP_SEVENDAY_TEMP "group_sevenday_temp_58"

#define CREATIVE_DAY_TEMP "creative_day_temp_58"
#define CREATIVE_YESTERDAY_TEMP "creative_yesterday_temp_58"
#define CREATIVE_SEVENDAY_TEMP "creative_sevenday_temp_58"

#else

#define	MONGO_DB_NAME "adx"
#define MONGO_REPORT_LIST "report_list"
#define MONGO_REPORT_STANDARD "report_standard"
#define MONGO_REPORT_REGION "report_region"
#define MONGO_DELIVERY_CENTER_LIST "delivery_center_list"

#define	ACCOUNT_DAY_TEMP "account_day_temp"
#define	ACCOUNT_YESTERDAY_TEMP "account_yesterday_temp"
#define	ACCOUNT_SEVENDAY_TEMP "account_sevenday_temp"
#define	ACCOUNT_SEVENDAY_DAILY_TEMP "account_sevenday_daily_temp"

#define CAMPAIGN_DAY_TEMP "campaign_day_temp"
#define CAMPAIGN_YESTERDAY_TEMP "campaign_yesterday_temp"
#define CAMPAIGN_SEVENDAY_TEMP "campaign_sevenday_temp"

#define GROUP_DAY_TEMP "group_day_temp"
#define GROUP_YESTERDAY_TEMP "group_yesterday_temp"
#define GROUP_SEVENDAY_TEMP "group_sevenday_temp"

#define CREATIVE_DAY_TEMP "creative_day_temp"
#define CREATIVE_YESTERDAY_TEMP "creative_yesterday_temp"
#define CREATIVE_SEVENDAY_TEMP "creative_sevenday_temp"

#endif

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
    extern void ad_system_mongo_list_free(lg_list_t *head);

    //标准报告载入
    extern int ad_system_mongo_report_standard_load(lg_list_t *head,
	    int display_style, int bidding_model, const char *account_id, const char *time_s, const char *time_e,int time_type);

    //地域报告载入
    extern int ad_system_mongo_report_region_load(lg_list_t *head, 
	    int display_style, int bidding_model, const char *account_id, const char *time_s, const char *time_e,int time_type);

    //后台管理登录
    extern int ad_system_mongo_delivery_center_find(const char *key, char *account_id_str);

    //统计页面
    typedef struct {

	lg_list_t next;
	char name[128];
	int impression;
	int click;
	double cost;

    } ad_system_mongo_stat_t;

    extern int ad_system_mongo_stat_load(ad_system_mongo_stat_t *p, const char *table, int display_style, const char *account_id);
    extern int ad_system_mongo_stat_list_load(lg_list_t *head, const char *table, int display_style,
	    const char *account_id, const char *name);
    extern ad_system_mongo_stat_t *ad_system_mongo_stat_list_find(lg_list_t *head, const char *name);

#ifdef __cplusplus
}
#endif

#endif




