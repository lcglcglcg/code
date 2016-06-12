
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <mshtml.h>
#include <exdisp.h>
#include <exdispid.h>
#include <oleidl.h>
#include <olectlid.h>
#include <locale.h>
#include <time.h>

class WebBrowser:
    public IDispatch,
    public IOleClientSite,
    public IOleInPlaceSite,
    public IOleInPlaceFrame
{

    public:
	WebBrowser();
	~WebBrowser();

	// ====================
	// IUnKnown
	// ====================

	HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void **ppObject);

	ULONG STDMETHODCALLTYPE AddRef() {

	    return 1;
	}

	ULONG STDMETHODCALLTYPE Release() {

	    return 1;
	}

	// ====================
	// IDispatch
	// ====================

	HRESULT STDMETHODCALLTYPE GetTypeInfoCount(UINT*) {

	    return E_NOTIMPL;
	}

	HRESULT STDMETHODCALLTYPE GetTypeInfo(UINT,LCID,LPTYPEINFO*) {

	    return E_NOTIMPL;
	}

	HRESULT STDMETHODCALLTYPE GetIDsOfNames(REFIID,LPOLESTR*,UINT,LCID,DISPID*) {

	    return E_NOTIMPL;
	}

	HRESULT STDMETHODCALLTYPE Invoke(DISPID,REFIID,LCID,WORD,DISPPARAMS*,VARIANT*,EXCEPINFO*,UINT*);

	// ====================
	// IOleWindow
	// ====================

	virtual void NewWindow(IDispatch **ppDisp, VARIANT_BOOL *Cancel);

	HRESULT STDMETHODCALLTYPE GetWindow(HWND *phwnd);

	HRESULT STDMETHODCALLTYPE ContextSensitiveHelp(BOOL) {

	    return E_NOTIMPL;
	}

	// ====================
	// IOleInPlaceUIWindow
	// ====================

	HRESULT STDMETHODCALLTYPE GetBorder(LPRECT) {

	    return ERROR_SUCCESS;
	}

	HRESULT STDMETHODCALLTYPE RequestBorderSpace(LPCBORDERWIDTHS) {

	    return ERROR_SUCCESS;
	}

	HRESULT STDMETHODCALLTYPE SetBorderSpace(LPCBORDERWIDTHS) {

	    return ERROR_SUCCESS;
	}

	HRESULT STDMETHODCALLTYPE SetActiveObject(LPOLEINPLACEACTIVEOBJECT,LPCOLESTR) {

	    return ERROR_SUCCESS;
	}

	// ====================
	// IOleInPlaceFrame
	// ====================

	HRESULT STDMETHODCALLTYPE InsertMenus(HMENU,LPOLEMENUGROUPWIDTHS) {

	    return E_NOTIMPL;
	}

	HRESULT STDMETHODCALLTYPE SetMenu(HMENU,HOLEMENU,HWND) {

	    return E_NOTIMPL;
	}

	HRESULT STDMETHODCALLTYPE RemoveMenus(HMENU) {

	    return E_NOTIMPL;
	}

	HRESULT STDMETHODCALLTYPE SetStatusText(LPCOLESTR) {

	    return E_NOTIMPL;
	}

	HRESULT STDMETHODCALLTYPE EnableModeless(BOOL) {

	    return E_NOTIMPL;
	}

	HRESULT STDMETHODCALLTYPE TranslateAccelerator(LPMSG,WORD) {

	    return TRUE;
	}

	// ====================
	// IOleClientSite
	// ====================

	HRESULT STDMETHODCALLTYPE SaveObject() {

	    return ERROR_SUCCESS;
	}

	HRESULT STDMETHODCALLTYPE GetMoniker(DWORD,DWORD,LPMONIKER*) {

	    return E_NOTIMPL;
	}

	HRESULT STDMETHODCALLTYPE GetContainer(LPOLECONTAINER*) {

	    return E_NOTIMPL;
	}

	HRESULT STDMETHODCALLTYPE ShowObject() {

	    return ERROR_SUCCESS;
	}

	HRESULT STDMETHODCALLTYPE OnShowWindow(BOOL) {

	    return ERROR_SUCCESS;
	}

	HRESULT STDMETHODCALLTYPE RequestNewObjectLayout() {

	    return ERROR_SUCCESS;
	}

	// ====================
	// IOleInPlaceSite
	// ====================

	HRESULT STDMETHODCALLTYPE CanInPlaceActivate() {

	    return ERROR_SUCCESS;
	}

	HRESULT STDMETHODCALLTYPE OnInPlaceActivate() {

	    return ERROR_SUCCESS;
	}

	HRESULT STDMETHODCALLTYPE OnUIActivate() {

	    return ERROR_SUCCESS;
	}

	HRESULT STDMETHODCALLTYPE GetWindowContext(IOleInPlaceFrame**,IOleInPlaceUIWindow**,LPRECT,LPRECT,LPOLEINPLACEFRAMEINFO);

	HRESULT STDMETHODCALLTYPE Scroll(SIZE) {

	    return E_NOTIMPL;
	}

	HRESULT STDMETHODCALLTYPE OnUIDeactivate(BOOL) {

	    return ERROR_SUCCESS;
	}

	HRESULT STDMETHODCALLTYPE OnInPlaceDeactivate() {

	    return ERROR_SUCCESS;
	}

	HRESULT STDMETHODCALLTYPE DiscardUndoState() {

	    return ERROR_SUCCESS;
	}

	HRESULT STDMETHODCALLTYPE DeactivateAndUndo() {

	    return ERROR_SUCCESS;
	}

	HRESULT STDMETHODCALLTYPE OnPosRectChange(LPCRECT) {

	    return ERROR_SUCCESS;
	}

	// ====================
	// Initialize
	// ====================

	HWND m_hWindow;
	WebBrowser *m_pWebChild;
	IOleObject *m_pBrowserObject;
	IWebBrowser2 *m_pIWebBrowser2;
	BOOL Initialize(HWND, BSTR);

	// ====================
	// External Interface
	// ====================

	IHTMLDocument2 *GetIHTMLDocument2(IWebBrowser2 *pIWebBrowser2);
	IOleInPlaceObject *GetIOleInPlaceObject(IOleObject *pBrowserObject);
	IOleInPlaceActiveObject *GetIOleInPlaceActiveObject(IOleObject *pBrowserObject);
};

WebBrowser::WebBrowser()
{
    m_hWindow = NULL;
    m_pWebChild = NULL;
    m_pBrowserObject = NULL;
    m_pIWebBrowser2 = NULL;
}

WebBrowser::~WebBrowser()
{
}

HRESULT STDMETHODCALLTYPE WebBrowser::QueryInterface(REFIID riid, void **ppObject)
{

    *ppObject = NULL;
    if (riid == IID_IUnknown)
	*ppObject = this;
    else if (riid == IID_IDispatch)
	*ppObject = (IDispatch*)this;
    else if (riid == IID_IOleClientSite)
	*ppObject = (IOleClientSite*)this;
    else if (riid == IID_IOleInPlaceSite)
	*ppObject = (IOleInPlaceSite*)this;
    else if (riid == IID_IOleInPlaceFrame)
	*ppObject = (IOleInPlaceFrame*)this;
    else if (riid == IID_IOleInPlaceUIWindow)
	*ppObject = (IOleInPlaceUIWindow*)this;
    else if (riid == DIID_DWebBrowserEvents2)
	*ppObject = (DWebBrowserEvents2*)this;
    else
	return E_NOINTERFACE;

    return ERROR_SUCCESS;
}

void WebBrowser::NewWindow(IDispatch **ppDisp, VARIANT_BOOL *Cancel) 
{

    if (m_pWebChild) {

	IDispatch *pIDispatch;
	m_pWebChild->m_pIWebBrowser2->get_Application(&pIDispatch);
	*ppDisp = pIDispatch;

    } else {

	IDispatch *pIDispatch;
	m_pIWebBrowser2->get_Application(&pIDispatch);
	*ppDisp = pIDispatch;
    }
}

HRESULT STDMETHODCALLTYPE WebBrowser::Invoke(
	DISPID dispIdMember,
	REFIID riid,
	LCID lcid,
	WORD wFlags,
	DISPPARAMS *pDispParams,
	VARIANT *pVarResult,
	EXCEPINFO *pExcepInfo,
	UINT *puArgErr)
{

    switch(dispIdMember) {

	case DISPID_NEWWINDOW2:
	    NewWindow(pDispParams->rgvarg[1].ppdispVal,pDispParams->rgvarg[0].pboolVal);
	    return S_OK;

	case DISPID_BEFORENAVIGATE2:
	    break;
    }

    return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE WebBrowser::GetWindow(HWND *phwnd)
{
    *phwnd = m_hWindow;
    return ERROR_SUCCESS;
}

HRESULT STDMETHODCALLTYPE WebBrowser::GetWindowContext(IOleInPlaceFrame **ppFrame, 
	IOleInPlaceUIWindow **ppDoc, LPRECT lprcPosRect,
	LPRECT lprcClipRect, LPOLEINPLACEFRAMEINFO lpFrameInfo)
{

    *ppDoc = NULL;
    *ppFrame = (IOleInPlaceFrame*)this;

    lpFrameInfo->fMDIApp = FALSE;
    lpFrameInfo->hwndFrame = m_hWindow;
    lpFrameInfo->haccel = NULL;
    lpFrameInfo->cAccelEntries = 0;
    lpFrameInfo->cb = sizeof(OLEINPLACEFRAMEINFO);

    GetClientRect(m_hWindow, lprcPosRect);
    GetClientRect(m_hWindow, lprcClipRect);
    return ERROR_SUCCESS;
}

BOOL WebBrowser::Initialize(HWND hwnd, BSTR url)
{

    m_hWindow = hwnd;

    IStorage *pStorage = NULL;
    StgCreateDocfile(NULL,
	    STGM_READWRITE | STGM_SHARE_EXCLUSIVE | STGM_DIRECT | STGM_CREATE,
	    0,
	    &pStorage);
    if (pStorage == NULL)
	return FALSE;

    OleCreate(CLSID_WebBrowser,
	    IID_IOleObject,
	    0,
	    NULL,
	    this,
	    pStorage,
	    (void**)&m_pBrowserObject);
    if (m_pBrowserObject == NULL)
	return FALSE;

    m_pBrowserObject->QueryInterface(IID_IWebBrowser2, (void**)&m_pIWebBrowser2);
    if (m_pIWebBrowser2 == NULL)
	return FALSE;

    if (m_pIWebBrowser2->put_Silent(VARIANT_TRUE) != ERROR_SUCCESS)
	return FALSE;

    RECT rect;
    GetClientRect(hwnd, &rect);
    if (m_pBrowserObject->DoVerb(OLEIVERB_INPLACEACTIVATE, 0, (IOleClientSite *)this, 0, hwnd, &rect) != ERROR_SUCCESS)
	return FALSE;

    IConnectionPointContainer *pIConnectionPointContainer = NULL;
    m_pIWebBrowser2->QueryInterface(IID_IConnectionPointContainer, (void**)&pIConnectionPointContainer);
    if (pIConnectionPointContainer == NULL)
	return FALSE;

    IConnectionPoint *pIConnectionPoint = NULL;
    pIConnectionPointContainer->FindConnectionPoint(DIID_DWebBrowserEvents2, &pIConnectionPoint);
    if (pIConnectionPoint == NULL)
	return FALSE;

    DWORD iCookie = 0;
    if (pIConnectionPoint->Advise((IUnknown*)(void*)this, &iCookie) != ERROR_SUCCESS)
	return FALSE;

    VARIANT vUrl;
    VariantInit(&vUrl);
    vUrl.vt = VT_BSTR;
    vUrl.bstrVal = SysAllocString(url);
    if (!vUrl.bstrVal)
	return FALSE;

    m_pIWebBrowser2->Navigate2(&vUrl, NULL, NULL, NULL, NULL);
    VariantClear(&vUrl);

    return TRUE;
}

IHTMLDocument2 *WebBrowser::GetIHTMLDocument2(IWebBrowser2 *pIWebBrowser2)
{

    VARIANT_BOOL status = -1;
    if (pIWebBrowser2->get_Busy(&status) != ERROR_SUCCESS)
	return NULL;

    if (status != 0)
	return NULL;

    IDispatch *pDispatch = NULL;
    if (pIWebBrowser2->get_Document(&pDispatch) == ERROR_SUCCESS) {

	IHTMLDocument2 *pIHTMLDocument2 = NULL;
	if (pDispatch->QueryInterface(IID_IHTMLDocument2, (void**)&pIHTMLDocument2) == ERROR_SUCCESS) {

	    return pIHTMLDocument2;
	}

    }

    return NULL;
}

IOleInPlaceObject *WebBrowser::GetIOleInPlaceObject(IOleObject *pBrowserObject)
{

    IOleInPlaceObject *pIOleInPlaceObject = NULL;
    pBrowserObject->QueryInterface(IID_IOleInPlaceObject, (void**)&pIOleInPlaceObject);
    return pIOleInPlaceObject;
}

IOleInPlaceActiveObject *WebBrowser::GetIOleInPlaceActiveObject(IOleObject *pBrowserObject)
{

    IOleInPlaceActiveObject *pIOleInPlaceActiveObject = NULL;
    pBrowserObject->QueryInterface(IID_IOleInPlaceActiveObject, (void**)&pIOleInPlaceActiveObject);
    return pIOleInPlaceActiveObject;
}

BOOL ClickElement(IHTMLDocument2 *pIHTMLDocument2, BSTR keyword)
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

	BSTR code;
	pIHTMLElement->get_outerHTML(&code);
	if (code)wprintf(L"[CODE][%s]\n", code);

	BSTR str;
	pIHTMLElement->get_innerText(&str);
	if (str)wprintf(L"[LINK][%s]\n", str);

	if (str && keyword && wcscmp(str, keyword) == 0) {

	    wprintf(L"[Success][%s]\n", keyword);
	    wprintf(L"[Click][%s]\n", keyword);
	    pIHTMLElement->click();
	    return TRUE;
	}
    }

    return FALSE;
}

BOOL ClickElementForeach(IHTMLDocument2 *pIHTMLDocument2, BSTR keyword)
{

    IOleContainer *pIOleContainer = NULL;
    pIHTMLDocument2->QueryInterface(IID_IOleContainer, (void**)&pIOleContainer);
    if (pIOleContainer == NULL)
	return FALSE;

    IEnumUnknown *pIEnumUnknown = NULL;
    pIOleContainer->EnumObjects(1, &pIEnumUnknown);
    if (pIEnumUnknown == NULL)
	return FALSE;

    IUnknown *pIUnknown = NULL;
    while(pIEnumUnknown->Next(1, &pIUnknown, NULL) == S_OK) {

	IWebBrowser2 *_pIWebBrowser2 = NULL;
	pIUnknown->QueryInterface(IID_IWebBrowser2, (void**)&_pIWebBrowser2);
	if (_pIWebBrowser2 == NULL)
	    return FALSE;

	IDispatch *_pDispatch;
	_pIWebBrowser2->get_Document(&_pDispatch);
	if (_pDispatch == NULL)
	    return FALSE;

	IHTMLDocument2 *_pIHTMLDocument2 = NULL;
	_pDispatch->QueryInterface(IID_IHTMLDocument2, (void**)&_pIHTMLDocument2);
	if (_pIHTMLDocument2 == NULL)
	    return FALSE;

	if (ClickElement(_pIHTMLDocument2, keyword) == TRUE)
	    return TRUE;

	if (ClickElementForeach(_pIHTMLDocument2, keyword) == TRUE)
	    return TRUE;
    }

    if (ClickElement(pIHTMLDocument2, keyword) == TRUE)
	return TRUE;

    return FALSE;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{

    switch(uMsg){

	case WM_DESTROY:
	    PostQuitMessage(0);
	    return TRUE;

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
    char ClassName[] = "WebBrowser";
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

    wchar_t mess[1024];
    BSTR keyword = L"十岁正太唱歌好听被围观";
    wsprintfW(mess, L"查询[http://web.duowan.com/zhao]页面[%s]链接 点击播放", keyword);
    MessageBoxW(NULL, mess, L"", MB_OK);

    HWND m_hChild = CreateWindowEx(
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

    HWND m_hParent = CreateWindowEx(
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

    WebBrowser webChild;
    WebBrowser webParent;
    webParent.m_pWebChild = &webChild;

    webChild.Initialize(m_hChild, (BSTR)L"about:blank");
    webParent.Initialize(m_hParent, (BSTR)L"http://web.duowan.com/zhao/");

    ShowWindow(m_hChild, SW_SHOW);
    ShowWindow(m_hParent, SW_SHOW);

    UpdateWindow(m_hChild);
    UpdateWindow(m_hParent);

    MSG msg;
    clock_t start = clock();
    IHTMLDocument2 *pIHTMLDocument2 = NULL;
    while (GetMessage(&msg, 0, 0, 0)) {

	TranslateMessage(&msg);
	DispatchMessage(&msg);

	if ((pIHTMLDocument2 = webParent.GetIHTMLDocument2(webParent.m_pIWebBrowser2))) {

	    if (ClickElementForeach(pIHTMLDocument2, keyword) == TRUE) {

		BSTR cookie;
		pIHTMLDocument2->get_cookie(&cookie);
		wsprintfW(mess, L"[%s]查询成功 已点击播放 Cookie信息如下", keyword);
		MessageBoxW(NULL, cookie, mess, MB_OK);
		break;

	    }
	}

	clock_t finish = clock();
	if (finish - start > 1000 * 10) {

	    if (pIHTMLDocument2) {

		BSTR cookie;
		pIHTMLDocument2->get_cookie(&cookie);

		wsprintfW(mess, L"[%s]找不到链接 Cookie信息如下", keyword);
		MessageBoxW(NULL, cookie, mess, MB_OK);

	    } else {

		wsprintfW(mess, L"[%s]找不到链接", keyword);
	    }
	    break;
	}
    }

    while (GetMessage(&msg, 0, 0, 0)) {

	TranslateMessage(&msg);
	DispatchMessage(&msg);
    }

    OleUninitialize();
    return 0;
}




