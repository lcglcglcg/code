
#include "soapH.h"

typedef	void(soap_baidu_account_info_call)(char **domain, int domain_size, void *arg);

int soap_baidu_get_account_info(
	char *account, 
	char *password, 
	char *token, 
	soap_baidu_account_info_call *function_call,
	void *arg)
{
    struct soap soap = {0};
    struct SOAP_ENV__Header soap_head = {0};
    struct ns2__AuthHeader soap_head_auth = {0};
    struct ns2__ResHeader soap_head_res = {0};

    soap_head.ns2__AuthHeader = &soap_head_auth;
    soap_head.ns2__ResHeader = &soap_head_res;
    soap_head_auth.username = account;
    soap_head_auth.password = password;
    soap_head_auth.token = token;

    soap_init(&soap);
    soap_set_mode(&soap, SOAP_C_UTFSTRING);
    soap.header = &soap_head;
    if(soap_ssl_client_context(&soap, SOAP_SSL_DEFAULT,
		NULL, NULL, "/etc/key.crt", NULL, NULL)) {

	return -1;
    }

    struct _ns1__getAccountInfoRequest getAccountInfoRequest = {0};
    struct _ns1__getAccountInfoResponse getAccountInfoResponse = {0};
    getAccountInfoRequest.type = 1;

    if(soap_call___ns1__getAccountInfo(
		&soap,
		NULL,
		NULL,
		&getAccountInfoRequest,
		&getAccountInfoResponse) != 0) {

	if (soap.header 
		&& soap.header->ns2__ResHeader 
		&& soap.header->ns2__ResHeader->failures) {

	    return soap.header->ns2__ResHeader->failures->code;
	}

	return -1;
    }

    struct ns1__AccountInfoType *type = NULL;
    if ((type = getAccountInfoResponse.accountInfoType))
	if (function_call)
	    function_call(type->openDomains, type->__sizeopenDomains, arg);

    soap_destroy(&soap); 
    soap_end(&soap); 
    soap_done(&soap);
    return 0;
}

