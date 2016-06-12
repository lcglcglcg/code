
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <time.h>
#include "sqlite3.h"

typedef struct {

    char *value[1024];

}sem_csv_value_t;

static char sem_sqlite_error[1024];
int sem_csv_row_analysis(char *row_str, sem_csv_value_t *sem_csv_value)
{

    char *str = row_str;
    int i, quote_status = 0;

    if (*str == '"')
	sem_csv_value->value[0] = &str[1];
    else
	sem_csv_value->value[0] = str;

    for (i = 0; *str; str++) {

	if (*str == '"') {

	    *str = 0;
	    quote_status = !quote_status;
	}

	if (!quote_status && *str == ',') {

	    *str = 0;
	    if (str[1] == '"')
		sem_csv_value->value[++i] = &str[2];
	    else
		sem_csv_value->value[++i] = &str[1];
	}
    }

    *str = 0;
    return (i + 1);
}

int sem_csv_open(char *csv_path, char **p)
{

    int size = 0;
    char *buffer = NULL;
    FILE *fp = NULL;

    if (!(fp = fopen(csv_path, "rb")))
	return -1;

    fseek(fp, 0, SEEK_END);
    if ((size = ftell(fp)) <= 0) {

	fclose(fp);
	return -1;
    }

    if (!(buffer = malloc(size))) {

	fclose(fp);
	return -1;
    }

    fseek(fp, 0, SEEK_SET);
    if (fread(buffer, 1, size, fp) != size) {

	fclose(fp);
	free(buffer);
	return -1;
    }

    fclose(fp);
    *p = buffer;
    return size;
}

sqlite3 *sem_sqlite_open(char *db_path)
{

    sqlite3 *db = NULL;
    sqlite3_initialize();
    sqlite3_open(db_path, &db);
    if (db && sqlite3_errcode(db) == SQLITE_OK ) {

	sqlite3_create_function(db, "shellstatic", 0, SQLITE_UTF8, 0, NULL, 0, 0);
    }

    if (!db || sqlite3_errcode(db) != SQLITE_OK ) {

	sprintf(sem_sqlite_error,"[sqlite3_open][%s]\n", sqlite3_errmsg(db));
	return NULL;
    }

    sqlite3_config(SQLITE_CONFIG_URI, 1);
    sqlite3_config(SQLITE_CONFIG_SINGLETHREAD);

#ifndef SQLITE_OMIT_LOAD_EXTENSION
    sqlite3_enable_load_extension(db, 1);
#endif

    return db;
}

int sem_csv_title_to_sqlite(sqlite3 *db, sqlite3_stmt **pStmt, char *table, int csv_col, sem_csv_value_t *sem_csv_value)
{


    int i;
    char sql_str[4096] = {0};
    sprintf(sql_str, "INSERT INTO %s (", table);

    for (i = 0; i < csv_col; i++) {

	//fprintf(stdout, "[%d][%s]\n", i, sem_csv_value->value[i]);
	strcat(sql_str, sem_csv_value->value[i]);
	if (i == csv_col - 1)
	    strcat(sql_str, ")");
	else
	    strcat(sql_str, ",");
    }

    strcat(sql_str, " VALUES (");
    for (i = 0; i < csv_col; i++) {

	strcat(sql_str, "?");
	if (i == csv_col - 1)
	    strcat(sql_str, ")");
	else
	    strcat(sql_str, ",");
    }

    if (sqlite3_prepare(db, sql_str, -1, pStmt, 0) != SQLITE_OK) {

	sprintf(sem_sqlite_error,"[sqlite3_prepare][%s]\n", sqlite3_errmsg(db));
	return -1;
    }

    if (sqlite3_exec(db, "BEGIN", 0, 0, 0) != SQLITE_OK) {

	sprintf(sem_sqlite_error,"[sqlite3_exec][%s]\n", sqlite3_errmsg(db));
	return -1;
    }

    return 0;
}

char *sem_sqlite_csv_import(char *db_path, char *db_table, char *csv_path)
{

    sqlite3 *db = NULL;
    sqlite3_stmt *pStmt = NULL;
    if (!(db = sem_sqlite_open(db_path)))
	return sem_sqlite_error;

    //sqlite3_key(db, NULL, 0);
    if (!sqlite3_mprintf("SELECT * FROM %s", db_table)) {

	sqlite3_close(db);
	sprintf(sem_sqlite_error, "[not %s table]", db_table);
	return sem_sqlite_error;
    }

    int i, j, size;
    char *buffer = NULL;
    if ((size = sem_csv_open(csv_path, &buffer)) == -1) {

	free(buffer);
	sqlite3_close(db);
	sprintf(sem_sqlite_error, "[sem_csv_open][error]\n");
	return sem_sqlite_error;
    }

    sprintf(sem_sqlite_error, "[sem_csv_open][ok]\n");

    sem_csv_value_t *sem_csv_value = NULL;
    if (!(sem_csv_value = malloc(sizeof(sem_csv_value_t)))) {

	free(buffer);
	sqlite3_close(db);
	sprintf(sem_sqlite_error, "[sem_csv_value malloc][error]\n");
	return sem_sqlite_error;
    }

    int csv_row = 0;
    int csv_col = 0;
    int title_col = 0;
    char *row_str = buffer;
    for (i = 0; i < size; i++) {

	if (buffer[i] == '\r' || buffer[i] == '\n') {

	    csv_row++;
	    buffer[i] = 0;
	    csv_col = sem_csv_row_analysis(row_str, sem_csv_value);

	    if (title_col && title_col != csv_col) {

		free(buffer);
		sqlite3_finalize(pStmt);
		sqlite3_exec(db, "ROLLBACK", 0, 0, 0);
		sqlite3_close(db);
		sprintf(sem_sqlite_error, "[sem_csv_row_analysis error][row=%d][col=%d]\n", csv_row, csv_col);
		return sem_sqlite_error;

	    } else {

		title_col = csv_col;
	    }

	    if (row_str == buffer) {

		if (sem_csv_title_to_sqlite(db, &pStmt, db_table, csv_col, sem_csv_value) == -1) {

		    free(buffer);
		    sqlite3_finalize(pStmt);
		    sqlite3_exec(db, "ROLLBACK", 0, 0, 0);
		    sqlite3_close(db);
		    return sem_sqlite_error;
		}

	    } else {

		for (j = 0; j < csv_col; j++)
		    sqlite3_bind_text(pStmt, j + 1, sem_csv_value->value[j], -1, SQLITE_STATIC);

		sqlite3_step(pStmt);
		if (sqlite3_reset(pStmt) != SQLITE_OK) {

		    free(buffer);
		    sqlite3_finalize(pStmt);
		    sqlite3_exec(db, "ROLLBACK", 0, 0, 0);
		    sqlite3_close(db);
		    sprintf(sem_sqlite_error,"[sqlite3_reset][%s]\n", sqlite3_errmsg(db));
		    return sem_sqlite_error;
		}
	    }

	    row_str = &buffer[i + 1];
	}
    }

    free(buffer);
    sqlite3_finalize(pStmt);
    sqlite3_exec(db, "COMMIT", 0, 0, 0);
    sqlite3_close(db);
    sprintf(sem_sqlite_error, "[sem_sqlite][ok]");
    return sem_sqlite_error;
}

void service_time_print(char *str)
{

    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    fprintf(stdout, "[%s][%04d-%02d-%02d][%02d:%02d:%02d]\n",
	    str,
	    tm->tm_year + 1900,
	    tm->tm_mon + 1,
	    tm->tm_mday,
	    tm->tm_hour,
	    tm->tm_min,
	    tm->tm_sec);
}

int main(int argc, char *argv[])
{

    if (argc != 4) {

	fprintf(stdout, "Program does not specify the parameter\n");
	fprintf(stdout, "Program \"db_path\" \"db_table\" \"csv_path\"\n");
	return -1;
    }

    char *db_path = argv[1];
    char *db_table = argv[2];
    char *csv_path = argv[3];
    fprintf(stdout, "[db_path][%s]\n", db_path);
    fprintf(stdout, "[db_table][%s]\n", db_table);
    fprintf(stdout, "[csv_path][%s]\n", csv_path);

    service_time_print("service start");
    char *str = sem_sqlite_csv_import(db_path, db_table, csv_path);
    fprintf(stdout, "[sem_sqlite_csv_import][%s]\n", str);
    service_time_print("service stop");

    return 0;
}


