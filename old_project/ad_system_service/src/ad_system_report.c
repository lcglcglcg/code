
#include "lg_config_file.h"
#include "ad_system_sql.h"
#include "ad_system_mongo.h"
#include "ad_system_report.h"
#include "ad_system_report_account_info.h"
#include "ad_system_report_xls_file.h"
#include "ad_system_region_code.h"

#include <uuid/uuid.h>

int ad_system_report_path_create(char *key, char *down_path, char *file_path)
{

    int i, j;
    char path[128] = {0};
    char _path[128] = {0};

    uuid_t uuid;
    uuid_generate(uuid);

    for (i = 0, j = 0; i < 16; i++, j++, j++)
	sprintf(&key[j], "%02x", uuid[i]);

    for (i = 0, j = 0; i < 32; i++) {

	path[j++] = '/';
	path[j++] = key[i];

	sprintf(_path, "%s%s", REPORT_XLS_FILE_PATH, path);
	if (access(_path, 0))
	    if (mkdir(_path, S_IRWXU | S_IRWXG | S_IRWXO) != 0)
		return -1;
    }

    sprintf(down_path, "%s/report.xls", path);
    sprintf(file_path, "%s%s/report.xls", REPORT_XLS_FILE_PATH, path);
    return 0;
}

/*******************/
/* report standard */
/*******************/

int ad_system_report_standard_creative_add(lg_list_t *head, 
	const char *creative_id,
	int impression,
	int click, 
	double cost)
{

    lg_list_t *p = NULL;
    ad_system_report_standard_creative_t *node = NULL;
    lg_list_for_each(p, head) {

	node = (ad_system_report_standard_creative_t *)p;
	if (strcmp(node->creative_id, creative_id) == 0) {

	    node->impression += impression;;
	    node->click += click;
	    node->cost += cost;
	    if (node->click && node->impression)
		node->ctr = (double)node->click / (double)node->impression;
	    if (node->cost && node->click)
		node->cpc = node->cost / (double)node->click;
	    if (node->cost && node->impression)
		node->cpm = node->cost / (double)node->impression * (double)1000;
	    return 0;
	}
    }

    if (!(node = malloc(sizeof(ad_system_report_standard_creative_t))))
	return -1;

    strcpy(node->creative_id, creative_id);
    strcpy(node->creative_name, "其他");
    node->impression = impression;
    node->click = click;
    node->cost = cost;
    node->ctr = 0;
    node->cpc = 0;
    node->cpm = 0;
    if (node->click && node->impression)
	node->ctr = (double)node->click / (double)node->impression;
    if (node->cost && node->click)
	node->cpc = node->cost / (double)node->click;
    if (node->cost && node->impression)
	node->cpm = node->cost / (double)node->impression * (double)1000;
    lg_list_add(head, &node->next);
    return 0;
}

int ad_system_report_standard_group_add(lg_list_t *head, 
	const char *group_id, 
	const char *creative_id, 
	int impression,
	int click, 
	double cost)
{

    lg_list_t *p = NULL;
    ad_system_report_standard_group_t *node = NULL;
    lg_list_for_each(p, head) {

	node = (ad_system_report_standard_group_t *)p;
	if (strcmp(node->group_id, group_id) == 0) {

	    node->impression += impression;;
	    node->click += click;
	    node->cost += cost;
	    if (node->click && node->impression)
		node->ctr = (double)node->click / (double)node->impression;
	    if (node->cost && node->click)
		node->cpc = node->cost / (double)node->click;
	    if (node->cost && node->impression)
		node->cpm = node->cost / (double)node->impression * (double)1000;

	    return ad_system_report_standard_creative_add(&node->head,
		    creative_id,
		    impression,
		    click,
		    cost);
	}
    }

    if (!(node = malloc(sizeof(ad_system_report_standard_group_t))))
	return -1;

    lg_list_init(&node->head);
    lg_list_add(head, &node->next);
    strcpy(node->group_id, group_id);
    strcpy(node->group_name, "其他");
    node->impression = impression;
    node->click = click;
    node->cost = cost;
    node->ctr = 0;
    node->cpc = 0;
    node->cpm = 0;
    if (node->click && node->impression)
	node->ctr = (double)node->click / (double)node->impression;
    if (node->cost && node->click)
	node->cpc = node->cost / (double)node->click;
    if (node->cost && node->impression)
	node->cpm = node->cost / (double)node->impression * (double)1000;

    return ad_system_report_standard_creative_add(&node->head,
	    creative_id,
	    impression,
	    click,
	    cost);
}

int ad_system_report_standard_campaign_add(lg_list_t *head,
	const char *campaign_id,
	const char *group_id,
	const char *creative_id,
	int impression,
	int click,
	double cost)
{

    lg_list_t *p = NULL;
    ad_system_report_standard_campaign_t*node = NULL;
    lg_list_for_each(p, head) {

	node = (ad_system_report_standard_campaign_t *)p;
	if (strcmp(node->campaign_id, campaign_id) == 0) {

	    node->impression += impression;;
	    node->click += click;
	    node->cost += cost;
	    if (node->click && node->impression)
		node->ctr = (double)node->click / (double)node->impression;
	    if (node->cost && node->click)
		node->cpc = node->cost / (double)node->click;
	    if (node->cost && node->impression)
		node->cpm = node->cost / (double)node->impression * (double)1000;
	    return ad_system_report_standard_group_add(&node->head,
		    group_id,
		    creative_id,
		    impression,
		    click,
		    cost);
	}
    }

    if (!(node = malloc(sizeof(ad_system_report_standard_campaign_t))))
	return -1;

    lg_list_init(&node->head);
    lg_list_add(head, &node->next);
    strcpy(node->campaign_id, campaign_id);
    strcpy(node->campaign_name, "其他");
    node->impression = impression;
    node->click = click;
    node->cost = cost;
    node->ctr = 0;
    node->cpc = 0;
    node->cpm = 0;
    if (node->click && node->impression)
	node->ctr = (double)node->click / (double)node->impression;
    if (node->cost && node->click)
	node->cpc = node->cost / (double)node->click;
    if (node->cost && node->impression)
	node->cpm = node->cost / (double)node->impression * (double)1000;

    return ad_system_report_standard_group_add(&node->head,
	    group_id,
	    creative_id,
	    impression,
	    click,
	    cost);
}

int ad_system_report_standard_add(lg_list_t *head,
	const char *date,
	const char *campaign_id,
	const char *group_id,
	const char *creative_id,
	int impression,
	int click,
	double cost)
{

    lg_list_t *p = NULL;
    ad_system_report_standard_t *node = NULL;
    lg_list_for_each(p, head) {

	node = (ad_system_report_standard_t *)p;
	if (strcmp(node->date, date) == 0) {

	    node->impression += impression;;
	    node->click += click;
	    node->cost += cost;
	    if (node->click && node->impression)
		node->ctr = (double)node->click / (double)node->impression;
	    if (node->cost && node->click)
		node->cpc = node->cost / (double)node->click;
	    if (node->cost && node->impression)
		node->cpm = node->cost / (double)node->impression * (double)1000;

	    return ad_system_report_standard_campaign_add(&node->head,
		    campaign_id,
		    group_id,
		    creative_id,
		    impression,
		    click,
		    cost);
	}
    }

    if (!(node = malloc(sizeof(ad_system_report_standard_t))))
	return -1;

    lg_list_init(&node->head);
    lg_list_add(head, &node->next);
    strcpy(node->date, date);
    node->impression = impression;
    node->click = click;
    node->cost = cost;
    node->ctr = 0;
    node->cpc = 0;
    node->cpm = 0;
    if (node->click && node->impression)
	node->ctr = (double)node->click / (double)node->impression;
    if (node->cost && node->click)
	node->cpc = node->cost / (double)node->click;
    if (node->cost && node->impression)
	node->cpm = node->cost / (double)node->impression * (double)1000;

    return ad_system_report_standard_campaign_add(&node->head,
	    campaign_id,
	    group_id,
	    creative_id,
	    impression,
	    click,
	    cost);
}

void ad_system_report_standard_creative_free(lg_list_t *head)
{

    lg_list_t *p = head->next;
    ad_system_report_standard_creative_t *node = NULL;

    while(p != head) {

	node = (ad_system_report_standard_creative_t *)p;
	//fprintf(stdout, "[creative][%s]\n", node->creative);
	p = p->next;
	free(node);
    }
}

void ad_system_report_standard_group_free(lg_list_t *head)
{

    lg_list_t *p = head->next;
    ad_system_report_standard_group_t *node = NULL;

    while(p != head) {

	node = (ad_system_report_standard_group_t *)p;
	//fprintf(stdout, "[group][%s]\n", node->group);
	ad_system_report_standard_creative_free(&node->head);
	p = p->next;
	free(node);
    }
}

void ad_system_report_standard_campaign_free(lg_list_t *head)
{

    lg_list_t *p = head->next;
    ad_system_report_standard_campaign_t *node = NULL;

    while(p != head) {

	node = (ad_system_report_standard_campaign_t *)p;
	//fprintf(stdout, "[campaign][%s]\n", node->campaign);
	ad_system_report_standard_group_free(&node->head);
	p = p->next;
	free(node);
    }
}

void ad_system_report_standard_free(lg_list_t *head)
{

    lg_list_t *p = head->next;
    ad_system_report_standard_t *node = NULL;

    while(p != head) {

	node = (ad_system_report_standard_t *)p;
	//fprintf(stdout, "[data][%s]\n", node->date);
	ad_system_report_standard_campaign_free(&node->head);
	p = p->next;
	free(node);
    }
}

int ad_system_report_standard_load(lg_list_t *head,
	int display_style,
	int bidding_model,
	const char *account_id, 
	const char *time_s,
	const char *time_e,
	int time_type)
{

    ad_system_report_account_info_root_t account_info_head;
    if (ad_system_get_report_account_info(&account_info_head, display_style))
	return -1;

    lg_list_init(head);
    if (ad_system_mongo_report_standard_load(head,
		display_style, 
		bidding_model, 
		account_id, 
		time_s, 
		time_e, 
		time_type)) {

	ad_system_report_account_info_free(&account_info_head);
	return -1;
    }

    lg_list_t *p = NULL;
    lg_list_for_each(p, head){

	ad_system_report_standard_t *node = (ad_system_report_standard_t *)p;

	lg_list_t *campaign_p = NULL;
	lg_list_for_each(campaign_p, &node->head){

	    ad_system_report_standard_campaign_t *campaign = (ad_system_report_standard_campaign_t *)campaign_p;

	    lg_list_t *group_p = NULL;
	    lg_list_for_each(group_p, &campaign->head){

		ad_system_report_standard_group_t *group = (ad_system_report_standard_group_t *)group_p;

		lg_list_t *creative_p = NULL;
		lg_list_for_each(creative_p, &group->head){

		    ad_system_report_standard_creative_t *creative = (ad_system_report_standard_creative_t *)creative_p;

		    ad_system_report_account_info_t *account_info = NULL;
		    if ((account_info = ad_system_report_account_info_find(&account_info_head, creative->creative_id))) {

			strcpy(campaign->campaign_name, account_info->campaign_name);
			strcpy(group->group_name, account_info->group_name);
			strcpy(creative->creative_name, account_info->creative_name);
		    }
		}
	    }
	}
    }

    ad_system_report_account_info_free(&account_info_head);
    return 0;
}

int ad_system_report_standard_create(
	int display_style,
	const char *account_id, 
	const char *time_s,
	const char *time_e,
	int time_type,
	int report_type,
	const char *report_path)
{

    lg_list_t cpc_head, cpm_head;
    ad_system_report_standard_load(&cpc_head,
	    display_style,
	    2, //bidding_model=cpc
	    account_id, 
	    time_s,
	    time_e,
	    time_type);

    if (display_style == 1)
	ad_system_report_standard_load(&cpm_head,
		1,//img
		1,//cpm
		account_id, 
		time_s,
		time_e,
		time_type);

    if (display_style == 1)
	ad_system_report_xls_file_standard(&cpc_head, &cpm_head, report_type, report_path);
    else
	ad_system_report_xls_file_standard(&cpc_head, NULL, report_type, report_path);

    if (display_style == 1)
	ad_system_report_standard_free(&cpm_head);
    ad_system_report_standard_free(&cpc_head);
    return 0;
}


/******************/
/* report complex */
/******************/

int ad_system_report_complex_child_add(lg_list_t *head,
	const char *region_child,
	int impression,
	int click,
	double cost)
{

    lg_list_t *p = NULL;
    ad_system_report_complex_child_t *node = NULL;
    lg_list_for_each(p, head) {

	node = (ad_system_report_complex_child_t *)p;
	if (strcmp(node->region_child, region_child) == 0) {

	    node->impression += impression;;
	    node->click += click;
	    node->cost += cost;
	    if (node->click && node->impression)
		node->ctr = (double)node->click / (double)node->impression;
	    if (node->cost && node->click)
		node->cpc = node->cost / (double)node->click;
	    if (node->cost && node->impression)
		node->cpm = node->cost / (double)node->impression * (double)1000;

	    return 0;
	}
    }

    if (!(node = malloc(sizeof(ad_system_report_complex_child_t))))
	return -1;

    lg_list_add(head, &node->next);
    strcpy(node->region_child, region_child);
    node->impression = impression;
    node->click = click;
    node->cost = cost;
    node->ctr = 0;
    node->cpc = 0;
    node->cpm = 0;
    if (node->click && node->impression)
	node->ctr = (double)node->click / (double)node->impression;
    if (node->cost && node->click)
	node->cpc = node->cost / (double)node->click;
    if (node->cost && node->impression)
	node->cpm = node->cost / (double)node->impression * (double)1000;

    return 0;
}

int ad_system_report_complex_father_add(lg_list_t *head,
	const char *region_father,
	const char *region_child,
	int impression,
	int click,
	double cost)

{

    lg_list_t *p = NULL;
    ad_system_report_complex_father_t *node = NULL;
    lg_list_for_each(p, head) {

	node = (ad_system_report_complex_father_t *)p;
	if (strcmp(node->region_father, region_father) == 0) {

	    node->impression += impression;;
	    node->click += click;
	    node->cost += cost;
	    if (node->click && node->impression)
		node->ctr = (double)node->click / (double)node->impression;
	    if (node->cost && node->click)
		node->cpc = node->cost / (double)node->click;
	    if (node->cost && node->impression)
		node->cpm = node->cost / (double)node->impression * (double)1000;

	    return ad_system_report_complex_child_add(
		    &node->head,
		    region_child,
		    impression,
		    click,
		    cost);
	}
    }

    if (!(node = malloc(sizeof(ad_system_report_complex_father_t))))
	return -1;

    lg_list_add(head, &node->next);
    lg_list_init(&node->head);
    strcpy(node->region_father, region_father);
    node->impression = impression;
    node->click = click;
    node->cost = cost;
    node->ctr = 0;
    node->cpc = 0;
    node->cpm = 0;
    if (node->click && node->impression)
	node->ctr = (double)node->click / (double)node->impression;
    if (node->cost && node->click)
	node->cpc = node->cost / (double)node->click;
    if (node->cost && node->impression)
	node->cpm = node->cost / (double)node->impression * (double)1000;

    return ad_system_report_complex_child_add(
	    &node->head,
	    region_child,
	    impression,
	    click,
	    cost);
}

int ad_system_report_complex_add(lg_list_t *head,
	const char *date,
	int region_code,
	int impression,
	int click,
	double cost)
{

    ad_system_region_code_t *region = ad_system_region_code_find(region_code);

    lg_list_t *p = NULL;
    ad_system_report_complex_t *node = NULL;
    lg_list_for_each(p, head) {

	node = (ad_system_report_complex_t *)p;
	if (strcmp(node->date, date) == 0) {

	    node->impression += impression;
	    node->click += click;
	    node->cost += cost;
	    if (node->click && node->impression)
		node->ctr = (double)node->click / (double)node->impression;
	    if (node->cost && node->click)
		node->cpc = node->cost / (double)node->click;
	    if (node->cost && node->impression)
		node->cpm = node->cost / (double)node->impression * (double)1000;

	    return ad_system_report_complex_father_add(
		    &node->head,
		    region?region->father:"其他",
		    region?region->child:"其他",
		    impression,
		    click,
		    cost);
	}
    }

    if (!(node = malloc(sizeof(ad_system_report_complex_t))))
	return -1;

    lg_list_init(&node->head);
    lg_list_add(head, &node->next);
    strcpy(node->date, date);
    node->impression = impression;
    node->click = click;
    node->cost = cost;
    node->ctr = 0;
    node->cpc = 0;
    node->cpm = 0;
    if (node->click && node->impression)
	node->ctr = (double)node->click / (double)node->impression;
    if (node->cost && node->click)
	node->cpc = node->cost / (double)node->click;
    if (node->cost && node->impression)
	node->cpm = node->cost / (double)node->impression * (double)1000;

    return ad_system_report_complex_father_add(
	    &node->head,
	    region?region->father:"其他",
	    region?region->child:"其他",
	    impression,
	    click,
	    cost);
}

void ad_system_report_complex_child_free(lg_list_t *head)
{

    lg_list_t *p = head->next;
    ad_system_report_complex_child_t *node = NULL;

    while(p != head) {

	node = (ad_system_report_complex_child_t *)p;
	//fprintf(stdout, "[region_child][%s]\n", node->region_child);

	p = p->next;
	free(node);
    }
}

void ad_system_report_complex_father_free(lg_list_t *head)
{

    lg_list_t *p = head->next;
    ad_system_report_complex_father_t *node = NULL;

    while(p != head) {

	node = (ad_system_report_complex_father_t *)p;
	//fprintf(stdout, "[region_father][%s]\n", node->region_father);

	ad_system_report_complex_child_free(&node->head);
	p = p->next;
	free(node);
    }
}

void ad_system_report_complex_free(lg_list_t *head)
{

    lg_list_t *p = head->next;
    ad_system_report_complex_t *node = NULL;

    while(p != head) {

	node = (ad_system_report_complex_t *)p;
	//fprintf(stdout, "[data][%s]\n", node->date);

	ad_system_report_complex_father_free(&node->head);
	p = p->next;
	free(node);
    }
}

int ad_system_report_complex_create(
	int display_style,
	const char *account_id,
	const char *time_s,
	const char *time_e,
	int time_type,
	int report_type,
	const char *report_path)
{

    lg_list_t cpc_head, cpm_head;
    ad_system_mongo_report_region_load(&cpc_head,
	    display_style,//display_style,
	    2,//cpc
	    account_id,//account_id, 
	    time_s,
	    time_e,
	    time_type);//time_type

    if (display_style == 1)
	ad_system_mongo_report_region_load(&cpm_head,
		display_style,//display_style, 
		1,//cpm
		account_id,//account_id, 
		time_s,
		time_e,
		time_type);//time_type

    if (display_style == 1)
	ad_system_report_xls_file_complex(&cpc_head, &cpm_head, report_type, report_path);
    else
	ad_system_report_xls_file_complex(&cpc_head, NULL, report_type, report_path);

    if (display_style == 1)
	ad_system_report_complex_free(&cpm_head);
    ad_system_report_complex_free(&cpc_head);
    return 0;
}


