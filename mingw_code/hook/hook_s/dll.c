#include <windows.h>

HHOOK hKeyboard __attribute__((section ("Shared"), shared)) = NULL;

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD dwReason, LPVOID lpvReserved)
{
    switch(dwReason){

	case DLL_PROCESS_ATTACH:
	    break;
	case DLL_PROCESS_DETACH:
	    break;
	case DLL_THREAD_ATTACH:
	    break;
	case DLL_THREAD_DETACH:
	    break;
    }

    return TRUE;
}

__declspec(dllexport) LRESULT CALLBACK KeyBoardProc(int nCode, WPARAM wParam, LPARAM lParam)
{

    MessageBox(NULL, "KeyBoardProc!", "", MB_OK);
    //return CallNextHookEx(hKeyboard, nCode, wParam, lParam); 
    return TRUE;
}

void HookAdd(void)
{

    hKeyboard = SetWindowsHookEx(WH_KEYBOARD, KeyBoardProc, GetModuleHandle("hook.dll"), 0);
}

void HookDel(void)
{

    UnhookWindowsHookEx(hKeyboard);
    hKeyboard = NULL;
}


