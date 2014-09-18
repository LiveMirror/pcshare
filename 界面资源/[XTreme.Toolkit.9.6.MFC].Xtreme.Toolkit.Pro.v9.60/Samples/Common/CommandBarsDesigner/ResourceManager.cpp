// ResourceManager.cpp: implementation of the CResourceManager class.
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
#include "Resource.h"
#include "ResourceManager.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

struct STANDARDRESOURCE
{
	LPCTSTR lpstrCaption;
	UINT nValue;
};

const STANDARDRESOURCE lpStandardResources[] =
{
	_T("ID_FILE_NEW"),          ID_FILE_NEW,
	_T("ID_FILE_OPEN"),         ID_FILE_OPEN,
	_T("ID_FILE_CLOSE"),        ID_FILE_CLOSE,
	_T("ID_FILE_SAVE"),         ID_FILE_SAVE,
	_T("ID_FILE_SAVE_AS"),      ID_FILE_SAVE_AS,
	_T("ID_FILE_PRINT"),        ID_FILE_PRINT,
	_T("ID_APP_EXIT"),          ID_APP_EXIT,
	_T("ID_EDIT_UNDO"),         ID_EDIT_UNDO,
	_T("ID_EDIT_CUT"),          ID_EDIT_CUT,
	_T("ID_EDIT_COPY"),         ID_EDIT_COPY,
	_T("ID_EDIT_PASTE"),        ID_EDIT_PASTE,
	_T("ID_VIEW_STATUS_BAR"),   ID_VIEW_STATUS_BAR,
	_T("ID_WINDOW_CASCADE"),    ID_WINDOW_CASCADE,
	_T("ID_WINDOW_TILE_HORZ"),  ID_WINDOW_TILE_HORZ,
	_T("ID_WINDOW_ARRANGE"),    ID_WINDOW_ARRANGE,
	_T("ID_APP_ABOUT"),         ID_APP_ABOUT,

	_T("IDR_MENUBAR"),          1,
	_T("IDR_STANDARD"),         IDR_EMBEDDEDFRAME,

	_T("XTP_ID_WINDOWLIST"),        XTP_ID_WINDOWLIST,
	_T("XTP_ID_CUSTOMIZE"),         XTP_ID_CUSTOMIZE,
	_T("XTP_ID_TOOLBARLIST"),       ID_VIEW_TOOLBAR
};


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CResourceManager::CResourceManager()
{
	m_nNextID = 1020;

	for (int i = 0; i < _countof(lpStandardResources); i++)
	{
		m_mapResources.SetAt(lpStandardResources[i].lpstrCaption, new CResourceInfo(lpStandardResources[i].nValue, TRUE));
	}

}

CResourceManager::~CResourceManager()
{
	FreeAll();
}

void CResourceManager::FreeAll()
{
	POSITION pos = m_mapResources.GetStartPosition();
	while (pos)
	{
		CResourceInfo* pInfo;
		CString strCaption;
		m_mapResources.GetNextAssoc(pos, strCaption, (CObject*&)pInfo);
		delete pInfo;
	}
	m_mapResources.RemoveAll();

}

UINT CResourceManager::GetNextID()
{
	return m_nNextID++;
}


UINT CResourceManager::GetStringID(CString str)
{
	CResourceInfo* pInfo;

	if (m_mapResources.Lookup(str, (CObject*&)pInfo))
	{
		return pInfo->m_nID;

	}

	REMOVE_S(str, _T(' '));
	if (str == _T("0")) return 0;
	if (str == _T("")) return 0;

	int i = _ttoi(str);
	CString strInteger;
	strInteger.Format(_T("%i"), i);
	if (strInteger == str)
		return i;



	UINT nId = GetNextID();
	m_mapResources.SetAt(str, new CResourceInfo(nId));
	return nId;

}

CResourceInfo* CResourceManager::Find(UINT nID)
{
	POSITION pos = m_mapResources.GetStartPosition();
	while (pos)
	{
		CResourceInfo* pInfo;
		CString strCaption;
		m_mapResources.GetNextAssoc(pos, strCaption, (CObject*&)pInfo);

		pInfo->m_strCaption = strCaption;

		if (pInfo->m_nID == nID) return pInfo;
	}
	return NULL;
}


CString CResourceManager::GetStringID(UINT nID)
{
	if (nID == 0) return _T("");

	CResourceInfo* pInfo = Find(nID);

	if (pInfo)
		return pInfo->m_strCaption;

	CString strInteger;
	strInteger.Format(_T("%i"), nID);

	return strInteger == _T("-1")? _T(""): strInteger;
}

void CResourceManager::DoPropExchange(CXTPPropExchange* pPX)
{
	PX_DWord(pPX, _T("NextId"), (DWORD&)m_nNextID, 1020);

	DWORD nCount = (DWORD)m_mapResources.GetCount();
	CXTPPropExchangeEnumeratorPtr pEnumerator(pPX->GetEnumerator(_T("Item")));
	POSITION posEnum = pEnumerator->GetPosition(nCount);

	if (pPX->IsStoring())
	{
		if (nCount == 0)
			return;  // nothing more to do

		POSITION pos = m_mapResources.GetStartPosition();
		while(pos)
		{
			CResourceInfo* pInfo;
			CString strCaption;
			m_mapResources.GetNextAssoc(pos, strCaption, (CObject*&)pInfo);

			CXTPPropExchangeSection secItem(pEnumerator->GetNext(posEnum));
			PX_String(&secItem, _T("Caption"), strCaption, _T(""));
			PX_Object(&secItem, pInfo, RUNTIME_CLASS(CResourceInfo));
		}
	}
	else
	{
		FreeAll();

		CString strCaption;
		CResourceInfo* pInfo = 0;

		while (posEnum)
		{
			CXTPPropExchangeSection secItem(pEnumerator->GetNext(posEnum));
			PX_String(&secItem, _T("Caption"), strCaption, _T(""));
			PX_Object(&secItem, pInfo, RUNTIME_CLASS(CResourceInfo));
			m_mapResources.SetAt(strCaption, pInfo);
		}
	}

}

void CResourceManager::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		ar << m_nNextID;
	}
	else
	{
		ar >> m_nNextID;

		FreeAll();
	}

	m_mapResources.Serialize(ar);
}

IMPLEMENT_SERIAL(CResourceInfo, CObject, 1)

void CResourceInfo::Serialize(CArchive& ar)
{
	m_strCaption = _T("");

	if (ar.IsStoring())
	{
		ar << m_nID;
		ar << m_bStandardResource;

	} else
	{
		ar >> m_nID;
		ar >> m_bStandardResource;
	}
}
void CResourceInfo::DoPropExchange(CXTPPropExchange* pPX)
{
	m_strCaption = _T("");

	PX_DWord(pPX, _T("Id"), (DWORD&)m_nID, 0);
	PX_Bool(pPX, _T("Std"), m_bStandardResource, FALSE);
}
