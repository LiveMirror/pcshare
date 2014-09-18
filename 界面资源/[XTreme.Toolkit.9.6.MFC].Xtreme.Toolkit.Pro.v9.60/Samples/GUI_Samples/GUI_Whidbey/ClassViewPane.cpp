// ClassViewPane.cpp : implementation file
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
#include "GUI_Whidbey.h"
#include "ClassViewPane.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CClassViewPane

CClassViewPane::CClassViewPane()
{
}

CClassViewPane::~CClassViewPane()
{
}


BEGIN_MESSAGE_MAP(CClassViewPane, CWnd)
	//{{AFX_MSG_MAP(CClassViewPane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_COMMAND(ID_PROJECT_ADDEXISTINGITEM, OnProjectAddexistingitem)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


static UINT toolbar[] =
{
	ID_PROJECT_ADDEXISTINGITEM
};
/////////////////////////////////////////////////////////////////////////////
// CClassViewPane message handlers

int CClassViewPane::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	VERIFY(m_wndToolBar.CreateToolBar(WS_TABSTOP|WS_VISIBLE|WS_CHILD|CBRS_TOOLTIPS, this));
	m_wndToolBar.SetButtons(toolbar, 1);


	if (m_wndClassView.GetSafeHwnd() == 0)
	{
		if (!m_wndClassView.Create( WS_VISIBLE | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS | TVS_SHOWSELALWAYS,
			CRect(0,0,0,0), this, 0xFF ))
		{
			TRACE0( "Unable to create tree control.\n" );
			return 0;
		}
		m_wndClassView.ModifyStyleEx(0, WS_EX_STATICEDGE);

		if (!CreateImageList(m_ilClassView, IDB_CLASSTREE))
			return 0;

		// Set the image list for the tree control.
		m_wndClassView.SetImageList( &m_ilClassView, TVSIL_NORMAL );

		// Add the parent item
		HTREEITEM htItem = m_wndClassView.InsertItem(_T("XtremeDemo classes"));
		m_wndClassView.SetItemState( htItem, TVIS_BOLD, TVIS_BOLD );

		// Add children
		HTREEITEM hti = m_wndClassView.InsertItem(_T("CAboutDlg"), 3, 3, htItem);
		m_wndClassView.InsertItem(_T("CAboutDlg()"), 4, 4, hti);
		m_wndClassView.InsertItem(_T("DoDataExchange(CDataExchange *pDX)"), 5, 5, hti);

		hti = m_wndClassView.InsertItem(_T("CChildFrame"), 3, 3, htItem);
		m_wndClassView.InsertItem(_T("AssertValid()"), 4, 4, hti);
		m_wndClassView.InsertItem(_T("CChildFrame()"), 4, 4, hti);
		m_wndClassView.InsertItem(_T("~CChildFrame()"), 4, 4, hti);
		m_wndClassView.InsertItem(_T("Dump(CDumpContext &dc)"), 4, 4, hti);
		m_wndClassView.InsertItem(_T("PreCreateWindow(CREATESTRUCT& cs)"), 4, 4, hti);

		hti = m_wndClassView.InsertItem(_T("CMainFrame"), 3, 3, htItem);
		m_wndClassView.InsertItem(_T("AssertValid()"), 4, 4, hti);
		m_wndClassView.InsertItem(_T("CMainFrame()"), 4, 4, hti);
		m_wndClassView.InsertItem(_T("~CMainFrame()"), 4, 4, hti);
		m_wndClassView.InsertItem(_T("Dump(CDumpContext &dc)"), 4, 4, hti);
		m_wndClassView.InsertItem(_T("OnCreate(LPCREATESTRUCT lpCreateStruct)"), 5, 5, hti);
		m_wndClassView.InsertItem(_T("PreCreateWindow(CREATESTRUCT& cs)"), 4, 4, hti);
		m_wndClassView.InsertItem(_T("m_wndStatusBar"), 7, 7, hti);
		m_wndClassView.InsertItem(_T("m_wndToolBar"), 7, 7, hti);

		hti = m_wndClassView.InsertItem(_T("CXtremeDemoApp"), 3, 3, htItem);
		m_wndClassView.InsertItem(_T("CXtremeDemoApp()"), 4, 4, hti);
		m_wndClassView.InsertItem(_T("InitInstance()"), 4, 4, hti);
		m_wndClassView.InsertItem(_T("OnAppAbout()"), 4, 4, hti);

		hti = m_wndClassView.InsertItem(_T("CXtremeDemoDoc"), 3, 3, htItem);
		m_wndClassView.InsertItem(_T("AssertValid()"), 5, 5, hti);
		m_wndClassView.InsertItem(_T("CXtremeDemoDoc()"), 4, 4, hti);
		m_wndClassView.InsertItem(_T("~CXtremeDemoDoc()"), 4, 4, hti);
		m_wndClassView.InsertItem(_T("Dump(CDumpContext &dc)"), 4, 4, hti);
		m_wndClassView.InsertItem(_T("OnNewDocument()"), 4, 4, hti);
		m_wndClassView.InsertItem(_T("Serialize(CArchive& ar)"), 4, 4, hti);
		m_wndClassView.Expand(hti, TVE_EXPAND);

		hti = m_wndClassView.InsertItem(_T("CXtremeDemoView"), 3, 3, htItem);
		m_wndClassView.InsertItem(_T("AssertValid()"), 4, 4, hti);
		m_wndClassView.InsertItem(_T("CXtremeDemoView()"), 5, 5, hti);
		m_wndClassView.InsertItem(_T("~CXtremeDemoView()"), 4, 4, hti);
		m_wndClassView.InsertItem(_T("Dump(CDumpContext& dc)"), 4, 4, hti);
		m_wndClassView.InsertItem(_T("GetDocument()"), 4, 4, hti);
		m_wndClassView.InsertItem(_T("OnBeginPrinting(CDC* pDC, CPrintInfo *pInfo)"), 5, 5, hti);
		m_wndClassView.InsertItem(_T("OnDraw(CDC *pDC)"), 4, 4, hti);
		m_wndClassView.InsertItem(_T("OnEndPrinting(CDC *pDC, CPrintInfo *pInfo)"), 5, 5, hti);
		m_wndClassView.InsertItem(_T("OnPreparePrinting(CPrintInfo *pInfo)"), 5, 5, hti);
		m_wndClassView.InsertItem(_T("PreCreateWindow(CREATESTRUCT &cs)"), 4, 4, hti);

		hti = m_wndClassView.InsertItem(_T("Globals"), 1, 2, htItem);
		m_wndClassView.InsertItem(_T("theApp"), 6, 6, hti);
		m_wndClassView.Expand(hti, TVE_EXPAND);

		m_wndClassView.Expand(htItem, TVE_EXPAND);


	}

	return 0;
}

void CClassViewPane::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	int nTop = 0;

	if (m_wndToolBar.GetSafeHwnd())
	{
		CSize sz = m_wndToolBar.CalcDockingLayout(cx, /*LM_HIDEWRAP|*/ LM_HORZDOCK|LM_HORZ | LM_COMMIT);

		m_wndToolBar.MoveWindow(0, nTop, cx, sz.cy);
		m_wndToolBar.Invalidate(FALSE);
		nTop += sz.cy;
	}

	if (m_wndClassView.GetSafeHwnd())
	{
		m_wndClassView.MoveWindow(0, nTop, cx, cy - nTop);
		m_wndClassView.Invalidate(FALSE);
	}
}

void CClassViewPane::OnProjectAddexistingitem()
{
	// TODO: Add your command handler code here

}
