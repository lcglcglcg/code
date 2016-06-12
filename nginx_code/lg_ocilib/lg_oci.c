
#include <ocilib.h>
#include <ocilib_internal.h>
#include "lg_oci.h"

//static OCI_ConnPool *lg_oci_pool = NULL;
lg_oci_pool_t *lg_oci_init(char *oracle_host, char *oracle_user, char *oracle_pass, int oracle_pool)
{

    setenv("NLS_LANG","AMERICAN_AMERICA.UTF8",1);
    if (!OCI_Initialize(NULL, NULL, OCI_ENV_DEFAULT | OCI_ENV_THREADED))
	return NULL;

    lg_oci_pool_t *pool = malloc(sizeof(lg_oci_pool_t));
    if (!pool) return NULL;

    pool->host = oracle_host;
    pool->user = oracle_user;
    pool->pass = oracle_pass;
    pool->pool_count = oracle_pool;
    if (!(pool->pool_conn = OCI_PoolCreate(
		    oracle_host,
		    oracle_user,
		    oracle_pass,
		    OCI_POOL_CONNECTION, 
		    OCI_SESSION_DEFAULT, 
		    0,   
		    oracle_pool,
		    1))) {
	free(pool);
	return NULL;
    }

    return 0;
}

int lg_oci_conn(lg_oci_pool_t *pool, lg_oci_t *p)
{

    OCI_Connection *cn = NULL;
    if (!(cn = OCI_PoolGetConnection(pool->pool_conn, NULL)))
	return -1;

    OCI_Statement *st = NULL;
    if (!(st = OCI_StatementCreate(cn))) {

	OCI_ConnectionFree(cn);
	return -1;
    }

    p->cn = cn;
    p->st = st;
    p->rs = NULL;
    return 0;
}

void lg_oci_free(lg_oci_t *p)
{
    OCI_StatementFree(p->st);
    OCI_ConnectionFree(p->cn);
}

int lg_oci_select(lg_oci_pool_t *pool, lg_oci_t *p, char *sql_str)
{

    if (lg_oci_conn(pool, p))
	return -1;

    if (!(OCI_ExecuteStmt(p->st, sql_str))) {

	OCI_ConnectionFree(p->cn);
	return -1;
    }

    OCI_Resultset *rs = NULL;
    if (!(rs = OCI_GetResultset(p->st))) {

	OCI_ConnectionFree(p->cn);
	return -1;
    }

    p->rs = rs;
    return 0;
}

int lg_oci_fetch_one(lg_oci_t *p)
{

    if (OCI_FetchNext(p->rs))
	return 0;

    OCI_StatementFree(p->st);
    OCI_ConnectionFree(p->cn);
    return -1;
}

int lg_oci_fetch_next(lg_oci_t *p)
{
    return (OCI_FetchNext(p->rs));
}

int lg_oci_execute(lg_oci_t *p, char *sql_str)
{

    if (!(OCI_ExecuteStmt(p->st, sql_str))) {

	OCI_Rollback(p->cn);
	OCI_StatementFree(p->st);
	OCI_ConnectionFree(p->cn);
	return -1;
    }

    return 0;
}

void lg_oci_commit(lg_oci_t *p)
{
    OCI_Commit(p->cn);
}

void lg_oci_rollback(lg_oci_t *p)
{
    OCI_Rollback(p->cn);
}

int lg_oci_get_number(lg_oci_t *p, int index)
{
    return OCI_GetInt(p->rs, index);
}

int lg_oci_get_number_name(lg_oci_t *p, char *name)
{
    return OCI_GetInt2(p->rs, name);
}

double lg_oci_get_double(lg_oci_t *p, int index)
{
    return OCI_GetDouble(p->rs, index);
}

double lg_oci_get_double_name(lg_oci_t *p, char *name)
{

    return OCI_GetDouble2(p->rs, name);
}

const char *lg_oci_get_string(lg_oci_t *p, int index)
{
    return OCI_GetString(p->rs, index);
}

const char *lg_oci_get_string_name(lg_oci_t *p, char *name)
{
    return OCI_GetString2(p->rs, name);
}




