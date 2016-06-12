
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <tchar.h>
#include <process.h>
#include <locale.h>
#include <winuser.h>
#include "webbrowser.h"
#include "lg_list.h"
#include "lg_config_file.h"

#define ELEMENT_KW L"kw"
#define ELEMENT_SU L"su"

HWND m_hParent = NULL;
HWND m_hChild = NULL;

typedef struct {

    BSTR KeyWord;
    BSTR Condition;
    int HomeInputTime;
    int HomeClickTime;
    int PageCount;
    int PageNextTime;
    int PageClickTime;
    int ChildLoadTime;

} KEYWORD_INFO_T;

typedef BOOL(BaiduClickRunCall)(IHTMLDocument2 *pIHTMLDocument2, KEYWORD_INFO_T *pInfo);

void MessageSleep(int time)
{

    MSG msg;
    clock_t start_clock = clock();
    while (GetMessage(&msg, 0, 0, 0)) {

	TranslateMessage(&msg);
	DispatchMessage(&msg);

	clock_t finish_clock = clock();
	if (finish_clock - start_clock > time)
	    return;
    }
}

BOOL GetKeywordInfo(KEYWORD_INFO_T *pInfo)
{
    lg_list_t config_file;
    lg_config_file_load(&config_file, "./appClick.ini");
    char *KeyWord =       lg_config_file_find(&config_file, "KeyWord");
    char *Condition =     lg_config_file_find(&config_file, "Condition");
    char *HomeInputTime = lg_config_file_find(&config_file, "HomeInputTime");
    char *HomeClickTime = lg_config_file_find(&config_file, "HomeClickTime");
    char *PageNextTime =  lg_config_file_find(&config_file, "PageNextTime");
    char *PageClickTime = lg_config_file_find(&config_file, "PageClickTime");
    char *ChildLoadTime = lg_config_file_find(&config_file, "ChildLoadTime");
    if (!(KeyWord && Condition && HomeInputTime && HomeClickTime && PageNextTime && PageClickTime && ChildLoadTime))
	return FALSE;

    wchar_t KeyWord_buffer[4096] = {0};
    if (MultiByteToWideChar(CP_ACP, 0, KeyWord, -1, KeyWord_buffer, 4096) == FALSE)
	return FALSE;

    wchar_t Condition_buffer[4096] = {0};
    if (MultiByteToWideChar(CP_ACP, 0, Condition, -1, Condition_buffer, 4096) == FALSE)
	return FALSE;

    pInfo->KeyWord = SysAllocString(KeyWord_buffer);
    pInfo->Condition = SysAllocString(Condition_buffer);

    pInfo->HomeInputTime = atoi(HomeInputTime);
    pInfo->HomeClickTime = atoi(HomeClickTime);
    pInfo->PageCount = 1;
    pInfo->PageNextTime = atoi(PageNextTime);
    pInfo->PageClickTime = atoi(PageClickTime);
    pInfo->ChildLoadTime = atoi(ChildLoadTime);

    wprintf(L"[KeyWord][%s]\n",		pInfo->KeyWord);
    wprintf(L"[Condition][%s]\n",       pInfo->Condition);
    wprintf(L"[HomeInputTime][%d]\n",   pInfo->HomeInputTime);
    wprintf(L"[HomeClickTime][%d]\n",   pInfo->HomeClickTime);
    wprintf(L"[PageNextTime][%d]\n",    pInfo->PageNextTime);
    wprintf(L"[PageClickTime][%d]\n",   pInfo->PageClickTime);
    wprintf(L"[ChildLoadTime][%d]\n",   pInfo->ChildLoadTime);
    return TRUE;
}

BOOL BaiduClickHome(IHTMLDocument2 *pIHTMLDocument2, KEYWORD_INFO_T *pInfo)
{

    IHTMLElement *pIHTMLElementkw = NULL;
    IHTMLElement *pIHTMLElementsu = NULL;

    IHTMLElementCollection *pIHTMLElementCollection = NULL;
    pIHTMLDocument2->get_all(&pIHTMLElementCollection);
    if (pIHTMLElementCollection == NULL)
	return FALSE;

    LONG iCount = 0;
    pIHTMLElementCollection->get_length(&iCount);
    for (LONG i = 0; i < iCount; i++) {

	VARIANT vIndex;
	VariantInit(&vIndex);
	V_VT(&vIndex) = VT_I4;
	V_I4(&vIndex) = i;

	IDispatch *pIDispatch = NULL;
	pIHTMLElementCollection->item(vIndex, vIndex, &pIDispatch);
	if (pIDispatch == NULL)
	    return FALSE;

	IHTMLElement *pIHTMLElement = NULL;
	pIDispatch->QueryInterface(IID_IHTMLElement, (void**)&pIHTMLElement);
	if (pIHTMLElement == NULL)
	    return FALSE;

	BSTR str;
	pIHTMLElement->get_id(&str);
	if (str && wcscmp(str, ELEMENT_KW) == 0)
	    pIHTMLElementkw = pIHTMLElement;
	else if (str && wcscmp(str, ELEMENT_SU) == 0)
	    pIHTMLElementsu = pIHTMLElement;
    }

    if (pIHTMLElementkw && pIHTMLElementsu) {

	MessageSleep(pInfo->HomeInputTime);
	pIHTMLElementkw->put_innerText(pInfo->KeyWord);
	MessageSleep(pInfo->HomeClickTime);
	pIHTMLElementsu->click();
	return TRUE;
    }

    return FALSE;
}

BOOL BaiduClickPage(IHTMLDocument2 *pIHTMLDocument2, KEYWORD_INFO_T *pInfo)
{

    IHTMLElementCollection *pIHTMLElementCollection = NULL;
    pIHTMLDocument2->get_links(&pIHTMLElementCollection);
    if (pIHTMLElementCollection == NULL)
	return FALSE;

    LONG iCount = 0;
    pIHTMLElementCollection->get_length(&iCount);
    for (LONG i = 0; i < iCount; i++) {

	VARIANT vIndex;
	VariantInit(&vIndex);
	V_VT(&vIndex) = VT_I4;
	V_I4(&vIndex) = i;

	IDispatch *pIDispatch = NULL;
	pIHTMLElementCollection->item(vIndex, vIndex, &pIDispatch);
	if (pIDispatch == NULL)
	    return FALSE;

	IHTMLElement *pIHTMLElement = NULL;
	pIDispatch->QueryInterface(IID_IHTMLElement, (void**)&pIHTMLElement);
	if (pIHTMLElement == NULL)
	    return FALSE;

	BSTR str;
	pIHTMLElement->get_innerText(&str);
	if (str && wcscmp(str, pInfo->Condition) == 0) {

	    MessageSleep(pInfo->PageClickTime);
	    pIHTMLElement->click();
	    wprintf(L"[Success][page=%d][%s]\n", pInfo->PageCount, str);
	    return TRUE;

	} else if (str && wcscmp(str, L"下一页>") == 0) {

	    pInfo->PageCount++;
	    MessageSleep(pInfo->PageNextTime);
	    pIHTMLElement->click();
	    return FALSE;
	}

	wprintf(L"[find][page=%d][%s]\n", pInfo->PageCount, str);
    }

    return FALSE;
}

int BaiduClickRun(WebBrowser *pWebBrowser, BaiduClickRunCall *pCall, KEYWORD_INFO_T *pInfo)
{

    if (pCall == BaiduClickHome) {

	pWebBrowser->OpenUrl((BSTR)L"http://www.baidu.com");
	pWebBrowser->m_pWebChild->OpenUrl((BSTR)L"about:blank");
    }

    MSG msg;
    clock_t start_clock = clock();
    while (GetMessage(&msg, 0, 0, 0)) {

	TranslateMessage(&msg);
	DispatchMessage(&msg);

	clock_t finish_clock = clock();
	if (finish_clock - start_clock > 100000)
	    return FALSE;

	IHTMLDocument2 *pIHTMLDocument2 = NULL;
	if (pIHTMLDocument2 = GetIHTMLDocument2(pWebBrowser->m_pIWebBrowser2))
	    if (pCall(pIHTMLDocument2, pInfo) == TRUE)
		return TRUE;
    }

    return FALSE;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{

    switch(uMsg){

	case WM_DESTROY:
	    PostQuitMessage(0);
	    break;

	case WM_CREATE:
	case WM_CLOSE://点击关闭按钮消息
	case WM_TIMER://时钟消息
	    break;
    }

    return(DefWindowProc(hwnd, uMsg, wParam, lParam));
}

int __main(int argc, char *argv[])
{

    WNDCLASSEX wc;
    char ClassName[] = "appClick";
    ZeroMemory(&wc, sizeof(WNDCLASSEX));
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.lpfnWndProc = WindowProc;
    wc.lpszClassName = ClassName;
    RegisterClassEx(&wc);
    setlocale(LC_CTYPE, "chs");

    if (OleInitialize(NULL) != S_OK) {

	MessageBoxA(0, "can't open OLE!", "error", MB_OK);
	return FALSE;
    }

    m_hParent = CreateWindowEx(
	    0,
	    ClassName,
	    "Parent",
	    0,
	    CW_USEDEFAULT,
	    CW_USEDEFAULT,
	    CW_USEDEFAULT,
	    CW_USEDEFAULT,
	    HWND_DESKTOP,
	    NULL,
	    NULL,
	    0);

    m_hChild = CreateWindowEx(
	    0,
	    ClassName,
	    "Child",
	    0,
	    CW_USEDEFAULT,
	    CW_USEDEFAULT,
	    CW_USEDEFAULT,
	    CW_USEDEFAULT,
	    HWND_DESKTOP,
	    NULL,
	    NULL,
	    0);

    WebBrowser webParent;
    WebBrowser webChild;
    webParent.m_pWebChild = &webChild;
    webParent.Initialize(m_hParent);
    webChild.Initialize(m_hChild);

    if (argc == 2 && strcmp(argv[1], "debug") == 0) {

	ShowWindow(m_hChild, SW_SHOW);
	ShowWindow(m_hParent, SW_SHOW);
	UpdateWindow(m_hChild);
	UpdateWindow(m_hParent);
    }

    KEYWORD_INFO_T Info;
    if (GetKeywordInfo(&Info) == FALSE) {

	MessageBoxA(0, "can't read appClick.ini", "error", MB_OK);
	WSACleanup();
	OleUninitialize();
	return 0;
    }

    if (BaiduClickRun(&webParent, BaiduClickHome, &Info) == FALSE) {

	MessageBoxA(0, "Baidu Home time out", "error", MB_OK);
	WSACleanup();
	OleUninitialize();
	return 0;
    }

    if (BaiduClickRun(&webParent, BaiduClickPage, &Info) == FALSE) {

	MessageBoxA(0, "Baidu Page time out", "error", MB_OK);
	WSACleanup();
	OleUninitialize();
	return 0;
    }
    
    ShellExecuteA(m_hParent, "open", "rundll32.exe", " InetCpl.cpl,ClearMyTracksByProcess 2", "", 0);
    MessageSleep(Info.ChildLoadTime);

    WSACleanup();
    OleUninitialize();
    return 0;
}

int main(int argc, char *argv[])
{

    //ShellExecuteA(NULL, "open", "calc.exe", NULL, NULL, SW_SHOWNORMAL);
    ShellExecuteA(NULL, "open", "rundll32.exe", " InetCpl.cpl,ClearMyTracksByProcess 2", "", 0);
    return 0;
}

