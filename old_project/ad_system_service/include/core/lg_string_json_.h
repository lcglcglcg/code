
#ifndef __LG_STRING_JSON_H__
#define	__LG_STRING_JSON_H__

#ifdef __cplusplus
extern "C" { 
#endif

#include "json.h"

    extern int lg_string_to_json(struct printbuf *pb, const char *str);
    extern char *lg_string_to_json_r(char *dest, const char *str);

#ifdef __cplusplus
}
#endif
#endif//__LG_STRING_JSON_H__

