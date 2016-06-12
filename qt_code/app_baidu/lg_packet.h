#ifndef	__LG_PACKET_H__
#define	__LG_PACKET_H__
#define lg_network_t void

#ifdef __cplusplus
extern "C" { 
#endif

#define	PACKET_UINT int
#define	PACKET_CHAR char
#define	PACKET_DATA void
#define	PACKET_BUFF_SIZE 40960

#define	PACKET_SUCCESS			50001
#define	PACKET_COMPLETE			50002
#define	PACKET_ERROR			50003

    /* app packet */

#define PACKET_APP_HEARTBEAT		10001
#define PACKET_APP_HEARTBEAT_RET	10002

#define	PACKET_APP_LOGIN		10003
#define	PACKET_APP_LOGIN_RET		10004

#define	PACKET_APP_KEYWORD_INSERT	10005
#define	PACKET_APP_KEYWORD_INSERT_RET	10006

#define	PACKET_APP_KEYWORD_UPDATE	10007
#define	PACKET_APP_KEYWORD_UPDATE_RET	10008

#define	PACKET_APP_KEYWORD_DELETE	10009
#define	PACKET_APP_KEYWORD_DELETE_RET	10010

#define	PACKET_APP_KEYWORD_DOWNLOAD	10011
#define	PACKET_APP_KEYWORD_DOWNLOAD_RET	10012


    //报文头
    typedef struct {

	PACKET_UINT msgid;
	PACKET_UINT size;

    }PACKET_HEAD;

    //app登录
    typedef struct {

	PACKET_HEAD head;
	PACKET_UINT account_size;
	PACKET_UINT password_size;
	PACKET_UINT data;//占位

    }PACKET_APP_LOGIN_T;

    typedef struct {

	PACKET_HEAD head;
	PACKET_UINT retval;

    }PACKET_APP_LOGIN_RET_T;

    //app心跳
    typedef struct {

	PACKET_HEAD head;
	PACKET_UINT account_size;
	PACKET_UINT data;//占位

    }PACKET_APP_HEARTBEAT_T;

    typedef struct {

	PACKET_HEAD head;
	PACKET_UINT retval;

    }PACKET_APP_HEARTBEAT_RET_T;

    //app关键词插入
    typedef struct {

	PACKET_HEAD head;
	PACKET_UINT keyword_size;
	PACKET_UINT url_size;
	PACKET_UINT account_size;
	PACKET_UINT click_time[24];
	PACKET_UINT data;//占位

    }PACKET_APP_KEYWORD_INSERT_T;

    typedef struct {

	PACKET_HEAD head;
	PACKET_UINT retval;

    }PACKET_APP_KEYWORD_INSERT_RET_T;

    //app关键词修改
    typedef struct {

	PACKET_HEAD head;
	PACKET_UINT keyword_size;
	PACKET_UINT account_size;
	PACKET_UINT click_time[24];
	PACKET_UINT data;//占位

    }PACKET_APP_KEYWORD_UPDATE_T;

    typedef struct {

	PACKET_HEAD head;
	PACKET_UINT retval;

    }PACKET_APP_KEYWORD_UPDATE_RET_T;

    //app关键词删除
    typedef struct {

	PACKET_HEAD head;
	PACKET_UINT keyword_size;
	PACKET_UINT account_size;
	PACKET_UINT data;//占位

    }PACKET_APP_KEYWORD_DELETE_T;

    typedef struct {

	PACKET_HEAD head;
	PACKET_UINT keyword_size;
	PACKET_UINT data;//占位

    }PACKET_APP_KEYWORD_DELETE_RET_T;

    //app关键词下载
    typedef struct {

	PACKET_HEAD head;
	PACKET_UINT index;//索引
	PACKET_UINT account_size;
	PACKET_UINT click_time[24];
	PACKET_UINT data;//占位

    }PACKET_APP_KEYWORD_DOWNLOAD_T;

    typedef struct {

	PACKET_HEAD head;

	PACKET_UINT retval;
	PACKET_UINT index;//索引
	PACKET_UINT keyword_size;
	PACKET_UINT url_size;
	PACKET_UINT click_time[24];
	PACKET_UINT data;//占位

    }PACKET_APP_KEYWORD_DOWNLOAD_RET_T;

    extern void lg_packet_app_analysis(lg_network_t *network, char *buffer, size_t size, void *arg);
    extern void lg_packet_app_heartbeat(lg_network_t *network, char *buffer, size_t size, void *arg);
    extern void lg_packet_app_login(lg_network_t *network, char *buffer, size_t size, void *arg);
    extern void lg_packet_app_keyword_insert(lg_network_t *network, char *buffer, size_t size, void *arg);
    extern void lg_packet_app_keyword_update(lg_network_t *network, char *buffer, size_t size, void *arg);
    extern void lg_packet_app_keyword_delete(lg_network_t *network, char *buffer, size_t size, void *arg);
    extern void lg_packet_app_keyword_download(lg_network_t *network, char *buffer, size_t size, void *arg);


    /* broiler packet */
#define	PACKET_BROILER_KEYWORD		20001
#define	PACKET_BROILER_KEYWORD_RET	20002

#define	PACKET_BROILER_CLICK		20003
#define	PACKET_BROILER_CLICK_RET	20004

#define	PACKET_BROILER_VERSION		20005
#define	PACKET_BROILER_VERSION_RET	20006


    //下载关键词
    typedef struct {

	PACKET_HEAD head;
	PACKET_UINT index;//请求索引

    }PACKET_BROILER_KEYWORD_T;

    typedef struct {

	PACKET_HEAD head;
	PACKET_UINT retval;//返回值

	PACKET_UINT index;//索引
	PACKET_UINT keyword_size;
	PACKET_UINT url_size;
	PACKET_UINT data;//占位

    }PACKET_BROILER_KEYWORD_RET_T;

    //请求点击
    typedef struct {

	PACKET_HEAD head;

    }PACKET_BROILER_CLICK_T;

    typedef struct {

	PACKET_HEAD head;
	PACKET_UINT click_total;//需要点击的总数
	PACKET_UINT click_sleep;//点击延时
	PACKET_UINT data;//int data[click_total]取数组中的索引点击

    }PACKET_BROILER_CLICK_RET_T;


    //获取服务器版本
    typedef struct {

	PACKET_HEAD head;

    }PACKET_BROILER_VERSION_T;

    typedef struct {

	PACKET_HEAD head;
	PACKET_UINT version;//服务器版本

    }PACKET_BROILER_VERSION_RET_T;


    //解析报文类型
    extern void lg_packet_broiler_analysis(lg_network_t *network, char *buffer, size_t size, void *arg);

    //下载关键词
    extern void lg_packet_broiler_keyword(lg_network_t *network, char *buffer, size_t size, void *arg);

    //获取服务器版本
    extern void lg_packet_broiler_version(lg_network_t *network, char *buffer, size_t size, void *arg);

    //请求点击
    extern void lg_packet_broiler_click(lg_network_t *network, char *buffer, size_t size, void *arg);



#ifdef __cplusplus
}
#endif
#endif//__LG_PACKET_H__



