// XTDirWatcher.cpp : implementation file
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
#include "XTPVC80Helpers.h"  // Visual Studio 2005 helper functions

#include "XTDefines.h"
#include "XTGlobal.h"
#include "XTDirWatcher.h"

#include <io.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CXTDirWatcher

CXTDirWatcher::CXTDirWatcher()
: m_dwWaitStatus(0)
{
	m_bAutoDelete = FALSE;
	m_dwChangeHandles[0] = INVALID_HANDLE_VALUE;
	m_dwChangeHandles[1] = INVALID_HANDLE_VALUE;
}

CXTDirWatcher::~CXTDirWatcher()
{
}

IMPLEMENT_DYNCREATE(CXTDirWatcher, CWinThread)

BEGIN_MESSAGE_MAP(CXTDirWatcher, CWinThread)
	//{{AFX_MSG_MAP(CXTDirWatcher)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CXTDirWatcher::InitInstance()
{
	if (IsPathValid(m_strFolderPath))
	{
		// Watch the directory for file creation and
		// deletion.

		m_dwChangeHandles[0] = ::FindFirstChangeNotification(
			m_strFolderPath,               // directory to watch
			FALSE,                         // do not watch the subtree
			FILE_NOTIFY_CHANGE_FILE_NAME); // watch file name changes

		if (m_dwChangeHandles[0] == INVALID_HANDLE_VALUE)
			return FALSE;

		// Watch the tree for directory creation and
		// deletion.

		m_dwChangeHandles[1] = ::FindFirstChangeNotification(
			m_strFolderPath,               // directory to watch
			TRUE,                          // watch the subtree
			FILE_NOTIFY_CHANGE_DIR_NAME);  // watch dir. name changes

		if (m_dwChangeHandles[1] == INVALID_HANDLE_VALUE)
			return FALSE;

		// Change notification is set. Now wait on both notification
		// handles and refresh accordingly.

		while (TRUE)
		{
			// Wait for notification.

			m_dwWaitStatus = ::WaitForMultipleObjects(_countof(m_dwChangeHandles),
				m_dwChangeHandles, FALSE, INFINITE);

			switch (m_dwWaitStatus)
			{
			case WAIT_OBJECT_0:

				// A file was created or deleted in C:\WINDOWS.
				// Refresh this directory and restart the
				// change notification. RefreshDirectory is an
				// application-defined function.

				RefreshFolder();

				if (!::FindNextChangeNotification(m_dwChangeHandles[0]))
					return FALSE;

				break;

			case WAIT_OBJECT_0 + 1:

				// A directory was created or deleted in C:\.
				// Refresh the directory tree and restart the
				// change notification. RefreshTree is an
				// application-defined function.

				RefreshTree();

				if (!FindNextChangeNotification(m_dwChangeHandles[1]))
					return FALSE;

				break;

			default:
				return FALSE;
			}
		}
	}

	return FALSE;
}

BOOL CXTDirWatcher::IsPathValid(LPCTSTR lpszFolderPath)
{
	if (_tcslen(lpszFolderPath) == 0)
		return FALSE;

	if (ACCESS_S(lpszFolderPath, 0) == -1)
		return FALSE;

	return TRUE;
}

BOOL CXTDirWatcher::SetFolderPath(CWnd* pMainWnd, LPCTSTR lpszFolderPath)
{
	if (IsPathValid(lpszFolderPath))
	{
		if (GetFolderData(lpszFolderPath, m_tvid))
		{
			m_pMainWnd = pMainWnd;
			m_strFolderPath = lpszFolderPath;

			return TRUE;
		}
	}

	return FALSE;
}

BOOL CXTDirWatcher::SetFolderData(CWnd* pMainWnd, XT_TVITEMDATA* lpTVID)
{
	if (!lpTVID)
		return FALSE;

	TCHAR szFolderPath[_MAX_PATH];
	if (::SHGetPathFromIDList(lpTVID->lpifq, szFolderPath))
	{
		return SetFolderPath(pMainWnd, szFolderPath);
	}

	return FALSE;
}

BOOL CXTDirWatcher::GetFolderData(LPCTSTR lpszFolderPath, XT_TVITEMDATA& lpTVID)
{
	LPITEMIDLIST  pidl;
	LPSHELLFOLDER pDesktopFolder;
	OLECHAR       szOleChar[MAX_PATH];
	ULONG         chEaten;
	ULONG         dwAttributes;

	if (!IsPathValid(lpszFolderPath))
		return FALSE;

	// Get a pointer to the Desktop's IShellFolder interface.
	if ( SUCCEEDED( ::SHGetDesktopFolder( &pDesktopFolder ) ) )
	{
		// IShellFolder::ParseDisplayName requires the file name be in
		// Unicode.

#if !defined( _UNICODE )
		::MultiByteToWideChar( CP_ACP, MB_PRECOMPOSED, lpszFolderPath, -1,
			szOleChar, MAX_PATH );
#else
		STRCPY_S(szOleChar, MAX_PATH, lpszFolderPath);
#endif

		// Convert the path to an ITEMIDLIST.
		if ( SUCCEEDED( pDesktopFolder->ParseDisplayName( NULL,NULL,szOleChar,
			&chEaten,&pidl,&dwAttributes) ) )
		{
			IShellFolder *psfMyFolder;

			lpTVID.lpi = lpTVID.lpifq = pidl;

			pDesktopFolder->BindToObject( lpTVID.lpifq,NULL,
				IID_IShellFolder,(LPVOID*)&psfMyFolder );

			lpTVID.lpsfParent = psfMyFolder;
			pDesktopFolder->Release();

			return TRUE;
		}

		pDesktopFolder->Release();
	}

	return FALSE;
}

void CXTDirWatcher::RefreshFolder()
{
	m_pMainWnd->SendMessage( XTWM_SHELL_NOTIFY,
		SHN_XT_REFRESHFOLDER, (LPARAM)&m_tvid );
}

void CXTDirWatcher::RefreshTree()
{
	m_pMainWnd->SendMessage( XTWM_SHELL_NOTIFY,
		SHN_XT_REFRESHTREE, (LPARAM)&m_tvid );
}
