
#include <string.h>
#include "lg_json.h"
#include "manager_account_json.h"

int manager_account_json_parse(lg_json_t *json_obj, manager_account_json_t *p)
{

    memset(p, 0, sizeof(manager_account_json_t));
    GET_JSON_VALUE_STR(PACKET_TYPE, "PACKET_TYPE");
    if (!p->PACKET_TYPE) return -1;

    GET_JSON_VALUE_STR(ACCOUNT_NAME, "ACCOUNT_NAME");
    GET_JSON_VALUE_STR(PASSWORD, "PASSWORD");
    GET_JSON_VALUE_STR(VAR_CODE, "VAR_CODE");
    GET_JSON_VALUE_STR(PASS_OLD, "PASS_OLD");
    GET_JSON_VALUE_STR(PASS_NEW, "PASS_NEW");


    return 0;
}


