
#include <stdio.h>
#include <windows.h>
#include <winnls.h>

#include <mshtml.h>
#include <exdisp.h>
#include <exdispid.h>

#include <oleidl.h>
#include <olectlid.h>

typedef struct ClickElementNode ClickElementNode;
struct  ClickElementNode {

    ClickElementNode *next;
    char str[4096];
};

void ClickElementNode_add(ClickElementNode **head, BSTR str)
{

    int str_len;
    if ((str_len = wcslen(str)) > 2048)
	return;

    ClickElementNode *node = NULL;
    if (!(node = malloc(sizeof(ClickElementNode))))
	return;

    memset(node, 0, sizeof(ClickElementNode));
    WideCharToMultiByte(CP_ACP, 0, str, str_len, node->str, 4096, NULL, NULL);

    node->next = *head;
    *head = node;
}

void ClickElement(ClickElementNode **head, IHTMLDocument2 *pIHTMLDocument2)
{

    IHTMLElementCollection *pIHTMLElementCollection = NULL;
    pIHTMLDocument2->lpVtbl->get_links(pIHTMLDocument2, &pIHTMLElementCollection);
    if (pIHTMLElementCollection == NULL)
	return;

    LONG i, iCount = 0;
    pIHTMLElementCollection->lpVtbl->get_length(pIHTMLElementCollection, &iCount);

    for (i = 0; i < iCount; i++) {

	VARIANT vIndex;
	VariantInit(&vIndex);
	V_VT(&vIndex) = VT_I4;
	V_I4(&vIndex) = i;

	IDispatch *pIDispatch = NULL;
	pIHTMLElementCollection->lpVtbl->item(pIHTMLElementCollection, vIndex, vIndex, &pIDispatch);
	if (pIDispatch == NULL)
	    return;

	IHTMLElement *pIHTMLElement = NULL;
	pIDispatch->lpVtbl->QueryInterface(pIDispatch, &IID_IHTMLElement, (void**)&pIHTMLElement);
	if (pIHTMLElement == NULL)
	    return;

	BSTR str;
	pIHTMLElement->lpVtbl->get_innerText(pIHTMLElement, &str);
	if (str) ClickElementNode_add(head, str);
    }
}

void ClickElementForeach(ClickElementNode **head, IHTMLDocument2 *pIHTMLDocument2)
{

    if (pIHTMLDocument2 == NULL) return;
    ClickElement(head, pIHTMLDocument2);

    IOleContainer *pIOleContainer = NULL;
    pIHTMLDocument2->lpVtbl->QueryInterface(pIHTMLDocument2, &IID_IOleContainer, (void**)&pIOleContainer);
    if (pIOleContainer == NULL)
	return;

    IEnumUnknown *pIEnumUnknown = NULL;
    pIOleContainer->lpVtbl->EnumObjects(pIOleContainer, 1, &pIEnumUnknown);
    if (pIEnumUnknown == NULL)
	return;

    IUnknown *pIUnknown = NULL;
    while(pIEnumUnknown->lpVtbl->Next(pIEnumUnknown, 1, &pIUnknown, NULL) == S_OK) {

	IWebBrowser2 *_pIWebBrowser2 = NULL;
	pIUnknown->lpVtbl->QueryInterface(pIUnknown, &IID_IWebBrowser2, (void**)&_pIWebBrowser2);
	if (_pIWebBrowser2 == NULL)
	    return;

	IDispatch *_pDispatch;
	_pIWebBrowser2->lpVtbl->get_Document(_pIWebBrowser2, &_pDispatch);
	if (_pDispatch == NULL)
	    return;

	IHTMLDocument2 *_pIHTMLDocument2 = NULL;
	_pDispatch->lpVtbl->QueryInterface(_pDispatch, &IID_IHTMLDocument2, (void**)&_pIHTMLDocument2);
	if (_pIHTMLDocument2 == NULL) 
	    return;

	ClickElement(head, _pIHTMLDocument2);
	ClickElementForeach(head, _pIHTMLDocument2);
    }
}

ClickElementNode *GetClickElement(IHTMLDocument2 *pIHTMLDocument2)
{

    ClickElementNode *head = NULL;
    ClickElementForeach(&head, pIHTMLDocument2);
    return head;
}

void GetClickElementFree(ClickElementNode *head)
{

    ClickElementNode *node = head;
    ClickElementNode *temp = NULL;

    while(node) {

	temp = node;
	node = node->next;
	free(temp);
    }
}

void demo(char *str) 
{

    wchar_t buffer[1024] = {0};
    MultiByteToWideChar(CP_ACP, 0, str, strlen(str), buffer, 1024);
    MessageBoxW(NULL, buffer, L"W", MB_OK);
}


