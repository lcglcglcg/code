
#include "lg_string.h"
#include "site_builder_page.h"

int site_builder_page_load_template(ngx_http_request_t *r,
	site_builder_loc_conf_t *loc_conf, 
	site_builder_page_info_t *page_info)
{

    // rand _template
    int template_id = rand() % loc_conf->template_list.template_total;

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

    return -1;
}

int site_builder_page_load_title(ngx_http_request_t *r,
	site_builder_loc_conf_t *loc_conf, 
	site_builder_page_info_t *page_info)
{

    // rand title
    site_builder_element_t *element = site_builder_element_index_rand(loc_conf->keyword);
    if (!element) return -1;

    page_info->title_str = element->buffer;
    page_info->title_size = element->buffer_size;
    return 0;
}

int site_builder_page_load_keyword(ngx_http_request_t *r,
	site_builder_loc_conf_t *loc_conf, 
	site_builder_page_info_t *page_info)
{

    // rand keyword
    site_builder_element_t *element = site_builder_element_index_rand(loc_conf->keyword);
    if (!element) return -1;

    page_info->keyword_str = element->buffer;
    page_info->keyword_size = element->buffer_size;
    return 0;
}

int site_builder_page_load_content(ngx_http_request_t *r,
	site_builder_loc_conf_t *loc_conf, 
	site_builder_page_info_t *page_info)
{

    // rand content
    int i, max = rand() % 6 + 10;
    for (i = 0; i < max; i++) {

	site_builder_page_str_t *content_node = ngx_pcalloc(r->pool, sizeof(site_builder_page_str_t));
	site_builder_element_t *element = site_builder_element_index_rand(loc_conf->content);
	if (content_node && element) {

	    content_node->str = element->buffer;
	    content_node->str_size = element->buffer_size;
	    ngx_queue_insert_tail(&page_info->content, &content_node->next);
	}
    }

    // empty content
    if (ngx_queue_empty(&page_info->content))
	return -1;

    return 0;
}

int site_builder_page_careate_link_internal(ngx_http_request_t *r,
	site_builder_loc_conf_t *loc_conf,
	site_builder_page_info_t *page_info)
{

    int i, link_size = 0;
    char rand_buf[10], link_buf[1024];
    site_builder_uri_t *uri = &page_info->uri;
    for (i = 0; i < page_info->template->link_internal_total; i++) {

	lg_string_rand_number(rand_buf, 4);
	link_size = sprintf(link_buf, "%s/%s.html", uri->url, rand_buf);

	char *link_str = ngx_pcalloc(r->pool, link_size);
	site_builder_element_t *element_keyword = site_builder_element_index_rand(loc_conf->keyword);
	site_builder_page_link_t *link = ngx_pcalloc(r->pool, sizeof(site_builder_page_link_t));
	if (link_str && element_keyword && link) {

	    link->keyword = element_keyword->buffer;
	    link->keyword_size = element_keyword->buffer_size;

	    link->link = link_str;
	    link->link_size = link_size;

	    memcpy((char *)link->link, link_buf, link_size);
	    link_str[link_size] = 0;

	    ngx_queue_insert_tail(&page_info->link_internal, &link->next);
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
	site_builder_element_t *element = site_builder_element_index_rand(loc_conf->link_external);
	if (element) {

	    site_builder_page_str_t *link = ngx_pcalloc(r->pool, sizeof(site_builder_page_str_t));
	    if (link) {

		link->str = element->buffer;
		link->str_size = element->buffer_size;
		ngx_queue_insert_tail(&page_info->link_external, &link->next);
	    }
	}
    }

    return 0;
}

int site_builder_page_careate_link_bare(ngx_http_request_t *r,
	site_builder_loc_conf_t *loc_conf,
	site_builder_page_info_t *page_info)
{

    return 0;
}

int site_builder_page_careate_link_group(ngx_http_request_t *r,
	site_builder_loc_conf_t *loc_conf,
	site_builder_page_info_t *page_info)
{

    int i, link_size = 0;
    char rand_buf[10], link_buf[1024];
    for (i = 0; i < page_info->template->link_group_total; i++) {

	site_builder_element_t *element_group = site_builder_element_index_rand(loc_conf->link_group);
	site_builder_element_t *element_keyword = site_builder_element_index_rand(loc_conf->keyword);

	if (element_group && element_keyword) {

	    if (rand() % 2) {

		lg_string_rand_number(rand_buf, 4);
		link_size = sprintf(link_buf, "www.%s/%s.html", element_group->buffer, rand_buf);

	    } else {

		lg_string_rand_keyword(rand_buf, 4);
		link_size = sprintf(link_buf, "%s.%s", rand_buf, element_group->buffer);
	    }

	    char *link_str = ngx_pcalloc(r->pool, link_size);
	    site_builder_page_link_t *link = ngx_pcalloc(r->pool, sizeof(site_builder_page_link_t));
	    if (link_str && link) {

		link->keyword = element_keyword->buffer;
		link->keyword_size = element_keyword->buffer_size;

		link->link = link_str;
		link->link_size = link_size;

		memcpy((char *)link->link, link_buf, link_size);
		link_str[link_size] = 0;

		ngx_queue_insert_tail(&page_info->link_group, &link->next);
	    }
	}
    }

    return 0;
}

int site_builder_page_load(ngx_http_request_t *r,
	site_builder_loc_conf_t *loc_conf, 
	site_builder_page_info_t *page_info)
{

    /* init link list */
    ngx_queue_init(&page_info->content);

    ngx_queue_init(&page_info->link_internal);
    ngx_queue_init(&page_info->link_external);
    ngx_queue_init(&page_info->link_bare);
    ngx_queue_init(&page_info->link_group);

    if (site_builder_page_load_template(r, loc_conf, page_info))
	return -1;

    site_builder_page_load_title(r, loc_conf, page_info);
    site_builder_page_load_keyword(r, loc_conf, page_info);
    site_builder_page_load_content(r, loc_conf, page_info);

    site_builder_page_careate_link_internal(r, loc_conf, page_info);
    site_builder_page_careate_link_external(r, loc_conf, page_info);
    site_builder_page_careate_link_group(r, loc_conf, page_info);

    return 0;
}



