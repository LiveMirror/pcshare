// ResourceTreeView.cpp : implementation file
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
#include "TabbedView.h"
#include "ResourceTreeView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CResourceTreeView

IMPLEMENT_DYNCREATE(CResourceTreeView, CTreeView)

CResourceTreeView::CResourceTreeView()
{
}

CResourceTreeView::~CResourceTreeView()
{
}


BEGIN_MESSAGE_MAP(CResourceTreeView, CTreeView)
	//{{AFX_MSG_MAP(CResourceTreeView)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CResourceTreeView drawing

void CResourceTreeView::OnDraw(CDC*)
{
}

/////////////////////////////////////////////////////////////////////////////
// CResourceTreeView diagnostics

#ifdef _DEBUG
void CResourceTreeView::AssertValid() const
{
	CTreeView::AssertValid();
}

void CResourceTreeView::Dump(CDumpContext& dc) const
{
	CTreeView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CResourceTreeView message handlers

BOOL CResourceTreeView::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.style |= TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS | TVS_SHOWSELALWAYS;

	return CTreeView::PreCreateWindow(cs);
}

int CResourceTreeView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CTreeView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// load the tree images bitmap and add it to the image list.
	if (!CreateImageList(m_ilResourceView, IDB_RESTREE))
		return 0;

	// Set the image list for the tree control.
	GetTreeCtrl().SetImageList( &m_ilResourceView, TVSIL_NORMAL );

	// add the parent item
	HTREEITEM htItem = GetTreeCtrl().InsertItem(_T("XtremeDemo resources"), 0, 1);
	GetTreeCtrl().SetItemState(htItem, TVIS_BOLD, TVIS_BOLD);

	HTREEITEM hti = GetTreeCtrl().InsertItem(_T("Accelerator"), 0, 1, htItem);
	GetTreeCtrl().InsertItem(_T("IDR_MAINFRAME"), 2, 2, hti);
	GetTreeCtrl().Expand(hti, TVE_EXPAND);

	hti = GetTreeCtrl().InsertItem(_T("Dialog"), 0, 1, htItem);
	GetTreeCtrl().InsertItem(_T("IDD_ABOUTBOX"), 3, 3, hti);
	GetTreeCtrl().Expand(hti, TVE_EXPAND);

	hti = GetTreeCtrl().InsertItem(_T("Icon"), 0, 1, htItem);
	GetTreeCtrl().InsertItem(_T("IDR_MAINFRAME"), 4, 4, hti);
	GetTreeCtrl().InsertItem(_T("IDR_XTREMETYPE"), 4, 4, hti);
	GetTreeCtrl().Expand(hti, TVE_EXPAND);

	hti = GetTreeCtrl().InsertItem(_T("Menu"), 0, 1, htItem);
	GetTreeCtrl().InsertItem(_T("IDR_MAINFRAME"), 5, 5, hti);
	GetTreeCtrl().InsertItem(_T("IDR_XTREMETYPE"), 5, 5, hti);

	hti = GetTreeCtrl().InsertItem(_T("String Table"), 0, 1, htItem);
	GetTreeCtrl().InsertItem(_T("String Table"), 6, 6, hti);

	hti = GetTreeCtrl().InsertItem(_T("Toolbar"), 0, 1, htItem);
	GetTreeCtrl().InsertItem(_T("IDR_MAINFRAME"), 7, 7, hti);

	hti = GetTreeCtrl().InsertItem(_T("Version"), 0, 1, htItem);
	GetTreeCtrl().InsertItem(_T("VS_VERSION_INFO"), 8, 8, hti);

	GetTreeCtrl().Expand (htItem, TVE_EXPAND);


	return 0;
}
