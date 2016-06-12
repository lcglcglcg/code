
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <tchar.h>
#include <process.h>
#include <locale.h>
#include <winuser.h>
#include "webbrowser.h"

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

void KeywordInfoPrintf(KEYWORD_INFO_T *pInfo)
{
    wprintf(L"[KeyWord][%s]\n",		pInfo->KeyWord);
    wprintf(L"[Condition][%s]\n",       pInfo->Condition);
    wprintf(L"[HomeInputTime][%d]\n",   pInfo->HomeInputTime);
    wprintf(L"[HomeClickTime][%d]\n",   pInfo->HomeClickTime);
    wprintf(L"[PageNextTime][%d]\n",    pInfo->PageNextTime);
    wprintf(L"[PageClickTime][%d]\n",   pInfo->PageClickTime);
}

BOOL GetKeywordInfo(IHTMLDocument2 *pIHTMLDocument2, KEYWORD_INFO_T *pInfo)
{

    IHTMLElement *KeyWord = NULL;
    IHTMLElement *Condition = NULL;
    IHTMLElement *HomeInputTime = NULL;
    IHTMLElement *HomeClickTime = NULL;
    IHTMLElement *PageNextTime = NULL;
    IHTMLElement *PageClickTime = NULL;

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
	if (str && wcscmp(str, L"KeyWord") == 0)
	    KeyWord = pIHTMLElement;
	else if (str && wcscmp(str, L"Condition") == 0)
	    Condition = pIHTMLElement;
	else if (str && wcscmp(str, L"HomeInputTime") == 0)
	    HomeInputTime = pIHTMLElement;
	else if (str && wcscmp(str, L"HomeClickTime") == 0)
	    HomeClickTime = pIHTMLElement;
	else if (str && wcscmp(str, L"PageNextTime") == 0)
	    PageNextTime = pIHTMLElement;
	else if (str && wcscmp(str, L"PageClickTime") == 0)
	    PageClickTime = pIHTMLElement;
    }

    if (KeyWord
	    && Condition
	    && HomeInputTime
	    && HomeClickTime
	    && PageNextTime
	    && PageClickTime) {

	KeyWord->get_innerText(&pInfo->KeyWord);
	Condition->get_innerText(&pInfo->Condition);

	BSTR HomeInputTimeStr;
	HomeInputTime->get_innerText(&HomeInputTimeStr);

	BSTR HomeClickTimeStr;
	HomeClickTime->get_innerText(&HomeClickTimeStr);

	BSTR PageNextTimeStr;
	PageNextTime->get_innerText(&PageNextTimeStr);

	BSTR PageClickTimeStr;
	PageClickTime->get_innerText(&PageClickTimeStr);

	pInfo->PageCount = 1;
	pInfo->HomeInputTime = _wtoi(HomeInputTimeStr);
	pInfo->HomeClickTime = _wtoi(HomeClickTimeStr);
	pInfo->PageNextTime = _wtoi(PageNextTimeStr);
	pInfo->PageClickTime = _wtoi(PageClickTimeStr);
	KeywordInfoPrintf(pInfo);
	return TRUE;
    }

    return FALSE;
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

    if (pCall == GetKeywordInfo) {

	pWebBrowser->OpenUrl((BSTR)L"http://lcg.leechat.com");

    } else if (pCall == BaiduClickHome) {

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

int main(int argc, char *argv[])
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
    if (BaiduClickRun(&webParent, GetKeywordInfo, &Info) == TRUE)
	if (BaiduClickRun(&webParent, BaiduClickHome, &Info) == TRUE)
	    if (BaiduClickRun(&webParent, BaiduClickPage, &Info) == TRUE)
		MessageSleep(100000);

    WSACleanup();
    OleUninitialize();
    return 0;
}



