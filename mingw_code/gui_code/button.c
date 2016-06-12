#include <windows.h>

int APIENTRY WinMain(HINSTANCE hInstance, //应用程序的实例句柄
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, //命令行
	int nCmdShow) //显示方式
{

    HWND hWndButton = CreateWindow("button",
	    "button",
	    BS_PUSHBUTTON | WS_BORDER | WS_VISIBLE,
	    20,
	    110,
	    300,
	    400,
	    NULL,
	    NULL,
	    NULL,
	    NULL);

    ShowWindow(hWndButton, nCmdShow);
    UpdateWindow(hWndButton);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) // 消息循环:
    {
	TranslateMessage(&msg); //转化虚拟按键到字符消息
	DispatchMessage(&msg); //分派消息调用回调函数
    }

    return msg.wParam;
}

