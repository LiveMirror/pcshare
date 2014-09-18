// XTShellTreeView.cpp : implementation file
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

#include "XTDefines.h"
#include "XTGlobal.h"
#include "XTVC50Helpers.h"
#include "XTFunctions.h"
#include "XTShellPidl.h"
#include "XTDropSource.h"
#include "XTShellSettings.h"
#include "XTTreeBase.h"
#include "XTShellTreeBase.h"
#include "XTComboBoxEx.h"
#include "XTHelpers.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CXTShellTreeBase

CXTShellTreeBase::CXTShellTreeBase()
: m_bTunneling(false)
{
	m_bContextMenu = TRUE;
	m_pComboBox    = NULL;
	m_uFlags       = SHCONTF_FOLDERS | SHCONTF_NONFOLDERS;

	if (m_shSettings.ShowAllFiles() && !m_shSettings.ShowSysFiles()) {
		m_uFlags |= SHCONTF_INCLUDEHIDDEN;
	}
}

CXTShellTreeBase::~CXTShellTreeBase()
{

}

/////////////////////////////////////////////////////////////////////////////
// CXTShellTreeBase message handlers

HTREEITEM CXTShellTreeBase::InsertDesktopItem()
{
	HTREEITEM hItem = TVI_ROOT;

	LPMALLOC lpMalloc = NULL;
	if (SUCCEEDED(::SHGetMalloc(&lpMalloc)))
	{
		// Get ShellFolder Pidl
		LPITEMIDLIST pidlDesktop = NULL;
		::SHGetSpecialFolderLocation( NULL, CSIDL_DESKTOP, &pidlDesktop );

		// insert the desktop.
		if ( pidlDesktop )
		{
			SHFILEINFO fileInfo;
			::ZeroMemory( &fileInfo, sizeof( fileInfo ) );

			::SHGetFileInfo( ( LPCTSTR )pidlDesktop, NULL, &fileInfo, sizeof( fileInfo ),
				SHGFI_PIDL | SHGFI_ATTRIBUTES | SHGFI_DISPLAYNAME );

			TV_ITEM  tvi;
			tvi.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_PARAM;

			// Allocate memory for ITEMDATA struct
			XT_TVITEMDATA* lptvid = (XT_TVITEMDATA*)lpMalloc->Alloc(sizeof(XT_TVITEMDATA));
			if (lptvid != NULL )
			{
				GetNormalAndSelectedIcons( pidlDesktop, &tvi );

				// Now, make a copy of the ITEMIDLIST and store the parent folders SF.
				lptvid->lpi        = DuplicateItem(lpMalloc, pidlDesktop);
				lptvid->lpsfParent = NULL;
				lptvid->lpifq      = ConcatPidls(NULL, pidlDesktop);

				TCHAR szBuff[MAX_PATH];
				STRCPY_S(szBuff, MAX_PATH, fileInfo.szDisplayName);

				tvi.lParam     = (LPARAM)lptvid;
				tvi.pszText    = szBuff;
				tvi.cchTextMax = MAX_PATH;

				// Populate the TreeVeiw Insert Struct
				// The item is the one filled above.
				// Insert it after the last item inserted at this level.
				// And indicate this is a root entry.
				TV_INSERTSTRUCT tvins;
				tvins.item         = tvi;
				tvins.hInsertAfter = hItem;
				tvins.hParent      = hItem;

				// Add the item to the tree
				hItem = m_pTreeCtrl->InsertItem(&tvins);
			}

			if ( lpMalloc && pidlDesktop ) {
				lpMalloc->Free( pidlDesktop );
			}
		}
		if ( lpMalloc ) {
			lpMalloc->Release();
		}
	}

	return hItem;
}

void CXTShellTreeBase::PopulateTreeView()
{
	// Get a pointer to the desktop folder.
	LPSHELLFOLDER lpsf = NULL;
	if ( SUCCEEDED( ::SHGetDesktopFolder( &lpsf ) ) )
	{
		// turn off redraw and remove all tree items.
		m_pTreeCtrl->SetRedraw(FALSE);
		m_pTreeCtrl->DeleteAllItems( );

		HTREEITEM hItemDesktop = InsertDesktopItem( );

		// Fill in the tree view from the root.
		InitTreeViewItems( lpsf, NULL, hItemDesktop );

		// Release the folder pointer.
		lpsf->Release( );

		TV_SORTCB tvscb;
		tvscb.hParent     = hItemDesktop;
		tvscb.lParam      = 0;
		tvscb.lpfnCompare = TreeViewCompareProc;

		// Sort the items in the tree view
		m_pTreeCtrl->SortChildrenCB( &tvscb );

		HTREEITEM hItemRoot = m_pTreeCtrl->GetRootItem( );
		m_pTreeCtrl->Expand( hItemRoot, TVE_EXPAND );

		if ( hItemDesktop != TVI_ROOT )
		{
			HTREEITEM hItemChild = m_pTreeCtrl->GetChildItem( hItemDesktop );
			m_pTreeCtrl->Select( hItemChild, TVGN_CARET );

			if ( ( ::GetWindowLong( m_pTreeCtrl->m_hWnd, GWL_STYLE ) & TVS_SINGLEEXPAND ) == 0 )
			{
				m_pTreeCtrl->Expand( hItemChild, TVE_EXPAND );
			}
		}
		else
		{
			m_pTreeCtrl->Select( hItemRoot, TVGN_CARET );
		}

		// turn on redraw and refresh tree.
		m_pTreeCtrl->SetRedraw(TRUE);
		m_pTreeCtrl->RedrawWindow();
		m_pTreeCtrl->SetFocus( );
	}
}

void CXTShellTreeBase::SetAttributes(HTREEITEM hItem, DWORD dwAttributes)
{
	MapShellFlagsToItemAttributes(m_pTreeCtrl, hItem, dwAttributes, TVIS_CUT);
}

BOOL CXTShellTreeBase::InitTreeViewItems(LPSHELLFOLDER lpsf, LPITEMIDLIST  lpifq, HTREEITEM     hParent)
{
	CWaitCursor wait; // show wait cursor.

	// Allocate a shell memory object.
	LPMALLOC lpMalloc = NULL;
	if (SUCCEEDED(::SHGetMalloc(&lpMalloc)))
	{
		// Get the IEnumIDList object for the given folder.
		LPENUMIDLIST lpe = NULL;
		if (SUCCEEDED(lpsf->EnumObjects(::GetParent(m_pTreeCtrl->m_hWnd), m_uFlags, &lpe)))
		{
			ULONG        ulFetched = 0;
			HTREEITEM    hPrev     = NULL;
			LPITEMIDLIST lpi       = NULL;

			// Enumerate through the list of folder and non-folder objects.
			while (lpe->Next(1, &lpi, &ulFetched) == S_OK)
			{
				// Create a fully qualified path to the current item
				// the SH* shell api's take a fully qualified path pidl,
				// (see GetIcon above where I call SHGetFileInfo) whereas the
				// interface methods take a relative path pidl.
				ULONG ulAttrs = SFGAO_HASSUBFOLDER | SFGAO_FOLDER | SFGAO_DISPLAYATTRMASK | SFGAO_REMOVABLE;

				// Determine what type of object we have.
				lpsf->GetAttributesOf(1, (const struct _ITEMIDLIST **)&lpi, &ulAttrs);

				if (ulAttrs & (SFGAO_HASSUBFOLDER | SFGAO_FOLDER))
				{
					// We need this next if statement so that we don't add things like
					// the MSN to our tree.  MSN is not a folder, but according to the
					// shell it has subfolders.
					if (ulAttrs & SFGAO_FOLDER)
					{
						TV_ITEM  tvi;
						tvi.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_PARAM;

						if (ulAttrs & SFGAO_HASSUBFOLDER)
						{
							//This item has sub-folders, so let's put the + in the TreeView.
							//The first time the user clicks on the item, we'll populate the
							//sub-folders.
							tvi.cChildren = 1;
							tvi.mask |= TVIF_CHILDREN;
						}

						// Allocate memory for ITEMDATA struct
						TCHAR szBuff[256];
						XT_TVITEMDATA* lptvid = (XT_TVITEMDATA*)lpMalloc->Alloc(sizeof(XT_TVITEMDATA));
						if (lptvid == NULL || GetName(lpsf, lpi, SHGDN_NORMAL, szBuff) == FALSE)
						{
							if (lpe) {
								lpe->Release();
							}
							if (lpi && lpMalloc) {
								lpMalloc->Free(lpi);
							}
							if (lpMalloc) {
								lpMalloc->Release();
							}
							return FALSE;
						}

						LPITEMIDLIST lpifqThisItem = ConcatPidls(lpifq, lpi);
						GetNormalAndSelectedIcons(lpifqThisItem, &tvi);

						// Now, make a copy of the ITEMIDLIST and store the parent folders SF.
						lptvid->lpi        = DuplicateItem(lpMalloc, lpi);
						lptvid->lpsfParent = lpsf;
						lptvid->lpifq      = ConcatPidls(lpifq, lpi);
						lpsf->AddRef();

						tvi.lParam     = (LPARAM)lptvid;
						tvi.pszText    = szBuff;
						tvi.cchTextMax = MAX_PATH;

						// Populate the TreeVeiw Insert Struct
						// The item is the one filled above.
						// Insert it after the last item inserted at this level.
						// And indicate this is a root entry.
						TV_INSERTSTRUCT tvins;
						tvins.item         = tvi;
						tvins.hInsertAfter = hPrev;
						tvins.hParent      = hParent;

						// Add the item to the tree
						hPrev = m_pTreeCtrl->InsertItem(&tvins);
						SetAttributes(hPrev, ulAttrs);

						// Free this items task allocator.
						if (lpifqThisItem && lpMalloc) {
							lpMalloc->Free(lpifqThisItem);
						}
					}
				}

				// Free the pidl that the shell gave us.
				if (lpi && lpMalloc) {
					lpMalloc->Free(lpi);
					lpi = 0;
				}
			}
			if (lpi && lpMalloc) {
				lpMalloc->Free(lpi);
			}
			if (lpe) {
				lpe->Release();
			}
		}
		if (lpMalloc) {
			lpMalloc->Release();
		}

		return TRUE;
	}

	return FALSE;
}

void CXTShellTreeBase::GetNormalAndSelectedIcons(LPITEMIDLIST lpifq, LPTV_ITEM lptvitem)
{
	// Note that we don't check the return value here because if GetIcon()
	// fails, then we're in big trouble...
	lptvitem->iImage = GetItemIcon(lpifq,
		SHGFI_PIDL | SHGFI_SYSICONINDEX | SHGFI_SMALLICON);

	lptvitem->iSelectedImage = GetItemIcon(lpifq,
		SHGFI_PIDL | SHGFI_SYSICONINDEX | SHGFI_SMALLICON | SHGFI_OPENICON);
}

void CXTShellTreeBase::OnFolderExpanding(NM_TREEVIEW* pNMTreeView)
{
	if (!(pNMTreeView->itemNew.state & TVIS_EXPANDEDONCE))
	{
		// Long pointer to TreeView item data
		XT_TVITEMDATA* lptvid = (XT_TVITEMDATA*)pNMTreeView->itemNew.lParam;
		if (lptvid != NULL && lptvid->lpsfParent != NULL )
		{
			LPSHELLFOLDER lpsf = NULL;
			if (SUCCEEDED(lptvid->lpsfParent->BindToObject(lptvid->lpi,
				0, IID_IShellFolder,(LPVOID *)&lpsf))) {
				InitTreeViewItems(lpsf, lptvid->lpifq, pNMTreeView->itemNew.hItem);
			}

			TV_SORTCB tvscb;
			tvscb.hParent     = pNMTreeView->itemNew.hItem;
			tvscb.lParam      = 0;
			tvscb.lpfnCompare = TreeViewCompareProc;
			m_pTreeCtrl->SortChildrenCB(&tvscb);
		}
	}
}

HTREEITEM CXTShellTreeBase::GetContextMenu()
{
	CPoint point;
	::GetCursorPos(&point);
	m_pTreeCtrl->ScreenToClient(&point);

	TV_HITTESTINFO tvhti;
	tvhti.pt = point;
	m_pTreeCtrl->HitTest(&tvhti);

	if (tvhti.flags & (TVHT_ONITEMLABEL|TVHT_ONITEMICON))
	{
		// Long pointer to TreeView item data
		XT_TVITEMDATA*  lptvid = (XT_TVITEMDATA*)m_pTreeCtrl->GetItemData(tvhti.hItem);

		m_pTreeCtrl->ClientToScreen(&point);

		if ( lptvid->lpsfParent == NULL )
		{
			LPMALLOC lpMalloc = NULL;
			if ( FAILED( ::SHGetMalloc( &lpMalloc ) ) )
			{
				return NULL;
			}

			LPSHELLFOLDER lpShellFolder;
			if ( FAILED( ::SHGetDesktopFolder( &lpShellFolder ) ) )
			{
				return NULL;
			}

			LPITEMIDLIST pidlDesktop = NULL;
			if ( FAILED( ::SHGetSpecialFolderLocation( NULL, CSIDL_DESKTOP, &pidlDesktop ) ) )
			{
				lpShellFolder->Release();
				return NULL;
			}

			IShellFolder *pFolder = NULL;
			if ( lpShellFolder->CompareIDs( 0, lptvid->lpifq,pidlDesktop ) )
			{
				pFolder = lpShellFolder;
			}
			else
			{
				lpShellFolder->BindToObject( lptvid->lpifq, 0, IID_IShellFolder, ( LPVOID* )&pFolder );
			}

			ShowContextMenu(m_pTreeCtrl->m_hWnd,
				lpShellFolder, lptvid->lpi, &point);

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
			ShowContextMenu(m_pTreeCtrl->m_hWnd,
				lptvid->lpsfParent, lptvid->lpi, &point);
		}

		return tvhti.hItem;
	}

	return NULL;
}

BOOL CXTShellTreeBase::OnFolderSelected(NM_TREEVIEW* pNMTreeView, CString &strFolderPath)
{
	BOOL bRet = FALSE;

	HTREEITEM hItem = m_pTreeCtrl->GetSelectedItem();
	if (hItem != NULL)
	{
		LPSHELLFOLDER lpsf = NULL;

		// Long pointer to TreeView item data
		XT_TVITEMDATA*  lptvid = (XT_TVITEMDATA*)m_pTreeCtrl->GetItemData(hItem);
		if (lptvid && lptvid->lpsfParent && lptvid->lpi)
		{
			if (SUCCEEDED(lptvid->lpsfParent->BindToObject(lptvid->lpi,
				0, IID_IShellFolder,(LPVOID*)&lpsf)))
			{
				ULONG ulAttrs = SFGAO_FILESYSTEM;

				// Determine what type of object we have.
				lptvid->lpsfParent->GetAttributesOf(1, (const struct _ITEMIDLIST **)&lptvid->lpi, &ulAttrs);

				if (ulAttrs & (SFGAO_FILESYSTEM))
				{
					TCHAR szBuff[MAX_PATH];
					if(::SHGetPathFromIDList(lptvid->lpifq, szBuff))
					{
						strFolderPath = szBuff;
						bRet = TRUE;
					}
				}

				if (m_pTreeCtrl->ItemHasChildren(pNMTreeView->itemNew.hItem) && !(pNMTreeView->itemNew.state & TVIS_EXPANDEDONCE))
				{
					InitTreeViewItems(lpsf, lptvid->lpifq, pNMTreeView->itemNew.hItem);

					TV_SORTCB tvscb;
					tvscb.hParent     = pNMTreeView->itemNew.hItem;
					tvscb.lParam      = 0;
					tvscb.lpfnCompare = TreeViewCompareProc;
					m_pTreeCtrl->SortChildrenCB(&tvscb);

					pNMTreeView->itemNew.state     |= TVIS_EXPANDEDONCE;
					pNMTreeView->itemNew.stateMask |= TVIS_EXPANDEDONCE;
					pNMTreeView->itemNew.mask      |= TVIF_STATE;
					m_pTreeCtrl->SetItem(&pNMTreeView->itemNew);
				}

				if(lpsf) {
					lpsf->Release();
				}
			}
		}
	}

	return bRet;
}

BOOL CXTShellTreeBase::InitSystemImageLists()
{
	SHFILEINFO sfi;
	HIMAGELIST himlSmall = (HIMAGELIST)::SHGetFileInfo( _T("C:\\"), 0, &sfi,
		sizeof(SHFILEINFO), SHGFI_SYSICONINDEX | SHGFI_SMALLICON );

	if (himlSmall) {
		m_pTreeCtrl->SetImageList(CImageList::FromHandle(himlSmall), TVSIL_NORMAL);
		return TRUE;
	}

	return FALSE;
}

BOOL CXTShellTreeBase::GetSelectedFolderPath(CString &strFolderPath)
{
	BOOL bRet = FALSE;

	HTREEITEM hItem = m_pTreeCtrl->GetSelectedItem();
	if (hItem != NULL)
	{
		// Long pointer to TreeView item data
		XT_TVITEMDATA*  lptvid = (XT_TVITEMDATA*)m_pTreeCtrl->GetItemData(hItem);
		if (lptvid && lptvid->lpsfParent && lptvid->lpi)
		{
			LPSHELLFOLDER lpsf = NULL;
			if (SUCCEEDED(lptvid->lpsfParent->BindToObject(lptvid->lpi,
				0,IID_IShellFolder,(LPVOID *)&lpsf)))
			{
				ULONG ulAttrs = SFGAO_FILESYSTEM;

				// Determine what type of object we have.
				lptvid->lpsfParent->GetAttributesOf(1, (const struct _ITEMIDLIST **)&lptvid->lpi, &ulAttrs);

				if (ulAttrs & (SFGAO_FILESYSTEM))
				{
					TCHAR szBuff[MAX_PATH];
					if(::SHGetPathFromIDList(lptvid->lpifq,szBuff))
					{
						strFolderPath = szBuff;
						bRet = TRUE;
					}
				}
				if(lpsf) {
					lpsf->Release();
				}
			}
		}
	}

	return bRet;
}

BOOL CXTShellTreeBase::FindTreeItem(HTREEITEM hItem, XT_LVITEMDATA* lplvid)
{
	while (hItem)
	{
		// Long pointer to TreeView item data
		XT_TVITEMDATA*  lptvid = (XT_TVITEMDATA*)m_pTreeCtrl->GetItemData(hItem);
		if(lptvid && lplvid)
		{
			if (SCODE_CODE(GetScode(lplvid->lpsfParent->CompareIDs(
				0, lplvid->lpi, lptvid->lpi))) == 0)
			{
				m_pTreeCtrl->EnsureVisible(hItem);
				m_pTreeCtrl->SelectItem(hItem);
				return TRUE;
			}
		}

		HTREEITEM hNextItem = m_pTreeCtrl->GetChildItem(hItem);
		if(hNextItem)
		{
			if (FindTreeItem(hNextItem, lplvid)) {
				return TRUE;
			}
		}

		hItem = m_pTreeCtrl->GetNextSiblingItem(hItem);
	}

	return FALSE;
}

void CXTShellTreeBase::OnItemexpanding(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	OnFolderExpanding(pNMTreeView);
	*pResult = 0;
}

void CXTShellTreeBase::SelectionChanged(HTREEITEM hItem, CString strFolderPath)
{
	if (hItem != NULL && !m_bTunneling)
	{
		/*CDocument* pDoc = GetDocument( );
		if ( pDoc != NULL )
		{
			pDoc->UpdateAllViews( this, SHN_XT_TREESELCHANGE,
				( CObject* )GetTreeCtrl( ).GetItemData( hItem ) );
		}   */

		if (!m_pComboBox || !::IsWindow(m_pComboBox->m_hWnd))
			return;

		// update combo box association.
		if (m_pComboBox->IsKindOf(RUNTIME_CLASS(CXTComboBoxEx)))
		{
			CXTComboBoxEx* pComboBoxEx = DYNAMIC_DOWNCAST(CXTComboBoxEx, m_pComboBox);
			ASSERT_VALID(pComboBoxEx);

			int nFound = CB_ERR;
			int nIndex;
			for (nIndex = 0; nIndex < pComboBoxEx->GetCount(); ++nIndex)
			{
				CString strText;
				pComboBoxEx->GetLBText(nIndex, strText);

				if (strFolderPath.Compare(strText) == 0)
				{
					nFound = nIndex;
					pComboBoxEx->SetCurSel(nIndex);
					break;
				}
			}

			if (nFound == CB_ERR)
			{
				HTREEITEM hti = m_pTreeCtrl->GetSelectedItem();
				ASSERT(hti);

				if (strFolderPath.IsEmpty()) {
					strFolderPath = m_pTreeCtrl->GetItemText(hti);
				}

				int nImage, nSelectedImage;
				m_pTreeCtrl->GetItemImage(hti, nImage, nSelectedImage);

				pComboBoxEx->InsertItem(0, strFolderPath, 0, nImage, nImage);
				pComboBoxEx->SetCurSel(0);
				pComboBoxEx->SetItemDataPtr(0, hti);
			}
		}

		else if (m_pComboBox->IsKindOf(RUNTIME_CLASS(CComboBox)))
		{
			CComboBox* pComboBox = DYNAMIC_DOWNCAST(CComboBox, m_pComboBox);
			ASSERT_VALID(pComboBox);

			int nFound = pComboBox->FindString(-1, strFolderPath);
			if (nFound == CB_ERR)
			{
				HTREEITEM hti = m_pTreeCtrl->GetSelectedItem();
				ASSERT(hti);

				pComboBox->InsertString(0, strFolderPath);
				pComboBox->SetCurSel(0);
				pComboBox->SetItemDataPtr(0, (HTREEITEM)hti);
			}
			else {
				pComboBox->SetCurSel(nFound);
			}
		}
	}
}

void CXTShellTreeBase::OnSelchanged(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;

	CString strFolderPath;
	OnFolderSelected(pNMTreeView, strFolderPath);

	if (strFolderPath.IsEmpty()) {
		strFolderPath = m_pTreeCtrl->GetItemText(pNMTreeView->itemNew.hItem);
	}

	// currently selected TreeItem
	SelectionChanged(m_pTreeCtrl->GetSelectedItem(), strFolderPath);

	*pResult = 0;
}

void CXTShellTreeBase::OnRclick(NMHDR* pNMHDR, LRESULT* pResult)
{
	UNUSED_ALWAYS(pNMHDR);

	// Display the shell context menu.
	if (m_bContextMenu == TRUE)
	{
		HTREEITEM hItem = GetContextMenu();
		if (hItem != NULL)
		{
			// TODO: Additional error handling.
		}
	}

	*pResult = 0;
}


/*BOOL CXTShellTreeBase::PreCreateWindow(CREATESTRUCT& cs)
{
	if (!CXTTreeView::PreCreateWindow(cs))
		return FALSE;

	cs.style |= TVS_HASLINES | TVS_HASBUTTONS | TVS_LINESATROOT;

	return TRUE;
}*/

BOOL CXTShellTreeBase::InitializeTree(DWORD dwStyle/*=TVS_HASBUTTONS|TVS_HASLINES|TVS_LINESATROOT*/)
{
	if (m_pTreeCtrl->GetImageList(TVSIL_NORMAL) == NULL)
	{
		// Initialize the image list for the list view and populate it.
		if (!InitSystemImageLists())
			return FALSE;

		// Set the style for the tree control.
		m_pTreeCtrl->ModifyStyle(0, dwStyle);

		// Make sure multi-select mode is disabled.
		EnableMultiSelect(FALSE);

		return TRUE;
	}

	return FALSE;
}

/*void CXTShellTreeBase::OnInitialUpdate()
{
	CXTTreeView::OnInitialUpdate();

	// Initialize the image list for the list view and populate it.
	if (InitializeTree( XTOSVersionInfo()->IsWinXPOrGreater() ?
		(TVS_HASBUTTONS|TVS_SINGLEEXPAND|TVS_TRACKSELECT|TVS_SHOWSELALWAYS) :
		(TVS_HASBUTTONS|TVS_HASLINES|TVS_LINESATROOT) ))
	{
		PopulateTreeView();
	}
}

void CXTShellTreeBase::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	UNUSED_ALWAYS(pSender);

	switch (lHint)
	{
	// Update selection of tree view to the
	case SHN_XT_SELECTCHILD:
		{
			XT_LVITEMDATA*  lplvid = (XT_LVITEMDATA*)pHint;
			ASSERT(lplvid != NULL);

			LockWindowUpdate();
			if(!FindTreeItem(m_pTreeCtrl->GetSelectedItem(), lplvid))
			{
				// The folder was not found so we send back a message
				// to the listview to execute the itemid
				CDocument* pDoc = GetDocument();
				pDoc->UpdateAllViews(this, SHN_XT_NOFOLDER, (CObject*)lplvid);
			}
			UnlockWindowUpdate();
		}
		break;

	default:
		break;
	}
}*/

void CXTShellTreeBase::InitTreeNode(HTREEITEM hItem, XT_TVITEMDATA* lptvid)
{
	m_pTreeCtrl->SetRedraw(FALSE);
	if (lptvid)
	{
		LPSHELLFOLDER lpsf = NULL;
		if (SUCCEEDED(lptvid->lpsfParent->BindToObject(lptvid->lpi,
			0, IID_IShellFolder,(LPVOID *)&lpsf))) {
			InitTreeViewItems(lpsf, lptvid->lpifq, hItem);
		}

		TV_SORTCB tvscb;
		tvscb.hParent     = hItem;
		tvscb.lParam      = 0;
		tvscb.lpfnCompare = TreeViewCompareProc;

		m_pTreeCtrl->SortChildrenCB(&tvscb);
	}

	m_pTreeCtrl->SetRedraw();
}

BOOL CXTShellTreeBase::SearchTree(HTREEITEM hItem, CString strSearchName, XTFindType attr, BOOL bFindRoot)
{
	BOOL bRet = FALSE;

	while (hItem && bRet == FALSE)
	{
		// Long pointer to TreeView item data
		XT_TVITEMDATA*  lptvid = (XT_TVITEMDATA*)m_pTreeCtrl->GetItemData(hItem);
		if (lptvid && lptvid->lpsfParent && lptvid->lpi)
		{
			LPSHELLFOLDER lpsf = NULL;
			if (SUCCEEDED(lptvid->lpsfParent->BindToObject(lptvid->lpi,
				0,IID_IShellFolder,(LPVOID *)&lpsf)))
			{
				ULONG ulAttrs = SFGAO_FILESYSTEM;
				lptvid->lpsfParent->GetAttributesOf(1,
					(const struct _ITEMIDLIST **)&lptvid->lpi, &ulAttrs);
				if (ulAttrs & (SFGAO_FILESYSTEM))
				{
					TCHAR szBuff[MAX_PATH];
					if (::SHGetPathFromIDList(lptvid->lpifq, szBuff))
					{
						switch(attr)
						{
						case xtFindTypeDrive:
							{
								TCHAR szDrive[_MAX_DRIVE];
								TCHAR szDir[_MAX_DIR];
								TCHAR szFileName[_MAX_FNAME];
								TCHAR szExt[_MAX_EXT];

								SPLITPATH_S(szBuff, szDrive, szDir, szFileName, szExt);
								STRCPY_S(szBuff, MAX_PATH, szDrive);

								// Making this null will cause the compare to fail
								if ( bFindRoot && _tcslen( szDir ) != 1 )
								{
									szBuff[0] = 0;
								}
							}
							break;

						case xtFindTypeFolder:
							{
								CString strItemText(m_pTreeCtrl->GetItemText(hItem));
								STRCPY_S(szBuff, MAX_PATH, strItemText);
							}
							break;
						}

						if (strSearchName.CompareNoCase(szBuff) == 0)
						{
							m_pTreeCtrl->EnsureVisible(hItem);
							m_pTreeCtrl->SelectItem(hItem);
							bRet = TRUE;
						}
					}
				}
				lpsf->Release();
			}
		}
		hItem = m_pTreeCtrl->GetNextSiblingItem(hItem);
	}
	return bRet;
}

void CXTShellTreeBase::TunnelTree(CString strFindPath)
{
	if ( strFindPath.GetLength() == 1 )
	{
		strFindPath += _T(":\\");
	}
	if ( strFindPath.GetLength() == 2 && strFindPath.GetAt( 1 ) == ':' )
	{
		strFindPath += _T("\\");
	}

	if (ACCESS_S(strFindPath, 0) == -1)
	{
		if (strFindPath.GetLength() != 3) {
			m_pTreeCtrl->MessageBox(strFindPath, _T("Folder not found"), MB_ICONERROR);
			return;
		}
	}

	CString strSelectedPath;
	GetSelectedFolderPath(strSelectedPath);

	if (strSelectedPath == strFindPath)
		return;

	m_bTunneling = true;

	if(strFindPath.ReverseFind(_T('\\')) != strFindPath.GetLength()-1) {
		strFindPath += _T("\\");
	}

	BOOL bFound = FALSE;
	BOOL bLock = m_pTreeCtrl->LockWindowUpdate();
	BOOL bFindRoot = FALSE;

	HTREEITEM hItemRoot = m_pTreeCtrl->GetRootItem();
	hItemRoot = m_pTreeCtrl->GetChildItem( hItemRoot );

	while (hItemRoot && !bFound)
	{
		TCHAR szDrive[_MAX_DRIVE];
		TCHAR szDir[_MAX_DIR];
		TCHAR szFileName[_MAX_FNAME];
		TCHAR szExt[_MAX_EXT];

		SPLITPATH_S(strFindPath, szDrive, szDir, szFileName, szExt);
		bFindRoot = _tcslen(szDir) == 1; // There will only be a single backslash if it's the root

		// Ensure child items are populated
		UINT uState = m_pTreeCtrl->GetItemState(hItemRoot, TVIS_EXPANDEDONCE);
		if (!(uState & TVIS_EXPANDEDONCE))
		{
			InitTreeNode(hItemRoot, (XT_TVITEMDATA*)m_pTreeCtrl->GetItemData(hItemRoot));
			m_pTreeCtrl->SetItemState(hItemRoot, TVIS_EXPANDEDONCE, TVIS_EXPANDEDONCE);
		}

		CString strItemText = m_pTreeCtrl->GetItemText(hItemRoot);
		HTREEITEM hItemNext = m_pTreeCtrl->GetChildItem(hItemRoot);
		strItemText = m_pTreeCtrl->GetItemText(hItemNext);

		// search the szDrive first
		if ( SearchTree( hItemNext, szDrive, xtFindTypeDrive, bFindRoot ) )
		{
			if ( bFindRoot )
			{
				bFound = TRUE;
			}
			else
			{
				TCHAR* lpszContext = 0;
				// break down subfolders and search
				TCHAR* pszNext = STRTOK_S(szDir, _T("\\"), &lpszContext);
				while(pszNext)
				{
					HTREEITEM hItem = m_pTreeCtrl->GetSelectedItem();
					strItemText = m_pTreeCtrl->GetItemText(hItem);

					// Ensure child items are populated
					uState = m_pTreeCtrl->GetItemState(hItem, TVIS_EXPANDEDONCE);
					if (!(uState & TVIS_EXPANDEDONCE))
					{
						InitTreeNode(hItem, (XT_TVITEMDATA*)m_pTreeCtrl->GetItemData(hItem));
						m_pTreeCtrl->SetItemState(hItem, TVIS_EXPANDEDONCE, TVIS_EXPANDEDONCE);
					}

					HTREEITEM hItemChild = m_pTreeCtrl->GetChildItem(hItem);
					if (hItemChild) {
						strItemText = m_pTreeCtrl->GetItemText(hItemChild);
					}

					// Find?
					if(SearchTree(hItemChild, pszNext, xtFindTypeFolder))
					{
						// We know this was successful - expand at this
						m_pTreeCtrl->Expand(hItem, TVE_EXPAND);
						pszNext = STRTOK_S(NULL, _T("\\"), &lpszContext);

						// No more?
						if (pszNext == NULL)
						{
							// Tunnelled successfully down
							bFound = TRUE;
							break;
						}
					}
					else
					{
						// Abort as this part not found
						pszNext = NULL;
					}
				}
			}
		}

		// Try next item in the namespace
		hItemRoot = m_pTreeCtrl->GetNextSiblingItem(hItemRoot);
	}

	if (bLock) {
		m_pTreeCtrl->UnlockWindowUpdate();
	}

	m_bTunneling = false;

	// make sure list gets updated.
	SelectionChanged(m_pTreeCtrl->GetSelectedItem(), strFindPath);
}

BOOL CXTShellTreeBase::GetFolderItemPath(HTREEITEM hItem, CString &strFolderPath)
{
	BOOL bRet = FALSE;

	// Long pointer to TreeView item data
	XT_TVITEMDATA*  lptvid = (XT_TVITEMDATA*)m_pTreeCtrl->GetItemData(hItem);
	if (lptvid && lptvid->lpsfParent && lptvid->lpi)
	{
		LPSHELLFOLDER lpsf = NULL;
		if (SUCCEEDED(lptvid->lpsfParent->BindToObject(lptvid->lpi,
			0,IID_IShellFolder,(LPVOID *)&lpsf)))
		{
			ULONG ulAttrs = SFGAO_FILESYSTEM;

			// Determine what type of object we have.
			lptvid->lpsfParent->GetAttributesOf(1,
				(const struct _ITEMIDLIST **)&lptvid->lpi, &ulAttrs);

			if (ulAttrs & (SFGAO_FILESYSTEM))
			{
				TCHAR szBuff[MAX_PATH];
				if (::SHGetPathFromIDList(lptvid->lpifq, szBuff))
				{
					strFolderPath = szBuff;
					bRet = TRUE;
				}
			}
		}
		if(lpsf) {
			lpsf->Release();
		}
	}

	return bRet;
}

void CXTShellTreeBase::PopulateTree(LPCTSTR lpszPath)
{
	CString strFolder = lpszPath;
	CString strNextFolder;
	CString strPath;

	LPMALLOC lpMalloc;
	if (::SHGetMalloc(&lpMalloc) == NOERROR)
	{
		// Get a pointer to the desktop folder.
		LPSHELLFOLDER lpSF = NULL;
		if (SUCCEEDED(::SHGetDesktopFolder(&lpSF)))
		{
			LPITEMIDLIST lpIDL = NULL;
			USES_CONVERSION;

			// turn off redraw and remove all tree items.
			m_pTreeCtrl->SetRedraw(FALSE);
			m_pTreeCtrl->DeleteAllItems();

			do
			{
				// Get the Next Component
				strNextFolder = XTFuncPathFindNextComponent((TCHAR*)(LPCTSTR)strFolder);
				if (!strNextFolder.IsEmpty())
				{
					strPath = strFolder.Left(strFolder.GetLength() -
						strNextFolder.GetLength());
				}
				else
				{
					strPath = strFolder;
					strNextFolder.Empty();
				}

				// Get ShellFolder Pidl
				ULONG eaten;
				if (FAILED(lpSF->ParseDisplayName( NULL, NULL, T2OLE((TCHAR*)(LPCTSTR)strPath),
					&eaten, &lpIDL, NULL))) {
					break;
				}

				LPSHELLFOLDER lpSF2 = NULL;
				if (FAILED(lpSF->BindToObject(lpIDL, 0, IID_IShellFolder, (LPVOID*)&lpSF2))) {
					break;
				}

				lpMalloc->Free(lpIDL);

				// Release the Parent Folder pointer.
				lpSF->Release();

				// Change Folder Info
				lpSF = lpSF2;
				strFolder = strNextFolder;
			}
			while (!strNextFolder.IsEmpty());

			// get the base folders item ide list.
			lpIDL = IDLFromPath(lpszPath);

			SHFILEINFO fileInfo;
			::ZeroMemory( &fileInfo, sizeof( fileInfo ) );

			::SHGetFileInfo((LPCTSTR)lpIDL, NULL, &fileInfo, sizeof(fileInfo),
				SHGFI_PIDL | SHGFI_ATTRIBUTES | SHGFI_DISPLAYNAME);

			TV_ITEM  tvi;
			tvi.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_PARAM;

			// Allocate memory for ITEMDATA struct
			XT_TVITEMDATA* lptvid = (XT_TVITEMDATA*)lpMalloc->Alloc(sizeof(XT_TVITEMDATA));
			if (lptvid != NULL )
			{
				HTREEITEM hItem = TVI_ROOT;

				// get the normal and selected icons for the path.
				GetNormalAndSelectedIcons(lpIDL, &tvi);

				// Now, make a copy of the ITEMIDLIST and store the parent folders SF.
				lptvid->lpi        = DuplicateItem(lpMalloc, lpIDL);
				lptvid->lpsfParent = NULL;
				lptvid->lpifq      = ConcatPidls(NULL, lpIDL);

				TCHAR szBuff[MAX_PATH];
				STRCPY_S(szBuff, MAX_PATH, fileInfo.szDisplayName);

				tvi.lParam     = (LPARAM)lptvid;
				tvi.pszText    = szBuff;
				tvi.cchTextMax = MAX_PATH;

				// Populate the TreeView Insert Struct
				// The item is the one filled above.
				// Insert it after the last item inserted at this level.
				// And indicate this is a root entry.
				TV_INSERTSTRUCT tvins;
				tvins.item         = tvi;
				tvins.hInsertAfter = hItem;
				tvins.hParent      = hItem;

				// Add the item to the tree
				hItem = m_pTreeCtrl->InsertItem(&tvins);

				// insert child items.
				InitTreeViewItems(lpSF, lpIDL, hItem);

				// Sort the items in the tree view
				TV_SORTCB tvscb;
				tvscb.hParent     = TVI_ROOT;
				tvscb.lParam      = 0;
				tvscb.lpfnCompare = TreeViewCompareProc;
				m_pTreeCtrl->SortChildrenCB(&tvscb);

				// expand parent.
				m_pTreeCtrl->Expand(hItem, TVE_EXPAND);
			}

			// turn on redraw and refresh tree.
			m_pTreeCtrl->SetRedraw(TRUE);
			m_pTreeCtrl->RedrawWindow();

			lpMalloc->Free(lpIDL);

			if ( lpSF ) {
				lpSF->Release();
			}
		}
		if ( lpMalloc ) {
			lpMalloc->Release();
		}
	}
}

void CXTShellTreeBase::AssociateCombo(CWnd* pWnd)
{
	ASSERT(pWnd != NULL);
	m_pComboBox = pWnd;

	if (m_pComboBox->IsKindOf(RUNTIME_CLASS(CXTComboBoxEx)))
	{
		SHFILEINFO sfi;
		HIMAGELIST hImageList = (HIMAGELIST)::SHGetFileInfo( _T("C:\\"), 0, &sfi,
			sizeof(SHFILEINFO), SHGFI_SYSICONINDEX | SHGFI_SMALLICON );

		if (hImageList != NULL)
		{
			CXTComboBoxEx* pComboBoxEx = DYNAMIC_DOWNCAST(CXTComboBoxEx, m_pComboBox);
			ASSERT_VALID(pComboBoxEx);
			pComboBoxEx->SetImageList(CImageList::FromHandle(hImageList));
		}
	}
}

BOOL CXTShellTreeBase::OnEraseBkgnd(CDC* pDC)
{
	UNUSED_ALWAYS(pDC);
	return TRUE;
}

void CXTShellTreeBase::OnPaint()
{
	CPaintDC dc(m_pTreeCtrl);
	DoPaint(dc, FALSE);
}

void CXTShellTreeBase::OnDragDrop(NM_TREEVIEW* pNMTreeView)
{
	// Long pointer to ListView item data
	XT_TVITEMDATA*  lplvid = (XT_TVITEMDATA*)m_pTreeCtrl->GetItemData(pNMTreeView->itemNew.hItem);
	ASSERT(lplvid);

	if ( lplvid && lplvid->lpsfParent )
	{
		LPDATAOBJECT lpdo;

		HRESULT hResult = lplvid->lpsfParent->GetUIObjectOf( AfxGetMainWnd( )->m_hWnd, 1,
			( const struct _ITEMIDLIST** )&lplvid->lpi, IID_IDataObject, 0, ( LPVOID* )&lpdo );

		if ( SUCCEEDED( hResult ) )
		{
			LPDROPSOURCE lpds = new CXTDropSource();
			ASSERT(lpds != NULL);

			DWORD dwEffect;
			::DoDragDrop(lpdo, lpds,
				DROPEFFECT_COPY | DROPEFFECT_MOVE | DROPEFFECT_LINK, &dwEffect);

			lpdo->Release();
			lpds->Release();
		}
	}
}
