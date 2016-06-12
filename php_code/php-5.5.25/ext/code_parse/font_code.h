
#ifndef	__font_code_h__
#define	__font_code_h__

#include "lg_list.h"

typedef struct {

    lg_list_t next;
    char ch;
    int height;
    int width;
    double pixel_max;
    char buffer[13][10];

} font_code_t;

extern void font_code_init(lg_list_t *head);
extern void font_code_print(lg_list_t *head);

#endif


