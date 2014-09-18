// XTShellPidl.cpp : implementation file
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

#include "XTShellPidl.h"
#include "XTPMacros.h"

#include "XTGlobal.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CXTShellPidl

CXTShellPidl::CXTShellPidl()
{

}

CXTShellPidl::~CXTShellPidl()
{

}

/////////////////////////////////////////////////////////////////////////////
// Functions that deal with PIDLs

LPITEMIDLIST CXTShellPidl::IDLFromPath(CString strPath)
{
	USES_CONVERSION;

	LPMALLOC      pMalloc     = NULL;
	IShellFolder *psfDeskTop  = NULL;
	LPITEMIDLIST  pidlPath    = NULL;

	// Get malloc interface
	if ( FAILED( ::SHGetMalloc( &pMalloc ) ) )
	{
		return NULL;
	}

	if ( FAILED( ::SHGetDesktopFolder(&psfDeskTop) ) )
	{
		if ( pMalloc ) {
			pMalloc->Release( );
		}

		return NULL;
	}

	// Get the folders IDL
	psfDeskTop->ParseDisplayName( NULL, NULL,
		T2OLE((TCHAR*)(LPCTSTR)strPath), NULL, &pidlPath, NULL );

	if ( pMalloc ) {
		pMalloc->Release( );
	}
	if ( psfDeskTop ) {
		psfDeskTop->Release( );
	}

	return pidlPath;
}

LPITEMIDLIST CXTShellPidl::OneUpPIDL(LPITEMIDLIST pidlPath)
{
	if(pidlPath)
	{
		// Get the last item
		LPITEMIDLIST pidlLast = GetLastITEM(pidlPath);
		// if not a NULL
		if(pidlLast)
		{
			// don’t attempt to go any lower than 1 IDL
			if(pidlLast == pidlPath) return pidlPath;
			// set its cb to 0
			pidlLast->mkid.cb = 0;
		}
	}

	return pidlPath;
}
CString CXTShellPidl::OneUpPATH(CString path)
{
	// Make a string buffer
	TCHAR newPath[MAX_PATH];
	ZeroMemory(newPath, sizeof(newPath));

	if(path.GetLength())
	{
		LPITEMIDLIST pidlPath = IDLFromPath(path);
		if(OneUpPIDL(pidlPath))
		{
			SHGetPathFromIDList(pidlPath, newPath);
		}
	}

	return CString(newPath);

}
LPITEMIDLIST CXTShellPidl::GetLastITEM(LPITEMIDLIST pidl)
{
	LPSTR lpMem = NULL;
	LPITEMIDLIST pidlLast = NULL;;
	if(pidl)
	{
		lpMem = (LPSTR)pidl;
		// walk the list until we find a null
		while(*lpMem != 0)
		{
			if(LPITEMIDLIST(lpMem)->mkid.cb == 0) break;// maybe?

			pidlLast = (LPITEMIDLIST)lpMem;
			lpMem += LPITEMIDLIST(lpMem)->mkid.cb;
		}
	}

	return pidlLast;
}

LPITEMIDLIST CXTShellPidl::CopyIDList(LPITEMIDLIST pidl)
{
	LPITEMIDLIST pidlCopy = NULL;
	UINT bytes = 0;
	bytes = GetPidlItemCount(pidl);
	pidlCopy = CreatePidl(bytes);
	if(pidlCopy)
	{
		ZeroMemory(pidlCopy,bytes);
		MEMCPY_S(pidlCopy,pidl,bytes);
	}

	return pidlCopy;

}

LPITEMIDLIST CXTShellPidl::GetNextPidlItem(LPCITEMIDLIST pidl)
{
	LPSTR lpMem = (LPSTR)pidl;
	lpMem += pidl->mkid.cb;
	return (LPITEMIDLIST)lpMem;
}

UINT CXTShellPidl::GetPidlItemCount(LPCITEMIDLIST pidl)
{
	UINT cbTotal = 0;
	if (pidl)
	{
		cbTotal += sizeof(pidl->mkid.cb);       // Null terminator
		while (pidl->mkid.cb)
		{
			cbTotal += pidl->mkid.cb;
			pidl = GetNextPidlItem(pidl);
		}
	}

	return cbTotal;
}

LPITEMIDLIST CXTShellPidl::CreatePidl(UINT cbSize)
{
	LPMALLOC lpMalloc;
	if (FAILED(::SHGetMalloc(&lpMalloc))) {
		return 0;
	}

	// zero-init for external task allocate
	LPITEMIDLIST pidl = (LPITEMIDLIST)lpMalloc->Alloc(cbSize);
	if (pidl) {
		memset(pidl, 0, cbSize);
	}

	if (lpMalloc) {
		lpMalloc->Release();
	}

	return pidl;
}

LPITEMIDLIST CXTShellPidl::ConcatPidls(LPCITEMIDLIST pidl1, LPCITEMIDLIST pidl2)
{
	LPITEMIDLIST pidlNew;
	UINT cb1;
	UINT cb2;

	// May be NULL
	if (pidl1) {
		cb1 = GetPidlItemCount(pidl1) - sizeof(pidl1->mkid.cb);
	}
	else {
		cb1 = 0;
	}

	cb2 = GetPidlItemCount(pidl2);
	pidlNew = CreatePidl(cb1 + cb2);

	if (pidlNew)
	{
		if (pidl1)
		{
			MEMCPY_S(pidlNew, pidl1, cb1);
		}

		MEMCPY_S(((LPSTR)pidlNew) + cb1, pidl2, cb2);
	}

	return pidlNew;
}

LPITEMIDLIST CXTShellPidl::DuplicateItem(LPMALLOC lpMalloc, LPITEMIDLIST lpi)
{
	LPITEMIDLIST lpiTemp = (LPITEMIDLIST)lpMalloc->Alloc(lpi->mkid.cb+sizeof(lpi->mkid.cb));

	MEMCPY_S((PVOID)lpiTemp, (CONST VOID *)lpi, lpi->mkid.cb+sizeof(lpi->mkid.cb));

	return lpiTemp;
}

BOOL CXTShellPidl::GetName(LPSHELLFOLDER lpsf, LPITEMIDLIST lpi, DWORD dwFlags, TCHAR* lpFriendlyName)
{
	STRRET str;
	if (lpsf->GetDisplayNameOf(lpi,dwFlags, &str) == NOERROR)
	{
		switch (str.uType)
		{
		case STRRET_WSTR:
#if defined( _UNICODE )
			STRCPY_S(lpFriendlyName, MAX_PATH, str.pOleStr);
#else
			::WideCharToMultiByte(CP_ACP, 0, str.pOleStr, -1, lpFriendlyName,
				MAX_PATH, NULL, NULL);
#endif
			LPMALLOC pMalloc;
			if (SUCCEEDED( ::SHGetMalloc(&pMalloc) ))
			{
				pMalloc->Free(str.pOleStr);
				pMalloc->Release();
			}
			break;

		case STRRET_OFFSET:
			{
#if defined( _UNICODE )
			::MultiByteToWideChar( CP_ACP, MB_PRECOMPOSED, (LPSTR)lpi+str.uOffset,
				-1, lpFriendlyName, MAX_PATH);
#else
			LPSTR lpszOffset = LPSTR(lpi+str.uOffset);
			STRCPY_S(lpFriendlyName, MAX_PATH, lpszOffset);
#endif
			}
			break;

		case STRRET_CSTR:
#if defined( _UNICODE )
			::MultiByteToWideChar( CP_ACP, MB_PRECOMPOSED, str.cStr,
				-1, lpFriendlyName, MAX_PATH);
#else
			STRCPY_S(lpFriendlyName, MAX_PATH, (LPSTR)str.cStr);
#endif
			break;

		default:
			return FALSE;
		}
	}
	else {
		return FALSE;
	}

	return TRUE;
}

LPITEMIDLIST CXTShellPidl::GetFullyQualPidl(LPSHELLFOLDER lpsf, LPITEMIDLIST lpi)
{
	TCHAR szBuff[MAX_PATH];
	OLECHAR szOleChar[MAX_PATH];
	LPSHELLFOLDER lpsfDeskTop;
	LPITEMIDLIST  lpifq;
	ULONG ulEaten, ulAttribs;

	if (!GetName(lpsf, lpi, SHGDN_FORPARSING, szBuff)) {
		return NULL;
	}

	if (FAILED(::SHGetDesktopFolder(&lpsfDeskTop))) {
		return NULL;
	}

#if !defined( _UNICODE )
	::MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, szBuff,
		-1, (LPWSTR)szOleChar, sizeof(szOleChar));
#else
	STRCPY_S(szOleChar, MAX_PATH, szBuff);
#endif

	if (FAILED(lpsfDeskTop->ParseDisplayName(NULL, NULL, szOleChar,
		&ulEaten, &lpifq, &ulAttribs)))
	{
		lpsfDeskTop->Release();
		return NULL;
	}

	lpsfDeskTop->Release();
	return lpifq;
}

// Static variables used for passing data to the subclassing wndProc

WNDPROC        CXTShellPidl::m_pOldWndProc = NULL;  // regular window proc
LPCONTEXTMENU2 CXTShellPidl::m_pIContext2  = NULL;  // active shell context menu

BOOL CXTShellPidl::ShowContextMenu(HWND hwnd, LPSHELLFOLDER lpsfParent, LPITEMIDLIST lpi, LPPOINT lppt)
{
	return ShowContextMenu(hwnd, lpsfParent, (LPCITEMIDLIST*)&lpi, 1, lppt);
}

void CXTShellPidl::OnShowContextMenu(int idCmd, CMINVOKECOMMANDINFO& cmi)
{
	// handle in derived class.
	UNREFERENCED_PARAMETER(idCmd);
	UNREFERENCED_PARAMETER(cmi);
}

BOOL CXTShellPidl::ShowContextMenu(HWND hwnd, LPSHELLFOLDER lpsfParent, LPCITEMIDLIST *lpi, int nCount, LPPOINT lppt)
{
	LPCONTEXTMENU       lpcm;
	TCHAR               szTemp[64];
	CMINVOKECOMMANDINFO cmi;
	BOOL                bSuccess = TRUE;
	int                 cmType; // "version" # of context menu

	// assume that psfFolder and pidl are valid
	HRESULT hr = GetSHContextMenu(lpsfParent, lpi, nCount, (void**)&lpcm, &cmType);

//  HRESULT hr = lpsfParent->GetUIObjectOf(hwnd, 1, (LPCITEMIDLIST*)&lpi,
//      IID_IContextMenu, NULL, (LPVOID*)&lpcm);

	if (SUCCEEDED(hr))
	{
		HMENU hMenu = ::CreatePopupMenu();

		if (hMenu != NULL)
		{
			hr = lpcm->QueryContextMenu(hMenu, 0, 1, 0x7fff, CMF_EXPLORE );
			if (SUCCEEDED(hr))
			{
				// install the subclassing "hook", for versions 2 or 3
				if(cmType > 1)
				{
					m_pOldWndProc = (WNDPROC)
					SetWindowLongPtr(hwnd, GWL_WNDPROC, (LONG_PTR)HookWndProc);
					m_pIContext2 = (LPCONTEXTMENU2)lpcm; // cast ok for ICMv3
				}
				else
					m_pOldWndProc = NULL;

				int idCmd = ::TrackPopupMenu(hMenu,
					TPM_LEFTALIGN | TPM_RETURNCMD | TPM_RIGHTBUTTON,
					lppt->x, lppt->y, 0, hwnd, NULL);

				if(m_pOldWndProc) // restore old wndProc
					SetWindowLongPtr(hwnd, GWL_WNDPROC, (LONG_PTR)m_pOldWndProc);

				if (idCmd != 0)
				{
					cmi.cbSize       = sizeof(CMINVOKECOMMANDINFO);
					cmi.fMask        = 0;
					cmi.hwnd         = hwnd;
					cmi.lpVerb       = (LPCSTR)MAKEINTRESOURCE(idCmd-1);
					cmi.lpParameters = NULL;
					cmi.lpDirectory  = NULL;
					cmi.nShow        = SW_SHOWNORMAL;
					cmi.dwHotKey     = 0;
					cmi.hIcon        = NULL;

					if ( SUCCEEDED( lpcm->InvokeCommand( &cmi ) ) )
					{
						OnShowContextMenu( idCmd-1, cmi );
					}
				}
				m_pIContext2 = NULL; // prevents accidental use
			}
			else {
				bSuccess = FALSE;
			}

			::DestroyMenu(hMenu);
		}
		else
			bSuccess = FALSE;

		lpcm->Release();
	}
	else
	{
		SPRINTF_S(szTemp, _countof(szTemp), _T("GetUIObjectOf failed! hr=%lx"), hr);
		AfxMessageBox(szTemp );
		bSuccess = FALSE;
	}

	return bSuccess;
}

int CXTShellPidl::GetItemIcon(LPITEMIDLIST lpi, UINT uFlags)
{
	SHFILEINFO sfi;
	::SHGetFileInfo((TCHAR*)lpi, 0, &sfi,
		sizeof(SHFILEINFO), uFlags);
	return sfi.iIcon;
}

HRESULT CXTShellPidl::GetSHContextMenu(LPSHELLFOLDER psfFolder, LPCITEMIDLIST *localPidl, int nCount, void** ppCM, int* pcmType)
{
	*ppCM               = NULL;
	LPCONTEXTMENU pICv1 = NULL; // plain version

	// try to obtain the lowest possible IContextMenu
	HRESULT hr = psfFolder->GetUIObjectOf(NULL, nCount, localPidl,
		IID_IContextMenu, NULL, (void**)&pICv1);

	// try to obtain a higher level pointer, first 3 then 2
	if(pICv1)
	{
		hr = pICv1->QueryInterface(IID_IContextMenu3, ppCM);

		if(NOERROR == hr)
			*pcmType = 3;

		else
		{
			hr = pICv1->QueryInterface(IID_IContextMenu2, ppCM);
			if(NOERROR == hr) *pcmType = 2;
		}

		// free initial "v1.0" interface
		if(*ppCM)
			pICv1->Release();

		// no higher version supported
		else
		{
			*pcmType = 1;
			*ppCM = pICv1;
			hr = NOERROR; // never mind the query failures, this'll do
		}
	}

	return hr;
}

LRESULT CALLBACK CXTShellPidl::HookWndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
	UINT  uItem;
	TCHAR szBuf[MAX_PATH];

	switch (msg)
	{
	case WM_DRAWITEM:
	case WM_MEASUREITEM:
		{
			if(wp)
				break; // not menu related
		}

	case WM_INITMENUPOPUP:
		{
			m_pIContext2->HandleMenuMsg(msg, wp, lp);
			return (msg == WM_INITMENUPOPUP ? 0 : TRUE); // handled
		}

	case WM_MENUSELECT:
		{
			// if this is a shell item, get it's descriptive text
			uItem = (UINT) LOWORD(wp);

			if(0 == (MF_POPUP & HIWORD(wp)) && uItem >= 1 && uItem <=  0x7fff)
			{
				m_pIContext2->GetCommandString(uItem-1, GCS_HELPTEXT,
					NULL, (LPSTR)szBuf, _countof(szBuf) );

				// set the status bar text
				CFrameWnd * pWnd = ((CFrameWnd*)(AfxGetApp()->m_pMainWnd));
				if (pWnd)
					pWnd->SetMessageText(szBuf);

				return 0;
			}
			break;
		}

	default:
		break;
	}

	// for all untreated messages, call the original wndproc
	return ::CallWindowProc(m_pOldWndProc, hWnd, msg, wp, lp);
}

int CXTShellPidl::TreeViewCompareProc(LPARAM lparam1, LPARAM lparam2, LPARAM /*lparamSort*/)
{
	XT_TVITEMDATA* lptvid1 = (XT_TVITEMDATA*)lparam1;
	XT_TVITEMDATA* lptvid2 = (XT_TVITEMDATA*)lparam2;

	HRESULT hr = lptvid1->lpsfParent->CompareIDs(0, lptvid1->lpi, lptvid2->lpi);
	if (FAILED(hr)) {
		return 0;
	}

	return (short)SCODE_CODE(GetScode(hr));
}

int CXTShellPidl::ListViewCompareProc(LPARAM lparam1, LPARAM lparam2, LPARAM lparamSort)
{
	BOOL bAscending = (BOOL)lparamSort;

	XT_LVITEMDATA* lplvid1 = (XT_LVITEMDATA*)lparam1;
	XT_LVITEMDATA* lplvid2 = (XT_LVITEMDATA*)lparam2;

	HRESULT hr = lplvid1->lpsfParent->CompareIDs(0,
		bAscending ? lplvid1->lpi : lplvid2->lpi,
		bAscending ? lplvid2->lpi : lplvid1->lpi);

	if (FAILED(hr))
	{
		return 0;
	}

	return (short)SCODE_CODE(GetScode(hr));
}