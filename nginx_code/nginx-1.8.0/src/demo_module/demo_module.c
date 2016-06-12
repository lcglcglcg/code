
#include <nginx.h>
#include <ngx_core.h>
#include <ngx_http.h>

#include "lg_leveldb.h"
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

void *p_db = NULL;
static ngx_int_t demo_module_handler(ngx_http_request_t *r)
{
    if (p_db == NULL) {

	p_db = lg_leveldb_open("/make/db_demo/demo");
	lg_leveldb_set(p_db, "lcglcg", "0");

    } 

    char *value = NULL;
    int size = lg_leveldb_get(p_db, "lcglcg", &value, r->pool);
    fprintf(stdout, "[%d][%d][%s]\n", getpid(), size, value);

    char buffer[1024];
    sprintf(buffer, "%d", atoi(value) + 1);
    lg_leveldb_set(p_db, "lcglcg", buffer);
    return lg_ngx_network_buffer_send(r, NULL, value, size);
}

static char *demo_module_init(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
    ngx_http_core_loc_conf_t *loc_conf = ngx_http_conf_get_module_loc_conf(cf, ngx_http_core_module);
    loc_conf->handler = demo_module_handler;
    return NGX_CONF_OK;
}



