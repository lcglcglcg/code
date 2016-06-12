
#include "soapH.h"
#include "soap_manager.h"

int soap_manager_login(char *account, char *password, int *sem_id, int *code)
{

    struct soap soap = {0};

    soap_init(&soap);
    soap_set_mode(&soap, SOAP_C_UTFSTRING);
    if(soap_ssl_client_context(&soap, SOAP_SSL_DEFAULT,
		NULL, NULL, NULL, NULL, NULL)) {

	return -1;
    }

    struct ns2__authLogin authLogin = {0};
    struct ns2__authLoginResponse authLoginResponse = {0};
    authLogin.loginName = account;
    authLogin.password = password;

    int retval = soap_call___ns1__authLogin(&soap,
	    NULL,
	    NULL,
	    &authLogin,
	    &authLoginResponse);

    if (retval == 0) {

	*code = *authLoginResponse.CompanyType->code;
	if (authLoginResponse.CompanyType)
	    if (authLoginResponse.CompanyType->companyId) {

		*sem_id = *authLoginResponse.CompanyType->companyId;
	    }
    }

    soap_destroy(&soap); 
    soap_end(&soap); 
    soap_done(&soap);
    soap_closesock(&soap);
    return retval;
}

int soap_manager_pass_update(char *account, char *password, char *new_password, int *code)
{

    struct soap soap = {0};

    soap_init(&soap);
    soap_set_mode(&soap, SOAP_C_UTFSTRING);
    if(soap_ssl_client_context(&soap, SOAP_SSL_DEFAULT,
		NULL, NULL, NULL, NULL, NULL)) {

	return -1;
    }

    struct ns2__updateLoginPassword LoginPassword = {0};
    struct ns2__updateLoginPasswordResponse LoginPasswordResponse = {0};

    LoginPassword.loginName = account;
    LoginPassword.password = password;
    LoginPassword.newPassword = new_password;

    int retval = soap_call___ns1__updateLoginPassword(
	    &soap,
	    NULL,
	    NULL,
	    &LoginPassword,
	    &LoginPasswordResponse);

    if (retval != 0) {

	soap_closesock(&soap);
	return retval;
    }

    struct ns2__OperateType *operate_type = NULL;
    operate_type = LoginPasswordResponse.OperateType;
    if (operate_type) {

	if (operate_type->code)
	    *code = *operate_type->code;
    }

    soap_closesock(&soap);
    return retval;
}

int soap_manager_baidu_get_all(soap_manager_callback *sem_call, void *sem_arg, int *code)
{

    struct soap soap = {0};

    soap_init(&soap);
    soap_set_mode(&soap, SOAP_C_UTFSTRING);
    if(soap_ssl_client_context(&soap, SOAP_SSL_DEFAULT,
		NULL, NULL, NULL, NULL, NULL)) {

	return -1;
    }

    struct ns2__queryAllAccount queryAllAccount;
    struct ns2__queryAllAccountResponse queryAllAccountResponse = {0};
    int retval = soap_call___ns1__queryAllAccount(
	    &soap,
	    NULL,
	    NULL,
	    &queryAllAccount,
	    &queryAllAccountResponse);

    if (retval != 0) {

	soap_closesock(&soap);
	return retval;
    }

    struct ns2__AccountItemType *account_info = NULL;
    account_info = queryAllAccountResponse.AccountItemType;
    if (!account_info) {

	soap_closesock(&soap);
	return -1;
    }

    int size = account_info->__sizeaccounts;
    struct ns2__BaiduAccountInfo *baidu_info = account_info->accounts;

    if (baidu_info) {

	if (code)
	    *code = *account_info->code;

	if (sem_call) {

	    int i;
	    for (i = 0; i < size; i++) {

		int sem_id = *baidu_info->companyId;
		int baidu_id = *baidu_info->accountId;
		char *account = baidu_info->accountName;
		char *password = baidu_info->password;
		char *token = baidu_info->apiToken;
		char *web_name = baidu_info->websiteName;
		char *domain = baidu_info->domain;
		char *company_name = baidu_info->compName;
		char *contact_str = baidu_info->pcContact;
		char *contact_tel = baidu_info->pcTelephone;
		int stop_marker = *baidu_info->status;
		sem_call(sem_id,
			baidu_id,
			account,
			password,
			token,
			web_name,
			domain,
			company_name,
			contact_str,
			contact_tel,
			stop_marker,
			sem_arg);

		baidu_info++;
	    }
	}
    }

    soap_closesock(&soap);
    return retval;
}

int soap_manager_baidu_get(int sem_id, soap_manager_callback *sem_call, void *sem_arg, int *code)
{

    struct soap soap = {0};

    soap_init(&soap);
    soap_set_mode(&soap, SOAP_C_UTFSTRING);
    if(soap_ssl_client_context(&soap, SOAP_SSL_DEFAULT,
		NULL, NULL, NULL, NULL, NULL)) {

	return -1;
    }

    LONG64 sem_id_temp = sem_id;
    struct ns2__queryAccount queryAccount = {0};
    struct ns2__queryAccountResponse queryAccountResponse = {0};
    queryAccount.companyId = &sem_id_temp;

    int retval = soap_call___ns1__queryAccount(
	    &soap,
	    NULL,
	    NULL,
	    &queryAccount, 
	    &queryAccountResponse);

    if (retval != 0) {

	soap_closesock(&soap);
	return retval;
    }

    struct ns2__AccountItemType *account_info = NULL;
    account_info = queryAccountResponse.AccountItemType;
    if (!account_info) {

	soap_closesock(&soap);
	return -1;
    }

    int size = account_info->__sizeaccounts;
    struct ns2__BaiduAccountInfo *baidu_info = NULL;
    baidu_info = account_info->accounts;

    if (baidu_info) {

	if (code)
	    *code = *account_info->code;

	if (sem_call) {

	    int i;
	    for (i = 0; i < size; i++) {

		int sem_id = *baidu_info->companyId;
		int baidu_id = *baidu_info->accountId;
		char *account = baidu_info->accountName;
		char *password = baidu_info->password;
		char *token = baidu_info->apiToken;
		char *web_name = baidu_info->websiteName;
		char *domain = baidu_info->domain;
		char *company_name = baidu_info->compName;
		char *contact_str = baidu_info->pcContact;
		char *contact_tel = baidu_info->pcTelephone;
		int stop_marker = *baidu_info->status;
		sem_call(sem_id,
			baidu_id,
			account,
			password,
			token,
			web_name,
			domain,
			company_name,
			contact_str,
			contact_tel,
			stop_marker,
			sem_arg);

		baidu_info++;
	    }
	}
    }

    soap_closesock(&soap);
    return retval;
}

int soap_manager_baidu_update(
	int sem_id,
	int baidu_id,
	char *account, 
	char *password, 
	char *domain, 
	char *token,
	char *web_name,
	char *company_name,
	char *contact_str,
	char *tel_str,
	int *code)
{

    struct soap soap = {0};

    soap_init(&soap);
    soap_set_mode(&soap, SOAP_C_UTFSTRING);
    if(soap_ssl_client_context(&soap, SOAP_SSL_DEFAULT,
		NULL, NULL, NULL, NULL, NULL)) {

	return -1;
    }

    LONG64 sem_id_temp = sem_id;
    LONG64 baidu_id_temp = baidu_id;

    struct ns2__BaiduAccountInfo baidu_info = {0};
    baidu_info.accountId = &baidu_id_temp;
    baidu_info.accountName = account;
    baidu_info.password = password;
    baidu_info.domain = domain;
    baidu_info.apiToken = token;
    baidu_info.websiteName = web_name;
    baidu_info.compName =company_name;
    baidu_info.pcContact = contact_str;
    baidu_info.pcTelephone = tel_str;
    baidu_info.companyId = &sem_id_temp;

    struct ns2__updateAccount updateAccount = {0};
    struct ns2__updateAccountResponse updateAccountResponse;
    updateAccount.BaiduAccountInfo = &baidu_info;

    int retval = soap_call___ns1__updateAccount(
	    &soap,
	    NULL,
	    NULL,
	    &updateAccount,
	    &updateAccountResponse);

    if (retval != 0) {

	soap_closesock(&soap);
	return retval;
    }

    struct ns2__OperateType *operate_type = NULL;
    operate_type = updateAccountResponse.OperateType;
    if (operate_type) {

	if (operate_type->code)
	    *code = *operate_type->code;
    }

    soap_closesock(&soap);
    return retval;
}

int soap_manager_baidu_add(
	int sem_id,
	char *account, 
	char *password, 
	char *domain, 
	char *token,
	char *web_name,
	char *company_name,
	char *contact_str,
	char *contact_tel,
	int *code)
{

    struct soap soap = {0};
    soap_init(&soap);
    soap_set_mode(&soap, SOAP_C_UTFSTRING);
    if(soap_ssl_client_context(&soap, SOAP_SSL_DEFAULT,
		NULL, NULL, NULL, NULL, NULL)) {

	return -1;
    }

    LONG64 companyId = sem_id;
    struct ns2__BaiduAccountInfo baidu_info = {0};
    baidu_info.accountName = account;
    baidu_info.password = password;
    baidu_info.domain = domain;
    baidu_info.apiToken = token;
    baidu_info.websiteName = web_name;
    baidu_info.compName =company_name;
    baidu_info.pcContact = contact_str;
    baidu_info.pcTelephone = contact_tel;
    baidu_info.companyId = &companyId;

    struct ns2__addAccount addAccount = {0};
    struct ns2__addAccountResponse addAccountResponse = {0};
    addAccount.BaiduAccountInfo = &baidu_info;


    int retval = soap_call___ns1__addAccount(
	    &soap,
	    NULL,
	    NULL,
	    &addAccount,
	    &addAccountResponse);

    if (retval != 0) {

	soap_closesock(&soap);
	return retval;
    }

    struct ns2__OperateType *operate_type = NULL;
    operate_type = addAccountResponse.OperateType;
    if (operate_type) {

	if (operate_type->code)
	    *code = *operate_type->code;
    }

    soap_closesock(&soap);
    return retval;
}

int soap_manager_baidu_del(
	int sem_id,
	int baidu_id,
	int *code)
{

    struct soap soap = {0};
    soap_init(&soap);
    soap_set_mode(&soap, SOAP_C_UTFSTRING);
    if(soap_ssl_client_context(&soap, SOAP_SSL_DEFAULT,
		NULL, NULL, NULL, NULL, NULL)) {

	return -1;
    }

    struct ns2__deleteAccount deleteAccount = {0};
    struct ns2__deleteAccountResponse deleteAccountResponse = {0};

    LONG64 sem_id_temp = sem_id;
    LONG64 baidu_id_temp = baidu_id;

    deleteAccount.companyId = &sem_id_temp;
    deleteAccount.accountId = &baidu_id_temp;

    int retval = soap_call___ns1__deleteAccount(
	    &soap,
	    NULL,
	    NULL,
	    &deleteAccount,
	    &deleteAccountResponse);

    if (retval != 0) {

	soap_closesock(&soap);
	return retval;
    }

    *code = deleteAccountResponse.code;
    soap_closesock(&soap);
    return retval;
}

