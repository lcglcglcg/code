
#include <stdio.h>
#include "lg_oci.h"
#include "lg_config_file.h"
#include "manager_account_sql.h"

int manager_account_sql_init(lg_list_t *config_file_head)
{

    char *oracle_name = NULL;
    if (!(oracle_name = lg_config_file_find(config_file_head, "oracle_name")))
	return -1;;

    char *oracle_pass = NULL;
    if (!(oracle_pass = lg_config_file_find(config_file_head, "oracle_pass")))
	return -1;;

    char *oracle_host = NULL;
    if (!(oracle_host = lg_config_file_find(config_file_head, "oracle_host")))
	return -1;;

    int oracle_pool = 0;
    if (!(oracle_pool = lg_config_file_find_number(config_file_head, "oracle_pool")))
	return -1;;

    // fprintf(stdout, "ORACLE_NAME=%s\n", oracle_name);
    // fprintf(stdout, "ORACLE_PASS=%s\n", oracle_pass);
    // fprintf(stdout, "ORACLE_HOST=%s\n", oracle_host);
    // fprintf(stdout, "ORACLE_POOL=%d\n", oracle_pool);
    return lg_oci_init(oracle_host, oracle_name, oracle_pass, oracle_pool);
}

int manager_account_sql_get_number(lg_oci_t *conn, int index)
{
    int ret = 0;
    if ((ret = lg_oci_get_number(conn, index)))
	return ret;

    lg_oci_free(conn);
    return 0;
}

const char *manager_account_sql_get_string(lg_oci_t *conn, int index)
{
    const char *ret = NULL;
    if ((ret = lg_oci_get_string(conn, index)))
	return ret;

    lg_oci_free(conn);
    return NULL;
}

int manager_account_sql_login(const char *account_name, const char *password, size_t *account_id)
{

    char sql_str[1024] = {0};
    sprintf(sql_str, "select "
	    "ACCOUNT_ID "
	    "from T_ACCOUNT "
	    "where ACCOUNT_NAME='%s' and PASSWORD_MD5='%s'",
	    account_name, password);

    lg_oci_t sql_conn;
    if (lg_oci_select(&sql_conn, sql_str))
	return -1;

    if (lg_oci_fetch_one(&sql_conn))
	return -1;

    if (!(*account_id = manager_account_sql_get_number(&sql_conn, 1)))
	return -1;

    lg_oci_free(&sql_conn);
    return 0;
}

int manager_account_sql_get_account_info(lg_oci_t *p, size_t account_id)
{

    char sql_str[1024] = {0};
    sprintf(sql_str, "select "
	    "t.account_name,"
	    "decode(wangmeng.wangmeng_id, null, 2, 1),"
	    "decode(ruanwen.ruanwen_id, null, 2, 1)"
	    "from t_account t "
	    "left join t_wangmeng wangmeng on t.account_id = wangmeng.account_id "
	    "left join t_ruanwen ruanwen on t.account_id = ruanwen.account_id "
	    "where t.account_id=%zd", account_id);

    return lg_oci_select(p, sql_str);
}

int manager_account_sql_bind_wangmeng(size_t account_id, size_t wangmeng_id)
{

    char sql_str[1024] = {0};
    sprintf(sql_str, "insert into t_wangmeng (account_id, wangmeng_id) values (%zd,%zd)", account_id, wangmeng_id);

    lg_oci_t sql_conn;
    if (lg_oci_conn(&sql_conn))
	return -1;

    if (lg_oci_execute(&sql_conn, sql_str))
	return -1;

    lg_oci_commit(&sql_conn);
    lg_oci_free(&sql_conn);
    return 0;
}

int manager_account_sql_get_wangmeng_account(size_t account_id, size_t *wangmeng_id)
{

    char sql_str[1024] = {0};
    sprintf(sql_str, "select wangmeng_id from t_wangmeng where account_id=%zd", account_id);

    lg_oci_t sql_conn;
    if (lg_oci_select(&sql_conn, sql_str))
	return -1;

    if (lg_oci_fetch_one(&sql_conn))
	return -1;

    if (!(*wangmeng_id = manager_account_sql_get_number(&sql_conn, 1)))
	return -1;

    lg_oci_free(&sql_conn);
    return 0;
}


int manager_account_sql_bind_ruanwen(size_t account_id, size_t ruanwen_id)
{

    char sql_str[1024] = {0};
    sprintf(sql_str, "insert into t_ruanwen (account_id, ruanwen_id) values (%zd,%zd)", account_id, ruanwen_id);

    lg_oci_t sql_conn;
    if (lg_oci_conn(&sql_conn))
	return -1;

    if (lg_oci_execute(&sql_conn, sql_str))
	return -1;

    lg_oci_commit(&sql_conn);
    lg_oci_free(&sql_conn);
    return 0;
}

int manager_account_sql_get_ruanwen_account(size_t account_id, size_t *ruanwen_id)
{

    char sql_str[1024] = {0};
    sprintf(sql_str, "select ruanwen_id from t_ruanwen where account_id=%zd", account_id);

    lg_oci_t sql_conn;
    if (lg_oci_select(&sql_conn, sql_str))
	return -1;

    if (lg_oci_fetch_one(&sql_conn))
	return -1;

    if (!(*ruanwen_id = manager_account_sql_get_number(&sql_conn, 1)))
	return -1;

    lg_oci_free(&sql_conn);
    return 0;
}


