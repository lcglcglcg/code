
#include "lg_rsa.h"
#include "lg_string.h"
#include "manager_account_module.h"

ngx_str_t *manager_account_wangmeng_rsa(ngx_http_request_t *r, char *packet_type, char *buffer, int size)
{

    // encode
    char temp[4096] = {0};
    size = lg_string_url_encode(temp, buffer, size);
    if (!size) return NULL;
    // fprintf(stdout, "[encode][%s]\n", temp);

    // rsa
    size = rsa_private_encrypt("/make/demo_rsa/rsa.key", buffer, temp);
    if (!size) return NULL;

    // encode
    size = lg_string_url_encode(temp, buffer, size);
    if (!size) return NULL;
    // fprintf(stdout, "[encode][%s]\n", temp);

    // json
    size = sprintf(buffer, "{\"PACKET_TYPE\":\"%s\",\"DATA\":\"%s\"}", packet_type, temp);
    // fprintf(stdout, "[json][%s]\n", buffer);

    // encode
    size = lg_string_url_encode(temp, buffer, size);
    if (!size) return NULL;
    // fprintf(stdout, "[encode][%s]\n", temp);

    ngx_str_t *str = ngx_palloc(r->pool, sizeof(ngx_str_t));
    if (!str) return NULL;

    str->data = ngx_palloc(r->pool, 4096);
    if (!str->data) return NULL;

    str->len = sprintf((char *)str->data, "server=%s", temp);
    // fprintf(stdout, "[get][%zd][%s]\n", str->len, (char *)str->data);
    return str;
}

ngx_str_t *manager_account_ruanwen_rsa(ngx_http_request_t *r, char *packet_type, char *buffer, int size)
{

    // encode
    char temp[4096] = {0};
    size = lg_string_url_encode(temp, buffer, size);
    if (!size) return NULL;
    // fprintf(stdout, "[encode][%s]\n", temp);

    // rsa
    size = rsa_private_encrypt("/make/demo_rsa/rsa.key", buffer, temp);
    if (!size) return NULL;

    // encode
    size = lg_string_url_encode(temp, buffer, size);
    if (!size) return NULL;
    // fprintf(stdout, "[encode][%s]\n", temp);

    // json
    size = sprintf(buffer, "{\"PACKET_TYPE\":\"%s\",\"DATA\":\"%s\"}", packet_type, temp);
    // fprintf(stdout, "[json][%s]\n", buffer);

    ngx_str_t *str = ngx_palloc(r->pool, sizeof(ngx_str_t));
    if (!str) return NULL;

    str->data = ngx_palloc(r->pool, 4096);
    if (!str->data) return NULL;

    str->len = lg_string_url_encode((char *)str->data, buffer, size);
    // fprintf(stdout, "[get][%zd][%s]\n", str->len, (char *)str->data);
    return str;
}







