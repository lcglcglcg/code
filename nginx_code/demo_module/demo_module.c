
#include <nginx.h>
#include <ngx_core.h>
#include <ngx_http.h>

#include "lg_var_code.h"
#include "lg_ngx_network.h"

static char *demo_module_init(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);

/* Commands */
static ngx_command_t  demo_module_commands[] = {
    { ngx_string("demo_module"),
	NGX_HTTP_LOC_CONF | NGX_CONF_NOARGS,
	demo_module_init,
	NGX_HTTP_LOC_CONF_OFFSET,
	0,
	NULL },

    ngx_null_command
};

static ngx_http_module_t demo_module_ctx = {
    NULL,			/* preconfiguration */
    NULL,			/* postconfiguration */
    NULL,			/* create main configuration */
    NULL,			/* init main configuration */
    NULL,			/* create server configuration */
    NULL,			/* merge server configuration */
    NULL,			/* create location configuration */
    NULL			/* merge location configuration */
};

/* hook */
ngx_module_t demo_module = {
    NGX_MODULE_V1,
    &demo_module_ctx,	/* module context */
    demo_module_commands,	/* module directives */
    NGX_HTTP_MODULE,		/* module type */
    NULL,			/* init master */
    NULL,			/* init module */
    NULL,			/* init process */
    NULL,			/* init thread */
    NULL,			/* exit thread */
    NULL,			/* exit process */
    NULL,			/* exit master */
    NGX_MODULE_V1_PADDING
};

char *demo_module_post_get_buffer(ngx_http_request_t *r)
{

    size_t content_length = atoi((char *)r->headers_in.content_length->value.data);
    if (!content_length) return NULL;

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

    buffer[size] = 0;
    if (content_length != size)
	return NULL;

    return buffer;
}

static void demo_module_post_handler(ngx_http_request_t *r)
{

    fprintf(stdout, "[demo][get][recv][%s]%s\n", 
	    lg_ngx_network_get_ipaddr(r),
	    demo_module_post_get_buffer(r));

    char buffer[128];
    size_t size = sprintf(buffer, "{\"code\":\"0\"}");
    return lg_ngx_network_buffer_post_send(r, NULL, buffer, size);
}

static ngx_int_t demo_module_handler(ngx_http_request_t *r)
{

    if (r->method == NGX_HTTP_GET) {

	fprintf(stdout, "[demo][get][recv][%s]%.*s\n", 
		lg_ngx_network_get_ipaddr(r),
		(int)r->args.len, r->args.data);

	char buffer[128];
	size_t size = sprintf(buffer, "{\"code\":\"0\"}");
	return lg_ngx_network_buffer_send(r, NULL, buffer, size);
    }

    if (r->method != NGX_HTTP_POST)
	return NGX_ERROR;

    ngx_int_t rc = ngx_http_read_client_request_body(r, demo_module_post_handler);
    if (rc >= NGX_HTTP_SPECIAL_RESPONSE)
	return rc; 

    return NGX_DONE;
}

static char *demo_module_init(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
    ngx_http_core_loc_conf_t *loc_conf = ngx_http_conf_get_module_loc_conf(cf, ngx_http_core_module);
    loc_conf->handler = demo_module_handler;
    return NGX_CONF_OK;
}


