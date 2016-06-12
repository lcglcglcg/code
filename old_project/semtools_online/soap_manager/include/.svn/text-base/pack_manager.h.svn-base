
#ifndef	__PACK_MANAGER_H__
#define	__PACK_MANAGER_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <signal.h>
#include <sys/wait.h>
#include <time.h>
#include <openssl/md5.h>
#include <pthread.h>

#define PACKET_SOAP_MANAGER_LOGIN 1000
#define PACKET_SOAP_MANAGER_SEM_PASS 1001
#define PACKET_SOAP_MANAGER_BAIDU_ADD 1002
#define PACKET_SOAP_MANAGER_BAIDU_DEL 1003
#define PACKET_SOAP_MANAGER_BAIDU_MOD 1004
#define PACKET_SOAP_MANAGER_BAIDU_GET 1005

typedef	struct {

    int msid;
    char sem_account[128];
    char sem_password[128];

}packet_login_t;

typedef struct {

    int msid;
    char sem_account[128];
    char sem_password[128];
    char new_sem_password[128];

}packet_sem_pass_update_t;

typedef struct {

    int msid;
    int sem_id;
    char baidu_account[128];
    char baidu_password[128];
    char baidu_domain[256];
    char baidu_token[64];
    char baidu_web_name[256];
    char baidu_company_name[256];
    char baidu_contact_str[256];
    char baidu_contact_tel[256];

}packet_baidu_add_t;

typedef struct {

    int msid;
    int sem_id;
    int baidu_id;

}packet_baidu_del_t;

typedef struct {

    int msid;
    int sem_id;
    int baidu_id;
    char baidu_account[128];
    char baidu_password[128];
    char baidu_domain[256];
    char baidu_token[64];
    char baidu_web_name[256];
    char baidu_company_name[256];
    char baidu_contact_str[256];
    char baidu_contact_tel[256];

}packet_baidu_mod_t;

typedef struct {

    int msid;
    int sem_id;
    int baidu_id;

}packet_baidu_get_t;

typedef struct {

    int sem_id;
    int baidu_id;
    char baidu_account[128];
    char baidu_password[128];
    char baidu_domain[256];
    char baidu_token[64];
    char baidu_web_name[256];
    char baidu_company_name[256];
    char baidu_contact_str[256];
    char baidu_contact_tel[256];
    int baidu_stop_marker;

}packet_baidu_get_data_t;


typedef struct {

    int ret;
    int sem_id;
    int code;

}packet_return_t;

#endif


