
#ifndef __LG_STRING_H__
#define	__LG_STRING_H__

#include <time.h>

#ifdef __cplusplus
extern "C" { 
#endif

    extern char *strptime(const char *s, const char *format, struct tm *tm);
    extern char *lg_string_strtok(const char *src, char *dest, char **save_ptr, int ch);
    extern char *lg_string_strstr(const char *haystack, const char *needle);
    extern char *lg_string_strspn(char *buffer, char ch);
    extern char *lg_string_cutting(char *buffer, char ch);
    extern void lg_string_remove_keyword(char *buffer, char *keyword, int keyword_size);
    extern int lg_string_url_encode(char *dest, char *src, size_t size);
    extern int lg_string_url_decode(char *dest, char *src, size_t size);

#ifdef __cplusplus
}
#endif
#endif//__LG_STRING_H__

