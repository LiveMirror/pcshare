// XTBrowseDialog.cpp : implementation of the CXTBrowseDialog class.
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
#include <io.h>
#include "XTPVC80Helpers.h"  // Visual Studio 2005 helper functions
#include "XTPResourceManager.h"

#include "XTResource.h"
#include "XTDefines.h"
#include "XTGlobal.h"
#include "XTBrowseDialog.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CXTBrowseDialog::CXTBrowseDialog(CWnd* pParent/*=NULL*/)
{
	// Initialize member data to 0.
	::ZeroMemory( (BROWSEINFO*)this, sizeof(BROWSEINFO) );
	::ZeroMemory( &m_szSelPath, sizeof(TCHAR) );

	VERIFY(XTPResourceManager()->LoadString(&m_strTitle, XT_IDS_SELDIR));

	// Setup some defaults for the BROWSEINFO base members.
	SetOwner( pParent ? pParent->m_hWnd : NULL );
	SetPidlRoot( NULL );
	SetDisplayName( m_szSelPath );
	SetTitle((TCHAR*)(LPCTSTR)m_strTitle);
	SetOptions( BIF_RETURNFSANCESTORS|BIF_RETURNONLYFSDIRS/*|BIF_STATUSTEXT*/ );
	SetCallback( BrowseCtrlCallback );
	SetData( (LPARAM)this );
}

CXTBrowseDialog::~CXTBrowseDialog()
{

}

int CALLBACK CXTBrowseDialog::BrowseCtrlCallback(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData)
{
	TCHAR szDir[MAX_PATH];
	::ZeroMemory(&szDir, sizeof(TCHAR));

	switch(uMsg)
	{
	// Set the selected directory at startup.
	case BFFM_INITIALIZED:
		{
			// use previously selected path if availiable.
			CXTBrowseDialog* pBrowseDlg = (CXTBrowseDialog*)lpData;
			if (pBrowseDlg && _tcslen(pBrowseDlg->GetSelPath()) != 0) {
				STRCPY_S(szDir, MAX_PATH, pBrowseDlg->GetSelPath());
			}

			// if no path was availiable, try the current directory.
			if (_tcslen(szDir) == 0) {
				::GetCurrentDirectory(_countof(szDir), szDir);
			}

			// set the directory path only if szDir is defined.
			if (_tcslen(szDir) != 0)
			{
				// wParam is TRUE since you are passing a path,
				// it would be FALSE if you were passing a pidl.
				::SendMessage(hwnd, BFFM_SETSELECTION, TRUE,(LPARAM)szDir);
			}
		}
		break;

	// Set the status window to the currently selected path.
	case BFFM_SELCHANGED:
		{
			CXTBrowseDialog* pBrowseDlg = (CXTBrowseDialog*)lpData;
			if (pBrowseDlg &&(pBrowseDlg->ulFlags & BIF_STATUSTEXT) != 0)
			{
				if (::SHGetPathFromIDList((LPITEMIDLIST) lParam ,szDir))
				{
					::SendMessage(hwnd, BFFM_SETSTATUSTEXT, TRUE, (LPARAM)szDir);
				}
			}
		}
		break;

	case BFFM_VALIDATEFAILED:
		break;

	default:
		break;
	}
	return 0;
}

INT_PTR CXTBrowseDialog::DoModal()
{
	UINT uReturn = IDCANCEL;
	LPMALLOC pMalloc = NULL;

	// Retrieve a pointer to the shell's IMalloc interface.
	if (::SHGetMalloc(&pMalloc) == NOERROR)
	{
		LPITEMIDLIST pItemIDList = NULL;

		// Display the browse dialog box that enables the user to
		// select a shell folder.
		if ((pItemIDList = ::SHBrowseForFolder(this)) != NULL)
		{
			// Converts an item identifier list to a file system path,
			// this will be the path that the user has just selected.
			if (::SHGetPathFromIDList(pItemIDList, m_szSelPath)) {
				uReturn = IDOK;
			}
			pMalloc->Free(pItemIDList);
		}
		pMalloc->Release();
	}

	return uReturn;
}

void CXTBrowseDialog::SetSelPath(TCHAR* szSelPath)
{
	STRCPY_S(m_szSelPath, MAX_PATH, szSelPath);
}
