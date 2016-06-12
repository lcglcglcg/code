
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_site_builder.h"
#include <uuid/uuid.h>

static int le_site_builder;
const zend_function_entry site_builder_functions[] = {

    PHP_ME(site_builder, __construct, NULL, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
	PHP_ME(site_builder, __destruct, NULL, ZEND_ACC_PUBLIC | ZEND_ACC_DTOR)

	PHP_ME(site_builder, create_template_id, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(site_builder, create_keyword_id, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(site_builder, create_content_id, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(site_builder, create_link_external_id, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(site_builder, create_link_group_id, NULL, ZEND_ACC_PUBLIC)

	PHP_ME(site_builder, add_template_dir, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(site_builder, add_keyword_file, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(site_builder, add_content_file, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(site_builder, add_link_group_file, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(site_builder, add_link_external_file, NULL, ZEND_ACC_PUBLIC)

	PHP_ME(site_builder, get_config)
	PHP_ME(site_builder, set_config)

	PHP_FE_END
};

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

#ifdef COMPILE_DL_SITE_BUILDER
ZEND_GET_MODULE(site_builder)
#endif

    zend_class_entry *site_builder_ce = NULL;
PHP_MINIT_FUNCTION(site_builder)
{
    zend_class_entry site_builder;
    INIT_CLASS_ENTRY(site_builder, "site_builder", site_builder_functions);
    site_builder_ce=zend_register_internal_class_ex(&site_builder, NULL, NULL TSRMLS_CC);
    zend_declare_property_null(site_builder_ce, ZEND_STRL("name"), ZEND_ACC_PUBLIC TSRMLS_CC);
    return SUCCESS;
}

PHP_MSHUTDOWN_FUNCTION(site_builder)
{
    return SUCCESS;
}

PHP_RINIT_FUNCTION(site_builder)
{
    return SUCCESS;
}

PHP_RSHUTDOWN_FUNCTION(site_builder)
{
    return SUCCESS;
}

PHP_MINFO_FUNCTION(site_builder)
{
    php_info_print_table_start();
    php_info_print_table_header(2, "site_builder support", "enabled");
    php_info_print_table_end();
}

char *lg_string_uuid(char *key)
{
    uuid_t uuid;
    uuid_generate(uuid);
    uuid_unparse(uuid, key);
    return key;
}

PHP_METHOD(site_builder, __construct)
{

}

PHP_METHOD(site_builder, __destruct)
{

}

PHP_METHOD(site_builder, create_template_id)
{

    if (redis_conn == NULL || redis_conn->err) {

	RETURN_STRING("errcode -1:: network err!", 1);
	return;
    }

    redisReply *reply = redisCommand(redis_conn, "INCR db_count");
    int db = reply->integer;
    freeReplyObject(reply);

    RETURN_LONG(db);
}

PHP_METHOD(site_builder, create_keyword_id)
{

    if (redis_conn == NULL || redis_conn->err) {

	RETURN_STRING("errcode -1:: network err!", 1);
	return;
    }

    redisReply *reply = redisCommand(redis_conn, "INCR db_count");
    int db = reply->integer;
    freeReplyObject(reply);

    RETURN_LONG(db);
}

PHP_METHOD(site_builder, create_content_id)
{

    if (redis_conn == NULL || redis_conn->err) {

	RETURN_STRING("errcode -1:: network err!", 1);
	return;
    }

    redisReply *reply = redisCommand(redis_conn, "INCR db_count");
    int db = reply->integer;
    freeReplyObject(reply);

    RETURN_LONG(db);
}

PHP_METHOD(site_builder, create_link_external_id)
{

    if (redis_conn == NULL || redis_conn->err) {

	RETURN_STRING("errcode -1:: network err!", 1);
	return;
    }

    redisReply *reply = redisCommand(redis_conn, "INCR db_count");
    int db = reply->integer;
    freeReplyObject(reply);

    RETURN_LONG(db);
}

PHP_METHOD(site_builder, create_link_group_id)
{

    if (redis_conn == NULL || redis_conn->err) {

	RETURN_STRING("errcode -1:: network err!", 1);
	return;
    }

    redisReply *reply = redisCommand(redis_conn, "INCR db_count");
    int db = reply->integer;
    freeReplyObject(reply);

    RETURN_LONG(db);
}

PHP_METHOD(site_builder, create_affiliated_file_id)
{

    if (redis_conn == NULL || redis_conn->err) {

	RETURN_STRING("errcode -1:: network err!", 1);
	return;
    }

    redisReply *reply = redisCommand(redis_conn, "INCR db_count");
    int db = reply->integer;
    freeReplyObject(reply);

    RETURN_LONG(db);
}

PHP_METHOD(site_builder, add_template)
{

    char *base = NULL;
    int base_size = 0;

    char *key = NULL;
    int key_size = 0;

    char *buffer = NULL;
    int buffer_size = 0;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "sss",
		&base, &base_size,
		&key, &key_size,
		&buffer, &buffer_size
		) == FAILURE)
	return;

    if (redis_conn == NULL || redis_conn->err) {

	RETURN_STRING("errcode -1:: network err!", 1);
	return;
    }

    redisReply *reply = redisCommand(redis_conn, "select %s", base);
    freeReplyObject(reply);

    reply = redisCommand(redis_conn, "set %s %b", key, buffer, buffer_size);
    freeReplyObject(reply);
}

PHP_METHOD(site_builder, add_keyword)
{

    char *base = NULL;
    int base_size = 0;

    char *buffer = NULL;
    int buffer_size = 0;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ss", 
		&base, &base_size,
		&buffer, &buffer_size
		) == FAILURE)
	return;

    if (redis_conn == NULL || redis_conn->err) {

	RETURN_STRING("errcode -1:: network err!", 1);
	return;
    }

    redisReply *reply = redisCommand(redis_conn, "select %s", base);
    freeReplyObject(reply);

    char key[128];
    lg_string_uuid(key);

    reply = redisCommand(redis_conn, "set %s %b", key, buffer, buffer_size);
    freeReplyObject(reply);
}

PHP_METHOD(site_builder, add_content)
{

    char *base = NULL;
    int base_size = 0;

    char *buffer = NULL;
    int buffer_size = 0;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ss", 
		&base, &base_size,
		&buffer, &buffer_size
		) == FAILURE)
	return;

    if (redis_conn == NULL || redis_conn->err) {

	RETURN_STRING("errcode -1:: network err!", 1);
	return;
    }

    redisReply *reply = redisCommand(redis_conn, "select %s", base);
    freeReplyObject(reply);

    char key[128];
    lg_string_uuid(key);

    reply = redisCommand(redis_conn, "set %s %b", key, buffer, buffer_size);
    freeReplyObject(reply);
}

PHP_METHOD(site_builder, add_link_group)
{

    char *base = NULL;
    int base_size = 0;

    char *buffer = NULL;
    int buffer_size = 0;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ss", 
		&base, &base_size,
		&buffer, &buffer_size
		) == FAILURE)
	return;

    if (redis_conn == NULL || redis_conn->err) {

	RETURN_STRING("errcode -1:: network err!", 1);
	return;
    }

    redisReply *reply = redisCommand(redis_conn, "select %s", base);
    freeReplyObject(reply);

    char key[128];
    lg_string_uuid(key);

    reply = redisCommand(redis_conn, "set %s %b", key, buffer, buffer_size);
    freeReplyObject(reply);
}

PHP_METHOD(site_builder, add_link_external)
{

    char *base = NULL;
    int base_size = 0;

    char *key = NULL;
    int key_size = 0;

    char *buffer = NULL;
    int buffer_size = 0;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "sss",
		&base, &base_size,
		&key, &key_size,
		&buffer, &buffer_size
		) == FAILURE)
	return;

    if (redis_conn == NULL || redis_conn->err) {

	RETURN_STRING("errcode -1:: network err!", 1);
	return;
    }

    redisReply *reply = redisCommand(redis_conn, "select %s", base);
    freeReplyObject(reply);

    reply = redisCommand(redis_conn, "set %s %b", key, buffer, buffer_size);
    freeReplyObject(reply);
}

PHP_METHOD(site_builder, add_affiliated_file)
{

    char *base = NULL;
    int base_size = 0;

    char *key = NULL;
    int key_size = 0;

    char *buffer = NULL;
    int buffer_size = 0;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "sss",
		&base, &base_size,
		&key, &key_size,
		&buffer, &buffer_size
		) == FAILURE)
	return;

    if (redis_conn == NULL || redis_conn->err) {

	RETURN_STRING("errcode -1:: network err!", 1);
	return;
    }

    redisReply *reply = redisCommand(redis_conn, "select %s", base);
    freeReplyObject(reply);

    reply = redisCommand(redis_conn, "set %s %b", key, buffer, buffer_size);
    freeReplyObject(reply);
}


PHP_METHOD(site_builder, ngx_reload)
{

    int ret = system("/make/nginx-1.8.0/objs/nginx -s reload");
    fprintf(stdout, "ret=%d\n", ret);
}


