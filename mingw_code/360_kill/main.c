#include <stdio.h>
#include <windows.h>
#include <winioctl.h>
#include <tlhelp32.h>

HANDLE GetProcessHandle(char *str)
{

    //创建打开的进程快照
    PROCESSENTRY32 pe32;
    pe32.dwSize = sizeof(pe32);
    HANDLE hp = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

    //遍历进程打开并返回句柄
    while(Process32Next(hp, &pe32)){

	if (strcmp(str, pe32.szExeFile) == 0) {

	    return (OpenProcess(PROCESS_ALL_ACCESS,
			FALSE,
			pe32.th32ProcessID));
	}
    }

    return NULL;
}

int GetProcessPID(char *str)
{

    //创建打开的进程快照
    PROCESSENTRY32 pe32;
    pe32.dwSize = sizeof(pe32);
    HANDLE hp = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

    //遍历进程打开并返回PID
    while(Process32Next(hp, &pe32)){

	if (strcmp(str, pe32.szExeFile) == 0) {

	    return (int)pe32.th32ProcessID;
	}
    }

    return FALSE;
}

int KillProcsee(HANDLE hDevice, int pid)
{

    ULONG RetCode;
    return DeviceIoControl(hDevice,
	    pid,
	    NULL,
	    0,
	    NULL,
	    0,
	    &RetCode,
	    NULL);
}

int DriverInstall(char *driverName, char *driverPath)
{

    SC_HANDLE SCManager = OpenSCManager(NULL,
	    NULL,
	    SC_MANAGER_ALL_ACCESS);
    if (!SCManager) {

	return FALSE;
    }

    SC_HANDLE SCService = CreateService(SCManager,
	    driverName,
	    driverName,
	    SERVICE_ALL_ACCESS,
	    SERVICE_KERNEL_DRIVER,
	    SERVICE_SYSTEM_START,
	    SERVICE_ERROR_NORMAL,
	    driverPath,
	    NULL,
	    NULL,
	    NULL,
	    NULL,
	    NULL);
    if (!SCService) {

	SCService = OpenService(SCManager,
		driverName,
		SERVICE_ALL_ACCESS);

	if (!SCService) {

	    CloseServiceHandle(SCManager);
	    return FALSE;
	}
    }

    CloseServiceHandle(SCService);
    CloseServiceHandle(SCManager);
    return TRUE;
}

int DriverUnInstall(char *service_name)
{

    SC_HANDLE sc_manager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
    if (sc_manager == NULL) {

	return FALSE;
    }

    SC_HANDLE sc_service = OpenService(sc_manager, service_name, SERVICE_STOP | DELETE);
    if (sc_service == NULL) {

	CloseServiceHandle(sc_manager);
	return FALSE;
    }    

    if (DeleteService(sc_service) == FALSE) {

	CloseServiceHandle(sc_service);
	CloseServiceHandle(sc_manager);
	return FALSE;
    }

    CloseServiceHandle(sc_service);
    CloseServiceHandle(sc_manager);
    return TRUE;
}

int DriverStart(char *driverName)
{

    SC_HANDLE SCManager = OpenSCManager(NULL,
	    NULL,
	    SC_MANAGER_ALL_ACCESS);
    if (!SCManager) {

	return FALSE;
    }

    SC_HANDLE SCService = OpenService(SCManager,
	    driverName,
	    SERVICE_ALL_ACCESS);
    if (!SCService) {

	CloseServiceHandle(SCManager);
	return FALSE;
    }

    if (!StartService(SCService, 0, NULL)) {

	//运行中
	if (GetLastError() != ERROR_SERVICE_ALREADY_RUNNING) {

	    CloseServiceHandle(SCService);
	    CloseServiceHandle(SCManager);
	    return FALSE;
	}
    }

    CloseServiceHandle(SCService);
    CloseServiceHandle(SCManager);
    return TRUE;
}

HANDLE DriverOpen(char *driverName)
{

    char path[4096] = {0};
    sprintf(path, "\\\\.\\%s", driverName);
    HANDLE hDevice = CreateFile(path,
	    GENERIC_READ | GENERIC_WRITE,
	    0,
	    NULL,
	    OPEN_EXISTING,
	    FILE_ATTRIBUTE_NORMAL,
	    NULL);

    if(hDevice == INVALID_HANDLE_VALUE) {

	return NULL;
    }

    return hDevice;
}

int main(int argc, char *argv[])
{

    DriverUnInstall("mydev");
    DriverInstall("mydev", "system32\\drivers\\mydev.sys");
    DriverStart("mydev");

    HANDLE hDevice = DriverOpen("mydev");
    if (!hDevice) {

	MessageBox(NULL, "load mydev.sys error!", "error", MB_OK);
	return -1;
    }

    int pid = GetProcessPID("360tray.exe");
    if (pid == FALSE) {

	MessageBox(NULL, "NotStarted 360tray.exe", "error", MB_OK);
	return -1;
    }

    KillProcsee(hDevice, pid);
    CloseHandle(hDevice);
    return 0;
}

