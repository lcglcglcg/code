
#ifndef	__bmp_code_h__
#define	__bmp_code_h__

typedef struct {

    int height;
    int width;
    char buffer[768][1024];
    int grey_value; //灰度阀值

    char code_str[32];

} bmp_code_t;

extern int bmp_code_open(bmp_code_t *p, const char *path);
extern int bmp_code_load(bmp_code_t *p, char *buffer);
extern void bmp_code_print(bmp_code_t *p);

#endif


