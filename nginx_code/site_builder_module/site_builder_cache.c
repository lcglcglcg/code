
#include "site_builder_cache.h"
#include "site_builder_flush.h"

/* cache */
lg_rbtree_head global_cache_title_head = RB_ROOT;
lg_rbtree_head global_cache_keyword_head = RB_ROOT;
lg_rbtree_head global_cache_content_head = RB_ROOT;
lg_rbtree_head global_cache_meta_head = RB_ROOT;

lg_rbtree_head global_cache_date_str_head = RB_ROOT;
lg_rbtree_head global_cache_time_str_head = RB_ROOT;

lg_rbtree_head global_cache_template_head = RB_ROOT;

lg_rbtree_head global_cache_link_save_internal_head = RB_ROOT;
lg_rbtree_head global_cache_link_save_external_head = RB_ROOT;
lg_rbtree_head global_cache_link_save_bare_head = RB_ROOT;
lg_rbtree_head global_cache_link_save_group_all_head = RB_ROOT;

lg_rbtree_head global_cache_link_save_group_0 = RB_ROOT;
lg_rbtree_head global_cache_link_save_group_1 = RB_ROOT;
lg_rbtree_head global_cache_link_save_group_2 = RB_ROOT;
lg_rbtree_head global_cache_link_save_group_3 = RB_ROOT;
lg_rbtree_head global_cache_link_save_group_4 = RB_ROOT;
lg_rbtree_head global_cache_link_save_group_5 = RB_ROOT;
lg_rbtree_head global_cache_link_save_group_6 = RB_ROOT;
lg_rbtree_head global_cache_link_save_group_7 = RB_ROOT;
lg_rbtree_head global_cache_link_save_group_8 = RB_ROOT;
lg_rbtree_head global_cache_link_save_group_9 = RB_ROOT;

ngx_pool_t *site_builder_cache_pool = NULL;
void site_builder_cache_init(ngx_pool_t *pool)
{

    global_cache_title_head = RB_ROOT;
    global_cache_keyword_head = RB_ROOT;
    global_cache_content_head = RB_ROOT;
    global_cache_meta_head = RB_ROOT;

    global_cache_date_str_head = RB_ROOT;
    global_cache_time_str_head = RB_ROOT;

    global_cache_template_head = RB_ROOT;

    global_cache_link_save_internal_head = RB_ROOT;
    global_cache_link_save_external_head = RB_ROOT;
    global_cache_link_save_bare_head = RB_ROOT;
    global_cache_link_save_group_all_head = RB_ROOT;

    global_cache_link_save_group_0 = RB_ROOT;
    global_cache_link_save_group_1 = RB_ROOT;
    global_cache_link_save_group_2 = RB_ROOT;
    global_cache_link_save_group_3 = RB_ROOT;
    global_cache_link_save_group_4 = RB_ROOT;
    global_cache_link_save_group_5 = RB_ROOT;
    global_cache_link_save_group_6 = RB_ROOT;
    global_cache_link_save_group_7 = RB_ROOT;
    global_cache_link_save_group_8 = RB_ROOT;
    global_cache_link_save_group_9 = RB_ROOT;

    site_builder_cache_pool = pool;
}

lg_rbtree_head *site_builder_cache_get_head(const char *base)
{

    if (strcmp(base, "title") == 0)
	return &global_cache_title_head;

    else if (strcmp(base, "keyword") == 0)
	return &global_cache_keyword_head;

    else if (strcmp(base, "content") == 0)
	return &global_cache_content_head;

    else if (strcmp(base, "meta") == 0)
	return &global_cache_meta_head;

    else if (strcmp(base, "date") == 0)
	return &global_cache_date_str_head;

    else if (strcmp(base, "time") == 0)
	return &global_cache_time_str_head;

    else if (strcmp(base, "template") == 0)
	return &global_cache_template_head;

    else if (strcmp(base, "link_save_internal") == 0)
	return &global_cache_link_save_internal_head;

    else if (strcmp(base, "link_save_external") == 0)
	return &global_cache_link_save_external_head;

    else if (strcmp(base, "link_save_bare") == 0)
	return &global_cache_link_save_bare_head;

    else if (strcmp(base, "link_save_group_all") == 0)
	return &global_cache_link_save_group_all_head;

    // group 0-9
    else if (strcmp(base, "link_save_group_0") == 0)
	return &global_cache_link_save_group_0;

    else if (strcmp(base, "link_save_group_1") == 0)
	return &global_cache_link_save_group_1;

    else if (strcmp(base, "link_save_group_2") == 0)
	return &global_cache_link_save_group_2;

    else if (strcmp(base, "link_save_group_3") == 0)
	return &global_cache_link_save_group_3;

    else if (strcmp(base, "link_save_group_4") == 0)
	return &global_cache_link_save_group_4;

    else if (strcmp(base, "link_save_group_5") == 0)
	return &global_cache_link_save_group_5;

    else if (strcmp(base, "link_save_group_6") == 0)
	return &global_cache_link_save_group_6;

    else if (strcmp(base, "link_save_group_7") == 0)
	return &global_cache_link_save_group_7;

    else if (strcmp(base, "link_save_group_8") == 0)
	return &global_cache_link_save_group_8;

    else if (strcmp(base, "link_save_group_9") == 0)
	return &global_cache_link_save_group_9;


    return NULL;
}

site_builder_element_tree_t *site_builder_cache_element_tree_add(lg_rbtree_head *head,
	char *key, int key_size, 
	const char *buffer, int buffer_size)
{

    site_builder_element_tree_t *element = ngx_pcalloc(site_builder_cache_pool, sizeof(site_builder_element_tree_t));
    if (!element) return NULL;

    element->key = ngx_pcalloc(site_builder_cache_pool, key_size + 1);
    if (!element->key) return NULL;

    memcpy(element->key, key, key_size);
    element->key_size = key_size;
    element->key[key_size] = 0;

    element->buffer = buffer;
    element->buffer_size = buffer_size;

    element->tree.index = 0;
    element->tree.key = element->key;
    if (lg_tree_element_key_add(head, &element->tree))
	return element;

    return element;
    free(element->key);
    free(element);
    return NULL;
}

site_builder_element_list_t *site_builder_cache_element_list_add(lg_rbtree_head *head,
	char *key, int key_size,
	site_builder_element_list_t *node)
{

    node->tree.key = ngx_pcalloc(site_builder_cache_pool, key_size + 1);
    if (!node->tree.key) return NULL;

    memcpy((char *)node->tree.key, key, key_size);
    ((char *)node->tree.key)[key_size] = 0;

    if (lg_tree_element_key_add(head, &node->tree))
	return node;

    return node;
    free((char *)node->tree.key);
    free(node);
    return NULL;
}

site_builder_element_tree_t *site_builder_cache_element_tree_add_r(lg_rbtree_head *head,
	char *key, int key_size, 
	const char *buffer, int buffer_size)
{
    char *p_buffer = ngx_pcalloc(site_builder_cache_pool, buffer_size + 1);
    if (!p_buffer) return NULL;

    memcpy(p_buffer, buffer, buffer_size);
    p_buffer[buffer_size] = 0;

    return site_builder_cache_element_tree_add(head, key, key_size, p_buffer, buffer_size);
}

/*************/
/* find str */
/*************/
site_builder_element_tree_t *site_builder_cache_find_str(
	const char *base,
	const char *server_location_name,
	const char *key)
{
    lg_rbtree_head *head = site_builder_cache_get_head(base);
    if (!head) return NULL;

    char cache_key[SITE_BUILDER_CACHE_KEY_SIZE];
    int cache_key_len = sprintf(cache_key, "%s%s", server_location_name, key);

    // find cache
    site_builder_element_tree_t *element = (site_builder_element_tree_t *)lg_tree_element_key_find(head, cache_key);
    if (element) return element;

    // find flush
    char *buffer = NULL;
    cache_key_len = sprintf(cache_key, "%s@%s%s", base, server_location_name, key);
    int buffer_size = site_builder_flush_load(cache_key, cache_key_len, &buffer);
    if (buffer_size <= 0 || buffer == NULL)
	return NULL;

    // save cache
    cache_key_len = sprintf(cache_key, "%s%s", server_location_name, key);
    return site_builder_cache_element_tree_add(head, cache_key, cache_key_len, buffer, buffer_size);
}

/************/
/* save str */
/************/
site_builder_element_tree_t *site_builder_cache_save_str(
	const char *base,
	const char *server_location_name,
	const char *key,
	const char *buffer, int buffer_size)
{
    lg_rbtree_head *head = site_builder_cache_get_head(base);
    if (!head) return NULL;

    char cache_key[SITE_BUILDER_CACHE_KEY_SIZE];
    int cache_key_len = sprintf(cache_key, "%s%s", server_location_name, key);

    // save cache
    site_builder_element_tree_t *element = site_builder_cache_element_tree_add(head, cache_key, cache_key_len, buffer, buffer_size);
    if (!element) return NULL;

    // save flush
    cache_key_len = sprintf(cache_key, "%s@%s%s", base, server_location_name, key);
    site_builder_flush_str(cache_key, cache_key_len, buffer, buffer_size);

    return element;
}

/**************/
/* save str-r */
/**************/
site_builder_element_tree_t *site_builder_cache_save_str_r(
	const char *base,
	const char *server_location_name,
	const char *key,
	const char *buffer, int buffer_size)
{
    char *p_buffer = ngx_pcalloc(site_builder_cache_pool, buffer_size + 1);
    if (!p_buffer) return NULL;

    memcpy(p_buffer, buffer, buffer_size);
    p_buffer[buffer_size] = 0;

    return site_builder_cache_save_str(base, server_location_name, key, p_buffer, buffer_size);
}

/***************/
/* find number */
/***************/
int site_builder_cache_find_number(
	const char *base,
	const char *server_location_name,
	const char *key)
{

    lg_rbtree_head *head = site_builder_cache_get_head(base);
    if (!head) return -1;

    char cache_key[SITE_BUILDER_CACHE_KEY_SIZE];
    int cache_key_len = sprintf(cache_key, "%s%s", server_location_name, key);

    // find cache
    site_builder_element_tree_t *element = (site_builder_element_tree_t *)lg_tree_element_key_find(head, cache_key);
    if (element && element->buffer && element->buffer_size > 0) {

	return atoi(element->buffer);
    }

    // find flush
    cache_key_len = sprintf(cache_key, "%s@%s%s", base, server_location_name, key);
    char *buffer = NULL;
    int buffer_size = site_builder_flush_load(cache_key, cache_key_len, &buffer);
    if (buffer_size <= 0 || buffer == NULL)
	return -1;

    // save cache
    cache_key_len = sprintf(cache_key, "%s%s", server_location_name, key);
    site_builder_cache_element_tree_add(head, cache_key, cache_key_len, buffer, buffer_size);
    return atoi(buffer);
}

/***************/
/* save number */
/***************/
int site_builder_cache_save_number(
	const char *base,
	const char *server_location_name,
	const char *key,
	int number)
{
    lg_rbtree_head *head = site_builder_cache_get_head(base);
    if (!head) return -1;

    char cache_key[SITE_BUILDER_CACHE_KEY_SIZE];
    int cache_key_len = sprintf(cache_key, "%s%s", server_location_name, key);

    char buffer[128];
    int buffer_size = sprintf(buffer, "%d", number);

    // save cache
    site_builder_element_tree_t *element = site_builder_cache_element_tree_add_r(head, cache_key, cache_key_len, buffer, buffer_size);
    if (!element) return -1;

    // save flush
    cache_key_len = sprintf(cache_key, "%s@%s%s", base, server_location_name, key);
    site_builder_flush_str(cache_key, cache_key_len, buffer, buffer_size);

    return 0;
}

/*************/
/* find list */
/*************/
site_builder_element_list_t *site_builder_cache_find_list(
	const char *base,
	const char *server_location_name,
	const char *key)
{

    lg_rbtree_head *head = site_builder_cache_get_head(base);
    if (!head) return NULL;

    char cache_key[SITE_BUILDER_CACHE_KEY_SIZE];
    int cache_key_len = sprintf(cache_key, "%s%s", server_location_name, key);

    site_builder_element_list_t *element_list = (site_builder_element_list_t *)lg_tree_element_key_find(head, cache_key);
    if (element_list) return element_list;

    char *buffer = NULL;
    cache_key_len = sprintf(cache_key, "%s@%s%s", base, server_location_name, key);
    int buffer_size = site_builder_flush_load(cache_key, cache_key_len, &buffer);
    if (buffer_size <= 0 || buffer == NULL)
	return NULL;

    // create cache list head
    element_list = ngx_pcalloc(site_builder_cache_pool, sizeof(site_builder_element_list_t));
    if (!element_list) return NULL;
    ngx_queue_init(&element_list->list_head);

    char *saveptr = NULL;
    char *str = strtok_r(buffer, "\r\n", &saveptr);
    while(str) {

	site_builder_element_str_t *node = ngx_pcalloc(site_builder_cache_pool, sizeof(site_builder_element_str_t));
	if (node) {

	    node->buffer = str;
	    node->buffer_size = strlen(str);
	    ngx_queue_insert_tail(&element_list->list_head, &node->next);
	}

	str = strtok_r(NULL, "\r\n", &saveptr);
    }

    // save cache
    cache_key_len = sprintf(cache_key, "%s%s", server_location_name, key);
    site_builder_cache_element_list_add(head, cache_key, cache_key_len, element_list);
    return element_list;
}

/*************/
/* save list */
/*************/
site_builder_element_list_t *site_builder_cache_save_list(
	const char *base,
	const char *server_location_name,
	const char *key,
	ngx_queue_t *list_head)
{
    lg_rbtree_head *head = site_builder_cache_get_head(base);
    if (!head) return NULL;

    char cache_key[SITE_BUILDER_CACHE_KEY_SIZE];
    int cache_key_len = sprintf(cache_key, "%s%s", server_location_name, key);

    // create cache list head
    site_builder_element_list_t *element_list = ngx_pcalloc(site_builder_cache_pool, sizeof(site_builder_element_list_t));
    if (!element_list) return NULL;
    ngx_queue_init(&element_list->list_head);

    ngx_queue_t *p = NULL;
    for (p = ngx_queue_head(list_head);
	    p != ngx_queue_sentinel(list_head);
	    p = ngx_queue_next(p)) {

	site_builder_element_str_t *node = (site_builder_element_str_t *)p;
	site_builder_element_str_t *new_node = ngx_pcalloc(site_builder_cache_pool, sizeof(site_builder_element_str_t));
	if (new_node) {

	    new_node->buffer = node->buffer;
	    new_node->buffer_size = node->buffer_size;
	    ngx_queue_insert_tail(&element_list->list_head, &new_node->next);
	}
    }

    // save cache
    site_builder_cache_element_list_add(head, cache_key, cache_key_len, element_list);

    // save flush
    cache_key_len = sprintf(cache_key, "%s@%s%s", base, server_location_name, key);
    site_builder_flush_list(cache_key, cache_key_len, list_head);

    return element_list;
}

/***************/
/* find list KV*/
/***************/
site_builder_element_list_t *site_builder_cache_find_list_key_value(
	const char *base,
	const char *server_location_name,
	const char *key)
{


    lg_rbtree_head *head = site_builder_cache_get_head(base);
    if (!head) return NULL;

    char cache_key[SITE_BUILDER_CACHE_KEY_SIZE];
    int cache_key_len = sprintf(cache_key, "%s%s", server_location_name, key);

    site_builder_element_list_t *element_list = (site_builder_element_list_t *)lg_tree_element_key_find(head, cache_key);
    if (element_list) return element_list;

    char *buffer = NULL;
    cache_key_len = sprintf(cache_key, "%s@%s%s", base, server_location_name, key);
    int buffer_size = site_builder_flush_load(cache_key, cache_key_len, &buffer);
    if (buffer_size <= 0 || buffer == NULL)
	return NULL;

    // create cache list head
    element_list = ngx_pcalloc(site_builder_cache_pool, sizeof(site_builder_element_list_t));
    if (!element_list) return NULL;
    ngx_queue_init(&element_list->list_head);

    char *saveptr = NULL;
    char *str = strtok_r(buffer, "\r\n", &saveptr);
    while(str) {

	char *value_saveptr = NULL;
	char *value = strtok_r(str, "\t", &value_saveptr);
	if (value) {

	    value = strtok_r(NULL, "\t", &value_saveptr);
	    if (value) {

		site_builder_element_key_value_t *node = ngx_pcalloc(site_builder_cache_pool, sizeof(site_builder_element_key_value_t));
		if (node) {

		    node->key = str;
		    node->key_size = strlen(str);
		    node->value = value;
		    node->value_size = strlen(value);
		    ngx_queue_insert_tail(&element_list->list_head, &node->next);
		    // fprintf(stdout, "[find][%s][%s]\t\t\t\t[%s]\n", cache_key, node->key, node->value);
		}
	    }
	}

	str = strtok_r(NULL, "\r\n", &saveptr);
    }

    cache_key_len = sprintf(cache_key, "%s%s", server_location_name, key);
    site_builder_cache_element_list_add(head, cache_key, cache_key_len, element_list);
    return element_list;
}

/***************/
/* save list KV*/
/***************/
site_builder_element_list_t *site_builder_cache_save_list_key_value(
	const char *base,
	const char *server_location_name,
	const char *key,
	ngx_queue_t *list_head)
{

    lg_rbtree_head *head = site_builder_cache_get_head(base);
    if (!head) return NULL;

    char cache_key[SITE_BUILDER_CACHE_KEY_SIZE];
    int cache_key_len = sprintf(cache_key, "%s%s", server_location_name, key);

    // create cache list head
    site_builder_element_list_t *element_list = ngx_pcalloc(site_builder_cache_pool, sizeof(site_builder_element_list_t));
    if (!element_list) return NULL;
    ngx_queue_init(&element_list->list_head);

    ngx_queue_t *p = NULL;
    for (p = ngx_queue_head(list_head);
	    p != ngx_queue_sentinel(list_head);
	    p = ngx_queue_next(p)) {

	site_builder_element_key_value_t *node = (site_builder_element_key_value_t *)p;
	site_builder_element_key_value_t *new_node = ngx_pcalloc(site_builder_cache_pool, sizeof(site_builder_element_key_value_t));
	if (new_node) {

	    new_node->key = node->key;
	    new_node->key_size = node->key_size;
	    new_node->value = node->value;
	    new_node->value_size = node->value_size;
	    ngx_queue_insert_tail(&element_list->list_head, &new_node->next);
	    // fprintf(stdout, "[cache save][%s][%s][%s]\n", base, cache_key, node->value);
	}
    }

    // save cache
    site_builder_cache_element_list_add(head, cache_key, cache_key_len, element_list);

    // save flush
    cache_key_len = sprintf(cache_key, "%s@%s%s", base, server_location_name, key);
    site_builder_flush_list_key_value(cache_key, cache_key_len, list_head);

    return element_list;
}



