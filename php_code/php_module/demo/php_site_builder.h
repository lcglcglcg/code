
#ifndef PHP_SITE_BUILDER_H
#define PHP_SITE_BUILDER_H

extern zend_module_entry site_builder_module_entry;
#define phpext_site_builder_ptr &site_builder_module_entry

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

PHP_MINIT_FUNCTION(site_builder);
PHP_MSHUTDOWN_FUNCTION(site_builder);
PHP_RINIT_FUNCTION(site_builder);
PHP_RSHUTDOWN_FUNCTION(site_builder);
PHP_MINFO_FUNCTION(site_builder);

PHP_METHOD(site_builder, __construct);
PHP_METHOD(site_builder, __destruct);

PHP_METHOD(site_builder, create_template_id);
PHP_METHOD(site_builder, create_keyword_id);
PHP_METHOD(site_builder, create_content_id);
PHP_METHOD(site_builder, create_link_group_id);
PHP_METHOD(site_builder, create_link_external_id);

PHP_METHOD(site_builder, add_template_dir);
PHP_METHOD(site_builder, add_keyword_file);
PHP_METHOD(site_builder, add_content_file);
PHP_METHOD(site_builder, add_link_group_file);
PHP_METHOD(site_builder, add_link_external_file);

PHP_METHOD(site_builder, get_config);
PHP_METHOD(site_builder, set_config);

#ifdef ZTS
#define SITE_BUILDER_G(v) TSRMG(site_builder_globals_id, zend_site_builder_globals *, v)
#else
#define SITE_BUILDER_G(v) (site_builder_globals.v)
#endif

#endif	/* PHP_SITE_BUILDER_H */



