
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <process.h>
#include <locale.h>
#include <winuser.h>

#include "network.h"
#include "webbrowser.h"

HWND m_hParent = NULL;
HWND m_hChild = NULL;
//HANDLE hThread = NULL;
//
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

int WebbrowserClickRun(WebBrowser *pWebBrowser, BSTR url_str, BSTR link_str, BSTR cookie_str)
{

    pWebBrowser->OpenUrl(url_str);
    pWebBrowser->m_pWebChild->OpenUrl((BSTR)L"about:blank");

    MSG msg;
    clock_t start_clock = clock();
    while (GetMessage(&msg, 0, 0, 0)) {

	TranslateMessage(&msg);
	DispatchMessage(&msg);

	clock_t finish_clock = clock();
	if (finish_clock - start_clock > 10000)
	    return FALSE;

	IHTMLDocument2 *pIHTMLDocument2 = NULL;
	if (pIHTMLDocument2 = GetIHTMLDocument2(pWebBrowser->m_pIWebBrowser2)) {

	    pIHTMLDocument2->get_cookie(&cookie_str);
	    if (ClickElementForeach(pIHTMLDocument2, link_str) == TRUE) {

		MessageSleep(10000);
		return TRUE;
	    }
	}
    }

    return TRUE;
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

    WSADATA wsadata;
    if (WSAStartup(MAKEWORD(2, 2), &wsadata) != NO_ERROR) {

	fprintf(stderr, "WSAStartup: error!\n");
	return FALSE;
    }

    if (OleInitialize(NULL) != S_OK) {

	fprintf(stderr, "OleInitialize: error!\n");
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

    ShowWindow(m_hChild, SW_SHOW);
    ShowWindow(m_hParent, SW_SHOW);
    UpdateWindow(m_hChild);
    UpdateWindow(m_hParent);


    task_info_t task_info = {0};
    while(1){

	SOCKET sockfd = network_connect(CLICK_SERVER_HOST, CLICK_SERVER_PORT);
	network_packet_click_send(sockfd, &task_info);
	network_packet_click_recv(sockfd, &task_info);
	closesocket(sockfd);

	WebbrowserClickRun(&webParent, task_info.url_str, task_info.link_str, task_info.cookie_str);
	Sleep(10000);
    }

    WSACleanup();
    OleUninitialize();
    return 0;
}

int demo_main(int argc, char *argv[])
{

    WSADATA wsadata;
    WSAStartup(MAKEWORD(2, 2), &wsadata);

    SOCKET sockfd;
    task_info_t task_info;

    sockfd = network_connect(CLICK_SERVER_HOST, CLICK_SERVER_PORT);
    network_packet_click_send(sockfd, &task_info);
    network_packet_click_recv(sockfd, &task_info);
    closesocket(sockfd);

    sockfd = network_connect(CLICK_SERVER_HOST, CLICK_SERVER_PORT);
    network_packet_click_send(sockfd, &task_info);
    network_packet_click_recv(sockfd, &task_info);
    closesocket(sockfd);

    sockfd = network_connect(CLICK_SERVER_HOST, CLICK_SERVER_PORT);
    network_packet_click_send(sockfd, &task_info);
    network_packet_click_recv(sockfd, &task_info);
    closesocket(sockfd);

    sockfd = network_connect(CLICK_SERVER_HOST, CLICK_SERVER_PORT);
    network_packet_click_send(sockfd, &task_info);
    network_packet_click_recv(sockfd, &task_info);
    closesocket(sockfd);

    sockfd = network_connect(CLICK_SERVER_HOST, CLICK_SERVER_PORT);
    network_packet_click_send(sockfd, &task_info);
    network_packet_click_recv(sockfd, &task_info);
    closesocket(sockfd);

    return 0;
}



