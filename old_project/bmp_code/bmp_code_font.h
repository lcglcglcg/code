
#ifndef	__bmp_code_font_h__
#define	__bmp_code_font_h__

#include <stdio.h>
#include <string.h>

typedef struct BMP_CODE_FONT bmp_code_font;
struct BMP_CODE_FONT {

    int id;
    int height;
    int width;
    int **buffer;

    int max;
    int count;
    int good;
    int x, y;

    bmp_code_font *next;
};

extern bmp_code_font *bmp_code_font_init();
extern void bmp_code_font_free(bmp_code_font *head);

#endif


