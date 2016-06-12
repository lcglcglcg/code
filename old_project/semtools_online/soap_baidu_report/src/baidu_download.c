
#include "baidu_download.h"

int http_download_file(char *url, char *path)
{

    FILE *fp = NULL;
    CURL *curl_handle = NULL;

    if (!(fp = fopen(path, "w")))
	return -1;

    curl_global_init(CURL_GLOBAL_ALL);
    if (!(curl_handle = curl_easy_init()))
	return -1;

    curl_easy_setopt(curl_handle, CURLOPT_URL, url);
    curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)fp);
    curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, fwrite);
    curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "libcurl");
    curl_easy_perform(curl_handle);

    curl_easy_cleanup(curl_handle);
    curl_global_cleanup();

    fclose(fp);
    return 0;
}

int request_baidu_report_search(
	time_t date_time,
	char *username,
	char *password,
	char *token,
	char *reportId)
{

    struct soap soap = {0};
    struct SOAP_ENV__Header soap_head = {0};
    struct ns2__AuthHeader soap_head_auth = {0};
    struct ns2__ResHeader soap_head_res = {0};

    soap_head.ns2__AuthHeader = &soap_head_auth;
    soap_head.ns2__ResHeader = &soap_head_res;
    soap_head_auth.username = username;
    soap_head_auth.password = password;
    soap_head_auth.token = token;

    soap_init(&soap);
    soap_set_mode(&soap, SOAP_C_UTFSTRING);
    soap.header = &soap_head;
    if(soap_ssl_client_context(&soap, SOAP_SSL_DEFAULT,
		NULL, NULL, "/etc/key.crt", NULL, NULL)) {

	return -1;
    }

    int levelOfDetails = 12;
    char *performanceData[] = {"impression", "click"};
    struct _ns1__getProfessionalReportIdRequest id_request = {0};
    struct _ns1__getProfessionalReportIdResponse id_return = {0};
    struct ns1__ReportRequestType _reportRequestType = {0};
    id_request.reportRequestType = &_reportRequestType;

    id_request.reportRequestType->__sizeperformanceData = 2;
    id_request.reportRequestType->performanceData = performanceData;
    id_request.reportRequestType->startDate = date_time;
    id_request.reportRequestType->endDate = date_time;
    id_request.reportRequestType->reportType = 6;
    id_request.reportRequestType->levelOfDetails = &levelOfDetails;

    if(soap_call___ns1__getProfessionalReportId(
		&soap,
		NULL,
		NULL,
		&id_request, 
		&id_return) != 0) {

	if (soap.header 
		&& soap.header->ns2__ResHeader 
		&& soap.header->ns2__ResHeader->failures) {

	    return soap.header->ns2__ResHeader->failures->code;
	}

	return -1;
    }

    strcpy(reportId, id_return.reportId);
    soap_destroy(&soap); 
    soap_end(&soap); 
    soap_done(&soap);
    return 0;
}

int request_baidu_report_pair(
	time_t date_time,
	char *username,
	char *password,
	char *token,
	char *reportId)
{

    struct soap soap = {0};
    struct SOAP_ENV__Header soap_head = {0};
    struct ns2__AuthHeader soap_head_auth = {0};
    struct ns2__ResHeader soap_head_res = {0};

    soap_head.ns2__AuthHeader = &soap_head_auth;
    soap_head.ns2__ResHeader = &soap_head_res;
    soap_head_auth.username = username;
    soap_head_auth.password = password;
    soap_head_auth.token = token;

    soap_init(&soap);
    soap_set_mode(&soap, SOAP_C_UTFSTRING);
    soap.header = &soap_head;
    if(soap_ssl_client_context(&soap, SOAP_SSL_DEFAULT,
		NULL, NULL, "/etc/key.crt", NULL, NULL)) {

	return -1;
    }

    char *performanceData[] = {"impression", "click", "cost"};
    struct _ns1__getProfessionalReportIdRequest id_request = {0};
    struct _ns1__getProfessionalReportIdResponse id_return = {0};
    struct ns1__ReportRequestType _reportRequestType = {0};
    id_request.reportRequestType = &_reportRequestType;

    id_request.reportRequestType->__sizeperformanceData = 3;
    id_request.reportRequestType->performanceData = performanceData;
    id_request.reportRequestType->startDate = date_time;
    id_request.reportRequestType->endDate = date_time;
    id_request.reportRequestType->reportType = 15;

    if(soap_call___ns1__getProfessionalReportId(
		&soap,
		NULL,
		NULL,
		&id_request, 
		&id_return) != 0) {

	if (soap.header 
		&& soap.header->ns2__ResHeader 
		&& soap.header->ns2__ResHeader->failures) {

	    return soap.header->ns2__ResHeader->failures->code;
	}

	return -1;
    }

    if (reportId)
	strcpy(reportId, id_return.reportId);
    soap_destroy(&soap); 
    soap_end(&soap); 
    soap_done(&soap);
    return 0;
}

int request_baidu_report_status(
	char *username,
	char *password,
	char *token,
	char *reportId)
{

    struct soap soap = {0};
    struct SOAP_ENV__Header soap_head = {0};
    struct ns2__AuthHeader soap_head_auth = {0};
    struct ns2__ResHeader soap_head_res = {0};

    soap_head.ns2__AuthHeader = &soap_head_auth;
    soap_head.ns2__ResHeader = &soap_head_res;
    soap_head_auth.username = username;
    soap_head_auth.password = password;
    soap_head_auth.token = token;

    soap_init(&soap);
    soap_set_mode(&soap, SOAP_C_UTFSTRING);
    soap.header = &soap_head;
    if(soap_ssl_client_context(&soap, SOAP_SSL_DEFAULT,
		NULL, NULL, "/etc/key.crt", NULL, NULL)) {

	return -1; 
    }

    struct _ns1__getReportStateRequest status_request = {0};
    struct _ns1__getReportStateResponse status_return = {0};
    status_request.reportId = reportId;

    if(soap_call___ns1__getReportState(
		&soap,
		NULL,
		NULL,
		&status_request, 
		&status_return) != 0) {

	if (soap.header 
		&& soap.header->ns2__ResHeader 
		&& soap.header->ns2__ResHeader->failures) {

	    return soap.header->ns2__ResHeader->failures->code;
	}

	return -1;
    }

    int isGenerated = (*status_return.isGenerated);
    soap_destroy(&soap); 
    soap_end(&soap); 
    soap_done(&soap);
    return isGenerated;
}

int request_baidu_report_url(
	char *username,
	char *password,
	char *token,
	char *reportId,
	char *reportFilePath)
{

    struct soap soap = {0};
    struct SOAP_ENV__Header soap_head = {0};
    struct ns2__AuthHeader soap_head_auth = {0};
    struct ns2__ResHeader soap_head_res = {0};

    soap_head.ns2__AuthHeader = &soap_head_auth;
    soap_head.ns2__ResHeader = &soap_head_res;
    soap_head_auth.username = username;
    soap_head_auth.password = password;
    soap_head_auth.token = token;

    soap_init(&soap);
    soap_set_mode(&soap, SOAP_C_UTFSTRING);
    soap.header = &soap_head;
    if(soap_ssl_client_context(&soap, SOAP_SSL_DEFAULT,
		NULL, NULL, "/etc/key.crt", NULL, NULL)) {

	return -1;
    }

    struct _ns1__getReportFileUrlRequest url_request = {0};
    struct _ns1__getReportFileUrlResponse url_return = {0};
    url_request.reportId = reportId;

    if(soap_call___ns1__getReportFileUrl(
		&soap,
		NULL,
		NULL,
		&url_request,
		&url_return) != 0) {

	if (soap.header
		&& soap.header->ns2__ResHeader
		&& soap.header->ns2__ResHeader->failures) {

	    return soap.header->ns2__ResHeader->failures->code;
	}

	return -1;
    }

    strcpy(reportFilePath, url_return.reportFilePath);
    soap_destroy(&soap);
    soap_end(&soap); 
    soap_done(&soap);
    return 0;
}

int baidu_download_report_search(
	char *date_str,
	char *username, 
	char *password, 
	char *token, 
	char *path)
{

    int code = -1;
    char reportId[1024] = {0};
    char reportFilePath[1024] = {0};

    struct tm date_tm = {0};
    strptime(date_str, "%Y-%m-%d", &date_tm);
    time_t date_time = mktime(&date_tm) - 57600 + 86400;

    code = request_baidu_report_search(
	    date_time,
	    username,
	    password,
	    token,
	    reportId);

    if (code != 0)
	return code;

    while(1) {

	sleep(10);
	code = request_baidu_report_status(
		username,
		password,
		token,
		reportId);

	switch (code) {

	    case 1:
	    case 2:
		break;

	    case 3:
		code = request_baidu_report_url(
			username,
			password,
			token,
			reportId,
			reportFilePath);
		if (code != 0)
		    return code;

		return http_download_file(reportFilePath, path);

	    case -1:
		return -1;

	    default:
		return code;
	}
    }

    return -1;
}

int baidu_download_report_pair(
	char *date_str,
	char *username, 
	char *password, 
	char *token, 
	char *path)
{

    int code = -1;
    char reportId[1024] = {0};
    char reportFilePath[1024] = {0};

    struct tm date_tm = {0};
    strptime(date_str, "%Y-%m-%d", &date_tm);
    time_t date_time = mktime(&date_tm) - 57600 + 86400;

    code = request_baidu_report_pair(
	    date_time,
	    username,
	    password,
	    token,
	    reportId);

    if (code != 0)
	return code;

    while(1) {

	sleep(10);
	code = request_baidu_report_status(
		username,
		password,
		token,
		reportId);

	switch (code) {

	    case 1:
	    case 2:
		break;

	    case 3:
		code = request_baidu_report_url(
			username,
			password,
			token,
			reportId,
			reportFilePath);

		if (code != 0)
		    return code;

		return http_download_file(reportFilePath, path);

	    case -1:
		return -1;

	    default:
		return code;
	}
    }

    return -1;
}


