
#ifndef	__parse_code_h__
#define	__parse_code_h__

#include "lg_list.h"
#include "font_code.h"

typedef struct {

    lg_list_t next;

    char ch;
    int x, y;
    int pixel_count;	//像素数量
    double pixel_pct;	//像素百分比

    font_code_t *font;

} parse_code_t;

extern void parse_code_run(lg_list_t *font_head, bmp_code_t*bmp, parse_code_t *parse);
extern void parse_code_sort(parse_code_t *parse);

#endif


