
#include "site_builder_dict.h"

ngx_queue_t global_keyword = {&global_keyword, &global_keyword};
ngx_queue_t global_content = {&global_content, &global_content};
ngx_queue_t global_link_external = {&global_link_external, &global_link_external};

int site_builder_dict_file_to_buffer(char *path, char **buffer, ngx_pool_t *pool)
{

    FILE *fp = fopen(path, "rb");
    if (!fp) return -1;

    fseek(fp, 0, SEEK_END);
    size_t size = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    if (!(*buffer = ngx_pcalloc(pool, size + 1))) {

	fclose(fp);
	return -1;
    }   

    if (fread(*buffer, 1, size, fp) != size) {

	fclose(fp);
	free(*buffer);
	return -1;
    }

    (*buffer)[size] = 0;
    fclose(fp);
    return size;
}

int site_builder_dict_load_file(char *dict_id, site_builder_element_head_t *p, ngx_pool_t *pool)
{

    site_builder_element_init(p, dict_id, pool);

    char path[256];
    sprintf(path, "%s/dict/%s", SITE_BUILDER_DICT_HOME_PATH, dict_id);
    FILE *fp = fopen(path, "rb");
    if (!fp) return -1;

    char buffer[SITE_BUILDER_DICT_BUF_SIZE];
    while(fgets(buffer, SITE_BUILDER_DICT_BUF_SIZE, fp)) {

	char *saveptr = NULL;
	char *str = strtok_r(buffer, "\r\n", &saveptr);
	if (str) {

	    int len = strlen(str);
	    if (len > 5) {

		// fprintf(stdout, "%d::%s\n", len, str);
		site_builder_element_index_add_r(p, NULL, 0, str, len);
	    }
	}
    }

    fclose(fp);
    return 0;
}

int site_builder_dict_load_file_cut(char *dict_id, site_builder_element_head_t *p, ngx_pool_t *pool)
{

    site_builder_element_init(p, dict_id, pool);

    char path[256];
    sprintf(path, "%s/dict/%s", SITE_BUILDER_DICT_HOME_PATH, dict_id);
    FILE *fp = fopen(path, "rb");
    if (!fp) return -1;

    char buffer[SITE_BUILDER_DICT_BUF_SIZE];
    while(fgets(buffer, SITE_BUILDER_DICT_BUF_SIZE, fp)) {

	char *saveptr = NULL;
	char *str1 = strtok_r(buffer, "\t\r\n", &saveptr);
	char *str2 = strtok_r(NULL, "\t\r\n", &saveptr);
	if (str1 && str2) {

	    int str1_len = strlen(str1);
	    int str2_len = strlen(str2);
	    if (str1_len > 0 && str2_len > 0) {

		site_builder_element_index_add_r(p, str2, str2_len, str1, str1_len);
	    }
	}
    }

    fclose(fp);
    return 0;
}

site_builder_element_head_t *site_builder_dict_keyword(char *dict_id, ngx_pool_t *pool)
{

    ngx_queue_t *p = NULL;
    site_builder_element_head_t *node = NULL;
    for (p = ngx_queue_head(&global_keyword);
	    p != ngx_queue_sentinel(&global_keyword);
	    p = ngx_queue_next(p)) {

	node = (site_builder_element_head_t *)p;
	// fprintf(stdout, "==>%s\n", node->name);
	if (strcmp(dict_id, node->name) == 0)
	    return node;
    }

    node = ngx_pcalloc(pool, sizeof(site_builder_element_head_t));
    if (!node) return NULL;

    if (site_builder_dict_load_file(dict_id, node, pool))
	return NULL;

    ngx_queue_insert_tail(&global_keyword, &node->next);
    return node;
}

site_builder_element_head_t *site_builder_dict_content(char *dict_id, ngx_pool_t *pool)
{

    ngx_queue_t *p = NULL;
    site_builder_element_head_t *node = NULL;
    for (p = ngx_queue_head(&global_content);
	    p != ngx_queue_sentinel(&global_content);
	    p = ngx_queue_next(p)) {

	node = (site_builder_element_head_t *)p;
	// fprintf(stdout, "==>%s\n", node->name);
	if (strcmp(dict_id, node->name) == 0)
	    return node;
    }

    node = ngx_pcalloc(pool, sizeof(site_builder_element_head_t));
    if (!node) return NULL;

    if (site_builder_dict_load_file(dict_id, node, pool))
	return NULL;

    ngx_queue_insert_tail(&global_content, &node->next);
    return node;
}

site_builder_element_head_t *site_builder_dict_link_external(char *dict_id, ngx_pool_t *pool)
{

    ngx_queue_t *p = NULL;
    site_builder_element_head_t *node = NULL;
    for (p = ngx_queue_head(&global_link_external);
	    p != ngx_queue_sentinel(&global_link_external);
	    p = ngx_queue_next(p)) {

	node = (site_builder_element_head_t *)p;
	// fprintf(stdout, "==>%s\n", node->name);
	if (strcmp(dict_id, node->name) == 0)
	    return node;
    }

    node = ngx_pcalloc(pool, sizeof(site_builder_element_head_t));
    if (!node) return NULL;

    if (site_builder_dict_load_file_cut(dict_id, node, pool))
	return NULL;

    ngx_queue_insert_tail(&global_link_external, &node->next);
    return node;
}













