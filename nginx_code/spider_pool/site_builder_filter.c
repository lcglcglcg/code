
#include "site_builder_page.h"

int site_builder_filter_init_network(ngx_http_request_t *r,
	site_builder_loc_conf_t *loc_conf,
	site_builder_page_info_t *page_info)
{

    switch(page_info->uri.uri_type) {

	case SITE_BUILDER_URI_TYPE_HTML:
	    page_info->network.content_type.data = SITE_BUILDER_URI_HTML_STR;
	    page_info->network.content_type.len = SITE_BUILDER_URI_HTML_SIZE;
	    break;

	case SITE_BUILDER_URI_TYPE_CSS:
	    page_info->network.content_type.data = SITE_BUILDER_URI_CSS_STR;
	    page_info->network.content_type.len = SITE_BUILDER_URI_CSS_SIZE;
	    break;

	case SITE_BUILDER_URI_TYPE_JS:
	    page_info->network.content_type.data = SITE_BUILDER_URI_JS_STR;
	    page_info->network.content_type.len = SITE_BUILDER_URI_JS_SIZE;
	    break;

	case SITE_BUILDER_URI_TYPE_GIF:
	    page_info->network.content_type.data = SITE_BUILDER_URI_GIF_STR;
	    page_info->network.content_type.len = SITE_BUILDER_URI_GIF_SIZE;
	    break;

	case SITE_BUILDER_URI_TYPE_ICO:
	    page_info->network.content_type.data = SITE_BUILDER_URI_ICO_STR;
	    page_info->network.content_type.len = SITE_BUILDER_URI_ICO_SIZE;
	    break;

	case SITE_BUILDER_URI_TYPE_JPEG:
	    page_info->network.content_type.data = SITE_BUILDER_URI_JPEG_STR;
	    page_info->network.content_type.len = SITE_BUILDER_URI_JPEG_SIZE;
	    break;

	case SITE_BUILDER_URI_TYPE_PNG:
	    page_info->network.content_type.data = SITE_BUILDER_URI_PNG_STR;
	    page_info->network.content_type.len = SITE_BUILDER_URI_PNG_SIZE;
	    break;

	case SITE_BUILDER_URI_TYPE_BMP:
	    page_info->network.content_type.data = SITE_BUILDER_URI_BMP_STR;
	    page_info->network.content_type.len = SITE_BUILDER_URI_BMP_SIZE;
	    break;

	case SITE_BUILDER_URI_TYPE_IMG:
	    page_info->network.content_type.data = SITE_BUILDER_URI_IMG_STR;
	    page_info->network.content_type.len = SITE_BUILDER_URI_IMG_SIZE;
	    break;

        case SITE_BUILDER_URI_TYPE_TXT:
            page_info->network.content_type.data = SITE_BUILDER_URI_TXT_STR;
            page_info->network.content_type.len = SITE_BUILDER_URI_TXT_SIZE;

	default:
	    page_info->network.content_type.data = SITE_BUILDER_URI_HTML_STR;
	    page_info->network.content_type.len = SITE_BUILDER_URI_HTML_SIZE;
    }

    return 0;
}

int site_builder_filter_affiliated_file(ngx_http_request_t *r, 
	site_builder_loc_conf_t *loc_conf,
	site_builder_page_info_t *page_info)
{

    site_builder_uri_t *uri = &page_info->uri;
    if (uri->uri_type == 0) return lg_ngx_network_not_found(r);

    site_builder_element_t *element = site_builder_element_key_find(loc_conf->affiliated_file, uri->key);
    if (!element) return lg_ngx_network_not_found(r);

    return lg_ngx_network_buffer_send(r, &page_info->network, element->buffer, element->buffer_size);
}

int site_builder_filter(ngx_http_request_t *r, site_builder_loc_conf_t *loc_conf)
{

    // page_info
    site_builder_page_info_t *page_info = ngx_pcalloc(r->pool, sizeof(site_builder_page_info_t));
    if (!page_info) return NGX_ERROR;
    memset(page_info, 0, sizeof(site_builder_page_info_t));

    // URI parse
    if (site_builder_uri_parse(r, &page_info->uri))
	return lg_ngx_network_not_found(r);

    // network init
    if (site_builder_filter_init_network(r, loc_conf, page_info))
	return lg_ngx_network_not_found(r);

    // affiliated_file (css, js, jpg ...)
    if (page_info->uri.uri_type != SITE_BUILDER_URI_TYPE_HTML)
	return site_builder_filter_affiliated_file(r, loc_conf, page_info);

    // page load
    if (site_builder_page_load(r, loc_conf, page_info))
	return lg_ngx_network_not_found(r);

    // page display
    return site_builder_page_display(r, loc_conf, page_info);
}


