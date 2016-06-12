#include <stdio.h>
#include <windows.h>

#include <tchar.h>
#include <tlhelp32.h>
#include <imagehlp.h>

#include <psapi.h>

//GetModuleHandle返回模组句柄
//GetCurrentProcess获得当前进程句柄
//OpenProcess打开一个进程句柄,根据核心对象

HHOOK hook = NULL;

void TestOpenProcesses(void)
{

    DWORD dwNeeded;
    DWORD dwProcs[1024];
    char ModuleName[512];

    //枚举所有进程ID
    if (!EnumProcesses(dwProcs, sizeof(dwProcs), &dwNeeded)) {

	printf("EnumProcesses:%ld\n", GetLastError());
	return;
    }    

    //计算有多少个进程ID
    DWORD dwProcCount = dwNeeded / sizeof(DWORD);
    printf("EnumProcesses Count:%ld\n", dwProcCount);

    //遍历进程ID
    for (DWORD i = 0; i < dwProcCount; i++) {

	printf("open Processes:%ld\n", dwProcs[i]);

	//打开进程
	HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ,
		FALSE,
		dwProcs[i]);

	if (hProcess) {

	    HMODULE hMod;
	    DWORD cbNeeded;

	    //获取进程第一个模块的句柄
	    if (EnumProcessModules(hProcess, &hMod, sizeof(hMod), &cbNeeded)) {

		//获取进程第一个模块的名称
		memset(ModuleName, 0, 512);
		if (GetModuleBaseName(hProcess, hMod, ModuleName, 512))
		    printf("ModuleName:%s\n", ModuleName);
	    }
	}
    }
}

HANDLE OpenProcess_hook(DWORD dwDesiredAccess, //渴望得到的访问权限(标志)
	BOOL bInheritHandle, //是否继承句柄
	DWORD dwProcessId) //进程标示符
{

    return NULL;
}

LRESULT WINAPI GetMsgProc(int code, WPARAM wParam, LPARAM lParam)
{
    return CallNextHookEx(hook, code, wParam, lParam);
}

HMODULE ModuleFromAddress(PVOID arg)
{

    MEMORY_BASIC_INFORMATION mbi;
    if(VirtualQuery(arg, &mbi, sizeof(mbi)) != 0)
	return (HMODULE)mbi.AllocationBase;

    return NULL;
}

/*
 * 进行IAT转换的关键函数
 * pszCalleeModuleName:模块名
 * pfnOriginApiAddress:原始的API地址
 * pfnDummyFuncAddress:自己的APO地址
 * hModCallerModule:要查找的模块名称,如果没有遍历程序调用的模块
 */

void WINAPI HookOneAPI(int code, WPARAM wParam, LPARAM lParam)
{


    ULONG size;

    //获取指向PE文件中的Import中IMAGE_DIRECTORY_DESCRIPTOR数组的指针
    PIMAGE_IMPORT_DESCRIPTOR pImportDesc = (PIMAGE_IMPORT_DESCRIPTOR)
	ImageDirectoryEntryToData(hModCallerModule,TRUE,IMAGE_DIRECTORY_ENTRY_IMPORT,&size);
    if (pImportDesc == NULL)
	return;

    //查找记录,看看有没有我们想要的DLL
    for (;pImportDesc->Name; pImportDesc++) {

	LPSTR pszDllName = (LPSTR)((PBYTE)hModCallerModule+pImportDesc->Name);
	if (lstrcmpiA(pszDllName, pszCalleeModuleName) == 0)
	    break;
    }

    //没找到
    if (!pImportDesc->Name) return;

    //寻找我们想要的函数
    PIMAGE_THUNK_DATA pThunk =
	(PIMAGE_THUNK_DATA)((PBYTE)hModCallerModule+pImportDesc->FirstThunk);//IAT

    for (;pThunk->u1.Function;pThunk++) {

	//ppfn记录了与IAT表项相应的函数的地址
	PROC * ppfn= (PROC *)&pThunk->u1.Function;

	if (*ppfn == pfnOriginApiAddress) {
	    //如果地址相同，也就是找到了我们想要的函数，进行改写，将其指向我们所定义的函数

	    WriteProcessMemory(GetCurrentProcess(),ppfn,&(pfnDummyFuncAddress),
		    sizeof(pfnDummyFuncAddress),NULL);
	    return;
	}
    }
}

/*
 * 查找所挂钩的进程所应用的dll模块的
 */
BOOL WINAPI HookAllAPI(char *pszCalleeModuleName,
	PROC pfnOriginApiAddress,
	PROC pfnDummyFuncAddress)
{

    if (!pszCalleeModuleName || !pfnOriginApiAddress)
	return FALSE;

    //HMODULE hModHookDLL = ModuleFromAddress(HookOneAPI);

    HANDLE hSnapshot;
    hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, 0);

    MODULEENTRY32 me;//描述了一个被指定进程所应用的模块的struct
    BOOL node = Module32First(hSnapshot, &me);

    while(node){	

	//if (me.hModule != hModHookDLL)
	//me.hModule指向当前被挂钩进程的每一个模块

#if 0
	HookOneAPI(pszCalleeModuleName,
		pfnOriginApiAddress,
		pfnDummyFuncAddress,
		me.hModule);
#endif

	node = Module32Next(hSnapshot,&me);
    }

    return TRUE;  
}

int main(int argc, char **argv)
{

    //HANDLE hwnd = GetCurrentProcess();//获得当前进程句柄
    //DWORD dwThreadId = GetWindowThreadProcessId((HWND)hwnd, NULL);//获得自身进程ID
    //DWORD dwThreadId = GetCurrentProcessId();

    //hook = SetWindowsHookEx(WH_GETMESSAGE, GetMsgProc, ModuleFromAddress(GetMsgProc), dwThreadId);
    //UnhookWindowsHookEx(hook);

    HookAllAPI("Kernel32.dll",
	    GetProcAddress(GetModuleHandle("Kernel32.dll"), "OpenProcess"),
	    (PROC)&OpenProcess_hook);


    //TestOpenProcesses();
    //printf("me=%ld\n", GetCurrentProcessId());

    //MEMORY_BASIC_INFORMATION mbi;
    //VirtualQuery(HookOneAPI, &mbi, sizeof(mbi));

    return 0;
}

