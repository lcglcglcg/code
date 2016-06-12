
#ifndef	__BAIDU_DOWNLOAD_H__
#define	__BAIDU_DOWNLOAD_H__

#include "soapH.h"
#include <curl/curl.h>
#include <sys/stat.h>
#include <sys/types.h>

extern int baidu_download_report_search(
	char *date_str,
	char *username, 
	char *password, 
	char *token, 
	char *path);

extern int baidu_download_report_pair(
	char *date_str,
	char *username, 
	char *password, 
	char *token, 
	char *path);

#endif

