
#include "lg_string.h"
#include "lg_ngx_network.h"
#include "site_builder_uri.h"

int site_builder_uri_key_parse(site_builder_uri_t *uri, char *s, char *e)
{

    uri->key_size = 0;
    uri->key = (char *)s;

    if ((s == e) || (*s == '/' && (s + 1) == e)) { 

	uri->key = "/";
	uri->key_size = 1;
	return 0;
    }

    char *str = NULL;
    for (str = s; str <= e; str++) {

	if (*str == ' ' || *str == '?')
	    break;

	if (*str == '/' || *str == '.' || *str == '_' || *str == '-' || isalnum(*str))
	    uri->key_size++;
	else
	    return -1;
    }

    if (uri->key_size <= 0)
	return -1;

    uri->key[uri->key_size] = 0;
    return 0;
}

int site_builder_uri_domain_parse(ngx_http_request_t *r, site_builder_uri_t *uri)
{

    ngx_http_headers_in_t *headers_in = &r->headers_in;
    if (!headers_in || headers_in->server.len == 0)
	return -1;

    if (headers_in->server.len + uri->key_size >= SITE_BUILDER_URI_KEY_SIZE)
	return -1;

    uri->url = (char *)headers_in->server.data;
    uri->url_size = headers_in->server.len;
    if (uri->url_size <= 0)
	return -1;

    uri->url[uri->url_size] = 0;
    uri->domain = lg_string_url_to_domain(uri->url);
    if (!uri->domain) 
	return -1;

    uri->domain_size = strlen(uri->domain);
    return 0;
}

void site_builder_uri_mobile_parse(ngx_http_request_t *r, site_builder_uri_t *uri)
{

    uri->mobile_type = SITE_BUILDER_URI_TYPE_URL;
    char *user_agent = lg_ngx_network_get_user_agent(r);
    if (user_agent) {

	if (
		lg_string_strstr(user_agent, "ipad")
		|| lg_string_strstr(user_agent, "iphone")
		|| lg_string_strstr(user_agent, "android")
		|| lg_string_strstr(user_agent, "windows ce")
		|| lg_string_strstr(user_agent, "windows mobile")
	   ) {

	    uri->mobile_type = SITE_BUILDER_URI_TYPE_MOBILE_URL;
	}
    }
}

int site_builder_uri_html_type_parse(char *key, int key_size)
{

    char *str = NULL;
    for (str = key; *str; str++)
	if (*str == '.')
	    return -1;

    return 0;
}

void site_builder_uri_type_parse(ngx_http_request_t *r, site_builder_uri_t *uri)
{

    char *key = uri->key;
    int size = uri->key_size;

    if (strstr(key, "baidu_verify_"))
	uri->uri_type = SITE_BUILDER_URI_TYPE_BAIDU;

    else if (site_builder_uri_html_type_parse(key, size) == 0)
	uri->uri_type = SITE_BUILDER_URI_TYPE_HTML;

    else if (size < 5) return; //防止越界

    else if (strncmp(&key[size - 4], ".htm", 4) == 0)
	uri->uri_type = SITE_BUILDER_URI_TYPE_HTML;

    else if (strncmp(&key[size - 5], ".html", 5) == 0)
	uri->uri_type = SITE_BUILDER_URI_TYPE_HTML;

    else if (strncmp(&key[size - 4], ".css", 4) == 0)
	uri->uri_type = SITE_BUILDER_URI_TYPE_CSS;

    else if (strncmp(&key[size - 3], ".js", 3) == 0)
	uri->uri_type = SITE_BUILDER_URI_TYPE_JS;

    else if (strncmp(&key[size - 4], ".gif", 4) == 0)
	uri->uri_type = SITE_BUILDER_URI_TYPE_GIF;

    else if (strncmp(&key[size - 4], ".ico", 4) == 0)
	uri->uri_type = SITE_BUILDER_URI_TYPE_ICO;

    else if (strncmp(&key[size - 4], ".jpe", 4) == 0)
	uri->uri_type = SITE_BUILDER_URI_TYPE_JPEG;

    else if (strncmp(&key[size - 4], ".jpg", 4) == 0)
	uri->uri_type = SITE_BUILDER_URI_TYPE_JPEG;

    else if (strncmp(&key[size - 5], ".jpeg", 5) == 0)
	uri->uri_type = SITE_BUILDER_URI_TYPE_JPEG;

    else if (strncmp(&key[size - 4], ".png", 4) == 0)
	uri->uri_type = SITE_BUILDER_URI_TYPE_PNG;

    else if (strncmp(&key[size - 4], ".bmp", 4) == 0)
	uri->uri_type = SITE_BUILDER_URI_TYPE_BMP;

    else if (strncmp(&key[size - 4], ".img", 4) == 0)
	uri->uri_type = SITE_BUILDER_URI_TYPE_IMG;

    else if (strncmp(&key[size - 4], ".txt", 4) == 0)
	uri->uri_type = SITE_BUILDER_URI_TYPE_TXT;
}

int site_builder_uri_parse(ngx_http_request_t *r, 
	const char *location_name,
	int location_name_len,
	site_builder_uri_t *uri)
{

    char *s = (char *)r->uri_start;
    char *e = (char *)r->uri_end;
    if (location_name_len > 0) {

	if (strncmp(location_name, s, location_name_len))
	    return -1;

	s += location_name_len;
	if (s > e) return -1;
    }

    if (site_builder_uri_key_parse(uri, s, e))
	return -1;

    if (site_builder_uri_domain_parse(r, uri))
	return -1;

    site_builder_uri_type_parse(r, uri);
    site_builder_uri_mobile_parse(r, uri);

    // fprintf(stdout, "1-->[%d][%s]\n", uri->key_size, uri->key);
    // fprintf(stdout, "2-->[%d][%s]\n", uri->url_size, uri->url);
    // fprintf(stdout, "3-->[%d][%s]\n", uri->domain_size, uri->domain);
    // fprintf(stdout, "4-->[%s][%s]\n", location_name, uri->key);
    // fprintf(stdout, "4-->[%s][%s][%d]\n", location_name, uri->key, uri->uri_type);
    return 0;
}



