
#ifndef	__AD_SYSTEM_SESSION_H__
#define	__AD_SYSTEM_SESSION_H__

#ifdef __cplusplus
extern "C" { 
#endif

#include "lg_list.h"
#include "lg_rbtree.h"
#include "img_libgd_captcha.h"

    typedef struct {

	lg_rbtree_node tree;

	char session_id[64];
	char account_id[64];
	char login_addr[64];
	
	img_libgd_captcha_t img;

	//lg_list_t list; //备用
	time_t time;//超时

    } ad_system_session_t;

    extern ad_system_session_t *_ad_system_session_add(char *session_id);
    extern ad_system_session_t *ad_system_session_parse(char *buffer);

#ifdef __cplusplus
}
#endif

#endif


