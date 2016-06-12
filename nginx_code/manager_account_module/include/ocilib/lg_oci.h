
#ifndef __LG_OCI_H__
#define __LG_OCI_H__

#ifdef __cplusplus
extern "C" { 
#endif

    /**************/
    /* oracle API */
    /**************/

    typedef struct {

	void *cn;
	void *st;
	void *rs;

    } lg_oci_t;

    extern int lg_oci_init(char *oracle_host, char *oracle_name, char *oracle_pass, int oracle_pool);
    extern int lg_oci_conn(lg_oci_t *p);
    extern void lg_oci_free(lg_oci_t *p);

    extern int lg_oci_select(lg_oci_t *p, char *sql_str);
    extern int lg_oci_fetch_one(lg_oci_t *p);
    extern int lg_oci_fetch_next(lg_oci_t *p);

    extern int lg_oci_execute(lg_oci_t *p, char *sql_str);
    extern void lg_oci_commit(lg_oci_t *p);
    extern void lg_oci_rollback(lg_oci_t *p);

    extern int lg_oci_get_number(lg_oci_t *p, int index);
    extern double lg_oci_get_double(lg_oci_t *p, int index);
    extern const char *lg_oci_get_string(lg_oci_t *p, int index);

    extern int lg_oci_get_number_name(lg_oci_t *p, char *name);
    extern double lg_oci_get_double_name(lg_oci_t *p, char *name);
    extern const char *lg_oci_get_string_name(lg_oci_t *p, char *name);

#ifdef __cplusplus
}
#endif

#endif



