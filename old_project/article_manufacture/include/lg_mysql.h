
#ifndef __LG_MYSQL_H__
#define	__LG_MYSQL_H__

#include <stdio.h>

#ifdef __cplusplus
extern "C" { 
#endif

    typedef struct {

	char *host;
	char *base;
	char *user;
	char *pass;

    } lg_mysql_t;

    typedef struct {

	int buffer_type;
	long unsigned int buffer_length;
	void *buffer;
	long unsigned int *length;

    } lg_mysql_bind_t;

    typedef int(lg_mysql_select_callback)(int row, int col, char **data, size_t *size, void *arg);
    extern int lg_mysql_select(lg_mysql_t *p, char *sql, lg_mysql_select_callback *callback, void *arg);

#ifdef __cplusplus
}
#endif

#endif

