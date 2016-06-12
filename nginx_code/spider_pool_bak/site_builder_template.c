
#include "site_builder_template.h"

#define SITE_BUILDER_TEMPLATE_TITLE			"<?title?>"			//标题
#define SITE_BUILDER_TEMPLATE_KEYWORD			"<?keyword?>"			//关键词
#define SITE_BUILDER_TEMPLATE_CONTENT			"<?content?>"			//文章内容

#define SITE_BUILDER_TEMPLATE_LINK_INTERNAL		"<?link_internal?>"		//内链接
#define SITE_BUILDER_TEMPLATE_LINK_EXTERNAL		"<?link_external?>"		//外链接
#define SITE_BUILDER_TEMPLATE_LINK_BARE			"<?link_bare?>"			//裸链接
#define SITE_BUILDER_TEMPLATE_LINK_GROUP		"<?link_group?>"		//组链接

#define SITE_BUILDER_TEMPLATE_TITLE_SIZE		9
#define SITE_BUILDER_TEMPLATE_KEYWORD_SIZE		11
#define SITE_BUILDER_TEMPLATE_CONTENT_SIZE		11

#define SITE_BUILDER_TEMPLATE_LINK_INTERNAL_SIZE	17
#define SITE_BUILDER_TEMPLATE_LINK_EXTERNAL_SIZE	17
#define SITE_BUILDER_TEMPLATE_LINK_BARE_SIZE		13
#define SITE_BUILDER_TEMPLATE_LINK_GROUP_SIZE		14

void site_builder_template_init(site_builder_template_t *template)
{

    memset(template, 0, sizeof(site_builder_template_t));
    ngx_queue_init(&template->head);
}

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

site_builder_template_t *site_builder_template_parse(char *buffer, ngx_pool_t*pool)
{

    site_builder_template_t *template = ngx_pcalloc(pool, sizeof(site_builder_template_t));
    if (!template) return NULL;

    char *str = buffer;
    char *temp = buffer;
    ngx_queue_t *head = &template->head;
    ngx_queue_init(head);

    while (*str) {

	/* TITLE */
	if (strncmp(str, SITE_BUILDER_TEMPLATE_TITLE, SITE_BUILDER_TEMPLATE_TITLE_SIZE) == 0) {

	    if (__site_builder_template_parse(pool, head, SITE_BUILDER_TEMPLATE_TYPE_TITLE, temp, str - temp))
		return NULL;

	    template->title_total++;
	    temp = str = str + SITE_BUILDER_TEMPLATE_TITLE_SIZE;

	    /* KEYWORD */
	} else if (strncmp(str, SITE_BUILDER_TEMPLATE_KEYWORD, SITE_BUILDER_TEMPLATE_KEYWORD_SIZE) == 0) {

	    if (__site_builder_template_parse(pool, head, SITE_BUILDER_TEMPLATE_TYPE_KEYWORD, temp, str - temp))
		return NULL;

	    template->keyword_total++;
	    temp = str = str + SITE_BUILDER_TEMPLATE_KEYWORD_SIZE;

	    /* CONTENT */
	} else if (strncmp(str, SITE_BUILDER_TEMPLATE_CONTENT, SITE_BUILDER_TEMPLATE_CONTENT_SIZE) == 0) {

	    if (__site_builder_template_parse(pool, head, SITE_BUILDER_TEMPLATE_TYPE_CONTENT, temp, str - temp))
		return NULL;

	    template->content_total++;
	    temp = str = str + SITE_BUILDER_TEMPLATE_CONTENT_SIZE;

	    /* LINK_INTERNAL */
	} else if (strncmp(str, SITE_BUILDER_TEMPLATE_LINK_INTERNAL, SITE_BUILDER_TEMPLATE_LINK_INTERNAL_SIZE) == 0) {

	    if (__site_builder_template_parse(pool, head, SITE_BUILDER_TEMPLATE_TYPE_LINK_INTERNAL, temp, str - temp))
		return NULL;

	    template->link_internal_total++;
	    temp = str = str + SITE_BUILDER_TEMPLATE_LINK_INTERNAL_SIZE;

	    /* LINK_EXTERNAL */
	} else if (strncmp(str, SITE_BUILDER_TEMPLATE_LINK_EXTERNAL, SITE_BUILDER_TEMPLATE_LINK_EXTERNAL_SIZE) == 0) {

	    if (__site_builder_template_parse(pool, head, SITE_BUILDER_TEMPLATE_TYPE_LINK_EXTERNAL, temp, str - temp))
		return NULL;

	    template->link_external_total++;
	    temp = str = str + SITE_BUILDER_TEMPLATE_LINK_EXTERNAL_SIZE;

	    /* LINK_BARE */
	} else if (strncmp(str, SITE_BUILDER_TEMPLATE_LINK_BARE, SITE_BUILDER_TEMPLATE_LINK_BARE_SIZE) == 0) {

	    if (__site_builder_template_parse(pool, head, SITE_BUILDER_TEMPLATE_TYPE_LINK_BARE, temp, str - temp))
		return NULL;

	    template->link_external_total++;
	    temp = str = str + SITE_BUILDER_TEMPLATE_LINK_EXTERNAL_SIZE;

	    /* LINK_GROUP */
	} else if (strncmp(str, SITE_BUILDER_TEMPLATE_LINK_GROUP, SITE_BUILDER_TEMPLATE_LINK_GROUP_SIZE) == 0) {

	    if (__site_builder_template_parse(pool, head, SITE_BUILDER_TEMPLATE_TYPE_LINK_GROUP, temp, str - temp))
		return NULL;

	    template->link_group_total++;
	    temp = str = str + SITE_BUILDER_TEMPLATE_LINK_GROUP_SIZE;

	} else {

	    str++;
	}
    }

    if (site_builder_template_node_add(pool, head, SITE_BUILDER_TEMPLATE_TYPE_HTML, temp, str - temp))
	return NULL;

    return template;
}

void site_builder_template_print(site_builder_template_t *template)
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

    fprintf(stdout, "template_id=%d\n",			template->template_id);
    fprintf(stdout, "template_name=%s\n",		template->template_name);

    fprintf(stdout, "title_total=%d\n",			template->title_total);
    fprintf(stdout, "keyword_total=%d\n",		template->keyword_total);
    fprintf(stdout, "content_total=%d\n",		template->content_total);

    fprintf(stdout, "link_internal=%d\n",		template->link_internal_total);
    fprintf(stdout, "link_external=%d\n",		template->link_external_total);

    fprintf(stdout, "================================\n");
}

int site_builder_template_load(
	site_builder_template_list_t *template_list,
	char *template_name,
	char *buffer, 
	ngx_pool_t *pool)
{

    site_builder_template_t *template = site_builder_template_parse(buffer, pool);
    if (!template) return -1;

    template->template_id = template_list->template_total++;

    int template_name_len = strlen(template_name);
    template->template_name = ngx_pcalloc(pool, template_name_len + 1);
    memcpy(template->template_name, template_name, template_name_len);
    template->template_name[template_name_len] = 0;

    ngx_queue_insert_tail(&template_list->head, &template->next);

    // site_builder_template_print(template);
    return 0;
}

int _site_builder_template_dir_load(
	char *path,
	int path_size,
	site_builder_template_list_t *template_list,
	site_builder_element_head_t *affiliated_file_list,
	ngx_pool_t *pool)
{

    DIR *dir = NULL;
    struct dirent *dir_info = NULL;
    char _path[SITE_BUILDER_BUF_SIZE];
    char *key = &_path[path_size];

    if (!(dir = opendir(path)))
	return -1;

    while((dir_info = readdir(dir))) {

	if((strcmp(dir_info->d_name,".") == 0))
	    continue;

	if ((strcmp(dir_info->d_name,"..")==0))
	    continue;

	int size = sprintf(_path, "%s/%s", path, dir_info->d_name);
	if (dir_info->d_type == 4) { //dir

	    _site_builder_template_dir_load(_path, path_size, template_list, affiliated_file_list, pool);

	} else if (dir_info->d_type == 8) { //file

	    char *buffer = NULL;
	    int buffer_size = site_builder_element_file_to_buffer(_path, &buffer, pool);
	    if (buffer_size >= 0 && buffer) {

		if (strncmp(&_path[size - 5], ".html", 5) == 0) {

		    // fprintf(stdout, "[%s][%s]\n", _path, dir_info->d_name);
		    site_builder_template_load(template_list, dir_info->d_name, buffer, pool);

		} else {

		    site_builder_element_key_add(affiliated_file_list,
			    key, strlen(key),
			    buffer, buffer_size);
		}
	    }
	}
    }

    closedir(dir);
    return 0;
}

int site_builder_template_dir_load(
	char *path,
	int path_size,
	site_builder_template_list_t *template_list,
	site_builder_element_head_t *affiliated_file_list,
	ngx_pool_t *pool)
{

    ngx_queue_init(&template_list->head);
    template_list->affiliated_file_list = affiliated_file_list;
    template_list->template_total = 0;

    site_builder_element_init(affiliated_file_list, pool);
    _site_builder_template_dir_load(path, path_size, template_list, affiliated_file_list, pool);
    return 0;
}




