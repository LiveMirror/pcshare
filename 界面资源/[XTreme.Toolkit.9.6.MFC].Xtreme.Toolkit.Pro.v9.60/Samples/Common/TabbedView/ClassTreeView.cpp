// ClassTreeView.cpp : implementation file
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
#include "ClassTreeView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CClassTreeView

IMPLEMENT_DYNCREATE(CClassTreeView, CTreeView)

CClassTreeView::CClassTreeView()
{
}

CClassTreeView::~CClassTreeView()
{
}


BEGIN_MESSAGE_MAP(CClassTreeView, CTreeView)
	//{{AFX_MSG_MAP(CClassTreeView)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CClassTreeView drawing

void CClassTreeView::OnDraw(CDC*)
{
}

/////////////////////////////////////////////////////////////////////////////
// CClassTreeView diagnostics

#ifdef _DEBUG
void CClassTreeView::AssertValid() const
{
	CTreeView::AssertValid();
}

void CClassTreeView::Dump(CDumpContext& dc) const
{
	CTreeView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CClassTreeView message handlers

BOOL CClassTreeView::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.style |= TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS | TVS_SHOWSELALWAYS;

	return CTreeView::PreCreateWindow(cs);
}

int CClassTreeView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CTreeView::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!CreateImageList(m_ilClassView, IDB_CLASSTREE))
		return 0;

	// Set the image list for the tree control.
	GetTreeCtrl().SetImageList( &m_ilClassView, TVSIL_NORMAL );

	// Add the parent item
	HTREEITEM htItem = GetTreeCtrl().InsertItem(_T("XtremeDemo classes"));
	GetTreeCtrl().SetItemState( htItem, TVIS_BOLD, TVIS_BOLD );

	// Add children
	HTREEITEM hti = GetTreeCtrl().InsertItem(_T("CAboutDlg"), 3, 3, htItem);
	GetTreeCtrl().InsertItem(_T("CAboutDlg()"), 4, 4, hti);
	GetTreeCtrl().InsertItem(_T("DoDataExchange(CDataExchange *pDX)"), 5, 5, hti);

	hti = GetTreeCtrl().InsertItem(_T("CChildFrame"), 3, 3, htItem);
	GetTreeCtrl().InsertItem(_T("AssertValid()"), 4, 4, hti);
	GetTreeCtrl().InsertItem(_T("CChildFrame()"), 4, 4, hti);
	GetTreeCtrl().InsertItem(_T("~CChildFrame()"), 4, 4, hti);
	GetTreeCtrl().InsertItem(_T("Dump(CDumpContext &dc)"), 4, 4, hti);
	GetTreeCtrl().InsertItem(_T("PreCreateWindow(CREATESTRUCT& cs)"), 4, 4, hti);

	hti = GetTreeCtrl().InsertItem(_T("CMainFrame"), 3, 3, htItem);
	GetTreeCtrl().InsertItem(_T("AssertValid()"), 4, 4, hti);
	GetTreeCtrl().InsertItem(_T("CMainFrame()"), 4, 4, hti);
	GetTreeCtrl().InsertItem(_T("~CMainFrame()"), 4, 4, hti);
	GetTreeCtrl().InsertItem(_T("Dump(CDumpContext &dc)"), 4, 4, hti);
	GetTreeCtrl().InsertItem(_T("OnCreate(LPCREATESTRUCT lpCreateStruct)"), 5, 5, hti);
	GetTreeCtrl().InsertItem(_T("PreCreateWindow(CREATESTRUCT& cs)"), 4, 4, hti);
	GetTreeCtrl().InsertItem(_T("m_wndStatusBar"), 7, 7, hti);
	GetTreeCtrl().InsertItem(_T("m_wndToolBar"), 7, 7, hti);

	hti = GetTreeCtrl().InsertItem(_T("CXtremeDemoApp"), 3, 3, htItem);
	GetTreeCtrl().InsertItem(_T("CXtremeDemoApp()"), 4, 4, hti);
	GetTreeCtrl().InsertItem(_T("InitInstance()"), 4, 4, hti);
	GetTreeCtrl().InsertItem(_T("OnAppAbout()"), 4, 4, hti);

	hti = GetTreeCtrl().InsertItem(_T("CXtremeDemoDoc"), 3, 3, htItem);
	GetTreeCtrl().InsertItem(_T("AssertValid()"), 5, 5, hti);
	GetTreeCtrl().InsertItem(_T("CXtremeDemoDoc()"), 4, 4, hti);
	GetTreeCtrl().InsertItem(_T("~CXtremeDemoDoc()"), 4, 4, hti);
	GetTreeCtrl().InsertItem(_T("Dump(CDumpContext &dc)"), 4, 4, hti);
	GetTreeCtrl().InsertItem(_T("OnNewDocument()"), 4, 4, hti);
	GetTreeCtrl().InsertItem(_T("Serialize(CArchive& ar)"), 4, 4, hti);
	GetTreeCtrl().Expand(hti, TVE_EXPAND);

	hti = GetTreeCtrl().InsertItem(_T("CXtremeDemoView"), 3, 3, htItem);
	GetTreeCtrl().InsertItem(_T("AssertValid()"), 4, 4, hti);
	GetTreeCtrl().InsertItem(_T("CXtremeDemoView()"), 5, 5, hti);
	GetTreeCtrl().InsertItem(_T("~CXtremeDemoView()"), 4, 4, hti);
	GetTreeCtrl().InsertItem(_T("Dump(CDumpContext& dc)"), 4, 4, hti);
	GetTreeCtrl().InsertItem(_T("GetDocument()"), 4, 4, hti);
	GetTreeCtrl().InsertItem(_T("OnBeginPrinting(CDC* pDC, CPrintInfo *pInfo)"), 5, 5, hti);
	GetTreeCtrl().InsertItem(_T("OnDraw(CDC *pDC)"), 4, 4, hti);
	GetTreeCtrl().InsertItem(_T("OnEndPrinting(CDC *pDC, CPrintInfo *pInfo)"), 5, 5, hti);
	GetTreeCtrl().InsertItem(_T("OnPreparePrinting(CPrintInfo *pInfo)"), 5, 5, hti);
	GetTreeCtrl().InsertItem(_T("PreCreateWindow(CREATESTRUCT &cs)"), 4, 4, hti);

	hti = GetTreeCtrl().InsertItem(_T("Globals"), 1, 2, htItem);
	GetTreeCtrl().InsertItem(_T("theApp"), 6, 6, hti);
	GetTreeCtrl().Expand(hti, TVE_EXPAND);

	GetTreeCtrl().Expand(htItem, TVE_EXPAND);

	return 0;
}
