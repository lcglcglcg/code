
#include "webbrowser.h"

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

BOOL WebBrowser::Initialize(HWND hwnd)
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

    return TRUE;
}

BOOL WebBrowser::OpenUrl(BSTR pUrl)
{

    VARIANT vUrl;
    VariantInit(&vUrl);
    vUrl.vt = VT_BSTR;
    vUrl.bstrVal = SysAllocString(pUrl);
    if (!vUrl.bstrVal)
	return FALSE;

    m_pIWebBrowser2->Navigate2(&vUrl, NULL, NULL, NULL, NULL);
    VariantClear(&vUrl);

    return TRUE;
}


IOleInPlaceObject *GetIOleInPlaceObject(IOleObject *pBrowserObject)
{

    IOleInPlaceObject *pIOleInPlaceObject = NULL;
    pBrowserObject->QueryInterface(IID_IOleInPlaceObject, (void**)&pIOleInPlaceObject);
    return pIOleInPlaceObject;
}

IOleInPlaceActiveObject *GetIOleInPlaceActiveObject(IOleObject *pBrowserObject)
{

    IOleInPlaceActiveObject *pIOleInPlaceActiveObject = NULL;
    pBrowserObject->QueryInterface(IID_IOleInPlaceActiveObject, (void**)&pIOleInPlaceActiveObject);
    return pIOleInPlaceActiveObject;
}

IHTMLDocument2 *GetIHTMLDocument2(IWebBrowser2 *pIWebBrowser2)
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

void DelCookie(IHTMLDocument2 *pIHTMLDocument2)
{

    //pIHTMLDocument2->Cookie->Remove(0, -1);
}




