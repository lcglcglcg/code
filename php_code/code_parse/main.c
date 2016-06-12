
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bmp_code.h"
#include "font_code.h"
#include "parse_code.h"
#include "gd.h"

int main(int argc, char **argv)
{

    // arg
    char *path = argv[1];
    if (argc < 2) path = "img/3117.jpg";

    // load img
    FILE *fp = fopen(path, "rb");
    gdImagePtr im = gdImageCreateFromJpeg(fp);

    int size = 0;
    char *buffer = gdImageBmpPtr(im, &size, 0);

    // bmp
    bmp_code_t bmp;
    bmp.grey_value = 230;
    bmp_code_load(&bmp, buffer);
    // bmp_code_print(&bmp);

    // font
    lg_list_t font_head;
    font_code_init(&font_head);
    // font_code_print(&font_head);

    // parse
    parse_code_t parse[5];
    parse_code_run(&font_head, &bmp, &parse[0]);
    parse_code_run(&font_head, &bmp, &parse[1]);
    parse_code_run(&font_head, &bmp, &parse[2]);
    parse_code_run(&font_head, &bmp, &parse[3]);
    parse_code_sort(parse);

    // display
    int i;
    for (i = 0; i < 4; i++) {

	fprintf(stdout, "[%c][%0.2f]\n", parse[i].ch, parse[i].pixel_pct);
    }

    return 0;
}



