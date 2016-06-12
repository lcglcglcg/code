
#include "lg_string.h"
#include "lg_captcha.h"
#include "lg_ngx_network.h"
#include "manager_account_module.h"

static ngx_int_t ngx_manager_account_module_worker_init(ngx_cycle_t *cycle);
static char *ngx_manager_account_module_command_init(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);
static ngx_int_t ngx_manager_account_module_handler(ngx_http_request_t *r);
static void ngx_manager_account_module_post_handler(ngx_http_request_t *r);

extern int manager_account_init();

/* Commands */
static ngx_command_t  ngx_manager_account_module_commands[] = {
    {
	ngx_string("manager_account_module"),
	NGX_HTTP_LOC_CONF | NGX_CONF_NOARGS,
	ngx_manager_account_module_command_init,
	NGX_HTTP_LOC_CONF_OFFSET,
	0,
	NULL },

    ngx_null_command
};

/* http module */
static ngx_http_module_t ngx_manager_account_module_ctx = {
    NULL,					/* preconfiguration */
    NULL,					/* postconfiguration */
    NULL,					/* create main configuration */
    NULL,					/* init main configuration */
    NULL,					/* create server configuration */
    NULL,					/* merge server configuration */
    NULL,					/* create location configuration */
    NULL					/* merge location configuration */
};

/* hook */
ngx_module_t manager_account_module = {

    NGX_MODULE_V1,
    &ngx_manager_account_module_ctx,		/* module context */
    ngx_manager_account_module_commands,	/* module directives */
    NGX_HTTP_MODULE,				/* module type */
    NULL,					/* init master */
    NULL,					/* init module */
    ngx_manager_account_module_worker_init,	/* init process */
    NULL,					/* init thread */
    NULL,					/* exit thread */
    NULL,					/* exit process */
    NULL,					/* exit master */
    NGX_MODULE_V1_PADDING
};

static ngx_int_t ngx_manager_account_module_worker_init(ngx_cycle_t *cycle)
{

    if (manager_account_init())
	return NGX_ERROR;
    return NGX_OK;
}

static char *ngx_manager_account_module_command_init(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{

    ngx_http_core_loc_conf_t *core_conf = ngx_http_conf_get_module_loc_conf(cf, ngx_http_core_module);
    core_conf->handler = ngx_manager_account_module_handler;

    lg_captcha_init(NULL);
    return NGX_CONF_OK;
}

static ngx_int_t ngx_manager_account_module_handler(ngx_http_request_t *r)
{

    if (r->method == NGX_HTTP_GET)
	return manager_account_packet_var_code(r);

    if (r->method != NGX_HTTP_POST)
	return NGX_ERROR;

    ngx_int_t rc = ngx_http_read_client_request_body(r, ngx_manager_account_module_post_handler);
    if (rc >= NGX_HTTP_SPECIAL_RESPONSE)
	return rc; 

    return NGX_DONE;
}

static void ngx_manager_account_module_post_handler(ngx_http_request_t *r)
{
    
    manager_account_module_request_t *m_request = ngx_pcalloc(r->pool, sizeof(manager_account_module_request_t));
    if (!m_request) return ngx_http_finalize_request(r, NGX_ERROR); 

    memset(m_request, 0, sizeof(manager_account_module_request_t));
    lg_ngx_network_get_session(r, "MAKEY", &m_request->session_id);

    r->m_request = m_request;
    return manager_account_packet_parse(r, r->m_request);
}


