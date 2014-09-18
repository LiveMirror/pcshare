// ListFrame.cpp : implementation file
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
#include "PaneOptions.h"
#include "ListFrame.h"

#include "MainFrm.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CListFrame

IMPLEMENT_DYNCREATE(CListFrame, CFrameWnd)

CListFrame::CListFrame()
{
}

CListFrame::~CListFrame()
{
}


BEGIN_MESSAGE_MAP(CListFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CListFrame)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CListFrame message handlers

BOOL CListFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	CFrameWnd::PreCreateWindow(cs);
	cs.dwExStyle = 0;
	cs.style |= WS_CLIPCHILDREN|WS_CLIPSIBLINGS;
	return TRUE;
}

int CListFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// create a view to occupy the client area of the frame
	if (!m_wndListCtrl.Create(WS_CLIPCHILDREN|WS_CLIPSIBLINGS|WS_VISIBLE|LVS_REPORT   , CRect(0, 0, 0, 0), this, AFX_IDW_PANE_FIRST))
	{
		TRACE0("Failed to create view window\n");
		return -1;
	}
	m_wndListCtrl.InsertColumn(0, _T("Name"), LVCFMT_LEFT, 150);
	m_wndListCtrl.InsertColumn(1, _T("Type"), LVCFMT_LEFT, 110);
	m_wndListCtrl.InsertColumn(2, _T("Data"), LVCFMT_LEFT, 200);



	m_paneManager.InstallDockingPanes(this);
	m_paneManager.SetTheme(xtpPaneThemeOffice);

	CMainFrame::m_arrManagers.Add(&m_paneManager);

	// Create docking panes.
	CXTPDockingPane* pwndPane1 = m_paneManager.CreatePane(
		1, CRect(0, 0,200, 120), xtpPaneDockRight);



	pwndPane1->SetTitle(_T("List Options"));
	pwndPane1->Hide();
	pwndPane1->SetOptions(xtpPaneNoCloseable);

	const int  nIDs[] = {1};
	m_paneManager.SetIcons(IDB_PANE_ICONS, nIDs, 1);

	return 0;
}
