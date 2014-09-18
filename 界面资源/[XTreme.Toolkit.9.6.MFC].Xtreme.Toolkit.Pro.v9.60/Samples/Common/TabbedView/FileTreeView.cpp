// FileTreeView.cpp : implementation file
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
#include "FileTreeView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFileTreeView

IMPLEMENT_DYNCREATE(CFileTreeView, CTreeView)

CFileTreeView::CFileTreeView()
{
}

CFileTreeView::~CFileTreeView()
{
}


BEGIN_MESSAGE_MAP(CFileTreeView, CTreeView)
	//{{AFX_MSG_MAP(CFileTreeView)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFileTreeView drawing

void CFileTreeView::OnDraw(CDC*)
{
}

/////////////////////////////////////////////////////////////////////////////
// CFileTreeView diagnostics

#ifdef _DEBUG
void CFileTreeView::AssertValid() const
{
	CTreeView::AssertValid();
}

void CFileTreeView::Dump(CDumpContext& dc) const
{
	CTreeView::Dump(dc);
}
#endif //_DEBUG

BOOL CFileTreeView::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.style |= TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS | TVS_SHOWSELALWAYS;

	return CTreeView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CFileTreeView message handlers

int CFileTreeView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CTreeView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// load the tree images bitmap and add it to the image list.
	if (!CreateImageList(m_ilSolutionView, IDB_FILETREE))
		return 0;

	// Set the image list for the tree control.
	GetTreeCtrl().SetImageList( &m_ilSolutionView, TVSIL_NORMAL );



	// add the parent item, make it bold
	HTREEITEM htiParent = GetTreeCtrl().InsertItem(_T("Solution 'XtremeDemo': 1 project(s)"));
	HTREEITEM htiChild; // child item

	htiChild = GetTreeCtrl().InsertItem (_T("XtremeDemo Files"), 1, 1, htiParent);
	GetTreeCtrl().SetItemState (htiChild, TVIS_BOLD, TVIS_BOLD);

	// add the children of the parent item
	HTREEITEM hti = GetTreeCtrl().InsertItem(_T("Source Files"), 2, 3, htiChild);
	GetTreeCtrl().InsertItem(_T("ChildFrm.cpp"), 4, 4, hti);
	GetTreeCtrl().InsertItem(_T("MainFrm.cpp"), 4, 4, hti);
	GetTreeCtrl().InsertItem(_T("StdAfx.cpp"), 4, 4, hti);
	GetTreeCtrl().InsertItem(_T("XtremeDemo.cpp"), 4, 4, hti);
	GetTreeCtrl().InsertItem(_T("XtremeDemo.rc"), 4, 4, hti);
	GetTreeCtrl().InsertItem(_T("XtremeDemoDoc.cpp"), 4, 4, hti);
	GetTreeCtrl().InsertItem(_T("XtremeDemoView.cpp"), 4, 4, hti);
	GetTreeCtrl().Expand(hti, TVE_EXPAND);

	hti = GetTreeCtrl().InsertItem(_T("Header Files"), 2, 3, htiChild);
	GetTreeCtrl().InsertItem(_T("ChildFrm.h"), 5, 5, hti);
	GetTreeCtrl().InsertItem(_T("MainFrm.h"), 5, 5, hti);
	GetTreeCtrl().InsertItem(_T("Resource.rc"), 5, 5, hti);
	GetTreeCtrl().InsertItem(_T("StdAfx.h"), 5, 5, hti);
	GetTreeCtrl().InsertItem(_T("XtremeDemo.h"), 5, 5, hti);
	GetTreeCtrl().InsertItem(_T("XtremeDemoDoc.h"), 5, 5, hti);
	GetTreeCtrl().InsertItem(_T("XtremeDemoView.h"), 5, 5, hti);

	hti = GetTreeCtrl().InsertItem(_T("Resource Files"), 2, 3, htiChild);
	GetTreeCtrl().InsertItem(_T("Toolbar.bmp"), 5, 5, hti);
	GetTreeCtrl().InsertItem(_T("XtremeDemo.ico"), 5, 5, hti);
	GetTreeCtrl().InsertItem(_T("XtremeDemo.rc2"), 5, 5, hti);
	GetTreeCtrl().InsertItem(_T("XtremeDemoDoc.ico"), 5, 5, hti);

	GetTreeCtrl().InsertItem(_T("ReadMe.txt"), 5, 5, htiChild);
	GetTreeCtrl().InsertItem(_T("External Dependencies"), 2, 3, htiChild);

	GetTreeCtrl().Expand(htiParent, TVE_EXPAND);
	GetTreeCtrl().Expand(htiChild, TVE_EXPAND);

	return 0;
}
