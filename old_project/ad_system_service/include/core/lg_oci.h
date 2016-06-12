
#ifndef	__LG_OCI_H__
#define	__LG_OCI_H__

#include "oci.h"

#define	SQLOCI_ENV() setenv("NLS_LANG","AMERICAN_AMERICA.UTF8",1);OCIInitialize(OCI_DEFAULT,NULL,NULL,NULL,NULL)
#define	SQLOCI_STR_MAX 1024
#define	SQLOCI_DAT_MAX 128

#ifdef __cplusplus
extern "C" { 
#endif

    typedef struct {

	OCIEnv *envhp;
	OCIError *errhp;
	OCISvcCtx *svchp;
	OCIStmt *stmthp;

	OCIDefine *dfnhp;
	OCIBind *bindhp;

	char err_buf[512];
	int err_code;

    } SQLOCI_CONN_T;

    typedef struct {

	char *name;
	char *value;
	/** CODE **/

    } SQLOCI_STORE_RESULT_T;

    extern int SQLOCI_Init(SQLOCI_CONN_T *conn);
    extern void SQLOCI_Close(SQLOCI_CONN_T *conn);
    extern char *SQLOCI_Error(SQLOCI_CONN_T *conn);

    extern int SQLOCI_Connect(SQLOCI_CONN_T *conn, const char *name, const char *pass, const char *host);
    extern int SQLOCI_Prepare(SQLOCI_CONN_T *conn, const char *sql_str, size_t sql_size);

    extern int SQLOCI_Bind_Input(SQLOCI_CONN_T *conn, const char *format, ...);
    extern int SQLOCI_Bind_Ouput(SQLOCI_CONN_T *conn, const char *format, ...);

    extern int SQLOCI_Execute(SQLOCI_CONN_T *conn);
    extern int SQLOCI_Next_Data(SQLOCI_CONN_T *conn);
    extern int SQLOCI_Commit(SQLOCI_CONN_T *conn);
    extern int SQLOCI_RollBack(SQLOCI_CONN_T *conn);

#ifdef __cplusplus
}
#endif

#endif


