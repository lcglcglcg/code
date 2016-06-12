
#include "log_file.h"

int log_file_open(logfile_t *logfile, struct tm *day_tm)
{

    char path[2048] = {0};
    sprintf(path, "%s/%04d_%02d_%02d.log",
	    logfile->path,
	    day_tm->tm_year + 1900,
	    day_tm->tm_mon + 1,
	    day_tm->tm_mday);

    FILE *logfp = NULL;
    if (!(logfp = fopen(path, "a+")))
	return -1;

    if (logfile->logfp)
	fclose(logfile->logfp);

    logfile->logfp = logfp;
    return 0;
}

int log_file_check(logfile_t *logfile, struct tm *day_tm)
{

    if (!logfile->logfp)
	return log_file_open(logfile, day_tm);

    if (day_tm->tm_mday != logfile->day.tm_mday) {

	if (log_file_open(logfile, day_tm) == 0)
	    memcpy(&logfile->day, day_tm, sizeof(struct tm));
	else 
	    return -1;
    }

    return 0;
}

void log_file_print(logfile_t *logfile, const char *format, ...)
{

    va_list argList;
    struct tm *day_tm = NULL;
    time_t day_time = time(NULL);
    if(!(day_tm = localtime(&day_time)))
	return;

    pthread_mutex_lock(logfile->mutex);

    if (log_file_check(logfile, day_tm) == 0) {

	fprintf(logfile->logfp, "[%04d-%02d-%02d][%02d:%02d:%02d]",
		day_tm->tm_year + 1900, 
		day_tm->tm_mon + 1, 
		day_tm->tm_mday,
		day_tm->tm_hour, 
		day_tm->tm_min, 
		day_tm->tm_sec);

	va_start(argList, format);
	vfprintf(logfile->logfp, format, argList);
	fflush(logfile->logfp);
	va_end(argList);
    }

    va_start(argList, format);
    vfprintf(stdout, format, argList);
    fflush(stdout);
    va_end(argList);

    pthread_mutex_unlock(logfile->mutex);
}

logfile_t *log_file_init(const char *path)
{

    logfile_t *logfile = NULL;
    if (!(logfile = malloc(sizeof(logfile_t))))
	return NULL;

    memset(logfile, 0, sizeof(logfile_t));
    logfile->path_len = strlen(path);
    memcpy(logfile->path, path, logfile->path_len);

    logfile->mutex = (pthread_mutex_t *)mmap(NULL,
	    sizeof(pthread_mutex_t),
	    PROT_READ | PROT_WRITE, 
	    MAP_SHARED | MAP_ANON,
	    -1, 
	    0);
    if (logfile->mutex == MAP_FAILED) {

	free(logfile);
	return NULL;
    }

    if (pthread_mutexattr_init(&logfile->mutex_attr) != 0) {

	munmap(logfile->mutex, sizeof(pthread_mutex_t));
	free(logfile);
	return NULL;
    }

    if (pthread_mutexattr_setpshared(&logfile->mutex_attr, PTHREAD_PROCESS_SHARED) != 0) {

	munmap(logfile->mutex, sizeof(pthread_mutex_t));
	free(logfile);
	return NULL;
    }

    if (pthread_mutex_init(logfile->mutex, &logfile->mutex_attr) != 0) {

	munmap(logfile->mutex, sizeof(pthread_mutex_t));
	free(logfile);
	return NULL;
    }

    struct tm *day_tm = NULL;
    time_t day_time = time(NULL);
    if(!(day_tm = localtime(&day_time))) {

	munmap(logfile->mutex, sizeof(pthread_mutex_t));
	free(logfile);
	return NULL;
    }

    memcpy(&logfile->day, day_tm, sizeof(struct tm));
    return logfile;
}

void log_file_free(logfile_t *logfile)
{
    munmap(logfile->mutex, sizeof(pthread_mutex_t));
    free(logfile);
}

