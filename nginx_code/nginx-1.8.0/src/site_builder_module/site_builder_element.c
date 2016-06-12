
#include "site_builder_element.h"

void site_builder_element_init(site_builder_element_head_t *p, char *name, ngx_pool_t *pool)
{
    p->name = name;
    p->head = RB_ROOT;
    p->pool = pool;
    p->total = 0;
}

site_builder_element_node_t *site_builder_element_key_find(site_builder_element_head_t *p,  char *key)
{

    return (site_builder_element_node_t *)lg_tree_element_key_find(&p->head, key);
}

site_builder_element_node_t *site_builder_element_index_find(site_builder_element_head_t *p, int index)
{

    return (site_builder_element_node_t *)lg_tree_element_index_find(&p->head, index);
}

site_builder_element_node_t *site_builder_element_index_rand(site_builder_element_head_t *p)
{

    if (!p || p->total <= 0) return NULL;

    int index = rand() % p->total;
    return (site_builder_element_node_t *)lg_tree_element_index_find(&p->head, index);
}

site_builder_element_node_t *site_builder_element_key_add(site_builder_element_head_t *p,
	char *key, int key_size,
	char *buffer, int buffer_size)
{

    if (!p) return NULL;

    site_builder_element_node_t *element = ngx_pcalloc(p->pool, sizeof(site_builder_element_node_t));
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

site_builder_element_node_t *site_builder_element_key_add_r(site_builder_element_head_t *p,
	char *key, int key_size,
	char *buffer, int buffer_size)
{

    char *p_buffer = ngx_pcalloc(p->pool, buffer_size + 1);
    if (!p_buffer) return NULL;

    memcpy(p_buffer, buffer, buffer_size);
    p_buffer[buffer_size] = 0;
    return site_builder_element_key_add(p, key, key_size, p_buffer, buffer_size);
}

site_builder_element_node_t *site_builder_element_index_add(site_builder_element_head_t *p,
	char *key, int key_size,
	char *buffer, int buffer_size)
{

    site_builder_element_node_t *element = ngx_pcalloc(p->pool, sizeof(site_builder_element_node_t));
    if (!element) return NULL;

    element->key = key;
    element->key_size = key_size;

    element->buffer = buffer;
    element->buffer_size = buffer_size;

    element->tree.key = NULL;
    element->tree.index = p->total++;
    lg_tree_element_index_add(&p->head, &element->tree);
    return element;
}

site_builder_element_node_t *site_builder_element_index_add_r(site_builder_element_head_t *p,
	char *key, int key_size,
	char *buffer, int buffer_size)
{

    char *p_key = NULL;
    char *p_buffer = NULL;

    if (key && key_size > 0) {

	p_key = ngx_pcalloc(p->pool, key_size + 1);
	if (!p_key) return NULL;

	memcpy(p_key, key, key_size);
	p_key[key_size] = 0;
    }

    if (buffer && buffer_size > 0) {

	p_buffer = ngx_pcalloc(p->pool, buffer_size + 1);
	if (!p_buffer) return NULL;

	memcpy(p_buffer, buffer, buffer_size);
	p_buffer[buffer_size] = 0;
    }

    return site_builder_element_index_add(p, p_key, key_size, p_buffer, buffer_size);
}


