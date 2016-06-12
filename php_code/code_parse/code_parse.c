/*
   +----------------------------------------------------------------------+
   | PHP Version 5                                                        |
   +----------------------------------------------------------------------+
   | Copyright (c) 1997-2015 The PHP Group                                |
   +----------------------------------------------------------------------+
   | This source file is subject to version 3.01 of the PHP license,      |
   | that is bundled with this package in the file LICENSE, and is        |
   | available through the world-wide-web at the following url:           |
   | http://www.php.net/license/3_01.txt                                  |
   | If you did not receive a copy of the PHP license and are unable to   |
   | obtain it through the world-wide-web, please send a note to          |
   | license@php.net so we can mail you a copy immediately.               |
   +----------------------------------------------------------------------+
   | Author:                                                              |
   +----------------------------------------------------------------------+
   */

/* $Id$ */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_code_parse.h"

#include "bmp_code.h"
#include "font_code.h"
#include "parse_code.h"
#include "gd.h"

#define php_gd_gdImageCreateFromJpeg gdImageCreateFromJpeg
#define php_gd_gdImageCreateFromJpegPtr	gdImageCreateFromJpegPtr

/* If you declare any globals in php_code_parse.h uncomment this:
   ZEND_DECLARE_MODULE_GLOBALS(code_parse)
   */

/* True global resources - no need for thread safety here */
static int le_code_parse;

/* {{{ code_parse_functions[]
 *
 * Every user visible function must have an entry in code_parse_functions[].
 */

const zend_function_entry code_parse_functions[] = {
    PHP_ME(code_parse, __construct, NULL, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
	PHP_ME(code_parse, __destruct, NULL, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
	PHP_ME(code_parse, bmp_path, NULL, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
	PHP_ME(code_parse, bmp_buffer, NULL, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
	PHP_ME(code_parse, jpeg_path, NULL, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
	PHP_ME(code_parse, jpeg_buffer, NULL, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
	{NULL, NULL, NULL}
};
/* }}} */

/* {{{ code_parse_module_entry
*/
zend_module_entry code_parse_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
    STANDARD_MODULE_HEADER,
#endif
    "code_parse",
    code_parse_functions,
    PHP_MINIT(code_parse),
    PHP_MSHUTDOWN(code_parse),
    PHP_RINIT(code_parse),		/* Replace with NULL if there's nothing to do at request start */
    PHP_RSHUTDOWN(code_parse),	/* Replace with NULL if there's nothing to do at request end */
    PHP_MINFO(code_parse),
#if ZEND_MODULE_API_NO >= 20010901
    PHP_CODE_PARSE_VERSION,
#endif
    STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_CODE_PARSE
ZEND_GET_MODULE(code_parse)
#endif

    /* {{{ PHP_INI
    */
    /* Remove comments and fill if you need to have entries in php.ini
       PHP_INI_BEGIN()
       STD_PHP_INI_ENTRY("code_parse.global_value",      "42", PHP_INI_ALL, OnUpdateLong, global_value, zend_code_parse_globals, code_parse_globals)
       STD_PHP_INI_ENTRY("code_parse.global_string", "foobar", PHP_INI_ALL, OnUpdateString, global_string, zend_code_parse_globals, code_parse_globals)
       PHP_INI_END()
       */
    /* }}} */

    /* {{{ php_code_parse_init_globals
    */
    /* Uncomment this function if you have INI entries
       static void php_code_parse_init_globals(zend_code_parse_globals *code_parse_globals)
       {
       code_parse_globals->global_value = 0;
       code_parse_globals->global_string = NULL;
       }
       */
    /* }}} */

    /* {{{ PHP_MINIT_FUNCTION
    */

    zend_class_entry *code_parse_ce = NULL;
PHP_MINIT_FUNCTION(code_parse)
{
    zend_class_entry code_parse;
    INIT_CLASS_ENTRY(code_parse, "code_parse", code_parse_functions);
    code_parse_ce=zend_register_internal_class_ex(&code_parse, NULL, NULL TSRMLS_CC);
    zend_declare_property_null(code_parse_ce, ZEND_STRL("name"), ZEND_ACC_PUBLIC TSRMLS_CC);
    return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
*/
PHP_MSHUTDOWN_FUNCTION(code_parse)
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
PHP_RINIT_FUNCTION(code_parse)
{
    return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request end */
/* {{{ PHP_RSHUTDOWN_FUNCTION
*/
PHP_RSHUTDOWN_FUNCTION(code_parse)
{
    return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
*/
PHP_MINFO_FUNCTION(code_parse)
{
    php_info_print_table_start();
    php_info_print_table_header(2, "code_parse support", "enabled");
    php_info_print_table_end();

    /* Remove comments if you have entries in php.ini
       DISPLAY_INI_ENTRIES();
       */
}
/* }}} */

PHP_METHOD(code_parse, __construct)
{

}

PHP_METHOD(code_parse, __destruct)
{

}

void code_parse_run(bmp_code_t *bmp)
{

    // font
    lg_list_t font_head;
    font_code_init(&font_head);
    // font_code_print(&font_head);

    // parse
    parse_code_t parse[5];
    parse_code_run(&font_head, bmp, &parse[0]);
    parse_code_run(&font_head, bmp, &parse[1]);
    parse_code_run(&font_head, bmp, &parse[2]);
    parse_code_run(&font_head, bmp, &parse[3]);
    parse_code_sort(parse);

    // display
    int i;
    for (i = 0; i < 4; i++) {

	bmp->code_str[i] = parse[i].ch;
	// fprintf(stdout, "[%c][%0.2f]\n", parse[i].ch, parse[i].pixel_pct);
    }

    bmp->code_str[4] = 0;
}

PHP_METHOD(code_parse, bmp_path)
{

    char *str = NULL;
    int size = 0;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s",
		&str,
		&size
		) == FAILURE) {

	RETVAL_STRING("errCode:1", 1); // 参数错误
	return;
    }

    bmp_code_t bmp;
    bmp.grey_value = 230;
    bmp_code_open(&bmp, str);

    code_parse_run(&bmp);
    RETVAL_STRING(bmp.code_str, 1);
}

PHP_METHOD(code_parse, bmp_buffer)
{

    char *str = NULL;
    int size = 0;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z",
		&str,
		&size
		) == FAILURE) {

	RETVAL_STRING("errCode:1", 1); // 参数错误
	return;
    }

    bmp_code_t bmp;
    bmp.grey_value = 230;
    bmp_code_load(&bmp, str);

    code_parse_run(&bmp);
    RETVAL_STRING(bmp.code_str, 1);
}

PHP_METHOD(code_parse, jpeg_path)
{

    char *str = NULL;
    int size = 0;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s",
		&str,
		&size
		) == FAILURE) {

	RETVAL_STRING("errCode:1", 1); // 参数错误
	return;
    }

    // load img
    FILE *fp = fopen(str, "rb");
    if (!fp) {

	RETVAL_STRING("errCode:2", 1); // 路径不存在
	return;
    }

    gdImagePtr im = (gdImagePtr)gdImageCreateFromJpeg(fp);
    if (!im) {

	RETVAL_STRING("errCode:3", 1); //图片格式错误
	return;
    }

    char *buffer = gdImageBmpPtr(im, &size, 0);
    if (!buffer && !size) {

	RETVAL_STRING("errCode:3", 1); //图片格式错误
	return;
    }

    // bmp
    bmp_code_t bmp;
    bmp.grey_value = 230;
    bmp_code_load(&bmp, buffer);

    code_parse_run(&bmp);
    RETVAL_STRING(bmp.code_str, 1);

    fclose(fp);
}

PHP_METHOD(code_parse, jpeg_buffer)
{

    char *str = NULL;
    int size = 0;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s",
		&str,
		&size
		) == FAILURE) {

	RETVAL_STRING("errCode:1", 1); // 参数错误
	return;
    }

    gdImagePtr im = (gdImagePtr)gdImageCreateFromJpegPtr(size, str);
    if (!im) {

	RETVAL_STRING("errCode:3", 1); //图片格式错误
	return;
    }

    char *buffer = gdImageBmpPtr(im, &size, 0);
    if (!buffer && !size) {

	RETVAL_STRING("errCode:3", 1); //图片格式错误
	return;
    }

    // bmp
    bmp_code_t bmp;
    bmp.grey_value = 230;
    bmp_code_load(&bmp, buffer);

    code_parse_run(&bmp);
    RETVAL_STRING(bmp.code_str, 1);
}


