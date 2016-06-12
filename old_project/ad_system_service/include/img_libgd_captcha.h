
#ifndef	__IMG_LIBGD_CAPTCHA_H__
#define	__IMG_LIBGD_CAPTCHA_H__

#ifdef __cplusplus
extern "C" { 
#endif
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include "gd.h"
#include "gdfontl.h"

    typedef struct {

	int size;
	char *buffer;
	char result[64];

    } img_libgd_captcha_t;

    extern int get_img_libgd_captcha(img_libgd_captcha_t *img);

#ifdef __cplusplus
}
#endif

#endif
