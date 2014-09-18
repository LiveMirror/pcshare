// XTWindowPos.cpp : implementation of the CXTWindowPos class.
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

#include "StdAfx.h"
#include "XTPVC80Helpers.h"  // Visual Studio 2005 helper functions
#include "XTPResourceManager.h"

#include "XTResource.h"
#include "XTDefines.h"
#include "XTGlobal.h"
#include "XTWindowPos.h"
#include "XTRegistryManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CXTWindowPos
/////////////////////////////////////////////////////////////////////////////
CXTWindowPos::CXTWindowPos()
{
	memset(this, 0, sizeof(CXTWindowPos));
	length  = sizeof(CXTWindowPos);
	showCmd = SW_SHOW;
}

CXTWindowPos::~CXTWindowPos()
{

}

BOOL CXTWindowPos::SaveWindowPos(CWnd* pWnd, LPCTSTR lpszWndPos/*=NULL*/)
{
	ASSERT(pWnd); // must be valid.

	if (::IsWindow(pWnd->m_hWnd))
	{
		if (pWnd->GetWindowPlacement(this))
		{
			CString strSection;
			VERIFY(XTPResourceManager()->LoadString(&strSection, XT_IDS_REG_SETTINGS));

			CString strEntry;
			VERIFY(XTPResourceManager()->LoadString(&strEntry, XT_IDS_REG_WNDPOS));

			if (lpszWndPos == NULL)
			{
				// Make sure we don't pop up
				// minimized the next time
				if (showCmd != SW_SHOWMAXIMIZED) {
					showCmd  = SW_SHOWNORMAL;
				}
			}

			if (lpszWndPos != NULL) {
				strEntry = lpszWndPos;
			}

			CXTRegistryManager regManager;
			regManager.WriteProfileBinary(strSection, strEntry,
				reinterpret_cast<LPBYTE>(this), sizeof(CXTWindowPos));

			return TRUE;
		}
	}

	return FALSE;
}

BOOL CXTWindowPos::LoadWindowPos(CWnd* pWnd, LPCTSTR lpszWndPos/*=NULL*/)
{
	ASSERT_VALID( pWnd ); // must be valid.

	if(::IsWindow(pWnd->m_hWnd))
	{
		if (LoadWindowPos(lpszWndPos))
		{
			pWnd->SetWindowPlacement(this);
			return TRUE;
		}
	}

	return FALSE;
}

BOOL CXTWindowPos::LoadWindowPos(LPCTSTR lpszWndPos/*=NULL*/)
{
	UINT nSize = 0;
	LPBYTE pbtData = 0;

	CString strSection;
	XTPResourceManager()->LoadString(&strSection, XT_IDS_REG_SETTINGS);

	CString strEntry;
	XTPResourceManager()->LoadString(&strEntry, XT_IDS_REG_WNDPOS);

	if (lpszWndPos != NULL) {
		strEntry = lpszWndPos;
	}

	CXTRegistryManager regManager;
	if (regManager.GetProfileBinary(strSection, strEntry,
		&pbtData, &nSize))
	{
		MEMCPY_S((void*)&*this, (const void*)reinterpret_cast<CXTWindowPos*>(pbtData),
			(DWORD)sizeof(CXTWindowPos));

		delete [] pbtData;

		return TRUE;
	}

	return FALSE;
}
