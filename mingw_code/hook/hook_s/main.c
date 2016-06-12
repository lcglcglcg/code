#include <stdio.h>
#include <windows.h>

typedef void (HookAddCall)(void);
typedef void (HookDelCall)(void);

int WINAPI WinMain(HINSTANCE hInstance, 
	HINSTANCE hPrevInstance,
	PSTR szCmdLine,
	int iCmdShow )
{

    HINSTANCE hDll = LoadLibrary("hook.dll");
    printf("hDll=%p\n", hDll);

    //HookAddCall *add = (HookAddCall *)GetProcAddress(hDll, "HookAdd");
    //printf("add=%p\n", add);
    //HookDelCall *del = (HookDelCall *)GetProcAddress(hDll, "HookDel");

    HOOKPROC pro = (HOOKPROC)GetProcAddress(hDll, "KeyBoardProc@12");
    printf("pro=%p\n", pro);

    pro(0, NULL, NULL);
    
    //SetWindowsHookEx(WH_KEYBOARD, pro, hDll, 0);
    
    //for(;;)Sleep(1000);
    return TRUE;
}

