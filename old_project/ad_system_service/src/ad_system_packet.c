
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lg_string.h"
#include "lg_string_json_.h"
#include "lg_config_file.h"
#include "img_libgd_captcha.h"
#include "lg_log_file.h"

#include "ad_system_oci.h"
#include "ad_system_sql.h"
#include "ad_system_mongo.h"
#include "ad_system_merger.h"
#include "ad_system_report.h"
#include "ad_system_network.h"
#include "ad_system_session.h"
#include "ad_system_packet.h"
#include "ad_system_global_var.h"
#include "ad_system_report.h"
#include "ad_system_upload_image.h"
#include "ad_system_disease_code.h"

#define	SEND_JSON_DATA_NODE 	0
#define	SEND_JSON_DATA_ONLY	1
#define	TEST_ACCOUNT_ID "191"
#define	GET_JSON_VALUE_STR(o,s) json_object*o=json_object_object_get(json_root, s);if(o)json->o=json_object_get_string(o)
#define	GET_JSON_VALUE_STR2(buf,o,s) json_object*o=json_object_object_get(json_root, s);if(o)json->o=CREATIVE_NAME(buf,o)
#define	GET_JSON_VALUE_NUM(o,s) json_object*o=json_object_object_get(json_root, s);if(o)json->o=ad_system_packet_json_value_to_number(o)

static void ad_system_packet_send_http_head(int sockfd)
{
    write(sockfd, HTTP_HEAD, strlen(HTTP_HEAD));
}

void ad_system_packet_session_send_code(int sockfd, ad_system_session_t *session, int code)
{

    char buffer[4096] = {0};
    size_t size = sprintf(buffer, 
	    "HTTP/1.1 200 OK\r\nContent-Type: text/html;charset=utf-8\r\nSet-Cookie: key=%s; Path=/action\r\n\r\n"
	    "{\"CODE\":\"%u\"}", 
	    session->session_id,
	    code);

    write(sockfd, buffer, size);
    close(sockfd);

    lg_log("[send][%s]%s\n", session ? session->account_id : "null", buffer);
}

void ad_system_packet_send_code(int sockfd, ad_system_session_t *session, ad_system_sql_t *sql_conn, int code)
{

    char buffer[1024];
    size_t size = sprintf(buffer, "%s{\"CODE\":\"%u\"}", HTTP_HEAD, code);
    write(sockfd, buffer, size);
    close(sockfd);

    if (sql_conn) ad_system_sql_free(sql_conn);
    //lg_log("[send][%s]%s\n", session ? session->account_id : "null", buffer);
}

void ad_system_packet_send_buffer(int sockfd, ad_system_session_t *session, char *buffer, size_t size)
{
    write(sockfd, HTTP_HEAD, strlen(HTTP_HEAD));
    write(sockfd, buffer, size);
    close(sockfd);

    //lg_log("[send][%s]%s\n", session ? session->account_id : "null", buffer);
}

void ad_system_packet_send_image(int sockfd, ad_system_session_t *session, char *buffer)
{

    get_img_libgd_captcha(&session->img);
    size_t size = sprintf(buffer, HTTP_IMAGE, session->img.size, session->session_id);

    write(sockfd, buffer, size);
    write(sockfd, session->img.buffer, session->img.size);
    close(sockfd);

    free(session->img.buffer);
    session->img.buffer = NULL;
}

void ad_system_packet_send_json(int sockfd, char *buffer, int only_status, char *head_str, char *end_str, char *format, ...)
{

    size_t size = 0;
    char comma[4] = {0};

    va_list arg_list;
    va_start(arg_list, format);

    if (only_status)
	size = sprintf(&buffer[size], HTTP_HEAD);

    if (head_str)
	size += sprintf(&buffer[size], head_str);

    for( ; *format; format++) {

	if (*format == '%') {

	    format++;
	    if (*format == 'd' || *format == 'u') {

		char *name = va_arg(arg_list, char *);
		int value = va_arg(arg_list, int);
		size += sprintf(&buffer[size], "%s\"%s\":\"%u\"", comma, name, value);
		comma[0] = ',';

	    } else if (*format == 'f') {

		char *name = va_arg(arg_list, char *);
		double value = va_arg(arg_list, double);
		size += sprintf(&buffer[size], "%s\"%s\":\"%0.2f\"", comma, name, value);
		comma[0] = ',';

	    } else if (*format == 'e') {

		char *name = va_arg(arg_list, char *);
		double value = va_arg(arg_list, double);
		size += sprintf(&buffer[size], "%s\"%s\":\"%0.2f%%\"", comma, name, value * 100);
		comma[0] = ',';

	    } else if (*format == 's') {

		char *name = va_arg(arg_list, char *);
		char *value = va_arg(arg_list, char *);

		if (value && *value) {

		    struct printbuf *json_value = NULL;
		    if ((json_value = printbuf_new())) {

			lg_string_to_json(json_value, value);
			size += sprintf(&buffer[size], "%s\"%s\":\"%s\"", comma, name, json_value->buf);
			printbuf_free(json_value);

		    }

		} else {

		    size += sprintf(&buffer[size], "%s\"%s\":\"\"", comma, name);
		}

		comma[0] = ',';
	    }
	}
    }

    if (end_str)
	size += sprintf(&buffer[size], end_str);

    va_end(arg_list);
    write(sockfd, buffer, size);
    if (only_status)close(sockfd);
}

void ad_system_packet_send_json_r(int sockfd, char *buffer, int only_status, char *head_str, char *end_str, int count, ...)
{

    size_t i, size = 0;
    char comma[4] = {0};

    va_list arg_list;
    va_start(arg_list, count);

    if (only_status)
	size = sprintf(&buffer[size], HTTP_HEAD);

    if (head_str)
	size += sprintf(&buffer[size], head_str);

    for(i = 0; i < count; i++) {

	char *format = va_arg(arg_list, char *);
	char *name = va_arg(arg_list, char *);

	if (strcmp(format, "%d") == 0 || strcmp(format, "%u") == 0) {

	    int value = va_arg(arg_list, int);
	    size += sprintf(&buffer[size], "%s\"%s\":\"%u\"", comma, name, value);
	    comma[0] = ',';

	} else if (strcmp(format, "%f") == 0) {

	    double value = va_arg(arg_list, double);
	    size += sprintf(&buffer[size], "%s\"%s\":\"%0.2f\"", comma, name, value);
	    comma[0] = ',';

	} else if (strcmp(format, "%e") == 0) {

	    double value = va_arg(arg_list, double);
	    size += sprintf(&buffer[size], "%s\"%s\":\"%0.2f%%\"", comma, name, value * 100);
	    comma[0] = ',';

	} else if (strcmp(format, "%g") == 0) {

	    char *value = va_arg(arg_list, char *);
	    size += sprintf(&buffer[size], "%s\"%s\":%s", comma, name, value);
	    comma[0] = 0;

	} else if (strcmp(format, "%i") == 0) {

	    size += sprintf(&buffer[size], "%s", name);
	    comma[0] = 0;

	} else if (strcmp(format, "%s") == 0) {

	    char *value = va_arg(arg_list, char *);
	    if (value && *value) {

		struct printbuf *json_value = NULL;
		if ((json_value = printbuf_new())) {

		    lg_string_to_json(json_value, value);
		    size += sprintf(&buffer[size], "%s\"%s\":\"%s\"", comma, name, json_value->buf);
		    printbuf_free(json_value);
		}

	    } else {

		size += sprintf(&buffer[size], "%s\"%s\":\"\"", comma, name);
	    }

	    comma[0] = ',';

	} else {

	    va_end(arg_list);
	    return;
	}
    }

    if (end_str)
	size += sprintf(&buffer[size], end_str);

    va_end(arg_list);
    write(sockfd, buffer, size);
    if (only_status)close(sockfd);
}

void ad_system_packet_send_json_end(int sockfd)
{
    write(sockfd, "]}", 2);
    close(sockfd);
}

//检测是否登录
void ad_system_packet_login_check(int sockfd, ad_system_session_t *session, char *buffer)
{

    //CODE DEMO
    //    strcpy(session->account_id, TEST_ACCOUNT_ID);
    //  return ad_system_packet_send_code(sockfd, session, NULL, PACKET_CODE_USER_LOGIN_CHECK_OK);

    if (*session->account_id)
	return ad_system_packet_send_code(sockfd, session, NULL, PACKET_CODE_USER_LOGIN_CHECK_OK);

    ad_system_packet_send_code(sockfd, session, NULL, PACKET_CODE_USER_LOGIN_CHECK_ERR);
}

//登录
void ad_system_packet_login(int sockfd, ad_system_session_t *session, json_object *root, char *buffer)
{
    json_object *ACCOUNT = NULL;
    if (!(ACCOUNT = json_object_object_get(root, "ACCOUNT")))
	return ad_system_packet_send_code(sockfd, session, NULL, PACKET_CODE_FORMAT_ERR);

    json_object *PASSWORD = NULL;
    if (!(PASSWORD = json_object_object_get(root, "PASSWORD")))
	return ad_system_packet_send_code(sockfd, session, NULL, PACKET_CODE_FORMAT_ERR);

    json_object *VAR_CODE = NULL;
    if (!(VAR_CODE = json_object_object_get(root, "VAR_CODE")))
	return ad_system_packet_send_code(sockfd, session, NULL, PACKET_CODE_FORMAT_ERR);

    char *account = NULL;
    if (!(account = (char *)json_object_get_string(ACCOUNT)))
	return ad_system_packet_send_code(sockfd, session, NULL, PACKET_CODE_PARAMETER_ERR);

    char *password = NULL;
    if (!(password = (char *)json_object_get_string(PASSWORD)))
	return ad_system_packet_send_code(sockfd, session, NULL, PACKET_CODE_PARAMETER_ERR);

    char *var_code = NULL;
    if (!(var_code = (char *)json_object_get_string(VAR_CODE)))
	return ad_system_packet_send_code(sockfd, session, NULL, PACKET_CODE_PARAMETER_ERR);

    if (!session->img.result[0])
	return ad_system_packet_send_code(sockfd, session, NULL, PACKET_CODE_USER_LOGIN_IMG_ERR);

    if (strcmp(var_code, session->img.result)) {

	session->img.result[0] = 0;
	session->account_id[0] = 0;
	return ad_system_packet_send_code(sockfd, session, NULL, PACKET_CODE_USER_LOGIN_IMG_ERR);
    }

    //初始化session
    session->img.result[0] = 0;
    session->account_id[0] = 0;

    ad_system_sql_t sql_conn;
    if (ad_system_sql_login(&sql_conn, account, password))
	return ad_system_packet_send_code(sockfd, session, NULL, PACKET_CODE_ERR);

    if (!ad_system_sql_fetch_next(&sql_conn))
	return ad_system_packet_send_code(sockfd, session, &sql_conn, PACKET_CODE_USER_LOGIN_ERR);

    const char *account_id = ad_system_sql_get_string(&sql_conn, NULL, 1);
    const char *status = ad_system_sql_get_string(&sql_conn, NULL, 2);
    if (!account_id || !status)
	return ad_system_packet_send_code(sockfd, session, &sql_conn, PACKET_CODE_USER_LOGIN_ERR);

    if (strcmp(status, "1"))
	return ad_system_packet_send_code(sockfd, session, &sql_conn, PACKET_CODE_USER_FROZEN);

    //更新登录时间地址
    strcpy(session->account_id, account_id);
    ad_system_sql_set_login(session->account_id, session->login_addr);
    ad_system_packet_send_code(sockfd, session, &sql_conn, PACKET_CODE_OK);
}

//后台管理->登录
void ad_system_packet_delivery_center_login(int sockfd, ad_system_session_t *session, ad_system_packet_json_data_t *json, char *buffer)
{

    if (strlen(json->KEY) != 36)
	return ad_system_packet_send_code(sockfd, session, NULL, PACKET_CODE_NOT_KEY);

    if (ad_system_mongo_delivery_center_find(json->KEY, session->account_id) != 0)
	return ad_system_packet_send_code(sockfd, session, NULL, PACKET_CODE_PARAMETER_ERR);

    session->time = time(NULL);
    ad_system_packet_session_send_code(sockfd, session, PACKET_CODE_OK);
}

//退出登录
void ad_system_packet_logout(int sockfd, ad_system_session_t *session, char *buffer)
{

    session->account_id[0] = 0;
    return ad_system_packet_send_code(sockfd, session, NULL, PACKET_CODE_OK);
}

void ad_system_packet_get_title_info(int sockfd, ad_system_session_t *session, char *buffer)
{
    ad_system_sql_t sql_conn;
    if ((ad_system_sql_get_title_info(&sql_conn, session->account_id)))
	return ad_system_packet_send_code(sockfd, session, NULL, PACKET_CODE_ERR);

    if (!ad_system_sql_fetch_next(&sql_conn))
	return ad_system_packet_send_code(sockfd, session, &sql_conn, PACKET_CODE_ERR);

    ad_system_packet_send_json(sockfd, buffer, SEND_JSON_DATA_ONLY, "{", "}",
	    "1%s2%s3%s4%f5%f6%f7%s8%s9%s10%s",

	    "CODE", "0",
	    "USER_LOGIN_NAME", ad_system_sql_get_string(&sql_conn, NULL, 1),
	    "COMPANY_NAME",    ad_system_sql_get_string(&sql_conn, NULL, 2),
	    "BALANCE",         ad_system_sql_get_double(&sql_conn, NULL, 3),
	    "INVEST",          ad_system_sql_get_double(&sql_conn, NULL, 4),
	    "CONSUMPTION",     ad_system_sql_get_double(&sql_conn, NULL, 5),
	    "CREATE_TIME",     ad_system_sql_get_string(&sql_conn, NULL, 6),
	    "DOMAIN",          ad_system_sql_get_string(&sql_conn, NULL, 7),
	    "GUAHAO_TOKEN",    ad_system_sql_get_string(&sql_conn, NULL, 8),
	    "WEBIM",           ad_system_sql_get_string(&sql_conn, NULL, 9)
	    );

    ad_system_sql_free(&sql_conn);
}

void ad_system_packet_get_user_info(int sockfd, ad_system_session_t *session, char *buffer)
{

    ad_system_sql_t sql_conn;
    if ((ad_system_sql_get_user_info(&sql_conn, session->account_id)))
	return ad_system_packet_send_code(sockfd, session, NULL, PACKET_CODE_ERR);

    if (!ad_system_sql_fetch_next(&sql_conn))
	return ad_system_packet_send_code(sockfd, session, &sql_conn, PACKET_CODE_ERR);

    ad_system_packet_send_json_r(sockfd, buffer, SEND_JSON_DATA_ONLY, "{", "}", 19,

	    "%s", "CODE", "0",
	    "%s", "USER_LOGIN_NAME",    ad_system_sql_get_string(&sql_conn, NULL, 1),
	    "%s", "COMPANY_NAME",      ad_system_sql_get_string(&sql_conn, NULL, 2),
	    "%s", "COMPANY_SITE",       ad_system_sql_get_string(&sql_conn, NULL, 3),
	    "%s", "INDUSTRY",          	ad_system_sql_get_string(&sql_conn, NULL, 4),
	    "%s", "API_TOKEN",         	ad_system_sql_get_string(&sql_conn, NULL, 5),
	    "%s", "PROVINCE_ID",       	ad_system_sql_get_string(&sql_conn, NULL, 6),
	    "%s", "CITY_ID",           	ad_system_sql_get_string(&sql_conn, NULL, 7),
	    "%s", "ADDRESS",           	ad_system_sql_get_string(&sql_conn, NULL, 8),
	    "%s", "POSTCODE",          	ad_system_sql_get_string(&sql_conn, NULL, 9),
	    "%s", "CONTACT",           	ad_system_sql_get_string(&sql_conn, NULL, 10),  			
	    "%s", "TELEPHONE",         	ad_system_sql_get_string(&sql_conn, NULL, 11),  			
	    "%s", "FAX",               	ad_system_sql_get_string(&sql_conn, NULL, 12),
	    "%s", "MOBILE",            	ad_system_sql_get_string(&sql_conn, NULL, 13),
	    "%s", "EMAIL",              ad_system_sql_get_string(&sql_conn, NULL, 14),
	    "%s", "WEBIM",              ad_system_sql_get_string(&sql_conn, NULL, 15),
	    "%s", "LATE_LOGIN_TIME",    ad_system_sql_get_string(&sql_conn, NULL, 16),
	    "%s", "LATE_LOGIN_IP",      ad_system_sql_get_string(&sql_conn, NULL, 17),
	    "%s", "LATE_LOGIN_ADDRESS", ad_system_sql_get_string(&sql_conn, NULL, 18));

    ad_system_sql_free(&sql_conn);
}

//设置用户信息
void ad_system_packet_set_user_info(int sockfd, ad_system_session_t *session, ad_system_packet_json_data_t *json, char *buffer)
{
    ad_system_sql_t sql_conn;
    if (ad_system_sql_set_user_info(&sql_conn,
		session->account_id,
		json->COMPANY_NAME,
		json->COMPANY_SITE,
		json->INDUSTRY,
		json->PROVINCE_ID,
		json->CITY_ID,
		json->ADDRESS,
		json->POSTCODE,
		json->CONTACT,
		json->TELEPHONE,
		json->FAX,
		json->MOBILE,
		json->EMAIL,
		json->WEBIM))
	return ad_system_packet_send_code(sockfd, NULL, NULL, PACKET_CODE_ERR);

    return ad_system_packet_send_code(sockfd, NULL, NULL, PACKET_CODE_OK);
}

//获取密保
void ad_system_packet_get_user_pass_protect(int sockfd, ad_system_session_t *session, char *buffer)
{

    return ad_system_packet_send_code(sockfd, NULL, NULL, PACKET_CODE_OK);
}

//验证密保
void ad_system_packet_authen_user_pass_protect(int sockfd, ad_system_session_t *session, 
	ad_system_packet_json_data_t *json_data, char *buffer)
{

    return ad_system_packet_send_code(sockfd, NULL, NULL, PACKET_CODE_OK);
}

//设置密保
void ad_system_packet_set_user_pass_protect(int sockfd, ad_system_session_t *session, 
	ad_system_packet_json_data_t *json_data, char *buffer)
{

    return ad_system_packet_send_code(sockfd, NULL, NULL, PACKET_CODE_OK);
}

//账户信息->修改密码
void ad_system_packet_set_user_pass(int sockfd, ad_system_session_t *session, ad_system_packet_json_data_t *json, char *buffer)
{

    if (!json->PASS_OLD || !json->PASS_OLD)
	return ad_system_packet_send_code(sockfd, session, NULL, PACKET_CODE_PARAMETER_ERR);

    ad_system_sql_t sql_conn;
    if (ad_system_sql_get_user_pass(&sql_conn, session->account_id))
	return ad_system_packet_send_code(sockfd, session, NULL, PACKET_CODE_ERR);

    if (!ad_system_sql_fetch_next(&sql_conn))
	return ad_system_packet_send_code(sockfd, session, &sql_conn, PACKET_CODE_ERR);

    const char *PASS_OLD = NULL;
    if (!(PASS_OLD = ad_system_sql_get_string(&sql_conn, NULL, 1)))
	return ad_system_packet_send_code(sockfd, session, &sql_conn, PACKET_CODE_ERR);

    if (strcmp(json->PASS_OLD, PASS_OLD))
	return ad_system_packet_send_code(sockfd, session, &sql_conn, PACKET_CODE_USER_LOGIN_ERR);

    if (ad_system_sql_set_user_pass(session->account_id, json->PASS_NEW))
	return ad_system_packet_send_code(sockfd, session, &sql_conn, PACKET_CODE_ERR);

    return ad_system_packet_send_code(sockfd, session, &sql_conn, PACKET_CODE_OK);
}

//首页->获取公告
void ad_system_packet_get_announcement(int sockfd, ad_system_session_t *session, ad_system_packet_json_data_t *json, char *buffer)
{

    ad_system_sql_t sql_count;
    if (ad_system_sql_get_announcement_count(&sql_count,
		json->FIND_TYPE==2 ? json->FIND_NAME : NULL))
	return ad_system_packet_send_code(sockfd, session, NULL, PACKET_CODE_ERR);

    if (!ad_system_sql_fetch_next(&sql_count))
	return ad_system_packet_send_code(sockfd, session, &sql_count, PACKET_CODE_ERR);

    int count = ad_system_sql_get_number(&sql_count, NULL, 1);
    ad_system_sql_free(&sql_count);

    if (!count) {

	size_t size = sprintf(buffer, "{\"CODE\":\"0\",\"INFO\":{},\"LIST\":[]}");
	return ad_system_packet_send_buffer(sockfd, session, buffer, size);
    }

    ad_system_packet_send_http_head(sockfd);
    size_t size = sprintf(buffer, "{\"CODE\":\"0\",\"INFO\":{\"COUNT\":\"%u\",\"PAGE_INDEX\":\"%u\"},\"LIST\":[",
	    count, json->PAGE_INDEX);
    send(sockfd, buffer, size, 0);

    ad_system_sql_t sql_conn;
    if (ad_system_sql_get_announcement(&sql_conn, 
		json->page_index_s, 
		json->page_index_e, 
		json->FIND_TYPE==2 ? json->FIND_NAME : NULL))
	return ad_system_packet_send_code(sockfd, session, NULL, PACKET_CODE_ERR);

    char comma[4] = "{";
    while(ad_system_sql_fetch_next(&sql_conn)) {

	ad_system_packet_send_json(sockfd, buffer, SEND_JSON_DATA_NODE, comma, "}",
		"1%s2%s3%s",

		"DATE", ad_system_sql_get_string(&sql_conn, NULL, 1),
		"TITLE", ad_system_sql_get_string(&sql_conn, NULL, 2),
		"CONTENT", ad_system_sql_get_string(&sql_conn, NULL, 3)
		); 

	strcpy(comma, ",{");
    }

    size = sprintf(buffer, "]}");
    send(sockfd, buffer, size, 0);
    close(sockfd);

    ad_system_sql_free(&sql_conn);
}

//首页->曲线图
void ad_system_packet_get_stat_trend(int sockfd, ad_system_session_t *session, ad_system_packet_json_data_t *json, char *buffer)
{

    if (!(json->DATE_TYPE == 1 || json->DATE_TYPE == 2 || json->DATE_TYPE == 3))
	return ad_system_packet_send_code(sockfd, session, NULL, PACKET_CODE_PARAMETER_ERR);

    /* 获取出价 */
    ad_system_sql_t sql_conn;
    if (ad_system_sql_get_budget(&sql_conn, session->account_id))
	return ad_system_packet_send_code(sockfd, session, NULL, PACKET_CODE_ERR);

    char img_budget[128] = "0"; 
    char text_budget[128] = "0"; 
    while(ad_system_sql_fetch_next(&sql_conn)) {

	const char *name = ad_system_sql_get_string(&sql_conn, NULL, 1);
	const char *value = ad_system_sql_get_string(&sql_conn, NULL, 2);
	if (name && value) {

	    if (strcmp(name, "1") == 0)
		strcpy(img_budget, value);
	    else if (strcmp(name, "2") == 0)
		strcpy(text_budget, value);
	}    
    }    

    ad_system_sql_free(&sql_conn);

    /* 获取 展现 点击 消费 */

    ad_system_mongo_stat_t img_data;
    ad_system_mongo_stat_t text_data;
    switch(json->DATE_TYPE) {

	case 1:
	    ad_system_mongo_stat_load(&img_data, ACCOUNT_DAY_TEMP, 1, session->account_id);
	    ad_system_mongo_stat_load(&text_data, ACCOUNT_DAY_TEMP, 2, session->account_id);
	    break;

	case 2:
	    ad_system_mongo_stat_load(&img_data, ACCOUNT_YESTERDAY_TEMP, 1, session->account_id);
	    ad_system_mongo_stat_load(&text_data, ACCOUNT_YESTERDAY_TEMP, 2, session->account_id);
	    break;

	case 3:
	    ad_system_mongo_stat_load(&img_data, ACCOUNT_SEVENDAY_TEMP, 1, session->account_id);
	    ad_system_mongo_stat_load(&text_data, ACCOUNT_SEVENDAY_TEMP, 2, session->account_id);
	    break;
    }

    ad_system_packet_send_http_head(sockfd);
    size_t size = sprintf(buffer, "{\"CODE\":\"0\","
	    "\"ALL\":{"
	    "\"IMG_BUDGET\":\"%s\","
	    "\"TEXT_BUDGET\":\"%s\","
	    "\"IMPRESSION\":\"%u\","
	    "\"CLICK\":\"%u\","
	    "\"COST\":\"%0.2f\"},"
	    "\"TEXT\":{"
	    "\"IMPRESSION\":\"%u\","
	    "\"CLICK\":\"%u\","
	    "\"COST\":\"%0.2f\"},"
	    "\"IMG\":{\"IMPRESSION\":\"%u\","
	    "\"CLICK\":\"%u\","
	    "\"COST\":\"%0.2f\"}}",

	    img_budget,
	    text_budget,
	    text_data.impression + img_data.impression,
	    text_data.click + img_data.click,
	    text_data.cost + img_data.cost,
	    text_data.impression,
	    text_data.click,
	    text_data.cost,
	    img_data.impression,
	    img_data.click,
	    img_data.cost);

    send(sockfd, buffer, size, 0);
    close(sockfd);
}

//首页->曲线图->TEXT
void ad_system_packet_get_stat_trend_text(int sockfd, ad_system_session_t *session, char *buffer)
{

    lg_list_t head;
    ad_system_mongo_stat_list_load(&head, ACCOUNT_SEVENDAY_DAILY_TEMP, 2, session->account_id, "time");

    char srv_time[128];
    ad_system_packet_send_http_head(sockfd);
    size_t size = sprintf(buffer, "{\"CODE\":\"0\",\"TIME\":\"%s\",\"LIST\":[",
	    lg_string_get_time_string(srv_time));
    send(sockfd, buffer, size, 0);

    char comma[4] = "";
    lg_list_t *p = NULL;
    lg_list_for_each(p, &head) { 

	ad_system_mongo_stat_t *node = (ad_system_mongo_stat_t *)p;

	double ctr = 0;
	if (node->click && node->impression)
	    ctr = (double)node->click / (double)node->impression;
	double cpc = 0;
	if (node->cost && node->click)
	    cpc = node->cost / (double)node->click;
	double cpm = 0;
	if (node->cost && node->impression)
	    cpm = node->cost / (double)node->impression * (double)1000;

	size = sprintf(buffer, "%s{"
		"\"DATE\":\"%s\","
		"\"IMPRESSION\":\"%u\","
		"\"CLICK\":\"%u\","
		"\"COST\":\"%0.2f\","
		"\"CTR\":\"%0.2f\","
		"\"CPC\":\"%0.2f\","
		"\"CPM\":\"%0.2f\"}",

		comma,
		node->name,
		node->impression,
		node->click,
		node->cost,
		ctr * 100,
		cpc,
		cpm);

	send(sockfd, buffer, size, 0);
	comma[0] = ','; 
    }    

    size = sprintf(buffer, "]}");
    send(sockfd, buffer, size, 0);
    close(sockfd);

    ad_system_mongo_list_free(&head);
}

//首页->曲线图->IMG
void ad_system_packet_get_stat_trend_img(int sockfd, ad_system_session_t *session, char *buffer)
{

    lg_list_t head;
    ad_system_mongo_stat_list_load(&head, ACCOUNT_SEVENDAY_DAILY_TEMP, 1, session->account_id, "time");

    char srv_time[128];
    ad_system_packet_send_http_head(sockfd);
    size_t size = sprintf(buffer, "{\"CODE\":\"0\",\"TIME\":\"%s\",\"LIST\":[",
	    lg_string_get_time_string(srv_time));
    send(sockfd, buffer, size, 0);

    char comma[4] = "";
    lg_list_t *p = NULL;
    lg_list_for_each(p, &head) { 

	ad_system_mongo_stat_t *node = (ad_system_mongo_stat_t *)p;

	double ctr = 0;
	if (node->click && node->impression)
	    ctr = (double)node->click / (double)node->impression;
	double cpc = 0;
	if (node->cost && node->click)
	    cpc = node->cost / (double)node->click;
	double cpm = 0;
	if (node->cost && node->impression)
	    cpm = node->cost / (double)node->impression * (double)1000;

	size = sprintf(buffer, "%s{"
		"\"DATE\":\"%s\","
		"\"IMPRESSION\":\"%u\","
		"\"CLICK\":\"%u\","
		"\"COST\":\"%0.2f\","
		"\"CTR\":\"%0.2f\","
		"\"CPC\":\"%0.2f\","
		"\"CPM\":\"%0.2f\"}",

		comma,
		node->name,
		node->impression,
		node->click,
		node->cost,
		ctr * 100,
		cpc,
		cpm);

	send(sockfd, buffer, size, 0);
	comma[0] = ','; 
    }    

    size = sprintf(buffer, "]}");
    send(sockfd, buffer, size, 0);
    close(sockfd);

    ad_system_mongo_list_free(&head);
}

//财务管理->曲线图
void ad_system_packet_get_money_trend(int sockfd, ad_system_session_t *session, char *buffer)
{

}

void ad_system_packet_get_money_recharge(int sockfd, ad_system_session_t *session, ad_system_packet_json_data_t *json, char *buffer)
{

    ad_system_sql_t sql_count;
    if (ad_system_sql_get_money_recharge_count(&sql_count,
		session->account_id,
		json->FIND_TYPE,
		json->MONEY_TYPE,
		json->FIND_NAME_TYPE,
		json->FIND_NAME,
		json->TIME_START,
		json->TIME_END))
	return ad_system_packet_send_code(sockfd, session, NULL, PACKET_CODE_ERR);

    ad_system_sql_t sql_data;
    if (ad_system_sql_get_money_recharge(&sql_data,
		session->account_id,
		json->FIND_TYPE,
		json->MONEY_TYPE,
		json->FIND_NAME_TYPE,
		json->FIND_NAME,
		json->TIME_START,
		json->TIME_END,
		json->SORT_TYPE,
		json->SORT_COLUMN,
		json->page_index_s,
		json->page_index_e)) {

	ad_system_sql_free(&sql_count);
	return ad_system_packet_send_code(sockfd, session, NULL, PACKET_CODE_ERR);
    }

    if (!(ad_system_sql_fetch_next(&sql_count))) {

	ad_system_sql_free(&sql_count);
	ad_system_sql_free(&sql_data);
	return ad_system_packet_send_code(sockfd, session, NULL, PACKET_CODE_ERR);
    }

    ad_system_packet_send_http_head(sockfd);
    size_t size = sprintf(buffer, "{\"CODE\":\"0\",\"INFO\":{\"PAGE_INDEX\":\"%u\",\"COUNT\":\"%u\"},\"LIST\":[", 
	    json->PAGE_INDEX, 
	    ad_system_sql_get_number(&sql_count, NULL, 1));
    send(sockfd, buffer, size, 0);

    char comma[4] = "";
    while(ad_system_sql_fetch_next(&sql_data)) {

	size = sprintf(buffer, "%s{"
		"\"SWIFT_NUMBER\":\"%s\","
		"\"MONEY\":\"%s\","
		"\"PAYMENT_TYPE\":\"%s\","
		"\"ACCOUNTING_DOCUMENT_NUMBER\":\"%s\","
		"\"ACCOUNTING_DOCUMENT_FILE\":\"%s\","
		"\"INVEST\":\"%s\","
		"\"BALANCE\":\"%s\","
		"\"CREATE_TIME\":\"%s\"}",

		comma,
		ad_system_sql_get_string(&sql_data, NULL, 1),
		ad_system_sql_get_string(&sql_data, NULL, 2),
		ad_system_sql_get_string(&sql_data, NULL, 3),
		ad_system_sql_get_string(&sql_data, NULL, 4),
		ad_system_sql_get_string(&sql_data, NULL, 5),
		ad_system_sql_get_string(&sql_data, NULL, 6),
		ad_system_sql_get_string(&sql_data, NULL, 7),
		ad_system_sql_get_string(&sql_data, NULL, 8));

	comma[0] = ',';
	write(sockfd, buffer, size);
    }

    size = sprintf(buffer, "]}");
    send(sockfd, buffer, size, 0);
    close(sockfd);

    ad_system_sql_free(&sql_count);
    ad_system_sql_free(&sql_data);
}

//推广->获取科室病种CODE
void ad_system_packet_get_disease_code(int sockfd, ad_system_session_t *session, char *buffer)
{

    ad_system_sql_t sql_conn;
    if (ad_system_sql_get_disease_code(&sql_conn))
	return ad_system_packet_send_code(sockfd, session, NULL, PACKET_CODE_ERR);

    ad_system_packet_send_http_head(sockfd);
    send(sockfd, "{", 1, 0);

    char comma[4] = "";
    while(ad_system_sql_fetch_next(&sql_conn)) {

	size_t size = sprintf(buffer, "%s\"%s\":\"%s\"", 
		comma, 
		ad_system_sql_get_string(&sql_conn, NULL, 2),
		ad_system_sql_get_string(&sql_conn, NULL, 1));

	send(sockfd, buffer, size, 0);
	comma[0] = ',';
    }

    send(sockfd, "}", 1, 0);
    close(sockfd);
    ad_system_sql_free(&sql_conn);
}

//计划重命名验证
void ad_system_packet_campaign_name_check(int sockfd, ad_system_session_t *session, ad_system_packet_json_data_t *json, char *buffer)
{

    int ret = ad_system_sql_campaign_name_check(session->account_id, json->CAMPAIGN_NAME, json->DISPLAY_STATUS);
    if (ret == -1) 
	return ad_system_packet_send_code(sockfd, session, NULL, PACKET_CODE_ERR);
    else if (ret != 100)
	return ad_system_packet_send_code(sockfd, session, NULL, PACKET_CODE_NAME_REPEAT);

    return ad_system_packet_send_code(sockfd, session, NULL, PACKET_CODE_OK);
}

//组重命名验证
void ad_system_packet_group_name_check(int sockfd, ad_system_session_t *session, ad_system_packet_json_data_t *json, char *buffer)
{

    int ret = ad_system_sql_group_name_check(json->CAMPAIGN_ID, json->DISPLAY_STATUS, json->GROUP_NAME);
    if (ret == -1) 
	return ad_system_packet_send_code(sockfd, session, NULL, PACKET_CODE_ERR);
    else if (ret != 100)
	return ad_system_packet_send_code(sockfd, session, NULL, PACKET_CODE_NAME_REPEAT);

    return ad_system_packet_send_code(sockfd, session, NULL, PACKET_CODE_OK);
}

//推广->添加计划
void ad_system_packet_add_campaign(int sockfd, ad_system_session_t *session, ad_system_packet_json_data_t *json, char *buffer)
{

    int ret = ad_system_sql_campaign_name_check(session->account_id, json->CAMPAIGN_NAME, json->DISPLAY_STATUS);
    if (ret == -1) 
	return ad_system_packet_send_code(sockfd, session, NULL, PACKET_CODE_ERR);
    else if (ret != 100)
	return ad_system_packet_send_code(sockfd, session, NULL, PACKET_CODE_NAME_REPEAT);

    char campaign_id[1024] = {0};
    if (ad_system_sql_get_campaign_id(campaign_id))
	return ad_system_packet_send_code(sockfd, session, NULL, PACKET_CODE_OK);

    if (ad_system_sql_add_campaign(
		session->account_id,
		json->DISPLAY_STATUS,
		campaign_id,
		json->CAMPAIGN_NAME,
		json->CAMPAIGN_BUDGET,
		json->REGION_TYPE,
		json->REGION_SERIALIZE,
		json->SCHEDULE_TYPE,
		json->SCHEDULE_SERIALIZE,
		json->TIME_TYPE,
		json->BEGIN_TIME,
		json->END_TIME))
	return ad_system_packet_send_code(sockfd, session, NULL, PACKET_CODE_ERR);

    size_t size = sprintf(buffer, "{\"CODE\":\"0\",\"CAMPAIGN_ID\":\"%s\"}", campaign_id);
    ad_system_packet_send_buffer(sockfd, session, buffer, size);
}

//删除计划
void ad_system_packet_del_campaign(int sockfd, ad_system_session_t *session, ad_system_packet_json_data_t *json, char *buffer)
{

    if (ad_system_sql_del_campaign(json->CAMPAIGN_ID))
	return ad_system_packet_send_code(sockfd, session, NULL, PACKET_CODE_ERR);
    ad_system_packet_send_code(sockfd, session, NULL, PACKET_CODE_OK);
}

//删除组
void ad_system_packet_del_group(int sockfd, ad_system_session_t *session, ad_system_packet_json_data_t *json, char *buffer)
{

    if (ad_system_sql_del_group(json->GROUP_ID))
	return ad_system_packet_send_code(sockfd, session, NULL, PACKET_CODE_ERR);
    ad_system_packet_send_code(sockfd, session, NULL, PACKET_CODE_OK);
}

//删除图片创意
void ad_system_packet_del_creative_img(int sockfd, ad_system_session_t *session, ad_system_packet_json_data_t *json, char *buffer)
{

    if (ad_system_sql_del_creative_img(json->CREATIVE_ID))
	return ad_system_packet_send_code(sockfd, session, NULL, PACKET_CODE_ERR);
    ad_system_packet_send_code(sockfd, session, NULL, PACKET_CODE_OK);
}

//删除文字创意
void ad_system_packet_del_creative_text(int sockfd, ad_system_session_t *session, ad_system_packet_json_data_t *json, char *buffer)
{

    if (ad_system_sql_del_creative_text(json->CREATIVE_ID))
	return ad_system_packet_send_code(sockfd, session, NULL, PACKET_CODE_ERR);
    ad_system_packet_send_code(sockfd, session, NULL, PACKET_CODE_OK);
}

//删除附加创意
void ad_system_packet_del_creative_append(int sockfd, ad_system_session_t *session, ad_system_packet_json_data_t *json, char *buffer)
{

    if (ad_system_sql_del_creative_append(json->CREATIVE_APPEND_ID))
	return ad_system_packet_send_code(sockfd, session, NULL, PACKET_CODE_ERR);
    ad_system_packet_send_code(sockfd, session, NULL, PACKET_CODE_OK);
}

extern int ad_system_merger_schedule(const char *sysdate, const char *schedule);
void ad_system_packet_get_group_unique(int sockfd, ad_system_session_t *session, ad_system_packet_json_data_t *json, char *buffer)
{

    ad_system_sql_t sql_conn;
    if (ad_system_sql_get_group_unique(&sql_conn, json->GROUP_ID))
	return ad_system_packet_send_code(sockfd, session, NULL, PACKET_CODE_ERR);

    if (!ad_system_sql_fetch_next(&sql_conn))
	return ad_system_packet_send_code(sockfd, session, NULL, PACKET_CODE_ERR);

    const char *CAMPAIGN_ID = ad_system_sql_get_string(&sql_conn, NULL, 1);
    const char *CAMPAIGN_NAME = ad_system_sql_get_string(&sql_conn, NULL, 2);
    const char *GROUP_ID = ad_system_sql_get_string(&sql_conn, NULL, 3);
    const char *GROUP_NAME = ad_system_sql_get_string(&sql_conn, NULL, 4);
    double GROUP_CPC_PRICE = ad_system_sql_get_double(&sql_conn, NULL, 5);
    double GROUP_CPM_PRICE = ad_system_sql_get_double(&sql_conn, NULL, 6);
    int GROUP_STATE = ad_system_sql_get_number(&sql_conn, NULL, 7);
    int CAMPAIGN_PAUSE = ad_system_sql_get_number(&sql_conn, NULL, 8);
    int CAMPAIGN_STATE = ad_system_sql_get_number(&sql_conn, NULL, 9);
    int SCHEDULE_TYPE = ad_system_sql_get_number(&sql_conn, NULL, 10);
    const char *SCHEDULE_SYS_TIME = ad_system_sql_get_string(&sql_conn, NULL, 11);
    const char *SCHEDULE_SERIALIZE = ad_system_sql_get_string(&sql_conn, NULL, 12);
    int DATE_SUB_BEGIN_TIME = ad_system_sql_get_number(&sql_conn, NULL, 13);
    int DATE_SUB_END_TIME = ad_system_sql_get_number(&sql_conn, NULL, 14);
    int GROUP_TARGETING_TYPE = ad_system_sql_get_number(&sql_conn, NULL, 15);
    const char *GROUP_DISEASE = ad_system_sql_get_string(&sql_conn, NULL, 16);

    if (GROUP_STATE == 2) {
	GROUP_STATE = 8;//组暂停

    } else if (CAMPAIGN_PAUSE == 2) {
	GROUP_STATE = 5;//计划暂停

    } else if (DATE_SUB_BEGIN_TIME == 2) {
	GROUP_STATE = 4;//未开始

    } else if (DATE_SUB_END_TIME == 2) {
	GROUP_STATE = 6;//下线

    } else if (CAMPAIGN_STATE == 13) {
	GROUP_STATE = 3;//预算不足

    } else if (SCHEDULE_TYPE == 2 
	    && ad_system_merger_schedule(SCHEDULE_SYS_TIME, SCHEDULE_SERIALIZE) == 0) {
	GROUP_STATE = 2;//暂停时段

    } else {
	GROUP_STATE = 1;//有效
    }

    ad_system_packet_send_json_r(sockfd, buffer, SEND_JSON_DATA_ONLY, "{", "}", 10,
	    "%s", "CODE", "0",
	    "%s", "CAMPAIGN_ID",                CAMPAIGN_ID,
	    "%s", "CAMPAIGN_NAME",              CAMPAIGN_NAME,
	    "%s", "GROUP_ID",                   GROUP_ID,
	    "%s", "GROUP_NAME",                 GROUP_NAME,
	    "%u", "GROUP_STATUS",               GROUP_STATE,
	    "%f", "GROUP_CPC_PRICE",            GROUP_CPC_PRICE,
	    "%f", "GROUP_CPM_PRICE",            GROUP_CPM_PRICE,
	    "%u", "GROUP_TARGETING_TYPE",       GROUP_TARGETING_TYPE,
	    "%s", "GROUP_DISEASE",              GROUP_DISEASE,
	    "%u", "GROUP_IMPRESSION",           0,
	    "%u", "GROUP_CLICK",                0,
	    "%e", "GROUP_CTR",                  0,
	    "%f", "GROUP_CPC",                  0,
	    "%f", "GROUP_COST",                 0,
	    "%f", "GROUP_CPM",                  0);

    ad_system_sql_free(&sql_conn);
}

void ad_system_packet_get_creative_append_unique(int sockfd, ad_system_session_t *session, 
	ad_system_packet_json_data_t *json, char *buffer)
{

    ad_system_sql_t sql_conn;
    if (ad_system_sql_get_creative_append_unique(&sql_conn, json->CREATIVE_APPEND_ID))
	ad_system_packet_send_code(sockfd, session, NULL, PACKET_CODE_OK);

    if (!ad_system_sql_fetch_next(&sql_conn))
	ad_system_packet_send_code(sockfd, session, NULL, PACKET_CODE_OK);

    ad_system_packet_send_json(sockfd, buffer, SEND_JSON_DATA_ONLY, "{", "}", "%s%s%s%s%s",
	    "CODE", "0",		
	    "APPEND_ID",		ad_system_sql_get_string(&sql_conn, NULL, 1),
	    "APPEND_STATUS",            ad_system_sql_get_string(&sql_conn, NULL, 2),
	    "APPEND_CONTENT",           ad_system_sql_get_string(&sql_conn, NULL, 3),
	    "APPEND_REFUSE_REASON",     ad_system_sql_get_string(&sql_conn, NULL, 4)
	    );

    ad_system_sql_free(&sql_conn);
}

void ad_system_packet_batch_operation(int sockfd, ad_system_session_t *session, ad_system_packet_json_data_t *json, char *buffer)
{

    strcpy(buffer, json->ID);
    char *id_array = buffer;

    char *id = NULL;
    char *saveptr = NULL;
    while((id = strtok_r(id_array, ",", &saveptr))) {

	if (json->BATCH_TYPE == 1) {

	    switch(json->ID_TYPE){

		case 1:
		    ad_system_sql_set_campaign(session->account_id, id, 1, json->STATUS, NULL, 0, NULL, 0, NULL, 0, NULL, NULL, NULL);
		    break;
		case 2:
		    ad_system_sql_set_group(session->account_id, id, 1, json->STATUS, NULL, NULL, 0, 0, NULL);
		    break;
		case 3:
		    ad_system_sql_set_creative_text(id, json->STATUS);
		    break;
		case 4:
		    ad_system_sql_set_creative_append(session->account_id, id, 1, json->STATUS, NULL);
		    break;
		case 5:
		    ad_system_sql_set_creative_img(session->account_id, id, json->STATUS);
		    break;
	    }

	} else if (json->BATCH_TYPE == 2) {

	    switch(json->ID_TYPE){

		case 1:
		    ad_system_sql_del_campaign(id);
		    break;
		case 2:
		    ad_system_sql_del_group(id);
		    break;
		case 3:
		    ad_system_sql_del_creative_text(id);
		    break;
		case 4:
		    ad_system_sql_del_creative_append(id);
		    break;
		case 5:
		    ad_system_sql_del_creative_img(id);
		    break;
	    }
	}

	id_array = NULL;
    }

    ad_system_packet_send_code(sockfd, session, NULL, PACKET_CODE_OK);
}

void ad_system_packet_add_creative_text(int sockfd, ad_system_session_t *session, ad_system_packet_json_data_t *json, char *buffer)
{

    ad_system_sql_t sql_campaign_id;
    if (ad_system_sql_get_campaign_group_from_group_or_creative_img(&sql_campaign_id, json->GROUP_ID, NULL))
	return ad_system_packet_send_code(sockfd, session, NULL, PACKET_CODE_ERR);

    if (ad_system_sql_add_creative_text(
		session->account_id,
		ad_system_sql_get_string(&sql_campaign_id, NULL, 1),
		ad_system_sql_get_string(&sql_campaign_id, NULL, 2),
		json->CREATIVE_NAME,
		json->CREATIVE_DESCRIPTION,
		json->CREATIVE_DESTINATION_URL,
		json->CREATIVE_DISPLAY_URL,
		json->CREATIVE_APPEND_TYPE,
		json->CREATIVE_APPEND_ID,
		json->MOBILE_DESTINATION_URL,
		json->MOBILE_DISPLAY_URL))
	return ad_system_packet_send_code(sockfd, session, &sql_campaign_id, PACKET_CODE_ERR);

    return ad_system_packet_send_code(sockfd, session, &sql_campaign_id, PACKET_CODE_OK);

}

void ad_system_packet_set_campaign(int sockfd, ad_system_session_t *session, ad_system_packet_json_data_t *json, char *buffer)
{

    if (ad_system_sql_set_campaign(session->account_id,
		json->CAMPAIGN_ID,
		json->UPDATE_TYPE,
		json->CAMPAIGN_STATUS,
		json->CAMPAIGN_BUDGET,
		json->REGION_TYPE,
		json->REGION_SERIALIZE,
		json->SCHEDULE_TYPE,
		json->SCHEDULE_SERIALIZE,
		json->TIME_TYPE,
		json->BEGIN_TIME,
		json->END_TIME,
		json->CAMPAIGN_NAME))
	return ad_system_packet_send_code(sockfd, session, NULL, PACKET_CODE_ERR);

    return ad_system_packet_send_code(sockfd, session, NULL, PACKET_CODE_OK);
}

void ad_system_packet_set_upload_img_info(int sockfd, ad_system_session_t *session, ad_system_packet_json_data_t *json, char *buffer)
{
    ad_system_image_t *img = NULL;
    if (ad_system_image_list_find(json->CREATIVE_IMG_ID))
	return ad_system_packet_send_code(sockfd, session, NULL, PACKET_CODE_IMG_ID_ERR);

    if (json->CREATIVE_NAME)
	strcpy(img->name, json->CREATIVE_NAME);

    ad_system_packet_send_code(sockfd, session, NULL, PACKET_CODE_OK);
}

void ad_system_packet_add_group(int sockfd, ad_system_session_t *session, ad_system_packet_json_data_t *json, char *buffer)
{

    char disease_code[40960] = {0};
    if (json->GROUP_TARGETING_TYPE == 1)
	ad_system_disease_code_perse(disease_code, json->GROUP_DISEASE);

    int ret = ad_system_sql_group_name_check(json->CAMPAIGN_ID, json->DISPLAY_STATUS, json->GROUP_NAME);
    if (ret == -1) 
	return ad_system_packet_send_code(sockfd, session, NULL, PACKET_CODE_ERR);
    else if (ret != 100)
	return ad_system_packet_send_code(sockfd, session, NULL, PACKET_CODE_NAME_REPEAT);

    char group_id[1024] = {0};
    if (ad_system_sql_get_group_id(group_id))
	return ad_system_packet_send_code(sockfd, session, NULL, PACKET_CODE_OK);

    if (json->DISPLAY_STATUS == 2)//文字没有CPM
	json->GROUP_PRICE_TYPE = 2;

    if (ad_system_sql_add_group(
		session->account_id,
		json->DISPLAY_STATUS,
		json->CAMPAIGN_ID,
		group_id,
		json->GROUP_NAME,
		json->GROUP_PRICE_TYPE,
		json->GROUP_CPC_PRICE,
		json->GROUP_CPM_PRICE,
		json->GROUP_TARGETING_TYPE))
	return ad_system_packet_send_code(sockfd, session, NULL, PACKET_CODE_OK);

    if (ad_system_sql_add_group_targeting(
		session->account_id,
		json->CAMPAIGN_ID,
		group_id,
		json->GROUP_TARGETING_TYPE,
		disease_code,
		json->GROUP_KEYWORD))
	return ad_system_packet_send_code(sockfd, session, NULL, PACKET_CODE_OK);

    if (json->GROUP_TARGETING_TYPE == 2) {

	if (!(json->GROUP_KEYWORD && *json->GROUP_KEYWORD))
	    return ad_system_packet_send_code(sockfd, session, NULL, PACKET_CODE_FORMAT_ERR);

	char *keyword = NULL;
	char *saveptr = NULL;
	char *keywordlist = buffer;
	strcpy(buffer, json->GROUP_KEYWORD);
	while((keyword= strtok_r(keywordlist, ",", &saveptr))) {

	    ad_system_sql_add_group_keyword(session->account_id, group_id, keyword);
	    keywordlist = NULL;
	}

	ad_system_sql_group_synch_state(session->account_id, json->GROUP_ID, NULL);
    }

    size_t size = sprintf(buffer, "{\"CODE\":\"0\",\"GROUP_ID\":\"%s\"}", group_id);
    ad_system_packet_send_buffer(sockfd, session, buffer, size);
}

void ad_system_packet_set_group(int sockfd, ad_system_session_t *session, ad_system_packet_json_data_t *json, char *buffer)
{

    char disease_code[40960] = {0};
    if (json->UPDATE_TYPE & 4 && json->GROUP_TARGETING_TYPE == 1)
	ad_system_disease_code_perse(disease_code, json->GROUP_DISEASE);

    ad_system_sql_t sql_budget_and_bidding;
    if (ad_system_sql_get_budget_and_bidding(&sql_budget_and_bidding, session->account_id, json->GROUP_ID))
	return ad_system_packet_send_code(sockfd, session, NULL, PACKET_CODE_ERR);
    if (!ad_system_sql_fetch_next(&sql_budget_and_bidding))
	return ad_system_packet_send_code(sockfd, session, &sql_budget_and_bidding, PACKET_CODE_USER_LOGIN_ERR);

    double CAMPAIGN_BUDGET = ad_system_sql_get_double(&sql_budget_and_bidding, NULL, 1);
    int GROUP_BIDDING_MODEL = ad_system_sql_get_number(&sql_budget_and_bidding, NULL, 2);
    //double GROUP_CPC_PRICE = ad_system_sql_get_double(&sql_budget_and_bidding, NULL, 3);
    //double GROUP_CPM_PRICE = ad_system_sql_get_double(&sql_budget_and_bidding, NULL, 4);
    ad_system_sql_free(&sql_budget_and_bidding);

    if (json->UPDATE_TYPE & 2) {

	if (strcmp(json->GROUP_CPC_PRICE, "null") == 0)
	    if (!(GROUP_BIDDING_MODEL & 1))
		return ad_system_packet_send_code(sockfd, session, NULL, PACKET_CODE_PIRCE_ERR);

	if (atof(json->GROUP_CPC_PRICE) > CAMPAIGN_BUDGET)
	    return ad_system_packet_send_code(sockfd, session, NULL, PACKET_CODE_PRICE_EXPENSIVE);
    }

    if (json->UPDATE_TYPE & 16) {

	if (strcmp(json->GROUP_CPM_PRICE, "null") == 0)
	    if (!(GROUP_BIDDING_MODEL & 2))
		return ad_system_packet_send_code(sockfd, session, NULL, PACKET_CODE_PIRCE_ERR);

	if (atof(json->GROUP_CPM_PRICE) > CAMPAIGN_BUDGET * 1000)
	    return ad_system_packet_send_code(sockfd, session, NULL, PACKET_CODE_PRICE_EXPENSIVE);
    }

    if (ad_system_sql_set_group(session->account_id,
		json->GROUP_ID,
		json->UPDATE_TYPE,
		json->STATUS,
		json->GROUP_CPC_PRICE,
		json->GROUP_CPM_PRICE,
		GROUP_BIDDING_MODEL,
		json->GROUP_TARGETING_TYPE,
		json->GROUP_NAME))
	return ad_system_packet_send_code(sockfd, session, NULL, PACKET_CODE_ERR);

    if (json->UPDATE_TYPE & 4)
	if (ad_system_sql_set_group_targeting(
		    session->account_id,
		    json->GROUP_ID,
		    json->UPDATE_TYPE, 
		    json->GROUP_TARGETING_TYPE, 
		    disease_code, 
		    json->GROUP_KEYWORD))
	    return ad_system_packet_send_code(sockfd, session, NULL, PACKET_CODE_ERR);

    ad_system_packet_send_code(sockfd, session, NULL, PACKET_CODE_OK);
}

void ad_system_packet_set_creative_text(int sockfd, ad_system_session_t *session, ad_system_packet_json_data_t *json, char *buffer)
{

    if (json->UPDATE_TYPE == 1) {

	if (ad_system_sql_set_creative_text(json->CREATIVE_ID, json->STATUS))
	    return ad_system_packet_send_code(sockfd, session, NULL, PACKET_CODE_ERR);

    } else {

	if (ad_system_sql_copy_creative_text(
		    json->UPDATE_TYPE,
		    json->CREATIVE_ID,
		    json->CREATIVE_NAME,
		    json->CREATIVE_DESCRIPTION,
		    json->CREATIVE_DESTINATION_URL,
		    json->CREATIVE_DISPLAY_URL,
		    json->CREATIVE_APPEND_TYPE,
		    json->CREATIVE_APPEND_ID,
		    json->MOBILE_DESTINATION_URL,
		    json->MOBILE_DISPLAY_URL))
	    return ad_system_packet_send_code(sockfd, session, NULL, PACKET_CODE_ERR);

	if (ad_system_sql_del_creative_text(json->CREATIVE_ID))
	    return ad_system_packet_send_code(sockfd, session, NULL, PACKET_CODE_ERR);
    }

    ad_system_packet_send_code(sockfd, session, NULL, PACKET_CODE_OK);
}

void ad_system_packet_set_creative_img(int sockfd, ad_system_session_t *session, ad_system_packet_json_data_t *json, char *buffer)
{

    if (json->UPDATE_TYPE & 2) {

	if (ad_system_sql_set_creative_img(
		    session->account_id,
		    json->CREATIVE_ID,
		    json->CREATIVE_STATUS))
	    return ad_system_packet_send_code(sockfd, session, NULL, PACKET_CODE_OK);

    } else {

	ad_system_sql_t sql_conn;
	if (ad_system_sql_connect(&sql_conn))
	    return ad_system_packet_send_code(sockfd, session, NULL, PACKET_CODE_ERR);

	if (json->UPDATE_TYPE & 16) {

	    //查询上传的图片信息
	    ad_system_image_t *img = NULL;
	    if (!(img = ad_system_image_list_find(json->CREATIVE_IMG_ID)))
		return ad_system_packet_send_code(sockfd, session, NULL, PACKET_CODE_IMG_ID_ERR);

	    //查询所属组与计划
	    ad_system_sql_t sql_campaign_id;
	    if (ad_system_sql_get_campaign_group_from_group_or_creative_img(&sql_campaign_id, json->GROUP_ID, NULL))
		return ad_system_packet_send_code(sockfd, session, &sql_conn, PACKET_CODE_ERR);

	    //创建新的图片创意
	    if (ad_system_sql_add_creative_img(&sql_conn,
			session->account_id,
			ad_system_sql_get_string(&sql_campaign_id, NULL, 1),
			ad_system_sql_get_string(&sql_campaign_id, NULL, 2),
			img->name,
			img->path,
			img->width,
			img->height,
			img->length,
			json->CREATIVE_DESTINATION_URL,
			json->CREATIVE_DISPLAY_URL)) {

		ad_system_sql_rollback(&sql_conn);
		ad_system_sql_free(&sql_conn);
		ad_system_sql_free(&sql_campaign_id);
		return ad_system_packet_send_code(sockfd, session, NULL, PACKET_CODE_ERR);
	    }

	} else {

	    if (ad_system_sql_copy_creative_img(&sql_conn,
			json->UPDATE_TYPE,
			json->CREATIVE_ID,
			json->CREATIVE_NAME,
			json->CREATIVE_DESTINATION_URL,
			json->CREATIVE_DISPLAY_URL)) {

		ad_system_sql_rollback(&sql_conn);
		ad_system_sql_free(&sql_conn);
		return ad_system_packet_send_code(sockfd, session, NULL, PACKET_CODE_ERR);
	    }
	}

	//删除原有创意
	if (ad_system_sql_del_creative_img(json->CREATIVE_ID)) {

	    ad_system_sql_rollback(&sql_conn);
	    ad_system_sql_free(&sql_conn);
	    return ad_system_packet_send_code(sockfd, session, NULL, PACKET_CODE_ERR);
	}

	//提交并释放内存
	ad_system_sql_commit(&sql_conn);
	ad_system_sql_free(&sql_conn);
	return ad_system_packet_send_code(sockfd, session, NULL, PACKET_CODE_OK);
    }

    return ad_system_packet_send_code(sockfd, session, NULL, PACKET_CODE_OK);
}

//推广->获取计划
void ad_system_packet_get_campaign(int sockfd, ad_system_session_t *session, ad_system_packet_json_data_t *json, char *buffer)
{

    ad_system_sql_t sql_conn;
    if (ad_system_sql_get_campaign(&sql_conn,
		session->account_id, 
		json->DISPLAY_STATUS,
		json->FIND_TYPE,
		json->FIND_NAME_TYPE,
		json->FIND_NAME))
	return ad_system_packet_send_code(sockfd, session, NULL, PACKET_CODE_ERR);

    //获取mongo数据
    lg_list_t mongo_head;
    if (json->DATE_TYPE == 1)
	ad_system_mongo_stat_list_load(&mongo_head, CAMPAIGN_DAY_TEMP, json->DISPLAY_STATUS, session->account_id, "campaign_id");
    else if (json->DATE_TYPE == 2)
	ad_system_mongo_stat_list_load(&mongo_head, CAMPAIGN_YESTERDAY_TEMP, json->DISPLAY_STATUS, session->account_id, "campaign_id");
    else if (json->DATE_TYPE == 3)
	ad_system_mongo_stat_list_load(&mongo_head, CAMPAIGN_SEVENDAY_TEMP, json->DISPLAY_STATUS, session->account_id, "campaign_id");
    else
	return ad_system_packet_send_code(sockfd, session, NULL, PACKET_CODE_FORMAT_ERR);

    //合并数据
    lg_list_t merger_head;
    ad_system_merger_campaign_info_t info;
    ad_system_merger_campaign(
	    &sql_conn, 
	    &mongo_head, 
	    json,
	    &merger_head,
	    &info);

    //发送数据
    ad_system_packet_send_http_head(sockfd);
    size_t size = sprintf(buffer, "{\"CODE\":\"0\","
	    "\"INFO\":{"
	    "\"PAGE_INDEX\":\"%u\","
	    "\"CAMPAIGN_COUNT\":\"%u\","
	    "\"CAMPAIGN_ENABLED\":\"%u\","
	    "\"CAMPAIGN_BUDGET\":\"%0.2f\","
	    "\"CAMPAIGN_IMPRESSION\":\"%u\","
	    "\"CAMPAIGN_CLICK\":\"%u\","
	    "\"CAMPAIGN_CTR\":\"%0.2f%%\","
	    "\"CAMPAIGN_CPC\":\"%0.2f\","
	    "\"CAMPAIGN_COST\":\"%0.2f\","
	    "\"CAMPAIGN_CPM\":\"%0.2f\""
	    "},\"LIST\":[",

	    json->PAGE_INDEX,
	    info.CAMPAIGN_COUNT,
	    info.CAMPAIGN_ENABLED,
	    info.CAMPAIGN_BUDGET,
	    info.CAMPAIGN_IMPRESSION,
	    info.CAMPAIGN_CLICK,
	    info.CAMPAIGN_CTR * 100,
	    info.CAMPAIGN_CPC,
	    info.CAMPAIGN_COST,
	    info.CAMPAIGN_CPM);
    send(sockfd, buffer, size, 0);

    int page_index = 1;
    char comma[4] = "{";
    lg_list_t *p = NULL;
    lg_list_for_each(p, &merger_head) {

	ad_system_merger_campaign_t *node = NULL;
	node = (ad_system_merger_campaign_t *)p;

	if (page_index >= json->page_index_s && page_index <= json->page_index_e) {

	    ad_system_packet_send_json(sockfd, buffer, SEND_JSON_DATA_NODE, comma, "}",
		    "1%s2%s3%u4%f5%u6%s7%u8%s9%u10%s11%s12%u13%u14%e15%f16%f17%f",

		    "CAMPAIGN_ID",          node->CAMPAIGN_ID,
		    "CAMPAIGN_NAME",        node->CAMPAIGN_NAME,
		    "CAMPAIGN_STATUS",      node->CAMPAIGN_STATE,
		    "CAMPAIGN_BUDGET",      node->CAMPAIGN_BUDGET,
		    "SCHEDULE_TYPE",        node->SCHEDULE_TYPE,
		    "SCHEDULE_SERIALIZE",   node->SCHEDULE_SERIALIZE,
		    "REGION_TYPE",          node->REGION_TYPE,
		    "REGION_SERIALIZE",     node->REGION_SERIALIZE,
		    "TIME_TYPE",            node->TIME_TYPE,
		    "BEGIN_TIME",           node->BEGIN_TIME,
		    "END_TIME",             node->END_TIME,
		    "CAMPAIGN_IMPRESSION",  node->CAMPAIGN_IMPRESSION,
		    "CAMPAIGN_CLICK",       node->CAMPAIGN_CLICK,
		    "CAMPAIGN_CTR",         node->CAMPAIGN_CTR,
		    "CAMPAIGN_CPC",         node->CAMPAIGN_CPC,
		    "CAMPAIGN_COST",        node->CAMPAIGN_COST,
		    "CAMPAIGN_CPM",         node->CAMPAIGN_CPM
		    );

	    strcpy(comma, ",{");
	}

	page_index++;
    }

    ad_system_packet_send_json_end(sockfd);

    ad_system_sql_free(&sql_conn);
    ad_system_merger_free(&merger_head);
}

//推广->获取计划(单个)
void ad_system_packet_get_campaign_unique(int sockfd, ad_system_session_t *session, ad_system_packet_json_data_t *json, char *buffer)
{

    ad_system_sql_t sql_conn;
    if (ad_system_sql_get_campaign_unique(&sql_conn, json->CAMPAIGN_ID))
	return ad_system_packet_send_code(sockfd, session, NULL, PACKET_CODE_ERR);

    if (!ad_system_sql_fetch_next(&sql_conn))
	return ad_system_packet_send_code(sockfd, session, NULL, PACKET_CODE_ERR);

    ad_system_merger_campaign_t temp;
    ad_system_merger_campaign_t *node = &temp;
    memset(&temp, 0, sizeof(ad_system_merger_campaign_t));
    sprintf(temp.CAMPAIGN_ID, "%s",  ad_system_sql_get_string(&sql_conn, NULL, 1));
    sprintf(temp.CAMPAIGN_NAME, "%s", ad_system_sql_get_string(&sql_conn, NULL, 2));
    int CAMPAIGN_STATE = ad_system_sql_get_number(&sql_conn, NULL, 3);
    int PAUSE = ad_system_sql_get_number(&sql_conn, NULL, 4);
    temp.CAMPAIGN_BUDGET = ad_system_sql_get_double(&sql_conn, NULL, 5);
    temp.SCHEDULE_TYPE = ad_system_sql_get_number(&sql_conn, NULL, 6);
    sprintf(temp.SCHEDULE_SERIALIZE, "%s", ad_system_sql_get_string(&sql_conn, NULL, 7));
    const char *SCHEDULE_SYS_TIME = ad_system_sql_get_string(&sql_conn, NULL, 8);
    temp.REGION_TYPE = ad_system_sql_get_number(&sql_conn, NULL, 9);
    sprintf(temp.REGION_SERIALIZE, "%s", ad_system_sql_get_string(&sql_conn, NULL, 10));
    sprintf(temp.BEGIN_TIME, "%s", ad_system_sql_get_string(&sql_conn, NULL, 11));
    sprintf(temp.END_TIME, "%s", ad_system_sql_get_string(&sql_conn, NULL, 12));
    int DATE_SUB_BEGIN_TIME = ad_system_sql_get_number(&sql_conn, NULL, 13);
    int DATE_SUB_END_TIME = ad_system_sql_get_number(&sql_conn, NULL, 14);

    if (PAUSE == 2) {
	temp.CAMPAIGN_STATE = 5;//暂停

    } else if (DATE_SUB_BEGIN_TIME == 2) {
	temp.CAMPAIGN_STATE = 4;//未开始

    } else if (DATE_SUB_END_TIME == 2) {
	temp.CAMPAIGN_STATE = 6;//下线

    } else if (CAMPAIGN_STATE == 13) {
	temp.CAMPAIGN_STATE = 3;//预算不足

    } else if (temp.SCHEDULE_TYPE == 2 
	    && ad_system_merger_schedule(SCHEDULE_SYS_TIME, temp.SCHEDULE_SERIALIZE) == 0) {
	temp.CAMPAIGN_STATE = 2;//暂停时段

    } else {
	temp.CAMPAIGN_STATE = 1;//有效
    }

    //结束时间类型
    if (*temp.END_TIME) temp.TIME_TYPE = 2;
    else temp.TIME_TYPE = 1;

    //发送数据
    ad_system_packet_send_json_r(sockfd, buffer, SEND_JSON_DATA_ONLY, "{", "}", 12,

	    "%s", "CODE", "0",
	    "%s", "CAMPAIGN_ID",          node->CAMPAIGN_ID,
	    "%s", "CAMPAIGN_NAME",        node->CAMPAIGN_NAME,
	    "%d", "CAMPAIGN_STATUS",      node->CAMPAIGN_STATE,
	    "%f", "CAMPAIGN_BUDGET",      node->CAMPAIGN_BUDGET,
	    "%d", "SCHEDULE_TYPE",        node->SCHEDULE_TYPE,
	    "%s", "SCHEDULE_SERIALIZE",   node->SCHEDULE_SERIALIZE,
	    "%d", "REGION_TYPE",          node->REGION_TYPE,
	    "%s", "REGION_SERIALIZE",     node->REGION_SERIALIZE,
	    "%d", "TIME_TYPE",            node->TIME_TYPE,
	    "%s", "BEGIN_TIME",           node->BEGIN_TIME,
	    "%s", "END_TIME",             node->END_TIME);

#if 0
    "%d",		    "CAMPAIGN_IMPRESSION",  node->CAMPAIGN_IMPRESSION,
	"%d",		    "CAMPAIGN_CLICK",       node->CAMPAIGN_CLICK,
	"%f",		    "CAMPAIGN_CTR",         node->CAMPAIGN_CTR,
	"%f",		    "CAMPAIGN_CPC",         node->CAMPAIGN_CPC,
	"%f",		    "CAMPAIGN_COST",        node->CAMPAIGN_COST,
	"%f",		    "CAMPAIGN_CPM",         node->CAMPAIGN_CPM);
#endif

    ad_system_sql_free(&sql_conn);
}

void ad_system_packet_get_group_keyword(int sockfd, ad_system_session_t *session, ad_system_packet_json_data_t *json, char *buffer)
{

    if (!(json->DISPLAY_STATUS == 1 || json->DISPLAY_STATUS == 2))
	return ad_system_packet_send_code(sockfd, session, NULL, PACKET_CODE_FORMAT_ERR);

    if (!json->page_index_s || !json->page_index_e)
	return ad_system_packet_send_code(sockfd, session, NULL, PACKET_CODE_FORMAT_ERR);

    ad_system_sql_t sql_count_conn;
    if (ad_system_sql_get_group_keyword_count(&sql_count_conn, session->account_id, 
		json->DISPLAY_STATUS,
		json->CAMPAIGN_ID,
		json->GROUP_ID,
		json->FIND_TYPE,
		json->FIND_NAME_TYPE,
		json->FIND_NAME))
	return ad_system_packet_send_code(sockfd, session, NULL, PACKET_CODE_ERR);

    if (!ad_system_sql_fetch_next(&sql_count_conn))
	return ad_system_packet_send_code(sockfd, session, NULL, PACKET_CODE_FORMAT_ERR);

    size_t size = sprintf(buffer, "%s{\"CODE\":\"0\",\"INFO\":{\"PAGE_INDEX\":\"%d\",\"COUNT\":\"%s\"},\"LIST\":[",
	    HTTP_HEAD,
	    json->PAGE_INDEX, 
	    ad_system_sql_get_string(&sql_count_conn, NULL, 1));
    write(sockfd, buffer, size);
    ad_system_sql_free(&sql_count_conn);

    ad_system_sql_t sql_conn;
    if (ad_system_sql_get_group_keyword(&sql_conn, session->account_id,
		json->page_index_s,
		json->page_index_e,
		json->DISPLAY_STATUS,
		json->CAMPAIGN_ID,
		json->GROUP_ID,
		json->FIND_TYPE,
		json->FIND_NAME_TYPE,
		json->FIND_NAME) == 0) {

	char comma[4] = {0};
	while(ad_system_sql_fetch_next(&sql_conn)) {

	    size = sprintf(buffer, "%s{"
		    "\"GROUP_KEYWORD_ID\":\"%s\","
		    "\"GROUP_KEYWORD\":\"%s\","
		    "\"CAMPAIGN_ID\":\"%s\","
		    "\"CAMPAIGN_NAME\":\"%s\","
		    "\"GROUP_ID\":\"%s\","
		    "\"GROUP_NAME\":\"%s\","
		    "\"GROUP_CPC_PRICE\":\"%0.2f\","
		    "\"GROUP_CMP_PRICE\":\"%0.2f\"}",
		    comma,
		    ad_system_sql_get_string(&sql_conn, NULL, 2),
		    ad_system_sql_get_string(&sql_conn, NULL, 3),
		    ad_system_sql_get_string(&sql_conn, NULL, 4),
		    ad_system_sql_get_string(&sql_conn, NULL, 5),
		    ad_system_sql_get_string(&sql_conn, NULL, 6),
		    ad_system_sql_get_string(&sql_conn, NULL, 7),
		    ad_system_sql_get_double(&sql_conn, NULL, 8),
		    ad_system_sql_get_double(&sql_conn, NULL, 9));

	    comma[0] = ',';
	    write(sockfd, buffer, size);
	}
    }

    size = sprintf(buffer, "]}");
    write(sockfd, buffer, size);
    close(sockfd);

    ad_system_sql_free(&sql_conn);
}

void ad_system_packet_del_group_keyword(int sockfd, ad_system_session_t *session, ad_system_packet_json_data_t *json, char *buffer)
{

    if (!json->GROUP_KEYWORD_ID)
	return ad_system_packet_send_code(sockfd, session, NULL, PACKET_CODE_FORMAT_ERR);

    strcpy(buffer, json->GROUP_KEYWORD_ID);

    char *saveptr = NULL;
    char *keyword_id = NULL;
    while((keyword_id = strtok_r(buffer, ",", &saveptr))) {

	ad_system_sql_group_synch_state(session->account_id, NULL, keyword_id);
	ad_system_sql_del_group_keyword(keyword_id);

	buffer = NULL;
    }

    //    if (ad_system_sql_del_group_keyword(json->GROUP_KEYWORD_ID))
    //	return ad_system_packet_send_code(sockfd, session, NULL, PACKET_CODE_ERR);

    //if (json->GROUP_ID && *json->GROUP_ID)
    //	ad_system_sql_group_synch_state(session->account_id, json->GROUP_ID, NULL);
    return ad_system_packet_send_code(sockfd, session, NULL, PACKET_CODE_OK);
}

void ad_system_packet_add_group_keyword(int sockfd, ad_system_session_t *session, ad_system_packet_json_data_t *json, char *buffer)
{

    if (!json->GROUP_ID || !json->GROUP_KEYWORD)
	return ad_system_packet_send_code(sockfd, session, NULL, PACKET_CODE_FORMAT_ERR);

    strcpy(buffer, json->GROUP_KEYWORD);

    char *keyword = NULL;
    char *saveptr = NULL;
    while((keyword= strtok_r(buffer, ",", &saveptr))) {

	ad_system_sql_add_group_keyword(session->account_id, json->GROUP_ID, keyword);
	buffer = NULL;
    }

    ad_system_sql_group_synch_state(session->account_id, json->GROUP_ID, NULL);
    return ad_system_packet_send_code(sockfd, session, NULL, PACKET_CODE_OK);
}

void ad_system_packet_get_group(int sockfd, ad_system_session_t *session, ad_system_packet_json_data_t *json, char *buffer)
{

    ad_system_sql_t sql_conn;
    if (ad_system_sql_get_group(&sql_conn,
		session->account_id, 
		json->DISPLAY_STATUS,
		json->FIND_TYPE,
		json->CAMPAIGN_ID,
		json->FIND_NAME_TYPE,
		json->FIND_NAME))
	return ad_system_packet_send_code(sockfd, session, NULL, PACKET_CODE_ERR);

    lg_list_t mongo_head;
    if (json->DATE_TYPE == 1)
	ad_system_mongo_stat_list_load(&mongo_head, GROUP_DAY_TEMP, json->DISPLAY_STATUS, session->account_id, "group_id");
    else if (json->DATE_TYPE == 2)
	ad_system_mongo_stat_list_load(&mongo_head, GROUP_YESTERDAY_TEMP, json->DISPLAY_STATUS, session->account_id, "group_id");
    else if (json->DATE_TYPE == 3)
	ad_system_mongo_stat_list_load(&mongo_head, GROUP_SEVENDAY_TEMP, json->DISPLAY_STATUS, session->account_id, "group_id");
    else
	return ad_system_packet_send_code(sockfd, session, NULL, PACKET_CODE_FORMAT_ERR);

    lg_list_t merger_head;
    ad_system_merger_group_info_t info;
    ad_system_merger_group(
	    &sql_conn,
	    &mongo_head,
	    json,
	    &merger_head,
	    &info);

    ad_system_packet_send_http_head(sockfd);
    size_t size = sprintf(buffer, "{\"CODE\":\"0\","
	    "\"INFO\":{"
	    "\"PAGE_INDEX\":\"%u\","
	    "\"GROUP_COUNT\":\"%u\","
	    "\"GROUP_ENABLED\":\"%u\","
	    "\"GROUP_IMPRESSION\":\"%u\","
	    "\"GROUP_CLICK\":\"%u\","
	    "\"GROUP_CTR\":\"%0.2f%%\","
	    "\"GROUP_CPC\":\"%0.2f\","
	    "\"GROUP_COST\":\"%0.2f\","
	    "\"GROUP_CPM\":\"%0.2f\""
	    "},\"LIST\":[",

	    json->PAGE_INDEX,
	    info.GROUP_COUNT,
	    info.GROUP_ENABLED,
	    info.GROUP_IMPRESSION,
	    info.GROUP_CLICK,
	    info.GROUP_CTR * 100,
	    info.GROUP_CPC,
	    info.GROUP_COST,
	    info.GROUP_CPM);
    send(sockfd, buffer, size, 0);

    int page_index = 1;
    char comma[4] = "{"; 
    lg_list_t *p = NULL;
    lg_list_for_each(p, &merger_head) {

	ad_system_merger_group_t *node = NULL;
	node = (ad_system_merger_group_t *)p; 

	if (page_index >= json->page_index_s && page_index <= json->page_index_e) {

	    ad_system_packet_send_json_r(sockfd, buffer, SEND_JSON_DATA_NODE, comma, "}", 15,
		    "%s", "CAMPAIGN_ID",                node->CAMPAIGN_ID,
		    "%s", "CAMPAIGN_NAME",              node->CAMPAIGN_NAME,
		    "%s", "GROUP_ID",                   node->GROUP_ID,
		    "%s", "GROUP_NAME",                 node->GROUP_NAME,
		    "%u", "GROUP_STATUS",               node->GROUP_STATE,
		    "%f", "GROUP_CPC_PRICE",            node->GROUP_CPC_PRICE,
		    "%f", "GROUP_CPM_PRICE",            node->GROUP_CPM_PRICE,
		    "%u", "GROUP_TARGETING_TYPE",       node->GROUP_TARGETING_TYPE,
		    "%s", "GROUP_DISEASE",              node->GROUP_DISEASE,
		    "%u", "GROUP_IMPRESSION",           node->GROUP_IMPRESSION,
		    "%u", "GROUP_CLICK",                node->GROUP_CLICK,
		    "%e", "GROUP_CTR",                  node->GROUP_CTR,
		    "%f", "GROUP_CPC",                  node->GROUP_CPC,
		    "%f", "GROUP_COST",                 node->GROUP_COST,
		    "%f", "GROUP_CPM",                  node->GROUP_CPM);

	    strcpy(comma, ",{");
	}

	page_index++;
    }

    ad_system_packet_send_json_end(sockfd);

    ad_system_sql_free(&sql_conn);
    ad_system_merger_free(&merger_head);
}

//推广->获取文字创意
void ad_system_packet_get_creative_text(int sockfd, ad_system_session_t *session, ad_system_packet_json_data_t *json, char *buffer)
{

    ad_system_sql_t sql_conn;
    if (json->FIND_STATUS == 6) {

	if (ad_system_sql_get_creative_del_text(&sql_conn,
		    session->account_id, 
		    json->FIND_TYPE, 
		    json->CAMPAIGN_ID,
		    json->GROUP_ID,
		    json->FIND_NAME_TYPE,
		    json->FIND_NAME))
	    return ad_system_packet_send_code(sockfd, session, NULL, PACKET_CODE_ERR);

    } else {

	if (ad_system_sql_get_creative_text(&sql_conn, 
		    session->account_id, 
		    json->FIND_TYPE, 
		    json->CAMPAIGN_ID,
		    json->GROUP_ID,
		    json->FIND_NAME_TYPE,
		    json->FIND_NAME))
	    return ad_system_packet_send_code(sockfd, session, NULL, PACKET_CODE_ERR);
    }

    //获取mongo数据
    lg_list_t mongo_head;
    if (json->DATE_TYPE == 1)
	ad_system_mongo_stat_list_load(&mongo_head, CREATIVE_DAY_TEMP, 2, session->account_id, "creative_id");
    else if (json->DATE_TYPE == 2)
	ad_system_mongo_stat_list_load(&mongo_head, CREATIVE_YESTERDAY_TEMP, 2, session->account_id, "creative_id");
    else if (json->DATE_TYPE == 3)
	ad_system_mongo_stat_list_load(&mongo_head, CREATIVE_SEVENDAY_TEMP, 2, session->account_id, "creative_id");
    else
	return ad_system_packet_send_code(sockfd, session, NULL, PACKET_CODE_FORMAT_ERR);

    //合并数据
    lg_list_t merger_head;
    ad_system_merger_creative_info_t info;
    ad_system_merger_creative_text(
	    &sql_conn, 
	    &mongo_head, 
	    json,
	    &merger_head,
	    &info);

    ad_system_packet_send_http_head(sockfd);
    size_t size = sprintf(buffer, "{\"CODE\":\"0\","
	    "\"INFO\":{"
	    "\"PAGE_INDEX\":\"%u\","
	    "\"CREATIVE_COUNT\":\"%u\","
	    "\"CREATIVE_ENABLED\":\"%u\","
	    "\"CREATIVE_IMPRESSION\":\"%u\","
	    "\"CREATIVE_CLICK\":\"%u\","
	    "\"CREATIVE_CTR\":\"%0.2f%%\","
	    "\"CREATIVE_CPC\":\"%0.2f\","
	    "\"CREATIVE_COST\":\"%0.2f\","
	    "\"CREATIVE_CPM\":\"%0.2f\""
	    "},\"LIST\":[", 

	    json->PAGE_INDEX,
	    info.CREATIVE_COUNT,
	    info.CREATIVE_ENABLED,
	    info.CREATIVE_IMPRESSION,
	    info.CREATIVE_CLICK,
	    info.CREATIVE_CTR * 100,
	    info.CREATIVE_CPC,
	    info.CREATIVE_COST,
	    info.CREATIVE_CPM);
    send(sockfd, buffer, size, 0);

    int page_index = 1;
    char comma[4] = "{";
    lg_list_t *p = NULL;
    lg_list_for_each(p, &merger_head) {

	ad_system_merger_creative_text_t *node = NULL;
	node = (ad_system_merger_creative_text_t *)p;

	if (page_index >= json->page_index_s && page_index <= json->page_index_e) {

	    ad_system_packet_send_json_r(sockfd, buffer, SEND_JSON_DATA_NODE, comma, "}", 21,
		    "%s", "CAMPAIGN_ID",              node->CAMPAIGN_ID,
		    "%s", "CAMPAIGN_NAME",            node->CAMPAIGN_NAME,
		    "%s", "GROUP_ID",                 node->GROUP_ID,
		    "%s", "GROUP_NAME",               node->GROUP_NAME,
		    "%s", "CREATIVE_ID",              node->CREATIVE_ID,
		    "%s", "CREATIVE_NAME",            node->CREATIVE_NAME,
		    "%u", "CREATIVE_STATUS",          node->CREATIVE_STATE,
		    "%s", "CREATIVE_REFUSE_REASON",   node->CREATIVE_REFUSE_REASON,
		    "%s", "CREATIVE_DESCRIPTION",     node->CREATIVE_DESCRIPTION,
		    "%s", "CREATIVE_DESTINATION_URL", node->CREATIVE_DESTINATION_URL,
		    "%s", "CREATIVE_DISPLAY_URL",     node->CREATIVE_DISPLAY_URL,
		    "%u", "CREATIVE_APPEND_TYPE",     node->CREATIVE_APPEND_TYPE,
		    "%s", "CREATIVE_APPEND_ID",       node->CREATIVE_APPEND_ID,
		    "%u", "CREATIVE_IMPRESSION",      node->CREATIVE_IMPRESSION,
		    "%u", "CREATIVE_CLICK",           node->CREATIVE_CLICK,
		    "%e", "CREATIVE_CTR",             node->CREATIVE_CTR,
		    "%f", "CREATIVE_CPC",             node->CREATIVE_CPC,
		    "%f", "CREATIVE_COST",            node->CREATIVE_COST,
		    "%f", "CREATIVE_CPM",             node->CREATIVE_CPM,
		    "%s", "MOBILE_DESTINATION_URL",   node->MOBILE_DESTINATION_URL,
		    "%s", "MOBILE_DISPLAY_URL",       node->MOBILE_DISPLAY_URL
			);

	    strcpy(comma, ",{");
	}

	page_index++;
    }

    ad_system_packet_send_json_end(sockfd);
    ad_system_sql_free(&sql_conn);
    ad_system_merger_free(&merger_head);
}

void ad_system_packet_get_img_creative(int sockfd, ad_system_session_t *session, ad_system_packet_json_data_t *json, char *buffer)
{

    ad_system_sql_t sql_conn;
    if (json->FIND_STATUS == 6) {

	if (ad_system_sql_get_creative_del_img(&sql_conn,
		    session->account_id, 
		    json->FIND_TYPE,
		    json->CAMPAIGN_ID,
		    json->GROUP_ID,
		    json->FIND_NAME_TYPE,
		    json->FIND_NAME))
	    return ad_system_packet_send_code(sockfd, session, NULL, PACKET_CODE_ERR);

    } else {

	if (ad_system_sql_get_img_creative(&sql_conn,
		    session->account_id, 
		    json->FIND_TYPE,
		    json->CAMPAIGN_ID,
		    json->GROUP_ID,
		    json->FIND_NAME_TYPE,
		    json->FIND_NAME))
	    return ad_system_packet_send_code(sockfd, session, NULL, PACKET_CODE_ERR);
    }

    //获取mongo数据
    lg_list_t mongo_head;
    if (json->DATE_TYPE == 1)
	ad_system_mongo_stat_list_load(&mongo_head, CREATIVE_DAY_TEMP, 1, session->account_id, "creative_id");
    else if (json->DATE_TYPE == 2)
	ad_system_mongo_stat_list_load(&mongo_head, CREATIVE_YESTERDAY_TEMP, 1, session->account_id, "creative_id");
    else if (json->DATE_TYPE == 3)
	ad_system_mongo_stat_list_load(&mongo_head, CREATIVE_SEVENDAY_TEMP, 1, session->account_id, "creative_id");
    else
	return ad_system_packet_send_code(sockfd, session, NULL, PACKET_CODE_FORMAT_ERR);

    //合并数据
    lg_list_t merger_head;
    ad_system_merger_creative_info_t info;
    ad_system_merger_creative_img(
	    &sql_conn,
	    &mongo_head, 
	    json,
	    &merger_head,
	    &info);

    //发送数据
    size_t size = sprintf(buffer, HTTP_HEAD);
    send(sockfd, buffer, size, 0);

    size = sprintf(buffer, "{\"CODE\":\"0\","
	    "\"INFO\":{"
	    "\"PAGE_INDEX\":\"%u\","
	    "\"CREATIVE_COUNT\":\"%u\","
	    "\"CREATIVE_ENABLED\":\"%u\","
	    "\"CREATIVE_IMPRESSION\":\"%u\","
	    "\"CREATIVE_CLICK\":\"%u\","
	    "\"CREATIVE_CTR\":\"%0.2f%%\","
	    "\"CREATIVE_CPC\":\"%0.2f\","
	    "\"CREATIVE_COST\":\"%0.2f\","
	    "\"CREATIVE_CPM\":\"%0.2f\""
	    "},\"LIST\":[", 
	    json->PAGE_INDEX,
	    info.CREATIVE_COUNT,
	    info.CREATIVE_ENABLED,
	    info.CREATIVE_IMPRESSION,
	    info.CREATIVE_CLICK,
	    info.CREATIVE_CTR * 100,
	    info.CREATIVE_CPC,
	    info.CREATIVE_COST,
	    info.CREATIVE_CPM);
    send(sockfd, buffer, size, 0);

    int page_index = 1;
    char  comma[4] = "{";
    lg_list_t *p = NULL;
    lg_list_for_each(p, &merger_head) {

	ad_system_merger_creative_img_t *node = NULL;
	node = (ad_system_merger_creative_img_t*)p;

	if (page_index >= json->page_index_s && page_index <= json->page_index_e) {

	    ad_system_packet_send_json_r(sockfd, buffer, SEND_JSON_DATA_NODE, comma, "}", 20,
		    "%s", "CAMPAIGN_ID",              	    node->CAMPAIGN_ID,
		    "%s", "CAMPAIGN_ID",		    node->CAMPAIGN_ID,                  
		    "%s", "CAMPAIGN_NAME",                  node->CAMPAIGN_NAME, 
		    "%s", "GROUP_ID",                       node->GROUP_ID, 
		    "%s", "GROUP_NAME",                     node->GROUP_NAME, 
		    "%s", "CREATIVE_ID",                    node->CREATIVE_ID, 
		    "%s", "CREATIVE_NAME",                  node->CREATIVE_NAME, 
		    "%u", "CREATIVE_STATUS",                node->CREATIVE_STATE, 
		    "%s", "CREATIVE_REFUSE_REASON",         node->CREATIVE_REFUSE_REASON, 
		    "%s", "CREATIVE_IMG_SIZE",              node->CREATIVE_IMG_SIZE, 
		    "%s", "CREATIVE_IMG_SIZE_IMGLENGTH",    node->CREATIVE_IMG_SIZE_IMGLENGTH, 
		    "%s", "CREATIVE_FILE_PATH",             node->CREATIVE_FILE_PATH, 
		    "%s", "CREATIVE_DESTINATION_URL",       node->CREATIVE_DESTINATION_URL, 
		    "%s", "CREATIVE_DISPLAY_URL",           node->CREATIVE_DISPLAY_URL, 
		    "%u", "CREATIVE_IMPRESSION",            node->CREATIVE_IMPRESSION, 
		    "%u", "CREATIVE_CLICK",                 node->CREATIVE_CLICK, 
		    "%e", "CREATIVE_CTR",                   node->CREATIVE_CTR, 
		    "%f", "CREATIVE_CPC",                   node->CREATIVE_CPC, 
		    "%f", "CREATIVE_COST",                  node->CREATIVE_COST, 
		    "%f", "CREATIVE_CPM",                   node->CREATIVE_CPM
			);

	    strcpy(comma, ",{");
	}

	page_index++;
    }

    ad_system_packet_send_json_end(sockfd);
    ad_system_sql_free(&sql_conn);
    ad_system_merger_free(&merger_head);
}

void ad_system_packet_set_creative_append(int sockfd, ad_system_session_t *session, ad_system_packet_json_data_t *json, char *buffer)
{

    if (ad_system_sql_set_creative_append(
		session->account_id,
		json->CREATIVE_APPEND_ID,
		json->UPDATE_TYPE,
		json->APPEND_STATUS,
		json->APPEND_CONTENT))
	return ad_system_packet_send_code(sockfd, session, NULL, PACKET_CODE_ERR);

    return ad_system_packet_send_code(sockfd, session, NULL, PACKET_CODE_OK);
}

void ad_system_packet_add_creative_append(int sockfd, ad_system_session_t *session, ad_system_packet_json_data_t *json, char *buffer)
{

    if (ad_system_sql_add_creative_append(session->account_id, json->APPEND_CONTENT))
	return ad_system_packet_send_code(sockfd, session, NULL, PACKET_CODE_ERR);

    return ad_system_packet_send_code(sockfd, session, NULL, PACKET_CODE_OK);
}

void ad_system_packet_get_creative_append(int sockfd, ad_system_session_t *session, ad_system_packet_json_data_t *json, char *buffer)
{

    ad_system_sql_t sql_conn;
    if (ad_system_sql_get_creative_append(&sql_conn,
		session->account_id, 
		json->FIND_TYPE,
		json->SORT_TYPE,
		json->SORT_COLUMN,
		json->FIND_NAME,
		json->FIND_STATUS))
	return ad_system_packet_send_code(sockfd, session, NULL, PACKET_CODE_ERR);

    ad_system_packet_send_http_head(sockfd);
    size_t size = sprintf(buffer, "{\"CODE\":\"0\",\"LIST\":[");
    send(sockfd, buffer, size, 0);

    char comma[4] = "{";
    while(ad_system_sql_fetch_next(&sql_conn)) {

	ad_system_packet_send_json(sockfd, buffer, SEND_JSON_DATA_NODE, comma, "}", "1%s2%s3%s4%s",
		"APPEND_ID",		ad_system_sql_get_string(&sql_conn, NULL, 1),
		"APPEND_CONTENT",       ad_system_sql_get_string(&sql_conn, NULL, 3),
		"APPEND_STATUS",        ad_system_sql_get_string(&sql_conn, NULL, 2),
		"APPEND_REFUSE_REASON", ad_system_sql_get_string(&sql_conn, NULL, 4));

	strcpy(comma, ",{");
    }

    ad_system_packet_send_json_end(sockfd);
    ad_system_sql_free(&sql_conn);

}

void ad_system_packet_add_creative_img(int sockfd, ad_system_session_t *session, ad_system_packet_json_data_t *json, char *buffer)
{

    ad_system_sql_t sql_conn;
    if (ad_system_sql_connect(&sql_conn))
	return ad_system_packet_send_code(sockfd, session, NULL, PACKET_CODE_ERR);

    ad_system_sql_t sql_campaign_id;
    if (ad_system_sql_get_campaign_group_from_group_or_creative_img(&sql_campaign_id, json->GROUP_ID, NULL))
	return ad_system_packet_send_code(sockfd, session, &sql_conn, PACKET_CODE_ERR);

    char *img_id = NULL;
    char *saveptr = NULL;
    char *img_array = buffer;
    strcpy(buffer, json->CREATIVE_IMG_ID);

    for (;;) {

	if (!(img_id = strtok_r(img_array, ",", &saveptr)))
	    break;

	ad_system_image_t *img = NULL;
	if (!(img = ad_system_image_list_find(img_id))) {

	    ad_system_sql_rollback(&sql_conn);
	    ad_system_sql_free(&sql_conn);
	    ad_system_sql_free(&sql_campaign_id);
	    return ad_system_packet_send_code(sockfd, session, NULL, PACKET_CODE_IMG_ID_ERR);
	}

	if (ad_system_sql_add_creative_img(&sql_conn,
		    session->account_id,
		    ad_system_sql_get_string(&sql_campaign_id, NULL, 1),
		    ad_system_sql_get_string(&sql_campaign_id, NULL, 2),
		    img->name,
		    img->path,
		    img->width,
		    img->height,
		    img->length,
		    json->CREATIVE_DESTINATION_URL,
		    json->CREATIVE_DISPLAY_URL)) {

	    ad_system_sql_rollback(&sql_conn);
	    ad_system_sql_free(&sql_conn);
	    ad_system_sql_free(&sql_campaign_id);
	    return ad_system_packet_send_code(sockfd, session, NULL, PACKET_CODE_IMG_ID_ERR);
	}

	img_array = NULL;
    }

    ad_system_sql_commit(&sql_conn);
    ad_system_sql_free(&sql_conn);
    ad_system_sql_free(&sql_campaign_id);
    ad_system_packet_send_code(sockfd, session, NULL, PACKET_CODE_OK);
}

void ad_system_packet_report_standard(int sockfd, ad_system_session_t *session, ad_system_packet_json_data_t *json, char *buffer)
{

    //text or img
    int display_style = 0;
    if (json->REPORT_SPECIES & 1) display_style = 2;
    else display_style = 1;

    char key[128]={0}, down_path[256]={0}, file_path[256]={0};
    ad_system_report_path_create(key, down_path, file_path);
    sprintf(buffer, "%s->%s", json->TIME_START, json->TIME_END);

    if (ad_system_mongo_report_list_add(session->account_id, 
		key,
		json->REPORT_NAME,
		json->REPORT_TYPE,
		display_style,
		buffer, 
		down_path))
	return ad_system_packet_send_code(sockfd, session, NULL, PACKET_CODE_ERR);

    //返回,等待list状态改变
    ad_system_packet_send_code(sockfd, session, NULL, PACKET_CODE_OK);

    if (ad_system_report_standard_create(
		display_style,
		session->account_id,
		json->TIME_START, 
		json->TIME_END,
		json->TIME_TYPE,
		json->REPORT_TYPE,
		file_path) == 0) {
	//创建成功
	ad_system_mongo_report_list_set(session->account_id, key, 2);

    } else {

	ad_system_mongo_report_list_set(session->account_id, key, 3);
    }
}

void ad_system_packet_report_complex(int sockfd, ad_system_session_t *session, ad_system_packet_json_data_t *json, char *buffer)
{

    //text or img
    int display_style = 0;
    if (json->REPORT_SPECIES & 1) display_style = 2;
    else display_style = 1;

    char key[128]={0}, down_path[256]={0}, file_path[256]={0};
    ad_system_report_path_create(key, down_path, file_path);
    sprintf(buffer, "%s->%s", json->TIME_START, json->TIME_END);

    if (ad_system_mongo_report_list_add(session->account_id, 
		key,
		json->REPORT_NAME,
		json->REPORT_TYPE + 4,
		display_style,
		buffer, 
		down_path))
	return ad_system_packet_send_code(sockfd, session, NULL, PACKET_CODE_ERR);

    //返回,等待list状态改变
    ad_system_packet_send_code(sockfd, session, NULL, PACKET_CODE_OK);

    if (ad_system_report_complex_create(
		display_style,
		session->account_id, 
		json->TIME_START, 
		json->TIME_END,
		json->TIME_TYPE,
		json->REPORT_TYPE,
		file_path) == 0) {

	//创建成功
	ad_system_mongo_report_list_set(session->account_id, key, 2);

    } else {

	ad_system_mongo_report_list_set(session->account_id, key, 3);
    }
}

void ad_system_packet_get_report_list(int sockfd, ad_system_session_t *session, ad_system_packet_json_data_t *json, char *buffer)
{

    lg_list_t head;
    if (ad_system_mongo_report_list_find(session->account_id, json->REPORT_BELONG, &head) != 0)
	return ad_system_packet_send_code(sockfd, session, NULL, PACKET_CODE_ERR);

    ad_system_packet_send_http_head(sockfd);
    size_t size = sprintf(buffer, "{\"CODE\":\"0\",\"LIST\":[");
    send(sockfd, buffer, size, 0);

    char comma[4] = "";
    lg_list_t *p = NULL;
    lg_list_for_tail(p, &head) {

	ad_system_mongo_report_list_t *node = NULL;
	node = (ad_system_mongo_report_list_t *)p;

	size = sprintf(buffer, "%s{"
		"\"REPORT_ID\":\"%s\","
		"\"REPORT_NAME\":\"%s\","
		"\"REPORT_TYPE\":\"%s\","
		"\"RANGE_TIME\":\"%s\","
		"\"CREATE_TIME\":\"%s\","
		"\"STATUS\":\"%s\","
		"\"PATH\":\"%s\"}",
		comma,
		node->report_id,
		node->report_name,
		node->report_type,
		node->range_time,
		node->create_time,
		node->status,
		node->path);

	send(sockfd, buffer, size, 0);
	comma[0] = ',';
    }

    size = sprintf(buffer, "]}");
    send(sockfd, buffer, size, 0);
    close(sockfd);

    ad_system_mongo_list_free(&head);
}

void ad_system_packet_del_report(int sockfd, ad_system_session_t *session, ad_system_packet_json_data_t *json, char *buffer)
{

    if (ad_system_mongo_report_list_del(session->account_id, json->REPORT_ID) != 0)
	ad_system_packet_send_code(sockfd, session, NULL, PACKET_CODE_ERR);

    ad_system_packet_send_code(sockfd, session, NULL, PACKET_CODE_OK);
}

int ad_system_packet_json_value_to_number(json_object *json_value)
{

    const char *str = NULL;
    if ((str = json_object_get_string(json_value)))
	return atoi(str);

    return 0;
}

char *ad_system_packet_json_value_to_string(char *buffer, json_object *json_value, int mode)
{

    const char *str = NULL;
    if (!(str = json_object_get_string(json_value)))
	return NULL;

    char *val = buffer;
    for (; *str; str++) {

	if (mode && *str == '%')
	    *val++ = '\\';

	if (*str == '\'')
	    *val++ = '\'';

	*val++ = *str;
    }

    *val = 0;
    return buffer;
}

int ad_system_packet_json_value_parse(json_object *json_root, ad_system_packet_json_data_t *json)
{

    memset(json, 0, sizeof(ad_system_packet_json_data_t));

    json_object *PASS_OLD = json_object_object_get(json_root, "PASS_OLD");
    if (PASS_OLD) json->PASS_OLD = json_object_get_string(PASS_OLD);

    json_object *PASS_NEW = json_object_object_get(json_root, "PASS_NEW");
    if (PASS_NEW) json->PASS_NEW = json_object_get_string(PASS_NEW);

    json_object *FIND_TYPE = json_object_object_get(json_root, "FIND_TYPE");
    if (FIND_TYPE) json->FIND_TYPE = ad_system_packet_json_value_to_number(FIND_TYPE);

    json_object *FIND_NAME = json_object_object_get(json_root, "FIND_NAME");
    if (FIND_NAME) ad_system_packet_json_value_to_string(json->FIND_NAME, FIND_NAME, 1);

    json_object *PAGE_COUNT = json_object_object_get(json_root, "PAGE_COUNT");
    if (PAGE_COUNT) json->PAGE_COUNT = ad_system_packet_json_value_to_number(PAGE_COUNT);

    json_object *PAGE_INDEX = json_object_object_get(json_root, "PAGE_INDEX");
    if (PAGE_INDEX) json->PAGE_INDEX = ad_system_packet_json_value_to_number(PAGE_INDEX);

    if (json->PAGE_COUNT && json->PAGE_INDEX)
	json->page_index_s = json->PAGE_COUNT * json->PAGE_INDEX - json->PAGE_COUNT + 1;

    if (json->PAGE_COUNT && json->PAGE_INDEX)
	json->page_index_e = json->page_index_s + json->PAGE_COUNT - 1;

    json_object *DATE_TYPE = json_object_object_get(json_root, "DATE_TYPE");
    if (DATE_TYPE) json->DATE_TYPE = ad_system_packet_json_value_to_number(DATE_TYPE);

    json_object *DISPLAY_STATUS = json_object_object_get(json_root, "DISPLAY_STATUS");
    if (DISPLAY_STATUS) json->DISPLAY_STATUS = ad_system_packet_json_value_to_number(DISPLAY_STATUS);

    json_object *SORT_COLUMN = json_object_object_get(json_root, "SORT_COLUMN");
    if (SORT_COLUMN) json->SORT_COLUMN = ad_system_packet_json_value_to_number(SORT_COLUMN);

    json_object *SORT_TYPE = json_object_object_get(json_root, "SORT_TYPE");
    if (SORT_TYPE) json->SORT_TYPE = ad_system_packet_json_value_to_number(SORT_TYPE);

    json_object *FIND_NAME_TYPE = json_object_object_get(json_root, "FIND_NAME_TYPE");
    if (FIND_NAME_TYPE) json->FIND_NAME_TYPE = ad_system_packet_json_value_to_number(FIND_NAME_TYPE);

    json_object *FIND_STATUS = json_object_object_get(json_root, "FIND_STATUS");
    if (FIND_STATUS) json->FIND_STATUS = ad_system_packet_json_value_to_number(FIND_STATUS);

    json_object *UPDATE_TYPE = json_object_object_get(json_root, "UPDATE_TYPE");
    if (UPDATE_TYPE) json->UPDATE_TYPE = ad_system_packet_json_value_to_number(UPDATE_TYPE);

    json_object *TIME_TYPE = json_object_object_get(json_root, "TIME_TYPE");
    if (TIME_TYPE) json->TIME_TYPE = ad_system_packet_json_value_to_number(TIME_TYPE);

    json_object *BEGIN_TIME = json_object_object_get(json_root, "BEGIN_TIME");
    if (BEGIN_TIME) json->BEGIN_TIME = json_object_get_string(BEGIN_TIME);

    json_object *END_TIME = json_object_object_get(json_root, "END_TIME");
    if (END_TIME) json->END_TIME = json_object_get_string(END_TIME);

    json_object *CAMPAIGN_ID = json_object_object_get(json_root, "CAMPAIGN_ID");
    if (CAMPAIGN_ID) json->CAMPAIGN_ID = json_object_get_string(CAMPAIGN_ID);

    json_object *CAMPAIGN_NAME = json_object_object_get(json_root, "CAMPAIGN_NAME");
    if (CAMPAIGN_NAME) ad_system_packet_json_value_to_string(json->CAMPAIGN_NAME, CAMPAIGN_NAME, 0);

    json_object *CAMPAIGN_BUDGET = json_object_object_get(json_root, "CAMPAIGN_BUDGET");
    if (CAMPAIGN_BUDGET) json->CAMPAIGN_BUDGET = json_object_get_string(CAMPAIGN_BUDGET);

    json_object *CAMPAIGN_STATUS = json_object_object_get(json_root, "CAMPAIGN_STATUS");
    if (CAMPAIGN_STATUS) json->CAMPAIGN_STATUS = ad_system_packet_json_value_to_number(CAMPAIGN_STATUS);

    json_object *REGION_TYPE = json_object_object_get(json_root, "REGION_TYPE");
    if (REGION_TYPE) json->REGION_TYPE = ad_system_packet_json_value_to_number(REGION_TYPE);

    json_object *REGION_SERIALIZE = json_object_object_get(json_root, "REGION_SERIALIZE");
    if (REGION_SERIALIZE) json->REGION_SERIALIZE = json_object_get_string(REGION_SERIALIZE);

    json_object *SCHEDULE_TYPE = json_object_object_get(json_root, "SCHEDULE_TYPE");
    if (SCHEDULE_TYPE) json->SCHEDULE_TYPE = ad_system_packet_json_value_to_number(SCHEDULE_TYPE);

    json_object *SCHEDULE_SERIALIZE = json_object_object_get(json_root, "SCHEDULE_SERIALIZE");
    if (SCHEDULE_SERIALIZE) json->SCHEDULE_SERIALIZE = json_object_get_string(SCHEDULE_SERIALIZE);

    json_object *GROUP_ID = json_object_object_get(json_root, "GROUP_ID");
    if (GROUP_ID) json->GROUP_ID = json_object_get_string(GROUP_ID);

    json_object *GROUP_NAME_TYPE = json_object_object_get(json_root, "GROUP_NAME_TYPE");
    if (GROUP_NAME_TYPE) json->GROUP_NAME_TYPE = ad_system_packet_json_value_to_number(GROUP_NAME_TYPE);

    json_object *GROUP_NAME = json_object_object_get(json_root, "GROUP_NAME");
    if (GROUP_NAME) ad_system_packet_json_value_to_string(json->GROUP_NAME, GROUP_NAME, 0);

    GET_JSON_VALUE_NUM(GROUP_PRICE_TYPE, "GROUP_PRICE_TYPE");

    json_object *GROUP_CPC_PRICE = json_object_object_get(json_root, "GROUP_CPC_PRICE");
    if (GROUP_CPC_PRICE) json->GROUP_CPC_PRICE = json_object_get_string(GROUP_CPC_PRICE);

    json_object *GROUP_CPM_PRICE = json_object_object_get(json_root, "GROUP_CPM_PRICE");
    if (GROUP_CPM_PRICE) json->GROUP_CPM_PRICE = json_object_get_string(GROUP_CPM_PRICE);

    json_object *GROUP_TARGETING_TYPE = json_object_object_get(json_root, "GROUP_TARGETING_TYPE");
    if (GROUP_TARGETING_TYPE) json->GROUP_TARGETING_TYPE = ad_system_packet_json_value_to_number(GROUP_TARGETING_TYPE);

    json_object *GROUP_DISEASE = json_object_object_get(json_root, "GROUP_DISEASE");
    if (GROUP_DISEASE) json->GROUP_DISEASE = json_object_get_string(GROUP_DISEASE);

    json_object *GROUP_KEYWORD_ID = json_object_object_get(json_root, "GROUP_KEYWORD_ID");
    if (GROUP_KEYWORD_ID) json->GROUP_KEYWORD_ID = json_object_get_string(GROUP_KEYWORD_ID);

    json_object *GROUP_KEYWORD = json_object_object_get(json_root, "GROUP_KEYWORD");
    if (GROUP_KEYWORD) json->GROUP_KEYWORD = json_object_get_string(GROUP_KEYWORD);

    json_object *CREATIVE_DESTINATION_URL = json_object_object_get(json_root, "CREATIVE_DESTINATION_URL");
    if (CREATIVE_DESTINATION_URL) json->CREATIVE_DESTINATION_URL = json_object_get_string(CREATIVE_DESTINATION_URL);

    json_object *CREATIVE_DISPLAY_URL = json_object_object_get(json_root, "CREATIVE_DISPLAY_URL");
    if (CREATIVE_DISPLAY_URL) json->CREATIVE_DISPLAY_URL = json_object_get_string(CREATIVE_DISPLAY_URL);

    json_object *CREATIVE_IMG_ID = json_object_object_get(json_root, "CREATIVE_IMG_ID");
    if (CREATIVE_IMG_ID) json->CREATIVE_IMG_ID = json_object_get_string(CREATIVE_IMG_ID);

    json_object *CREATIVE_ID = json_object_object_get(json_root, "CREATIVE_ID");
    if (CREATIVE_ID) json->CREATIVE_ID = json_object_get_string(CREATIVE_ID);

    json_object *CREATIVE_NAME = json_object_object_get(json_root, "CREATIVE_NAME");
    if (CREATIVE_NAME) ad_system_packet_json_value_to_string(json->CREATIVE_NAME, CREATIVE_NAME, 0);

    json_object *CREATIVE_DESCRIPTION = json_object_object_get(json_root, "CREATIVE_DESCRIPTION");
    if (CREATIVE_DESCRIPTION) ad_system_packet_json_value_to_string(json->CREATIVE_DESCRIPTION, CREATIVE_DESCRIPTION, 0);

    json_object *MOBILE_DESTINATION_URL = json_object_object_get(json_root, "MOBILE_DESTINATION_URL");
    if (MOBILE_DESTINATION_URL) json->MOBILE_DESTINATION_URL = json_object_get_string(MOBILE_DESTINATION_URL);

    json_object *MOBILE_DISPLAY_URL = json_object_object_get(json_root, "MOBILE_DISPLAY_URL");
    if (MOBILE_DISPLAY_URL) json->MOBILE_DISPLAY_URL = json_object_get_string(MOBILE_DISPLAY_URL);

    GET_JSON_VALUE_STR(CREATIVE_APPEND_ID, "CREATIVE_APPEND_ID");
    GET_JSON_VALUE_NUM(APPEND_STATUS, "APPEND_STATUS");
    GET_JSON_VALUE_NUM(CREATIVE_APPEND_TYPE, "CREATIVE_APPEND_TYPE");
    GET_JSON_VALUE_STR(APPEND_CONTENT, "APPEND_CONTENT");

    GET_JSON_VALUE_STR(ID, "ID");
    GET_JSON_VALUE_NUM(ID_TYPE, "ID_TYPE");
    GET_JSON_VALUE_NUM(BATCH_TYPE, "BATCH_TYPE");
    GET_JSON_VALUE_NUM(STATUS, "STATUS");

    GET_JSON_VALUE_STR(REPORT_ID, "REPORT_ID");
    GET_JSON_VALUE_NUM(REPORT_SPECIES, "REPORT_SPECIES");
    GET_JSON_VALUE_NUM(REPORT_TYPE, "REPORT_TYPE");
    GET_JSON_VALUE_NUM(REPORT_BELONG, "REPORT_BELONG");
    GET_JSON_VALUE_STR(REPORT_NAME, "REPORT_NAME");
    GET_JSON_VALUE_STR(TIME_START, "TIME_START");
    GET_JSON_VALUE_STR(TIME_END, "TIME_END");

    GET_JSON_VALUE_NUM(MONEY_TYPE, "MONEY_TYPE");

    GET_JSON_VALUE_STR(COMPANY_NAME, "COMPANY_NAME");
    GET_JSON_VALUE_STR(COMPANY_SITE, "COMPANY_SITE");
    GET_JSON_VALUE_STR(INDUSTRY    , "INDUSTRY");
    GET_JSON_VALUE_STR(PROVINCE_ID , "PROVINCE_ID");
    GET_JSON_VALUE_STR(CITY_ID     , "CITY_ID");
    GET_JSON_VALUE_STR(ADDRESS     , "ADDRESS");
    GET_JSON_VALUE_STR(POSTCODE    , "POSTCODE");
    GET_JSON_VALUE_STR(CONTACT     , "CONTACT");
    GET_JSON_VALUE_STR(TELEPHONE   , "TELEPHONE");
    GET_JSON_VALUE_STR(FAX         , "FAX");
    GET_JSON_VALUE_STR(MOBILE      , "MOBILE");
    GET_JSON_VALUE_STR(EMAIL       , "EMAIL");
    GET_JSON_VALUE_STR(WEBIM       , "WEBIM");
    GET_JSON_VALUE_STR(KEY, "KEY");

    return 0;
}

void ad_system_packet_get_sperad_list(int sockfd, ad_system_session_t *session, ad_system_packet_json_data_t *json, char *buffer)
{

    lg_list_t head;
    ad_system_oci_get_sperad_list(session->account_id, json->DISPLAY_STATUS, &head);

    size_t size = sprintf(buffer, "%s{\"CODE\":\"0\",\"CAMPAIGN_LIST\":[", HTTP_HEAD);
    send(sockfd, buffer, size, 0);

    lg_list_t *campaign_p = NULL;
    lg_list_for_each(campaign_p, &head) {

	ad_system_oci_sperad_list_campaign_t *campaign_node = NULL;
	campaign_node = (ad_system_oci_sperad_list_campaign_t *)campaign_p;

	if (campaign_p == head.next) 
	    size = sprintf(buffer, "{\"CAMPAIGN_ID\":\"%s\",\"CAMPAIGN_NAME\":\"%s\",\"CAMPAIGN_STATUS\":\"%u\",\"GROUP_LIST\":[",
		    campaign_node->CAMPAIGN_ID,
		    campaign_node->CAMPAIGN_NAME,
		    campaign_node->CAMPAIGN_STATUS?1:0);
	else
	    size = sprintf(buffer, ",{\"CAMPAIGN_ID\":\"%s\",\"CAMPAIGN_NAME\":\"%s\",\"CAMPAIGN_STATUS\":\"%u\",\"GROUP_LIST\":[",
		    campaign_node->CAMPAIGN_ID,
		    campaign_node->CAMPAIGN_NAME,
		    campaign_node->CAMPAIGN_STATUS?1:0);

	send(sockfd, buffer, size, 0);

	lg_list_t *group_p = NULL;
	lg_list_for_each(group_p, &campaign_node->head) {

	    ad_system_oci_sperad_list_group_t *group_node = NULL;
	    group_node = (ad_system_oci_sperad_list_group_t *)group_p;

	    int GROUP_STATUS = 1;
	    if (strcmp(group_node->GROUP_STATUS, "21") == 0)
		GROUP_STATUS = 1;
	    else if (strcmp(group_node->GROUP_STATUS, "22") == 0)
		GROUP_STATUS = 2;

	    if (group_p == campaign_node->head.next)
		size = sprintf(buffer, "{\"GROUP_ID\":\"%s\", \"GROUP_NAME\":\"%s\", \"GROUP_STATUS\":\"%u\"}",
			group_node->GROUP_ID,
			group_node->GROUP_NAME,
			GROUP_STATUS);
	    else
		size = sprintf(buffer, ",{\"GROUP_ID\":\"%s\", \"GROUP_NAME\":\"%s\", \"GROUP_STATUS\":\"%u\"}",
			group_node->GROUP_ID,
			group_node->GROUP_NAME,
			GROUP_STATUS);

	    send(sockfd, buffer, size, 0);
	}

	size = sprintf(buffer, "]}");
	send(sockfd, buffer, size, 0);
    }

    size = sprintf(buffer, "]}");
    send(sockfd, buffer, size, 0);
    close(sockfd);

    ad_system_oci_get_sperad_list_free(&head);
}

void ad_system_packet_json_parse(int sockfd, ad_system_session_t *session, char *buffer)
{

    lg_log("[recv][%s]%s\n", session->account_id, buffer);
    json_object *root = NULL;
    if (!(root = json_tokener_parse(buffer)))
	return ad_system_packet_send_code(sockfd, session, NULL, PACKET_CODE_FORMAT_ERR);

    if (is_error(root))
	return ad_system_packet_send_code(sockfd, session, NULL, PACKET_CODE_FORMAT_ERR);

    json_object *PACKET_TYPE = NULL;
    if (!(PACKET_TYPE = json_object_object_get(root, "PACKET_TYPE"))) {

	json_object_put(root);
	return ad_system_packet_send_code(sockfd, session, NULL, PACKET_CODE_FORMAT_ERR);
    }

    const char *packet_type = NULL;
    if (!(packet_type = json_object_get_string(PACKET_TYPE))) {

	json_object_put(root);
	return ad_system_packet_send_code(sockfd, session, NULL, PACKET_CODE_FORMAT_ERR);
    }

    /********************/
    /**** 登录 ****/
    /********************/

    if (strcmp(packet_type, "USER_LOGIN_CHECK") == 0) {
	ad_system_packet_login_check(sockfd, session, buffer);

    } else if (strcmp(packet_type, "GET_CAPTCHA") == 0) {
	ad_system_packet_send_image(sockfd, session, buffer);

    } else if (strcmp(packet_type, "USER_LOGIN") == 0) {
	ad_system_packet_login(sockfd, session, root, buffer);

    } else if (strcmp(packet_type, "USER_LOGOUT") == 0) {
	ad_system_packet_logout(sockfd, session, buffer);
    }

    //解析json
    ad_system_packet_json_data_t json_data;
    if (ad_system_packet_json_value_parse(root, &json_data)) {

	json_object_put(root);
	ad_system_packet_send_code(sockfd, session, NULL, PACKET_CODE_PARAMETER_ERR);
	pthread_exit(NULL);
    }

    //后台直接登录
    if (strcmp(packet_type, "DELIVERY_CENTER_LOGIN") == 0) {
	ad_system_packet_delivery_center_login(sockfd, session, &json_data, buffer);

	//未登录
    } else if (!(*session->account_id)) {
	ad_system_packet_send_code(sockfd, session, NULL, PACKET_CODE_USER_LOGIN_CHECK_ERR);


	/********************/
	/**** 首页 ****/
	/********************/

    } else if (strcmp(packet_type, "GET_TITLE_INFO") == 0) {
	ad_system_packet_get_title_info(sockfd, session, buffer);

    } else if (strcmp(packet_type, "GET_ANNOUNCEMENT") == 0) {
	ad_system_packet_get_announcement(sockfd, session, &json_data, buffer);

    } else if (strcmp(packet_type, "GET_STAT_TREND") == 0) {
	ad_system_packet_get_stat_trend(sockfd, session, &json_data, buffer);

    } else if (strcmp(packet_type, "GET_STAT_TREND_TEXT") == 0) {
	ad_system_packet_get_stat_trend_text(sockfd, session, buffer);

    } else if (strcmp(packet_type, "GET_STAT_TREND_IMG") == 0) {
	ad_system_packet_get_stat_trend_img(sockfd, session, buffer);

	/********************/
	/**** 推广 ****/
	/********************/

    } else if (strcmp(packet_type, "GET_REGION_CODE") == 0) {
	ad_system_packet_send_buffer(sockfd, session, ad_system_global_region_code.buffer, ad_system_global_region_code.size);

    } else if (strcmp(packet_type, "GET_DISEASE_CODE") == 0) {
	ad_system_packet_get_disease_code(sockfd, session, buffer);

    } else if (strcmp(packet_type, "GET_SPERAD_LIST") == 0) {
	ad_system_packet_get_sperad_list(sockfd, session, &json_data, buffer);

	//计划
    } else if (strcmp(packet_type, "CAMPAIGN_NAME_CHECK") == 0) {
	ad_system_packet_campaign_name_check(sockfd, session, &json_data, buffer);

    } else if (strcmp(packet_type, "GET_CAMPAIGN") == 0) {
	ad_system_packet_get_campaign(sockfd, session, &json_data, buffer);

    } else if (strcmp(packet_type, "SET_CAMPAIGN") == 0) {
	ad_system_packet_set_campaign(sockfd, session, &json_data, buffer);

    } else if (strcmp(packet_type, "ADD_CAMPAIGN") == 0) {
	ad_system_packet_add_campaign(sockfd, session, &json_data, buffer);

    } else if (strcmp(packet_type, "DEL_CAMPAIGN") == 0) {
	ad_system_packet_del_campaign(sockfd, session, &json_data, buffer);

    } else if (strcmp(packet_type, "GET_CAMPAIGN_UNIQUE") == 0) {
	ad_system_packet_get_campaign_unique(sockfd, session, &json_data, buffer);


	//组
    } else if (strcmp(packet_type, "GROUP_NAME_CHECK") == 0) {
	ad_system_packet_group_name_check(sockfd, session, &json_data, buffer);

    } else if (strcmp(packet_type, "GET_GROUP") == 0) {
	ad_system_packet_get_group(sockfd, session, &json_data, buffer);

    } else if (strcmp(packet_type, "ADD_GROUP_KEYWORD") == 0) {
	ad_system_packet_add_group_keyword(sockfd, session, &json_data, buffer);

    } else if (strcmp(packet_type, "GET_GROUP_KEYWORD") == 0) {
	ad_system_packet_get_group_keyword(sockfd, session, &json_data, buffer);

    } else if (strcmp(packet_type, "DEL_GROUP_KEYWORD") == 0) {
	ad_system_packet_del_group_keyword(sockfd, session, &json_data, buffer);

    } else if (strcmp(packet_type, "SET_GROUP") == 0) {
	ad_system_packet_set_group(sockfd, session, &json_data, buffer);

    } else if (strcmp(packet_type, "ADD_GROUP") == 0) {
	ad_system_packet_add_group(sockfd, session, &json_data, buffer);

    } else if (strcmp(packet_type, "DEL_GROUP") == 0) {
	ad_system_packet_del_group(sockfd, session, &json_data, buffer);

    } else if (strcmp(packet_type, "GET_GROUP_UNIQUE") == 0) {
	ad_system_packet_get_group_unique(sockfd, session, &json_data, buffer);


	//文字创意
    } else if (strcmp(packet_type, "GET_CREATIVE") == 0) {
	ad_system_packet_get_creative_text(sockfd, session, &json_data, buffer);

    } else if (strcmp(packet_type, "SET_CREATIVE") == 0) {
	ad_system_packet_set_creative_text(sockfd, session, &json_data, buffer);

    } else if (strcmp(packet_type, "ADD_CREATIVE") == 0) {
	ad_system_packet_add_creative_text(sockfd, session, &json_data, buffer);

    } else if (strcmp(packet_type, "DEL_CREATIVE") == 0) {
	ad_system_packet_del_creative_text(sockfd, session, &json_data, buffer);


	//图片创意
    } else if (strcmp(packet_type, "GET_IMG_CREATIVE") == 0) {
	ad_system_packet_get_img_creative(sockfd, session, &json_data, buffer);

    } else if (strcmp(packet_type, "SET_IMG_CREATIVE") == 0) {
	ad_system_packet_set_creative_img(sockfd, session, &json_data, buffer);

    } else if (strcmp(packet_type, "ADD_IMG_CREATIVE") == 0) {
	ad_system_packet_add_creative_img(sockfd, session, &json_data, buffer);

    } else if (strcmp(packet_type, "DEL_IMG_CREATIVE") == 0) {
	ad_system_packet_del_creative_img(sockfd, session, &json_data, buffer);

    } else if (strcmp(packet_type, "SET_UPLOAD_IMG_INFO") == 0) {
	ad_system_packet_set_upload_img_info(sockfd, session, &json_data, buffer);


	//附加创意
    } else if (strcmp(packet_type, "GET_CREATIVE_APPEND") == 0) {
	ad_system_packet_get_creative_append(sockfd, session, &json_data, buffer);

    } else if (strcmp(packet_type, "SET_CREATIVE_APPEND") == 0) {
	ad_system_packet_set_creative_append(sockfd, session, &json_data, buffer);

    } else if (strcmp(packet_type, "ADD_CREATIVE_APPEND") == 0) {
	ad_system_packet_add_creative_append(sockfd, session, &json_data, buffer);

    } else if (strcmp(packet_type, "DEL_CREATIVE_APPEND") == 0) {
	ad_system_packet_del_creative_append(sockfd, session, &json_data, buffer);

    } else if (strcmp(packet_type, "GET_CREATIVE_APPEND_UNIQUE") == 0) {
	ad_system_packet_get_creative_append_unique(sockfd, session, &json_data, buffer);

    } else if (strcmp(packet_type, "BATCH_OPERATION") == 0) {
	ad_system_packet_batch_operation(sockfd, session, &json_data, buffer);

	/********************/
	/**** 报告 ****/
	/********************/

    } else if (strcmp(packet_type, "REPORT_STANDARD_CREATE") == 0) {
	ad_system_packet_report_standard(sockfd, session, &json_data, buffer);

    } else if (strcmp(packet_type, "REPORT_COMPLEX_CREATE") == 0) {
	ad_system_packet_report_complex(sockfd, session, &json_data, buffer);

    } else if (strcmp(packet_type, "GET_REPORT_LIST") == 0) {
	ad_system_packet_get_report_list(sockfd, session, &json_data, buffer);

    } else if (strcmp(packet_type, "DET_REPORT") == 0) {
	ad_system_packet_del_report(sockfd, session, &json_data, buffer);


	/********************/
	/**** 财务管理 ****/
	/********************/

    } else if (strcmp(packet_type, "GET_MONEY_TREND") == 0) {
	ad_system_packet_get_money_trend(sockfd, session, buffer);

    } else if (strcmp(packet_type, "GET_MONEY_RECHARGE") == 0) {
	ad_system_packet_get_money_recharge(sockfd, session, &json_data, buffer);

	/********************/
	/**** 账户信息 ****/
	/********************/

    } else if (strcmp(packet_type, "GET_USER_INFO") == 0) {
	ad_system_packet_get_user_info(sockfd, session, buffer);

    } else if (strcmp(packet_type, "SET_USER_INFO") == 0) {
	ad_system_packet_set_user_info(sockfd, session, &json_data, buffer);

#if 0
    } else if (strcmp(packet_type, "GET_USER_PASS_PROTECT") == 0) {
	//ad_system_packet_get_user_pass_protect(sockfd, session, buffer);

    } else if (strcmp(packet_type, "USER_PASS_PROTECT_AUTHEN") == 0) {
	//ad_system_packet_authen_user_pass_protect(sockfd, session, &json_data, buffer);

    } else if (strcmp(packet_type, "SET_USER_PASS_PROTECT") == 0) {
	//ad_system_packet_set_user_pass_protect(sockfd, session, &json_data, buffer);
#endif

    } else if (strcmp(packet_type, "SET_USER_PASS") == 0) {
	ad_system_packet_set_user_pass(sockfd, session, &json_data, buffer);

    } else {
	ad_system_packet_send_code(sockfd, session, NULL, PACKET_CODE_FORMAT_ERR);
    }

    json_object_put(root);
}

void ad_system_packet_parse(int sockfd, char *buffer, char *cookies, char *addr)
{

    ad_system_session_t *session = NULL;
    if (!(session = ad_system_session_parse(cookies)))
	return ad_system_packet_send_code(sockfd, NULL, NULL, PACKET_CODE_ERR);

    strcpy(session->login_addr, addr);

    if (strncmp(buffer, "/action/?server=", 16) == 0)
	buffer += 16;
    else if (strncmp(buffer, "server=", 7) == 0)
	buffer += 7;
    else
	ad_system_packet_send_code(sockfd, NULL, NULL, PACKET_CODE_FORMAT_ERR);

    ad_system_packet_json_parse(sockfd, session, buffer);
}





