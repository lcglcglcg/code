
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_site_builder.h"

/* If you declare any globals in php_baidu_trans.h uncomment this:
   ZEND_DECLARE_MODULE_GLOBALS(baidu_trans)
   */

/* True global resources - no need for thread safety here */
static int le_baidu_trans;

/* {{{ baidu_trans_functions[]
 *
 * Every user visible function must have an entry in baidu_trans_functions[].
 */
const zend_function_entry baidu_trans_functions[] = {
    PHP_ME(baidu_trans, __construct, NULL, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
	PHP_ME(baidu_trans, __destruct, NULL, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
	PHP_ME(baidu_trans, get_config, NULL, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
	PHP_ME(baidu_trans, set_config, NULL, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
	{NULL, NULL, NULL}
    //	PHP_FE_END	/* Must be the last line in baidu_trans_functions[] */
};
/* }}} */

/* {{{ baidu_trans_module_entry
*/
zend_module_entry baidu_trans_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
    STANDARD_MODULE_HEADER,
#endif
    "baidu_trans",
    baidu_trans_functions,
    PHP_MINIT(baidu_trans),
    PHP_MSHUTDOWN(baidu_trans),
    PHP_RINIT(baidu_trans),		/* Replace with NULL if there's nothing to do at request start */
    PHP_RSHUTDOWN(baidu_trans),	/* Replace with NULL if there's nothing to do at request end */
    PHP_MINFO(baidu_trans),
#if ZEND_MODULE_API_NO >= 20010901
    PHP_SITE_BUILDER_VERSION,
#endif
    STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_SITE_BUILDER
ZEND_GET_MODULE(baidu_trans)
#endif

    /* {{{ PHP_INI
    */
    /* Remove comments and fill if you need to have entries in php.ini
       PHP_INI_BEGIN()
       STD_PHP_INI_ENTRY("baidu_trans.global_value",      "42", PHP_INI_ALL, OnUpdateLong, global_value, zend_baidu_trans_globals, baidu_trans_globals)
       STD_PHP_INI_ENTRY("baidu_trans.global_string", "foobar", PHP_INI_ALL, OnUpdateString, global_string, zend_baidu_trans_globals, baidu_trans_globals)
       PHP_INI_END()
       */
    /* }}} */

    /* {{{ php_baidu_trans_init_globals
    */
    /* Uncomment this function if you have INI entries
       static void php_baidu_trans_init_globals(zend_baidu_trans_globals *baidu_trans_globals)
       {
       baidu_trans_globals->global_value = 0;
       baidu_trans_globals->global_string = NULL;
       }
       */
    /* }}} */

    /* {{{ PHP_MINIT_FUNCTION
    */
    zend_class_entry *baidu_trans_ce = NULL;
PHP_MINIT_FUNCTION(baidu_trans)
{
    zend_class_entry baidu_trans;
    INIT_CLASS_ENTRY(baidu_trans, "baidu_trans", baidu_trans_functions);
    baidu_trans_ce=zend_register_internal_class_ex(&baidu_trans, NULL, NULL TSRMLS_CC);
    zend_declare_property_null(baidu_trans_ce, ZEND_STRL("name"), ZEND_ACC_PUBLIC TSRMLS_CC);
    return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
*/
PHP_MSHUTDOWN_FUNCTION(baidu_trans)
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
PHP_RINIT_FUNCTION(baidu_trans)
{
    return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request end */
/* {{{ PHP_RSHUTDOWN_FUNCTION
*/
PHP_RSHUTDOWN_FUNCTION(baidu_trans)
{
    return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
*/
PHP_MINFO_FUNCTION(baidu_trans)
{
    php_info_print_table_start();
    php_info_print_table_header(2, "baidu_trans support", "enabled");
    php_info_print_table_end();

    /* Remove comments if you have entries in php.ini
       DISPLAY_INI_ENTRIES();
       */
}
/* }}} */

PHP_METHOD(baidu_trans, __construct)
{

}

PHP_METHOD(baidu_trans, __destruct)
{

}

PHP_METHOD(baidu_trans, set_config)
{

    char *id = NULL;
    int id_size = 0;

    char *file_path = NULL;
    int file_path_size = 0;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ss",
		&id,
		&id_size,
		&file_path,
		&file_path_size
		) == FAILURE)
	return;


    fprintf(stdout, "%s:%s\n", id, file_path);
#if 0
    if (id_size != 36) 
	RETURN_LONG(SITE_BUILDER_NOT_ID);

    char dest_path[128] = SITE_BUILDER_HOME;
    sprintf(dest_path, "%s/%s", SITE_BUILDER_HOME, id);
    if (access(dest_path, 0)) 
	RETURN_LONG(SITE_BUILDER_NOT_ID);

    if (file_path_size <= 0) 
	RETURN_LONG(SITE_BUILDER_NOT_FILE);

    if (access(file_path, 0)) 
	RETURN_LONG(SITE_BUILDER_NOT_FILE);

    baidu_trans_copy(dest_path, file_path);
#endif
}

PHP_METHOD(baidu_trans, get_config)
{

}


