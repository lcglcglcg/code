#include <windows.h>
#include <exdisp.h>
#include <mshtml.h>
#include <shlguid.h>
#include <locale.h>
#include <wchar.h> 

HRESULT STDMETHODCALLTYPE Storage_QueryInterface(IStorage FAR* This, REFIID riid, LPVOID FAR* ppvObj);
ULONG STDMETHODCALLTYPE Storage_AddRef(IStorage FAR* This);
ULONG STDMETHODCALLTYPE Storage_Release(IStorage FAR* This);
HRESULT STDMETHODCALLTYPE Storage_CreateStream(IStorage FAR* This, const WCHAR *pwcsName, DWORD grfMode, DWORD reserved1, DWORD reserved2, IStream **ppstm);
HRESULT STDMETHODCALLTYPE Storage_OpenStream(IStorage FAR* This, const WCHAR * pwcsName, void *reserved1, DWORD grfMode, DWORD reserved2, IStream **ppstm);
HRESULT STDMETHODCALLTYPE Storage_CreateStorage(IStorage FAR* This, const WCHAR *pwcsName, DWORD grfMode, DWORD reserved1, DWORD reserved2, IStorage **ppstg);
HRESULT STDMETHODCALLTYPE Storage_OpenStorage(IStorage FAR* This, const WCHAR * pwcsName, IStorage * pstgPriority, DWORD grfMode, SNB snbExclude, DWORD reserved, IStorage **ppstg);
HRESULT STDMETHODCALLTYPE Storage_CopyTo(IStorage FAR* This, DWORD ciidExclude, IID const *rgiidExclude, SNB snbExclude,IStorage *pstgDest);
HRESULT STDMETHODCALLTYPE Storage_MoveElementTo(IStorage FAR* This, const OLECHAR *pwcsName,IStorage * pstgDest, const OLECHAR *pwcsNewName, DWORD grfFlags);
HRESULT STDMETHODCALLTYPE Storage_Commit(IStorage FAR* This, DWORD grfCommitFlags);
HRESULT STDMETHODCALLTYPE Storage_Revert(IStorage FAR* This);
HRESULT STDMETHODCALLTYPE Storage_EnumElements(IStorage FAR* This, DWORD reserved1, void * reserved2, DWORD reserved3, IEnumSTATSTG ** ppenum);
HRESULT STDMETHODCALLTYPE Storage_DestroyElement(IStorage FAR* This, const OLECHAR *pwcsName);
HRESULT STDMETHODCALLTYPE Storage_RenameElement(IStorage FAR* This, const WCHAR *pwcsOldName, const WCHAR *pwcsNewName);
HRESULT STDMETHODCALLTYPE Storage_SetElementTimes(IStorage FAR* This, const WCHAR *pwcsName, FILETIME const *pctime, FILETIME const *patime, FILETIME const *pmtime);
HRESULT STDMETHODCALLTYPE Storage_SetClass(IStorage FAR* This, REFCLSID clsid);
HRESULT STDMETHODCALLTYPE Storage_SetStateBits(IStorage FAR* This, DWORD grfStateBits, DWORD grfMask);
HRESULT STDMETHODCALLTYPE Storage_Stat(IStorage FAR* This, STATSTG * pstatstg, DWORD grfStatFlag);

IStorageVtbl MyIStorageTable = {Storage_QueryInterface,
    Storage_AddRef,
    Storage_Release,
    Storage_CreateStream,
    Storage_OpenStream,
    Storage_CreateStorage,
    Storage_OpenStorage,
    Storage_CopyTo,
    Storage_MoveElementTo,
    Storage_Commit,
    Storage_Revert,
    Storage_EnumElements,
    Storage_DestroyElement,
    Storage_RenameElement,
    Storage_SetElementTimes,
    Storage_SetClass,
    Storage_SetStateBits,
    Storage_Stat};

IStorage MyIStorage = { &MyIStorageTable };

HRESULT STDMETHODCALLTYPE Frame_QueryInterface(IOleInPlaceFrame FAR* This, REFIID riid, LPVOID FAR* ppvObj);
ULONG STDMETHODCALLTYPE Frame_AddRef(IOleInPlaceFrame FAR* This);
ULONG STDMETHODCALLTYPE Frame_Release(IOleInPlaceFrame FAR* This);
HRESULT STDMETHODCALLTYPE Frame_GetWindow(IOleInPlaceFrame FAR* This, HWND FAR* lphwnd);
HRESULT STDMETHODCALLTYPE Frame_ContextSensitiveHelp(IOleInPlaceFrame FAR* This, BOOL fEnterMode);
HRESULT STDMETHODCALLTYPE Frame_GetBorder(IOleInPlaceFrame FAR* This, LPRECT lprectBorder);
HRESULT STDMETHODCALLTYPE Frame_RequestBorderSpace(IOleInPlaceFrame FAR* This, LPCBORDERWIDTHS pborderwidths);
HRESULT STDMETHODCALLTYPE Frame_SetBorderSpace(IOleInPlaceFrame FAR* This, LPCBORDERWIDTHS pborderwidths);
HRESULT STDMETHODCALLTYPE Frame_SetActiveObject(IOleInPlaceFrame FAR* This, IOleInPlaceActiveObject *pActiveObject, LPCOLESTR pszObjName);
HRESULT STDMETHODCALLTYPE Frame_InsertMenus(IOleInPlaceFrame FAR* This, HMENU hmenuShared, LPOLEMENUGROUPWIDTHS lpMenuWidths);
HRESULT STDMETHODCALLTYPE Frame_SetMenu(IOleInPlaceFrame FAR* This, HMENU hmenuShared, HOLEMENU holemenu, HWND hwndActiveObject);
HRESULT STDMETHODCALLTYPE Frame_RemoveMenus(IOleInPlaceFrame FAR* This, HMENU hmenuShared);
HRESULT STDMETHODCALLTYPE Frame_SetStatusText(IOleInPlaceFrame FAR* This, LPCOLESTR pszStatusText);
HRESULT STDMETHODCALLTYPE Frame_EnableModeless(IOleInPlaceFrame FAR* This, BOOL fEnable);
HRESULT STDMETHODCALLTYPE Frame_TranslateAccelerator(IOleInPlaceFrame FAR* This, LPMSG lpmsg, WORD wID);

IOleInPlaceFrameVtbl MyIOleInPlaceFrameTable = {Frame_QueryInterface,
    Frame_AddRef,
    Frame_Release,
    Frame_GetWindow,
    Frame_ContextSensitiveHelp,
    Frame_GetBorder,
    Frame_RequestBorderSpace,
    Frame_SetBorderSpace,
    Frame_SetActiveObject,
    Frame_InsertMenus,
    Frame_SetMenu,
    Frame_RemoveMenus,
    Frame_SetStatusText,
    Frame_EnableModeless,
    Frame_TranslateAccelerator};

typedef struct _IOleInPlaceFrameEx {

    IOleInPlaceFrame frame;
    HWND window;

} IOleInPlaceFrameEx;

HRESULT STDMETHODCALLTYPE Site_QueryInterface(IOleClientSite FAR* This, REFIID riid, void ** ppvObject);
ULONG STDMETHODCALLTYPE Site_AddRef(IOleClientSite FAR* This);
ULONG STDMETHODCALLTYPE Site_Release(IOleClientSite FAR* This);
HRESULT STDMETHODCALLTYPE Site_SaveObject(IOleClientSite FAR* This);
HRESULT STDMETHODCALLTYPE Site_GetMoniker(IOleClientSite FAR* This, DWORD dwAssign, DWORD dwWhichMoniker, IMoniker ** ppmk);
HRESULT STDMETHODCALLTYPE Site_GetContainer(IOleClientSite FAR* This, LPOLECONTAINER FAR* ppContainer);
HRESULT STDMETHODCALLTYPE Site_ShowObject(IOleClientSite FAR* This);
HRESULT STDMETHODCALLTYPE Site_OnShowWindow(IOleClientSite FAR* This, BOOL fShow);
HRESULT STDMETHODCALLTYPE Site_RequestNewObjectLayout(IOleClientSite FAR* This);

IOleClientSiteVtbl MyIOleClientSiteTable = {Site_QueryInterface,
    Site_AddRef,
    Site_Release,
    Site_SaveObject,
    Site_GetMoniker,
    Site_GetContainer,
    Site_ShowObject,
    Site_OnShowWindow,
    Site_RequestNewObjectLayout};

HRESULT STDMETHODCALLTYPE InPlaceSite_QueryInterface(IOleInPlaceSite FAR* This, REFIID riid, void ** ppvObject);
ULONG STDMETHODCALLTYPE InPlaceSite_AddRef(IOleInPlaceSite FAR* This);
ULONG STDMETHODCALLTYPE InPlaceSite_Release(IOleInPlaceSite FAR* This);
HRESULT STDMETHODCALLTYPE InPlaceSite_GetWindow(IOleInPlaceSite FAR* This, HWND FAR* lphwnd);
HRESULT STDMETHODCALLTYPE InPlaceSite_ContextSensitiveHelp(IOleInPlaceSite FAR* This, BOOL fEnterMode);
HRESULT STDMETHODCALLTYPE InPlaceSite_CanInPlaceActivate(IOleInPlaceSite FAR* This);
HRESULT STDMETHODCALLTYPE InPlaceSite_OnInPlaceActivate(IOleInPlaceSite FAR* This);
HRESULT STDMETHODCALLTYPE InPlaceSite_OnUIActivate(IOleInPlaceSite FAR* This);
HRESULT STDMETHODCALLTYPE InPlaceSite_GetWindowContext(IOleInPlaceSite FAR* This, LPOLEINPLACEFRAME FAR* lplpFrame,LPOLEINPLACEUIWINDOW FAR* lplpDoc,LPRECT lprcPosRect,LPRECT lprcClipRect,LPOLEINPLACEFRAMEINFO lpFrameInfo);
HRESULT STDMETHODCALLTYPE InPlaceSite_Scroll(IOleInPlaceSite FAR* This, SIZE scrollExtent);
HRESULT STDMETHODCALLTYPE InPlaceSite_OnUIDeactivate(IOleInPlaceSite FAR* This, BOOL fUndoable);
HRESULT STDMETHODCALLTYPE InPlaceSite_OnInPlaceDeactivate(IOleInPlaceSite FAR* This);
HRESULT STDMETHODCALLTYPE InPlaceSite_DiscardUndoState(IOleInPlaceSite FAR* This);
HRESULT STDMETHODCALLTYPE InPlaceSite_DeactivateAndUndo(IOleInPlaceSite FAR* This);
HRESULT STDMETHODCALLTYPE InPlaceSite_OnPosRectChange(IOleInPlaceSite FAR* This, LPCRECT lprcPosRect);

IOleInPlaceSiteVtbl MyIOleInPlaceSiteTable = {InPlaceSite_QueryInterface,
    InPlaceSite_AddRef,
    InPlaceSite_Release,
    InPlaceSite_GetWindow,
    InPlaceSite_ContextSensitiveHelp,
    InPlaceSite_CanInPlaceActivate,
    InPlaceSite_OnInPlaceActivate,
    InPlaceSite_OnUIActivate,
    InPlaceSite_GetWindowContext,
    InPlaceSite_Scroll,
    InPlaceSite_OnUIDeactivate,
    InPlaceSite_OnInPlaceDeactivate,
    InPlaceSite_DiscardUndoState,
    InPlaceSite_DeactivateAndUndo,
    InPlaceSite_OnPosRectChange};

typedef struct __IOleInPlaceSiteEx {

    IOleInPlaceSite inplace;
    IOleInPlaceFrameEx *frame;

} _IOleInPlaceSiteEx;

typedef struct __IOleClientSiteEx {

    IOleClientSite client;
    _IOleInPlaceSiteEx	inplace;

} _IOleClientSiteEx;


HRESULT STDMETHODCALLTYPE Storage_QueryInterface(IStorage FAR* This, REFIID riid, LPVOID FAR* ppvObj)
{
    return E_NOTIMPL;
}

ULONG STDMETHODCALLTYPE Storage_AddRef(IStorage FAR* This)
{
    return 1;
}

ULONG STDMETHODCALLTYPE Storage_Release(IStorage FAR* This)
{
    return 1;
}

HRESULT STDMETHODCALLTYPE Storage_CreateStream(IStorage FAR* This, const WCHAR *pwcsName, DWORD grfMode, DWORD reserved1, DWORD reserved2, IStream **ppstm)
{
    return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE Storage_OpenStream(IStorage FAR* This, const WCHAR * pwcsName, void *reserved1, DWORD grfMode, DWORD reserved2, IStream **ppstm)
{
    return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE Storage_CreateStorage(IStorage FAR* This, const WCHAR *pwcsName, DWORD grfMode, DWORD reserved1, DWORD reserved2, IStorage **ppstg)
{
    return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE Storage_OpenStorage(IStorage FAR* This, const WCHAR * pwcsName, IStorage * pstgPriority, DWORD grfMode, SNB snbExclude, DWORD reserved, IStorage **ppstg)
{
    return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE Storage_CopyTo(IStorage FAR* This, DWORD ciidExclude, IID const *rgiidExclude, SNB snbExclude,IStorage *pstgDest)
{
    return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE Storage_MoveElementTo(IStorage FAR* This, const OLECHAR *pwcsName,IStorage * pstgDest, const OLECHAR *pwcsNewName, DWORD grfFlags)
{
    return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE Storage_Commit(IStorage FAR* This, DWORD grfCommitFlags)
{
    return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE Storage_Revert(IStorage FAR* This)
{
    return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE Storage_EnumElements(IStorage FAR* This, DWORD reserved1, void * reserved2, DWORD reserved3, IEnumSTATSTG ** ppenum)
{
    return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE Storage_DestroyElement(IStorage FAR* This, const OLECHAR *pwcsName)
{
    return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE Storage_RenameElement(IStorage FAR* This, const WCHAR *pwcsOldName, const WCHAR *pwcsNewName)
{
    return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE Storage_SetElementTimes(IStorage FAR* This, const WCHAR *pwcsName, FILETIME const *pctime, FILETIME const *patime, FILETIME const *pmtime)
{
    return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE Storage_SetClass(IStorage FAR* This, REFCLSID clsid)
{
    return S_OK;
}

HRESULT STDMETHODCALLTYPE Storage_SetStateBits(IStorage FAR* This, DWORD grfStateBits, DWORD grfMask)
{
    return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE Storage_Stat(IStorage FAR* This, STATSTG * pstatstg, DWORD grfStatFlag)
{
    return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE Site_QueryInterface(IOleClientSite FAR* This, REFIID riid, void ** ppvObject)
{

    if (!memcmp(riid, &IID_IUnknown, sizeof(GUID))
	    || !memcmp(riid, &IID_IOleClientSite, sizeof(GUID))) {

	*ppvObject = &((_IOleClientSiteEx *)This)->client;

    } else if (!memcmp(riid, &IID_IOleInPlaceSite, sizeof(GUID))) {

	*ppvObject = &((_IOleClientSiteEx *)This)->inplace;

    } else {

	*ppvObject = 0;
	return E_NOINTERFACE;
    }

    return S_OK;
}

ULONG STDMETHODCALLTYPE Site_AddRef(IOleClientSite FAR* This)
{
    return 1;
}

ULONG STDMETHODCALLTYPE Site_Release(IOleClientSite FAR* This)
{
    return 1;
}

HRESULT STDMETHODCALLTYPE Site_SaveObject(IOleClientSite FAR* This)
{
    return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE Site_GetMoniker(IOleClientSite FAR* This, DWORD dwAssign, DWORD dwWhichMoniker, IMoniker ** ppmk)
{
    return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE Site_GetContainer(IOleClientSite FAR* This, LPOLECONTAINER FAR* ppContainer)
{
    *ppContainer = 0;
    return E_NOINTERFACE;
}

HRESULT STDMETHODCALLTYPE Site_ShowObject(IOleClientSite FAR* This)
{
    return NOERROR;
}

HRESULT STDMETHODCALLTYPE Site_OnShowWindow(IOleClientSite FAR* This, BOOL fShow)
{
    return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE Site_RequestNewObjectLayout(IOleClientSite FAR* This)
{
    return E_NOTIMPL;
}
HRESULT STDMETHODCALLTYPE InPlaceSite_QueryInterface(IOleInPlaceSite FAR* This, REFIID riid, void ** ppvObject)
{

    if (!memcmp(riid, &IID_IUnknown, sizeof(GUID))
	    || !memcmp(riid, &IID_IOleClientSite, sizeof(GUID))) {

	*ppvObject = &((_IOleClientSiteEx *)This)->client;

    } else if (!memcmp(riid, &IID_IOleInPlaceSite, sizeof(GUID))) {

	*ppvObject = &((_IOleClientSiteEx *)This)->inplace;

    } else {

	*ppvObject = 0;
	return E_NOINTERFACE;
    }

    return S_OK;
}

ULONG STDMETHODCALLTYPE InPlaceSite_AddRef(IOleInPlaceSite FAR* This)
{
    return 1;
}

ULONG STDMETHODCALLTYPE InPlaceSite_Release(IOleInPlaceSite FAR* This)
{
    return 1;
}

HRESULT STDMETHODCALLTYPE InPlaceSite_GetWindow(IOleInPlaceSite FAR* This, HWND FAR* lphwnd)
{
    *lphwnd = ((_IOleInPlaceSiteEx FAR*)This)->frame->window;
    return S_OK;
}

HRESULT STDMETHODCALLTYPE InPlaceSite_ContextSensitiveHelp(IOleInPlaceSite FAR* This, BOOL fEnterMode)
{
    return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE InPlaceSite_CanInPlaceActivate(IOleInPlaceSite FAR* This)
{
    return S_OK;
}

HRESULT STDMETHODCALLTYPE InPlaceSite_OnInPlaceActivate(IOleInPlaceSite FAR* This)
{
    return S_OK;
}

HRESULT STDMETHODCALLTYPE InPlaceSite_OnUIActivate(IOleInPlaceSite FAR* This)
{
    return S_OK;
}

HRESULT STDMETHODCALLTYPE InPlaceSite_GetWindowContext(IOleInPlaceSite FAR* This, LPOLEINPLACEFRAME FAR* lplpFrame, LPOLEINPLACEUIWINDOW FAR* lplpDoc, LPRECT lprcPosRect, LPRECT lprcClipRect, LPOLEINPLACEFRAMEINFO lpFrameInfo)
{

    *lplpDoc = NULL;
    *lplpFrame = (LPOLEINPLACEFRAME)((_IOleInPlaceSiteEx FAR*)This)->frame;

    lpFrameInfo->fMDIApp = FALSE;
    lpFrameInfo->hwndFrame = ((IOleInPlaceFrameEx FAR*)*lplpFrame)->window;
    lpFrameInfo->haccel = 0;
    lpFrameInfo->cAccelEntries = 0;

    GetClientRect(lpFrameInfo->hwndFrame, lprcPosRect);
    GetClientRect(lpFrameInfo->hwndFrame, lprcClipRect);
    return S_OK;
}

HRESULT STDMETHODCALLTYPE InPlaceSite_Scroll(IOleInPlaceSite FAR* This, SIZE scrollExtent)
{
    return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE InPlaceSite_OnUIDeactivate(IOleInPlaceSite FAR* This, BOOL fUndoable)
{
    return S_OK;
}

HRESULT STDMETHODCALLTYPE InPlaceSite_OnInPlaceDeactivate(IOleInPlaceSite FAR* This)
{
    return S_OK;
}

HRESULT STDMETHODCALLTYPE InPlaceSite_DiscardUndoState(IOleInPlaceSite FAR* This)
{
    return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE InPlaceSite_DeactivateAndUndo(IOleInPlaceSite FAR* This)
{
    return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE InPlaceSite_OnPosRectChange(IOleInPlaceSite FAR* This, LPCRECT lprcPosRect)
{
    return S_OK;
}

HRESULT STDMETHODCALLTYPE Frame_QueryInterface(IOleInPlaceFrame FAR* This, REFIID riid, LPVOID FAR* ppvObj)
{
    return E_NOTIMPL;
}

ULONG STDMETHODCALLTYPE Frame_AddRef(IOleInPlaceFrame FAR* This)
{
    return 1;
}

ULONG STDMETHODCALLTYPE Frame_Release(IOleInPlaceFrame FAR* This)
{
    return 1;
}

HRESULT STDMETHODCALLTYPE Frame_GetWindow(IOleInPlaceFrame FAR* This, HWND FAR* lphwnd)
{
    *lphwnd = ((IOleInPlaceFrameEx FAR*)This)->window;
    return S_OK;
}

HRESULT STDMETHODCALLTYPE Frame_ContextSensitiveHelp(IOleInPlaceFrame FAR* This, BOOL fEnterMode)
{
    return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE Frame_GetBorder(IOleInPlaceFrame FAR* This, LPRECT lprectBorder)
{
    return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE Frame_RequestBorderSpace(IOleInPlaceFrame FAR* This, LPCBORDERWIDTHS pborderwidths)
{
    return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE Frame_SetBorderSpace(IOleInPlaceFrame FAR* This, LPCBORDERWIDTHS pborderwidths)
{
    return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE Frame_SetActiveObject(IOleInPlaceFrame FAR* This, IOleInPlaceActiveObject *pActiveObject, LPCOLESTR pszObjName)
{
    return S_OK;
}

HRESULT STDMETHODCALLTYPE Frame_InsertMenus(IOleInPlaceFrame FAR* This, HMENU hmenuShared, LPOLEMENUGROUPWIDTHS lpMenuWidths)
{
    return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE Frame_SetMenu(IOleInPlaceFrame FAR* This, HMENU hmenuShared, HOLEMENU holemenu, HWND hwndActiveObject)
{
    return S_OK;
}

HRESULT STDMETHODCALLTYPE Frame_RemoveMenus(IOleInPlaceFrame FAR* This, HMENU hmenuShared)
{
    return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE Frame_SetStatusText(IOleInPlaceFrame FAR* This, LPCOLESTR pszStatusText)
{
    return S_OK;
}

HRESULT STDMETHODCALLTYPE Frame_EnableModeless(IOleInPlaceFrame FAR* This, BOOL fEnable)
{
    return S_OK;
}

HRESULT STDMETHODCALLTYPE Frame_TranslateAccelerator(IOleInPlaceFrame FAR* This, LPMSG lpmsg, WORD wID)
{
    return E_NOTIMPL;
}

int WebBrowserPageHome(IWebBrowser2 *webBrowser2, IHTMLDocument2 *pDoc2, BSTR str)
{

    if (pDoc2 == NULL)
	return FALSE;

    IHTMLElement *pElemkw = NULL;
    IHTMLElement *pElemsu = NULL;
    BSTR IDkw = SysAllocString(L"kw");
    BSTR IDsu = SysAllocString(L"su");

    IHTMLElementCollection *pColl = NULL;
    pDoc2->lpVtbl->get_all(pDoc2, &pColl);
    if (pColl == NULL)
	return FALSE;

    LONG i, ICount = 0;
    if (pColl->lpVtbl->get_length(pColl, &ICount) != 0)
	return FALSE;

    for (i = 0; i < ICount; i++) {

	VARIANT vIndex;
	VariantInit(&vIndex);
	V_VT(&vIndex) = VT_I4;
	V_I4(&vIndex) = i;

	IDispatch *pCollDisp = NULL;
	pColl->lpVtbl->item(pColl, vIndex, vIndex, &pCollDisp);
	if (pCollDisp == NULL)
	    return FALSE;

	IHTMLElement *pElem = NULL;
	pCollDisp->lpVtbl->QueryInterface(pCollDisp, &IID_IHTMLElement, (void**)&pElem);
	if (pElem == NULL)
	    return FALSE;

	BSTR temp;
	if (pElem->lpVtbl->get_id(pElem, &temp) != 0)
	    return FALSE;

	if (temp && wcscmp(IDkw, temp) == 0) {

	    pElemkw = pElem;

	} else if (temp && wcscmp(IDsu, temp) == 0) {

	    pElemsu = pElem;
	}

	if (pElemkw && pElemsu) {

	    pElemkw->lpVtbl->put_innerText(pElemkw, str);
	    pElemsu->lpVtbl->click(pElemsu);

	    SysFreeString(IDkw);
	    SysFreeString(IDsu);
	    return TRUE;
	}
    }

    SysFreeString(IDkw);
    SysFreeString(IDsu);
    return FALSE;
}

int WebBrowserPage(HWND hwnd, IWebBrowser2 *webBrowser2, IHTMLDocument2 *pDoc2, BSTR str)
{

    if (pDoc2 == NULL)
	return FALSE;

    IHTMLElementCollection *pColl = NULL;
    pDoc2->lpVtbl->get_links(pDoc2, &pColl);
    if (pColl == NULL)
	return FALSE;

    LONG i, ICount = 0;
    if (pColl->lpVtbl->get_length(pColl, &ICount) != 0)
	return FALSE;

    for (i = 0; i < ICount; i++) {

	VARIANT vIndex;
	VariantInit(&vIndex);
	V_VT(&vIndex) = VT_I4;
	V_I4(&vIndex) = i;

	IDispatch *pCollDisp = NULL;
	pColl->lpVtbl->item(pColl, vIndex, vIndex, &pCollDisp);
	if (pCollDisp == NULL)
	    return FALSE;

	IHTMLElement *pElem = NULL;
	pCollDisp->lpVtbl->QueryInterface(pCollDisp, &IID_IHTMLElement, (void**)&pElem);
	if (pElem == NULL)
	    return FALSE;

	BSTR temp;
	if (pElem->lpVtbl->get_innerText(pElem, &temp) != 0)
	    return FALSE;

	if (str && temp && wcscmp(str, temp) == 0) {


	    VARIANT value;
	    value.vt = VT_BSTR;
	    value.bstrVal = SysAllocString(NULL);
	    //pElem->lpVtbl->put_onclick(pElem, value);
	    //pElem->lpVtbl->put_onmousemove(pElem, value);
	    //pElem->lpVtbl->put_onmousedown(pElem, value);
	    //pElem->lpVtbl->put_onmouseup(pElem, value);
	    //pElem->lpVtbl->removeAttribute(pElem, L"target", 0, NULL);

	    pElem->lpVtbl->get_outerHTML(pElem, &temp);
	    wprintf(L"%s\n", temp);

	    //pElem->lpVtbl->getAttribute(pElem, L"href", 0, &value);
	    //wprintf(L"href=%s\n", value.bstrVal);

	    //pElem->lpVtbl->getAttribute(pElem, L"data-click", 0, &value);
	    //wprintf(L"data-click=%s\n", value.bstrVal);

	    //webBrowser2->lpVtbl->Navigate2(webBrowser2, &value, 0, 0, 0, 0);
	    //webBrowser2->lpVtbl->Release(webBrowser2);

	    //LPARAM lParam = MAKEWPARAM(1000, 1000);
	    //PostMessage(hwnd, WM_LBUTTONDOWN, 1, lParam);

	    pElem->lpVtbl->click(pElem);
	    return TRUE;
	}
    }

    return FALSE;
}

void WebBrowserClose(HWND hwnd)
{

    IOleObject *browserObject = NULL;
    IOleObject **browserHandle = NULL;

    if ((browserHandle = (IOleObject **)GetWindowLong(hwnd, GWL_USERDATA))) {

	browserObject = *browserHandle;
	browserObject->lpVtbl->Close(browserObject, OLECLOSE_NOSAVE);
	browserObject->lpVtbl->Release(browserObject);
	GlobalFree(browserHandle);
    }
}

IWebBrowser2 *WebBrowserOpen(HWND hwnd)
{

    void *ptr = NULL;
    int ptr_size = sizeof(IOleInPlaceFrameEx) + sizeof(_IOleClientSiteEx);
    if (!(ptr = GlobalAlloc(GMEM_FIXED, ptr_size)))
	return NULL;

    IOleInPlaceFrameEx *iOleInPlaceFrameEx = NULL;
    iOleInPlaceFrameEx = (IOleInPlaceFrameEx *)ptr;
    iOleInPlaceFrameEx->frame.lpVtbl = &MyIOleInPlaceFrameTable;
    iOleInPlaceFrameEx->window = hwnd;

    _IOleClientSiteEx *_iOleClientSiteEx = NULL;
    _iOleClientSiteEx = (_IOleClientSiteEx *)(ptr + sizeof(IOleInPlaceFrameEx));
    _iOleClientSiteEx->client.lpVtbl = &MyIOleClientSiteTable;
    _iOleClientSiteEx->inplace.inplace.lpVtbl = &MyIOleInPlaceSiteTable;
    _iOleClientSiteEx->inplace.frame = iOleInPlaceFrameEx;

    IOleObject *browserObject = NULL;
    if (OleCreate(&CLSID_WebBrowser,
		&IID_IOleObject,
		0,
		0,
		(IOleClientSite *)_iOleClientSiteEx,
		&MyIStorage,
		(void**)&browserObject) != 0) {

	WebBrowserClose(hwnd);
	GlobalFree(ptr);
	return NULL;
    }

#if 0
    SetWindowLong(hwnd, GWL_USERDATA, (LONG)browserObject);
    browserObject->lpVtbl->SetHostNames(browserObject, L"Baidu Host Name", 0);
    if (OleSetContainedObject((struct IUnknown *)browserObject, TRUE) != 0) {

	WebBrowserClose(hwnd);
	GlobalFree(ptr);
	return NULL;
    }
#endif

    if (browserObject->lpVtbl->DoVerb(browserObject,
		OLEIVERB_SHOW,
		NULL,
		(IOleClientSite *)_iOleClientSiteEx,
		-1,
		hwnd,
		NULL) != 0) {

	WebBrowserClose(hwnd);
	GlobalFree(ptr);
	return NULL;
    }

    IWebBrowser2 *webBrowser2 = NULL;
    if (browserObject->lpVtbl->QueryInterface(browserObject,
		&IID_IWebBrowser2,
		(void**)&webBrowser2) != 0) {

	WebBrowserClose(hwnd);
	GlobalFree(ptr);
	return NULL;
    }

    VARIANT url;
    VariantInit(&url);
    url.vt = VT_BSTR;
    url.bstrVal = SysAllocString(L"http://www.baidu.com");

    webBrowser2->lpVtbl->Navigate2(webBrowser2, &url, 0, 0, 0, 0);
    webBrowser2->lpVtbl->Release(webBrowser2);
    VariantClear(&url);
    return webBrowser2;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{

    switch(uMsg){

	case WM_CREATE:
	    return TRUE;

	case WM_DESTROY:
	    WebBrowserClose(hwnd);
	    PostQuitMessage(0);
	    return TRUE;
    }

    return(DefWindowProc(hwnd, uMsg, wParam, lParam));
}

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hInstNULL, LPSTR lpszCmdLine, int nCmdShow)
{

    MSG msg;
    WNDCLASSEX wc;
    char ClassName[] = "WebBrowser";

    setlocale(LC_CTYPE, "chs");
    if (OleInitialize(NULL) != S_OK) {

	MessageBoxA(0, "can't open OLE!", "error", MB_OK);
	return FALSE;
    }

    ZeroMemory(&wc, sizeof(WNDCLASSEX));
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.hInstance = hInstance;
    wc.lpfnWndProc = WindowProc;
    wc.lpszClassName = ClassName;
    RegisterClassEx(&wc);

    msg.hwnd = CreateWindowEx(0,
	    ClassName,
	    ClassName,
	    WS_OVERLAPPEDWINDOW,
	    CW_USEDEFAULT,
	    0,
	    CW_USEDEFAULT,
	    0,
	    HWND_DESKTOP,
	    NULL,
	    hInstance,
	    0);

    ShowWindow(msg.hwnd, nCmdShow);
    UpdateWindow(msg.hwnd);
    IWebBrowser2 *webBrowser2 = WebBrowserOpen(msg.hwnd);

    BSTR keyword = SysAllocString(L"lcglcg");
    BSTR keyurl = SysAllocString(L"临沂春光磁业有限公司");
    //goto loop;

    while (GetMessage(&msg, 0, 0, 0)) {

	TranslateMessage(&msg);
	DispatchMessage(&msg);

	IDispatch *dispathch = NULL;
	webBrowser2->lpVtbl->get_Document(webBrowser2, &dispathch);
	if (dispathch) {

	    IHTMLDocument2 *htmldocument2 = NULL;
	    dispathch->lpVtbl->QueryInterface(dispathch, &IID_IHTMLDocument2, (void**)&htmldocument2);
	    if (htmldocument2) {

		VARIANT_BOOL busy;
		HRESULT hr = webBrowser2->lpVtbl->get_Busy(webBrowser2, &busy);
		if (SUCCEEDED(hr) && busy == 0)
		    if (WebBrowserPageHome(webBrowser2, htmldocument2, keyword) == TRUE)
			break;
	    }
	}
    }

    goto loop;
loop:
    while (GetMessage(&msg, 0, 0, 0)) {

	TranslateMessage(&msg);
	DispatchMessage(&msg);

	IDispatch *dispathch = NULL;
	webBrowser2->lpVtbl->get_Document(webBrowser2, &dispathch);
	if (dispathch) {

	    IHTMLDocument2 *htmldocument2 = NULL;
	    dispathch->lpVtbl->QueryInterface(dispathch, &IID_IHTMLDocument2, (void**)&htmldocument2);
	    if (htmldocument2) {

		VARIANT_BOOL busy;
		HRESULT hr = webBrowser2->lpVtbl->get_Busy(webBrowser2, &busy);
		if (SUCCEEDED(hr) && busy == 0)
		    if (WebBrowserPage(msg.hwnd, webBrowser2, htmldocument2, keyurl) == TRUE)
			break;
	    }
	}
    }

    while (GetMessage(&msg, 0, 0, 0)) {

	TranslateMessage(&msg);
	DispatchMessage(&msg);
    }

    OleUninitialize();
    return 0;
}


