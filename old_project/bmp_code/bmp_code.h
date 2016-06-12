#ifndef BMP_CODE_H
#define BMP_CODE_H

#include "bmp_code_font.h"
#include "bmp_code_load.h"
#include "bmp_code_analysis.h"

typedef struct {

    char id;
    int percentage;
    int x;
    int y;

} bmp_code_t;

class bmp_code
{

public:
    bmp_code();
    ~bmp_code();

    bmp_code_font *head;
    bmp_code_load bmp;
    bmp_code_analysis analysis;

    void bmp_code_run(bmp_code_load *bmp);
    bmp_code_t *bmp_code_from_buff(char *buffer);


    char id;
    int percentage;
    int x;
    int y;

    bmp_code_t code[5];
};


#endif // BMP_CODE_H
