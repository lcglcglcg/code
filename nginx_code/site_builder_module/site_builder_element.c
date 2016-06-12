
#include "site_builder_element.h"

void site_builder_element_init(site_builder_element_head_tree_t *p, ngx_pool_t *pool)
{
    p->head = RB_ROOT;
    p->pool = pool;
    p->total = 0;
}

site_builder_element_tree_t *site_builder_element_key_find(site_builder_element_head_tree_t *p, const char *key)
{

    return (site_builder_element_tree_t *)lg_tree_element_key_find(&p->head, key);
}

site_builder_element_tree_t *site_builder_element_index_find(site_builder_element_head_tree_t *p, int index)
{

    return (site_builder_element_tree_t *)lg_tree_element_index_find(&p->head, index);
}

site_builder_element_tree_t *site_builder_element_index_rand(site_builder_element_head_tree_t *p)
{

    if (!p || p->total <= 0) return NULL;

    int index = rand() % p->total;
    return (site_builder_element_tree_t *)lg_tree_element_index_find(&p->head, index);
}

site_builder_element_tree_t *site_builder_element_key_add(site_builder_element_head_tree_t *p,
	const char *key, int key_size,
	const char *buffer, int buffer_size)
{

    if (!p) return NULL;

    site_builder_element_tree_t *element = ngx_pcalloc(p->pool, sizeof(site_builder_element_tree_t));
    if (!element) return NULL;

    element->key = ngx_pcalloc(p->pool, key_size + 1);
    if (!element->key) return NULL;

    memcpy(element->key, key, key_size);
    element->key_size = key_size;
    element->key[key_size] = 0;

    element->buffer = buffer;
    element->buffer_size = buffer_size;

    p->total++;
    element->tree.index = 0;
    element->tree.key = element->key;
    lg_tree_element_key_add(&p->head, &element->tree);
    return element;
}

site_builder_element_tree_t *site_builder_element_key_add_r(site_builder_element_head_tree_t *p,
	const char *key, int key_size,
	const char *buffer, int buffer_size)
{

    char *p_buffer = ngx_pcalloc(p->pool, buffer_size + 1);
    if (!p_buffer) return NULL;

    memcpy(p_buffer, buffer, buffer_size);
    p_buffer[buffer_size] = 0;
    return site_builder_element_key_add(p, key, key_size, p_buffer, buffer_size);
}

site_builder_element_tree_t *site_builder_element_index_add(site_builder_element_head_tree_t *p,
	const char *buffer, int buffer_size)
{

    site_builder_element_tree_t *element = ngx_pcalloc(p->pool, sizeof(site_builder_element_tree_t));
    if (!element) return NULL;

    element->key = NULL;
    element->key_size = 0;

    element->buffer = buffer;
    element->buffer_size = buffer_size;

    element->tree.key = NULL;
    element->tree.index = p->total++;
    lg_tree_element_index_add(&p->head, &element->tree);
    return element;
}

site_builder_element_tree_t *site_builder_element_index_add_r(site_builder_element_head_tree_t *p,
	const char *buffer, int buffer_size)
{

    char *p_buffer = ngx_pcalloc(p->pool, buffer_size + 1);
    if (!p_buffer) return NULL;

    memcpy(p_buffer, buffer, buffer_size);
    p_buffer[buffer_size] = 0;
    return site_builder_element_index_add(p, p_buffer, buffer_size);
}

int site_builder_element_buffer_to_file(const char *path, const char *buffer, int buffer_size)
{

    FILE *fp = fopen(path, "w");
    if (!fp) {

	fprintf(stdout, "file path ::err\n");
	return -1;
    }

    fwrite(buffer, 1, buffer_size, fp);
    fclose(fp);
    return 0;
}

int site_builder_element_file_to_buffer(const char *path, char **buffer, ngx_pool_t *pool)
{

    FILE *fp = fopen(path, "r");
    if (!fp) {

	fprintf(stdout, "file path ::err\n");
	return -1;
    }

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

int site_builder_element_load_file(const char *file_path, site_builder_element_head_tree_t *p, ngx_pool_t *pool)
{

    site_builder_element_init(p, pool);

    FILE *fp = fopen(file_path, "r");
    if (!fp) return -1;

    char buffer[SITE_BUILDER_BUF_SIZE];
    while(fgets(buffer, SITE_BUILDER_BUF_SIZE, fp)) {

	char *saveptr = NULL;
	char *str = strtok_r(buffer, "\r\n", &saveptr);
	if (str) {

	    int len = strlen(str);
	    if (len > SITE_BUILDER_BUF_MIN) {

		site_builder_element_index_add_r(p, str, len);
	    }
	}
    }

    fclose(fp);
    return 0;
}

int site_builder_element_load_link_external_file(const char *file_path, site_builder_element_head_tree_t *p, ngx_pool_t *pool)
{

    site_builder_element_init(p, pool);

    FILE *fp = fopen(file_path, "r");
    if (!fp) return -1;

    char buffer[SITE_BUILDER_BUF_SIZE];
    char buffer_link[SITE_BUILDER_BUF_SIZE];
    while(fgets(buffer, SITE_BUILDER_BUF_SIZE, fp)) {

	char *saveptr = NULL;
	char *str1 = strtok_r(buffer, "\t\r\n", &saveptr);
	char *str2 = strtok_r(NULL, "\t\r\n", &saveptr);
	if (str1 && str2) {

	    int str1_len = strlen(str1);
	    int str2_len = strlen(str2);
	    if (str1_len > 0 && str2_len > 0) {

		size_t size = sprintf(buffer_link, "<a href=\"http://%s\" title=\"\%s\" target=\"_blank\">%s</a>",
			str2,
			str1,
			str1);

		// fprintf(stdout, "%s\n", buffer_link);
		site_builder_element_index_add_r(p, buffer_link, size);
	    }
	}
    }

    fclose(fp);
    return 0;
}

int site_builder_element_load_link_bare_file(const char *file_path, site_builder_element_head_tree_t *p, ngx_pool_t *pool)
{

    site_builder_element_init(p, pool);

    FILE *fp = fopen(file_path, "r");
    if (!fp) return -1;

    char buffer[SITE_BUILDER_BUF_SIZE];
    while(fgets(buffer, SITE_BUILDER_BUF_SIZE, fp)) {

	char *saveptr = NULL;
	char *str = strtok_r(buffer, "\t\r\n", &saveptr);
	str = strtok_r(NULL, "\t\r\n", &saveptr);
	if (str) {

	    int len = strlen(str);
	    if (len > SITE_BUILDER_BUF_MIN) {

		// fprintf(stdout, "%s\n", str);
		site_builder_element_index_add_r(p, str, len);
	    }
	}
    }

    fclose(fp);
    return 0;
}


