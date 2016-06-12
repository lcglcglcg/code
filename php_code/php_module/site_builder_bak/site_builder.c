
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_site_builder.h"
#include <uuid/uuid.h>

/* If you declare any globals in php_site_builder.h uncomment this:
   ZEND_DECLARE_MODULE_GLOBALS(site_builder)
   */

/* True global resources - no need for thread safety here */
static int le_site_builder;

/* {{{ site_builder_functions[]
 *
 * Every user visible function must have an entry in site_builder_functions[].
 */
const zend_function_entry site_builder_functions[] = {
    PHP_ME(site_builder, __construct, NULL, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
	PHP_ME(site_builder, __destruct, NULL, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
	PHP_ME(site_builder, create_template_id, NULL, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
	PHP_ME(site_builder, create_keyword_id, NULL, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
	PHP_ME(site_builder, create_content_id, NULL, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
	PHP_ME(site_builder, create_link_group_id, NULL, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
	PHP_ME(site_builder, create_link_external_id, NULL, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
	PHP_ME(site_builder, add_template_dir, NULL, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
	PHP_ME(site_builder, add_keyword_file, NULL, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
	PHP_ME(site_builder, add_content_file, NULL, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
	PHP_ME(site_builder, add_link_group_file, NULL, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
	PHP_ME(site_builder, add_link_external_file, NULL, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
	PHP_ME(site_builder, get_config, NULL, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
	PHP_ME(site_builder, set_config, NULL, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
	PHP_FE_END	/* Must be the last line in site_builder_functions[] */
};
/* }}} */

/* {{{ site_builder_module_entry
*/
zend_module_entry site_builder_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
    STANDARD_MODULE_HEADER,
#endif
    "site_builder",
    site_builder_functions,
    PHP_MINIT(site_builder),
    PHP_MSHUTDOWN(site_builder),
    PHP_RINIT(site_builder),		/* Replace with NULL if there's nothing to do at request start */
    PHP_RSHUTDOWN(site_builder),	/* Replace with NULL if there's nothing to do at request end */
    PHP_MINFO(site_builder),
#if ZEND_MODULE_API_NO >= 20010901
    PHP_SITE_BUILDER_VERSION,
#endif
    STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_SITE_BUILDER
ZEND_GET_MODULE(site_builder)
#endif

    /* {{{ PHP_INI
    */
    /* Remove comments and fill if you need to have entries in php.ini
       PHP_INI_BEGIN()
       STD_PHP_INI_ENTRY("site_builder.global_value",      "42", PHP_INI_ALL, OnUpdateLong, global_value, zend_site_builder_globals, site_builder_globals)
       STD_PHP_INI_ENTRY("site_builder.global_string", "foobar", PHP_INI_ALL, OnUpdateString, global_string, zend_site_builder_globals, site_builder_globals)
       PHP_INI_END()
       */
    /* }}} */

    /* {{{ php_site_builder_init_globals
    */
    /* Uncomment this function if you have INI entries
       static void php_site_builder_init_globals(zend_site_builder_globals *site_builder_globals)
       {
       site_builder_globals->global_value = 0;
       site_builder_globals->global_string = NULL;
       }
       */
    /* }}} */

    /* {{{ PHP_MINIT_FUNCTION
    */
    zend_class_entry *site_builder_ce = NULL;
PHP_MINIT_FUNCTION(site_builder)
{
    zend_class_entry site_builder;
    INIT_CLASS_ENTRY(site_builder, "site_builder", site_builder_functions);
    site_builder_ce=zend_register_internal_class_ex(&site_builder, NULL, NULL TSRMLS_CC);
    zend_declare_property_null(site_builder_ce, ZEND_STRL("name"), ZEND_ACC_PUBLIC TSRMLS_CC);
    return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
*/
PHP_MSHUTDOWN_FUNCTION(site_builder)
{
    /* uncomment this line if you have INI entries
       UNREGISTER_INI_ENTRIES();
       */
    return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request start */
/* {{{ PHP_RINIT_FUNCTION
*/
PHP_RINIT_FUNCTION(site_builder)
{
    return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request end */
/* {{{ PHP_RSHUTDOWN_FUNCTION
*/
PHP_RSHUTDOWN_FUNCTION(site_builder)
{
    return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
*/
PHP_MINFO_FUNCTION(site_builder)
{
    php_info_print_table_start();
    php_info_print_table_header(2, "site_builder support", "enabled");
    php_info_print_table_end();

    /* Remove comments if you have entries in php.ini
       DISPLAY_INI_ENTRIES();
       */
}
/* }}} */

void site_builder_uuid(char *key)
{
    uuid_t uuid;
    uuid_generate(uuid);
    uuid_unparse(uuid, key);
}

PHP_METHOD(site_builder, __construct)
{

    chdir(SITE_BUILDER_HOME);
}

PHP_METHOD(site_builder, __destruct)
{

}

PHP_METHOD(site_builder, create_template_id)
{

    char dir_path[64];
    site_builder_uuid(dir_path);
    if (mkdir(dir_path, 0755))
	RETURN_STRING("create template_id: error!", 1);

    RETURN_STRING(dir_path, 1);
}

PHP_METHOD(site_builder, create_keyword_id)
{

    char file_path[64];
    site_builder_uuid(file_path);
    RETURN_STRING(file_path, 1);
}

PHP_METHOD(site_builder, create_content_id)
{

    char file_path[64];
    site_builder_uuid(file_path);
    RETURN_STRING(file_path, 1);
}

PHP_METHOD(site_builder, create_link_group_id)
{

    char file_path[64];
    site_builder_uuid(file_path);
    RETURN_STRING(file_path, 1);
}

PHP_METHOD(site_builder, create_link_external_id)
{

    char file_path[64];
    site_builder_uuid(file_path);
    RETURN_STRING(file_path, 1);
}

int site_builder_load_dir(const char *path, int path_len)
{

    DIR *dir = NULL;
    struct dirent *dir_info = NULL;
    char _path[2048];
    char *key = &_path[path_len];

    if (!(dir = opendir(path)))
	return -1;

    while((dir_info = readdir(dir))) {

	if((strcmp(dir_info->d_name,".") == 0))
	    continue;

	if ((strcmp(dir_info->d_name,"..")==0))
	    continue;

	sprintf(_path, "%s/%s", path, dir_info->d_name);
	if (dir_info->d_type == 4) { //dir

	    site_builder_load_dir(_path, path_len);

	} else if (dir_info->d_type == 8) { //file

	    if (strcmp(dir_info->d_name, "index.html") == 0) {

		fprintf(stdout, "[index.html]%s\n", _path);

	    } else if (strcmp(dir_info->d_name, "content.html") == 0) {

		fprintf(stdout, "[content.html]%s\n", _path);

	    } else {

		fprintf(stdout, "[affiliated_file]%s\n", _path);
	    }
	}
    }

    closedir(dir);
    return 0;
}

PHP_METHOD(site_builder, add_template_dir)
{

    char *template_id = NULL;
    int template_id_size = 0;

    char *dir_path = NULL;
    int dir_path_size = 0;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ss",
		&template_id,
		&template_id_size,
		&dir_path,
		&dir_path_size
		) == FAILURE)
	return;

    if (template_id_size != 36)
	RETURN_LONG(-2);

    site_builder_load_dir(dir_path, dir_path_size);
    RETURN_LONG(0);
}

PHP_METHOD(site_builder, add_keyword_file)
{

}

PHP_METHOD(site_builder, add_content_file)
{

}

PHP_METHOD(site_builder, add_link_group_file)
{

}

PHP_METHOD(site_builder, add_link_external_file)
{

}

PHP_METHOD(site_builder, get_config)
{

    RETURN_STRING("errcode -1:: network err!", 1);
}

PHP_METHOD(site_builder, set_config)
{

    char *str = NULL;
    int len = 0;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s",
		&str,
		&len
		) == FAILURE)
	return;

    fprintf(stdout, "[%s]\n", str);
}



