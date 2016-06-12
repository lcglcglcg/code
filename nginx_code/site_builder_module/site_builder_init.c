
#include "site_builder_module.h"
#include "site_builder_cache.h"
#include "site_builder_flush.h"

/* global element */
ngx_queue_t global_keyword_list = {&global_keyword_list, &global_keyword_list};
int global_keyword_list_count = 0;

ngx_queue_t global_content_list = {&global_content_list, &global_content_list};
int global_content_list_count = 0;

int site_builder_init_server_name(ngx_conf_t *cf, site_builder_loc_conf_t *loc_conf)
{
    // init server_name
    ngx_http_core_srv_conf_t *srv_conf = ngx_http_conf_get_module_srv_conf(cf, ngx_http_core_module);
    if (srv_conf->server_name.len == 0) {

	fprintf(stderr, "nginx.conf Configuration Not server_name: error!\n");
	return -1;
    }

    // init location name
    ngx_http_core_loc_conf_t *core_conf = ngx_http_conf_get_module_loc_conf(cf, ngx_http_core_module);
    if (core_conf->name.len == 0 || *((char *)core_conf->name.data) != '/') {

	fprintf(stderr, "nginx.conf Configuration [%.*s] location: error!\n", 
		(int)srv_conf->server_name.len,
		(char *)srv_conf->server_name.data);
	return -1;
    }

    char *server_name = (char *)srv_conf->server_name.data;
    int server_name_len = srv_conf->server_name.len;

    char *location_name = (char *)core_conf->name.data;
    int location_name_len = core_conf->name.len;

    if (server_name[server_name_len - 1] == '/') server_name_len--;
    if (location_name[location_name_len - 1] == '/') location_name_len--;
    if (server_name_len <= 0 || location_name_len < 0) {

	fprintf(stderr, "nginx.conf Configuration: error!\n");
	return -1;
    }

    loc_conf->server_name = ngx_pcalloc(cf->pool, server_name_len + 1);
    if (!loc_conf->server_name) {

	fprintf(stderr, "ngx_pcalloc: error!\n");
	return -1;
    }

    loc_conf->location_name = ngx_pcalloc(cf->pool, location_name_len + 1);
    if (!loc_conf->location_name) {

	fprintf(stderr, "ngx_pcalloc: error!\n");
	return -1;
    }

    loc_conf->server_location_name = ngx_pcalloc(cf->pool, server_name_len + location_name_len + 1);
    if (!loc_conf->server_location_name) {

	fprintf(stderr, "ngx_pcalloc: error!\n");
	return -1;
    }

    loc_conf->server_name_len = server_name_len;
    memcpy(loc_conf->server_name, srv_conf->server_name.data, server_name_len);
    loc_conf->server_name[server_name_len] = 0;
    // fprintf(stdout, "[S][%s]\n", loc_conf->server_name);

    loc_conf->location_name_len = location_name_len;
    memcpy(loc_conf->location_name, core_conf->name.data, location_name_len);
    loc_conf->location_name[location_name_len] = 0;
    // fprintf(stdout, "[L][%s]\n", loc_conf->location_name);

    loc_conf->server_location_name_len = server_name_len + location_name_len;
    memcpy(loc_conf->server_location_name, loc_conf->server_name, server_name_len);
    memcpy(&loc_conf->server_location_name[server_name_len], loc_conf->location_name, location_name_len);
    loc_conf->server_location_name[server_name_len + location_name_len] = 0;
    // fprintf(stdout, "[A][%s]\n", loc_conf->server_location_name);
    return 0;
}

char *site_builder_init(ngx_conf_t *cf, site_builder_loc_conf_t *loc_conf)
{
    int i;

    if (site_builder_init_server_name(cf, loc_conf)) 
	return NGX_CONF_ERROR;

    if (loc_conf->template_dir.len == 0
	    || loc_conf->keyword_file.len == 0
	    || loc_conf->content_file.len == 0
	    || loc_conf->link_external_file.len == 0
	    //	    || loc_conf->mate_file.len == 0
	    //	    || loc_conf->subdomain_file.len == 0
       ) {

	fprintf(stderr, "nginx.conf Configuration [server_name:%s]->[location:%s] site_builder_module: lack!\n",
		loc_conf->server_name,
		loc_conf->location_name_len > 0 ? loc_conf->location_name : "/");
	return NGX_CONF_ERROR;
    }

    /********************/
    /* init config data */
    /********************/

    /* init element */
    site_builder_element_init(&loc_conf->affiliated_file, cf->pool);
    site_builder_element_init(&loc_conf->keyword, cf->pool);
    site_builder_element_init(&loc_conf->content, cf->pool);
    site_builder_element_init(&loc_conf->link_external, cf->pool);
    site_builder_element_init(&loc_conf->link_bare, cf->pool);
    site_builder_element_init(&loc_conf->meta, cf->pool);
    site_builder_element_init(&loc_conf->link_subdomain, cf->pool);

    /* set global element name */
    loc_conf->keyword.name = loc_conf->server_location_name;
    loc_conf->content.name = loc_conf->server_location_name;

    /* add global element */
    global_keyword_list_count++;
    global_content_list_count++;
    ngx_queue_insert_tail(&global_keyword_list, &loc_conf->keyword.next);
    ngx_queue_insert_tail(&global_content_list, &loc_conf->content.next);

    /* get global Pointer */
    loc_conf->global_keyword_list = &global_keyword_list;
    loc_conf->global_content_list = &global_content_list;
    loc_conf->global_keyword_list_count = &global_keyword_list_count;
    loc_conf->global_content_list_count = &global_content_list_count;

    /********************/
    /* load config data */
    /********************/

    fprintf(stdout, "site builder init -->[%s]: Loading ...\n", loc_conf->server_location_name);

    // load template
    if (site_builder_template_dir_load(
		(char *)loc_conf->template_dir.data,
		loc_conf->template_dir.len,
		&loc_conf->template_list,
		&loc_conf->affiliated_file,
		cf->pool)) {

	fprintf(stderr, "load template [%s]: error!\n", (char *)loc_conf->template_dir.data);
	return NGX_CONF_ERROR;
    }

    if (loc_conf->template_list.template_total <= 0) {

	fprintf(stderr, "not template file[%s]: error!\n", (char *)loc_conf->template_dir.data);
	return NGX_CONF_ERROR;
    }

    // load keyword
    if (site_builder_element_load_file(
		(char *)loc_conf->keyword_file.data,
		&loc_conf->keyword,
		cf->pool)) {

	fprintf(stderr, "load keyword [%s]: error!\n", (char *)loc_conf->keyword_file.data);
	return NGX_CONF_ERROR;
    }

    // load content
    if (site_builder_element_load_file(
		(char *)loc_conf->content_file.data,
		&loc_conf->content,
		cf->pool)) {

	fprintf(stderr, "load content [%s]: error!\n", (char *)loc_conf->content_file.data);
	return NGX_CONF_ERROR;
    }

    // load link_external
    if (site_builder_element_load_link_external_file(
		(char *)loc_conf->link_external_file.data,
		&loc_conf->link_external,
		cf->pool)) {

	fprintf(stderr, "load link_external [%s]: error!\n", (char *)loc_conf->link_external_file.data);
	return NGX_CONF_ERROR;
    }

    // load link_bare
    if (site_builder_element_load_link_bare_file(
		(char *)loc_conf->link_external_file.data,
		&loc_conf->link_bare,
		cf->pool)) {

	fprintf(stderr, "load link_external [%s]: error!\n", (char *)loc_conf->link_external_file.data);
	return NGX_CONF_ERROR;
    }

    // load mate
    if (loc_conf->mate_file.len > 0) {

	if (site_builder_element_load_file(
		    (char *)loc_conf->mate_file.data,
		    &loc_conf->meta,
		    cf->pool)) {

	    fprintf(stderr, "load mate file [%s]: error!\n", (char *)loc_conf->mate_file.data);
	    return NGX_CONF_ERROR;
	}
    }

    // load link_subdomain
    if (loc_conf->link_subdomain_file.len > 0) {

	if (site_builder_element_load_file(
		    (char *)loc_conf->link_subdomain_file.data,
		    &loc_conf->link_subdomain,
		    cf->pool)) {

	    fprintf(stderr, "load link_subdomain file [%s]: error!\n", (char *)loc_conf->link_subdomain_file.data);
	    return NGX_CONF_ERROR;
	}
    }

    // init link group
    ngx_queue_init(&loc_conf->link_group_config);
    for (i = 0; i < SITE_BUILDER_TEMPLATE_GROUP_COUNT; i++) {

	if (loc_conf->link_group[i].len > 0) {

	    link_group_config_t *group_node = ngx_pcalloc(cf->pool, sizeof(link_group_config_t));
	    if (!group_node) {

		fprintf(stderr, "ngx_pcalloc: error!\n");
		return NGX_CONF_ERROR;
	    }

	    group_node->index = i;
	    group_node->link_group_name = ngx_pcalloc(cf->pool, loc_conf->link_group[i].len + 1);
	    if (!group_node->link_group_name) {

		fprintf(stderr, "ngx_pcalloc: error!\n");
		return NGX_CONF_ERROR;
	    }

	    memcpy(group_node->link_group_name, loc_conf->link_group[i].data, loc_conf->link_group[i].len);
	    group_node->link_group_name[loc_conf->link_group[i].len] = 0;

	    ngx_queue_insert_tail(&loc_conf->link_group_config, &group_node->next);
	}
    }

#if 0
    ngx_queue_t *p = NULL;
    for (p = ngx_queue_head(&loc_conf->link_group_config); 
	    p != ngx_queue_sentinel(&loc_conf->link_group_config); 
	    p = ngx_queue_next(p)) {

	link_group_config_t *node = (link_group_config_t *)p;
	fprintf(stdout, "[guoup%d][%s]\n", node->index, node->link_group_name);
    }
#endif

#if 0
    /***************/
    /* cache init */
    /***************/
    loc_conf->title_list_flush = &global_title_list_flush;
    loc_conf->keyword_flush = &global_keyword_list_flush;
    loc_conf->content_flush = &global_content_list_flush;

    loc_conf->link_save_internal_list_flush = &global_link_save_internal_list_flush;
    loc_conf->link_save_external_list_flush = &global_link_save_external_list_flush;
    loc_conf->link_save_subdomain_list_flush = &global_link_save_subdomain_list_flush;
    loc_conf->link_save_bare_list_flush = &global_link_save_bare_list_flush;
#endif

#if 0
    /**************/
    /* flush init */
    /**************/
    if (site_builder_flush_init()) {

	fprintf(stderr, "flush init: error!\n");
	return NGX_CONF_ERROR;
    }
#endif
    if (site_builder_flush_check()) {

	fprintf(stderr, "flush init: error!\n");
	return NGX_CONF_ERROR;
    }

    fprintf(stdout, "site builder init -->[%s]: OK\n", loc_conf->server_location_name);
    return NGX_CONF_OK;
}

ngx_int_t site_builder_global_init(ngx_cycle_t *cycle)
{

    site_builder_cache_init(cycle->pool);
    site_builder_flush_init(cycle->pool);
    return NGX_OK;
}





