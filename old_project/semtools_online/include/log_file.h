
#ifndef	__LOG_FILE_H__
#define	__LOG_FILE_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <stdarg.h>
#include <sys/mman.h>
#include <sys/types.h>
#define	LOG_PATH_LEN 1024

typedef struct {

    FILE *logfp;
    struct tm day;

    char path[LOG_PATH_LEN];
    size_t path_len;

    pthread_mutex_t *mutex;
    pthread_mutexattr_t mutex_attr;

}logfile_t;

extern logfile_t *log_file_init(const char *path);
extern void log_file_print(logfile_t *logfile, const char *format, ...);
extern void log_file_free(logfile_t *logfile);

#endif


