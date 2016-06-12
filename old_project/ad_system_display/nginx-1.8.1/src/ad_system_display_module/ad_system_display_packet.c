
#include "lg_string.h"
#include "lg_ngx_uri.h"
#include "lg_ngx_network.h"
#include "ad_system_display_module.h"
#include "ad_system_display_packet.h"
#include "ad_system_display_db.h"

ad_system_kv_t *ad_system_kv_find(ngx_queue_t *head, char *key)
{
    if (!key) return NULL;

    ngx_queue_t *p = NULL;
    for (p = ngx_queue_head(head);
	    p != ngx_queue_sentinel(head);
	    p = ngx_queue_next(p)) {

	ad_system_kv_t *node = (ad_system_kv_t *)p;
	if (strcmp(node->key, key) == 0)
	    return node;
    }

    return NULL;
}

void ad_system_kv_print(ngx_queue_t *head)
{

    ngx_queue_t *p = NULL;
    for (p = ngx_queue_head(head);
	    p != ngx_queue_sentinel(head);
	    p = ngx_queue_next(p)) {

	ad_system_kv_t *node = (ad_system_kv_t *)p;
	if (node && node->value)
	    fprintf(stdout, "[%s][%s]\n", node->key, node->value);
	else
	    fprintf(stdout, "[%s][null]\n", node->key);
    }

    fprintf(stdout, "============================\n");
}

void ad_system_display_packet_parse_arg(ngx_http_request_t *r, char *buffer, ngx_queue_t *head)
{

    char *saveptr = NULL;
    char *str = strtok_r(buffer, "&", &saveptr);
    while(str) {

	ad_system_kv_t *node = ngx_pcalloc(r->pool, sizeof(ad_system_kv_t));
	node->key = str;
	node->value = lg_string_cutting(str, '=');
	ngx_queue_insert_tail(head, &node->next);

	str = strtok_r(NULL, "&", &saveptr);
    }
}

void ad_system_display_packet_parse_url(ngx_http_request_t *r, char *buffer, ngx_queue_t *head)
{

    char *saveptr = NULL;
    char *str = strtok_r(buffer, "./:", &saveptr);
    while(str) {

	if (strcmp(str, "http") != 0
		&& strcmp(str, "yyk") != 0
		&& strcmp(str, "39") != 0
		&& strcmp(str, "net") != 0) {

	    ad_system_kv_t *node = ngx_pcalloc(r->pool, sizeof(ad_system_kv_t));

	    if (strcmp(str, "doctors") == 0) {
		node->key = "db_type";
		node->value = "doctors";
		node->type = 10;

	    } else if (strcmp(str, "hospitals") == 0) {
		node->key = "db_type";
		node->value = "hospitals";
		node->type = 10;

	    } else {

		ad_system_code_t *code = ad_system_db_get_code(r, str);
		if (code) {

		    node->key = str;
		    node->value = code->code;
		    node->type = code->type;

		} else {
		    node->key = str;
		    node->value = NULL;
		    node->type = 0;
		}
	    }

	    ngx_queue_insert_tail(head, &node->next);
	}

	str = strtok_r(NULL, "./:", &saveptr);
    }
}

int ad_system_display_packet_parse_place(ngx_http_request_t *r, char *buffer, ngx_queue_t *head)
{

    int count = 0;
    char *saveptr = NULL;
    char *str = strtok_r(buffer, ",", &saveptr);
    while(str) {

	ad_system_kv_t *node = ngx_pcalloc(r->pool, sizeof(ad_system_kv_t));
	ad_system_code_t *code = ad_system_db_get_place(r, str);
	if (code) {
	    node->key = str;
	    node->value = code->code;
	    node->type = code->type;
	    count++;

	} else {
	    node->key = str;
	    node->value = NULL;
	    node->type = 0;
	}

	ngx_queue_insert_tail(head, &node->next);
	str = strtok_r(NULL, ",", &saveptr);
    }

    return count;
}

char *ad_system_display_packet_find_code(ngx_queue_t *head, int type)
{

    ngx_queue_t *p = NULL;
    for (p = ngx_queue_head(head);
	    p != ngx_queue_sentinel(head);
	    p = ngx_queue_next(p)) {

	ad_system_kv_t *node = (ad_system_kv_t *)p;
	if (node->value && node->type == type)
	    return node->value;
    }

    return NULL;
}

int ad_system_display_packet_parse(ngx_http_request_t *r, char *buffer, size_t size)
{

    ngx_queue_t arg_head;
    ngx_queue_t url_head;
    ngx_queue_t place_head;

    ngx_queue_init(&arg_head);
    ngx_queue_init(&url_head);
    ngx_queue_init(&place_head);

    lg_ngx_network_t network;
    memset(&network, 0, sizeof(lg_ngx_network_t));
    network.content_type.data = LG_NGX_URI_JS2_STR;
    network.content_type.len = LG_NGX_URI_JS2_SIZE;

    // arg parse
    ad_system_display_packet_parse_arg(r, buffer, &arg_head);
    // ad_system_kv_print(&arg_head);

    // url parse 
    ad_system_kv_t *node = ad_system_kv_find(&arg_head, "url");
    if (!node || !node->value) return lg_ngx_network_str_send(r, &network, "errCode:1");
    ad_system_display_packet_parse_url(r, node->value, &url_head);
    // ad_system_kv_print(&url_head);

    // get db_type region_code disease_code
    char *db_type = ad_system_display_packet_find_code(&url_head, 10);
    char *region_code = ad_system_display_packet_find_code(&url_head, 2);
    char *disease_code = ad_system_display_packet_find_code(&url_head, 3);
    if (!db_type || !region_code || !disease_code) return lg_ngx_network_str_send(r, &network, "errCode:1");

    // get cb (js back call)
    node = ad_system_kv_find(&arg_head, "cb");
    if (!node || !node->value) return lg_ngx_network_str_send(r, &network, "errCode:2");
    char *js_back_call = node->value;

    // place parse
    node = ad_system_kv_find(&arg_head, "cabin");
    if (!node || !node->value) return lg_ngx_network_str_send(r, &network, "errCode:3");
    if (ad_system_display_packet_parse_place(r, node->value, &place_head) == 0)
	return lg_ngx_network_str_send(r, &network, "errCode:3");
    // ad_system_kv_print(&place_head);

    // get ele 1:img 2:txt
    node = ad_system_kv_find(&arg_head, "ele");
    if (!node || !node->value) return lg_ngx_network_str_send(r, &network, "errCode:4");
    int ele = atoi(node->value);
    if (!(ele == 1 || ele == 2)) return lg_ngx_network_str_send(r, &network, "errCode:4");

    return ad_system_display_run(r, &network,
	    ele,
	    db_type,
	    js_back_call,
	    region_code,
	    disease_code,
	    &place_head);
}



