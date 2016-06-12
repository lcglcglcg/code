
#ifndef	__WEBBORWSER_H__
#define	__WEBBORWSER_H__

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
	BOOL Initialize(HWND);
	BOOL OpenUrl(BSTR);
};

extern IOleInPlaceObject *GetIOleInPlaceObject(IOleObject *pBrowserObject);
extern IOleInPlaceActiveObject *GetIOleInPlaceActiveObject(IOleObject *pBrowserObject);
extern IHTMLDocument2 *GetIHTMLDocument2(IWebBrowser2 *pIWebBrowser2);
extern BOOL ClickElementForeach(IHTMLDocument2 *pIHTMLDocument2, BSTR link_str);

#endif


