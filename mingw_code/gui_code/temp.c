
#include <windows.h>

// 框架窗口类名
TCHAR frameWndClassName[] = TEXT("FrameClass");
// 系统配置窗口类名
TCHAR sysConfWndClassName[] = TEXT("SysConfClass");

// 框架窗口句柄和客户区窗口句柄
HWND hwndFrame, hwndClient;
// 全局类的HINSTANCE，便于其余地方使用
HINSTANCE g_hInst;

// 系统配置窗口(子窗口)回调函数
LRESULT CALLBACK SysConfWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
// 框架窗口回调函数
LRESULT CALLBACK FrameWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

// 入口函数
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    MSG msg;

    WNDCLASS frameWndClass;
    WNDCLASS sysConfWndClass;

    g_hInst = hInstance;
    // 对框架窗口类的配置和注册
    frameWndClass.cbClsExtra = 0;
    frameWndClass.style = CS_HREDRAW | CS_VREDRAW;
    frameWndClass.lpszMenuName = NULL;
    frameWndClass.lpszClassName = frameWndClassName;
    frameWndClass.lpfnWndProc = FrameWindowProc;
    frameWndClass.hInstance = hInstance;
    frameWndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    frameWndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    frameWndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    frameWndClass.cbWndExtra = 0;
    RegisterClass(&frameWndClass);

    // 对系统配置窗口类的配置和注册
    sysConfWndClass.cbClsExtra = 0;
    sysConfWndClass.style = CS_HREDRAW | CS_VREDRAW;
    sysConfWndClass.lpszMenuName = NULL;
    sysConfWndClass.lpszClassName = sysConfWndClassName;
    sysConfWndClass.lpfnWndProc = SysConfWindowProc;
    sysConfWndClass.hInstance = hInstance;
    sysConfWndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    sysConfWndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    sysConfWndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    sysConfWndClass.cbWndExtra = 0;
    RegisterClass(&sysConfWndClass);

    hwndFrame = CreateWindow(frameWndClassName, TEXT("框架窗口"), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, g_hInst, NULL);
    ShowWindow(hwndFrame, SW_SHOWDEFAULT);
    UpdateWindow(hwndFrame);

    while(GetMessage(&msg, NULL, 0, 0) != 0)
    {
	if(!TranslateMDISysAccel(hwndClient, &msg))
	{
	    TranslateMessage(&msg);
	    DispatchMessage(&msg);
	}
    }

    return 0;
}

LRESULT CALLBACK FrameWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    CLIENTCREATESTRUCT clientcreate;
    MDICREATESTRUCT mdicrete;

    switch(uMsg)
    {
	case WM_CREATE:
	    // 创建客户区窗口
	    clientcreate.hWindowMenu = NULL; // 不需要显示子窗口列表，故设置为NULL
	    clientcreate.idFirstChild = 1000; //  子窗口ID从1000开始设置
	    // 建立客户区窗口
	    hwndClient = CreateWindow(TEXT("MDICLIENT"), NULL, WS_CHILD | WS_VISIBLE | WS_CHILDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, hwnd, NULL, g_hInst, &clientcreate);

	    /*****************************************************************************************************************************/
	    // 建立个按钮，然后对这个按钮进行响应，问题主要出现在这里，就是接受WM_COMMAND的时候没反应...
	    // 用1 建立的话，设置hwnd，则这个按钮建立到了框架窗口上了，按不动就。。
	    /*1:*/ //CreateWindow(TEXT("BUTTON"), TEXT("Send"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 20, 20, 50, 20, hwnd, NULL, g_hInst, NULL);
	    // 用2 建立的话，可以按得动，建立到了客户区的窗口上了，但是无法响应WM_COMMAND消息
	    /*2:*/ CreateWindow(TEXT("BUTTON"), TEXT("Send"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 20, 20, 50, 20, hwndClient, NULL, g_hInst, NULL);
	    /*****************************************************************************************************************************/
	    break;
	case WM_COMMAND:
	    /*****************************************************************************************************************************/
	    // 响应按钮的处理...，目前是无反应的。。我怀疑是这个处理只对建立到hwndFrame上的按钮才处理的
	    MessageBox(hwnd, TEXT("HI"), TEXT("TITLE"), MB_OK);
	    /*****************************************************************************************************************************/
	    break;
	case WM_CLOSE:
	    DestroyWindow(hwnd);
	    break;
	case WM_DESTROY:
	    PostQuitMessage(0);
	    break;
	default:
	    return DefFrameProc(hwnd, hwndClient, uMsg, wParam, lParam);
    }
    return 0;
}

LRESULT CALLBACK SysConfWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    return DefMDIChildProc(hwnd, uMsg, wParam, lParam);
}


