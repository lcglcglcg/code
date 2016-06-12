
#include <nginx.h>
#include <ngx_core.h>
#include <ngx_http.h>

#include "lg_var_code.h"
#include "lg_ngx_network.h"

int redis_conn = 0;
static char *demo_module_init(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);
void demo_module_set_upstream_conf(ngx_http_upstream_conf_t *conf);
static ngx_int_t demo_module_worker_init(ngx_cycle_t *cycle);
static void demo_module_worker_exit(ngx_cycle_t *cycle);
static ngx_int_t demo_module_handler(ngx_http_request_t *r);

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
    demo_module_worker_init,	/* init process */
    NULL,			/* init thread */
    NULL,			/* exit thread */
    demo_module_worker_exit,	/* exit process */
    NULL,			/* exit master */
    NGX_MODULE_V1_PADDING
};

static ngx_int_t ngx_http_redis2_create_request(ngx_http_request_t *r)
{

    char *buffer = 
	"*2\r\n"
	"$3\r\n"
	"get\r\n"
	"$6\r\n"
	"lcglcg\r\n";

    size_t size = strlen(buffer);
    ngx_buf_t *b = ngx_create_temp_buf(r->pool, size);
    memcpy(b->pos, buffer, size);
    b->last = ngx_copy(b->pos, buffer, size);

    ngx_chain_t *cl = ngx_alloc_chain_link(r->pool);
    cl->buf = b;
    cl->next = NULL;
    cl->buf->memory = 1;
    cl->buf->last_buf = 1;
    r->upstream->request_bufs = cl;
    r->header_hash = 1;

    return NGX_OK;
}

int count = 0;
static ngx_int_t ngx_http_redis2_process_header(ngx_http_request_t *r)
{

    ngx_http_upstream_t *u = r->upstream;
    ngx_buf_t *b = &u->buffer;

    if (b->last - b->pos < (ssize_t) sizeof(u_char)) {
	return NGX_AGAIN;
    }

    fprintf(stdout, "[recv][%d]=%p\n", count, r);
    // fprintf(stdout, "%.*s\n", (int)(b->last - b->pos), b->pos);

    // return demo_module_handler(r);
    return lg_ngx_network_buffer_send(r, NULL, (char *)b->pos, (size_t)(b->last - b->pos));
}

static void ngx_http_redis2_finalize_request(ngx_http_request_t *r, ngx_int_t rc)
{

    // if (rc >= NGX_HTTP_SPECIAL_RESPONSE)
    //	r->headers_out.status = rc;
    fprintf(stdout, "[free][%d]=%p\n\n", count++, r);
    r->headers_out.status = NGX_OK;
}

ngx_http_upstream_conf_t demo_upstream_conf;
static ngx_int_t demo_module_handler(ngx_http_request_t *r)
{

    fprintf(stdout, "%d::%d\n", getpid(), redis_conn);
    return lg_ngx_network_buffer_send(r, NULL, "ok", 2);
    fprintf(stdout, "[demo][%d]=%p\n", count, r);

    if (ngx_http_set_content_type(r) != NGX_OK) {
	return NGX_HTTP_INTERNAL_SERVER_ERROR;
    }

    if (ngx_http_upstream_create(r) != NGX_OK) {
	return NGX_HTTP_INTERNAL_SERVER_ERROR;
    }

    ngx_http_upstream_t *u = r->upstream;
    u->create_request = ngx_http_redis2_create_request; //准备数据
    u->process_header = ngx_http_redis2_process_header; //返回数据
    u->finalize_request = ngx_http_redis2_finalize_request; //摧毁子请求

    u->resolved = ngx_pcalloc(r->pool, sizeof(ngx_http_upstream_resolved_t));
    if (u->resolved == NULL)
	return NGX_ERROR;

    ngx_str_t addr = ngx_string("127.0.0.1");
    u->resolved->host = addr;
    u->resolved->port = 6379;

    demo_module_set_upstream_conf(&demo_upstream_conf);
    u->conf = &demo_upstream_conf;

    ngx_int_t rc = ngx_http_read_client_request_body(r, ngx_http_upstream_init);
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

static ngx_int_t demo_module_worker_init(ngx_cycle_t *cycle)
{

    if ((redis_conn = socket(AF_INET, SOCK_STREAM, 0)) == -1) {

	return NGX_ERROR;
    }

    struct sockaddr_in addr;
    bzero(&addr, sizeof(struct sockaddr_in));
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr.sin_family = AF_INET;
    addr.sin_port = htons(6379);
    if (connect(redis_conn, (struct sockaddr *)&addr, sizeof(addr))) {

	return NGX_ERROR;
    }

    fprintf(stdout, "%d::%d\n", getpid(), redis_conn);
    return NGX_OK;
}

static void demo_module_worker_exit(ngx_cycle_t *cycle)
{

}

void demo_module_set_upstream_conf(ngx_http_upstream_conf_t *conf)
{

    memset(conf, 0, sizeof(ngx_http_upstream_conf_t));
    conf->connect_timeout = 1000;
    conf->send_timeout = 1000;
    conf->read_timeout = 1000;
    conf->buffering = 1;
    conf->buffer_size = ngx_pagesize;
    conf->hide_headers = NGX_CONF_UNSET_PTR;
    conf->pass_headers = NGX_CONF_UNSET_PTR;
}


