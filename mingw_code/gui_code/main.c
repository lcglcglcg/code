
#include <windows.h>

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    if (message == WM_DESTROY) {

	PostQuitMessage(0);
    }

    return DefWindowProc(hWnd, message, wParam, lParam);
}

int APIENTRY WinMain(HINSTANCE hInstance, //应用程序的实例句柄，
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, //命令行
	int nCmdShow) //显示方式
{

    MSG msg;
    HWND hWnd;
    char szTitle[]="create windows"; // The title bar text
    char szWindowClass[]="lcglcg"; // The title bar text
    WNDCLASSEX wcex={0};
    wcex.cbSize = sizeof(WNDCLASSEX); //WNDCLASSEX结构体大小
    wcex.style = CS_HREDRAW | CS_VREDRAW; //位置改变时重绘
    wcex.lpfnWndProc = (WNDPROC)WndProc; //消息处理函数
    wcex.hInstance = 0; //当前实例句柄
    wcex.hbrBackground = (HBRUSH)COLOR_WINDOWFRAME; //背景色
    wcex.lpszClassName = szWindowClass; //参窗口类名
    wcex.hIcon =0; //图标
    wcex.hCursor =LoadCursor(NULL, IDC_ARROW); //光标
    wcex.lpszMenuName =0; //菜单名称
    wcex.hIconSm =0; //最小化图标
    RegisterClassEx(&wcex); //注册窗口类
    hWnd = CreateWindow(szWindowClass, szTitle, WS_SYSMENU|WS_VISIBLE, //创建窗口
	    CW_USEDEFAULT,CW_USEDEFAULT, 400, 300, NULL, NULL, 0, NULL);
    if (!hWnd){
	return FALSE;
    }

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);
    while (GetMessage(&msg, NULL, 0, 0)) // 消息循环:
    {
	TranslateMessage(&msg); //转化虚拟按键到字符消息
	DispatchMessage(&msg); //分派消息调用回调函数
    }
    return msg.wParam;
}

