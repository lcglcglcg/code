
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lg_oci.h"

int SQLOCI_Init(SQLOCI_CONN_T *conn)
{

    if (!conn)return -1;
    memset(conn, 0, sizeof(SQLOCI_CONN_T));

    if (OCIEnvInit(
		&conn->envhp,
		OCI_DEFAULT,
		0,
		NULL) != 0) {

	SQLOCI_Error(conn);
	return -1;
    }

    if (OCIHandleAlloc(
		conn->envhp,
		(dvoid **)&conn->errhp,
		OCI_HTYPE_ERROR,
		0,
		NULL) != 0) {

	SQLOCI_Error(conn);
	SQLOCI_Close(conn);
	return -1;
    }

    if (OCIHandleAlloc(
		conn->envhp,
		(dvoid **)&conn->svchp,
		OCI_HTYPE_SVCCTX,
		0,
		NULL) != 0) {

	SQLOCI_Error(conn);
	SQLOCI_Close(conn);
	return -1;
    }

    if (OCIHandleAlloc(
		conn->envhp,
		(dvoid **)&conn->stmthp,
		OCI_HTYPE_STMT,
		0,
		NULL) != 0) {

	SQLOCI_Error(conn);
	SQLOCI_Close(conn);
	return -1;
    }

    return 0;
}

void SQLOCI_Close(SQLOCI_CONN_T *conn)
{

    OCILogoff(conn->svchp, conn->errhp);
    OCIHandleFree((dvoid *)conn->envhp, OCI_HTYPE_ENV);
    OCIHandleFree((dvoid *)conn->errhp, OCI_HTYPE_ERROR);
    OCIHandleFree((dvoid *)conn->svchp,  OCI_HTYPE_SVCCTX);
    OCIHandleFree((dvoid *)conn->stmthp, OCI_HTYPE_STMT);
}

char *SQLOCI_Error(SQLOCI_CONN_T *conn)
{

    memcpy(conn->err_buf, "ok", 3);
    OCIErrorGet(
	    (dvoid *)conn->errhp,
	    1,
	    NULL,
	    &conn->err_code,
	    (text *)conn->err_buf,
	    512,
	    OCI_HTYPE_ERROR);

    return conn->err_buf;
}

int SQLOCI_Connect(SQLOCI_CONN_T *conn, const char *name, const char *pass, const char *host)
{

    return OCILogon(
	    conn->envhp,
	    conn->errhp,
	    &conn->svchp,
	    (text *)name, strlen(name),
	    (text *)pass, strlen(pass),
	    (text *)host, strlen(host));
}

int SQLOCI_Prepare(SQLOCI_CONN_T *conn, const char *sql_str, size_t sql_size)
{

    return OCIStmtPrepare(
	    conn->stmthp,
	    conn->errhp,
	    (text *)sql_str,
	    sql_size,
	    OCI_NTV_SYNTAX,
	    OCI_DEFAULT);
}

int SQLOCI_Bind_Input(SQLOCI_CONN_T *conn, const char *format, ...)
{

    int col = 0;
    int *num = NULL;
    char *str = NULL;
    va_list arg_list;
    va_start(arg_list, format);

    for (col = 0; *format; format++) {

	if (*format == '%') {

	    switch(*(format + 1)){

		case 'd':
		    num = va_arg(arg_list, int *);
		    if (OCIBindByPos(
				conn->stmthp,
				&conn->bindhp,
				conn->errhp,
				++col,
				num,
				(sizeof(int)),
				SQLT_INT,
				NULL,
				NULL,
				NULL,
				0,
				NULL,
				OCI_DEFAULT) != 0) {

			va_end(arg_list);
			SQLOCI_Error(conn);
			return -1;
		    }
		    break;

		case 's':
		    str = va_arg(arg_list, char *);
		    if (OCIBindByPos(
				conn->stmthp,
				&conn->bindhp,
				conn->errhp,
				++col,
				str,
				SQLOCI_STR_MAX,
				SQLT_STR,
				NULL,
				NULL,
				NULL,
				0,
				NULL,
				OCI_DEFAULT) != 0) {

			va_end(arg_list);
			SQLOCI_Error(conn);
			return -1;
		    }
		    break;

		case 't':
		    str = va_arg(arg_list, char *);
		    if (OCIBindByPos(
				conn->stmthp,
				&conn->bindhp,
				conn->errhp,
				++col,
				str,
				SQLOCI_DAT_MAX,
				SQLT_DAT,
				NULL,
				NULL,
				NULL,
				0,
				NULL,
				OCI_DEFAULT) != 0) {

			va_end(arg_list);
			SQLOCI_Error(conn);
			return -1;
		    }
		    break;

		default:
		    break;
	    }
	}
    }

    va_end(arg_list);
    return 0;
}

int SQLOCI_Bind_Ouput(SQLOCI_CONN_T *conn, const char *format, ...)
{

    int col = 0;
    int *num = NULL;
    char *str = NULL;
    double *flt = NULL;

    va_list arg_list;
    va_start(arg_list, format);

    for (col = 0; *format; format++) {

	if (*format == '%') {

	    switch(*(format + 1)){

		case 'd':
		    num = va_arg(arg_list, int *);
		    if (OCIDefineByPos(
				conn->stmthp,
				&conn->dfnhp,
				conn->errhp,
				++col,
				num,
				sizeof(int),
				SQLT_INT,
				NULL,
				NULL,
				NULL,
				OCI_DEFAULT) != 0) {

			va_end(arg_list);
			SQLOCI_Error(conn);
			return -1;
		    }
		    break;

		case 's':
		    str = va_arg(arg_list, char *);
		    if (OCIDefineByPos(
				conn->stmthp,
				&conn->dfnhp,
				conn->errhp,
				++col,
				str,
				SQLOCI_STR_MAX,
				SQLT_STR,
				NULL,
				NULL,
				NULL,
				OCI_DEFAULT) != 0) {

			va_end(arg_list);
			SQLOCI_Error(conn);
			return -1;
		    }
		    break;

		case 'f':
		    flt = va_arg(arg_list, double *);
		    if (OCIDefineByPos(
				conn->stmthp,
				&conn->dfnhp,
				conn->errhp,
				++col,
				flt,
				sizeof(double),
				SQLT_FLT,
				NULL,
				NULL,
				NULL,
				OCI_DEFAULT) != 0) {

			va_end(arg_list);
			SQLOCI_Error(conn);
			return -1;
		    }
		    break;

		default:
		    break;
	    }
	}
    }

    va_end(arg_list);
    return 0;
}

int SQLOCI_Execute(SQLOCI_CONN_T *conn)
{

    return OCIStmtExecute(
	    conn->svchp,
	    conn->stmthp,
	    conn->errhp,
	    1,
	    0,
	    NULL,
	    NULL,
	    OCI_DEFAULT);
}

int SQLOCI_Next_Data(SQLOCI_CONN_T *conn)
{

    return OCIStmtFetch(conn->stmthp, conn->errhp, 1, 0, 0);
}

int SQLOCI_Commit(SQLOCI_CONN_T *conn)
{

    return OCITransCommit(conn->svchp, conn->errhp, 0);
}

int SQLOCI_RollBack(SQLOCI_CONN_T *conn)
{

    return OCITransRollback(conn->svchp, conn->errhp, OCI_DEFAULT);
}

int SQLOCI_STORE_RESULT(SQLOCI_CONN_T *conn)
{

    int col;
    OCIParam *param = NULL;

    //获取列句柄
    if (OCIDefineByPos(
		conn->stmthp, 
		&conn->dfnhp, 
		conn->errhp, 
		1, 
		NULL, 
		0, 
		SQLT_STR, 
		NULL, 
		NULL, 
		NULL,
		OCI_DYNAMIC_FETCH) != OCI_SUCCESS)
	return -1;

    //空执行
    if (OCIStmtExecute(conn->svchp, 
		conn->stmthp, 
		conn->errhp, 
		0,
		0,
		NULL,
		NULL,
		OCI_DEFAULT) != OCI_SUCCESS)
	return -1;

    //获取列信息
    for(col = 1; 
	    (OCIParamGet(conn->stmthp,
			 OCI_HTYPE_STMT,
			 conn->errhp,
			 (dvoid **)&param,
			 col) == OCI_SUCCESS);
	    col++) {

	size_t size = 0;
	if (OCIAttrGet(param,
		    OCI_DTYPE_PARAM, 
		    &size, 
		    0,
		    OCI_ATTR_DATA_SIZE, 
		    conn->errhp) != OCI_SUCCESS)

	    return -1;

#if 0
	char *buffer = malloc(size);
	if (OCIDefineByPos(conn->stmthp,
		    &conn->dfnhp,
		    conn->errhp,
		    col,
		    buffer,
		    size,
		    SQLT_STR,
		    NULL,
		    NULL,
		    NULL,
		    OCI_DEFAULT) != OCI_SUCCESS)
	    return -1;
#endif
    }

    return 0;
}



