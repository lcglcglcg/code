#ifndef	__BAIDU_SERVICE_H__
#define	__BAIDU_SERVICE_H__

#include <windows.h>

typedef void (BAIDU_CALLBACK)(void);

extern int baidu_init(TCHAR *service_name, BAIDU_CALLBACK *callback);
extern int baidu_install(LPCTSTR service_name, LPCTSTR program_path, LPCTSTR dependent);
extern int baidu_uninstall(LPCTSTR service_name);
extern int baidu_start(LPCTSTR service_name);
extern int baidu_stop(LPCTSTR service_name);
extern int baidu_check_running(LPCTSTR service_name);
extern int baidu_check_install(LPCTSTR service_name);

#endif
