
#ifndef	__AD_SYSTEM_REPORT_ACCOUNT_INFO_H__
#define	__AD_SYSTEM_REPORT_ACCOUNT_INFO_H__

#include "lg_list.h"
#include "lg_rbtree.h"

#ifdef __cplusplus
extern "C" { 
#endif

    typedef struct {

	lg_rbtree_node tree;
	lg_list_t list;

	char *account_id;
	char *account_name;

	char *campaign_id;
	char *campaign_name;

	char *group_id;
	char *group_name;

	char *creative_id;
	char *creative_name;

    } ad_system_report_account_info_t;

    typedef struct {

	lg_rbtree_head head;
	lg_list_t list_head;

    } ad_system_report_account_info_root_t;

    extern void ad_system_report_account_info_init(ad_system_report_account_info_root_t *root);
    extern int ad_system_report_account_info_add(
	    ad_system_report_account_info_root_t *root, ad_system_report_account_info_t *info);
    extern ad_system_report_account_info_t *ad_system_report_account_info_find(
	    ad_system_report_account_info_root_t *root, char *creative_id);
    extern void ad_system_report_account_info_free(ad_system_report_account_info_root_t *root);
    extern void ad_system_report_account_info_print(ad_system_report_account_info_root_t *root);
    extern int ad_system_get_report_account_info(ad_system_report_account_info_root_t *head, int mode);

#ifdef __cplusplus
}
#endif

#endif


