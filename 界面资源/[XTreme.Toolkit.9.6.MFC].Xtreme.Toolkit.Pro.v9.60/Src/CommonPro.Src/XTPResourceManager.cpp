// XTPResourceManager.cpp: implementation of the CXTPResourceManager class.
//
// This file is a part of the XTREME TOOLKIT PRO MFC class library.
// ©1998-2005 Codejock Software, All Rights Reserved.
//
// THIS SOURCE FILE IS THE PROPERTY OF CODEJOCK SOFTWARE AND IS NOT TO BE
// RE-DISTRIBUTED BY ANY MEANS WHATSOEVER WITHOUT THE EXPRESSED WRITTEN
// CONSENT OF CODEJOCK SOFTWARE.
//
// THIS SOURCE CODE CAN ONLY BE USED UNDER THE TERMS AND CONDITIONS OUTLINED
// IN THE XTREME TOOLKIT PRO LICENSE AGREEMENT. CODEJOCK SOFTWARE GRANTS TO
// YOU (ONE SOFTWARE DEVELOPER) THE LIMITED RIGHT TO USE THIS SOFTWARE ON A
// SINGLE COMPUTER.
//
// CONTACT INFORMATION:
// support@codejock.com
// http://www.codejock.com
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XTPVC80Helpers.h"
#include "XTPVC50Helpers.h"
#include "XTPManageState.h"
#include "XTPResourceManager.h"
#include "XTPMacros.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CProcessLocal<CXTPResourceManager> CXTPResourceManager::m_managerInstance;

CXTPResourceManager* AFX_CDECL XTPResourceManager()
{
	return CXTPResourceManager::m_managerInstance.GetData();
}

BOOL CXTPResourceManager::SetResourceManager(CXTPResourceManager* pResourceManager)
{
	if (pResourceManager == NULL)
		return FALSE;

	if (!pResourceManager->m_bValid)
	{
		delete pResourceManager;
		return FALSE;

	}

	if (m_managerInstance.m_pObject != NULL)
		delete m_managerInstance.m_pObject;

	m_managerInstance.m_pObject = pResourceManager;
	return TRUE;
}


//////////////////////////////////////////////////////////////////////////
// CXTPResourceManager

CXTPResourceManager::CXTPResourceManager()
{
	m_bValid = TRUE;
}


BOOL CXTPResourceManager::LoadString(CString* pString, UINT nIDResource)
{
	if (::FindResource(CXTPManageState::GetResourceHandle(), MAKEINTRESOURCE((nIDResource>>4)+1), RT_STRING))
	{
		XT_MANAGE_STATE

		if (pString->LoadString(nIDResource))
			return TRUE;
	}

	return pString->LoadString(nIDResource);
}

BOOL CXTPResourceManager::LoadMenu(CMenu* lpMenu, UINT nIDResource)
{
	if (::FindResource(CXTPManageState::GetResourceHandle(), MAKEINTRESOURCE(nIDResource), RT_MENU))
	{
		XT_MANAGE_STATE

		if (lpMenu->LoadMenu(nIDResource))
			return TRUE;
	}

	return lpMenu->LoadMenu(nIDResource);
}

BOOL CXTPResourceManager::LoadToolBar(CToolBar* pToolBar, UINT nIDResource)
{
	XT_MANAGE_STATE

	return pToolBar->LoadToolBar(nIDResource);
}

BOOL CXTPResourceManager::LoadBitmap(CBitmap* pBitmap, UINT nIDResource)
{
	if (::FindResource(CXTPManageState::GetResourceHandle(), MAKEINTRESOURCE(nIDResource), RT_BITMAP))
	{
		XT_MANAGE_STATE

		if (pBitmap->LoadBitmap(nIDResource))
			return TRUE;

	}

	return pBitmap->LoadBitmap(nIDResource);
}

LPCDLGTEMPLATE CXTPResourceManager::LoadDialogTemplate(LPCTSTR pszTemplate)
{
	XT_MANAGE_STATE

	HINSTANCE hInst = AfxFindResourceHandle(pszTemplate, RT_DIALOG);
	if (hInst == NULL)
		return NULL;

	HRSRC hResource = FindResource(hInst, pszTemplate, RT_DIALOG);
	if (!hResource)
		return NULL;

	HGLOBAL hTemplate = LoadResource(hInst, hResource);
	return (LPCDLGTEMPLATE)LockResource(hTemplate);
}

int CXTPResourceManager::ShowMessageBox (UINT nIDPrompt, UINT nType)
{
	CString strPrompt;
	VERIFY(LoadString(&strPrompt, nIDPrompt));

	return AfxMessageBox(strPrompt, nType);
}



//////////////////////////////////////////////////////////////////////////
// Utils

struct CXTPResourceManagerText::DLGITEMTEMPLATEINFO : public DLGITEMTEMPLATE
{
	CString strCaption;
	CString strClassName;
	int nCaptionID;

public:
	ULONG AlignDWord(ULONG uLong) const {  return ((uLong + 3) & ~3); }

	UINT GetLength() const {
		return AlignDWord(sizeof(DLGITEMTEMPLATE)
			+ strCaption.GetLength() * sizeof(WORD) +  sizeof(WORD)
			+ strClassName.GetLength() * sizeof(WORD) +  sizeof(WORD)
			+ 0
			+ sizeof(WORD)
		);
	}
};

struct CXTPResourceManagerText::DLGTEMPLATEINFO : public DLGTEMPLATE
{
	CString strCaption;
	CString strFaceName;
	int nPointSize;

	CArray<DLGITEMTEMPLATEINFO, DLGITEMTEMPLATEINFO&> aItems;

public:
	ULONG AlignDWord(ULONG uLong) const {  return ((uLong + 3) & ~3); }

	UINT GetHeaderLength() const {
		return AlignDWord( sizeof(DLGTEMPLATE)
			+ sizeof(WORD)
			+ sizeof(WORD)
			+ (strCaption.GetLength() * sizeof(WORD) +  sizeof(WORD))
			+ (style & DS_SETFONT ? strFaceName.GetLength() * sizeof(WORD) + sizeof(WORD) * 2: 0)
		);
	}

	UINT GetLength() const {
		int nLength = GetHeaderLength();

		for (int i = 0; i < aItems.GetSize(); i++)
		{
			nLength += aItems[i].GetLength();
		}

		return nLength;
	}
};

LPCDLGTEMPLATE CXTPResourceManagerText::CreateDialogTemplate(DLGTEMPLATEINFO& dlgTemplate)
{
	HLOCAL hDlgTemplate = LocalAlloc(LHND, dlgTemplate.GetLength());
	LPVOID lpDlgTemplate = LocalLock(hDlgTemplate);

	*(DLGTEMPLATE*)lpDlgTemplate = dlgTemplate;

	BYTE* pMain = (BYTE*) lpDlgTemplate + sizeof(DLGTEMPLATE) + sizeof(WORD) * 2;

	// Caption
	MBSTOWCS_S((LPWSTR)pMain, dlgTemplate.strCaption, dlgTemplate.strCaption.GetLength() + 1);
	pMain += (dlgTemplate.strCaption.GetLength() + 1) * sizeof(WORD);


	if (dlgTemplate.style & DS_SETFONT)
	{
		*((WORD *)pMain) = (WORD)dlgTemplate.nPointSize;
		pMain += sizeof(WORD);

		MBSTOWCS_S((LPWSTR)pMain, dlgTemplate.strFaceName, dlgTemplate.strFaceName.GetLength() + 1);
	}

	pMain = (BYTE*) lpDlgTemplate + dlgTemplate.GetHeaderLength();
	ASSERT(dlgTemplate.cdit == dlgTemplate.aItems.GetSize());


	CArray<DLGITEMTEMPLATEINFO, DLGITEMTEMPLATEINFO&>& aItems = dlgTemplate.aItems;

	for (int i = 0; i < aItems.GetSize(); i++)
	{
		DLGITEMTEMPLATEINFO& itemInfo = aItems[i];

		// Copy constant part of the template
		*(DLGITEMTEMPLATE*)pMain = itemInfo;
		BYTE* pControlMain = pMain + sizeof(DLGITEMTEMPLATE);

		// Class
		MBSTOWCS_S((LPWSTR)pControlMain, itemInfo.strClassName, itemInfo.strClassName.GetLength() + 1);
		pControlMain += (itemInfo.strClassName.GetLength() + 1) * sizeof(WORD);


		// Copy Caption
		if (itemInfo.nCaptionID != 0)
		{
			*((PWORD)pControlMain) = 0xFFFF;
			*((PWORD)pControlMain + 1) = (WORD)itemInfo.nCaptionID;

			ASSERT (itemInfo.strCaption.GetLength() == 1);
		}
		else
		{
			MBSTOWCS_S((LPWSTR)pControlMain, itemInfo.strCaption, itemInfo.strCaption.GetLength() + 1);
		}
		pControlMain += (itemInfo.strCaption.GetLength() + 1) * sizeof(WORD);

		// Init Data length
		*((WORD *)pControlMain) = 0;
		pControlMain += sizeof(WORD);

		pMain += itemInfo.GetLength();
	}

	return (LPCDLGTEMPLATE)lpDlgTemplate;
}

LPCDLGTEMPLATE CXTPResourceManagerText::CreateDialogTemplate(CString strTemplate)
{
	DLGTEMPLATEINFO dlgTemplate;

	int nResult = SCANF_S(strTemplate, _T("DIALOG %d, %d, %d, %d CAPTION \"%[^\"]\" STYLE %x FONT %d, \"%[^\"]\\"),
		&dlgTemplate.x, &dlgTemplate.y, &dlgTemplate.cx, &dlgTemplate.cy, SCANF_PARAM_S(dlgTemplate.strCaption.GetBufferSetLength(255), 255),
		&dlgTemplate.style, &dlgTemplate.nPointSize, SCANF_PARAM_S(dlgTemplate.strFaceName.GetBufferSetLength(255), 255));

	dlgTemplate.strCaption.ReleaseBuffer();
	dlgTemplate.strFaceName.ReleaseBuffer();
	dlgTemplate.dwExtendedStyle = 0;
	dlgTemplate.cdit = 0;
	dlgTemplate.style |= DS_SETFONT;

	if (nResult != 8)
		return NULL;


	TCHAR* lpTemplate = new TCHAR[strTemplate.GetLength() + 1];
	STRCPY_S(lpTemplate, strTemplate.GetLength() + 1, strTemplate);

	CArray<DLGITEMTEMPLATEINFO, DLGITEMTEMPLATEINFO&>& aItems = dlgTemplate.aItems;

	TCHAR* lpszContext = 0;
	TCHAR* token = STRTOK_S(lpTemplate, _T("{\n"), &lpszContext);
	while (token)
	{
		token = STRTOK_S( NULL, _T("{\n"), &lpszContext);
		CString strControl(token);
		strControl.TrimLeft();
		REPLACE_S(strControl, _T("\"\""), _T("\" \""));

		if (strControl.IsEmpty())
			continue;

		if (strControl == _T("}"))
			break;

		DLGITEMTEMPLATEINFO itemInfo;
		itemInfo.nCaptionID = 0;

		int id = 0;

		int nResult = SCANF_S(strControl, _T("%s \"%[^\"]\", %d, %d, %d, %d, %d, %x"),
			SCANF_PARAM_S(itemInfo.strClassName.GetBufferSetLength(255), 255), SCANF_PARAM_S(itemInfo.strCaption.GetBufferSetLength(255), 255),
			&id, &itemInfo.x, &itemInfo.y, &itemInfo.cx, &itemInfo.cy, &itemInfo.style);


		itemInfo.strClassName.ReleaseBuffer();
		itemInfo.strCaption.ReleaseBuffer();

		if (nResult == 1)
		{
			nResult = SCANF_S(strControl, _T("%s %i, %d, %d, %d, %d, %d, %x"),
			SCANF_PARAM_S(itemInfo.strClassName.GetBufferSetLength(255), 255), &itemInfo.nCaptionID,
			&id, &itemInfo.x, &itemInfo.y, &itemInfo.cx, &itemInfo.cy, &itemInfo.style);

			itemInfo.strClassName.ReleaseBuffer();
			itemInfo.strCaption = " ";
		}

		itemInfo.id = (WORD)id;
		itemInfo.dwExtendedStyle = 0;

		if (nResult != 8)
		{
			delete[] lpTemplate;
			return NULL;
		}

		aItems.Add(itemInfo);
		dlgTemplate.cdit ++;
	}

	delete[] lpTemplate;


	return CreateDialogTemplate(dlgTemplate);
}

struct CXTPResourceManagerText::MENUITEMTEMPLATEINFO : public MENUITEMTEMPLATE
{
	CString strCaption;

public:
	UINT GetLength() const {
		return sizeof (mtOption) + sizeof(WCHAR) * (strCaption.GetLength() + 1) +
			(mtOption & MF_POPUP? 0: sizeof (mtID));
	}
};

struct CXTPResourceManagerText::MENUTEMPLATEINFO
{
	CArray<MENUITEMTEMPLATEINFO, MENUITEMTEMPLATEINFO&> aItems;

public:
	UINT GetLength() const {
		int nLength = sizeof(MENUITEMTEMPLATEHEADER);

		for (int i = 0; i < aItems.GetSize(); i++)
		{
			nLength += aItems[i].GetLength();
		}

		return nLength;
	}
};


BOOL CXTPResourceManagerText::CreateMenu(CMenu& menu, CString strTemplate)
{
	MENUTEMPLATEINFO menuTemplate;


	CList<int, int&> aLastItems;

	TCHAR* lpTemplate = new TCHAR[strTemplate.GetLength() + 1];
	STRCPY_S(lpTemplate, strTemplate.GetLength() + 1, strTemplate);

	TCHAR* lpszContext = 0;
	TCHAR* token = STRTOK_S(lpTemplate, _T("\n"), &lpszContext);
	while (token)
	{
		token = STRTOK_S( NULL, _T("\n"), &lpszContext);
		CString strControl(token);
		strControl.TrimLeft();
		REPLACE_S(strControl, _T("\"\""), _T("\" \""));

		if (strControl.IsEmpty())
			continue;

		if (strControl == _T("}"))
		{
			ASSERT(aLastItems.GetCount() != 0);

			int nIndex = aLastItems.RemoveTail();
			if (aLastItems.GetCount() == 0)
				break;

			menuTemplate.aItems[nIndex].mtOption |= MF_END;

			continue;
		}

		if (strControl == _T("{"))
		{
			int nIndex = max(0, (int) menuTemplate.aItems.GetSize() - 1);
			aLastItems.AddTail(nIndex);
			continue;
		}

		MENUITEMTEMPLATEINFO itemInfo;
		itemInfo.mtOption = 0;

		CString strType;
		int nID = -1;
		int nResult = SCANF_S(strControl, _T("%s \"%[^\"]\", %d"),
			SCANF_PARAM_S(strType.GetBufferSetLength(20), 20), SCANF_PARAM_S(itemInfo.strCaption.GetBufferSetLength(255), 255), &nID);

		itemInfo.mtID = (WORD)nID;
		strType.ReleaseBuffer();
		itemInfo.strCaption.ReleaseBuffer();

		if (strType == _T("POPUP"))
		{
			itemInfo.mtOption |= MF_POPUP;
		}
		else if (strType == _T("MENUITEM"))
		{
			if (nResult == 1)
			{
				itemInfo.mtOption |= MF_SEPARATOR;
				itemInfo.mtID = 0;
			}

		} else
		{
			delete[] lpTemplate;
			return FALSE;
		}

		aLastItems.RemoveTail();
		int nIndex = (int) menuTemplate.aItems.Add(itemInfo);
		aLastItems.AddTail(nIndex);
	}

	delete[] lpTemplate;

	if (menuTemplate.aItems.GetSize() == 0)
		return FALSE;

	ASSERT(menuTemplate.aItems[menuTemplate.aItems.GetSize() -1].mtOption & MF_END);

	int nLength = menuTemplate.GetLength() + sizeof(MENUITEMTEMPLATE);
	LPVOID lpDlgTemplate = malloc(nLength);
	ZeroMemory(lpDlgTemplate, nLength);

	MENUITEMTEMPLATE* mitem = (MENUITEMTEMPLATE*) ((BYTE*)lpDlgTemplate + sizeof(MENUITEMTEMPLATEHEADER));

	for (int i = 0; i < menuTemplate.aItems.GetSize(); i++ )
	{
		MENUITEMTEMPLATEINFO& itemInfo = menuTemplate.aItems[i];
		mitem->mtOption  = itemInfo.mtOption;

		if (itemInfo.mtOption & MF_POPUP )
		{
			MBSTOWCS_S((WCHAR*)((BYTE*)mitem + sizeof (mitem->mtOption)), itemInfo.strCaption, itemInfo.strCaption.GetLength() + 1);
		}
		else
		{
			mitem->mtID = itemInfo.mtID;
			MBSTOWCS_S(mitem->mtString, itemInfo.strCaption, itemInfo.strCaption.GetLength() + 1);
		}

		mitem = (MENUITEMTEMPLATE*) ((BYTE*)mitem + itemInfo.GetLength());
	}
	mitem->mtOption = MF_END;

	BOOL bResult = menu.LoadMenuIndirect(lpDlgTemplate);

	free(lpDlgTemplate);

	return bResult;
}




//////////////////////////////////////////////////////////////////////////
// CXTPResourceManagerText

CXTPResourceManagerText::CXTPResourceManagerText(CString strFileName)
{
	m_bValid = (ACCESS_S(strFileName, 0) != -1);
	m_strFileName = strFileName;
}

BOOL CXTPResourceManagerText::LoadString(CString* pString, UINT nIDResource)
{
	if (LoadResource(pString, nIDResource, _T("string")))
		return TRUE;

	return CXTPResourceManager::LoadString(pString, nIDResource);
}


LPCDLGTEMPLATE CXTPResourceManagerText::LoadDialogTemplate(LPCTSTR pszResource)
{
	UINT nIDResource = (UINT)(DWORD_PTR)pszResource;
	CString strTemplate;

	if (LoadResource(&strTemplate, nIDResource, _T("dialog")))
	{
		LPCDLGTEMPLATE pTemplate =  CreateDialogTemplate(strTemplate);

		if (pTemplate)
			return pTemplate;
	}

	return CXTPResourceManager::LoadDialogTemplate(pszResource);
}

BOOL CXTPResourceManagerText::LoadMenu(CMenu* lpMenu, UINT nIDResource)
{
	CString strTemplate;

	if (LoadResource(&strTemplate, nIDResource, _T("menu")))
	{
		if (CreateMenu(*lpMenu, strTemplate))
			return TRUE;
	}
	return CXTPResourceManager::LoadMenu(lpMenu, nIDResource);
}

BOOL CXTPResourceManagerText::LoadResource(CString* pString, UINT nIDResource, CString strResourceType)
{
	ASSERT(m_bValid);

	CString strResource;
	strResource.Format(_T("%i"), nIDResource);

	CString strReturn;

	if (!GetPrivateProfileString(strResource, strResourceType, _T(""), strReturn.GetBufferSetLength(2550), 2550, m_strFileName))
	{
		strReturn.ReleaseBuffer();
		return FALSE;
	}

	strReturn.ReleaseBuffer();
	REPLACE_S(strReturn, _T("\\n"), _T("\n"));
	REPLACE_S(strReturn, _T("\\t"), _T("\t"));

	*pString = strReturn;
	return TRUE;
}

#ifndef  _XTP_EXCLUDE_XML

//////////////////////////////////////////////////////////////////////////
// CXTPResourceManagerXML

CXTPResourceManagerXML::CXTPResourceManagerXML(CString strFileName)
	: CXTPResourceManagerText(strFileName)
{
	OleInitialize(NULL);


	CXTPDOMElementPtr rootPtr = 0;

	if (!LoadDocument(strFileName, rootPtr, TRUE))
	{
		m_bValid = FALSE;
	}

}

BOOL CXTPResourceManagerXML::LoadDocument(CString strFileName, CXTPDOMElementPtr& rootPtr, BOOL bCheckRootName /*= FALSE*/)
{
	CXTPDOMDocumentPtr docPtr = 0;

	if (!SUCCEEDED(docPtr.CreateInstance(CLSID_XTPDOMDocument)))
		return FALSE;

	VARIANT_BOOL bResult;
	HRESULT hr = docPtr->load(COleVariant(strFileName), &bResult);
	if (FAILED(hr) || bResult == VARIANT_FALSE)
		return FALSE;

	docPtr->get_documentElement(&rootPtr);
	if (rootPtr == 0)
		return FALSE;

	if (!bCheckRootName)
		return TRUE;

	BSTR bstrRootName = 0;
	if( FAILED(rootPtr->get_nodeName(&bstrRootName))
		|| _bstr_t(L"Resources") != _bstr_t(bstrRootName))
	{
		if (bstrRootName) SysFreeString(bstrRootName);
		return FALSE;
	}
	if (bstrRootName) SysFreeString(bstrRootName);

	return TRUE;
}

CXTPResourceManagerXML::~CXTPResourceManagerXML()
{
	OleUninitialize();
}


BOOL CXTPResourceManagerXML::LoadResource(CString* pString, UINT nIDResource, CString strResourceType)
{
	CXTPDOMElementPtr rootPtr = 0;

	if (!LoadDocument(m_strFileName, rootPtr))
		return FALSE;

	CXTPDOMNodePtr nodeResource = 0;

	CString strPattern;
	strPattern.Format(_T("Resource[@id = \"%i\" and @type = \"%s\"]"), nIDResource, strResourceType);

	rootPtr->selectSingleNode(_bstr_t(strPattern), &nodeResource);
	if (nodeResource == 0)
		return FALSE;

	CXTPDOMNamedNodeMapPtr attributtes = 0;
	nodeResource->get_attributes(&attributtes);
	if (attributtes  == 0)
		return FALSE;


	CXTPDOMNodePtr xmlNodeSectionPtr = 0;
	attributtes->getNamedItem(L"value", &xmlNodeSectionPtr);
	if (xmlNodeSectionPtr == 0)
		return FALSE;

	BSTR bstrValue = NULL;
	xmlNodeSectionPtr->get_text(&bstrValue);
	CString strReturn(bstrValue);
	if (bstrValue) SysFreeString(bstrValue);

	REPLACE_S(strReturn, _T("\\n"), _T("\n"));
	REPLACE_S(strReturn, _T("\\t"), _T("\t"));

	*pString = strReturn;
	return TRUE;
}

#endif
