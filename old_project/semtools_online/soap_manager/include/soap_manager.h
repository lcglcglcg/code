
#ifndef	__SOAP_MANAGER_H__
#define	__SOAP_MANAGER_H__

typedef	void(soap_manager_callback)(
	int sem_id,
	int baidu_id,
	char *account,
	char *password,
	char *token,
	char *web_name,
	char *domain,
	char *company_name,
	char *contact_str,
	char *contact_tel,
	int stop_marker,
	void *arg);

extern int soap_manager_login(char *account, char *password, int *sem_id, int *code);
extern int soap_manager_pass_update(char *account, char *password, char *new_password, int *code);
extern int soap_manager_baidu_get_all(soap_manager_callback *sem_call, void *sem_arg, int *code);
extern int soap_manager_baidu_get(int sem_id, soap_manager_callback *sem_call, void *sem_arg, int *code);
extern int soap_manager_baidu_update(
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
	int *code);
extern int soap_manager_baidu_add(
	int sem_id,
	char *account, 
	char *password, 
	char *domain, 
	char *token,
	char *web_name,
	char *company_name,
	char *contact_str,
	char *contact_tel,
	int *code);
extern int soap_manager_baidu_del(
	int sem_id,
	int baidu_id,
	int *code);

#endif



