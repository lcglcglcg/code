#include "service.h"

static BAIDU_CALLBACK *baidu_run = NULL;

int baidu_check_running(LPCTSTR service_name)
{

    SC_HANDLE sc_manager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
    if (sc_manager == NULL) {

	//MessageBox(NULL, "OpenSCManager error!", "baidu_Running", MB_OK);
	return FALSE;
    }

    SC_HANDLE sc_service = OpenService(sc_manager, service_name, SERVICE_ALL_ACCESS);
    if (sc_service == NULL) {

	//MessageBox(NULL, "OpenService error!", "baidu_Running", MB_OK);
	CloseServiceHandle(sc_manager);
	return FALSE;
    }    

    SERVICE_STATUS status;
    QueryServiceStatus(sc_service, &status);
    if(status.dwCurrentState == SERVICE_RUNNING) { 

	CloseServiceHandle(sc_service);
	CloseServiceHandle(sc_manager);
	return TRUE;
    }

    CloseServiceHandle(sc_service);
    CloseServiceHandle(sc_manager);
    return FALSE;
}

int baidu_check_install(LPCTSTR service_name)
{

    SC_HANDLE sc_manager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
    if (sc_manager == NULL) {

	//MessageBox(NULL, "OpenSCManager error!", "baidu_Running", MB_OK);
	return FALSE;
    }

    SC_HANDLE sc_service = OpenService(sc_manager, service_name, SERVICE_ALL_ACCESS);
    if (sc_service == NULL) {

	//MessageBox(NULL, "OpenService error!", "baidu_Running", MB_OK);
	CloseServiceHandle(sc_manager);
	return FALSE;
    }    

    CloseServiceHandle(sc_service);
    CloseServiceHandle(sc_manager);
    return TRUE;
}

int baidu_install(LPCTSTR service_name, LPCTSTR program_path, LPCTSTR dependent)
{

    SC_HANDLE sc_manager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
    if (sc_manager == NULL) {

	//MessageBox(NULL, "OpenSCManager error!", "baidu_Install", MB_OK);
	return FALSE;
    }

    if (program_path == NULL) {

	TCHAR self_path[MAX_PATH] = {0};
	GetModuleFileName(NULL, self_path, MAX_PATH);
	program_path = self_path;
    }

    SC_HANDLE sc_service = CreateService(sc_manager,
	    service_name,
	    service_name,
	    SERVICE_ALL_ACCESS,
	    SERVICE_WIN32_OWN_PROCESS | SERVICE_INTERACTIVE_PROCESS,
	    //SERVICE_WIN32_OWN_PROCESS,
	    SERVICE_AUTO_START,
	    SERVICE_ERROR_NORMAL,
	    program_path,
	    NULL,
	    NULL,
	    NULL,//依赖
	    NULL,
	    NULL);

    if (sc_service == NULL) {

	CloseServiceHandle(sc_manager);
	//MessageBox(NULL, "CreateService error!", "baidu_Install", MB_OK);
	return FALSE;
    }

    SERVICE_DESCRIPTION description;
    description.lpDescription = (LPTSTR)service_name;
    ChangeServiceConfig2(sc_service, SERVICE_CONFIG_DESCRIPTION, &description);

    CloseServiceHandle(sc_service);
    CloseServiceHandle(sc_manager);
    return TRUE;
}

int baidu_uninstall(LPCTSTR service_name)
{

    SC_HANDLE sc_manager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
    if (sc_manager == NULL) {

	//MessageBox(NULL, "OpenSCManager error!", "baidu_Uninstall", MB_OK);
	return FALSE;
    }

    SC_HANDLE sc_service = OpenService(sc_manager, service_name, SERVICE_STOP | DELETE);
    if (sc_service == NULL) {

	CloseServiceHandle(sc_manager);
	//MessageBox(NULL, "OpenService error!", "baidu_Uninstall", MB_OK);
	return FALSE;
    }    

    //del service
    BOOL retval = DeleteService(sc_service);
    CloseServiceHandle(sc_service);
    CloseServiceHandle(sc_manager);

    if (retval)
	return TRUE;

    return FALSE;
}

int baidu_start(LPCTSTR service_name)
{

    SERVICE_STATUS status;

    SC_HANDLE sc_manager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS); 
    if (sc_manager == NULL) {

	//MessageBox(NULL, "OpenSCManager error!", "baidu_Start", MB_OK);
	return FALSE;
    }

    SC_HANDLE sc_service = OpenService(sc_manager, service_name, SERVICE_START | SERVICE_STOP | SERVICE_QUERY_STATUS);
    if (sc_service == NULL) {

	//MessageBox(NULL, "OpenService error!", "baidu_Start", MB_OK);
	CloseServiceHandle(sc_manager);
	return FALSE;
    }    

    if (QueryServiceStatus(sc_service, &status) == FALSE) {

	//MessageBox(NULL, "QueryServiceStatus = FALSE", "baidu_Start", MB_OK);
	CloseServiceHandle(sc_service);
	CloseServiceHandle(sc_manager);
	return FALSE;
    }

    if (status.dwCurrentState == SERVICE_RUNNING) {

	//MessageBox(NULL, "SERVICE_STATUS = SERVICE_RUNNING", "baidu_Start", MB_OK);
	CloseServiceHandle(sc_service );
	CloseServiceHandle(sc_manager );
	return FALSE;
    }

    if (StartService(sc_service, 0, NULL ) == FALSE) {

	//MessageBox(NULL, "StartService error!", "baidu_Start", MB_OK);
	CloseServiceHandle(sc_service );
	CloseServiceHandle(sc_manager );
	return FALSE;
    } 

    //MessageBox(NULL, "baidu_Start","baidu_Start", MB_OK);
    CloseServiceHandle(sc_service); 
    CloseServiceHandle(sc_manager);
    return TRUE;
}

int baidu_stop(LPCTSTR service_name)
{

    SERVICE_STATUS status;

    SC_HANDLE sc_manager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
    if (sc_manager == NULL) {

	//MessageBox(NULL, "OpenSCManager error!", "baidu_Start", MB_OK);
	return FALSE;
    }

    SC_HANDLE sc_service = OpenService(sc_manager, service_name, SERVICE_START | SERVICE_STOP | SERVICE_QUERY_STATUS);
    if (sc_service == NULL) {

	//MessageBox(NULL, "OpenService error!", "baidu_Start", MB_OK);
	CloseServiceHandle(sc_manager);
	return FALSE;
    }

    if (QueryServiceStatus(sc_service, &status) == FALSE) {

	//MessageBox(NULL, "QueryServiceStatus = FALSE", "baidu_Start", MB_OK);
	CloseServiceHandle( sc_service);
	CloseServiceHandle( sc_manager);
	return FALSE;
    }

    if (status.dwCurrentState == SERVICE_STOPPED) {

	//MessageBox(NULL, "SERVICE_STATUS = SERVICE_STOPPED", "baidu_Start", MB_OK);
	CloseServiceHandle ( sc_service );
	CloseServiceHandle( sc_manager );
	return FALSE;
    }

    if (ControlService(sc_service, SERVICE_CONTROL_STOP, &status) == FALSE) {

	//MessageBox(NULL, "ControlService error!", "baidu_Start", MB_OK);
	CloseServiceHandle ( sc_service );
	CloseServiceHandle( sc_manager );
	return FALSE;
    }

    //MessageBox(NULL, "baidu_Stop ok!", "baidu_Start", MB_OK);
    CloseServiceHandle (sc_service); 
    CloseServiceHandle(sc_manager);
    return TRUE;
}

int baidu_change_config(LPCTSTR service_name)
{

    SC_HANDLE sc_manager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
    if (sc_manager == NULL) {

	//MessageBox(NULL, "OpenSCManager error!", "baidu_Start", MB_OK);
	return FALSE;
    }

    SC_HANDLE sc_service = OpenService(sc_manager, service_name, SERVICE_START | SERVICE_STOP | SERVICE_QUERY_STATUS);
    if (sc_service == NULL) {

	//MessageBox(NULL, "OpenService error!", "baidu_Start", MB_OK);
	CloseServiceHandle(sc_manager);
	return FALSE;
    }

    SC_ACTION action[3];
    action[0].Delay = 60 * 1000;
    action[0].Type = SC_ACTION_RESTART;
    action[1].Delay = 0;
    action[1].Type = SC_ACTION_NONE;
    action[2].Delay = 0;
    action[2].Type = SC_ACTION_NONE;

    SERVICE_FAILURE_ACTIONS info = {0};
    info.lpRebootMsg = NULL;
    info.dwResetPeriod = 3600 * 24;
    info.cActions = 3; 
    info.lpsaActions = action;
    info.lpCommand = NULL;

    ChangeServiceConfig2(sc_service,                
	    SERVICE_CONFIG_FAILURE_ACTIONS,
	    &info);

    CloseServiceHandle (sc_service); 
    CloseServiceHandle(sc_manager);
    return TRUE;
}

void WINAPI ServiceCtrlHandler(DWORD control)
{

    switch (control) {

	case SERVICE_CONTROL_PAUSE:
	    //status_val.dwCurrentState = SERVICE_PAUSE_PENDING;
	    //SetServiceStatus(status_handle, &status_val);
	    break;

	case SERVICE_CONTROL_CONTINUE:
	    //status_val.dwCurrentState = SERVICE_CONTINUE_PENDING;
	    //SetServiceStatus(status_handle, &status_val);
	    break;

	case SERVICE_CONTROL_STOP:
	    //status_val.dwCurrentState = SERVICE_STOP_PENDING;
	    //SetServiceStatus(status_handle, &status_val);
	    break;

	case SERVICE_CONTROL_SHUTDOWN:
	    break;

	case SERVICE_CONTROL_INTERROGATE:
	    //status_val.dwCurrentState = SERVICE_RUNNING;
	    //SetServiceStatus(status_handle, &status_val);
	    break;

	default:
	    break;
    }
}

void WINAPI ServiceMain(DWORD argc, CHAR **argv)
{

    SERVICE_STATUS_HANDLE status_handle;
    status_handle = RegisterServiceCtrlHandler(argv[0], ServiceCtrlHandler);
    if (!status_handle) return;

    SERVICE_STATUS status_val;
    memset(&status_val, 0, sizeof(SERVICE_STATUS));
    status_val.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
    status_val.dwCurrentState = SERVICE_RUNNING;
    //status_val.dwControlsAccepted = SERVICE_ACCEPT_STOP;
    SetServiceStatus(status_handle, &status_val);

    if (baidu_run) baidu_run();
}

int baidu_init(TCHAR *service_name, BAIDU_CALLBACK *callback)
{
    baidu_run = callback;

    SERVICE_TABLE_ENTRY st[] = {
	{service_name, ServiceMain},
	{NULL, NULL}
    };

    return StartServiceCtrlDispatcher(st);
}

