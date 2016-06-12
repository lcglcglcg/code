
#include "site_builder_page.h"

ngx_queue_t *site_builder_page_display_pop(ngx_queue_t *queue)
{

    ngx_queue_t *p = ngx_queue_next(queue);
    if (p == ngx_queue_sentinel(queue))
	return NULL;

    ngx_queue_remove(p);
    return p;
}

int site_builder_page_display_str(ngx_http_request_t *r,
	lg_ngx_network_t *network,
	ngx_queue_t *queue)
{

    if (!queue) return -1;

    site_builder_element_str_t *node = (site_builder_element_str_t *)site_builder_page_display_pop(queue);
    if (!node) return -1;

    if (node->buffer && node->buffer_size > 0) {

	if (lg_ngx_network_buffer_add(r, network, node->buffer, node->buffer_size))
	    return -1;
    }

    return 0;
}

int site_builder_page_display_count(ngx_http_request_t *r,
	lg_ngx_network_t *network)
{

    char buffer[32];
    int size = sprintf(buffer, "%d", rand() % 100 + 10);
    return lg_ngx_network_buffer_add(r, network, buffer, size);
}

int site_builder_page_display(ngx_http_request_t *r, site_builder_loc_conf_t *loc_conf, site_builder_page_info_t *page_info)
{

    ngx_queue_t *p = NULL;
    ngx_queue_t *head = &page_info->template->head;
    lg_ngx_network_t *network = &page_info->network;
    for (p = ngx_queue_head(head); p != ngx_queue_sentinel(head); p = ngx_queue_next(p)) {

	site_builder_template_node_t *node = (site_builder_template_node_t *)p;
	switch(node->type) {

	    case SITE_BUILDER_TEMPLATE_TYPE_HTML:
		lg_ngx_network_buffer_add(r, network, node->buffer, node->size);
		break;

	    case SITE_BUILDER_TEMPLATE_TYPE_TITLE:
		lg_ngx_network_buffer_add(r, network, page_info->title_str, page_info->title_size);
		break;

	    case SITE_BUILDER_TEMPLATE_TYPE_KEYWORD:
		site_builder_page_display_str(r, network, &page_info->keyword);
		break;

	    case SITE_BUILDER_TEMPLATE_TYPE_CONTENT:
		site_builder_page_display_str(r, network, &page_info->content);
		break;

	    case SITE_BUILDER_TEMPLATE_TYPE_META:
		lg_ngx_network_buffer_add(r, network, page_info->meta_str, page_info->meta_size);
		break;

	    case SITE_BUILDER_TEMPLATE_TYPE_DATE:
		lg_ngx_network_buffer_add(r, network, page_info->date_str, 10);
		break;

	    case SITE_BUILDER_TEMPLATE_TYPE_TIME:
		lg_ngx_network_buffer_add(r, network, page_info->time_str, 19);
		break;

	    case SITE_BUILDER_TEMPLATE_TYPE_COUNT:
		site_builder_page_display_count(r, network);
		break;

	    case SITE_BUILDER_TEMPLATE_TYPE_LINK_INTERNAL:
		site_builder_page_display_str(r, network, &page_info->link_internal);
		break;

	    case SITE_BUILDER_TEMPLATE_TYPE_LINK_EXTERNAL:
		site_builder_page_display_str(r, network, &page_info->link_external);
		break;

	    case SITE_BUILDER_TEMPLATE_TYPE_LINK_BARE:
		site_builder_page_display_str(r, network, &page_info->link_bare);
		break;

	    case SITE_BUILDER_TEMPLATE_TYPE_LINK_GROUP:
		site_builder_page_display_str(r, network, &page_info->link_group);
		break;
	}
    }

    return lg_ngx_network_send(r, network);
}


