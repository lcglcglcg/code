
#include "lg_json.h"
#include "lg_string.h"
#include "lg_captcha.h"
#include "lg_ngx_network.h"

#include "manager_account_module.h"
#include "manager_account_packet.h"
#include "manager_account_sql.h"
#include "manager_account_redis.h"
#include "manager_account_subrequest.h"

void manager_account_packet_json_parse(ngx_http_request_t *r, manager_account_module_request_t *m_request, manager_account_json_t *json);
void manager_account_packet_login_check(ngx_http_request_t *r, manager_account_module_request_t *m_request, manager_account_json_t *json);
void manager_account_packet_login(ngx_http_request_t *r, manager_account_module_request_t *m_request, manager_account_json_t *json);
void manager_account_packet_logout(ngx_http_request_t *r, manager_account_module_request_t *m_request, manager_account_json_t *json);
void manager_account_packet_get_account_info(ngx_http_request_t *r, manager_account_module_request_t *m_request, manager_account_json_t *json);
void manager_account_packet_set_account_info(ngx_http_request_t *r, manager_account_module_request_t *m_request, manager_account_json_t *json);

extern ngx_str_t *manager_account_wangmeng_rsa(ngx_http_request_t *r, char *packet_type, char *buffer, int size);
void manager_account_packet_bind_wangmeng_account(ngx_http_request_t *r, manager_account_module_request_t *m_request, manager_account_json_t *json);
void manager_account_packet_login_wangmeng(ngx_http_request_t *r, manager_account_module_request_t *m_request, manager_account_json_t *json);

extern ngx_str_t *manager_account_ruanwen_rsa(ngx_http_request_t *r, char *packet_type, char *buffer, int size);
void manager_account_packet_bind_ruanwen_account(ngx_http_request_t *r, manager_account_module_request_t *m_request, manager_account_json_t *json);
void manager_account_packet_login_ruanwen(ngx_http_request_t *r, manager_account_module_request_t *m_request, manager_account_json_t *json);

//返回CODE
void ad_system_packet_code(ngx_http_request_t *r, int code)
{

    char buffer[32];
    size_t size = sprintf(buffer, "{\"CODE\":\"%u\"}", code);
    lg_ngx_network_buffer_post_send(r, NULL, buffer, size);
}

// 发送验证码
int manager_account_packet_var_code(ngx_http_request_t *r)
{

    char *session_id = NULL;
    if (lg_ngx_network_get_session(r, "MAKEY", &session_id))
	if (lg_ngx_network_create_session(r, "MAKEY", &session_id))
	    return NGX_ERROR;

    lg_captcha_t img;
    lg_captcha_create(&img);
    manager_account_redis_set_var_code(session_id, img.key);

    lg_ngx_network_t network;
    memset(&network, 0, sizeof(lg_ngx_network_t));
    network.content_type.data = (u_char *)"image/png;";
    network.content_type.len = sizeof("image/png");

    if (lg_ngx_network_buffer_add(r, &network, img.buffer, img.size))
	return -1;

    r->headers_out.status = NGX_HTTP_OK;
    r->headers_out.content_type = network.content_type;
    r->headers_out.content_length_n = network.content_length;

    int rc = ngx_http_send_header(r);
    if (rc == NGX_ERROR || rc > NGX_OK || r->header_only)
	return rc;

    return ngx_http_output_filter(r, network.head);
}

// 解析post buffer
char *manager_account_packet_post_buffer(ngx_http_request_t *r)
{

    size_t content_length = atoi((char *)r->headers_in.content_length->value.data);
    char *buffer = ngx_palloc(r->pool, content_length + 1);
    if (!buffer) return NULL;

    size_t size = 0;
    ngx_chain_t *p = NULL;
    for (p = r->request_body->bufs; p; p = p->next) {

	ngx_buf_t *node_buffer = p->buf;
	memcpy(&buffer[size], node_buffer->pos, node_buffer->last - node_buffer->pos);
	size += node_buffer->last - node_buffer->pos;

	if (size > content_length)
	    return NULL;
    }

    buffer[size] = 0;
    if (content_length != size)
	return NULL;

    if (!(size = lg_string_url_decode_r(buffer, size)))
	return NULL;

    if (strncmp("server=", buffer, 7) != 0)
	return NULL;

    buffer[size] = 0;
    return buffer + 7;
}

// 报文分析
void manager_account_packet_parse(ngx_http_request_t *r,
	// ngx_http_core_srv_conf_t *srv_conf,
	// manager_account_module_conf_t *loc_conf,
	// ngx_http_core_loc_conf_t *core_conf,
	manager_account_module_request_t *m_request)
{


    char *buffer = manager_account_packet_post_buffer(r);
    if (!buffer) return ad_system_packet_code(r, PACKET_CODE_FORMAT_ERR);
    lg_ngx_network_recv_print(buffer);

    lg_json_t *json_obj = lg_json_parse(buffer);
    if (!json_obj) return ad_system_packet_code(r, PACKET_CODE_FORMAT_ERR);

    manager_account_json_t json;
    if (manager_account_json_parse(json_obj, &json)) {

	lg_json_free(json_obj);
	return ad_system_packet_code(r, PACKET_CODE_FORMAT_ERR);
    }

    manager_account_packet_json_parse(r, m_request, &json);
    lg_json_free(json_obj);
}

// json分析
void manager_account_packet_json_parse(ngx_http_request_t *r, manager_account_module_request_t *m_request, manager_account_json_t *json)
{

    //检测登录
    if (strcmp(json->PACKET_TYPE, "LOGIN_CHECK") == 0) {
	return manager_account_packet_login_check(r, m_request, json);

	//登录
    } else if (strcmp(json->PACKET_TYPE, "LOGIN") == 0) {
	return manager_account_packet_login(r, m_request, json);

	//调试
    } else if (strcmp(json->PACKET_TYPE, "lcglcg") == 0) {

    } else {

	// 获取session映射的用户ID
	if (manager_account_redis_get_session(m_request->session_id, &m_request->account_id))
	    return ad_system_packet_code(r, PACKET_CODE_USER_LOGIN_CHECK_ERR);

	//登出
	if (strcmp(json->PACKET_TYPE, "LOGOUT") == 0) {
	    return manager_account_packet_logout(r, m_request, json);

	} else if (strcmp(json->PACKET_TYPE, "GET_REGION_CODE") == 0) {
	} else if (strcmp(json->PACKET_TYPE, "GET_DISEASE_CODE") == 0) {

	} else if (strcmp(json->PACKET_TYPE, "GET_ACCOUNT_INFO") == 0) {
	    return manager_account_packet_get_account_info(r, m_request, json);

	} else if (strcmp(json->PACKET_TYPE, "SET_ACCOUNT_INFO") == 0) {
	    return manager_account_packet_set_account_info(r, m_request, json);

	} else if (strcmp(json->PACKET_TYPE, "BIND_WANGMENG_ACCOUNT") == 0) {
	    return manager_account_packet_bind_wangmeng_account(r, m_request, json);

	} else if (strcmp(json->PACKET_TYPE, "LOGIN_WANGMENG") == 0) {
	    return manager_account_packet_login_wangmeng(r, m_request, json);

	} else if (strcmp(json->PACKET_TYPE, "BIND_RUANWEN_ACCOUNT") == 0) {
	    return manager_account_packet_bind_ruanwen_account(r, m_request, json);

	} else if (strcmp(json->PACKET_TYPE, "LOGIN_RUANWEN") == 0) {
	    return manager_account_packet_login_ruanwen(r, m_request, json);

	}
    }

    ad_system_packet_code(r, PACKET_CODE_FORMAT_ERR);
}

// 检测登录
void manager_account_packet_login_check(ngx_http_request_t *r, manager_account_module_request_t *m_request, manager_account_json_t *json)
{

    if (manager_account_redis_get_session(m_request->session_id, &m_request->account_id))
	return ad_system_packet_code(r, PACKET_CODE_USER_LOGIN_CHECK_ERR);
    return ad_system_packet_code(r, PACKET_CODE_USER_LOGIN_CHECK_OK);
}

// 登录
void manager_account_packet_login(ngx_http_request_t *r, manager_account_module_request_t *m_request, manager_account_json_t *json)
{

    char var_code[8] = {0};
    if (manager_account_redis_get_var_code(m_request->session_id, var_code))
	return ad_system_packet_code(r, PACKET_CODE_USER_LOGIN_ERR);

    if(strncasecmp(var_code, json->VAR_CODE, 4) != 0)
	return ad_system_packet_code(r, PACKET_CODE_USER_LOGIN_ERR);

    if (manager_account_sql_login(json->ACCOUNT_NAME, json->PASSWORD, &m_request->account_id))
	return ad_system_packet_code(r, PACKET_CODE_USER_LOGIN_ERR);

    if (manager_account_redis_set_session(m_request->session_id, m_request->account_id))
	return ad_system_packet_code(r, PACKET_CODE_ERR);

    return ad_system_packet_code(r, PACKET_CODE_OK);
}

void manager_account_packet_logout_event(ngx_http_request_t *r)
{

    fprintf(stdout, "[ruanwen logout][%s]\n", r->subrequest_buffer.data);
    return ad_system_packet_code(r, PACKET_CODE_OK);
}

// 用户登出
void manager_account_packet_logout(ngx_http_request_t *r, manager_account_module_request_t *m_request, manager_account_json_t *json)
{

    if (manager_account_redis_del_session(m_request->session_id))
	return ad_system_packet_code(r, PACKET_CODE_ERR);
    return ad_system_packet_code(r, PACKET_CODE_OK);

    // 推出软文
    char ruanwen_session[128] = {0};
    if (manager_account_redis_get_ruanwen_session(m_request->session_id, ruanwen_session) == 0) {

	char buffer[4096] = {0};
	size_t size = sprintf(buffer, ruanwen_session);
	fprintf(stdout, "[ruanwen-session][%s]\n", ruanwen_session);

	ngx_str_t loc = ngx_string("/ruanwen");
	ngx_str_t *args = manager_account_ruanwen_rsa(r, "3", buffer, size);
	if (!args) return ad_system_packet_code(r, PACKET_CODE_NOT_KEY);

	return lg_ngx_network_subrequest_init(r,
		manager_account_packet_logout_event,
		&loc,
		args);

    } else {

	return ad_system_packet_code(r, PACKET_CODE_OK);
    }

#if 0
    // 推出网盟
    char wangmeng_session[128] = {0};
    if (manager_account_redis_get_wangmeng_session(m_request->session_id, wangmeng_session) == 0) {

	char buffer[4096] = {0};
	size_t size = sprintf(buffer, wangmeng_session);

	ngx_str_t loc = ngx_string("/wangmeng");
	ngx_str_t *args = manager_account_wangmeng_rsa(r, "MA_LOGOUT", buffer, size);
	if (!args) return ad_system_packet_code(r, PACKET_CODE_NOT_KEY);

	return lg_ngx_network_subrequest_init(r,
		manager_account_packet_logout_event,
		&loc,
		args);

    } else {

	return ad_system_packet_code(r, PACKET_CODE_OK);
    }
#endif
}

//获取用户信息
void manager_account_packet_get_account_info(ngx_http_request_t *r, manager_account_module_request_t *m_request, manager_account_json_t *json)
{

    lg_oci_t sql_conn;
    if (manager_account_sql_get_account_info(&sql_conn, m_request->account_id))
	return ad_system_packet_code(r, PACKET_CODE_ERR);

    if (lg_oci_fetch_one(&sql_conn))
	return ad_system_packet_code(r, PACKET_CODE_ERR);

    char buffer[4096] = {0};
    size_t size = lg_json_sprintf(buffer, "{", "}", 4,
	    "%s", "CODE",		"0",
	    "%s", "ACCOUNT_NAME",	lg_oci_get_string(&sql_conn, 1),
	    "%s", "WANGMENG_TYPE",	lg_oci_get_string(&sql_conn, 2),
	    "%s", "RUANWEN_TYPE",	lg_oci_get_string(&sql_conn, 3)
	    );

    lg_oci_free(&sql_conn);
    return lg_ngx_network_buffer_post_send(r, NULL, buffer, size);
}

//修改用户信息
void manager_account_packet_set_account_info(ngx_http_request_t *r, manager_account_module_request_t *m_request, manager_account_json_t *json)
{

}

//绑定网盟->上游服务器返回
void manager_account_packet_bind_wangmeng_account_event(ngx_http_request_t *r)
{

    if (!r->subrequest_buffer.data)
	return ad_system_packet_code(r, PACKET_CODE_NOT_KEY);

    manager_account_module_request_t *m_request = r->m_request;
    if (!m_request) return ad_system_packet_code(r, PACKET_CODE_NOT_KEY);

    lg_json_t *json_root = lg_json_parse((char *)r->subrequest_buffer.data);
    if (!json_root) return ad_system_packet_code(r, PACKET_CODE_NOT_KEY);

    lg_json_t *json_code  = lg_json_get_object(json_root, "CODE");
    lg_json_t *json_account_id = lg_json_get_object(json_root, "ACCOUNT_ID");
    const char *code = lg_json_value_to_string(json_code);
    size_t account_id = lg_json_value_to_number(json_account_id);
    if (!code || !account_id) {

	lg_json_free(json_root);
	return ad_system_packet_code(r, PACKET_CODE_NOT_KEY);
    }

    if (strcmp(code, "0") != 0) {

	lg_json_free(json_root);
	return ad_system_packet_code(r, PACKET_CODE_NOT_KEY);
    }

    lg_json_free(json_root);
    if (manager_account_sql_bind_wangmeng(m_request->account_id, account_id))
	return ad_system_packet_code(r, PACKET_CODE_REPEAT_ERR);

    return ad_system_packet_code(r, PACKET_CODE_OK);
}

//绑定网盟
void manager_account_packet_bind_wangmeng_account(ngx_http_request_t *r, manager_account_module_request_t *m_request, manager_account_json_t *json)
{

    char buffer[4096];
    size_t size = sprintf(buffer, "%s:%s", json->ACCOUNT_NAME, json->PASSWORD);

    ngx_str_t loc = ngx_string("/wangmeng");
    ngx_str_t *args = manager_account_wangmeng_rsa(r, "MA_CHECK", buffer, size);
    if (!args) return ad_system_packet_code(r, PACKET_CODE_NOT_KEY);

    return lg_ngx_network_subrequest_init(r, 
	    manager_account_packet_bind_wangmeng_account_event,
	    &loc,
	    args);
}

//登录网盟->上游服务器返回
void manager_account_packet_login_wangmeng_event(ngx_http_request_t *r)
{

    if (!r->subrequest_buffer.data)
	return ad_system_packet_code(r, PACKET_CODE_NOT_KEY);

    manager_account_module_request_t *m_request = r->m_request;
    if (!m_request) return ad_system_packet_code(r, PACKET_CODE_NOT_KEY);

    if (strcmp("{\"CODE\":\"0\"}", (char *)r->subrequest_buffer.data) != 0)
	return ad_system_packet_code(r, PACKET_CODE_NOT_KEY);

    char buffer[1024];
    size_t size = sprintf(buffer, PACKET_WANGMENG_LOGIN_URL, m_request->wangmeng_session);
    return lg_ngx_network_buffer_post_send(r, NULL, buffer, size);
}

//登录网盟
void manager_account_packet_login_wangmeng(ngx_http_request_t *r, manager_account_module_request_t *m_request, manager_account_json_t *json)
{

    size_t wangmeng_id = 0;
    if (manager_account_sql_get_wangmeng_account(m_request->account_id, &wangmeng_id))
	return ad_system_packet_code(r, PACKET_CODE_NOT_KEY);

    char buffer[4096];
    size_t size = sprintf(buffer, "%s:%zd",
	    lg_string_uuid(m_request->wangmeng_session),
	    wangmeng_id);

    if (manager_account_redis_set_wangmeng_session(m_request->session_id, m_request->wangmeng_session))
	return ad_system_packet_code(r, PACKET_CODE_ERR);

    ngx_str_t loc = ngx_string("/wangmeng");
    ngx_str_t *args = manager_account_wangmeng_rsa(r, "MA_SESSION", buffer, size);
    if (!args) return ad_system_packet_code(r, PACKET_CODE_NOT_KEY);

    return lg_ngx_network_subrequest_init(r,
	    manager_account_packet_login_wangmeng_event,
	    &loc,
	    args);
}







//绑定软文->上游服务器返回
void manager_account_packet_bind_ruanwen_account_event(ngx_http_request_t *r)
{

    if (!r->subrequest_buffer.data)
	return ad_system_packet_code(r, PACKET_CODE_UPSTREAM_ERR);

    fprintf(stdout, "[bind data][%s]\n", r->subrequest_buffer.data);

    manager_account_module_request_t *m_request = r->m_request;
    if (!m_request) return ad_system_packet_code(r, PACKET_CODE_UPSTREAM_ERR);

    lg_json_t *json_root = lg_json_parse((char *)r->subrequest_buffer.data);
    if (!json_root) return ad_system_packet_code(r, PACKET_CODE_UPSTREAM_ERR);

    lg_json_t *json_code  = lg_json_get_object(json_root, "code");
    lg_json_t *json_account_id = lg_json_get_object(json_root, "account_id");
    const char *code = lg_json_value_to_string(json_code);
    size_t account_id = lg_json_value_to_number(json_account_id);
    if (!code || !account_id) {

	lg_json_free(json_root);
	return ad_system_packet_code(r, PACKET_CODE_PARAMETER_ERR);
    }

    if (strcmp(code, "0") != 0) {

	lg_json_free(json_root);
	return ad_system_packet_code(r, PACKET_CODE_PARAMETER_ERR);
    }

    lg_json_free(json_root);
    if (manager_account_sql_bind_ruanwen(m_request->account_id, account_id))
	return ad_system_packet_code(r, PACKET_CODE_REPEAT_ERR);

    return ad_system_packet_code(r, PACKET_CODE_OK);
}

//绑定软文
void manager_account_packet_bind_ruanwen_account(ngx_http_request_t *r, manager_account_module_request_t *m_request, manager_account_json_t *json)
{

    char buffer[4096];
    size_t size = sprintf(buffer, "%s:%s", json->ACCOUNT_NAME, json->PASSWORD);

    ngx_str_t loc = ngx_string("/ruanwen");
    ngx_str_t *args = manager_account_ruanwen_rsa(r, "1", buffer, size);
    if (!args) return ad_system_packet_code(r, PACKET_CODE_NOT_KEY);

    return lg_ngx_network_subrequest_init(r, 
	    manager_account_packet_bind_ruanwen_account_event,
	    &loc,
	    args);
}

//登录软文->上游服务器返回
void manager_account_packet_login_ruanwen_event(ngx_http_request_t *r)
{

    if (!r->subrequest_buffer.data)
	return ad_system_packet_code(r, PACKET_CODE_NOT_KEY);

    fprintf(stdout, "[login data][%s]\n", r->subrequest_buffer.data);

    manager_account_module_request_t *m_request = r->m_request;
    if (!m_request) return ad_system_packet_code(r, PACKET_CODE_NOT_KEY);

    char buffer[1024];
    size_t size = sprintf(buffer, "%s&session_id=%s", (char *)r->subrequest_buffer.data, m_request->ruanwen_session);
    return lg_ngx_network_buffer_post_send(r, NULL, buffer, size);
}

//登录软文
void manager_account_packet_login_ruanwen(ngx_http_request_t *r, manager_account_module_request_t *m_request, manager_account_json_t *json)
{

    size_t ruanwen_id = 0;
    if (manager_account_sql_get_ruanwen_account(m_request->account_id, &ruanwen_id))
	return ad_system_packet_code(r, PACKET_CODE_NOT_KEY);

    char buffer[4096];
    size_t size = sprintf(buffer, "%s:%zd",
	    lg_string_uuid(m_request->ruanwen_session),
	    ruanwen_id);

    if (manager_account_redis_set_ruanwen_session(m_request->session_id, m_request->ruanwen_session))
	return ad_system_packet_code(r, PACKET_CODE_ERR);

    ngx_str_t loc = ngx_string("/ruanwen");
    ngx_str_t *args = manager_account_ruanwen_rsa(r, "2", buffer, size);
    if (!args) return ad_system_packet_code(r, PACKET_CODE_NOT_KEY);

    return lg_ngx_network_subrequest_init(r,
	    manager_account_packet_login_ruanwen_event,
	    &loc,
	    args);
}



