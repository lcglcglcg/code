
#ifndef __LG_JSON_H__
#define	__LG_JSON_H__

#include <stdio.h>

#ifdef __cplusplus
extern "C" { 
#endif

#define lg_json_t void
#define	GET_JSON_VALUE_STR(o,s) lg_json_t*o=lg_json_get_object(json_obj, s);if(o)p->o=lg_json_value_to_string(o)
#define	GET_JSON_VALUE_NUM(o,s) lg_json_t*o=lg_json_get_object(json_obj, s);if(o)p->o=lg_json_value_to_number(o)

    extern lg_json_t *lg_json_parse(char *buffer);
    extern void lg_json_free(lg_json_t *json_obj);

    extern lg_json_t *lg_json_get_object(lg_json_t *json_obj, const char *str);
    extern const char *lg_json_value_to_string(lg_json_t *json_obj);
    extern int lg_json_value_to_number(lg_json_t *json_obj);

    extern size_t lg_json_sprintf(char *buffer, char *head_str, char *end_str, int count, ...);

#ifdef __cplusplus
}
#endif
#endif//__LG_JSON_H__


