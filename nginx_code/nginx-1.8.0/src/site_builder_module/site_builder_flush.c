
#include "site_builder_flush.h"
#include "lg_leveldb.h"

typedef struct {

    lg_rbtree_node tree;
    void *db;

} site_builder_flush_base_t;

lg_rbtree_head site_builder_flush_base_head = RB_ROOT;
ngx_pool_t *site_builder_flush_base_pool = NULL;

void site_builder_flush_init_base(ngx_pool_t *pool)
{
    site_builder_flush_base_head = RB_ROOT;
    site_builder_flush_base_pool = pool;
}

void *site_builder_flush_open_base(char *base)
{
    char path[256];
    sprintf(path, "%s/%s", SITE_BUILDER_FLUSH_HOME_PATH, base);

    char *str = NULL;
    for (str = path + 1; *str; str++) {

	if (*str == '/') {

	    *str = 0;
	    if (access(path, 0)) {

		// fprintf(stdout, "mkdir::%s\n", path);
		if (mkdir(path, S_IRWXU | S_IRWXG | S_IRWXO) != 0)
		    return NULL;
	    }

	    *str = '/';
	}
    }

    return lg_leveldb_open(path);
}

void *site_builder_flush_get_base(char *base)
{

    site_builder_flush_base_t *p = (site_builder_flush_base_t *)lg_tree_element_key_find(&site_builder_flush_base_head, base);
    if (p) {

	// fprintf(stdout, "db=%p::%s\n", p->db, p->tree.key);
	return p->db;
    }

    p = ngx_pcalloc(site_builder_flush_base_pool, sizeof(site_builder_flush_base_t));
    if (!p) return NULL;

    int base_len = strlen(base);
    p->tree.key = ngx_pcalloc(site_builder_flush_base_pool, strlen(base));
    if (!p->tree.key) return NULL;

    if (!(p->db = site_builder_flush_open_base(base)))
	return NULL;

    memcpy(p->tree.key, base, base_len);
    lg_tree_element_key_add(&site_builder_flush_base_head, &p->tree);
    // fprintf(stdout, "db=%p::%s\n", p->db, p->tree.key);
    return p->db;
}

int site_builder_flush_find_str(char *row_key, char *base, char *key, char **buffer, ngx_pool_t *pool)
{

    // fprintf(stdout, "[find][%s][%s][%s]\n", row_key, base, key);
    void *db = site_builder_flush_get_base(base);
    if (!db) return -1;

    char flush_key[1024];
    sprintf(flush_key, "%s@%s", row_key, key);
    return lg_leveldb_get(db, flush_key, buffer, pool);
}

int site_builder_flush_save_str(char *row_key, char *base, char *key, char *buffer, int buffer_size)
{

    // fprintf(stdout, "[save][%s][%s][%s]\n", row_key, base, key);
    void *db = site_builder_flush_get_base(base);
    if (!db) return -1;

    char flush_key[1024];
    sprintf(flush_key, "%s@%s", row_key, key);
    return lg_leveldb_set(db, flush_key, buffer);
}

int site_builder_flush_find_list(char *row_key, char *base, char *key, ngx_queue_t *queue, ngx_pool_t *pool)
{

    char *buffer = NULL;
    int buffer_size = site_builder_flush_find_str(row_key, base, key, &buffer, pool);
    if (!buffer && buffer_size <= 0) return -1;
    // fprintf(stdout, "[find][%s][%s][%s][%d][%s]\n", row_key, base, key, buffer_size, buffer);

    int list_count = 0;
    char *saveptr = NULL;
    char *str = strtok_r(buffer, "\r\n", &saveptr);
    while(str) {

	site_builder_element_str_t *node = ngx_pcalloc(pool, sizeof(site_builder_element_str_t));
	if (!node) return -1;

	node->buffer = str;
	node->buffer_size = strlen(str);
	ngx_queue_insert_tail(queue, &node->next);
	// fprintf(stdout, "[find][%s][%s][%s][%s]\n", row_key, base, key, str);

	list_count++;
	str = strtok_r(NULL, "\r\n", &saveptr);
    }

    return list_count;
}

int site_builder_flush_save_list(char *row_key, char *base, char *key, ngx_queue_t *queue, ngx_pool_t *pool)
{

    ngx_queue_t *p = NULL;

    // Calculate buffer size
    int buffer_size = 0;
    for (p = ngx_queue_head(queue); 
	    p != ngx_queue_sentinel(queue); 
	    p = ngx_queue_next(p)) {

	site_builder_element_str_t *node = (site_builder_element_str_t *)p;
	buffer_size += node->buffer_size + 1; //\n
    }

    // buffer malloc
    char *buffer = ngx_pcalloc(pool, buffer_size + 1);
    if (!buffer) return -1;

    // copy buffer
    int size = 0;
    for (p = ngx_queue_head(queue); 
	    p != ngx_queue_sentinel(queue); 
	    p = ngx_queue_next(p)) {

	site_builder_element_str_t *node = (site_builder_element_str_t *)p;
	memcpy(&buffer[size], node->buffer, node->buffer_size);
	size += node->buffer_size;

	memcpy(&buffer[size], "\n", 1);
	size++;
    }

    buffer[buffer_size] = 0;
    return site_builder_flush_save_str(row_key, base, key, buffer, buffer_size);
}



