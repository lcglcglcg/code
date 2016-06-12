
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "ad_system_upload_image.h"

lg_list_t ad_system_image_list_head = {&ad_system_image_list_head, &ad_system_image_list_head};
pthread_mutex_t ad_system_image_list_lock = PTHREAD_MUTEX_INITIALIZER;

ad_system_image_t *ad_system_image_list_add(
	char *key,
	char *name,
	char *path,
	int width,
	int height,
	int length)
{

    ad_system_image_t *node = NULL;
    if (!(node = malloc(sizeof(ad_system_image_t))))
	return NULL;

    strcpy(node->key, key);
    strcpy(node->name, name);
    strcpy(node->path, path);
    node->width = width;
    node->height = height;
    node->length = length;

    pthread_mutex_lock(&ad_system_image_list_lock);
    lg_list_add(&ad_system_image_list_head, &node->list);
    pthread_mutex_unlock(&ad_system_image_list_lock);

    return node;
}

ad_system_image_t *ad_system_image_list_find(const char *key)
{
    if (!key) return NULL;

    pthread_mutex_lock(&ad_system_image_list_lock);

    lg_list_t *p = NULL;
    lg_list_for_each(p, &ad_system_image_list_head){

	ad_system_image_t *node = NULL;
	node = (ad_system_image_t *)p;
	if (strcmp(key, node->key) == 0) {

	    pthread_mutex_unlock(&ad_system_image_list_lock);
	    return node;
	}
    }

    pthread_mutex_unlock(&ad_system_image_list_lock);
    return NULL;
}




