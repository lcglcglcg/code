
#include "lg_ngx_network.h"

void lg_ngx_network_recv_print(char *buffer)
{
    struct tm tm;
    time_t t = time(NULL);
    localtime_r(&t, &tm);

    fprintf(stdout, "[send][%04d-%02d-%02d %02d:%02d:%02d]",
	    tm.tm_year + 1900,
	    tm.tm_mon + 1,
	    tm.tm_mday,
	    tm.tm_hour,
	    tm.tm_min,
	    tm.tm_sec);
    fprintf(stderr, "%s\n", buffer);
}

void lg_ngx_network_send_print(ngx_chain_t *buffer)
{
    struct tm tm;
    time_t t = time(NULL);
    localtime_r(&t, &tm);

    ngx_chain_t *p = NULL;
    for (p = buffer; p; p = p->next) {

	fprintf(stdout, "[send][%04d-%02d-%02d %02d:%02d:%02d]",
		tm.tm_year + 1900,
		tm.tm_mon + 1,
		tm.tm_mday,
		tm.tm_hour,
		tm.tm_min,
		tm.tm_sec);

	fwrite(p->buf->pos, (p->buf->last - p->buf->pos), 1, stdout);
	fprintf(stdout, "\n");
    }
}

int lg_ngx_network_send(ngx_http_request_t *r, lg_ngx_network_t *p)
{
    r->headers_out.status = NGX_HTTP_OK;
    r->headers_out.content_type = p->content_type;
    // r->headers_out.content_length_n = p->content_length;

    int rc = ngx_http_send_header(r);
    if (rc == NGX_ERROR || rc > NGX_OK || r->header_only)
	return rc;

    // lg_ngx_network_send_print(p->head);
    return ngx_http_output_filter(r, p->head);
}

void lg_ngx_network_post_send(ngx_http_request_t *r, lg_ngx_network_t *p)
{
    int rc = lg_ngx_network_send(r, p);
    return ngx_http_finalize_request(r, rc); 
}

ngx_chain_t *lg_ngx_network_buffer_to_chain(ngx_http_request_t *r, const char *p_buffer, size_t size)
{

    ngx_chain_t *buffer = NULL;
    if (!(buffer = ngx_pcalloc(r->pool, sizeof(ngx_chain_t))))
	return NULL;

    buffer->buf = ngx_pcalloc(r->pool, sizeof(ngx_buf_t));
    buffer->next = NULL;

    buffer->buf->pos = ngx_pcalloc(r->pool, size);
    if (!buffer->buf->pos)
	return NULL;

    buffer->buf->last = buffer->buf->pos + size;
    ngx_memcpy(buffer->buf->pos, p_buffer, size);
    buffer->buf->memory = 1;
    buffer->buf->last_buf = 1;
    return buffer;
}

ngx_chain_t *lg_ngx_network_buffer_to_chain_r(ngx_http_request_t *r, const char *p_buffer, size_t size)
{

    ngx_chain_t *buffer = NULL;
    if (!(buffer = ngx_pcalloc(r->pool, sizeof(ngx_chain_t))))
	return NULL;

    buffer->buf = ngx_pcalloc(r->pool, sizeof(ngx_buf_t));
    buffer->next = NULL;

    buffer->buf->pos = (u_char *)p_buffer;
    buffer->buf->last = buffer->buf->pos + size;
    buffer->buf->memory = 1;
    return buffer;
}

int lg_ngx_network_buffer_add(ngx_http_request_t *r, lg_ngx_network_t *network, const char *p_buffer, size_t size)
{

    ngx_chain_t *buffer = lg_ngx_network_buffer_to_chain(r, p_buffer, size);
    if (!buffer) return -1;

    if (!network->head) {

	network->head = buffer;
	network->head->next = NULL;

    } else if (!network->next) {

	network->head->buf->last_buf = 0;
	network->head->next = network->next = buffer;

    } else {

	network->next->buf->last_buf = 0;
	network->next->next = buffer;
	network->next = buffer;
    }

    network->content_length += size;
    return 0;
}

int lg_ngx_network_buffer_send(ngx_http_request_t *r, lg_ngx_network_t *network, const char *p_buffer, size_t size)
{
    lg_ngx_network_t *p = network;
    if (!network) {

	p = ngx_pcalloc(r->pool, sizeof(lg_ngx_network_t));
	memset(p, 0, sizeof(lg_ngx_network_t));
    }

    if (lg_ngx_network_buffer_add(r, p, p_buffer, size))
	return -1;

    return lg_ngx_network_send(r, p);
}

void lg_ngx_network_buffer_post_send(ngx_http_request_t *r, lg_ngx_network_t *network, const char *p_buffer, size_t size)
{
    int rc = lg_ngx_network_buffer_send(r, network, p_buffer, size);
    return ngx_http_finalize_request(r, rc); 
}

int lg_ngx_network_not_found(ngx_http_request_t *r)
{

    r->headers_out.status = NGX_HTTP_NOT_FOUND;
    r->headers_out.content_length_n = 13;
    int rc = ngx_http_send_header(r);
    if (rc == NGX_ERROR || rc > NGX_OK || r->header_only)
	return rc;

    ngx_chain_t *buffer = lg_ngx_network_buffer_to_chain(r, "404 Not Found", 13);
    if (!buffer) return NGX_ERROR;

    return ngx_http_output_filter(r, buffer);
}

void lg_ngx_network_post_not_found(ngx_http_request_t *r)
{

    int rc = lg_ngx_network_not_found(r);
    return ngx_http_finalize_request(r, rc); 
}

char *lg_ngx_network_get_user_agent(ngx_http_request_t *r)
{

    ngx_http_headers_in_t *headers_in = &r->headers_in;
    if (!headers_in) return NULL;

    ngx_table_elt_t *p = headers_in->user_agent;
    if (!p || !p->value.data) return NULL;

    return (char *)p->value.data;
}

char *lg_ngx_network_get_ipaddr(ngx_http_request_t *r)
{

    ngx_connection_t *connection = r->connection;
    if (!connection) return NULL;

    ngx_str_t *ip_addr = &connection->addr_text;
    if (!ip_addr)
	return NULL;

    return (char *)ip_addr->data;
}

char *lg_ngx_network_get_forwarded_ipaddr(ngx_http_request_t *r)
{
    ngx_http_headers_in_t *headers_in = &r->headers_in;
    if (!headers_in) return NULL;

    ngx_array_t *x_forwarded_for = &headers_in->x_forwarded_for;
    if (!x_forwarded_for) return NULL;

    ngx_int_t count = x_forwarded_for->nelts;
    if (count <= 0) return NULL;

    ngx_table_elt_t **p = x_forwarded_for->elts;
    return (char *)p[0]->value.data;
}

int lg_ngx_network_set_cookie(ngx_http_request_t *r, char *cookie, size_t cookie_size)
{

    ngx_table_elt_t *set_cookie;
    set_cookie = ngx_list_push (&r->headers_out.headers);
    if (set_cookie == NULL)
	return -1;

    char *buffer = ngx_pcalloc(r->pool, cookie_size);
    if (!buffer) 
	return -1;

    memcpy(buffer, cookie, cookie_size);
    set_cookie->hash = 1;
    ngx_str_set (&set_cookie->key, "Set-Cookie");
    set_cookie->value.data = (void *)buffer;
    set_cookie->value.len = cookie_size;
    return 0;
}

char *lg_ngx_network_get_cookie(ngx_http_request_t *r, char *cookie)
{

    ngx_str_t name;
    name.data = (u_char *)cookie;
    name.len = strlen(cookie);

    ngx_str_t *value = ngx_pcalloc(r->pool, sizeof(ngx_str_t));
    if (ngx_http_parse_multi_header_lines(&r->headers_in.cookies, &name, value) == NGX_DECLINED)
	return NULL;

    return (char *)value->data;
}

int lg_ngx_network_add_session(ngx_http_request_t *r, char *key, char *session_id)
{

    char session[128] = {0};
    size_t session_size = sprintf(session, "%s=%s; Path=/action;", key, session_id);
    return lg_ngx_network_set_cookie(r, session, session_size);
}

int lg_ngx_network_get_session(ngx_http_request_t *r, char *key, char **session_id)
{

    char *cookie = NULL;
    if (!(cookie = lg_ngx_network_get_cookie(r, key)))
	return -1;

    *session_id = ngx_pcalloc(r->pool, 40);
    memset(*session_id, 0, 40);
    memcpy(*session_id, cookie, 36);
    return 0;
}

ngx_str_t *lg_ngx_network_get_post_buffer(ngx_http_request_t *r)
{

    if (!r->headers_in.content_length)
	return NULL;

    if (!r->headers_in.content_length->value.data || r->headers_in.content_length->value.len <= 0)
	return NULL;

    size_t content_length = atoi((char *)r->headers_in.content_length->value.data);
    if (content_length <= 0) return NULL;

    ngx_str_t *str = ngx_palloc(r->pool, sizeof(ngx_str_t));
    if (!str) return NULL;

    char *buffer = ngx_palloc(r->pool, content_length + 1);
    if (!buffer) return NULL;

    size_t size = 0;
    ngx_chain_t *p = NULL;
    for (p = r->request_body->bufs; p; p = p->next) {

	ngx_buf_t *node_buffer = p->buf;
	memcpy(&buffer[size], node_buffer->pos, node_buffer->last - node_buffer->pos);
	size += node_buffer->last - node_buffer->pos;

	if (size > content_length)
	    return NULL;
    }

    if (content_length != size)
	return NULL;

    buffer[size] = 0;
    str->data = (void *)buffer;
    str->len = size;
    return str;
}


