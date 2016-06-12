
#include <stdio.h>
#include <stdlib.h>

#include <bcon.h>
#include <bson.h>
#include <mongoc.h>

#include "lg_list.h"
#include "lg_string.h"

#include "ad_system_mongo.h"
#include "ad_system_report.h"
#include "ad_system_region_code.h"

#define	IF_ITER_FIND_TIME(doc, iter, name) if (!bson_iter_init_find(iter, doc, name) || !BSON_ITER_HOLDS_DATE_TIME(iter))
#define	IF_ITER_FIND_STRING(doc, iter, name) if (!bson_iter_init_find(iter, doc, name) || !BSON_ITER_HOLDS_UTF8(iter))

static char MONGODB_HOST[128] = {0};
int ad_system_mongo_init(lg_list_t *config_file_head)
{

    char *mongodb_host_str = NULL;
    if (!(mongodb_host_str = lg_config_file_find(config_file_head, "mongodb_host")))
	return -1;

    char *mongodb_port_str = NULL;
    if (!(mongodb_port_str = lg_config_file_find(config_file_head, "mongodb_port")))
	return -1;

    fprintf(stdout, "MONGODB_HOST=%s\n", mongodb_host_str);
    fprintf(stdout, "MONGODB_PORT=%s\n", mongodb_port_str);
    sprintf(MONGODB_HOST, "mongodb://%s:%s", mongodb_host_str, mongodb_port_str);

    mongoc_init ();
    return 0;
}

extern char *strptime(const char *s, const char *format, struct tm *tm);
int ad_system_mongo_conn(ad_system_mongo_t *p)
{
    memset(p, 0, sizeof(ad_system_mongo_t));
    if (!(p->client = mongoc_client_new (MONGODB_HOST)))
	return -1;

    return 0;
}

int ad_system_mongo_insert(ad_system_mongo_t *p, const char *table, bson_t *query)
{

    if (!(p->collection = mongoc_client_get_collection (p->client, MONGO_DB_NAME, table)))
	return -1;

    return mongoc_collection_insert (p->collection, MONGOC_INSERT_NONE, query, NULL, &p->error);
}

int ad_system_mongo_update(ad_system_mongo_t *p, const char *table, bson_t *query, bson_t *u)
{

    if (!(p->collection = mongoc_client_get_collection (p->client, MONGO_DB_NAME, table)))
	return -1;

    return mongoc_collection_update(p->collection, MONGOC_INSERT_NONE, query, u, NULL, &p->error);
}

int ad_system_mongo_delete(ad_system_mongo_t *p, const char *table, bson_t *query)
{

    if (!(p->collection = mongoc_client_get_collection (p->client, MONGO_DB_NAME, table)))
	return -1;

    mongoc_collection_delete(p->collection, MONGOC_INSERT_NONE, query, NULL, &p->error);
    return 0;
}

int ad_system_mongo_find(ad_system_mongo_t *p, const char *table, bson_t *query)
{

    if (!(p->collection = mongoc_client_get_collection (p->client, MONGO_DB_NAME, table)))
	return -1;

    if (!(p->cursor = mongoc_collection_find (
		    p->collection,
		    MONGOC_QUERY_NONE,
		    0,
		    0,
		    0,
		    query,
		    NULL,  /* Fields, NULL for all. */
		    NULL))) /* Read Prefs, NULL for default */
	return -1;

    if (mongoc_cursor_error (p->cursor, &p->error))
	return -1;

    return 0;
}

int ad_system_mongo_aggregate(ad_system_mongo_t *p, const char *table, bson_t *query)
{

    p->collection = mongoc_client_get_collection (p->client, MONGO_DB_NAME, table);
    if (!p->collection)
	return -1;

    p->cursor = mongoc_collection_aggregate (p->collection, MONGOC_QUERY_NONE, query, NULL, NULL);
    if (!p->cursor)
	return -1;

    if (mongoc_cursor_error (p->cursor, &p->error))
	return -1;

    return 0;
}

void ad_system_mongo_free(ad_system_mongo_t *p)
{

    if (p->cursor)
	mongoc_cursor_destroy (p->cursor);
    mongoc_collection_destroy (p->collection);
    mongoc_client_destroy (p->client);
}

void ad_system_mongo_list_free(lg_list_t *head)
{

    lg_list_t *p = head->next;
    lg_list_t *node = NULL;

    while(p != head) {

	node = p;
	p = p->next;
	free(node);
    }

    lg_list_init(head);
}

char *ad_system_mongo_time_to_string_week(time_t date_time, char *buffer)
{

    struct tm date_tm;
    gmtime_r(&date_time, &date_tm);

    time_t date_time_s = 0;
    if (date_tm.tm_wday == 0)
	date_time_s = date_time - 86400 * 6;
    else
	date_time_s = date_time - 86400 * (date_tm.tm_wday - 1);

    time_t date_time_e = date_time_s + 86400 * 6;

    struct tm date_tm_s;
    struct tm date_tm_e;
    gmtime_r(&date_time_s, &date_tm_s);
    gmtime_r(&date_time_e, &date_tm_e);

    sprintf(buffer, "%04d-%02d-%02d至%04d-%02d-%02d",
	    date_tm_s.tm_year + 1900,
	    date_tm_s.tm_mon + 1,
	    date_tm_s.tm_mday,
	    date_tm_e.tm_year + 1900,
	    date_tm_e.tm_mon + 1,
	    date_tm_e.tm_mday);

    return buffer;
}

char *ad_system_mongo_time_to_string(time_t date_time, char *buffer, int time_type)
{

    struct tm tm;
    gmtime_r(&date_time, &tm);

    switch(time_type) {

	case 1://日
	    sprintf(buffer, "%04d-%02d-%02d",
		    tm.tm_year + 1900,
		    tm.tm_mon + 1,
		    tm.tm_mday);
	    return buffer;

	case 2://周
	    return ad_system_mongo_time_to_string_week(date_time, buffer);

	case 3://月
	    sprintf(buffer, "%04d-%02d", tm.tm_year + 1900, tm.tm_mon + 1);
	    return buffer;

	case 4://年
	    sprintf(buffer, "%04d", tm.tm_year + 1900);
	    return buffer;

	default:
	    sprintf(buffer, "%04d-%02d-%02d %02d:%02d:%02d",
		    tm.tm_year + 1900,
		    tm.tm_mon + 1,
		    tm.tm_mday,
		    tm.tm_hour,
		    tm.tm_min,
		    tm.tm_sec);
	    return buffer;
    }

    return NULL;
}

time_t ad_system_mongo_string_to_time(const char *time_str)
{

    struct tm tm;
    memset(&tm, 0, sizeof(struct tm));
    strptime(time_str, "%Y-%m-%d", &tm);
    return mktime(&tm) + 28800;
}

int ad_system_mongo_to_number(bson_iter_t *iter)
{

    int n = 0;
    int type = (bson_iter_type(iter));
    switch(type){

	case BSON_TYPE_DOUBLE:
	    n = bson_iter_double(iter);
	    break;

	case BSON_TYPE_INT32:
	    n = bson_iter_int32(iter);
	    break;

	case BSON_TYPE_INT64:
	    n = bson_iter_int64(iter);
	    break;

	case BSON_TYPE_UTF8:
	    n = atoi(bson_iter_utf8(iter, NULL));
	    break;
    }   

    return n;
}

double ad_system_mongo_to_double(bson_iter_t *iter)
{

    double n = 0;
    int type = (bson_iter_type(iter));
    switch(type){

	case BSON_TYPE_DOUBLE:
	    n = bson_iter_double(iter);
	    break;

	case BSON_TYPE_INT32:
	    n = bson_iter_int32(iter);
	    break;

	case BSON_TYPE_INT64:
	    n = bson_iter_int64(iter);
	    break;

	case BSON_TYPE_UTF8:
	    n = atof(bson_iter_utf8(iter, NULL));
	    break;
    }   

    return n;
}

//账户管理->去投放中心
int ad_system_mongo_delivery_center_add(char *key, char *account_id)
{

    ad_system_mongo_t conn;
    if (ad_system_mongo_conn(&conn))
	return -1;

    bson_t b;
    bson_init (&b);
    bson_append_utf8(&b, "_id", -1, key, -1);
    bson_append_utf8(&b, "account_id", -1, account_id, -1);
    ad_system_mongo_insert(&conn, "delivery_center_list", &b);

    bson_destroy (&b);
    ad_system_mongo_free(&conn);
    return 0;
}

//财务->媒体报告载入
int ad_system_mongo_report_media_element(lg_list_t *head, const bson_t *doc)
{

    bson_iter_t iter_time;
    if (!bson_iter_init_find(&iter_time, doc, "time"))
	return -1;

    bson_iter_t iter_account_id;
    if (!bson_iter_init_find(&iter_account_id, doc, "account_id"))
	return -1;

    bson_iter_t iter_visit_domain;
    if (!bson_iter_init_find(&iter_visit_domain, doc, "visit_domain"))
	return -1;

    bson_iter_t iter_impression;
    if (!bson_iter_init_find(&iter_impression, doc, "impression"))
	return -1;

    bson_iter_t iter_click;
    if (!bson_iter_init_find(&iter_click, doc, "click"))
	return -1;

    bson_iter_t iter_cost;
    if (!bson_iter_init_find(&iter_cost, doc, "cost"))
	return -1;

    char time_buffer[128];
    return ad_system_report_media_add(head,
	    ad_system_mongo_time_to_string(bson_iter_time_t(&iter_time), time_buffer, 1),
	    bson_iter_utf8(&iter_visit_domain, NULL),
	    ad_system_mongo_to_number(&iter_account_id),
	    ad_system_mongo_to_number(&iter_impression),
	    ad_system_mongo_to_number(&iter_click),
	    ad_system_mongo_to_double(&iter_cost));
}

int ad_system_mongo_report_media_load(lg_list_t *head,
	const char *visit_domain,
	const char *time_s,
	const char *time_e)
{

    lg_list_init(head);
    ad_system_mongo_t conn;
    if (ad_system_mongo_conn(&conn))
	return -1;

    bson_t query_time;
    bson_init (&query_time);
    bson_append_time_t(&query_time, "$gte", -1, ad_system_mongo_string_to_time(time_s));
    bson_append_time_t(&query_time, "$lte", -1, ad_system_mongo_string_to_time(time_e));

    bson_t query;
    bson_init (&query);
    if (visit_domain)
	bson_append_utf8(&query, "visit_domain", -1, visit_domain, -1);
    bson_append_document(&query, "time", -1, &query_time);

    if (ad_system_mongo_find(&conn, "report_media", &query)) {

	bson_destroy (&query);
	ad_system_mongo_free(&conn);
	return -1;
    }

    while (!mongoc_cursor_error (conn.cursor, &conn.error) &&
	    mongoc_cursor_more (conn.cursor)) {

	if (mongoc_cursor_next(conn.cursor, &conn.doc)) {

	    ad_system_mongo_report_media_element(head, conn.doc);
	}
    }

    bson_destroy (&query);
    ad_system_mongo_free(&conn);
    return 0;
}

//财务->用户报告
int ad_system_mongo_report_account_element(lg_list_t *head, const bson_t *doc, int mode)
{

    bson_iter_t iter_time;
    if (!bson_iter_init_find(&iter_time, doc, "time"))
	return -1;

    bson_iter_t iter_account_id;
    if (!bson_iter_init_find(&iter_account_id, doc, "account_id"))
	return -1;

    bson_iter_t iter_impression;
    if (!bson_iter_init_find(&iter_impression, doc, "impression"))
	return -1;

    bson_iter_t iter_click;
    if (!bson_iter_init_find(&iter_click, doc, "click"))
	return -1;

    bson_iter_t iter_cost;
    if (!bson_iter_init_find(&iter_cost, doc, "cost"))
	return -1;

    char time_buffer[128] = {0};
    return ad_system_report_account_add(head,
	    ad_system_mongo_time_to_string(bson_iter_time_t(&iter_time), time_buffer, 1),
	    ad_system_mongo_to_number(&iter_account_id),
	    ad_system_mongo_to_number(&iter_impression),
	    ad_system_mongo_to_number(&iter_click),
	    ad_system_mongo_to_double(&iter_cost),
	    mode);
    return 0;
}

int ad_system_mongo_report_account_load(lg_list_t *head,
	const char *visit_domain,
	const char *time_s,
	const char *time_e,
	int mode)
{

    lg_list_init(head);
    ad_system_mongo_t conn;
    if (ad_system_mongo_conn(&conn))
	return -1;

    bson_t query_time;
    bson_init (&query_time);
    bson_append_time_t(&query_time, "$gte", -1, ad_system_mongo_string_to_time(time_s));
    bson_append_time_t(&query_time, "$lte", -1, ad_system_mongo_string_to_time(time_e));

    bson_t query;
    bson_init (&query);
    bson_append_document(&query, "time", -1, &query_time);

    if (visit_domain) {
	bson_append_utf8(&query, "visit_domain", -1, visit_domain, -1);

	if (ad_system_mongo_find(&conn, "report_media", &query)) {

	    bson_destroy (&query);
	    ad_system_mongo_free(&conn);
	    return -1;
	}

    } else {

	if (ad_system_mongo_find(&conn, "report_standard", &query)) {

	    bson_destroy (&query);
	    ad_system_mongo_free(&conn);
	    return -1;
	}
    }

    while (!mongoc_cursor_error (conn.cursor, &conn.error) &&
	    mongoc_cursor_more (conn.cursor)) {

	if (mongoc_cursor_next(conn.cursor, &conn.doc)) {

	    ad_system_mongo_report_account_element(head, conn.doc, mode);
	}
    }

    bson_destroy (&query);
    ad_system_mongo_free(&conn);
    return 0;
}

//财务->获取媒体列表
int ad_system_mongo_get_media_domain_list_element(lg_list_t *head, const bson_t *doc)
{

    bson_iter_t iter_oid;
    if (!bson_iter_init_find(&iter_oid, doc, "_id"))
	return -1;

    const bson_oid_t *oid = NULL;
    if (!(oid = bson_iter_oid(&iter_oid)))
	return -1;

    char domain_id[128];
    bson_oid_to_string(oid, domain_id);

    bson_iter_t iter_domain;
    if (!bson_iter_init_find(&iter_domain, doc, "domain"))
	return -1;

    if (strcmp(bson_iter_utf8(&iter_domain, NULL), "DSP") == 0)
	return 0;

    ad_system_mongo_domain_list_t *node = NULL;
    if (!(node = malloc(sizeof(ad_system_mongo_domain_list_t))))
	return -1;

    strcpy(node->domain_id, domain_id);
    strcpy(node->domain_str, bson_iter_utf8(&iter_domain, NULL));

    lg_list_add(head, &node->next);
    return 0;
}

int ad_system_mongo_get_media_domain_list(lg_list_t *head)
{

    lg_list_init(head);
    ad_system_mongo_t conn;
    if (ad_system_mongo_conn(&conn))
	return -1;

    bson_t query;
    bson_init (&query);
    if (ad_system_mongo_find(&conn, "domain_list", &query)) {

	ad_system_mongo_free(&conn);
	return -1;
    }

    while (!mongoc_cursor_error (conn.cursor, &conn.error) &&
	    mongoc_cursor_more (conn.cursor)) {

	if (mongoc_cursor_next(conn.cursor, &conn.doc)) {

	    ad_system_mongo_get_media_domain_list_element(head, conn.doc);
	}
    }

    ad_system_mongo_free(&conn);
    return 0;
}

int ad_system_mongo_get_media_domain_unique(const char *domain_id, char *domain_str)
{

    ad_system_mongo_t conn;
    if (ad_system_mongo_conn(&conn))
	return -1;

    bson_t query;
    bson_init (&query);
    //bson_append_utf8(&query, "_id", -1, domain_id, -1);

    bson_oid_t oid;
    bson_oid_init_from_string (&oid, domain_id);
    bson_append_oid (&query, "_id", -1, &oid);


    if (ad_system_mongo_find(&conn, "domain_list", &query)) {

	ad_system_mongo_free(&conn);
	return -1;
    }

    if (!mongoc_cursor_error (conn.cursor, &conn.error) &&
	    mongoc_cursor_more (conn.cursor)) {

	if (mongoc_cursor_next(conn.cursor, &conn.doc)) {

	    bson_iter_t iter_domain;
	    if (!bson_iter_init_find(&iter_domain, conn.doc, "domain")) {

		bson_destroy (&query);
		ad_system_mongo_free(&conn);
		return -1;
	    }

	    strcpy(domain_str, bson_iter_utf8(&iter_domain, NULL));
	    bson_destroy (&query);
	    ad_system_mongo_free(&conn);
	    return 0;
	}
    }

    bson_destroy (&query);
    ad_system_mongo_free(&conn);
    return -1;
}

double ad_system_mongo_get_account_cost_total(ad_system_mongo_t *conn, const char *date_str, int account_id)
{

    bson_t date;
    bson_init (&date);
    bson_append_time_t(&date, "$lt", -1, ad_system_mongo_string_to_time(date_str));

    bson_t match;
    bson_init (&match);
    bson_append_int32(&match, "account_id", -1, account_id);
    bson_append_document(&match, "date", -1, &date);
    //fprintf(stdout, "%s\n", bson_as_json (&match, NULL));

    bson_t cost;
    bson_init (&cost);
    bson_append_utf8(&cost, "$sum", -1, "$cost", -1);
    //fprintf(stdout, "%s\n", bson_as_json (&cost, NULL));

    bson_t group;
    bson_init (&group);
    bson_append_utf8(&group, "_id", -1, "$account_id", -1);
    bson_append_document(&group, "cost", -1, &cost);
    //fprintf(stdout, "%s\n", bson_as_json (&group, NULL));

    bson_t array_group;
    bson_init(&array_group);
    bson_append_document(&array_group, "$group", -1, &group);
    //fprintf(stdout, "%s\n", bson_as_json (&array_group, NULL));

    bson_t array_match;
    bson_init(&array_match);
    bson_append_document(&array_match, "$match", -1, &match);
    //fprintf(stdout, "%s\n", bson_as_json (&array_match, NULL));

    bson_t array;
    bson_init(&array);
    bson_append_document(&array, "0", -1, &array_match);
    bson_append_document(&array, "1", -1, &array_group);
    //fprintf(stdout, "%s\n", bson_as_json (&array, NULL));

    bson_t pipe;
    bson_init (&pipe);
    bson_append_array(&pipe, "pipeline", -1, &array);
    //fprintf(stderr, "%s\n", bson_as_json (&pipe, NULL));


    //ad_system_mongo_t conn;
    //if (ad_system_mongo_conn(&conn))
    //return 0;

    if (ad_system_mongo_aggregate(conn, "report_standard", &pipe)) {

	bson_destroy (&pipe);
	return 0;
    }

    const bson_t *doc = NULL;
    if (!mongoc_cursor_next (conn->cursor, &doc)) {

	bson_destroy (&pipe);
	return 0;
    }

    bson_iter_t iter_cost;
    if (!bson_iter_init_find(&iter_cost, doc, "cost")) {

	bson_destroy (&pipe);
	return 0;
    }

    if (mongoc_cursor_error (conn->cursor, &conn->error)) {

	bson_destroy (&pipe);
	return 0;
    }

    double cost_total = ad_system_mongo_to_double(&iter_cost);

    bson_destroy (&pipe);
    return cost_total;
}


