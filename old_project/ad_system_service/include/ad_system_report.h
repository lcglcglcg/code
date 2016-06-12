
#ifndef	__AD_SYSTEM_REPORT_H__
#define	__AD_SYSTEM_REPORT_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lg_list.h"

#define	REPORT_XLS_FILE_PATH "/var/www/html"

#ifdef __cplusplus
extern "C" { 
#endif

    /* standard report */

    typedef struct {

	lg_list_t next;
	char creative_id[256];
	char creative_name[1024];
	int impression;
	int click;
	double cost;
	double ctr;
	double cpc;
	double cpm;

    } ad_system_report_standard_creative_t;

    typedef struct {

	lg_list_t next;
	lg_list_t head;
	char group_id[256];
	char group_name[1024];
	int impression;
	int click;
	double cost;
	double ctr;
	double cpc;
	double cpm;

    } ad_system_report_standard_group_t;

    typedef struct {

	lg_list_t next;
	lg_list_t head;
	char campaign_id[256];
	char campaign_name[1024];
	int impression;
	int click;
	double cost;
	double ctr;
	double cpc;
	double cpm;

    } ad_system_report_standard_campaign_t;

    typedef struct {

	lg_list_t next;
	lg_list_t head;
	char date[128];
	int impression;
	int click;
	double cost;
	double ctr;
	double cpc;
	double cpm;

    } ad_system_report_standard_t;

    extern int ad_system_report_standard_add(lg_list_t *head,
	    const char *date,
	    const char *campaign_id,
	    const char *group_id,
	    const char *creative_id,
	    int impression,
	    int click,
	    double cost);

    extern int ad_system_report_standard_create(
	    int display_style,
	    const char *account_id, 
	    const char *time_s,
	    const char *time_e,
	    int time_type,
	    int report_type,
	    const char *report_path);

    extern void ad_system_report_standard_free(lg_list_t *head);


    /* report complex */

    typedef struct {

	lg_list_t next;
	char region_child[256];
	int impression;
	int click;
	double cost;
	double ctr;
	double cpc;
	double cpm;

    } ad_system_report_complex_child_t;

    typedef struct {

	lg_list_t next;
	lg_list_t head;
	char region_father[256];
	int impression;
	int click;
	double cost;
	double ctr;
	double cpc;
	double cpm;

    } ad_system_report_complex_father_t;

    typedef struct {

	lg_list_t next;
	lg_list_t head;
	char date[128];
	int impression;
	int click;
	double cost;
	double ctr;
	double cpc;
	double cpm;

    } ad_system_report_complex_t;

    extern int ad_system_report_complex_add(lg_list_t *head,
	    const char *date,
	    int region_code,
	    int impression,
	    int click,
	    double cost);

    extern int ad_system_report_complex_create(
	    int display_style,
	    const char *account_id,
	    const char *time_s,
	    const char *time_e,
	    int time_type,
	    int report_type,
	    const char *report_path);

    extern void ad_system_report_complex_free(lg_list_t *head);


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
    extern int ad_system_report_path_create(char *key, char *down_path, char *file_path);
    extern int ad_system_mongo_report_list_set(char *account_id, char *report_id, int status);
    extern int ad_system_mongo_report_list_find(char *account_id, int report_belong, lg_list_t *head);
    extern int ad_system_mongo_report_list_del(char *account_id, const char *report_id);



#ifdef __cplusplus
}
#endif

#endif


