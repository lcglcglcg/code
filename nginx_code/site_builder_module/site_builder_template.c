
#include "site_builder_template.h"

#define SITE_BUILDER_TEMPLATE_TITLE			"<?title?>"			//标题
#define SITE_BUILDER_TEMPLATE_KEYWORD			"<?keyword?>"			//关键词
#define SITE_BUILDER_TEMPLATE_CONTENT			"<?content?>"			//文章内容
#define SITE_BUILDER_TEMPLATE_CONTENT_META		"<?content_meta?>"		//文章描述
#define SITE_BUILDER_TEMPLATE_CONTENT_RAND		"<?content_rand?>"		//文章随机
#define SITE_BUILDER_TEMPLATE_META			"<?meta?>"			//描述

#define SITE_BUILDER_TEMPLATE_DATE			"<?date?>"			//2015-01-01
#define SITE_BUILDER_TEMPLATE_TIME			"<?time?>"			//2015-01-01 00:00:00

#define SITE_BUILDER_TEMPLATE_LINK_INTERNAL		"<?link_internal?>"		//内链接
#define SITE_BUILDER_TEMPLATE_LINK_EXTERNAL		"<?link_external?>"		//外链接
#define SITE_BUILDER_TEMPLATE_LINK_SUBDOMAIN		"<?link_subdomain?>"		//域链接
#define SITE_BUILDER_TEMPLATE_LINK_BARE			"<?link_bare?>"			//裸链接

#define SITE_BUILDER_TEMPLATE_LINK_SAVE_INTERNAL	"<?link_save_internal?>"	//内链接(save)
#define SITE_BUILDER_TEMPLATE_LINK_SAVE_EXTERNAL	"<?link_save_external?>"	//外链接(save)
#define SITE_BUILDER_TEMPLATE_LINK_SAVE_SUBDOMAIN	"<?link_save_subdomain?>"	//域链接(save)
#define SITE_BUILDER_TEMPLATE_LINK_SAVE_BARE		"<?link_save_bare?>"		//裸链接(save)

#define SITE_BUILDER_TEMPLATE_LINK_GROUP_ALL		"<?link_group?>"		//组链接全部
#define SITE_BUILDER_TEMPLATE_LINK_GROUP0		"<?link_group0?>"		//组链接
#define SITE_BUILDER_TEMPLATE_LINK_GROUP1		"<?link_group1?>"		//组链接
#define SITE_BUILDER_TEMPLATE_LINK_GROUP2		"<?link_group2?>"		//组链接
#define SITE_BUILDER_TEMPLATE_LINK_GROUP3		"<?link_group3?>"		//组链接
#define SITE_BUILDER_TEMPLATE_LINK_GROUP4		"<?link_group4?>"		//组链接
#define SITE_BUILDER_TEMPLATE_LINK_GROUP5		"<?link_group5?>"		//组链接
#define SITE_BUILDER_TEMPLATE_LINK_GROUP6		"<?link_group6?>"		//组链接
#define SITE_BUILDER_TEMPLATE_LINK_GROUP7		"<?link_group7?>"		//组链接
#define SITE_BUILDER_TEMPLATE_LINK_GROUP8		"<?link_group8?>"		//组链接
#define SITE_BUILDER_TEMPLATE_LINK_GROUP9		"<?link_group9?>"		//组链接

#define SITE_BUILDER_TEMPLATE_LINK_SAVE_GROUP_ALL	"<?link_save_group?>"		//组链接全部(save)
#define SITE_BUILDER_TEMPLATE_LINK_SAVE_GROUP0		"<?link_save_group0?>"		//组链接(save)
#define SITE_BUILDER_TEMPLATE_LINK_SAVE_GROUP1		"<?link_save_group1?>"		//组链接(save)
#define SITE_BUILDER_TEMPLATE_LINK_SAVE_GROUP2		"<?link_save_group2?>"		//组链接(save)
#define SITE_BUILDER_TEMPLATE_LINK_SAVE_GROUP3		"<?link_save_group3?>"		//组链接(save)
#define SITE_BUILDER_TEMPLATE_LINK_SAVE_GROUP4		"<?link_save_group4?>"		//组链接(save)
#define SITE_BUILDER_TEMPLATE_LINK_SAVE_GROUP5		"<?link_save_group5?>"		//组链接(save)
#define SITE_BUILDER_TEMPLATE_LINK_SAVE_GROUP6		"<?link_save_group6?>"		//组链接(save)
#define SITE_BUILDER_TEMPLATE_LINK_SAVE_GROUP7		"<?link_save_group7?>"		//组链接(save)
#define SITE_BUILDER_TEMPLATE_LINK_SAVE_GROUP8		"<?link_save_group8?>"		//组链接(save)
#define SITE_BUILDER_TEMPLATE_LINK_SAVE_GROUP9		"<?link_save_group9?>"		//组链接(save)

#define SITE_BUILDER_TEMPLATE_TITLE_SIZE		9
#define SITE_BUILDER_TEMPLATE_KEYWORD_SIZE		11
#define SITE_BUILDER_TEMPLATE_CONTENT_SIZE		11
#define SITE_BUILDER_TEMPLATE_CONTENT_META_SIZE		16
#define SITE_BUILDER_TEMPLATE_CONTENT_RAND_SIZE		16
#define SITE_BUILDER_TEMPLATE_META_SIZE			8

#define SITE_BUILDER_TEMPLATE_DATE_SIZE			8
#define SITE_BUILDER_TEMPLATE_TIME_SIZE			8

#define SITE_BUILDER_TEMPLATE_LINK_INTERNAL_SIZE	17
#define SITE_BUILDER_TEMPLATE_LINK_EXTERNAL_SIZE	17
#define SITE_BUILDER_TEMPLATE_LINK_SUBDOMAIN_SIZE	18
#define SITE_BUILDER_TEMPLATE_LINK_BARE_SIZE		13

#define SITE_BUILDER_TEMPLATE_LINK_SAVE_INTERNAL_SIZE	22
#define SITE_BUILDER_TEMPLATE_LINK_SAVE_EXTERNAL_SIZE	22
#define SITE_BUILDER_TEMPLATE_LINK_SAVE_SUBDOMAIN_SIZE	23
#define SITE_BUILDER_TEMPLATE_LINK_SAVE_BARE_SIZE	18

#define SITE_BUILDER_TEMPLATE_LINK_GROUP_ALL_SIZE	14
#define SITE_BUILDER_TEMPLATE_LINK_GROUP_SIZE		15

#define SITE_BUILDER_TEMPLATE_LINK_SAVE_GROUP_ALL_SIZE	19
#define SITE_BUILDER_TEMPLATE_LINK_SAVE_GROUP_SIZE	20

void site_builder_template_init(site_builder_template_t *template)
{

    memset(template, 0, sizeof(site_builder_template_t));
    ngx_queue_init(&template->head);
}

int site_builder_template_node_add(ngx_pool_t *pool, ngx_queue_t *head, int type, char *buffer, int size)
{

    if (type == SITE_BUILDER_TEMPLATE_TYPE_HTML && size <= 0) 
	return 0;

    site_builder_template_node_t *node = ngx_pcalloc(pool, sizeof(site_builder_template_node_t));
    if (!node) return -1;

    node->type = type;
    node->buffer = NULL;
    node->size = 0;

    if (type == SITE_BUILDER_TEMPLATE_TYPE_HTML) {

	node->buffer = ngx_pcalloc(pool, size + 1);
	if (!node->buffer) return -1;

	memcpy(node->buffer, buffer, size);
	node->buffer[size] = 0;
	node->size =size;
    }

    ngx_queue_insert_tail(head, &node->next);
    return 0;
}

int __site_builder_template_parse(ngx_pool_t *pool, ngx_queue_t *head, int type, char *buffer, int size)
{

    if (site_builder_template_node_add(pool, head, SITE_BUILDER_TEMPLATE_TYPE_HTML, buffer, size))
	return -1;

    if (site_builder_template_node_add(pool, head, type, NULL, 0))
	return -1;

    return 0;
}

site_builder_template_t *site_builder_template_parse(char *buffer, ngx_pool_t*pool)
{

    site_builder_template_t *template = ngx_pcalloc(pool, sizeof(site_builder_template_t));
    if (!template) return NULL;

    char *str = buffer;
    char *temp = buffer;
    ngx_queue_t *head = &template->head;
    ngx_queue_init(head);

    while (*str) {

	/* TITLE */
	if (strncmp(str, SITE_BUILDER_TEMPLATE_TITLE, SITE_BUILDER_TEMPLATE_TITLE_SIZE) == 0) {

	    if (__site_builder_template_parse(pool, head, SITE_BUILDER_TEMPLATE_TYPE_TITLE, temp, str - temp))
		return NULL;

	    template->title_total++;
	    temp = str = str + SITE_BUILDER_TEMPLATE_TITLE_SIZE;

	    /* KEYWORD */
	} else if (strncmp(str, SITE_BUILDER_TEMPLATE_KEYWORD, SITE_BUILDER_TEMPLATE_KEYWORD_SIZE) == 0) {

	    if (__site_builder_template_parse(pool, head, SITE_BUILDER_TEMPLATE_TYPE_KEYWORD, temp, str - temp))
		return NULL;

	    template->keyword_total++;
	    temp = str = str + SITE_BUILDER_TEMPLATE_KEYWORD_SIZE;

	    /* CONTENT */
	} else if (strncmp(str, SITE_BUILDER_TEMPLATE_CONTENT, SITE_BUILDER_TEMPLATE_CONTENT_SIZE) == 0) {

	    if (__site_builder_template_parse(pool, head, SITE_BUILDER_TEMPLATE_TYPE_CONTENT, temp, str - temp))
		return NULL;

	    template->content_total++;
	    temp = str = str + SITE_BUILDER_TEMPLATE_CONTENT_SIZE;

	    /* CONTENT_META */
	} else if (strncmp(str, SITE_BUILDER_TEMPLATE_CONTENT_META, SITE_BUILDER_TEMPLATE_CONTENT_META_SIZE) == 0) {

	    if (__site_builder_template_parse(pool, head, SITE_BUILDER_TEMPLATE_TYPE_CONTENT_META, temp, str - temp))
		return NULL;

	    template->content_meta_total++;
	    temp = str = str + SITE_BUILDER_TEMPLATE_CONTENT_META_SIZE;

	    /* CONTENT RAND */
	} else if (strncmp(str, SITE_BUILDER_TEMPLATE_CONTENT_RAND, SITE_BUILDER_TEMPLATE_CONTENT_RAND_SIZE) == 0) {

	    if (__site_builder_template_parse(pool, head, SITE_BUILDER_TEMPLATE_TYPE_CONTENT_RAND, temp, str - temp))
		return NULL;

	    template->content_rand_total++;
	    temp = str = str + SITE_BUILDER_TEMPLATE_CONTENT_RAND_SIZE;

	    /* META */
	} else if (strncmp(str, SITE_BUILDER_TEMPLATE_META, SITE_BUILDER_TEMPLATE_META_SIZE) == 0) {

	    if (__site_builder_template_parse(pool, head, SITE_BUILDER_TEMPLATE_TYPE_META, temp, str - temp))
		return NULL;

	    template->meta_total++;
	    temp = str = str + SITE_BUILDER_TEMPLATE_META_SIZE;

	    /* DATE */
	} else if (strncmp(str, SITE_BUILDER_TEMPLATE_DATE, SITE_BUILDER_TEMPLATE_DATE_SIZE) == 0) {

	    if (__site_builder_template_parse(pool, head, SITE_BUILDER_TEMPLATE_TYPE_DATE, temp, str - temp))
		return NULL;

	    template->date_total++;
	    temp = str = str + SITE_BUILDER_TEMPLATE_DATE_SIZE;

	    /* TIME */
	} else if (strncmp(str, SITE_BUILDER_TEMPLATE_TIME, SITE_BUILDER_TEMPLATE_TIME_SIZE) == 0) {

	    if (__site_builder_template_parse(pool, head, SITE_BUILDER_TEMPLATE_TYPE_TIME, temp, str - temp))
		return NULL;

	    template->time_total++;
	    temp = str = str + SITE_BUILDER_TEMPLATE_TIME_SIZE;

	    /* LINK_INTERNAL */
	} else if (strncmp(str, SITE_BUILDER_TEMPLATE_LINK_INTERNAL, SITE_BUILDER_TEMPLATE_LINK_INTERNAL_SIZE) == 0) {

	    if (__site_builder_template_parse(pool, head, SITE_BUILDER_TEMPLATE_TYPE_LINK_INTERNAL, temp, str - temp))
		return NULL;

	    template->link_internal_total++;
	    temp = str = str + SITE_BUILDER_TEMPLATE_LINK_INTERNAL_SIZE;

	    /* LINK_EXTERNAL */
	} else if (strncmp(str, SITE_BUILDER_TEMPLATE_LINK_EXTERNAL, SITE_BUILDER_TEMPLATE_LINK_EXTERNAL_SIZE) == 0) {

	    if (__site_builder_template_parse(pool, head, SITE_BUILDER_TEMPLATE_TYPE_LINK_EXTERNAL, temp, str - temp))
		return NULL;

	    template->link_external_total++;
	    temp = str = str + SITE_BUILDER_TEMPLATE_LINK_EXTERNAL_SIZE;

	    /* LINK_SUBDOMAIN */
	} else if (strncmp(str, SITE_BUILDER_TEMPLATE_LINK_SUBDOMAIN, SITE_BUILDER_TEMPLATE_LINK_SUBDOMAIN_SIZE) == 0) {

	    if (__site_builder_template_parse(pool, head, SITE_BUILDER_TEMPLATE_TYPE_LINK_SUBDOMAIN, temp, str - temp))
		return NULL;

	    template->link_subdomain_total++;
	    temp = str = str + SITE_BUILDER_TEMPLATE_LINK_SUBDOMAIN_SIZE;

	    /* LINK_BARE */
	} else if (strncmp(str, SITE_BUILDER_TEMPLATE_LINK_BARE, SITE_BUILDER_TEMPLATE_LINK_BARE_SIZE) == 0) {

	    if (__site_builder_template_parse(pool, head, SITE_BUILDER_TEMPLATE_TYPE_LINK_BARE, temp, str - temp))
		return NULL;

	    template->link_bare_total++;
	    temp = str = str + SITE_BUILDER_TEMPLATE_LINK_BARE_SIZE;

	    /* LINK_SAVE_INTERNAL */
	} else if (strncmp(str, SITE_BUILDER_TEMPLATE_LINK_SAVE_INTERNAL, SITE_BUILDER_TEMPLATE_LINK_SAVE_INTERNAL_SIZE) == 0) {

	    if (__site_builder_template_parse(pool, head, SITE_BUILDER_TEMPLATE_TYPE_LINK_SAVE_INTERNAL, temp, str - temp))
		return NULL;

	    template->link_save_internal_total++;
	    temp = str = str + SITE_BUILDER_TEMPLATE_LINK_SAVE_INTERNAL_SIZE;

	    /* LINK_SAVE_EXTERNAL */
	} else if (strncmp(str, SITE_BUILDER_TEMPLATE_LINK_SAVE_EXTERNAL, SITE_BUILDER_TEMPLATE_LINK_SAVE_EXTERNAL_SIZE) == 0) {

	    if (__site_builder_template_parse(pool, head, SITE_BUILDER_TEMPLATE_TYPE_LINK_SAVE_EXTERNAL, temp, str - temp))
		return NULL;

	    template->link_save_external_total++;
	    temp = str = str + SITE_BUILDER_TEMPLATE_LINK_SAVE_EXTERNAL_SIZE;

	    /* LINK_SAVE_SUBDOMAIN */
	} else if (strncmp(str, SITE_BUILDER_TEMPLATE_LINK_SAVE_SUBDOMAIN, SITE_BUILDER_TEMPLATE_LINK_SAVE_SUBDOMAIN_SIZE) == 0) {

	    if (__site_builder_template_parse(pool, head, SITE_BUILDER_TEMPLATE_TYPE_LINK_SAVE_SUBDOMAIN, temp, str - temp))
		return NULL;

	    template->link_save_subdomain_total++;
	    temp = str = str + SITE_BUILDER_TEMPLATE_LINK_SAVE_SUBDOMAIN_SIZE;

	    /* LINK_SAVE_BARE */
	} else if (strncmp(str, SITE_BUILDER_TEMPLATE_LINK_SAVE_BARE, SITE_BUILDER_TEMPLATE_LINK_SAVE_BARE_SIZE) == 0) {

	    if (__site_builder_template_parse(pool, head, SITE_BUILDER_TEMPLATE_TYPE_LINK_SAVE_BARE, temp, str - temp))
		return NULL;

	    template->link_save_bare_total++;
	    temp = str = str + SITE_BUILDER_TEMPLATE_LINK_SAVE_BARE_SIZE;

	    /* LINK_GROUP ALL */
	} else if (strncmp(str, SITE_BUILDER_TEMPLATE_LINK_GROUP_ALL, SITE_BUILDER_TEMPLATE_LINK_GROUP_ALL_SIZE) == 0) {

	    if (__site_builder_template_parse(pool, head, SITE_BUILDER_TEMPLATE_TYPE_LINK_GROUP_ALL, temp, str - temp))
		return NULL;

	    template->link_group_all_total++;
	    temp = str = str + SITE_BUILDER_TEMPLATE_LINK_GROUP_ALL_SIZE;

	    /* LINK_SAVE_GROUP ALL */
	} else if (strncmp(str, SITE_BUILDER_TEMPLATE_LINK_SAVE_GROUP_ALL, SITE_BUILDER_TEMPLATE_LINK_SAVE_GROUP_ALL_SIZE) == 0) {

	    if (__site_builder_template_parse(pool, head, SITE_BUILDER_TEMPLATE_TYPE_LINK_SAVE_GROUP_ALL, temp, str - temp))
		return NULL;

	    template->link_save_group_all_total++;
	    temp = str = str + SITE_BUILDER_TEMPLATE_LINK_SAVE_GROUP_ALL_SIZE;

	    /* LINK_GROUP0 */
	} else if (strncmp(str, SITE_BUILDER_TEMPLATE_LINK_GROUP0, SITE_BUILDER_TEMPLATE_LINK_GROUP_SIZE) == 0) {

	    if (__site_builder_template_parse(pool, head, SITE_BUILDER_TEMPLATE_TYPE_LINK_GROUP0, temp, str - temp))
		return NULL;

	    template->link_group_total[0]++;
	    temp = str = str + SITE_BUILDER_TEMPLATE_LINK_GROUP_SIZE;

	    /* LINK_GROUP1 */
	} else if (strncmp(str, SITE_BUILDER_TEMPLATE_LINK_GROUP1, SITE_BUILDER_TEMPLATE_LINK_GROUP_SIZE) == 0) {

	    if (__site_builder_template_parse(pool, head, SITE_BUILDER_TEMPLATE_TYPE_LINK_GROUP1, temp, str - temp))
		return NULL;

	    template->link_group_total[1]++;
	    temp = str = str + SITE_BUILDER_TEMPLATE_LINK_GROUP_SIZE;

	    /* LINK_GROUP2 */
	} else if (strncmp(str, SITE_BUILDER_TEMPLATE_LINK_GROUP2, SITE_BUILDER_TEMPLATE_LINK_GROUP_SIZE) == 0) {

	    if (__site_builder_template_parse(pool, head, SITE_BUILDER_TEMPLATE_TYPE_LINK_GROUP2, temp, str - temp))
		return NULL;

	    template->link_group_total[2]++;
	    temp = str = str + SITE_BUILDER_TEMPLATE_LINK_GROUP_SIZE;

	    /* LINK_GROUP3 */
	} else if (strncmp(str, SITE_BUILDER_TEMPLATE_LINK_GROUP3, SITE_BUILDER_TEMPLATE_LINK_GROUP_SIZE) == 0) {

	    if (__site_builder_template_parse(pool, head, SITE_BUILDER_TEMPLATE_TYPE_LINK_GROUP3, temp, str - temp))
		return NULL;

	    template->link_group_total[3]++;
	    temp = str = str + SITE_BUILDER_TEMPLATE_LINK_GROUP_SIZE;

	    /* LINK_GROUP4 */
	} else if (strncmp(str, SITE_BUILDER_TEMPLATE_LINK_GROUP4, SITE_BUILDER_TEMPLATE_LINK_GROUP_SIZE) == 0) {

	    if (__site_builder_template_parse(pool, head, SITE_BUILDER_TEMPLATE_TYPE_LINK_GROUP4, temp, str - temp))
		return NULL;

	    template->link_group_total[4]++;
	    temp = str = str + SITE_BUILDER_TEMPLATE_LINK_GROUP_SIZE;

	    /* LINK_GROUP5 */
	} else if (strncmp(str, SITE_BUILDER_TEMPLATE_LINK_GROUP5, SITE_BUILDER_TEMPLATE_LINK_GROUP_SIZE) == 0) {

	    if (__site_builder_template_parse(pool, head, SITE_BUILDER_TEMPLATE_TYPE_LINK_GROUP5, temp, str - temp))
		return NULL;

	    template->link_group_total[5]++;
	    temp = str = str + SITE_BUILDER_TEMPLATE_LINK_GROUP_SIZE;

	    /* LINK_GROUP6 */
	} else if (strncmp(str, SITE_BUILDER_TEMPLATE_LINK_GROUP6, SITE_BUILDER_TEMPLATE_LINK_GROUP_SIZE) == 0) {

	    if (__site_builder_template_parse(pool, head, SITE_BUILDER_TEMPLATE_TYPE_LINK_GROUP6, temp, str - temp))
		return NULL;

	    template->link_group_total[6]++;
	    temp = str = str + SITE_BUILDER_TEMPLATE_LINK_GROUP_SIZE;

	    /* LINK_GROUP7 */
	} else if (strncmp(str, SITE_BUILDER_TEMPLATE_LINK_GROUP7, SITE_BUILDER_TEMPLATE_LINK_GROUP_SIZE) == 0) {

	    if (__site_builder_template_parse(pool, head, SITE_BUILDER_TEMPLATE_TYPE_LINK_GROUP7, temp, str - temp))
		return NULL;

	    template->link_group_total[7]++;
	    temp = str = str + SITE_BUILDER_TEMPLATE_LINK_GROUP_SIZE;

	    /* LINK_GROUP8 */
	} else if (strncmp(str, SITE_BUILDER_TEMPLATE_LINK_GROUP8, SITE_BUILDER_TEMPLATE_LINK_GROUP_SIZE) == 0) {

	    if (__site_builder_template_parse(pool, head, SITE_BUILDER_TEMPLATE_TYPE_LINK_GROUP8, temp, str - temp))
		return NULL;

	    template->link_group_total[8]++;
	    temp = str = str + SITE_BUILDER_TEMPLATE_LINK_GROUP_SIZE;

	    /* LINK_GROUP9 */
	} else if (strncmp(str, SITE_BUILDER_TEMPLATE_LINK_GROUP9, SITE_BUILDER_TEMPLATE_LINK_GROUP_SIZE) == 0) {

	    if (__site_builder_template_parse(pool, head, SITE_BUILDER_TEMPLATE_TYPE_LINK_GROUP9, temp, str - temp))
		return NULL;

	    template->link_group_total[9]++;
	    temp = str = str + SITE_BUILDER_TEMPLATE_LINK_GROUP_SIZE;

	    /* LINK_SAVE_GROUP0 */
	} else if (strncmp(str, SITE_BUILDER_TEMPLATE_LINK_SAVE_GROUP0, SITE_BUILDER_TEMPLATE_LINK_SAVE_GROUP_SIZE) == 0) {

	    if (__site_builder_template_parse(pool, head, SITE_BUILDER_TEMPLATE_TYPE_LINK_SAVE_GROUP0, temp, str - temp))
		return NULL;

	    template->link_save_group_total[0]++;
	    temp = str = str + SITE_BUILDER_TEMPLATE_LINK_SAVE_GROUP_SIZE;

	    /* LINK_SAVE_GROUP1 */
	} else if (strncmp(str, SITE_BUILDER_TEMPLATE_LINK_SAVE_GROUP1, SITE_BUILDER_TEMPLATE_LINK_SAVE_GROUP_SIZE) == 0) {

	    if (__site_builder_template_parse(pool, head, SITE_BUILDER_TEMPLATE_TYPE_LINK_SAVE_GROUP1, temp, str - temp))
		return NULL;

	    template->link_save_group_total[1]++;
	    temp = str = str + SITE_BUILDER_TEMPLATE_LINK_SAVE_GROUP_SIZE;

	    /* LINK_SAVE_GROUP2 */
	} else if (strncmp(str, SITE_BUILDER_TEMPLATE_LINK_SAVE_GROUP2, SITE_BUILDER_TEMPLATE_LINK_SAVE_GROUP_SIZE) == 0) {

	    if (__site_builder_template_parse(pool, head, SITE_BUILDER_TEMPLATE_TYPE_LINK_SAVE_GROUP2, temp, str - temp))
		return NULL;

	    template->link_save_group_total[2]++;
	    temp = str = str + SITE_BUILDER_TEMPLATE_LINK_SAVE_GROUP_SIZE;

	    /* LINK_SAVE_GROUP3 */
	} else if (strncmp(str, SITE_BUILDER_TEMPLATE_LINK_SAVE_GROUP3, SITE_BUILDER_TEMPLATE_LINK_SAVE_GROUP_SIZE) == 0) {

	    if (__site_builder_template_parse(pool, head, SITE_BUILDER_TEMPLATE_TYPE_LINK_SAVE_GROUP3, temp, str - temp))
		return NULL;

	    template->link_save_group_total[3]++;
	    temp = str = str + SITE_BUILDER_TEMPLATE_LINK_SAVE_GROUP_SIZE;

	    /* LINK_SAVE_GROUP4 */
	} else if (strncmp(str, SITE_BUILDER_TEMPLATE_LINK_SAVE_GROUP4, SITE_BUILDER_TEMPLATE_LINK_SAVE_GROUP_SIZE) == 0) {

	    if (__site_builder_template_parse(pool, head, SITE_BUILDER_TEMPLATE_TYPE_LINK_SAVE_GROUP4, temp, str - temp))
		return NULL;

	    template->link_save_group_total[4]++;
	    temp = str = str + SITE_BUILDER_TEMPLATE_LINK_SAVE_GROUP_SIZE;

	    /* LINK_SAVE_GROUP5 */
	} else if (strncmp(str, SITE_BUILDER_TEMPLATE_LINK_SAVE_GROUP5, SITE_BUILDER_TEMPLATE_LINK_SAVE_GROUP_SIZE) == 0) {

	    if (__site_builder_template_parse(pool, head, SITE_BUILDER_TEMPLATE_TYPE_LINK_SAVE_GROUP5, temp, str - temp))
		return NULL;

	    template->link_save_group_total[5]++;
	    temp = str = str + SITE_BUILDER_TEMPLATE_LINK_SAVE_GROUP_SIZE;

	    /* LINK_SAVE_GROUP6 */
	} else if (strncmp(str, SITE_BUILDER_TEMPLATE_LINK_SAVE_GROUP6, SITE_BUILDER_TEMPLATE_LINK_SAVE_GROUP_SIZE) == 0) {

	    if (__site_builder_template_parse(pool, head, SITE_BUILDER_TEMPLATE_TYPE_LINK_SAVE_GROUP6, temp, str - temp))
		return NULL;

	    template->link_save_group_total[6]++;
	    temp = str = str + SITE_BUILDER_TEMPLATE_LINK_SAVE_GROUP_SIZE;

	    /* LINK_SAVE_GROUP7 */
	} else if (strncmp(str, SITE_BUILDER_TEMPLATE_LINK_SAVE_GROUP7, SITE_BUILDER_TEMPLATE_LINK_SAVE_GROUP_SIZE) == 0) {

	    if (__site_builder_template_parse(pool, head, SITE_BUILDER_TEMPLATE_TYPE_LINK_SAVE_GROUP7, temp, str - temp))
		return NULL;

	    template->link_save_group_total[7]++;
	    temp = str = str + SITE_BUILDER_TEMPLATE_LINK_SAVE_GROUP_SIZE;

	    /* LINK_SAVE_GROUP8 */
	} else if (strncmp(str, SITE_BUILDER_TEMPLATE_LINK_SAVE_GROUP8, SITE_BUILDER_TEMPLATE_LINK_SAVE_GROUP_SIZE) == 0) {

	    if (__site_builder_template_parse(pool, head, SITE_BUILDER_TEMPLATE_TYPE_LINK_SAVE_GROUP8, temp, str - temp))
		return NULL;

	    template->link_save_group_total[8]++;
	    temp = str = str + SITE_BUILDER_TEMPLATE_LINK_SAVE_GROUP_SIZE;

	    /* LINK_SAVE_GROUP9 */
	} else if (strncmp(str, SITE_BUILDER_TEMPLATE_LINK_SAVE_GROUP9, SITE_BUILDER_TEMPLATE_LINK_SAVE_GROUP_SIZE) == 0) {

	    if (__site_builder_template_parse(pool, head, SITE_BUILDER_TEMPLATE_TYPE_LINK_SAVE_GROUP9, temp, str - temp))
		return NULL;

	    template->link_save_group_total[9]++;
	    temp = str = str + SITE_BUILDER_TEMPLATE_LINK_SAVE_GROUP_SIZE;

	} else {

	    str++;
	}
    }

    if (site_builder_template_node_add(pool, head, SITE_BUILDER_TEMPLATE_TYPE_HTML, temp, str - temp))
	return NULL;

    return template;
}

void site_builder_template_print(site_builder_template_t *template)
{

    ngx_queue_t *p = NULL;
    ngx_queue_t *head = &template->head;
    for (p = ngx_queue_head(head); p != ngx_queue_sentinel(head); p = ngx_queue_next(p)) {

	site_builder_template_node_t *node = (site_builder_template_node_t *)p;
	switch(node->type) {

	    case SITE_BUILDER_TEMPLATE_TYPE_HTML:
		fprintf(stdout, "%s", node->buffer);
		break;

	    case SITE_BUILDER_TEMPLATE_TYPE_TITLE:
		fprintf(stdout, "[TITLE]");
		break;

	    case SITE_BUILDER_TEMPLATE_TYPE_KEYWORD:
		fprintf(stdout, "[KEYWORD]");
		break;

	    case SITE_BUILDER_TEMPLATE_TYPE_CONTENT:
	    case SITE_BUILDER_TEMPLATE_TYPE_CONTENT_META:
		fprintf(stdout, "[CONTENT]");
		break;

	    case SITE_BUILDER_TEMPLATE_TYPE_META:
		fprintf(stdout, "[META]");
		break;

	    case SITE_BUILDER_TEMPLATE_TYPE_LINK_INTERNAL:
	    case SITE_BUILDER_TEMPLATE_TYPE_LINK_SAVE_INTERNAL:
		fprintf(stdout, "[LINK_INTERNAL]");
		break;

	    case SITE_BUILDER_TEMPLATE_TYPE_LINK_EXTERNAL:
	    case SITE_BUILDER_TEMPLATE_TYPE_LINK_SAVE_EXTERNAL:
		fprintf(stdout, "[LINK_EXTERNAL]");
		break;

	    case SITE_BUILDER_TEMPLATE_TYPE_LINK_SUBDOMAIN:
	    case SITE_BUILDER_TEMPLATE_TYPE_LINK_SAVE_SUBDOMAIN:
		fprintf(stdout, "[LINK_SUBDOMAIN]");
		break;

	    case SITE_BUILDER_TEMPLATE_TYPE_LINK_BARE:
	    case SITE_BUILDER_TEMPLATE_TYPE_LINK_SAVE_BARE:
		fprintf(stdout, "[LINK_BARE]");
		break;

	    case SITE_BUILDER_TEMPLATE_TYPE_LINK_GROUP1:
	    case SITE_BUILDER_TEMPLATE_TYPE_LINK_GROUP2:
	    case SITE_BUILDER_TEMPLATE_TYPE_LINK_GROUP3:
	    case SITE_BUILDER_TEMPLATE_TYPE_LINK_GROUP4:
	    case SITE_BUILDER_TEMPLATE_TYPE_LINK_GROUP5:
	    case SITE_BUILDER_TEMPLATE_TYPE_LINK_GROUP6:
	    case SITE_BUILDER_TEMPLATE_TYPE_LINK_GROUP7:
	    case SITE_BUILDER_TEMPLATE_TYPE_LINK_GROUP8:
	    case SITE_BUILDER_TEMPLATE_TYPE_LINK_GROUP9:
	    case SITE_BUILDER_TEMPLATE_TYPE_LINK_SAVE_GROUP1:
	    case SITE_BUILDER_TEMPLATE_TYPE_LINK_SAVE_GROUP2:
	    case SITE_BUILDER_TEMPLATE_TYPE_LINK_SAVE_GROUP3:
	    case SITE_BUILDER_TEMPLATE_TYPE_LINK_SAVE_GROUP4:
	    case SITE_BUILDER_TEMPLATE_TYPE_LINK_SAVE_GROUP5:
	    case SITE_BUILDER_TEMPLATE_TYPE_LINK_SAVE_GROUP6:
	    case SITE_BUILDER_TEMPLATE_TYPE_LINK_SAVE_GROUP7:
	    case SITE_BUILDER_TEMPLATE_TYPE_LINK_SAVE_GROUP8:
	    case SITE_BUILDER_TEMPLATE_TYPE_LINK_SAVE_GROUP9:
		fprintf(stdout, "[LINK_GROUP]");
		break;
	}
    }

    fprintf(stdout, "================================\n");
}

int site_builder_template_load(
	site_builder_template_list_t *template_list,
	char *template_name,
	char *buffer, 
	ngx_pool_t *pool)
{

    site_builder_template_t *template = site_builder_template_parse(buffer, pool);
    if (!template) return -1;

    template->template_id = template_list->template_total++;

    int template_name_len = strlen(template_name);
    template->template_name = ngx_pcalloc(pool, template_name_len + 1);
    memcpy(template->template_name, template_name, template_name_len);
    template->template_name[template_name_len] = 0;

    ngx_queue_insert_tail(&template_list->head, &template->next);

    // site_builder_template_print(template);
    return 0;
}

int _site_builder_template_dir_load(
	char *path,
	int path_size,
	site_builder_template_list_t *template_list,
	site_builder_element_head_tree_t *affiliated_file_list,
	ngx_pool_t *pool)
{

    DIR *dir = NULL;
    struct dirent *dir_info = NULL;
    char _path[SITE_BUILDER_BUF_SIZE];
    char *key = &_path[path_size];

    if (!(dir = opendir(path)))
	return -1;

    while((dir_info = readdir(dir))) {

	if((strcmp(dir_info->d_name,".") == 0))
	    continue;

	if ((strcmp(dir_info->d_name,"..")==0))
	    continue;

	int size = sprintf(_path, "%s/%s", path, dir_info->d_name);
	if (dir_info->d_type == 4) { //dir

	    _site_builder_template_dir_load(_path, path_size, template_list, affiliated_file_list, pool);

	} else if (dir_info->d_type == 8) { //file

	    char *buffer = NULL;
	    int buffer_size = site_builder_element_file_to_buffer(_path, &buffer, pool);
	    if (buffer_size >= 0 && buffer) {

		if (strstr(key, "baidu_verify_")) {

		    site_builder_element_key_add(affiliated_file_list,
			    key, strlen(key),
			    buffer, buffer_size);

		} else if (strncmp(&_path[size - 5], ".html", 5) == 0) {

		    site_builder_template_load(template_list, dir_info->d_name, buffer, pool);

		} else {

		    site_builder_element_key_add(affiliated_file_list,
			    key, strlen(key),
			    buffer, buffer_size);
		}
	    }
	}
    }

    closedir(dir);
    return 0;
}

int site_builder_template_dir_load(
	char *path,
	int path_size,
	site_builder_template_list_t *template_list,
	site_builder_element_head_tree_t *affiliated_file_list,
	ngx_pool_t *pool)
{

    ngx_queue_init(&template_list->head);
    template_list->affiliated_file_list = affiliated_file_list;
    template_list->template_total = 0;

    site_builder_element_init(affiliated_file_list, pool);
    _site_builder_template_dir_load(path, path_size, template_list, affiliated_file_list, pool);
    return 0;
}




