
#include "manager_site_builder_module.h"

static void *ngx_site_builder_module_create_loc_conf(ngx_conf_t *cf);
static char *ngx_site_builder_module_init_loc_conf(ngx_conf_t *cf, void *parent, void *child);
static char *ngx_manager_site_builder_module_command_init(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);

/* Commands */
static ngx_command_t  ngx_manager_site_builder_module_commands[] = {
    {
	ngx_string("manager_site_builder_module"),
	NGX_HTTP_LOC_CONF | NGX_CONF_NOARGS,
	ngx_manager_site_builder_module_command_init,
	NGX_HTTP_LOC_CONF_OFFSET,
	0,
	NULL },

    ngx_null_command
};

/* http module */
static ngx_http_module_t ngx_manager_site_builder_module_ctx = {
    NULL,					/* preconfiguration */
    NULL,					/* postconfiguration */
    NULL,					/* create main configuration */
    NULL,					/* init main configuration */
    NULL,					/* create server configuration */
    NULL,					/* merge server configuration */
    ngx_site_builder_module_create_loc_conf,	/* create location configuration */
    ngx_site_builder_module_init_loc_conf	/* merge location configuration */
};

/* hook */
ngx_module_t manager_site_builder_module = {

    NGX_MODULE_V1,
    &ngx_manager_site_builder_module_ctx,	/* module context */
    ngx_manager_site_builder_module_commands,	/* module directives */
    NGX_HTTP_MODULE,				/* module type */
    NULL,					/* init master */
    NULL,					/* init module */
    NULL,					/* init process */
    NULL,					/* init thread */
    NULL,					/* exit thread */
    NULL,					/* exit process */
    NULL,					/* exit master */
    NGX_MODULE_V1_PADDING
};

static void *ngx_site_builder_module_create_loc_conf(ngx_conf_t *cf)
{

    ngx_manager_site_builder_conf_t *loc_conf = ngx_pcalloc(cf->pool, sizeof(ngx_manager_site_builder_conf_t));
    if (loc_conf == NULL) return NGX_CONF_ERROR;

    memset(loc_conf, 0, sizeof(ngx_manager_site_builder_conf_t));
    return loc_conf;
}

static char *ngx_site_builder_module_init_loc_conf(ngx_conf_t *cf, void *parent, void *child)
{

    // ngx_site_builder_module check
    ngx_manager_site_builder_conf_t *loc_conf = ngx_http_conf_get_module_loc_conf(cf, manager_site_builder_module);
    if (loc_conf->type) {

	// ...
    }

    return NGX_CONF_OK;
}

static void ngx_manager_site_builder_module_post_handler(ngx_http_request_t *r)
{

    ngx_str_t *buffer = lg_ngx_network_get_post_buffer(r);
    if (!buffer) lg_ngx_network_not_found(r);

    return manager_site_builder_packet(r, (char *)buffer->data, buffer->len);
}

static ngx_int_t ngx_manager_site_builder_module_handler(ngx_http_request_t *r)
{

    if (r->method != NGX_HTTP_POST)
	return NGX_ERROR;

    ngx_int_t rc = ngx_http_read_client_request_body(r, ngx_manager_site_builder_module_post_handler);
    if (rc >= NGX_HTTP_SPECIAL_RESPONSE)
	return rc; 

    return NGX_DONE;
}

static char *ngx_manager_site_builder_module_command_init(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{

    ngx_manager_site_builder_conf_t *loc_conf = (ngx_manager_site_builder_conf_t *)conf;
    loc_conf->type = 1;

    ngx_http_core_loc_conf_t *core_conf = ngx_http_conf_get_module_loc_conf(cf, ngx_http_core_module);
    core_conf->handler = ngx_manager_site_builder_module_handler;

    return NGX_CONF_OK;
}



