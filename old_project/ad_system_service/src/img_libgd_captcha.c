
#include "img_libgd_captcha.h"

int get_img_libgd_captcha(img_libgd_captcha_t *img)
{

    img->size = 0;
    img->buffer = NULL;
    sprintf(img->result, "%u", rand() % 8888 + 1000);

    gdImagePtr im = gdImageCreate(32, 16);
    gdImageColorAllocate(im, 255, 255, 255);
    gdImageString(im, gdFontGetLarge(), 0, 0, (unsigned char *)img->result, 1);

    img->buffer = gdImagePngPtr(im, &img->size);
    gdImageDestroy(im);
    return 0;
}


