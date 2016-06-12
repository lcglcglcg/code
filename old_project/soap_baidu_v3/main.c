
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include "soapH.h"

extern char *strptime(const char *s, const char *format, struct tm *tm);

int baidu_report_download(char *url, char *path)
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

    curl_easy_perform(curl_handle);
    curl_easy_cleanup(curl_handle);
    curl_global_cleanup();

    fclose(fp);
    return 0;
}

int baidu_report_pair(char *user, char *pass, char *token, char *date, int device, char *report_id)
{

    struct soap soap = {0};
    struct SOAP_ENV__Header soap_head = {0};
    struct ns2__AuthHeader AuthHeader = {0};
    struct ns2__ResHeader ResHeader = {0};

    soap_init(&soap);
    soap.header = &soap_head;
    soap_set_mode(&soap, SOAP_C_UTFSTRING);
    soap_head.ns2__AuthHeader = &AuthHeader;
    soap_head.ns2__ResHeader = &ResHeader;
    AuthHeader.username = user;
    AuthHeader.password = pass;
    AuthHeader.token = token;


    if(soap_ssl_client_context(&soap, SOAP_SSL_DEFAULT,
		NULL, NULL, "/etc/key.crt", NULL, NULL)) {

	return -1;
    }

    struct _ns1__getRealTimePairDataRequest getRealTimePairDataReques = {0};
    struct _ns1__getRealTimePairDataResponse getRealTimePairDataResponse = {0};

    int unitOfTime = 8;
    int statRange = 11;
    int levelOfDetails = 12;
    char *performanceData[] = {"impression", "click"};
    struct ns1__RealTimeRequestType RealTimeRequestType = {0};

    getRealTimePairDataReques.realTimePairRequestTypes = &RealTimeRequestType;
    RealTimeRequestType.__sizeperformanceData = 2;
    RealTimeRequestType.performanceData = performanceData;
    RealTimeRequestType.reportType = 15; 
    RealTimeRequestType.levelOfDetails = &levelOfDetails;
    RealTimeRequestType.statRange = &statRange;
    RealTimeRequestType.device = &device;
    RealTimeRequestType.unitOfTime = &unitOfTime;
    
    struct tm date_tm = {0};
    strptime(date, "%Y-%m-%d", &date_tm);
    time_t date_time = mktime(&date_tm) - 57600 + 86400;
    RealTimeRequestType.startDate = date_time;
    RealTimeRequestType.endDate = date_time + 86400;

    int ret = soap_call___ns1__getRealTimePairData(
	    &soap,
	    NULL,
	    NULL,
	    &getRealTimePairDataReques,
	    &getRealTimePairDataResponse);

    fprintf(stdout, "ret=%d\n", ret);


    soap_destroy(&soap); 
    soap_end(&soap); 
    soap_done(&soap);
    return 0;
}

int main(int argc, char *argv[])
{


    char user[] = "sgszzx";
    char pass[] = "Elikeme0817";
    char token[] = "da2590c34de0e439d7ab0c783a85e223";
    char date[] = "2013-08-01";
    char report_id[1024];
    baidu_report_pair(user, pass, token, date, 1, report_id);


    return 0;
}

