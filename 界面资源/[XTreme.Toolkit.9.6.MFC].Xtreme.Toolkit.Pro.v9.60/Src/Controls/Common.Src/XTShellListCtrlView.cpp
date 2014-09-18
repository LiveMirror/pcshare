// XTShellListCtrl.cpp : implementation file
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

#include "XTDefines.h"
#include "XTGlobal.h"
#include "XTVC50Helpers.h"
#include "XTFlatHeaderCtrl.h"
#include "XTListCtrlView.h"

#include "XTShellPidl.h"
#include "XTShellSettings.h"
#include "XTShellListBase.h"
#include "XTShellListCtrlView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CXTShellListView

CXTShellListView::CXTShellListView()
{
	ImplAttach(&GetListCtrl());
}

CXTShellListView::~CXTShellListView()
{
}

IMPLEMENT_DYNCREATE(CXTShellListView, CListView)

BEGIN_MESSAGE_MAP(CXTShellListView, CListView)
	ON_SHELLLIST_REFLECT()
	ON_MESSAGE(XTWM_SHELL_NOTIFY, OnUpdateShell)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXTShellListView message handlers

void CXTShellListView::OnDblclk(NMHDR* pNMHDR, LRESULT* pResult)
{
	UNUSED_ALWAYS(pNMHDR);

	int iIndex = GetDoubleClickedItem();
	if (iIndex >= 0)
	{
		CDocument* pDoc = GetDocument();
		pDoc->UpdateAllViews(this, SHN_XT_SELECTCHILD,
			(CObject*)GetListCtrl().GetItemData(iIndex));
	}
	*pResult = 0;
}

BOOL CXTShellListView::PreCreateWindow(CREATESTRUCT& cs)
{
	if (!CListView::PreCreateWindow(cs))
		return FALSE;

	cs.style |= LVS_REPORT | LVS_SHAREIMAGELISTS;

	return TRUE;
}

void CXTShellListView::OnInitialUpdate()
{
	CListView::OnInitialUpdate();

	if (m_bInitControl)
	{
		m_bInitControl = false;
		InitializeControl();
	}
}

LRESULT CXTShellListView::OnUpdateShell(WPARAM wParam, LPARAM lParam)
{
	UpdateList((int)wParam, (XT_TVITEMDATA*)lParam);
	return 0;
}

void CXTShellListView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	UNUSED_ALWAYS(pSender);

	UpdateList((int)lHint, (XT_TVITEMDATA*)pHint);
}


/////////////////////////////////////////////////////////////////////////////
// CXTShellListCtrl

CXTShellListCtrl::CXTShellListCtrl()
{
	ImplAttach(this);
	m_bInitControl = true;
	m_pTreeCtrl    = NULL;

	m_nNameColumnWidth = 200;
}

CXTShellListCtrl::~CXTShellListCtrl()
{
}

BEGIN_MESSAGE_MAP(CXTShellListCtrl, CListCtrl)
	ON_SHELLLIST_REFLECT()
	ON_MESSAGE(XTWM_SHELL_NOTIFY, OnUpdateShell)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXTShellListCtrl message handlers

void CXTShellListCtrl::OnDblclk(NMHDR* pNMHDR, LRESULT* pResult)
{
	UNUSED_ALWAYS(pNMHDR);

	int iIndex = GetDoubleClickedItem();
	if (iIndex >= 0 && m_pTreeCtrl != NULL)
	{
		m_pTreeCtrl->SendMessage(XTWM_SHELL_NOTIFY,
			(WPARAM)SHN_XT_SELECTCHILD, (LPARAM)(CObject*)GetItemData(iIndex));
	}
	*pResult = 0;
}


LRESULT CXTShellListCtrl::OnUpdateShell(WPARAM wParam, LPARAM lParam)
{
	UpdateList((int)wParam, (XT_TVITEMDATA*)lParam);
	return 0;
}



/////////////////////////////////////////////////////////////////////////////
// CXTShellListCtrlEx

CXTShellListCtrlEx::CXTShellListCtrlEx()
: m_pSyncWnd(0)
{
	m_bInitControl = false;
}

CXTShellListCtrlEx::~CXTShellListCtrlEx()
{
}


BEGIN_MESSAGE_MAP(CXTShellListCtrlEx, CXTShellListCtrl)
	//{{AFX_MSG_MAP(CXTShellListCtrlEx)
	ON_WM_LBUTTONDBLCLK()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXTShellListCtrlEx message handlers

bool CXTShellListCtrlEx::BrowseToFolder(LPCTSTR lpszPath)
{
	XT_TVITEMDATA lpTVID;

	LPITEMIDLIST  pidl;
	LPSHELLFOLDER pDesktopFolder;
	OLECHAR       szOleChar[MAX_PATH];
	ULONG         chEaten;
	ULONG         dwAttributes;
	HRESULT       hr;

	// Get a pointer to the Desktop's IShellFolder interface.
	if ( SUCCEEDED( ::SHGetDesktopFolder( &pDesktopFolder ) ) )
	{
		// IShellFolder::ParseDisplayName requires the file name be in
		// Unicode.

#if !defined( _UNICODE )
		::MultiByteToWideChar( CP_ACP, MB_PRECOMPOSED, lpszPath, -1,
			szOleChar, MAX_PATH );
#else
		STRCPY_S(szOleChar, MAX_PATH, lpszPath);
#endif

		// Convert the path to an ITEMIDLIST.
		hr = pDesktopFolder->ParseDisplayName( NULL,NULL,szOleChar,
			&chEaten,&pidl,&dwAttributes);

		if ( SUCCEEDED( hr ) )
		{
			IShellFolder *psfMyFolder;

			lpTVID.lpi = lpTVID.lpifq = pidl;

			pDesktopFolder->BindToObject( lpTVID.lpifq,NULL,
				IID_IShellFolder,(LPVOID*)&psfMyFolder );

			lpTVID.lpsfParent = psfMyFolder;
			PopulateListView( &lpTVID,lpTVID.lpsfParent );

			m_strItemPath = lpszPath;
			pDesktopFolder->Release();

			return true;
		}

		pDesktopFolder->Release();
	}

	return false;
}

void CXTShellListCtrlEx::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	CXTShellListCtrl::OnLButtonDblClk(nFlags, point);

	int iItem = GetDoubleClickedItem( );
	if ( iItem >= 0 )
	{
		XT_LVITEMDATA* lplvid = ( XT_LVITEMDATA* )GetItemData( iItem );
		ASSERT( lplvid != NULL );

		if ( ( lplvid->ulAttribs & SFGAO_FOLDER ) == SFGAO_FOLDER )
		{
			if ( m_strItemPath.GetAt( m_strItemPath.GetLength( ) -1 ) != _T( '\\' ) )
			{
				m_strItemPath += _T( '\\' );
			}

			m_strItemPath += GetItemText( iItem, 0 );

			BrowseToFolder( m_strItemPath );

			if ( m_pSyncWnd != 0 )
			{
				m_pSyncWnd->SetWindowText( m_strItemPath );
			}
		}
		else
		{
			ShellOpenItem( lplvid );
		}
	}
}
