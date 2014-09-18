// TreeFrame.cpp : implementation file
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
#include "TreeFrame.h"
#include "PaneOptionsDoc.h"
#include "PaneOptionsView.h"
#include "MainFrm.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTreeFrame

IMPLEMENT_DYNCREATE(CTreeFrame, CFrameWnd)

CTreeFrame::CTreeFrame()
{
}

CTreeFrame::~CTreeFrame()
{
}


BEGIN_MESSAGE_MAP(CTreeFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CTreeFrame)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTreeFrame message handlers





BOOL CTreeFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	CFrameWnd::PreCreateWindow(cs);
	cs.dwExStyle = 0;
	cs.style |= WS_CLIPCHILDREN|WS_CLIPSIBLINGS;
	return TRUE;
}

int CTreeFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

		// create a view to occupy the client area of the frame
	if (!m_wndTreeCtrl.Create(WS_CLIPCHILDREN|WS_CLIPSIBLINGS|WS_VISIBLE|TVS_HASLINES|TVS_LINESATROOT|TVS_HASBUTTONS|TVS_SHOWSELALWAYS, CRect(0, 0, 0, 0), this, AFX_IDW_PANE_FIRST))
	{
		TRACE0("Failed to create view window\n");
		return -1;
	}

	m_ilTreeIcons.Create(16, 16, ILC_MASK|ILC_COLOR24, 0, 1);

	CBitmap bmp;
	bmp.LoadBitmap(IDB_FOLDERS);

	m_ilTreeIcons.Add(&bmp, RGB(0, 128, 128));
	m_wndTreeCtrl.SetImageList(&m_ilTreeIcons, TVSIL_NORMAL);


	HTREEITEM hItem = m_wndTreeCtrl.InsertItem(_T("[Target Computer]"), 0, 1);
	m_wndTreeCtrl.InsertItem(_T("HKEY_CLASSES_ROOT"), 0, 1, hItem);
	m_wndTreeCtrl.InsertItem(_T("HKEY_CURRENT_USER"), 0, 1, hItem);
	m_wndTreeCtrl.InsertItem(_T("HKEY_LOCAL_MACHINE"), 0, 1, hItem);
	m_wndTreeCtrl.InsertItem(_T("HKEY_USERS"), 0, 1, hItem);

	m_wndTreeCtrl.Expand (hItem, TVE_EXPAND);


	m_paneManager.InstallDockingPanes(this);
	m_paneManager.SetTheme(xtpPaneThemeOffice);


	CMainFrame::m_arrManagers.Add(&m_paneManager);

	// Create docking panes.
	CXTPDockingPane* pwndPane1 = m_paneManager.CreatePane(
		1, CRect(0, 0,200, 120), xtpPaneDockLeft);

	pwndPane1->SetTitle(_T("Tree Options"));
	pwndPane1->Hide();
	pwndPane1->SetOptions(xtpPaneNoHideable | xtpPaneNoCloseable);

	const int  nIDs[] = {1};
	m_paneManager.SetIcons(IDB_PANE_ICONS, nIDs, 1);



	return 0;
}
