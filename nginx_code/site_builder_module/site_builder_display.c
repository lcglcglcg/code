
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

int site_builder_page_display_link(ngx_http_request_t *r,
	lg_ngx_network_t *network,
	ngx_queue_t *queue)
{
    if (!queue) return -1;

    site_builder_element_key_value_t *node = (site_builder_element_key_value_t *)site_builder_page_display_pop(queue);
    if (!node) return -1;

    if (node->key_size > 0 && node->value_size > 0) {

	if (lg_ngx_network_buffer_add(r, network, "<a href=\"http://", 16))
	    return -1;

	if (lg_ngx_network_buffer_add(r, network, node->key, node->key_size))
	    return -1;

	if (lg_ngx_network_buffer_add(r, network, "\" title=\"", 9))
	    return -1;

	if (lg_ngx_network_buffer_add(r, network, node->value, node->value_size))
	    return -1;

	if (lg_ngx_network_buffer_add(r, network, "\" target=\"_blank\">", 18))
	    return -1;

	if (lg_ngx_network_buffer_add(r, network, node->value, node->value_size))
	    return -1;

	if (lg_ngx_network_buffer_add(r, network, "</a>", 4))
	    return -1;
    }

    return 0;
}

int site_builder_page_display_content(ngx_http_request_t *r,
	lg_ngx_network_t *network,
	ngx_queue_t *queue)
{

    if (!queue) return -1;

    ngx_queue_t *p = NULL;
    for (p = ngx_queue_head(queue);
	    p != ngx_queue_sentinel(queue);
	    p = ngx_queue_next(p)) {

	site_builder_element_str_t *node = (site_builder_element_str_t*)p;
	if (node->buffer && node->buffer_size > 0) {

	    if (lg_ngx_network_buffer_add(r, network, "&nbsp;&nbsp;", 12))
		return -1; 

	    if (lg_ngx_network_buffer_add(r, network, node->buffer, node->buffer_size))
		return -1;

	    if (lg_ngx_network_buffer_add(r, network, "<br>\n", 5))
		return -1;
	}
    }

    return 0;
}

int site_builder_page_display_content_meta(ngx_http_request_t *r,
	lg_ngx_network_t *network,
	ngx_queue_t *queue)
{

    if (!queue) return -1;

    if (lg_ngx_network_buffer_add(r, network, "<meta name=\"description\" content=\"", 34))
	return -1; 

    ngx_queue_t *p = NULL;
    for (p = ngx_queue_head(queue);
	    p != ngx_queue_sentinel(queue);
	    p = ngx_queue_next(p)) {

	site_builder_element_str_t *node = (site_builder_element_str_t*)p;
	if (node->buffer && node->buffer_size > 0) {

	    if (lg_ngx_network_buffer_add(r, network, node->buffer, node->buffer_size))
		return -1;

	}
    }

    if (lg_ngx_network_buffer_add(r, network, "\">", 2)) 
	return -1; 

    return 0;
}

int site_builder_page_display_content_list(ngx_http_request_t *r,
	lg_ngx_network_t *network,
	ngx_queue_t *queue)
{

    if (!queue) return -1;
    site_builder_element_list_next_t *list = (site_builder_element_list_next_t *)site_builder_page_display_pop(queue);

    ngx_queue_t *p = NULL;
    for (p = ngx_queue_head(&list->head);
	    p != ngx_queue_sentinel(&list->head);
	    p = ngx_queue_next(p)) {

	site_builder_element_str_t *node = (site_builder_element_str_t *)p;

	if (lg_ngx_network_buffer_add(r, network, "&nbsp;&nbsp;", 12))
	    return -1; 

	if (lg_ngx_network_buffer_add(r, network, node->buffer, node->buffer_size))
	    return -1;

	if (lg_ngx_network_buffer_add(r, network, "<br>\n", 5))
	    return -1;
    }

    return 0;
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
		lg_ngx_network_buffer_add(r, network, page_info->keyword_str, page_info->keyword_size);
		break;

	    case SITE_BUILDER_TEMPLATE_TYPE_CONTENT:
		site_builder_page_display_content(r, network, &page_info->content);
		break;

	    case SITE_BUILDER_TEMPLATE_TYPE_CONTENT_META:
		site_builder_page_display_content_meta(r, network, &page_info->content);
		break;

	    case SITE_BUILDER_TEMPLATE_TYPE_CONTENT_RAND:
		site_builder_page_display_content_list(r, network, &page_info->content_rand);
		break;

	    case SITE_BUILDER_TEMPLATE_TYPE_META:
		site_builder_page_display_content_meta(r, network, &page_info->meta);
		break;

	    case SITE_BUILDER_TEMPLATE_TYPE_DATE:
		lg_ngx_network_buffer_add(r, network, page_info->date_str, page_info->date_str_size);
		break;

	    case SITE_BUILDER_TEMPLATE_TYPE_TIME:
		lg_ngx_network_buffer_add(r, network, page_info->time_str, page_info->time_str_size);
		break;

	    case SITE_BUILDER_TEMPLATE_TYPE_LINK_INTERNAL:
		site_builder_page_display_link(r, network, &page_info->link_internal);
		break;

	    case SITE_BUILDER_TEMPLATE_TYPE_LINK_EXTERNAL:
		site_builder_page_display_str(r, network, &page_info->link_external);
		break;

	    case SITE_BUILDER_TEMPLATE_TYPE_LINK_BARE:
		site_builder_page_display_str(r, network, &page_info->link_bare);
		break;

	    case SITE_BUILDER_TEMPLATE_TYPE_LINK_SUBDOMAIN:
		site_builder_page_display_link(r, network, &page_info->link_subdomain);
		break;

	    case SITE_BUILDER_TEMPLATE_TYPE_LINK_SAVE_INTERNAL:
		site_builder_page_display_link(r, network, &page_info->link_save_internal);
		break;

	    case SITE_BUILDER_TEMPLATE_TYPE_LINK_SAVE_EXTERNAL:
		site_builder_page_display_str(r, network, &page_info->link_save_external);
		break;

	    case SITE_BUILDER_TEMPLATE_TYPE_LINK_SAVE_SUBDOMAIN:
		site_builder_page_display_link(r, network, &page_info->link_save_subdomain);
		break;

	    case SITE_BUILDER_TEMPLATE_TYPE_LINK_SAVE_BARE:
		site_builder_page_display_str(r, network, &page_info->link_save_bare);
		break;

	    case SITE_BUILDER_TEMPLATE_TYPE_LINK_GROUP_ALL:
		site_builder_page_display_link(r, network, &page_info->link_group_all);
		break;

	    case SITE_BUILDER_TEMPLATE_TYPE_LINK_SAVE_GROUP_ALL:
		site_builder_page_display_link(r, network, &page_info->link_save_group_all);
		break;

	    case SITE_BUILDER_TEMPLATE_TYPE_LINK_GROUP0:
		site_builder_page_display_link(r, network, &page_info->link_group[0]);
		break;

	    case SITE_BUILDER_TEMPLATE_TYPE_LINK_GROUP1:
		site_builder_page_display_link(r, network, &page_info->link_group[1]);
		break;

	    case SITE_BUILDER_TEMPLATE_TYPE_LINK_GROUP2:
		site_builder_page_display_link(r, network, &page_info->link_group[2]);
		break;

	    case SITE_BUILDER_TEMPLATE_TYPE_LINK_GROUP3:
		site_builder_page_display_link(r, network, &page_info->link_group[3]);
		break;

	    case SITE_BUILDER_TEMPLATE_TYPE_LINK_GROUP4:
		site_builder_page_display_link(r, network, &page_info->link_group[4]);
		break;

	    case SITE_BUILDER_TEMPLATE_TYPE_LINK_GROUP5:
		site_builder_page_display_link(r, network, &page_info->link_group[5]);
		break;

	    case SITE_BUILDER_TEMPLATE_TYPE_LINK_GROUP6:
		site_builder_page_display_link(r, network, &page_info->link_group[6]);
		break;

	    case SITE_BUILDER_TEMPLATE_TYPE_LINK_GROUP7:
		site_builder_page_display_link(r, network, &page_info->link_group[7]);
		break;

	    case SITE_BUILDER_TEMPLATE_TYPE_LINK_GROUP8:
		site_builder_page_display_link(r, network, &page_info->link_group[8]);
		break;

	    case SITE_BUILDER_TEMPLATE_TYPE_LINK_GROUP9:
		site_builder_page_display_link(r, network, &page_info->link_group[9]);
		break;

	    case SITE_BUILDER_TEMPLATE_TYPE_LINK_SAVE_GROUP0:
		site_builder_page_display_link(r, network, &page_info->link_save_group[0]);
		break;

	    case SITE_BUILDER_TEMPLATE_TYPE_LINK_SAVE_GROUP1:
		site_builder_page_display_link(r, network, &page_info->link_save_group[1]);
		break;

	    case SITE_BUILDER_TEMPLATE_TYPE_LINK_SAVE_GROUP2:
		site_builder_page_display_link(r, network, &page_info->link_save_group[2]);
		break;

	    case SITE_BUILDER_TEMPLATE_TYPE_LINK_SAVE_GROUP3:
		site_builder_page_display_link(r, network, &page_info->link_save_group[3]);
		break;

	    case SITE_BUILDER_TEMPLATE_TYPE_LINK_SAVE_GROUP4:
		site_builder_page_display_link(r, network, &page_info->link_save_group[4]);
		break;

	    case SITE_BUILDER_TEMPLATE_TYPE_LINK_SAVE_GROUP5:
		site_builder_page_display_link(r, network, &page_info->link_save_group[5]);
		break;

	    case SITE_BUILDER_TEMPLATE_TYPE_LINK_SAVE_GROUP6:
		site_builder_page_display_link(r, network, &page_info->link_save_group[6]);
		break;

	    case SITE_BUILDER_TEMPLATE_TYPE_LINK_SAVE_GROUP7:
		site_builder_page_display_link(r, network, &page_info->link_save_group[7]);
		break;

	    case SITE_BUILDER_TEMPLATE_TYPE_LINK_SAVE_GROUP8:
		site_builder_page_display_link(r, network, &page_info->link_save_group[8]);
		break;

	    case SITE_BUILDER_TEMPLATE_TYPE_LINK_SAVE_GROUP9:
		site_builder_page_display_link(r, network, &page_info->link_save_group[9]);
		break;
	}
    }

    return lg_ngx_network_send(r, network);
}



