
#include "lg_string.h"
#include "site_builder_page.h"
#include "site_builder_cache.h"
#include "site_builder_flush.h"

#if 0
int site_builder_page_load_template(ngx_http_request_t *r,
	site_builder_loc_conf_t *loc_conf, 
	site_builder_page_info_t *page_info)
{

    // find _template
    char *template_name = "content.html";
    site_builder_uri_t *uri = &page_info->uri;
    if (strcmp(uri->key, "/") == 0)
	template_name = "index.html";

    ngx_queue_t *p = NULL;
    for (p = ngx_queue_head(&loc_conf->template_list.head); 
	    p != ngx_queue_sentinel(&loc_conf->template_list.head); 
	    p = ngx_queue_next(p)) {

	site_builder_template_t *node = ngx_queue_data(p, site_builder_template_t, next);
	// fprintf(stdout, "[%s][%s]\n", node->template_name, template_name);
	if (strcmp(node->template_name, template_name) == 0) {

	    page_info->template = node;

	    // return template
	    return 0;
	}
    }

    return -1;
}

#else

int site_builder_page_load_template(ngx_http_request_t *r,
	site_builder_loc_conf_t *loc_conf, 
	site_builder_page_info_t *page_info)
{

    // find _template
    site_builder_uri_t *uri = &page_info->uri;
    int template_id = site_builder_cache_find_number(
	    "template",
	    loc_conf->server_location_name,
	    uri->key);

    if (template_id < 0) {

	// rand _template
	template_id = rand() % loc_conf->template_list.template_total;
	site_builder_cache_save_number(
		"template",
		loc_conf->server_location_name,
		uri->key,
		template_id);
    }

    ngx_queue_t *p = NULL;
    for (p = ngx_queue_head(&loc_conf->template_list.head); 
	    p != ngx_queue_sentinel(&loc_conf->template_list.head); 
	    p = ngx_queue_next(p)) {

	site_builder_template_t *node = ngx_queue_data(p, site_builder_template_t, next);
	if (template_id == node->template_id) {

	    page_info->template = node;
	    return 0;
	}
    }

    // not exist template
    template_id = rand() % loc_conf->template_list.template_total;
    template_id = rand() % loc_conf->template_list.template_total;
    site_builder_cache_save_number(
	    "template",
	    loc_conf->server_location_name,
	    uri->key,
	    template_id);

    for (p = ngx_queue_head(&loc_conf->template_list.head); 
	    p != ngx_queue_sentinel(&loc_conf->template_list.head); 
	    p = ngx_queue_next(p)) {

	site_builder_template_t *node = ngx_queue_data(p, site_builder_template_t, next);

	if (template_id == node->template_id) {

	    page_info->template = node;
	    return 0;
	}
    }

    return -1;
}

#endif

int site_builder_page_load_title(ngx_http_request_t *r,
	site_builder_loc_conf_t *loc_conf, 
	site_builder_page_info_t *page_info)
{

    // find title
    site_builder_uri_t *uri = &page_info->uri;
    site_builder_element_tree_t *element = site_builder_cache_find_str(
	    "title",
	    loc_conf->server_location_name,
	    uri->key);
    if (element) {

	page_info->title_str = element->buffer;
	page_info->title_size = element->buffer_size;
	return 0;
    }

    // rand title
    element = site_builder_element_index_rand(&loc_conf->keyword);
    if (!element) return -1;

    // save title
    site_builder_cache_save_str(
	    "title",
	    loc_conf->server_location_name,
	    uri->key,
	    element->buffer, element->buffer_size);

    page_info->title_str = element->buffer;
    page_info->title_size = element->buffer_size;
    return 0;
}

int site_builder_page_load_keyword(ngx_http_request_t *r,
	site_builder_loc_conf_t *loc_conf, 
	site_builder_page_info_t *page_info)
{

    // find keyword
    site_builder_uri_t *uri = &page_info->uri;
    site_builder_element_tree_t *element = site_builder_cache_find_str(
	    "keyword",
	    loc_conf->server_location_name,
	    uri->key);
    if (element) {

	page_info->keyword_str = element->buffer;
	page_info->keyword_size = element->buffer_size;
	return 0;
    }

    // rand keyword
    element = site_builder_element_index_rand(&loc_conf->keyword);
    if (!element) return -1;

    // save keyword
    site_builder_cache_save_str(
	    "keyword",
	    loc_conf->server_location_name,
	    uri->key,
	    element->buffer, element->buffer_size);

    page_info->keyword_str = element->buffer;
    page_info->keyword_size = element->buffer_size;
    return 0;
}

int site_builder_page_load_content(ngx_http_request_t *r,
	site_builder_loc_conf_t *loc_conf, 
	site_builder_page_info_t *page_info)
{

    // find content
    site_builder_uri_t *uri = &page_info->uri;
    site_builder_element_list_t *element_list = site_builder_cache_find_list(
	    "content",
	    loc_conf->server_location_name,
	    uri->key);
    if (element_list) {

	ngx_queue_t *p = NULL;
	for (p = ngx_queue_head(&element_list->list_head);
		p != ngx_queue_sentinel(&element_list->list_head);
		p = ngx_queue_next(p)) {

	    site_builder_element_str_t *cache_node = (site_builder_element_str_t *)p;
	    site_builder_element_str_t *node = ngx_pcalloc(r->pool, sizeof(site_builder_element_str_t));
	    if (cache_node && node) {

		node->buffer = cache_node->buffer;
		node->buffer_size = cache_node->buffer_size;
		ngx_queue_insert_tail(&page_info->content, &node->next);
	    }
	}

	return 0;
    }

    // rand content
    int i, max = rand() % 6 + 10;
    for (i = 0; i < max; i++) {

	site_builder_element_tree_t *element = site_builder_element_index_rand(&loc_conf->content);
	site_builder_element_str_t *node = ngx_pcalloc(r->pool, sizeof(site_builder_element_str_t));
	if (element && node && element->buffer_size > 1) {

	    node->buffer = element->buffer;
	    node->buffer_size = element->buffer_size;
	    ngx_queue_insert_tail(&page_info->content, &node->next);
	    // fprintf(stdout, "[%d][%s]\n", i, node->buffer);
	}
    }

    // empty content
    if (ngx_queue_empty(&page_info->content)) {

	return -1;
    }

    // save content
    site_builder_cache_save_list(
	    "content",
	    loc_conf->server_location_name,
	    uri->key,
	    &page_info->content);
    return 0;
}

int site_builder_page_load_meta(ngx_http_request_t *r,
	site_builder_loc_conf_t *loc_conf, 
	site_builder_page_info_t *page_info)
{

    // find meta
    site_builder_uri_t *uri = &page_info->uri;
    site_builder_element_list_t *element_list = site_builder_cache_find_list(
	    "meta",
	    loc_conf->server_location_name,
	    uri->key);
    if (element_list) {

	ngx_queue_t *p = NULL;
	for (p = ngx_queue_head(&element_list->list_head);
		p != ngx_queue_sentinel(&element_list->list_head);
		p = ngx_queue_next(p)) {

	    site_builder_element_str_t *cache_node = (site_builder_element_str_t *)p;
	    site_builder_element_str_t *content_node = ngx_pcalloc(r->pool, sizeof(site_builder_element_str_t));
	    if (cache_node && content_node) {

		content_node->buffer = cache_node->buffer;
		content_node->buffer_size = cache_node->buffer_size;
		ngx_queue_insert_tail(&page_info->meta, &content_node->next);
	    }
	}

	return 0;
    }

    // rand meta
    int i;//, max = rand() % 6 + 10;
    for (i = 0; i < 2; i++) {

	site_builder_element_tree_t *element = site_builder_element_index_rand(&loc_conf->meta);
	site_builder_element_str_t *node = ngx_pcalloc(r->pool, sizeof(site_builder_element_str_t));
	if (element && node && element->buffer_size > 1) {

	    node->buffer = element->buffer;
	    node->buffer_size = element->buffer_size;
	    ngx_queue_insert_tail(&page_info->meta, &node->next);
	}
    }

    // empty meta
    if (ngx_queue_empty(&page_info->meta)) {

	return -1;
    }

    // save meta
    site_builder_cache_save_list(
	    "meta",
	    loc_conf->server_location_name,
	    uri->key,
	    &page_info->meta);

    return 0;
}

int site_builder_page_load_content_rand(ngx_http_request_t *r,
	site_builder_loc_conf_t *loc_conf, 
	site_builder_page_info_t *page_info)
{

    // rand
    int n;
    for (n = 0; n < page_info->template->content_rand_total; n++) {

	site_builder_element_list_next_t *list = ngx_pcalloc(r->pool, sizeof(site_builder_element_list_next_t));
	if (list) {

	    ngx_queue_init(&list->head);

	    int i, max = 2;//rand() % 6 + 10;
	    for (i = 0; i < max; i++) {

		site_builder_element_tree_t *element = site_builder_element_index_rand(&loc_conf->content);
		site_builder_element_str_t *node = ngx_pcalloc(r->pool, sizeof(site_builder_element_str_t));
		if (element && node && element->buffer_size > 1) {

		    node->buffer = element->buffer;
		    node->buffer_size = element->buffer_size;
		    ngx_queue_insert_tail(&list->head, &node->next);
		}
	    }

	    ngx_queue_insert_tail(&page_info->content_rand, &list->next);
	}
    }

    return 0;
}

int site_builder_page_load_date(ngx_http_request_t *r,
	site_builder_loc_conf_t *loc_conf, 
	site_builder_page_info_t *page_info)
{

    // find date str
    site_builder_uri_t *uri = &page_info->uri;
    site_builder_element_tree_t *element = site_builder_cache_find_str(
	    "date",
	    loc_conf->server_location_name,
	    uri->key);
    if (element) {

	page_info->date_str = element->buffer;
	page_info->date_str_size = element->buffer_size;
	return 0;
    }

    // create date_str
    struct tm tm;
    time_t t = time(NULL);
    localtime_r(&t, &tm);

    char buffer[128];
    int buffer_size = sprintf(buffer, "%04d-%02d-%02d",
	    tm.tm_year + 1900,
	    tm.tm_mon + 1,
	    tm.tm_mday);

    // save date str
    element = site_builder_cache_save_str_r(
	    "date",
	    loc_conf->server_location_name,
	    uri->key,
	    buffer, buffer_size);
    if (!element) return -1;

    page_info->date_str = element->buffer;
    page_info->date_str_size = element->buffer_size;
    return 0;
}

int site_builder_page_load_time(ngx_http_request_t *r,
	site_builder_loc_conf_t *loc_conf, 
	site_builder_page_info_t *page_info)
{

    // find time_str
    site_builder_uri_t *uri = &page_info->uri;
    site_builder_element_tree_t *element = site_builder_cache_find_str(
	    "time",
	    loc_conf->server_location_name,
	    uri->key);
    if (element) {

	page_info->time_str = element->buffer;
	page_info->time_str_size = element->buffer_size;
	return 0;
    }

    // create time_str
    struct tm tm;
    time_t t = time(NULL);
    localtime_r(&t, &tm);

    char buffer[128];
    int buffer_size = sprintf(buffer, "%04d-%02d-%02d %02d:%02d:%02d",
	    tm.tm_year + 1900,
	    tm.tm_mon + 1,
	    tm.tm_mday,
	    tm.tm_hour,
	    tm.tm_min,
	    tm.tm_sec);

    // save time_str
    element = site_builder_cache_save_str_r(
	    "time",
	    loc_conf->server_location_name,
	    uri->key,
	    buffer, buffer_size);
    if (!element) return -1;

    page_info->time_str = element->buffer;
    page_info->time_str_size = element->buffer_size;
    return 0;
}

// link_cat_title
site_builder_element_tree_t *site_builder_page_careate_link_cat_title(ngx_http_request_t *r,
	site_builder_loc_conf_t *loc_conf, 
	site_builder_page_info_t *page_info,
	const char *server_location_name,
	const char *key)
{

    // find title
    site_builder_element_tree_t *element = site_builder_cache_find_str(
	    "title",
	    server_location_name,
	    key);
    if (element) return element;

    // find server_location_name keyword
    site_builder_element_head_tree_t *keyword = NULL;
    if (strcmp(server_location_name, loc_conf->server_location_name) == 0) {

	keyword = &loc_conf->keyword;

    } else {

	ngx_queue_t *p = NULL;
	for (p = ngx_queue_head(loc_conf->global_keyword_list); 
		p != ngx_queue_sentinel(loc_conf->global_keyword_list); 
		p = ngx_queue_next(p)) {

	    site_builder_element_head_tree_t *node = (site_builder_element_head_tree_t *)p;
	    // fprintf(stdout, "==>[%s][%s]\n", server_location_name, node->name);
	    if (strcmp(server_location_name, node->name) == 0) {

		keyword = node;
		break;
	    }
	}
    }

    if (!keyword) return NULL;

    // rand title
    element = site_builder_element_index_rand(keyword);
    if (!element) return NULL;

    // save title
    element = site_builder_cache_save_str(
	    "title",
	    server_location_name,
	    key,
	    element->buffer, element->buffer_size);
    //    fprintf(stdout, "-->%p\n", element->buffer);
    return element;
}

/* save */
int site_builder_page_careate_link_save_internal(ngx_http_request_t *r,
	site_builder_loc_conf_t *loc_conf,
	site_builder_page_info_t *page_info)
{

    // find link_save_internal
    site_builder_uri_t *uri = &page_info->uri;
    site_builder_element_list_t *element_list = site_builder_cache_find_list_key_value(
	    "link_save_internal",
	    loc_conf->server_location_name,
	    uri->key);
    if (element_list) {

	ngx_queue_t *p = NULL;
	for (p = ngx_queue_head(&element_list->list_head);
		p != ngx_queue_sentinel(&element_list->list_head);
		p = ngx_queue_next(p)) {

	    site_builder_element_key_value_t *cache_node = (site_builder_element_key_value_t *)p;
	    site_builder_element_key_value_t *node = ngx_pcalloc(r->pool, sizeof(site_builder_element_key_value_t));
	    if (cache_node && node) {

		node->key = cache_node->key;
		node->key_size = cache_node->key_size;
		node->value = cache_node->value;
		node->value_size = cache_node->value_size;
		ngx_queue_insert_tail(&page_info->link_save_internal, &node->next);
	    }
	}

	return 0;
    }

    // rand link_save_internal
    int i;
    char key[64], rand_buf[20];
    for (i = 0; i < page_info->template->link_save_internal_total; i++) {

	lg_string_rand_number(rand_buf, rand() % 9 + 4);
	sprintf(key, "/%s.html", rand_buf);

	site_builder_element_key_value_t *node = ngx_pcalloc(r->pool, sizeof(site_builder_element_key_value_t));
	site_builder_element_tree_t *element = site_builder_page_careate_link_cat_title(r,
		loc_conf,
		page_info,
		loc_conf->server_location_name,
		key);
	if (element && node) {

	    node->key = element->key;
	    node->key_size = element->key_size;
	    node->value = element->buffer;
	    node->value_size = element->buffer_size;
	    ngx_queue_insert_tail(&page_info->link_save_internal, &node->next);
	}
    }

    // empty link_save_internal
    if (ngx_queue_empty(&page_info->link_save_internal)) {

	return -1;
    }

    // save link_save_internal
    site_builder_cache_save_list_key_value(
	    "link_save_internal",
	    loc_conf->server_location_name,
	    uri->key,
	    &page_info->link_save_internal);
    return 0;
}

int site_builder_page_careate_link_save_external(ngx_http_request_t *r,
	site_builder_loc_conf_t *loc_conf,
	site_builder_page_info_t *page_info)
{
    // find 
    site_builder_uri_t *uri = &page_info->uri;
    site_builder_element_list_t *element_list = site_builder_cache_find_list(
	    "link_save_external",
	    loc_conf->server_location_name,
	    uri->key);
    if (element_list) {

	ngx_queue_t *p = NULL;
	for (p = ngx_queue_head(&element_list->list_head);
		p != ngx_queue_sentinel(&element_list->list_head);
		p = ngx_queue_next(p)) {

	    site_builder_element_str_t *cache_node = (site_builder_element_str_t *)p;
	    site_builder_element_str_t *node = ngx_pcalloc(r->pool, sizeof(site_builder_element_str_t));
	    if (cache_node && node) {

		node->buffer = cache_node->buffer;
		node->buffer_size = cache_node->buffer_size;
		ngx_queue_insert_tail(&page_info->link_save_external, &node->next);
	    }
	}

	return 0;
    }

    int i;
    for (i = 0; i < page_info->template->link_save_external_total; i++) {

	// rand 
	site_builder_element_str_t *node = ngx_pcalloc(r->pool, sizeof(site_builder_element_str_t));
	site_builder_element_tree_t *element = site_builder_element_index_rand(&loc_conf->link_external);
	if (element && node) {

	    node->buffer = element->buffer;
	    node->buffer_size = element->buffer_size;
	    ngx_queue_insert_tail(&page_info->link_save_external, &node->next);
	}
    }

    // empty
    if (ngx_queue_empty(&page_info->link_save_external)) {

	return -1;
    }

    // save
    site_builder_cache_save_list(
	    "link_save_external",
	    loc_conf->server_location_name,
	    uri->key,
	    &page_info->link_save_external);

    return 0;
}

int site_builder_page_careate_link_save_subdomain(ngx_http_request_t *r,
	site_builder_loc_conf_t *loc_conf,
	site_builder_page_info_t *page_info)
{

    // rand 
    int i;
    char key[256], rand_buf[20];
    for (i = 0; i < page_info->template->link_save_subdomain_total; i++) {

	site_builder_element_tree_t *keyword = site_builder_element_index_rand(&loc_conf->keyword);
	site_builder_element_tree_t *domain = site_builder_element_index_rand(&loc_conf->link_subdomain);
	site_builder_element_key_value_t *node = ngx_pcalloc(r->pool, sizeof(site_builder_element_key_value_t));

	lg_string_rand_keyword(rand_buf, rand() % 6 + 2);
	int key_size = sprintf(key, "%s.%s", rand_buf, domain->buffer);
	char *_key = ngx_pcalloc(r->pool, key_size);

	if (keyword && domain && node) {

	    memcpy(_key, key, key_size);
	    _key[key_size] = 0;

	    node->key = _key;
	    node->key_size = key_size;
	    node->value = keyword->buffer;
	    node->value_size = keyword->buffer_size;
	    ngx_queue_insert_tail(&page_info->link_save_subdomain, &node->next);
	}
    }

    // empty link_save_internal
    if (ngx_queue_empty(&page_info->link_save_subdomain)) {

	return -1;
    }
#if 0
    // save link_save_internal
    site_builder_cache_save_list_key_value(
	    "link_save_internal",
	    loc_conf->server_location_name,
	    uri->key,
	    &page_info->link_save_internal);
#endif
    return 0;
}

int site_builder_page_careate_link_save_bare(ngx_http_request_t *r,
	site_builder_loc_conf_t *loc_conf,
	site_builder_page_info_t *page_info)
{
    // find 
    site_builder_uri_t *uri = &page_info->uri;
    site_builder_element_list_t *element_list = site_builder_cache_find_list(
	    "link_save_bare",
	    loc_conf->server_location_name,
	    uri->key);
    if (element_list) {

	ngx_queue_t *p = NULL;
	for (p = ngx_queue_head(&element_list->list_head);
		p != ngx_queue_sentinel(&element_list->list_head);
		p = ngx_queue_next(p)) {

	    site_builder_element_str_t *cache_node = (site_builder_element_str_t *)p;
	    site_builder_element_str_t *node = ngx_pcalloc(r->pool, sizeof(site_builder_element_str_t));
	    if (cache_node && node) {

		node->buffer = cache_node->buffer;
		node->buffer_size = cache_node->buffer_size;
		ngx_queue_insert_tail(&page_info->link_save_bare, &node->next);
	    }
	}

	return 0;
    }

    int i;
    for (i = 0; i < page_info->template->link_save_bare_total; i++) {

	// rand 
	site_builder_element_str_t *node = ngx_pcalloc(r->pool, sizeof(site_builder_element_str_t));
	site_builder_element_tree_t *element = site_builder_element_index_rand(&loc_conf->link_bare);
	if (element && node) {

	    node->buffer = element->buffer;
	    node->buffer_size = element->buffer_size;
	    ngx_queue_insert_tail(&page_info->link_save_bare, &node->next);
	}
    }

    // empty
    if (ngx_queue_empty(&page_info->link_save_bare)) {

	return -1;
    }

    // save
    site_builder_cache_save_list(
	    "link_save_bare",
	    loc_conf->server_location_name,
	    uri->key,
	    &page_info->link_save_bare);

    return 0;
}

int site_builder_page_careate_link_save_group(ngx_http_request_t *r,
	site_builder_loc_conf_t *loc_conf,
	site_builder_page_info_t *page_info)
{

    int i;
    char key[64], rand_buf[20], link_group_base[32];

    // group array next
    ngx_queue_t *group_p = NULL;
    for (group_p = ngx_queue_head(&loc_conf->link_group_config); 
	    group_p != ngx_queue_sentinel(&loc_conf->link_group_config); 
	    group_p = ngx_queue_next(group_p)) {

	link_group_config_t *guoup_node = (link_group_config_t *)group_p;
	sprintf(link_group_base, "link_save_group_%d", guoup_node->index);
	// fprintf(stdout, "[%s][%s]\n", link_group_base, guoup_node->link_group_name);

	// fprintf(stdout, "[template][%s][%d]\n", link_group_base, page_info->template->link_save_group_total[guoup_node->index]);
	if (page_info->template->link_save_group_total[guoup_node->index] > 0) {

	    // find link_group_XX
	    site_builder_uri_t *uri = &page_info->uri;
	    site_builder_element_list_t *element_list = site_builder_cache_find_list_key_value(
		    link_group_base,
		    loc_conf->server_location_name,
		    uri->key);
	    if (element_list) {

		ngx_queue_t *p = NULL;
		for (p = ngx_queue_head(&element_list->list_head);
			p != ngx_queue_sentinel(&element_list->list_head);
			p = ngx_queue_next(p)) {

		    site_builder_element_key_value_t *cache_node = (site_builder_element_key_value_t *)p;
		    site_builder_element_key_value_t *node = ngx_pcalloc(r->pool, sizeof(site_builder_element_key_value_t));
		    if (cache_node && node) {

			node->key = cache_node->key;
			node->key_size = cache_node->key_size;
			node->value = cache_node->value;
			node->value_size = cache_node->value_size;
			ngx_queue_insert_tail(&page_info->link_save_group[guoup_node->index], &node->next);
		    }
		}

	    } else {

		// rand 
		for (i = 0; i < page_info->template->link_save_group_total[guoup_node->index]; i++) {

		    lg_string_rand_number(rand_buf, rand() % 9 + 4);
		    sprintf(key, "/%s.html", rand_buf);

		    site_builder_element_key_value_t *node = ngx_pcalloc(r->pool, sizeof(site_builder_element_key_value_t));
		    site_builder_element_tree_t *element = site_builder_page_careate_link_cat_title(r,
			    loc_conf,
			    page_info,
			    guoup_node->link_group_name,
			    key);

		    if (element && node) {

			node->key = element->key;
			node->key_size = element->key_size;
			node->value = element->buffer;
			node->value_size = element->buffer_size;
			ngx_queue_insert_tail(&page_info->link_save_group[guoup_node->index], &node->next);
		    }
		}

		// empty 
		if (!ngx_queue_empty(&page_info->link_save_group[guoup_node->index])) {

		    // save 
		    site_builder_cache_save_list_key_value(
			    link_group_base,
			    loc_conf->server_location_name,
			    uri->key,
			    &page_info->link_save_group[guoup_node->index]);
		}
	    }
	}
    }

    return 0;
}

int site_builder_page_careate_link_save_group_all(ngx_http_request_t *r,
	site_builder_loc_conf_t *loc_conf,
	site_builder_page_info_t *page_info)
{

    // find 
    site_builder_uri_t *uri = &page_info->uri;
    site_builder_element_list_t *element_list = site_builder_cache_find_list_key_value(
	    "link_save_group_all",
	    loc_conf->server_location_name,
	    uri->key);
    if (element_list) {

	ngx_queue_t *p = NULL;
	for (p = ngx_queue_head(&element_list->list_head);
		p != ngx_queue_sentinel(&element_list->list_head);
		p = ngx_queue_next(p)) {

	    site_builder_element_key_value_t *cache_node = (site_builder_element_key_value_t *)p;
	    site_builder_element_key_value_t *node = ngx_pcalloc(r->pool, sizeof(site_builder_element_key_value_t));
	    if (cache_node && node) {

		node->key = cache_node->key;
		node->key_size = cache_node->key_size;
		node->value = cache_node->value;
		node->value_size = cache_node->value_size;
		ngx_queue_insert_tail(&page_info->link_save_group_all, &node->next);
	    }
	}

	return 0;
    }

    // rand 
    int i;
    char key[64], rand_buf[20];

    for (i = 0; i < page_info->template->link_save_group_all_total; i++) {

	int domain_count = 0;
	int domain_index = rand() % *loc_conf->global_keyword_list_count;

	ngx_queue_t *p = NULL;
	for (p = ngx_queue_head(loc_conf->global_keyword_list); 
		p != ngx_queue_sentinel(loc_conf->global_keyword_list); 
		p = ngx_queue_next(p)) {

	    site_builder_element_head_tree_t *global_keyword_node = (site_builder_element_head_tree_t *)p;
	    if (domain_count++ == domain_index) {

		lg_string_rand_number(rand_buf, rand() % 9 + 4);
		sprintf(key, "/%s.html", rand_buf);

		site_builder_element_key_value_t *node = ngx_pcalloc(r->pool, sizeof(site_builder_element_key_value_t));
		site_builder_element_tree_t *element = site_builder_page_careate_link_cat_title(r,
			loc_conf,
			page_info,
			global_keyword_node->name,
			key);
		if (element && node) {

		    node->key = element->key;
		    node->key_size = element->key_size;
		    node->value = element->buffer;
		    node->value_size = element->buffer_size;
		    ngx_queue_insert_tail(&page_info->link_save_group_all, &node->next);
		}
	    }
	}
    }

    // empty 
    if (ngx_queue_empty(&page_info->link_save_group_all)) {

	return -1;
    }

    // save 
    site_builder_cache_save_list_key_value(
	    "link_save_group_all",
	    loc_conf->server_location_name,
	    uri->key,
	    &page_info->link_save_group_all);
    return 0;
}

/*************/
/* rand link */
/*************/
int site_builder_page_careate_link_internal(ngx_http_request_t *r,
	site_builder_loc_conf_t *loc_conf,
	site_builder_page_info_t *page_info)
{

    int i;
    char key[64], rand_buf[20];
    for (i = 0; i < page_info->template->link_internal_total; i++) {

	lg_string_rand_number(rand_buf, rand() % 9 + 4);
	sprintf(key, "/%s.html", rand_buf);

	site_builder_element_key_value_t *node = ngx_pcalloc(r->pool, sizeof(site_builder_element_key_value_t));
	site_builder_element_tree_t *element = site_builder_page_careate_link_cat_title(r,
		loc_conf,
		page_info,
		loc_conf->server_location_name,
		key);
	if (element && node) {

	    node->key = element->key;
	    node->key_size = element->key_size;
	    node->value = element->buffer;
	    node->value_size = element->buffer_size;
	    ngx_queue_insert_tail(&page_info->link_internal, &node->next);
	}
    }

    return 0;
}

int site_builder_page_careate_link_external(ngx_http_request_t *r,
	site_builder_loc_conf_t *loc_conf,
	site_builder_page_info_t *page_info)
{

    int i;
    for (i = 0; i < page_info->template->link_external_total; i++) {

	// rand 
	site_builder_element_str_t *node = ngx_pcalloc(r->pool, sizeof(site_builder_element_str_t));
	site_builder_element_tree_t *element = site_builder_element_index_rand(&loc_conf->link_external);
	if (element && node) {

	    node->buffer = element->buffer;
	    node->buffer_size = element->buffer_size;
	    ngx_queue_insert_tail(&page_info->link_external, &node->next);
	}
    }

    return 0;
}

int site_builder_page_careate_link_bare(ngx_http_request_t *r,
	site_builder_loc_conf_t *loc_conf,
	site_builder_page_info_t *page_info)
{

    int i;
    for (i = 0; i < page_info->template->link_bare_total; i++) {

	// rand 
	site_builder_element_str_t *node = ngx_pcalloc(r->pool, sizeof(site_builder_element_str_t));
	site_builder_element_tree_t *element = site_builder_element_index_rand(&loc_conf->link_bare);
	if (element && node) {

	    node->buffer = element->buffer;
	    node->buffer_size = element->buffer_size;
	    ngx_queue_insert_tail(&page_info->link_bare, &node->next);
	}
    }

    return 0;
}

int site_builder_page_careate_link_group(ngx_http_request_t *r,
	site_builder_loc_conf_t *loc_conf,
	site_builder_page_info_t *page_info)
{

    int i;
    char key[64], rand_buf[20];

    ngx_queue_t *p = NULL;
    for (p = ngx_queue_head(&loc_conf->link_group_config); 
	    p != ngx_queue_sentinel(&loc_conf->link_group_config); 
	    p = ngx_queue_next(p)) {

	link_group_config_t *guoup_node = (link_group_config_t *)p;
	for (i = 0; i < page_info->template->link_group_total[guoup_node->index]; i++) {

	    lg_string_rand_number(rand_buf, rand() % 9 + 4);
	    sprintf(key, "/%s.html", rand_buf);

	    site_builder_element_key_value_t *node = ngx_pcalloc(r->pool, sizeof(site_builder_element_key_value_t));
	    site_builder_element_tree_t *element = site_builder_page_careate_link_cat_title(r,
		    loc_conf,
		    page_info,
		    guoup_node->link_group_name,
		    key);
	    if (element && node) {

		node->key = element->key;
		node->key_size = element->key_size;
		node->value = element->buffer;
		node->value_size = element->buffer_size;
		ngx_queue_insert_tail(&page_info->link_group[guoup_node->index], &node->next);
	    }
	}
    }

    return 0;
}

int site_builder_page_careate_link_group_all(ngx_http_request_t *r,
	site_builder_loc_conf_t *loc_conf,
	site_builder_page_info_t *page_info)
{

    // rand domain
    int i;
    char key[64], rand_buf[20];

    for (i = 0; i < page_info->template->link_group_all_total; i++) {

	int domain_count = 0;
	int domain_index = rand() % *loc_conf->global_keyword_list_count;

	ngx_queue_t *p = NULL;
	for (p = ngx_queue_head(loc_conf->global_keyword_list); 
		p != ngx_queue_sentinel(loc_conf->global_keyword_list); 
		p = ngx_queue_next(p)) {

	    site_builder_element_head_tree_t *global_keyword_node = (site_builder_element_head_tree_t *)p;
	    if (domain_count++ == domain_index) {

		lg_string_rand_number(rand_buf, rand() % 9 + 4);
		sprintf(key, "/%s.html", rand_buf);

		site_builder_element_key_value_t *node = ngx_pcalloc(r->pool, sizeof(site_builder_element_key_value_t));
		site_builder_element_tree_t *element = site_builder_page_careate_link_cat_title(r,
			loc_conf,
			page_info,
			global_keyword_node->name,
			key);
		if (element && node) {

		    node->key = element->key;
		    node->key_size = element->key_size;
		    node->value = element->buffer;
		    node->value_size = element->buffer_size;
		    ngx_queue_insert_tail(&page_info->link_group_all, &node->next);
		}
	    }
	}
    }

    return 0;
}

int site_builder_page_load(ngx_http_request_t *r,
	site_builder_loc_conf_t *loc_conf, 
	site_builder_page_info_t *page_info)
{

    int i;

    /* init link list */
    ngx_queue_init(&page_info->content);
    ngx_queue_init(&page_info->content_rand);
    ngx_queue_init(&page_info->meta);

    ngx_queue_init(&page_info->link_internal);
    ngx_queue_init(&page_info->link_external);
    ngx_queue_init(&page_info->link_bare);
    ngx_queue_init(&page_info->link_subdomain);
    ngx_queue_init(&page_info->link_group_all);

    ngx_queue_init(&page_info->link_save_internal);
    ngx_queue_init(&page_info->link_save_external);
    ngx_queue_init(&page_info->link_save_bare);
    ngx_queue_init(&page_info->link_save_subdomain);
    ngx_queue_init(&page_info->link_save_group_all);

    for (i = 0; i < SITE_BUILDER_TEMPLATE_GROUP_COUNT; i++) {

	ngx_queue_init(&page_info->link_group[i]);
	ngx_queue_init(&page_info->link_save_group[i]);
    }

    // template
    if (site_builder_page_load_template(r, loc_conf, page_info))
	return -1;

    // title
    if (page_info->template->title_total > 0)
	site_builder_page_load_title(r, loc_conf, page_info);

    // keyword
    if (page_info->template->keyword_total > 0)
	site_builder_page_load_keyword(r, loc_conf, page_info);

    // content
    if (page_info->template->content_total > 0)
	site_builder_page_load_content(r, loc_conf, page_info);

    // content rand
    if (page_info->template->content_rand_total > 0)
	site_builder_page_load_content_rand(r, loc_conf, page_info);

    //meta
    if (page_info->template->meta_total > 0)
	site_builder_page_load_meta(r, loc_conf, page_info);

    // date_str
    if (page_info->template->date_total > 0)
	site_builder_page_load_date(r, loc_conf, page_info);

    // time_str
    if (page_info->template->time_total > 0)
	site_builder_page_load_time(r, loc_conf, page_info);

    // link_save_internal
    if (page_info->template->link_save_internal_total > 0)
	site_builder_page_careate_link_save_internal(r, loc_conf, page_info);

    // link_save_external
    if (page_info->template->link_save_external_total > 0)
	site_builder_page_careate_link_save_external(r, loc_conf, page_info);

    // link_save_subdomain
    if (page_info->template->link_save_subdomain_total > 0)
	site_builder_page_careate_link_save_subdomain(r, loc_conf, page_info);

    // link_save_bare
    if (page_info->template->link_save_bare_total > 0)
	site_builder_page_careate_link_save_bare(r, loc_conf, page_info);

    // link_save_group_all
    if (page_info->template->link_save_group_all_total > 0)
	site_builder_page_careate_link_save_group_all(r, loc_conf, page_info);

    // link_internal
    if (page_info->template->link_internal_total > 0)
	site_builder_page_careate_link_internal(r, loc_conf, page_info);

    // link_external
    if (page_info->template->link_external_total > 0)
	site_builder_page_careate_link_external(r, loc_conf, page_info);

    // link_bare
    if (page_info->template->link_bare_total > 0)
	site_builder_page_careate_link_bare(r, loc_conf, page_info);

    // link_group_all
    if (page_info->template->link_group_all_total > 0)
	site_builder_page_careate_link_group_all(r, loc_conf, page_info);

    // group 0-9
    site_builder_page_careate_link_save_group(r, loc_conf, page_info);
    site_builder_page_careate_link_group(r, loc_conf, page_info);

    return 0;
}



