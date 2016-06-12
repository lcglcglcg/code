
#ifndef __MANAGER_ACCOUNT_REDIS_H__
#define __MANAGER_ACCOUNT_REDIS_H__

#include <stdio.h>
#include "lg_oci.h"
#include "lg_config_file.h"

#ifdef __cplusplus
extern "C" { 
#endif

    extern int manager_account_sql_init(lg_list_t *config_file_head);
    extern int manager_account_sql_login(const char *account_name, const char *password, size_t *account_id);

    // 获取用户信息
    extern int manager_account_sql_get_account_info(lg_oci_t *p, size_t account_id);

    // 绑定网盟
    extern int manager_account_sql_bind_wangmeng(size_t account_id, size_t wangmeng_id);
    extern int manager_account_sql_get_wangmeng_account(size_t account_id, size_t *wangmeng_id);

    // 绑定软文
    extern int manager_account_sql_bind_ruanwen(size_t account_id, size_t ruanwen_id);
    extern int manager_account_sql_get_ruanwen_account(size_t account_id, size_t *ruanwen_id);


#ifdef __cplusplus
}
#endif

#endif




