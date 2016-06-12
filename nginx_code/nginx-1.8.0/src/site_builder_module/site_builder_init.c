
#include "site_builder_module.h"
#include "site_builder_flush.h"

int site_builder_init_server_name(ngx_conf_t *cf, site_builder_loc_conf_t *loc_conf)
{

    // get server_name
    ngx_http_core_srv_conf_t *srv_conf = ngx_http_conf_get_module_srv_conf(cf, ngx_http_core_module);
    char *server_name = (char *)srv_conf->server_name.data;
    int server_name_len = srv_conf->server_name.len;
    if (server_name_len <= 0 || server_name[server_name_len - 1] == '/') {

	fprintf(stderr, "nginx.conf server_name: error!\n");
	return -1;
    }

    // get location_name
    ngx_http_core_loc_conf_t *core_conf = ngx_http_conf_get_module_loc_conf(cf, ngx_http_core_module);
    char *location_name = (char *)core_conf->name.data;
    int location_name_len = core_conf->name.len;
    if (location_name_len <= 0 || location_name[0] != '/') {

	fprintf(stderr, "nginx.conf location: error!\n");
	return -1;
    }

    // server_name malloc
    loc_conf->server_name = ngx_pcalloc(cf->pool, server_name_len + 1);
    if (!loc_conf->server_name) {

	fprintf(stderr, "ngx_pcalloc: error!\n");
	return -1;
    }

    // location_name malloc
    loc_conf->location_name = ngx_pcalloc(cf->pool, location_name_len + 2);// last != '/'
    if (!loc_conf->location_name) {

	fprintf(stderr, "ngx_pcalloc: error!\n");
	return -1;
    }

    // server_location_name malloc
    loc_conf->server_location_name = ngx_pcalloc(cf->pool, server_name_len + location_name_len + 1);
    if (!loc_conf->server_location_name) {

	fprintf(stderr, "ngx_pcalloc: error!\n");
	return -1;
    }

    // server_name copy
    memcpy(loc_conf->server_name, srv_conf->server_name.data, server_name_len);
    loc_conf->server_name_len = server_name_len;
    loc_conf->server_name[server_name_len] = 0;
    // fprintf(stdout, "[S][%s]\n", loc_conf->server_name);

    // location_name copy
    memcpy(loc_conf->location_name, core_conf->name.data, location_name_len);
    if (loc_conf->location_name[location_name_len - 1] != '/') {

	loc_conf->location_name[location_name_len] = '/';
	location_name_len++;
    }
    loc_conf->location_name_len = location_name_len;
    loc_conf->location_name[location_name_len] = 0;
    // fprintf(stdout, "[L][%s]\n", loc_conf->location_name);

    // server_location_name copy
    memcpy(loc_conf->server_location_name, loc_conf->server_name, server_name_len);
    memcpy(&loc_conf->server_location_name[server_name_len], loc_conf->location_name, location_name_len);
    loc_conf->server_location_name_len = server_name_len + location_name_len;
    loc_conf->server_location_name[loc_conf->server_location_name_len] = 0;
    // fprintf(stdout, "[A][%s]\n", loc_conf->server_location_name);
    return 0;
}

char *site_builder_init(ngx_conf_t *cf, site_builder_loc_conf_t *loc_conf)
{

    if (site_builder_init_server_name(cf, loc_conf)) 
	return NGX_CONF_ERROR;

    if (loc_conf->template_id.len == 0
	    || loc_conf->keyword_dict.len == 0
	    || loc_conf->content_dict.len == 0
	    || loc_conf->link_external_dict.len == 0
       ) {

	fprintf(stderr, "nginx.conf Configuration site_builder_module: lack!\n");
	return NGX_CONF_ERROR;
    }

    /***************/
    /* init config */
    /***************/

    fprintf(stdout, "site builder init -->[%s%s]: Loading ...\n", loc_conf->server_name, loc_conf->location_name);

    // load template
    if (site_builder_template_load(
		(char *)loc_conf->template_id.data,
		&loc_conf->template_index,
		&loc_conf->template_content,
		&loc_conf->affiliated_file,
		cf->pool)) {

	fprintf(stderr, "load template [%s]: error!\n", (char *)loc_conf->template_id.data);
	return NGX_CONF_ERROR;
    }

    // load keyword
    loc_conf->keyword = site_builder_dict_keyword(
	    (char *)loc_conf->keyword_dict.data,
	    cf->pool);
    if (!loc_conf->keyword) {

	fprintf(stderr, "load keyword [%s]: error!\n", (char *)loc_conf->keyword_dict.data);
	return NGX_CONF_ERROR;
    }

    // load content
    loc_conf->content = site_builder_dict_content(
	    (char *)loc_conf->content_dict.data,
	    cf->pool);
    if (!loc_conf->content) {

	fprintf(stderr, "load content [%s]: error!\n", (char *)loc_conf->content_dict.data);
	return NGX_CONF_ERROR;
    }

    // load link_external
    loc_conf->link_external = site_builder_dict_link_external(
	    (char *)loc_conf->link_external_dict.data,
	    cf->pool);
    if (!loc_conf->link_external) {

	fprintf(stderr, "load link_external [%s]: error!\n", (char *)loc_conf->link_external_dict.data);
	return NGX_CONF_ERROR;
    }

    fprintf(stdout, "site builder init -->[%s%s]: OK...\n", loc_conf->server_name, loc_conf->location_name);
    return NGX_CONF_OK;
}

ngx_int_t site_builder_global_init(ngx_cycle_t *cycle)
{

    site_builder_flush_init_base(cycle->pool);
    return NGX_OK;
}





