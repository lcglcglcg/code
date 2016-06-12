#include <stdio.h>
#include <windows.h>
#include <tlhelp32.h>

HANDLE lg_process_get(char *process_name)
{

    //创建打开的进程快照
    PROCESSENTRY32 pe32;
    pe32.dwSize = sizeof(pe32);
    HANDLE hp = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

    //遍历进程打开并返回句柄
    while(Process32Next(hp, &pe32)){

	if (strcmp(process_name, pe32.szExeFile) == 0)
	    return (OpenProcess(PROCESS_ALL_ACCESS,
			FALSE,
			pe32.th32ProcessID));
    };

    return NULL;
}

HANDLE lg_process_injection(HANDLE hand_process, char *dll_str)
{

    int dll_str_size = (strlen(dll_str)) + sizeof(char);

    //在其他进程申请内存
    LPVOID dll_addr = VirtualAllocEx(hand_process,
	    NULL, 
	    dll_str_size, 
	    MEM_COMMIT, 
	    PAGE_READWRITE);

    if (!dll_addr) {

	MessageBox(NULL, "VirtualAllocEx failure", "", MB_OK);
	return NULL;
    }

    //拷贝dll名字
    if (!WriteProcessMemory(hand_process,
		dll_addr,
		(PVOID)dll_str,
		dll_str_size,
		NULL)) {

	MessageBox(NULL, "WriteProcessMemory failure", "", MB_OK);
	return NULL;
    }

    //计算LoadLibraryA函数偏移地址
    FARPROC LoadLibraryA_address = 
	GetProcAddress(GetModuleHandle("Kernel32"), "LoadLibraryA");

    //强转为线程调用函数指针
    LPTHREAD_START_ROUTINE LoadLibraryA_thread_address = 
	(LPTHREAD_START_ROUTINE)LoadLibraryA_address;

    if (!LoadLibraryA_thread_address) {

	MessageBox(NULL, "LoadLibraryA = NULL", "", MB_OK);
	return NULL;
    }

    //创建新线程调用dll
    return CreateRemoteThread(hand_process, 
	    NULL, 
	    0, 
	    LoadLibraryA_thread_address, 
	    dll_addr, 
	    0, 
	    NULL);
}

int main(int argc, char **argv)
{

#if 0

    HANDLE hand_thread[3];

    LPTHREAD_START_ROUTINE LoadLibraryA_thread_address = 
	(LPTHREAD_START_ROUTINE)LoadLibraryA;

    hand_thread[0] = CreateThread(NULL, 
	    0, 
	    LoadLibraryA_thread_address, 
	    "mydll.dll", 
	    0, 
	    NULL);

    hand_thread[1] = CreateThread(NULL, 
	    0, 
	    LoadLibraryA_thread_address, 
	    "mydll.dll", 
	    0, 
	    NULL);

    hand_thread[2] = CreateThread(NULL, 
	    0, 
	    LoadLibraryA_thread_address, 
	    "mydll.dll", 
	    0, 
	    NULL);

    WaitForMultipleObjects(3, hand_thread, TRUE, INFINITE);
    CloseHandle(hand_thread);
    return 0;

#endif

    HANDLE hand_process = lg_process_get("explorer.exe");
    if (hand_process) {

	MessageBox(NULL, "open explorer success", "", MB_OK);

	if (lg_process_injection(hand_process, "mydll.dll")) {

	    MessageBox(NULL, "injection success", "", MB_OK);

	} else {

	    MessageBox(NULL, "injection failure", "", MB_OK);
	}

    } else {

	MessageBox(NULL, "open explorer failure", "", MB_OK);
    }

    CloseHandle(hand_process);
    return 0;
}

