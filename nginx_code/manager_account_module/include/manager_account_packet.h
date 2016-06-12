
#ifndef __MANAGER_ACCOUNT_PACKET_H__
#define __MANAGER_ACCOUNT_PACKET_H__

#define	PACKET_CODE_OK					0//成功
#define	PACKET_CODE_USER_LOGIN_CHECK_OK			1000//用户已登录
#define	PACKET_CODE_USER_LOGIN_CHECK_ERR		1001//用户未登录
#define	PACKET_CODE_USER_LOGIN_ERR			1002//用户名/密码/验证码 错误
#define	PACKET_CODE_USER_FROZEN				1003//用户冻结

#define	PACKET_CODE_ERR					9999//服务器内部错误
#define	PACKET_CODE_FORMAT_ERR				9998//报文格式错误
#define	PACKET_CODE_PARAMETER_ERR			9997//报文参数错误
#define	PACKET_CODE_NOT_DATA				9996//没有数据
#define PACKET_CODE_NOT_KEY                             9995//没有这个ID
#define PACKET_CODE_REPEAT_ERR				9994//重复绑定
#define PACKET_CODE_UPSTREAM_ERR			9993//上游服务器无相应

#define PACKET_WANGMENG_LOGIN_URL "http://wangmeng.haomeit.com/action?server={\"PACKET_TYPE\":\"MA_LOGIN\",\"DATA\":\"%s\"}"

#ifdef __cplusplus
extern "C" { 
#endif


#ifdef __cplusplus
}
#endif

#endif




