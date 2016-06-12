
#include "ad_system_report.h"

/* report standard */

int ad_system_report_standard_creative_add(lg_list_t *head, 
	const char *creative_id,
	const char *creative_name,
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
    strcpy(node->creative_name, creative_name);
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
	const char *group_name, 
	const char *creative_id, 
	const char *creative_name, 
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
		    creative_name,
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
    strcpy(node->group_name, group_name);
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
	    creative_name,
	    impression,
	    click,
	    cost);
}

int ad_system_report_standard_campaign_add(lg_list_t *head,
	const char *campaign_id,
	const char *campaign_name,
	const char *group_id,
	const char *group_name,
	const char *creative_id,
	const char *creative_name,
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
		    group_name,
		    creative_id,
		    creative_name,
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
    strcpy(node->campaign_name, campaign_name);
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
	    group_name,
	    creative_id,
	    creative_name,
	    impression,
	    click,
	    cost);
}

int ad_system_report_standard_date_add(lg_list_t *head,
	const char *date,
	const char *campaign_id,
	const char *campaign_name,
	const char *group_id,
	const char *group_name,
	const char *creative_id,
	const char *creative_name,
	int impression,
	int click,
	double cost)
{

    lg_list_t *p = NULL;
    ad_system_report_standard_date_t *node = NULL;
    lg_list_for_each(p, head) {

	node = (ad_system_report_standard_date_t *)p;
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
		    campaign_name,
		    group_id,
		    group_name,
		    creative_id,
		    creative_name,
		    impression,
		    click,
		    cost);
	}
    }

    if (!(node = malloc(sizeof(ad_system_report_standard_date_t))))
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
	    campaign_name,
	    group_id,
	    group_name,
	    creative_id,
	    creative_name,
	    impression,
	    click,
	    cost);
}

int ad_system_report_standard_account_add(lg_list_t *head,
	const char *account_id,
	const char *account_name,
	const char *date,
	const char *campaign_id,
	const char *campaign_name,
	const char *group_id,
	const char *group_name,
	const char *creative_id,
	const char *creative_name,
	int impression,
	int click,
	double cost)
{

    lg_list_t *p = NULL;
    ad_system_report_standard_account_t *node = NULL;
    lg_list_for_each(p, head) {

	node = (ad_system_report_standard_account_t *)p;
	if (strcmp(node->account_id, account_id) == 0) {

	    node->impression += impression;;
	    node->click += click;
	    node->cost += cost;
	    if (node->click && node->impression)
		node->ctr = (double)node->click / (double)node->impression;
	    if (node->cost && node->click)
		node->cpc = node->cost / (double)node->click;
	    if (node->cost && node->impression)
		node->cpm = node->cost / (double)node->impression * (double)1000;

	    return ad_system_report_standard_date_add(&node->head,
		    date,
		    campaign_id,
		    campaign_name,
		    group_id,
		    group_name,
		    creative_id,
		    creative_name,
		    impression,
		    click,
		    cost);
	}
    }

    if (!(node = malloc(sizeof(ad_system_report_standard_account_t))))
	return -1;

    lg_list_init(&node->head);
    lg_list_add(head, &node->next);
    strcpy(node->account_id, account_id);
    strcpy(node->account_name, account_name);
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

    return ad_system_report_standard_date_add(&node->head,
	    date,
	    campaign_id,
	    campaign_name,
	    group_id,
	    group_name,
	    creative_id,
	    creative_name,
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

void ad_system_report_standard_date_free(lg_list_t *head)
{

    lg_list_t *p = head->next;
    ad_system_report_standard_date_t *node = NULL;

    while(p != head) {

	node = (ad_system_report_standard_date_t *)p;
	//fprintf(stdout, "[data][%s]\n", node->date);

	ad_system_report_standard_campaign_free(&node->head);
	p = p->next;
	free(node);
    }
}

void ad_system_report_standard_account_free(lg_list_t *head)
{

    lg_list_t *p = head->next;
    ad_system_report_standard_account_t *node = NULL;

    while(p != head) {

	node = (ad_system_report_standard_account_t *)p;
	//fprintf(stdout, "[account][%s]\n", node->account);

	ad_system_report_standard_date_free(&node->head);
	p = p->next;
	free(node);
    }
}

/* report complex */

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
	const char *region_father,
	const char *region_child,
	int impression,
	int click,
	double cost)
{

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
		    region_father,
		    region_child,
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
	    region_father,
	    region_child,
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



