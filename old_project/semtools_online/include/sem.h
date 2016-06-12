
#ifndef	__SEM_ONLINE_H__
#define	__SEM_ONLINE_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <iconv.h>
#include <time.h>
#include <fcntl.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "libxl.h"
#include "lg_list.h"
#include "lg_queue.h"
#include "network.h"
#include "thread_pool.h"
#include "log_file.h"

#include "calendar.h"
#include "sem_user.h"
#include "baidu_user.h"
#include "task_list.h"

#define	SQL_USER "websem"
#define	SQL_PASS "websem"
#define	SQL_HOST "127.0.0.1"
#define	SQL_DB "websem"

#define	HTTP_GET "GET %s HTTP/1.1\r\nHost: %s\r\n\r\n"
#define	HTTP_GET_RETURN "HTTP/1.1 200 OK\r\n\
Server: Sem Linux\r\n\
Content-Type: text/html;charset=utf-8\r\n\
Connection: Keep-Alive\r\n\r\n"

#define	HTTP_HOME_INDEX "<html>\
<form method=post enctype=multipart/form-data>\
<input type=file name=filepath />\
<input type=submit value=submit />\
</form>"

#define	UPLOADS_PATH "/www/mfs/uploads"
#define	DOWNLOAD_PATH "/www/mfs/download"
#define	ZOOSNET_REPORT_PATH "/www/mfs/zoosnet_report"
#define	BAIDU_REPORT_PATH "/www/mfs/baidu_report"
#define	LOG_FILE_PATH "/var/log/sem_online"
#define	TASK_LIST_PATH "/var/log/sem_task_list"
#define	BAIDU_USER_LIST_PATH "/var/log/sem_baidu_user_list"
#define	CALENDAR_BAIDU_PATH "/var/log/calendar_baidu"
#define	CALENDAR_ZOOSNET_PATH "/var/log/calendar_zoosnet"
    
#endif


