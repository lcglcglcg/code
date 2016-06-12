
#include "site_builder_module.h"

char *site_builder_init(ngx_conf_t *cf, site_builder_loc_conf_t *loc_conf)
{

    if (loc_conf->template_dir.len == 0
	    || loc_conf->keyword_file.len == 0
	    || loc_conf->content_file.len == 0
	    || loc_conf->link_external_file.len == 0
	    || loc_conf->link_group_file.len == 0
       ) {

	fprintf(stderr, "nginx.conf Configuration site_builder_module: lack!\n");
	return NGX_CONF_ERROR;
    }

    /********************/
    /* init config data */
    /********************/

    /* init element list */
    loc_conf->affiliated_file = ngx_pcalloc(cf->pool, sizeof(site_builder_element_head_t));
    loc_conf->keyword = ngx_pcalloc(cf->pool, sizeof(site_builder_element_head_t));
    loc_conf->content = ngx_pcalloc(cf->pool, sizeof(site_builder_element_head_t));
    loc_conf->link_external = ngx_pcalloc(cf->pool, sizeof(site_builder_element_head_t));
    loc_conf->link_group = ngx_pcalloc(cf->pool, sizeof(site_builder_element_head_t));

    /********************/
    /* load config data */
    /********************/

    fprintf(stdout, "site builder init: Loading ...\n");

    // load template
    if (site_builder_template_dir_load(
		(char *)loc_conf->template_dir.data,
		loc_conf->template_dir.len,
		&loc_conf->template_list,
		loc_conf->affiliated_file,
		cf->pool)) {

	fprintf(stderr, "not template file[%s]: error!\n", (char *)loc_conf->template_dir.data);
	return NGX_CONF_ERROR;
    }

    if (loc_conf->template_list.template_total <= 0) {

	fprintf(stderr, "not template file[%s]: error!\n", (char *)loc_conf->template_dir.data);
	return NGX_CONF_ERROR;
    }

    // load keyword
    if (site_builder_element_load_file(
		(char *)loc_conf->keyword_file.data,
		loc_conf->keyword,
		cf->pool)) {

	fprintf(stderr, "load keyword [%s]: error!\n", (char *)loc_conf->keyword_file.data);
	return NGX_CONF_ERROR;
    }

    // load content
    if (site_builder_element_load_file(
		(char *)loc_conf->content_file.data,
		loc_conf->content,
		cf->pool)) {

	fprintf(stderr, "load content [%s]: error!\n", (char *)loc_conf->content_file.data);
	return NGX_CONF_ERROR;
    }

    // load link_external
    if (site_builder_element_load_link_external_file(
		(char *)loc_conf->link_external_file.data,
		loc_conf->link_external,
		cf->pool)) {

	fprintf(stderr, "load link_external [%s]: error!\n", (char *)loc_conf->link_external_file.data);
	return NGX_CONF_ERROR;
    }

    // load link_group
    if (site_builder_element_load_file(
		(char *)loc_conf->link_group_file.data,
		loc_conf->link_group,
		cf->pool)) {

	fprintf(stderr, "load link_group_file [%s]: error!\n", (char *)loc_conf->link_group_file.data);
	return NGX_CONF_ERROR;
    }

    fprintf(stdout, "site builder init: OK\n");
    return NGX_CONF_OK;
}

void site_builder_init_global_value(ngx_cycle_t *cycle)
{

}





