
#ifndef PHP_SITE_BUILDER_H
#define PHP_SITE_BUILDER_H

extern zend_module_entry baidu_trans_module_entry;
#define phpext_baidu_trans_ptr &baidu_trans_module_entry

#define PHP_SITE_BUILDER_VERSION "0.1.0" /* Replace with version number for your extension */

#ifdef PHP_WIN32
#	define PHP_SITE_BUILDER_API __declspec(dllexport)
#elif defined(__GNUC__) && __GNUC__ >= 4
#	define PHP_SITE_BUILDER_API __attribute__ ((visibility("default")))
#else
#	define PHP_SITE_BUILDER_API
#endif

#ifdef ZTS
#include "TSRM.h"
#endif

PHP_MINIT_FUNCTION(baidu_trans);
PHP_MSHUTDOWN_FUNCTION(baidu_trans);
PHP_RINIT_FUNCTION(baidu_trans);
PHP_RSHUTDOWN_FUNCTION(baidu_trans);
PHP_MINFO_FUNCTION(baidu_trans);

PHP_METHOD(baidu_trans, __construct);
PHP_METHOD(baidu_trans, __destruct);

PHP_METHOD(baidu_trans, set_config);
PHP_METHOD(baidu_trans, get_config);

/* 
   Declare any global variables you may need between the BEGIN
   and END macros here:     

   ZEND_BEGIN_MODULE_GLOBALS(baidu_trans)
   long  global_value;
   char *global_string;
   ZEND_END_MODULE_GLOBALS(baidu_trans)
   */

/* In every utility function you add that needs to use variables 
   in php_baidu_trans_globals, call TSRMLS_FETCH(); after declaring other 
   variables used by that function, or better yet, pass in TSRMLS_CC
   after the last function argument and declare your utility function
   with TSRMLS_DC after the last declared argument.  Always refer to
   the globals in your function as SITE_BUILDER_G(variable).  You are 
   encouraged to rename these macros something shorter, see
   examples in any other php module directory.
   */

#ifdef ZTS
#define SITE_BUILDER_G(v) TSRMG(baidu_trans_globals_id, zend_baidu_trans_globals *, v)
#else
#define SITE_BUILDER_G(v) (baidu_trans_globals.v)
#endif

#endif	/* PHP_SITE_BUILDER_H */

