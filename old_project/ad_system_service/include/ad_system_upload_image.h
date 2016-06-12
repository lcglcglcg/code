
#ifndef	__AD_SYSTEM_UPLOAD_IMG_H__
#define	__AD_SYSTEM_UPLOAD_IMG_H__

#ifdef __cplusplus
extern "C" { 
#endif

#include "lg_list.h"
#define	UPLOAD_IMG_FILE_PATH "/var/www/html"

    typedef struct {

	lg_list_t list;

	char key[64];
	char name[512];
	char path[1024];

	int width;
	int height;
	int length;

    } ad_system_image_t;

    extern ad_system_image_t *ad_system_image_list_find(const char *key);
    extern ad_system_image_t *ad_system_image_list_add(
	    char *key,
	    char *name,
	    char *path,
	    int width,
	    int height,
	    int length);

#ifdef __cplusplus
}
#endif

#endif

