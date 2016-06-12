
#include <stdlib.h>
#include "ad_system_report_account_info.h"

void ad_system_report_account_info_init(ad_system_report_account_info_root_t *root)
{

    root->head = RB_ROOT;
    lg_list_init(&root->list_head);
}

char *ad_system_report_account_info_malloc(char *str)
{

    if (str) {

	char *buffer = NULL;
	if (!(buffer = malloc(strlen(str) + 1)))
	    return NULL;

	strcpy(buffer, str);
	return buffer;
    }

    return NULL;
}

int ad_system_report_account_info_add(ad_system_report_account_info_root_t *root, ad_system_report_account_info_t *_info)
{

    lg_rbtree_node *parent = NULL;
    lg_rbtree_node **node = &root->head.rb_node;
    ad_system_report_account_info_t *temp = NULL;

    while (*node) {

	parent = *node;
	temp = (ad_system_report_account_info_t *)parent;
	int retval = strcmp(_info->creative_id, temp->creative_id);

	if (retval < 0)
	    node = &(*node)->rb_left;
	else if (retval > 0)
	    node = &(*node)->rb_right;
	else
	    return -1;
    }

    ad_system_report_account_info_t *info = NULL;
    if (!(info = malloc(sizeof(ad_system_report_account_info_t))))
	return -1;

    info->account_id=ad_system_report_account_info_malloc(_info->account_id);
    info->account_name=ad_system_report_account_info_malloc(_info->account_name);
    info->campaign_id=ad_system_report_account_info_malloc(_info->campaign_id);
    info->campaign_name=ad_system_report_account_info_malloc(_info->campaign_name);
    info->group_id=ad_system_report_account_info_malloc(_info->group_id);
    info->group_name=ad_system_report_account_info_malloc(_info->group_name);
    info->creative_id=ad_system_report_account_info_malloc(_info->creative_id);
    info->creative_name=ad_system_report_account_info_malloc(_info->creative_name);

    lg_rbtree_link_node(&info->tree, parent, node);
    lg_rbtree_insert_color(&info->tree, &root->head);
    lg_list_add(&root->list_head, &info->list);
    return 0;
}

ad_system_report_account_info_t *ad_system_report_account_info_find(ad_system_report_account_info_root_t *root, char *creative_id)
{

    lg_rbtree_node *node = root->head.rb_node;
    ad_system_report_account_info_t *info = NULL;

    while (node) {

	info = (ad_system_report_account_info_t *)node;
	int retval = strcmp(creative_id, info->creative_id);

	if (retval < 0)
	    node = node->rb_left;
	else if (retval > 0)
	    node = node->rb_right;
	else
	    return info;
    }

    return NULL;
}

void ad_system_report_account_info_free(ad_system_report_account_info_root_t *root)
{

    lg_list_t *p = root->list_head.next;
    ad_system_report_account_info_t *info = NULL;

    while(p != &root->list_head) {

	info = rb_entry(p, ad_system_report_account_info_t, list);
	p = p->next;

	free(info->account_name);
	free(info->campaign_id);
	free(info->campaign_name);
	free(info->group_id);
	free(info->group_name);
	free(info->creative_id);
	free(info->creative_name);
	free(info);
    }

    ad_system_report_account_info_init(root);
}

void ad_system_report_account_info_print(ad_system_report_account_info_root_t *root)
{

    lg_list_t *p = NULL;
    ad_system_report_account_info_t *info = NULL;
    lg_list_for_each(p, &root->list_head) {

	info = rb_entry(p, ad_system_report_account_info_t, list);
	fprintf(stdout, "[%s][%s][%s][%s][%s][%s][%s][%s]\n",
		info->account_id,
		info->account_name,
		info->campaign_id,
		info->campaign_name,
		info->group_id,
		info->group_name,
		info->creative_id,
		info->creative_name);
    }

    fprintf(stdout, "\n");
}

