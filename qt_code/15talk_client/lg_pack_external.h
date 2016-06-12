#ifndef __LG_PACK_EXTERNAL_H__
#define __LG_PACK_EXTERNAL_H__

#ifdef __cplusplus
extern "C" {
#endif

    typedef struct {

        unsigned int    msgid;
        unsigned int    time;
        unsigned int    size;

    }PACK_HEAD;//报文头

    typedef struct {

        PACK_HEAD       head;
        int             hospital;//医院ID
        char            user[64];//登陆用户名(昵称)
        char            pass[64];//密码(密文)
        int             status;//客服状态:1在线,2不在线,3繁忙

    }PACK_APP_LOGIN;//app用户登录

    typedef struct {

        PACK_HEAD       head;
        int             hospital;
        char            account[64];//唯一ID

        char            ftp_host[20];//ftp主机地址
        char            ftp_user[20];//ftp用户名
        char            ftp_pass[20];//ftp密码

        char            worker_id[20];//工号
        int             workage;//年龄

    }PACK_APP_LOGIN_RET;//app用户登录返回

    typedef struct {

        PACK_HEAD       head;
        int             hospital;//医院ID
        char            account[64];//唯一ID

    }PACK_APP_LOGOUT;//app离开

    typedef struct {

        PACK_HEAD       head;
        int             retval;

    }PACK_APP_LOGOUT_RET;//app离开返回

#ifdef __cplusplus
}
#endif
#endif // __LG_PACK_EXTERNAL_H__
