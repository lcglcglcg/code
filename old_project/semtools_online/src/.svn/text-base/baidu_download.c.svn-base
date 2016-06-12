
#include "sem.h"
#include "baidu_user.h"
#include "baidu_report.h"
#include "calendar.h"
#include "log_file.h"

extern int baidu_download_report_search(
	char *date_str,
	char *username, 
	char *password, 
	char *token, 
	char *path);

extern int baidu_download_report_pair(
	char *date_str,
	char *username, 
	char *password, 
	char *token, 
	char *path);

void _baidu_download(baidu_user_node_t *node, calendar_t *calendar)
{

    char date[10] = {0};
    char path[1024] = {0};
    struct tm date_tm = {0};

    sprintf(path, "%s/%s", BAIDU_REPORT_PATH, node->account);
    if (access(path, 0)) {

	fprintf(stdout, "[mkdir][%s]\n", path);
	if (mkdir(path, 0755) == -1)
	    return;
    }

    time_t date_time = time(NULL);
    gmtime_r(&date_time, &date_tm);
    date_tm.tm_sec = 0;
    date_tm.tm_min = 0;
    date_tm.tm_hour = 0;
    date_time = mktime(&date_tm) - 57600;

    int i, day = 3;
    for (i = 0; i < day; i++, date_time -= 86400) {

	gmtime_r(&date_time, &date_tm);
	sprintf(date, "%04d-%02d-%02d",
		date_tm.tm_year + 1900,
		date_tm.tm_mon + 1,
		date_tm.tm_mday);

	/* download baidu_pair */

	sprintf(path, "%s/%s/baidu_pair_%04d_%02d_%02d.csv",
		BAIDU_REPORT_PATH,
		node->account,
		date_tm.tm_year + 1900,
		date_tm.tm_mon + 1,
		date_tm.tm_mday);

	int code = baidu_download_report_pair(date, node->account, node->password, node->token, path);
	fprintf(stdout, "[%d][%s][%s][%s][%s][%s]\n",
		code,
		date,
		node->account,
		node->password,
		node->token,
		path);
	if (code == 0) {

	    baidu_pair_report_node_t *pair_report = NULL;
	    pair_report = baidu_pair_report_load(path);
	    fprintf(stdout, "pair_report=%p\n", pair_report);
	    sprintf(path, "%s/%s/baidu_pair_%04d_%02d_%02d.dat",
		    BAIDU_REPORT_PATH,
		    node->account,
		    date_tm.tm_year + 1900,
		    date_tm.tm_mon + 1,
		    date_tm.tm_mday);
	    baidu_pair_report_day_save(pair_report, path);
	    baidu_pair_report_free(pair_report);

	    calendar_user_t *user = NULL;
	    if ((user = calendar_user_add(calendar, node->baidu_id)))
		calendar_node_add(user, date_time);

	} else if (code > 0) {

	    node->status = code;
	    return;
	}


	/* download baidu_search */	

	sprintf(path, "%s/%s/baidu_search_%04d_%02d_%02d.csv",
		BAIDU_REPORT_PATH,
		node->account,
		date_tm.tm_year + 1900,
		date_tm.tm_mon + 1,
		date_tm.tm_mday);

	code = baidu_download_report_pair(date, node->account, node->password, node->token, path);
	fprintf(stdout, "[%d][%s][%s][%s][%s][%s]\n",
		code,
		date,
		node->account,
		node->password,
		node->token,
		path);
	if (code == 0) {

	    baidu_search_report_node_t *search_report = NULL;
	    search_report = baidu_search_report_load(path);
	    fprintf(stdout, "search_report=%p\n", search_report);
	    sprintf(path, "%s/%s/baidu_search_%04d_%02d_%02d.dat",
		    BAIDU_REPORT_PATH,
		    node->account,
		    date_tm.tm_year + 1900,
		    date_tm.tm_mon + 1,
		    date_tm.tm_mday);
	    baidu_search_report_day_save(search_report, path);
	    baidu_search_report_free(search_report);

	} else if (code > 0) {

	    node->status = code;
	    return;
	}

    }
}

void baidu_download(baidu_user_t *baidu_list, calendar_t *calendar)
{

    baidu_user_node_t *node = NULL;
    for (node = baidu_list->head; node; node = node->next) {

	if (node->status == 0 && node->del_mask == 0)
	    _baidu_download(node, calendar);
    }
}


void baidu_download_sem(baidu_user_t *baidu_list, calendar_t *calendar, int sem_id)
{

    baidu_user_node_t *node = NULL;
    for (node = baidu_list->head; node; node = node->next) {

	if (node->sem_id == sem_id && node->status == 0 && node->del_mask == 0)
	    _baidu_download(node, calendar);
    }
}


