// XTShellListBase.cpp : implementation file
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

#include "XTDefines.h"
#include "XTGlobal.h"
#include "XTVC50Helpers.h"
#include "XTFunctions.h"
#include "XTFlatHeaderCtrl.h"
#include "XTListCtrlView.h"

#include "XTShellPidl.h"
#include "XTShellSettings.h"
#include "XTDirWatcher.h"
#include "XTShellListBase.h"
#include "XTSortClass.h"
#include "XTResource.h"
#include "XTHelpers.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CXTShellListBase

CXTShellListBase::CXTShellListBase()
: m_pDirThread(0)
{
	m_bContextMenu = TRUE;
	m_uFlags       = SHCONTF_FOLDERS | SHCONTF_NONFOLDERS;

	if (m_shSettings.ShowAllFiles() && !m_shSettings.ShowSysFiles()) {
		m_uFlags |= SHCONTF_INCLUDEHIDDEN;
	}

	if (!SUCCEEDED(::SHGetSpecialFolderLocation(NULL, CSIDL_INTERNET, &m_pidlINet)))
	{
		m_pidlINet = NULL;
	}

	m_nNameColumnWidth = 150;
}

CXTShellListBase::~CXTShellListBase()
{
	// End the directory monitoring thread.
	SAFE_DELETE(m_pDirThread);

	if (m_pidlINet)
	{
		LPMALLOC lpMalloc = NULL;
		if (SUCCEEDED(::SHGetMalloc(&lpMalloc)))
		{
			if ( lpMalloc && m_pidlINet ) {
				lpMalloc->Free(m_pidlINet);
			}
			if ( lpMalloc ) {
				lpMalloc->Release();
			}
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// CXTShellListBase message handlers

BOOL CXTShellListBase::PopulateListView(XT_TVITEMDATA* lptvid, LPSHELLFOLDER lpsf)
{
	// Turn off redraw so the user does't see resorting
	m_pListCtrl->SetRedraw(false);

	//clear the view for new items
	m_pListCtrl->DeleteAllItems();

	if (InitListViewItems(lptvid, lpsf))
	{
		SortList((m_nSortedCol > -1) ? m_nSortedCol : 0,
			(m_nSortedCol > -1) ? m_bAscending : 1);

		// create the directory monitoring thread.
		if (m_pDirThread == NULL)
		{
			m_pDirThread = (CXTDirWatcher*)AfxBeginThread(RUNTIME_CLASS(CXTDirWatcher),
				THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED, NULL);

			m_pDirThread->SetFolderData(m_pListCtrl, lptvid);
			m_pDirThread->ResumeThread();
		}

		// if the folder changed, update the folder data.
		else
		{
			TCHAR szFolderPath[_MAX_PATH];
			if (::SHGetPathFromIDList(lptvid->lpifq, szFolderPath))
			{
				CString strFolderPath = m_pDirThread->GetFolderPath();
				if (strFolderPath.CompareNoCase(szFolderPath) != 0)
				{
					m_pDirThread->SuspendThread();
					m_pDirThread->SetFolderData(m_pListCtrl, lptvid);
					m_pDirThread->ResumeThread();
				}
			}
		}

		m_pListCtrl->SetRedraw(true);
		return TRUE;
	}

	m_pListCtrl->SetRedraw(true);
	return FALSE;
}

void CXTShellListBase::BuildDefaultColumns()
{
	CString strLabel;
	VERIFY(XTPResourceManager()->LoadString(&strLabel, XT_IDS_NAME));
	m_pListCtrl->InsertColumn(0, strLabel, LVCFMT_LEFT, m_nNameColumnWidth, 0);

	VERIFY(XTPResourceManager()->LoadString(&strLabel, XT_IDS_SIZE));
	m_pListCtrl->InsertColumn(1, strLabel, LVCFMT_RIGHT, 100, 1);

	VERIFY(XTPResourceManager()->LoadString(&strLabel, XT_IDS_TYPE));
	m_pListCtrl->InsertColumn(2, strLabel, LVCFMT_LEFT, 120, 2);

	VERIFY(XTPResourceManager()->LoadString(&strLabel, XT_IDS_MODIFIED));
	m_pListCtrl->InsertColumn(3, strLabel, LVCFMT_LEFT, 120, 3);
}

BOOL CXTShellListBase::InitSystemImageLists()
{
	SHFILEINFO sfi;
	HIMAGELIST himlSmall = (HIMAGELIST)::SHGetFileInfo( _T("C:\\"), 0, &sfi,
		sizeof(SHFILEINFO), SHGFI_SYSICONINDEX | SHGFI_SMALLICON );

	HIMAGELIST himlLarge = (HIMAGELIST)::SHGetFileInfo( _T("C:\\"), 0, &sfi,
		sizeof(SHFILEINFO), SHGFI_SYSICONINDEX | SHGFI_LARGEICON );

	if (himlSmall && himlLarge)
	{
		m_pListCtrl->SetImageList(CImageList::FromHandle(himlSmall), LVSIL_SMALL);
		m_pListCtrl->SetImageList(CImageList::FromHandle(himlLarge), LVSIL_NORMAL);
		return TRUE;
	}

	return FALSE;
}

BOOL CXTShellListBase::InitListViewItems(XT_TVITEMDATA* lptvid, LPSHELLFOLDER lpsf)
{
	LPMALLOC lpMalloc;
	if (SUCCEEDED(::SHGetMalloc(&lpMalloc)))
	{
		LPENUMIDLIST lpe = NULL;

		if (SUCCEEDED(lpsf->EnumObjects(::GetParent(m_pListCtrl->m_hWnd), m_uFlags, &lpe)))
		{
			int          iCtr      = 0;
			ULONG        ulFetched = 0;
			LPITEMIDLIST lpi       = NULL;

			while (lpe->Next(1, &lpi, &ulFetched) == S_OK)
			{
				// allocate memory for ITEMDATA struct
				XT_LVITEMDATA* lplvid = (XT_LVITEMDATA*)lpMalloc->Alloc(sizeof(XT_LVITEMDATA));
				if (lplvid == NULL)
				{
					if (lpe) {
						lpe->Release();
					}
					if (lpMalloc) {
						lpMalloc->Release();
					}
					return FALSE;
				}

				// Now get the friendly name that we'll put in the treeview...
				TCHAR szBuff[_MAX_PATH];
				GetName(lpsf, lpi, SHGDN_NORMAL, szBuff);

				// Note that since we are interested in the display attributes as well as
				// the other attributes, we need to set ulAttrs to SFGAO_DISPLAYATTRMASK
				// before calling GetAttributesOf();
				ULONG ulAttrs = SFGAO_DISPLAYATTRMASK | SFGAO_REMOVABLE;
				UINT  uFlags  = SHGFI_PIDL | SHGFI_SYSICONINDEX | SHGFI_SMALLICON;

				lpsf->GetAttributesOf(1, (const struct _ITEMIDLIST **)&lpi, &ulAttrs);
				lplvid->ulAttribs = ulAttrs;
				LPITEMIDLIST lpifqThisItem = ConcatPidls(lptvid->lpifq, lpi);

				LV_ITEM lvi;
				lvi.mask       = LVIF_TEXT | LVIF_IMAGE | LVIF_PARAM;
				lvi.iItem      = iCtr++;
				lvi.iSubItem   = 0;
				lvi.pszText    = szBuff;
				lvi.cchTextMax = MAX_PATH;
				lvi.iImage     = GetItemIcon(ConcatPidls(lptvid->lpifq, lpi), uFlags);

				lplvid->lpsfParent = lpsf;
				lpsf->AddRef();

				// Make a copy of the ITEMIDLIST
				lplvid->lpi = DuplicateItem(lpMalloc, lpi);
				lvi.lParam = (LPARAM)lplvid;

				// Add the item to the listview
				int iIndex = m_pListCtrl->InsertItem(&lvi);
				SetAttributes(iIndex, ulAttrs);
				if (iIndex >= 0)
				{
					TCHAR szItemPath[_MAX_PATH];
					::SHGetPathFromIDList( lpifqThisItem, szItemPath );

					if (((ulAttrs & SFGAO_FILESYSTEM) == SFGAO_FILESYSTEM) &&
						((ulAttrs & SFGAO_FOLDER) == 0))
					{
						WIN32_FIND_DATA fdata;
						HANDLE handle = ::FindFirstFile( szItemPath, &fdata );

						if ( handle != INVALID_HANDLE_VALUE )
						{
							LONGLONG fsize = fdata.nFileSizeHigh*( ( LONGLONG )ULONG_MAX+1 ) + fdata.nFileSizeLow;

							TCHAR szBuffer[16];
							CString strSize;
							strSize.Format( _T( "%s KB" ), InsertCommas( ( fsize+1024 )/1024, szBuffer,15 ) );

							m_pListCtrl->SetItemText( iIndex, 1, strSize );

							FILETIME ltime;
							::FileTimeToLocalFileTime( &fdata.ftLastWriteTime, &ltime );

							SYSTEMTIME time;
							::FileTimeToSystemTime( &ltime, &time );

							CTime cTime;
							cTime = CTime(
								time.wYear,
								time.wMonth,
								time.wDay,
								time.wHour,
								time.wMinute,
								time.wSecond);

							m_pListCtrl->SetItemText( iIndex, 3, cTime.Format( _T( "%m/%d/%y %I:%M %p" ) ) );

							::FindClose( handle );
						}

						/*CFileFind ff;
						if (ff.FindFile(szItemPath))
						{
							ff.FindNextFile();
							CString strSize;
							strSize.Format(_T("%ldKB"), (ff.GetLength()+1024)/1024);
							m_pListCtrl->SetItemText(iIndex, 1, strSize);

							CTime time;
							ff.GetCreationTime(time);
							m_pListCtrl->SetItemText(iIndex, 3, time.Format(_T("%m/%d/%y %I:%M %p")));
						}*/
					}
					else {
						m_pListCtrl->SetItemText(iIndex, 2, _T("0 bytes"));
					}

					SHFILEINFO sfi;
					::SHGetFileInfo((TCHAR*)lpifqThisItem, 0, &sfi,
						sizeof(SHFILEINFO), SHGFI_PIDL|SHGFI_TYPENAME);

					m_pListCtrl->SetItemText(iIndex,2,sfi.szTypeName);
				}
				else {
					return FALSE;
				}

				if (lpifqThisItem) {
					lpMalloc->Free(lpifqThisItem);
				}

				// Free the pidl that the shell gave us.
				if (lpi) {
					lpMalloc->Free(lpi);
					lpi = NULL;
				}
			}
		}

		if (lpe) {
			lpe->Release();
		}
		if (lpMalloc) {
			lpMalloc->Release();
		}

		return TRUE;
	}

	return FALSE;
}

TCHAR* CXTShellListBase::InsertCommas(LONGLONG value, TCHAR* szBufferOut, UINT nSize)
{
	TCHAR szBufferIn[ 30 ]; // 30 digits is a really big number
	TCHAR szDecimalSep[ 5 ];
	TCHAR szThousandSep[ 5 ];

	NUMBERFMT fmt;
	fmt.NumDigits = 0; // No decimal places
	::GetLocaleInfo( LOCALE_USER_DEFAULT, LOCALE_ILZERO, szBufferIn, 2 );
	fmt.LeadingZero = _ttoi( szBufferIn );
	fmt.Grouping = 3;
	::GetLocaleInfo( LOCALE_USER_DEFAULT, LOCALE_SDECIMAL, szDecimalSep, 4 );
	fmt.lpDecimalSep = szDecimalSep;
	::GetLocaleInfo( LOCALE_USER_DEFAULT, LOCALE_STHOUSAND, szThousandSep,4 );
	fmt.lpThousandSep = szThousandSep;
	::GetLocaleInfo( LOCALE_USER_DEFAULT, LOCALE_INEGNUMBER, szBufferIn, 2 );
	fmt.NegativeOrder = _ttoi( szBufferIn );
	SPRINTF_S( szBufferIn, _countof(szBufferIn), _T( "%I64d" ), value );
	::GetNumberFormat( LOCALE_USER_DEFAULT, 0, szBufferIn, &fmt, szBufferOut, nSize );

	return szBufferOut;
}

void CXTShellListBase::GetNormalAndSelectedIcons(LPITEMIDLIST lpifq, LPTV_ITEM lptvitem)
{
	// Note that we don't check the return value here because if GetIcon()
	// fails, then we're in big trouble...
	lptvitem->iImage = GetItemIcon(lpifq,
		SHGFI_PIDL | SHGFI_SYSICONINDEX | SHGFI_SMALLICON);

	lptvitem->iSelectedImage = GetItemIcon(lpifq,
		SHGFI_PIDL | SHGFI_SYSICONINDEX | SHGFI_SMALLICON | SHGFI_OPENICON);
}

int CXTShellListBase::GetDoubleClickedItem()
{
	CPoint point;
	::GetCursorPos(&point);
	m_pListCtrl->ScreenToClient(&point);

	LV_HITTESTINFO  lvhti;
	lvhti.pt = point;
	m_pListCtrl->HitTest(&lvhti);

	if ((lvhti.flags & LVHT_ONITEM) == 0) {
		return -1;
	}

	return lvhti.iItem;
}

int CXTShellListBase::GetContextMenu()
{
	CPoint point;
	::GetCursorPos(&point);
	m_pListCtrl->ScreenToClient(&point);

	LV_HITTESTINFO lvhti;
	lvhti.pt = point;
	m_pListCtrl->HitTest(&lvhti);

	if (lvhti.flags & LVHT_ONITEM)
	{
		LV_ITEM lvi;
		lvi.mask     = LVIF_PARAM;
		lvi.iItem    = lvhti.iItem;
		lvi.iSubItem = 0;

		// Long pointer to ListView item data
		XT_LVITEMDATA* lplvid = (XT_LVITEMDATA*)m_pListCtrl->GetItemData(lvhti.iItem);
		ASSERT(lplvid != NULL);

		m_pListCtrl->ClientToScreen(&point);
		ShowContextMenu(m_pListCtrl->m_hWnd,
			lplvid->lpsfParent, lplvid->lpi, &point);

		return lvhti.iItem;
	}

	return -1;
}

bool CXTShellListBase::ShellOpenItem(int iItem)
{
	// Long pointer to ListView item data
	XT_LVITEMDATA*  lplvid = (XT_LVITEMDATA*)m_pListCtrl->GetItemData(iItem);
	return ShellOpenItem(lplvid);
}

bool CXTShellListBase::ShellOpenItem(XT_LVITEMDATA* lplvid)
{
	// Long pointer to ListView item data
	if (!(lplvid->ulAttribs & (SFGAO_FOLDER | SFGAO_FILESYSANCESTOR | SFGAO_REMOVABLE)))
	{
		SHELLEXECUTEINFO sei;
		::ZeroMemory(&sei, sizeof(SHELLEXECUTEINFO));

		sei.cbSize   = sizeof(SHELLEXECUTEINFO);
		sei.fMask    = SEE_MASK_INVOKEIDLIST;
		sei.hwnd     = ::GetParent(m_pListCtrl->m_hWnd);
		sei.nShow    = SW_SHOWNORMAL;
		sei.hInstApp = AfxGetInstanceHandle();
		sei.lpIDList = GetFullyQualPidl(lplvid->lpsfParent, lplvid->lpi);

		if ( ::ShellExecuteEx(&sei) )
		{
			return true;
		}
	}

	return false;
}

BOOL CXTShellListBase::GetItemPath(int iItem, CString &strItemPath)
{
	if(iItem >= 0)
	{
		// Long pointer to TreeView item data
		XT_LVITEMDATA*  lplvid = (XT_LVITEMDATA*)m_pListCtrl->GetItemData(iItem);
		if (lplvid != 0)
		{
			LPITEMIDLIST lpid = GetFullyQualPidl(lplvid->lpsfParent, lplvid->lpi);

			TCHAR szItemPath[_MAX_PATH];
			if (::SHGetPathFromIDList(lpid, szItemPath))
			{
				strItemPath = szItemPath;
				return TRUE;
			}
		}
	}

	return FALSE;
}

void CXTShellListBase::OnRclickImpl(NMHDR* pNMHDR, LRESULT* pResult)
{
	UNUSED_ALWAYS(pNMHDR);

	// Display the shell context menu.
	if (m_bContextMenu == TRUE)
	{
		int iIndex = GetContextMenu();
		if (iIndex != -1)
		{
			// TODO: Additional error handling.
		}
	}

	*pResult = 0;
}


void CXTShellListBase::UpdateList(int nMessage, XT_TVITEMDATA* pItemData)
{
	switch (nMessage)
	{
		case SHN_XT_CONTENTSCHANGED:
		case SHN_XT_TREESELCHANGE:
			{
				CWnd* pOwner = m_pListCtrl->GetOwner();
				ASSERT_VALID(pOwner);

				// The tree view selection has changed, so update the contents
				// of the list view
				XT_TVITEMDATA*  lptvid = (XT_TVITEMDATA*)pItemData;
				ASSERT(lptvid != NULL);

				if ( lptvid->lpsfParent == NULL )
				{
					LPMALLOC lpMalloc = NULL;
					if ( FAILED( ::SHGetMalloc( &lpMalloc ) ) )
					{
						return;
					}

					LPSHELLFOLDER lpShellFolder;
					if ( FAILED( ::SHGetDesktopFolder( &lpShellFolder ) ) )
					{
						return;
					}

					LPITEMIDLIST pidlDesktop = NULL;
					if ( FAILED( ::SHGetSpecialFolderLocation( NULL, CSIDL_DESKTOP, &pidlDesktop ) ) )
					{
						lpShellFolder->Release();
						return;
					}

					IShellFolder *pFolder = NULL;
					if ( lpShellFolder->CompareIDs( 0, lptvid->lpifq,pidlDesktop ) )
					{
						pFolder = lpShellFolder;
					}
					else
					{
						lpShellFolder->BindToObject( lptvid->lpifq, 0, IID_IShellFolder, ( LPVOID* )&pFolder );
						lpShellFolder->Release();
					}

					PopulateListView( lptvid, lpShellFolder );

					if ( m_pidlINet && ( lpShellFolder->CompareIDs( 0, lptvid->lpifq, m_pidlINet ) == 0 ) )
					{
						pOwner->SendMessage( XTWM_SHELL_NOTIFY, SHN_XT_INETFOLDER );
					}
					else
					{
						pOwner->SendMessage( XTWM_SHELL_NOTIFY );
					}

					if ( lpMalloc && pidlDesktop ) {
						lpMalloc->Free( pidlDesktop );
					}
					if ( lpMalloc ) {
						lpMalloc->Release();
					}
					if ( lpShellFolder ) {
						lpShellFolder->Release();
					}
				}
				else
				{
					LPSHELLFOLDER lpsf = NULL;
					if (SUCCEEDED(lptvid->lpsfParent->BindToObject(lptvid->lpi,
						0, IID_IShellFolder, (LPVOID*)&lpsf)))
					{
						PopulateListView(lptvid, lpsf);
						lpsf->Release();

						if (SUCCEEDED(::SHGetDesktopFolder(&lpsf)))
						{
							if (m_pidlINet && (lpsf->CompareIDs(0, lptvid->lpifq, m_pidlINet) == 0)) {
								pOwner->SendMessage(XTWM_SHELL_NOTIFY, SHN_XT_INETFOLDER);
							}
							else {
								pOwner->SendMessage(XTWM_SHELL_NOTIFY);
							}

							lpsf->Release();
						}
					}
					else
					{
						if (SUCCEEDED(::SHGetDesktopFolder(&lpsf)))
						{
							if (m_pidlINet && (lpsf->CompareIDs(0, lptvid->lpifq, m_pidlINet) == 0)) {
								pOwner->SendMessage(XTWM_SHELL_NOTIFY, SHN_XT_INETFOLDER);
							}

							lpsf->Release();
						}
					}
				}
				break;
			}

		case SHN_XT_REFRESHFOLDER:
		case SHN_XT_REFRESHTREE:
			{
				// Directory monitory thread has issued an update notification,
				// refresh the list control.
				XT_TVITEMDATA*  lpTVID = (XT_TVITEMDATA*)pItemData;
				ASSERT(lpTVID);

				PopulateListView(lpTVID, lpTVID->lpsfParent);
				break;
			}

		case SHN_XT_NOFOLDER:
			{
				// The item double clicked was not found in the treeview
				// so it sent us back a confirmation to execute it
				XT_LVITEMDATA*  lplvid = (XT_LVITEMDATA*)pItemData;
				ASSERT(lplvid);

				ShellOpenItem(lplvid);
			}
			break;

		default:
			break;
	}
}

bool CXTShellListBase::SortList( int nCol, bool bAscending )
{
	if (m_nSortedCol >= 0)
	{
		CXTFlatHeaderCtrl* pHeaderCtrl = GetFlatHeaderCtrl();
		if (pHeaderCtrl && pHeaderCtrl->GetSortedCol() < 0)
		{
			pHeaderCtrl->SetSortImage(nCol, bAscending);
		}
	}

	if (nCol == 0)
	{
		return m_pListCtrl->SortItems(
			ListViewCompareProc, (DWORD)bAscending) == TRUE;
	}

	XTSortType arrColType[] = {
		xtSortString, xtSortInt, xtSortString, xtSortDateTime
	};

	CXTSortClass sortClass(m_pListCtrl, nCol);
	sortClass.Sort(bAscending, arrColType[nCol]);

	return true;
}

void CXTShellListBase::SetAttributes(int iItem, DWORD dwAttributes)
{
	MapShellFlagsToItemAttributes(m_pListCtrl, iItem, dwAttributes, LVIS_CUT);
}

void CXTShellListBase::OnDragDrop(NM_LISTVIEW* pNMListView)
{
	UNREFERENCED_PARAMETER(pNMListView);

	COleDataSource oleDataSource;
	HGLOBAL        hgDrop;
	DROPFILES*     pDrop;
	CStringList    lsDraggedFiles;
	POSITION       pos;
	int            nSelItem;
	CString        sFile;
	UINT           uBuffSize = 0;
	TCHAR*         pszBuff;

	FORMATETC      etc = { CF_HDROP, NULL, DVASPECT_CONTENT, -1, TYMED_HGLOBAL };

	// For every selected item in the list, put the filename into lsDraggedFiles.
	pos = m_pListCtrl->GetFirstSelectedItemPosition();

	while ( NULL != pos )
	{
		nSelItem = m_pListCtrl->GetNextSelectedItem(pos);
		GetItemPath(nSelItem, sFile);
		lsDraggedFiles.AddTail ( sFile );

		// Calculate the # of chars required to hold this string.
		uBuffSize += lstrlen ( sFile ) + 1;
	}

	// Add 1 extra for the final null char, and the size of the DROPFILES struct.
	uBuffSize = sizeof(DROPFILES) + sizeof(TCHAR) * (uBuffSize + 1);

	// Allocate memory from the heap for the DROPFILES struct.
	hgDrop = ::GlobalAlloc ( GHND | GMEM_SHARE, uBuffSize );

	if ( NULL == hgDrop )
		return;

	pDrop = (DROPFILES*) ::GlobalLock ( hgDrop );

	if ( NULL == pDrop )
	{
		::GlobalFree ( hgDrop );
		return;
	}

	// Fill in the DROPFILES struct.
	pDrop->pFiles = sizeof(DROPFILES);

#ifdef _UNICODE
	// If we're compiling for Unicode, set the Unicode flag in the struct to
	// indicate it contains Unicode strings.
	pDrop->fWide = TRUE;
#endif

	// Copy all the filenames into memory after the end of the DROPFILES struct.
	pos = lsDraggedFiles.GetHeadPosition();
	pszBuff = (TCHAR*) (LPBYTE(pDrop) + sizeof(DROPFILES));

	while ( NULL != pos )
	{
		lstrcpy ( pszBuff, (LPCTSTR) lsDraggedFiles.GetNext ( pos ) );
		pszBuff = 1 + _tcschr ( pszBuff, '\0' );
	}

	::GlobalUnlock ( hgDrop );

	// Put the data in the data source.
	oleDataSource.CacheGlobalData ( CF_HDROP, hgDrop, &etc );

	// Add in our own custom data, so we know that the drag originated from our
	// window.  OnDragEnter() checks for this custom format, and
	// doesn't allow the drop if it's present.  This is how we prevent the user
	// from dragging and then dropping in our own window.
	// The data will just be a dummy bool.
	HGLOBAL hgBool;

	hgBool = ::GlobalAlloc ( GHND | GMEM_SHARE, sizeof(bool) );

	if ( NULL == hgBool )
	{
		::GlobalFree ( hgDrop );
		return;
	}

	static CLIPFORMAT clpFormat = (CLIPFORMAT)
		::RegisterClipboardFormat(_T("{B0D76F7A-B5D9-436c-8F10-BA16AEE69D42}"));

	// Put the data in the data source.
	etc.cfFormat = clpFormat;
	oleDataSource.CacheGlobalData(clpFormat, hgBool, &etc);

	// Start the drag 'n' drop!
	DROPEFFECT dwEffect = oleDataSource.DoDragDrop ( DROPEFFECT_COPY | DROPEFFECT_MOVE );

	// If the DnD completed OK, we remove all of the dragged items from our
	//
	switch ( dwEffect )
	{
	case DROPEFFECT_MOVE:
		{
			// The files were copied or moved.
			// Note: Don't call ::GlobalFree() because the data will be freed by the drop target.
			for ( nSelItem = m_pListCtrl->GetNextItem ( -1, LVNI_SELECTED );
			nSelItem != -1;
			nSelItem = m_pListCtrl->GetNextItem ( nSelItem, LVNI_SELECTED ) )
			{
				m_pListCtrl->DeleteItem ( nSelItem );
				nSelItem--;
			}
		}
	case DROPEFFECT_COPY: // fall thru.
		{
			// Resize the list columns.
//          m_pListCtrl->SetColumnWidth ( 0, LVSCW_AUTOSIZE_USEHEADER );
//          m_pListCtrl->SetColumnWidth ( 1, LVSCW_AUTOSIZE_USEHEADER );
//          m_pListCtrl->SetColumnWidth ( 2, LVSCW_AUTOSIZE_USEHEADER );
		}
		break;

	case DROPEFFECT_NONE:
		{
			// This needs special handling, because on NT, DROPEFFECT_NONE
			// is returned for move operations, instead of DROPEFFECT_MOVE.
			// See Q182219 for the details.
			// So if we're on NT, we check each selected item, and if the
			// file no longer exists, it was moved successfully and we can
			// remove it from the
			if ((GetVersion() & 0x80000000) == 0)
			{
				bool bDeletedAnything = false;

				for ( nSelItem = m_pListCtrl->GetNextItem ( -1, LVNI_SELECTED );
				nSelItem != -1;
				nSelItem = m_pListCtrl->GetNextItem ( nSelItem, LVNI_SELECTED ) )
				{
					GetItemPath(nSelItem, sFile);

					if ( 0xFFFFFFFF == GetFileAttributes ( sFile ) &&
						GetLastError() == ERROR_FILE_NOT_FOUND )
					{
						// We couldn't read the file's attributes, and GetLastError()
						// says the file doesn't exist, so remove the corresponding
						// item from the
						m_pListCtrl->DeleteItem ( nSelItem );

						nSelItem--;
						bDeletedAnything = true;
					}
				}

				// Resize the list columns if we deleted any items.
				if ( bDeletedAnything )
				{
					m_pListCtrl->SetColumnWidth ( 0, LVSCW_AUTOSIZE_USEHEADER );
					m_pListCtrl->SetColumnWidth ( 1, LVSCW_AUTOSIZE_USEHEADER );
					m_pListCtrl->SetColumnWidth ( 2, LVSCW_AUTOSIZE_USEHEADER );

					// Note: Don't call ::GlobalFree() because the data belongs to
					// the caller.
				}
				else
				{
					// The DnD operation wasn't accepted, or was canceled, so we
					// should call ::GlobalFree() to clean up.
					::GlobalFree ( hgDrop );
					::GlobalFree ( hgBool );
				}
			}   // end if (NT)
			else
			{
				// We're on 9x, and a return of DROPEFFECT_NONE always means
				// that the DnD operation was aborted.  We need to free the
				// allocated memory.
				::GlobalFree ( hgDrop );
				::GlobalFree ( hgBool );
			}
		}
		break;  // end case DROPEFFECT_NONE
	}   // end switch
}


void CXTShellListBase::InitializeControl()
{
	CXTListBase::InitializeControl();

	if (!m_pListCtrl->GetImageList(LVSIL_SMALL))
	{
		// Initialize the columns and image list for the list control.
		BuildDefaultColumns();
		InitSystemImageLists();
		m_pListCtrl->ModifyStyle(NULL, LVS_REPORT | LVS_SHAREIMAGELISTS);

		SubclassHeader(FALSE);
		GetFlatHeaderCtrl()->ShowSortArrow(TRUE);

		m_pListCtrl->DragAcceptFiles(TRUE);
	}
}
