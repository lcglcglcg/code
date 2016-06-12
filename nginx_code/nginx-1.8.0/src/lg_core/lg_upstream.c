
#include "lg_upstream.h"

#if 0
ngx_int_t lg_ngx_network_subrequest_handler(ngx_http_request_t *r, void *data, ngx_int_t rc)
{
    
    ngx_http_request_t *pr = r->parent;
    if(r->headers_out.status == NGX_HTTP_OK) {

ngx_buf_t *p = &r->upstream->buffer;
pr->subrequest_buffer.len = (p->last - p->pos);
//	pr->subrequest_buffer.data = ngx_palloc(pr->pool, pr->subrequest_buffer.len + 1);
//	if (!pr->subrequest_buffer.data)
//	    return NGX_ERROR;

//	memcpy(pr->subrequest_buffer.data, p->pos, pr->subrequest_buffer.len);
//	pr->subrequest_buffer.data[pr->subrequest_buffer.len] = 0;
    }
    
    fprintf(stdout, "lg_ngx_network_subrequest_handler[%zd][%zd]\n", rc, pr->subrequest_buffer.len);
    pr->headers_out.status = r->headers_out.status;
    pr->write_event_handler = data;
    // return NGX_OK;
    return rc;
}

void lg_ngx_network_subrequest_init(ngx_http_request_t *r,
	ngx_http_event_handler_pt fun,
	ngx_str_t *loc,
	ngx_str_t *args)
{

    fprintf(stdout, "lg_ngx_network_subrequest_init\n");

    ngx_http_request_t *sr = NULL;
    ngx_http_post_subrequest_t *psr = ngx_palloc(r->pool, sizeof(ngx_http_post_subrequest_t));
    if (psr == NULL) return ngx_http_finalize_request(r, NGX_ERROR);

    psr->data = fun;
    psr->handler = lg_ngx_network_subrequest_handler;
    ngx_int_t rc = ngx_http_subrequest(r, loc, args, &sr, psr, NGX_HTTP_SUBREQUEST_IN_MEMORY | NGX_HTTP_SUBREQUEST_WAITED);
    ngx_http_finalize_request(r, rc);
}

#endif


