
#include "lg_string.h"
#include "site_builder_page.h"
#include "site_builder_flush.h"

int site_builder_page_load_template(ngx_http_request_t *r,
	site_builder_loc_conf_t *loc_conf, 
	site_builder_page_info_t *page_info)
{

#if 0
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
#endif

    page_info->template = &loc_conf->template_index;
    return 0;
}

int site_builder_page_load_title(ngx_http_request_t *r,
	site_builder_loc_conf_t *loc_conf, 
	site_builder_page_info_t *page_info)
{

    // find title
    char *buffer = NULL;
    site_builder_uri_t *uri = &page_info->uri;
    int buffer_size = site_builder_flush_find_str(
	    "title",
	    loc_conf->server_location_name,
	    uri->key,
	    &buffer,
	    r->pool);
    if (buffer_size > 0) {

	page_info->title_str = buffer;
	page_info->title_size = buffer_size;
	return 0;
    }

    // rand title
    site_builder_element_node_t *element = site_builder_element_index_rand(loc_conf->keyword);
    if (!element) return -1;

    // save title
    site_builder_flush_save_str(
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
    int i, list_count = site_builder_flush_find_list(
	    "keyword",
	    loc_conf->server_location_name,
	    uri->key,
	    &page_info->keyword,
	    r->pool);
    if (list_count >= page_info->template->keyword_total) {

	return 0;
    }

    // rand keyword
    for (i = list_count;
	    i < page_info->template->keyword_total;
	    i++) {

	site_builder_element_node_t *element = site_builder_element_index_rand(loc_conf->keyword);
	site_builder_element_str_t *node = ngx_pcalloc(r->pool, sizeof(site_builder_element_str_t));
	if (element && node && element->buffer_size > 1) {

	    node->buffer = element->buffer;
	    node->buffer_size = element->buffer_size;
	    ngx_queue_insert_tail(&page_info->keyword, &node->next);
	}
    }

    // save keyword
    site_builder_flush_save_list(
	    "keyword",
	    loc_conf->server_location_name,
	    uri->key,
	    &page_info->keyword,
	    r->pool);
    return 0;
}

int site_builder_page_load_content(ngx_http_request_t *r,
	site_builder_loc_conf_t *loc_conf, 
	site_builder_page_info_t *page_info)
{

    site_builder_uri_t *uri = &page_info->uri;
    int i, list_count = site_builder_flush_find_list(
	    "content",
	    loc_conf->server_location_name,
	    uri->key,
	    &page_info->content,
	    r->pool);
    if (list_count >= page_info->template->content_total) {

	return 0;
    }

    for (i = list_count;
	    i < page_info->template->content_total;
	    i++) {

	site_builder_element_node_t *element = site_builder_element_index_rand(loc_conf->content);
	site_builder_element_str_t *node = ngx_pcalloc(r->pool, sizeof(site_builder_element_str_t));
	if (element && node && element->buffer_size > 1) {

	    node->buffer = element->buffer;
	    node->buffer_size = element->buffer_size;
	    ngx_queue_insert_tail(&page_info->content, &node->next);
	}
    }

    site_builder_flush_save_list(
	    "content",
	    loc_conf->server_location_name,
	    uri->key,
	    &page_info->content,
	    r->pool);
    return 0;
}

int site_builder_page_load_time_str(ngx_http_request_t *r,
	site_builder_loc_conf_t *loc_conf, 
	site_builder_page_info_t *page_info)
{

    // find 
    char *buffer = NULL;
    site_builder_uri_t *uri = &page_info->uri;
    int buffer_size = site_builder_flush_find_str(
	    "time_str",
	    loc_conf->server_location_name,
	    uri->key,
	    &buffer,
	    r->pool);
    if (buffer_size > 0) {

	memcpy(page_info->date_str, buffer, 10);
	memcpy(page_info->time_str, buffer, 19);
	return 0;
    }

    // create 
    struct tm tm;
    time_t t = time(NULL);
    localtime_r(&t, &tm);

    buffer = page_info->date_str;
    buffer_size = sprintf(buffer, "%04d-%02d-%02d",
	    tm.tm_year + 1900,
	    tm.tm_mon + 1,
	    tm.tm_mday);

    buffer = page_info->time_str;
    buffer_size = sprintf(buffer, "%04d-%02d-%02d %02d:%02d:%02d",
	    tm.tm_year + 1900,
	    tm.tm_mon + 1,
	    tm.tm_mday,
	    tm.tm_hour,
	    tm.tm_min,
	    tm.tm_sec);

    // save 
    site_builder_flush_save_str(
	    "time_str",
	    loc_conf->server_location_name,
	    uri->key,
	    buffer, buffer_size);
    return 0;
}

site_builder_element_str_t *site_builder_page_create_link(ngx_http_request_t *r,
	char *server_location_name, int server_location_name_size,
	char *key, int key_size,
	char *keyword, int keyword_size)
{

    if (*key == '/') {

	key++;
	key_size--;
    }

    site_builder_element_str_t *str = ngx_pcalloc(r->pool, sizeof(site_builder_element_str_t));
    if (!str) return NULL;

    str->buffer = ngx_pcalloc(r->pool, server_location_name_size + key_size + keyword_size + keyword_size + 48);
    if (!str->buffer) return NULL;

    str->buffer_size = sprintf(str->buffer, "<a href=\"http://%s%s\" title=\"%s\" target=\"_blank\">%s</a>",
	    server_location_name,
	    key,
	    keyword,
	    keyword);
    return str;
}

int site_builder_page_link_cat_title(ngx_http_request_t *r,
	site_builder_loc_conf_t *loc_conf,
	char *server_location_name,
	char *key,
	char **buffer)
{
    // find title
    int buffer_size = site_builder_flush_find_str(
	    "title",
	    server_location_name,
	    key,
	    buffer,
	    r->pool);
    if (buffer_size > 0) {

	return buffer_size;
    }

    // rand title
    site_builder_element_node_t *element = site_builder_element_index_rand(loc_conf->keyword);
    if (!element) return -1;

    // save title
    site_builder_flush_save_str(
	    "title",
	    server_location_name,
	    key,
	    element->buffer, element->buffer_size);

    *buffer = element->buffer;
    return element->buffer_size;
}

int site_builder_page_load_link_internal(ngx_http_request_t *r,
	site_builder_loc_conf_t *loc_conf,
	site_builder_page_info_t *page_info)
{

    // find
    site_builder_uri_t *uri = &page_info->uri;
    int i, list_count = site_builder_flush_find_list(
	    "link_internal",
	    loc_conf->server_location_name,
	    uri->key,
	    &page_info->link_internal,
	    r->pool);
    if (list_count >= page_info->template->link_internal_total) {

	return 0;
    }

    // rand
    char key[64], rand_buf[20];
    for (i = list_count;
	    i < page_info->template->link_internal_total;
	    i++) {

	lg_string_rand_number(rand_buf, rand() % 9 + 4);
	int key_size = sprintf(key, "/%s.html", rand_buf);

	char *keyword = NULL;
	int keyword_size = site_builder_page_link_cat_title(r, loc_conf,
		loc_conf->server_location_name,
		key,
		&keyword);
	site_builder_element_str_t *node = site_builder_page_create_link(r,
		loc_conf->server_location_name, loc_conf->server_location_name_len,
		key, key_size,
		keyword, keyword_size);
	if (node) {

	    ngx_queue_insert_tail(&page_info->link_internal, &node->next);
	}
    }

    // save
    site_builder_flush_save_list(
	    "link_internal",
	    loc_conf->server_location_name,
	    uri->key,
	    &page_info->link_internal,
	    r->pool);
    return 0;
}

int site_builder_page_load_link_external(ngx_http_request_t *r,
	site_builder_loc_conf_t *loc_conf,
	site_builder_page_info_t *page_info)
{
    // find
    site_builder_uri_t *uri = &page_info->uri;
    int i, list_count = site_builder_flush_find_list(
	    "link_external",
	    loc_conf->server_location_name,
	    uri->key,
	    &page_info->link_external,
	    r->pool);
    if (list_count >= page_info->template->link_external_total) {

	return 0;
    }

    // rand
    for (i = list_count;
	    i < page_info->template->link_external_total;
	    i++) {

	site_builder_element_node_t *element = site_builder_element_index_rand(loc_conf->link_external);
	if (element) {

	    site_builder_element_str_t *node = site_builder_page_create_link(r, 
		    "", 0,
		    element->key, element->key_size,
		    element->buffer, element->buffer_size);
	    if (node) {

		ngx_queue_insert_tail(&page_info->link_external, &node->next);
	    }
	}
    }

    // save
    site_builder_flush_save_list(
	    "link_external",
	    loc_conf->server_location_name,
	    uri->key,
	    &page_info->link_external,
	    r->pool);
    return 0;
}

int site_builder_page_load_link_bare(ngx_http_request_t *r,
	site_builder_loc_conf_t *loc_conf,
	site_builder_page_info_t *page_info)
{
    return 0;
}

int site_builder_page_load_link_group(ngx_http_request_t *r,
	site_builder_loc_conf_t *loc_conf,
	site_builder_page_info_t *page_info)
{

    return 0;
}

int site_builder_page_load(ngx_http_request_t *r,
	site_builder_loc_conf_t *loc_conf, 
	site_builder_page_info_t *page_info)
{

    /* init list */
    ngx_queue_init(&page_info->keyword);
    ngx_queue_init(&page_info->content);

    ngx_queue_init(&page_info->link_internal);
    ngx_queue_init(&page_info->link_external);
    ngx_queue_init(&page_info->link_bare);
    ngx_queue_init(&page_info->link_group);

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

    // time_str
    if (page_info->template->time_total > 0 || page_info->template->date_total > 0)
	site_builder_page_load_time_str(r, loc_conf, page_info);

    // link_internal
    if (page_info->template->link_internal_total > 0)
	site_builder_page_load_link_internal(r, loc_conf, page_info);

    // link_external
    if (page_info->template->link_external_total > 0)
	site_builder_page_load_link_external(r, loc_conf, page_info);

    // link_bare
    if (page_info->template->link_bare_total > 0)
	site_builder_page_load_link_bare(r, loc_conf, page_info);

    // link_group
    if (page_info->template->link_group_total > 0)
	site_builder_page_load_link_group(r, loc_conf, page_info);

    return 0;
}




