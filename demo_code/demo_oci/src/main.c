
#include <stdio.h>
#include <ocilib.h>
#include "lg_ocilib.h"

char *host = "188.188.3.152/dev";
char *user = "lcg";
char *pass = "lcg";

int _main()
{

    fprintf(stdout, "start...\n");

    setenv("NLS_LANG","AMERICAN_AMERICA.UTF8",1);
    if (!OCI_Initialize(NULL, NULL, OCI_ENV_DEFAULT))
	return -1;; 

    OCI_Connection *cn = NULL;
    cn = OCI_ConnectionCreate(host, user, pass, 0);
    fprintf(stdout, "cn=%p\n", cn);

    OCI_Statement *st = NULL;
    if (!(st = OCI_StatementCreate(cn))) {

	OCI_ConnectionFree(cn);
	return -1;
    }

    fprintf(stdout, "st=%p\n", st);

    char *sql_str = "select * from lcglcg";
    if (!(OCI_ExecuteStmt(st, sql_str)))
	return -1;

    OCI_Resultset *rs = NULL;
    if (!(rs = (OCI_Resultset *)OCI_GetResultset(st)))
	return -1;

    fprintf(stdout, "rs=%p\n", rs);
    while(OCI_FetchNext(rs)) {

	fprintf(stdout, "%s:%s:%s\n",
		OCI_GetString(rs, 1),
		OCI_GetString(rs, 2),
		OCI_GetString(rs, 3));
    }

    return 0;
}

int main()
{

    lg_ocilib_init();

    while (1) {

	lg_ocilib_t conn;
	lg_ocilib_t *p = &conn;
	lg_ocilib_conn(p, host, user, pass);
	lg_ocilib_find(p, "select * from lcglcg");
	while(lg_ocilib_fetch_next(p)) {

	    fprintf(stdout, "%s:%s:%s\n",
		    lg_ocilib_get_string(p, 1),
		    lg_ocilib_get_string(p, 2),
		    lg_ocilib_get_string(p, 3)
		   );
	}

	lg_ocilib_free(p);
    }

    return 0;
}





