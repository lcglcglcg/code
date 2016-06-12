
#ifndef	UNICODE
#define	UNICODE
#endif

#include <windows.h>
#include <winuser.h>
#include <richedit.h>
#include <dsclient.h>
#include <shlobj.h>
#include <stdio.h>
#include "bmp_code_load.h"

#define	ID_BUTTON_START 4000

HWND hPath = NULL;
HWND hEdit = NULL;

BOOL SelectFolder(HWND hEdit)
{

    BROWSEINFOA broinfo = {0};
    LPITEMIDLIST pIDList = NULL;
    char path[MAX_PATH] = {0};

    broinfo.pszDisplayName = path;
    broinfo.ulFlags = BIF_RETURNONLYFSDIRS;

    if ((pIDList = SHBrowseForFolderA(&broinfo))) {

	SHGetPathFromIDListA(pIDList, path);
	SetWindowTextA(hEdit, path);
	return TRUE;
    }

    return FALSE;
}

BOOL SelectFile(HWND hEdit)
{

    char szFile[260];
    OPENFILENAMEA ofn;      

    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = NULL;
    ofn.lpstrFile = szFile;
    ofn.lpstrFile[0] = '\0';
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrFilter = "*.bmp\0";
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = NULL;
    ofn.Flags = 0;

    if (GetOpenFileNameA(&ofn)==FALSE)
	return FALSE;

    SetWindowTextA(hEdit, ofn.lpstrFile);
    return TRUE;
}

void ButtonStart()
{

    char path[MAX_PATH] = {0};
    SelectFile(hPath);
    GetWindowTextA(hPath, path, MAX_PATH);

    bmp_code_load bmp;
    char buffer[40960] = {0};
    if (bmp.open(path) == true) {

	bmp.grey_threshold_sort();
	bmp.binarize();
	bmp.delete_noise();
	bmp.print();

	for (int n = 0, i = 0; i < bmp.height; i++) {

	    for (int j = 0; j < bmp.width; j++) {

		if (bmp.buffer[i][j])
		    buffer[n++] = '#';
		else
		    buffer[n++] = ' ';
	    }

	    buffer[n++] = '\r';
	    buffer[n++] = '\n';
	}
    }

    fprintf(stdout, "%s\n", buffer);
    SetWindowTextA(hEdit, buffer);
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{

    switch(uMsg){

	case WM_CREATE:

	    hPath = CreateWindowExA(WS_EX_CLIENTEDGE,
		    "RICHEDIT",
		    "File Path",
		    WS_CHILD | WS_VISIBLE | EM_SETLIMITTEXT,
		    10, 10,
		    400, 30,
		    hwnd,
		    NULL,
		    NULL,
		    NULL);

	    CreateWindowExA(0,
		    "BUTTON",
		    "Start",
		    WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		    410, 10,
		    100, 30,
		    hwnd,
		    (HMENU)ID_BUTTON_START,
		    NULL,
		    NULL);

	    hEdit = CreateWindowExA(WS_EX_CLIENTEDGE,
		    "EDIT",
		    NULL,
		    WS_CHILD | WS_VISIBLE | ES_MULTILINE | ES_SAVESEL | WS_HSCROLL | WS_VSCROLL,
		    10, 50,
		    600, 600,
		    hwnd,
		    NULL,
		    NULL,
		    NULL);

	    return 0;

	case WM_COMMAND:

	    if (LOWORD(wParam) == ID_BUTTON_START)
		ButtonStart();
	    return 0;

	case WM_CLOSE:
	    DestroyWindow(hwnd);
	    return 0;

	case WM_DESTROY:
	    PostQuitMessage(0);
	    return 0;
    }

    return(DefWindowProc(hwnd, uMsg, wParam, lParam));
}

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hInstNULL, LPSTR lpszCmdLine, int nCmdShow)
{

    MSG msg = {0};
    WNDCLASSEX wc = {0};
    wchar_t *ClassName = L"app";

    wc.cbSize = sizeof(WNDCLASSEX);
    wc.hInstance = hInstance;
    wc.lpfnWndProc = WindowProc;
    wc.lpszClassName = ClassName;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)COLOR_3DDKSHADOW;
    RegisterClassEx(&wc);

    if (!(LoadLibrary(L"RICHED32"))) {

	MessageBox(0, L"LoadLibrary RICHED32 Error!", NULL, MB_OK);
	return -1;
    }

    RECT rc = {0};
    HWND hDesktop = GetDesktopWindow();
    GetClientRect(hDesktop, &rc);

    if (!(msg.hwnd = CreateWindowEx(0,
		    ClassName,
		    ClassName,
		    WS_SYSMENU | WS_VISIBLE,
		    CW_USEDEFAULT,
		    CW_USEDEFAULT,
		    CW_USEDEFAULT,
		    CW_USEDEFAULT,
		    NULL,
		    NULL,
		    hInstance,
		    NULL))) {

	MessageBox(0, L"CreateWindowEx error!", NULL, MB_OK);
	return -1;
    }

    ShowWindow(msg.hwnd, nCmdShow);
    UpdateWindow(msg.hwnd);

    while (GetMessage(&msg, 0, 0, 0)) {

	TranslateMessage(&msg);
	DispatchMessage(&msg);
    }

    return 0;
}


