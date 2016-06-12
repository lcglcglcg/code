
#ifndef __MANAGER_ACCOUNT_REDIS_DB_H__
#define __MANAGER_ACCOUNT_REDIS_DB_H__

#include <stdio.h>
#include "lg_config_file.h"

#define REDIS_DB_SESSION		NULL
#define REDIS_DB_VAR_CODE		"1"
#define REDIS_DB_WANGMENG_SESSION	"2"
#define REDIS_DB_RUANWEN_SESSION	"3"

#define REDIS_SESSION_TIME_OUT		1800
#define REDIS_VAR_CODE_TIME_OUT		30

#ifdef __cplusplus
extern "C" { 
#endif

    extern int manager_account_redis_init(lg_list_t *config_file_head);

    extern int manager_account_redis_get_session(const char *session_id, size_t *account_id);
    extern int manager_account_redis_set_session(const char *session_id, size_t account_id);
    extern int manager_account_redis_del_session(const char *session_id);

    extern int manager_account_redis_get_var_code(const char *session_id, char *var_code);
    extern int manager_account_redis_set_var_code(const char *session_id, const char *var_code);

    extern int manager_account_redis_get_wangmeng_session(const char *session_id, char *wangmeng_session);
    extern int manager_account_redis_set_wangmeng_session(const char *session_id, const char *wangmeng_session);

    extern int manager_account_redis_set_ruanwen_session(const char *session_id, const char *ruanwen_session);
    extern int manager_account_redis_get_ruanwen_session(const char *session_id, char *ruanwen_session);

#ifdef __cplusplus
}
#endif

#endif




