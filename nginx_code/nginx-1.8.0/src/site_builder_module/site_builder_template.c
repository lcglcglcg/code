
#include "site_builder_template.h"

#define SITE_BUILDER_TEMPLATE_TITLE			"<?title?>"			//标题
#define SITE_BUILDER_TEMPLATE_KEYWORD			"<?keyword?>"			//关键词
#define SITE_BUILDER_TEMPLATE_CONTENT			"<?content?>"			//文章内容
#define SITE_BUILDER_TEMPLATE_META			"<?meta?>"			//描述

#define SITE_BUILDER_TEMPLATE_DATE			"<?date?>"			//2015-01-01
#define SITE_BUILDER_TEMPLATE_TIME			"<?time?>"			//2015-01-01 00:00:00
#define SITE_BUILDER_TEMPLATE_COUNT			"<?count?>"

#define SITE_BUILDER_TEMPLATE_LINK_INTERNAL		"<?link_internal?>"		//内链接
#define SITE_BUILDER_TEMPLATE_LINK_EXTERNAL		"<?link_external?>"		//外链接
#define SITE_BUILDER_TEMPLATE_LINK_BARE			"<?link_bare?>"			//裸链接
#define SITE_BUILDER_TEMPLATE_LINK_GROUP		"<?link_group?>"		//组链接

#define SITE_BUILDER_TEMPLATE_TITLE_SIZE		9
#define SITE_BUILDER_TEMPLATE_KEYWORD_SIZE		11
#define SITE_BUILDER_TEMPLATE_CONTENT_SIZE		11
#define SITE_BUILDER_TEMPLATE_META_SIZE			8

#define SITE_BUILDER_TEMPLATE_DATE_SIZE			8
#define SITE_BUILDER_TEMPLATE_TIME_SIZE			8
#define SITE_BUILDER_TEMPLATE_COUNT_SIZE		9

#define SITE_BUILDER_TEMPLATE_LINK_INTERNAL_SIZE	17
#define SITE_BUILDER_TEMPLATE_LINK_EXTERNAL_SIZE	17
#define SITE_BUILDER_TEMPLATE_LINK_BARE_SIZE		13
#define SITE_BUILDER_TEMPLATE_LINK_GROUP_SIZE		14

int site_builder_template_node_add(ngx_pool_t *pool, ngx_queue_t *head, int type, char *buffer, int size)
{

    if (type == SITE_BUILDER_TEMPLATE_TYPE_HTML && size <= 0) 
	return 0;

    site_builder_template_node_t *node = ngx_pcalloc(pool, sizeof(site_builder_template_node_t));
    if (!node) return -1;

    node->type = type;
    node->buffer = NULL;
    node->size = 0;

    if (type == SITE_BUILDER_TEMPLATE_TYPE_HTML) {

	node->buffer = ngx_pcalloc(pool, size + 1);
	if (!node->buffer) return -1;

	memcpy(node->buffer, buffer, size);
	node->buffer[size] = 0;
	node->size =size;
    }

    ngx_queue_insert_tail(head, &node->next);
    return 0;
}

int __site_builder_template_parse(ngx_pool_t *pool, ngx_queue_t *head, int type, char *buffer, int size)
{

    if (site_builder_template_node_add(pool, head, SITE_BUILDER_TEMPLATE_TYPE_HTML, buffer, size))
	return -1;

    if (site_builder_template_node_add(pool, head, type, NULL, 0))
	return -1;

    return 0;
}

int site_builder_template_parse(site_builder_template_head_t *template, char *buffer, ngx_pool_t*pool)
{

    char *str = buffer;
    char *temp = buffer;
    ngx_queue_t *head = &template->head;
    ngx_queue_init(head);

    while (*str) {

	/* TITLE */
	if (strncmp(str, SITE_BUILDER_TEMPLATE_TITLE, SITE_BUILDER_TEMPLATE_TITLE_SIZE) == 0) {

	    if (__site_builder_template_parse(pool, head, SITE_BUILDER_TEMPLATE_TYPE_TITLE, temp, str - temp))
		return -1;

	    template->title_total++;
	    temp = str = str + SITE_BUILDER_TEMPLATE_TITLE_SIZE;

	    /* KEYWORD */
	} else if (strncmp(str, SITE_BUILDER_TEMPLATE_KEYWORD, SITE_BUILDER_TEMPLATE_KEYWORD_SIZE) == 0) {

	    if (__site_builder_template_parse(pool, head, SITE_BUILDER_TEMPLATE_TYPE_KEYWORD, temp, str - temp))
		return -1;

	    template->keyword_total++;
	    temp = str = str + SITE_BUILDER_TEMPLATE_KEYWORD_SIZE;

	    /* CONTENT */
	} else if (strncmp(str, SITE_BUILDER_TEMPLATE_CONTENT, SITE_BUILDER_TEMPLATE_CONTENT_SIZE) == 0) {

	    if (__site_builder_template_parse(pool, head, SITE_BUILDER_TEMPLATE_TYPE_CONTENT, temp, str - temp))
		return -1;

	    template->content_total++;
	    temp = str = str + SITE_BUILDER_TEMPLATE_CONTENT_SIZE;

	    /* META */
	} else if (strncmp(str, SITE_BUILDER_TEMPLATE_META, SITE_BUILDER_TEMPLATE_META_SIZE) == 0) {

	    if (__site_builder_template_parse(pool, head, SITE_BUILDER_TEMPLATE_TYPE_META, temp, str - temp))
		return -1;

	    template->meta_total++;
	    temp = str = str + SITE_BUILDER_TEMPLATE_META_SIZE;

	    /* DATE */
	} else if (strncmp(str, SITE_BUILDER_TEMPLATE_DATE, SITE_BUILDER_TEMPLATE_DATE_SIZE) == 0) {

	    if (__site_builder_template_parse(pool, head, SITE_BUILDER_TEMPLATE_TYPE_DATE, temp, str - temp))
		return -1;

	    template->date_total++;
	    temp = str = str + SITE_BUILDER_TEMPLATE_DATE_SIZE;

	    /* TIME */
	} else if (strncmp(str, SITE_BUILDER_TEMPLATE_TIME, SITE_BUILDER_TEMPLATE_TIME_SIZE) == 0) {

	    if (__site_builder_template_parse(pool, head, SITE_BUILDER_TEMPLATE_TYPE_TIME, temp, str - temp))
		return -1;

	    template->time_total++;
	    temp = str = str + SITE_BUILDER_TEMPLATE_TIME_SIZE;

	    /* TIME */
	} else if (strncmp(str, SITE_BUILDER_TEMPLATE_COUNT, SITE_BUILDER_TEMPLATE_COUNT_SIZE) == 0) {

	    if (__site_builder_template_parse(pool, head, SITE_BUILDER_TEMPLATE_TYPE_COUNT, temp, str - temp))
		return -1;

	    template->count_total++;
	    temp = str = str + SITE_BUILDER_TEMPLATE_COUNT_SIZE;

	    /* LINK_INTERNAL */
	} else if (strncmp(str, SITE_BUILDER_TEMPLATE_LINK_INTERNAL, SITE_BUILDER_TEMPLATE_LINK_INTERNAL_SIZE) == 0) {

	    if (__site_builder_template_parse(pool, head, SITE_BUILDER_TEMPLATE_TYPE_LINK_INTERNAL, temp, str - temp))
		return -1;

	    template->link_internal_total++;
	    temp = str = str + SITE_BUILDER_TEMPLATE_LINK_INTERNAL_SIZE;

	    /* LINK_EXTERNAL */
	} else if (strncmp(str, SITE_BUILDER_TEMPLATE_LINK_EXTERNAL, SITE_BUILDER_TEMPLATE_LINK_EXTERNAL_SIZE) == 0) {

	    if (__site_builder_template_parse(pool, head, SITE_BUILDER_TEMPLATE_TYPE_LINK_EXTERNAL, temp, str - temp))
		return -1;

	    template->link_external_total++;
	    temp = str = str + SITE_BUILDER_TEMPLATE_LINK_EXTERNAL_SIZE;

	    /* LINK_BARE */
	} else if (strncmp(str, SITE_BUILDER_TEMPLATE_LINK_BARE, SITE_BUILDER_TEMPLATE_LINK_BARE_SIZE) == 0) {

	    if (__site_builder_template_parse(pool, head, SITE_BUILDER_TEMPLATE_TYPE_LINK_BARE, temp, str - temp))
		return -1;

	    template->link_bare_total++;
	    temp = str = str + SITE_BUILDER_TEMPLATE_LINK_BARE_SIZE;

	    /* LINK_GROUP */
	} else if (strncmp(str, SITE_BUILDER_TEMPLATE_LINK_GROUP, SITE_BUILDER_TEMPLATE_LINK_GROUP_SIZE) == 0) {

	    if (__site_builder_template_parse(pool, head, SITE_BUILDER_TEMPLATE_TYPE_LINK_GROUP, temp, str - temp))
		return -1;

	    template->link_group_total++;
	    temp = str = str + SITE_BUILDER_TEMPLATE_LINK_GROUP_SIZE;

	} else {

	    str++;
	}
    }

    if (site_builder_template_node_add(pool, head, SITE_BUILDER_TEMPLATE_TYPE_HTML, temp, str - temp))
	return -1;

    // site_builder_template_print(template);
    return 0;
}

void site_builder_template_print(site_builder_template_head_t *template)
{

    ngx_queue_t *p = NULL;
    ngx_queue_t *head = &template->head;
    for (p = ngx_queue_head(head); p != ngx_queue_sentinel(head); p = ngx_queue_next(p)) {

	site_builder_template_node_t *node = (site_builder_template_node_t *)p;
	switch(node->type) {

	    case SITE_BUILDER_TEMPLATE_TYPE_HTML:
		fprintf(stdout, "%s", node->buffer);
		break;

	    case SITE_BUILDER_TEMPLATE_TYPE_TITLE:
		fprintf(stdout, "[TITLE]");
		break;

	    case SITE_BUILDER_TEMPLATE_TYPE_KEYWORD:
		fprintf(stdout, "[KEYWORD]");
		break;

	    case SITE_BUILDER_TEMPLATE_TYPE_CONTENT:
		fprintf(stdout, "[CONTENT]");
		break;

	    case SITE_BUILDER_TEMPLATE_TYPE_META:
		fprintf(stdout, "[META]");
		break;

	    case SITE_BUILDER_TEMPLATE_TYPE_LINK_INTERNAL:
		fprintf(stdout, "[LINK_INTERNAL]");
		break;

	    case SITE_BUILDER_TEMPLATE_TYPE_LINK_EXTERNAL:
		fprintf(stdout, "[LINK_EXTERNAL]");
		break;

	    case SITE_BUILDER_TEMPLATE_TYPE_LINK_BARE:
		fprintf(stdout, "[LINK_BARE]");
		break;

	    case SITE_BUILDER_TEMPLATE_TYPE_LINK_GROUP:
		fprintf(stdout, "[LINK_GROUP]");
		break;
	}
    }

    fprintf(stdout, "================================\n");
}

int site_builder_template_dir_load(char *path,
	site_builder_template_head_t *template_index,
	site_builder_template_head_t *template_content,
	site_builder_element_head_t *affiliated_file,
	ngx_pool_t *pool)
{

    DIR *dir = NULL;
    struct dirent *dir_info = NULL;
    char _path[256];

    if (!(dir = opendir(path)))
	return -1;

    while((dir_info = readdir(dir))) {

	if((strcmp(dir_info->d_name,".") == 0))
	    continue;

	if ((strcmp(dir_info->d_name,"..")==0))
	    continue;

	sprintf(_path, "%s/%s", path, dir_info->d_name);
	if (dir_info->d_type == 4) { //dir

	    site_builder_template_dir_load(_path,
		    template_index,
		    template_content,
		    affiliated_file,
		    pool);

	} else if (dir_info->d_type == 8) { //file

	    char *buffer = NULL;
	    int buffer_size = site_builder_dict_file_to_buffer(_path, &buffer, pool);
	    if (buffer_size >= 0 && buffer) {

		if (strcmp(dir_info->d_name, "index.html") == 0) {

		    if (site_builder_template_parse(template_index, buffer, pool))
			return -1;

		} else if (strcmp(dir_info->d_name, "content.html") == 0) {

		    if (site_builder_template_parse(template_content, buffer, pool))
			return -1;

		} else {

		    char *key = _path;
		    int key_size = sprintf(key, "/%s", dir_info->d_name);
		    site_builder_element_key_add(affiliated_file,
			    key, key_size,
			    buffer, buffer_size);
		}
	    }
	}
    }

    closedir(dir);
    return 0;
}

int site_builder_template_load(char *template_id,
	site_builder_template_head_t *template_index,
	site_builder_template_head_t *template_content,
	site_builder_element_head_t *affiliated_file,
	ngx_pool_t *pool)
{

    ngx_queue_init(&template_index->head);
    ngx_queue_init(&template_content->head);
    site_builder_element_init(affiliated_file, NULL, pool);

    char path[256];
    sprintf(path, "%s/template/%s", SITE_BUILDER_DICT_HOME_PATH, template_id);
    site_builder_template_dir_load(path, 
	    template_index,
	    template_content,
	    affiliated_file,
	    pool);
    return 0;
}




