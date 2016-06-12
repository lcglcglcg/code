#include <windows.h>

BOOL APIENTRY DllMain(HANDLE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved)
{

    switch(ul_reason_for_call){

	case DLL_PROCESS_ATTACH:
	    MessageBox(NULL, "DLL_PROCESS_ATTACH", "mydll", MB_OK);
	    break;

	case DLL_PROCESS_DETACH:
	    MessageBox(NULL, "DLL_PROCESS_DETACH", "mydll", MB_OK);
	    break;


	case DLL_THREAD_ATTACH:
	    MessageBox(NULL, "DLL_THREAD_ATTACH", "mydll", MB_OK);
	    break;

	case DLL_THREAD_DETACH:
	    MessageBox(NULL, "DLL_THREAD_ATTACH", "mydll", MB_OK);
	    break;

	default:
	    MessageBox(NULL, "DLL_THREAD_ATTACH", "mydll", MB_OK);
	    break;
    }

    return TRUE;
}

