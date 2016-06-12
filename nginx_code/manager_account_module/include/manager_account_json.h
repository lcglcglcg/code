
#ifndef __MANAGER_ACCOUNT_JSON_H__
#define __MANAGER_ACCOUNT_JSON_H__

#include "lg_json.h"

#ifdef __cplusplus
extern "C" { 
#endif

    typedef struct {

	const char *PACKET_TYPE;
	const char *ACCOUNT_NAME;
	const char *PASSWORD;
	const char *VAR_CODE;
	const char *PASS_OLD;
	const char *PASS_NEW;

    } manager_account_json_t;

    extern int manager_account_json_parse(lg_json_t *json, manager_account_json_t *p);

#ifdef __cplusplus
}
#endif

#endif




