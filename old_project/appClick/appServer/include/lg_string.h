
#ifndef __LG_STRING_H__
#define	__LG_STRING_H__

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <iconv.h>

#ifdef __cplusplus
extern "C" { 
#endif

    extern int lg_string_url_encode(char *dest, char *src, size_t size);
    extern int lg_string_url_decode(char *dest, char *src, size_t size);
    extern size_t lg_string_utf8_to_gbk(char *in_buff, size_t in_size, char *out_buff, size_t out_size);
    extern size_t lg_string_gbk_to_utf8(char *in_buff, size_t in_size, char *out_buff, size_t out_size);

#ifdef __cplusplus
}
#endif
#endif//__LG_STRING_H__

