
#include "lg_json.h"
#include "lg_string.h"
#include "lg_ngx_uri.h"
#include "lg_ngx_network.h"
#include "ad_system_display_module.h"
#include "ad_system_display_packet.h"
#include "ad_system_display_db.h"


int ad_system_display_send_img(ngx_http_request_t *r, lg_ngx_network_t *network,
	char *js_back_call,
	char *region_code,
	ngx_queue_t *place_head,
	ngx_queue_t *info_head)
{

    char buffer[40960];
    size_t size = sprintf(buffer, "%s({\"data\":[", js_back_call);
    lg_ngx_network_buffer_add(r, network, buffer, size);

    int index = 1;
    char comma[4] = "{";
    ngx_queue_t *p = NULL;
    for (p = ngx_queue_head(info_head);
	    p != ngx_queue_sentinel(info_head);
	    p = ngx_queue_next(p)) {

	ad_system_info_img_t *info = (ad_system_info_img_t *)p;
	size = lg_json_sprintf(buffer, comma, "}", 9,

		"%s",	"region",		region_code,
		"%s",	"displayUrl",		info->display_url,
		"%u",	"num",			index++,
		"%s",	"imgSize",		info->img_size,
		"%s",	"webIM",		info->webim,
		"%s",	"creativeId",		info->creative_id,
		"%s",	"filePath",		info->img_path,
		"%s",	"destinationUrl",	info->destination_url,
		"%s",	"act",			info->account_id);

	strcpy(comma, ",{");
	lg_ngx_network_buffer_add(r, network, buffer, size);
    }

    size = sprintf(buffer, "],\"targetUrl\":\"%s\"});", "http://www.baidu.com");
    lg_ngx_network_buffer_add(r, network, buffer, size);
    return lg_ngx_network_send(r, network);
}

int ad_system_display_send_txt(ngx_http_request_t *r, lg_ngx_network_t *network, 
	char *js_back_call,
	char *region_code,
	ngx_queue_t *info_head)
{

    char buffer[40960];
    size_t size = sprintf(buffer, "%s({\"data\":[", js_back_call);
    lg_ngx_network_buffer_add(r, network, buffer, size);

    int index = 1;
    char comma[4] = "{";
    ngx_queue_t *p = NULL;
    for (p = ngx_queue_head(info_head);
	    p != ngx_queue_sentinel(info_head);
	    p = ngx_queue_next(p)) {

	ad_system_info_txt_t *info = (ad_system_info_txt_t *)p;
	size = lg_json_sprintf(buffer, comma, "}", 9,

		"%s",	"region",		region_code,
		"%s",	"displayUrl",		info->display_url,
		"%s",	"num",			index++,
		"%s",	"title",		info->title,
		"%s",	"webIM",		info->webim,
		"%s",	"creativeId",		info->creative_id,
		"%s",	"description1",		info->description,
		"%s",	"destinationUrl",	info->destination_url,
		"%s",	"act",			info->account_id);

	strcpy(comma, ",{");
	lg_ngx_network_buffer_add(r, network, buffer, size);
    }

    size = sprintf(buffer, "],\"targetUrl\":\"%s\"});", "http://www.baidu.com");
    lg_ngx_network_buffer_add(r, network, buffer, size);
    return lg_ngx_network_send(r, network);
}

int ad_system_display_run(ngx_http_request_t *r, lg_ngx_network_t *network,
	int ele_type,
	char *db_type,
	char *js_back_call,
	char *region_code,
	char *disease_code,
	ngx_queue_t *place_head)
{

    fprintf(stdout, "[ele_type][%d]\n", ele_type);
    fprintf(stdout, "[db_type][%s]\n", db_type);
    fprintf(stdout, "[region_code][%s]\n", region_code);
    fprintf(stdout, "[disease_code][%s]\n", disease_code);
    fprintf(stdout, "[js_back_call][%s]\n", js_back_call);
    ad_system_kv_print(place_head);

    if (ele_type == 1) {

	ngx_queue_t *info = ad_system_db_get_info_txt(r, region_code, disease_code);
	if (info) return ad_system_display_send_txt(r, network,
		js_back_call,
		region_code,
		info);

    } else if (ele_type == 2) {

	ngx_queue_t *info = ad_system_db_get_info_img(r, region_code, disease_code);
	fprintf(stdout, "info=%p\n", info);
	if (info) return ad_system_display_send_img(r, network,
		js_back_call,
		region_code,
		place_head,
		info);
    }

    return lg_ngx_network_str_send(r, network, "errCode:5");
}



