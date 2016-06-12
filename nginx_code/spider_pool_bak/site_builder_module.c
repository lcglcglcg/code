
#include "site_builder_module.h"

static void *ngx_site_builder_module_create_loc_conf(ngx_conf_t *cf);
static char *ngx_site_builder_module_init_loc_conf(ngx_conf_t *cf, void *parent, void *child);

static char *ngx_site_builder_module_command_init(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);
static ngx_int_t ngx_site_builder_module_handler(ngx_http_request_t *r);

static ngx_int_t ngx_site_builder_module_worker_init(ngx_cycle_t *cycle);
static void ngx_site_builder_module_worker_exit(ngx_cycle_t *cycle);

/* Commands */
static ngx_command_t  ngx_site_builder_module_commands[] = {
    {
	ngx_string("site_builder_module"),
	NGX_HTTP_LOC_CONF | NGX_CONF_NOARGS,
	ngx_site_builder_module_command_init,
	NGX_HTTP_LOC_CONF_OFFSET,
	0,
	NULL },
    {
	ngx_string("site_builder_module_template_dir"),
	NGX_HTTP_LOC_CONF | NGX_CONF_TAKE1,
	ngx_conf_set_str_slot,
	NGX_HTTP_LOC_CONF_OFFSET,
	offsetof(site_builder_loc_conf_t, template_dir),
	NULL },
    {
	ngx_string("site_builder_module_keyword_file"),
	NGX_HTTP_LOC_CONF | NGX_CONF_TAKE1,
	ngx_conf_set_str_slot,
	NGX_HTTP_LOC_CONF_OFFSET,
	offsetof(site_builder_loc_conf_t, keyword_file),
	NULL },
    {
	ngx_string("site_builder_module_content_file"),
	NGX_HTTP_LOC_CONF | NGX_CONF_TAKE1,
	ngx_conf_set_str_slot,
	NGX_HTTP_LOC_CONF_OFFSET,
	offsetof(site_builder_loc_conf_t, content_file),
	NULL },
    {
	ngx_string("site_builder_module_link_external_file"),
	NGX_HTTP_LOC_CONF | NGX_CONF_TAKE1,
	ngx_conf_set_str_slot,
	NGX_HTTP_LOC_CONF_OFFSET,
	offsetof(site_builder_loc_conf_t, link_external_file),
	NULL },
    {
	ngx_string("site_builder_module_link_group_file"),
	NGX_HTTP_LOC_CONF | NGX_CONF_TAKE1,
	ngx_conf_set_str_slot,
	NGX_HTTP_LOC_CONF_OFFSET,
	offsetof(site_builder_loc_conf_t, link_group_file),
	NULL },

    ngx_null_command
};

/* http module */
static ngx_http_module_t ngx_site_builder_module_ctx = {
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
ngx_module_t site_builder_module = {

    NGX_MODULE_V1,
    &ngx_site_builder_module_ctx,		/* module context */
    ngx_site_builder_module_commands,		/* module directives */
    NGX_HTTP_MODULE,				/* module type */
    NULL,					/* init master */
    NULL,					/* init module */
    ngx_site_builder_module_worker_init,	/* init process */
    NULL,					/* init thread */
    NULL,					/* exit thread */
    ngx_site_builder_module_worker_exit,	/* exit process */
    NULL,					/* exit master */
    NGX_MODULE_V1_PADDING
};

static void *ngx_site_builder_module_create_loc_conf(ngx_conf_t *cf)
{

    site_builder_loc_conf_t *loc_conf = ngx_pcalloc(cf->pool, sizeof(site_builder_loc_conf_t));
    if (loc_conf == NULL)
	return NGX_CONF_ERROR;

    memset(loc_conf, 0, sizeof(site_builder_loc_conf_t));
    return loc_conf;
}

static char *ngx_site_builder_module_init_loc_conf(ngx_conf_t *cf, void *parent, void *child)
{

    // 判断是否是 site_builder_module
    site_builder_loc_conf_t *loc_conf = ngx_http_conf_get_module_loc_conf(cf, site_builder_module);
    if (loc_conf->type) {

	return site_builder_init(cf, loc_conf);
    }

    return NGX_CONF_OK;
}

static char *ngx_site_builder_module_command_init(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{

    site_builder_loc_conf_t *loc_conf = (site_builder_loc_conf_t *)conf;
    loc_conf->type = 1;

    ngx_http_core_loc_conf_t *core_conf = ngx_http_conf_get_module_loc_conf(cf, ngx_http_core_module);
    core_conf->handler = ngx_site_builder_module_handler;

    return NGX_CONF_OK;
}

static ngx_int_t ngx_site_builder_module_handler(ngx_http_request_t *r)
{

    if (r->method != NGX_HTTP_GET)
	return NGX_ERROR;

    site_builder_loc_conf_t *loc_conf = ngx_http_get_module_loc_conf(r, site_builder_module);
    if (!loc_conf) return NGX_ERROR;

    return site_builder_filter(r, loc_conf);
}

static ngx_int_t ngx_site_builder_module_worker_init(ngx_cycle_t *cycle)
{

    site_builder_init_global_value(cycle);
    return NGX_OK;
}

static void ngx_site_builder_module_worker_exit(ngx_cycle_t *cycle)
{

}



