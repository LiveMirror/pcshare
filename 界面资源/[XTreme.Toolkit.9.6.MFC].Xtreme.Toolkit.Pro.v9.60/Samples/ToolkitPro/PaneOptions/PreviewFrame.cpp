// PreviewFrame.cpp : implementation file
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
#include "PreviewFrame.h"

#include "TreeFrame.h"
#include "ListFrame.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPreviewFrame

IMPLEMENT_DYNCREATE(CPreviewFrame, CFrameWnd)

CPreviewFrame::CPreviewFrame()
{
}

CPreviewFrame::~CPreviewFrame()
{
}


BEGIN_MESSAGE_MAP(CPreviewFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CPreviewFrame)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
	ON_MESSAGE(XTPWM_DOCKINGPANE_NOTIFY, OnDockingPaneNotify)

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPreviewFrame message handlers

BOOL CPreviewFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	CFrameWnd::PreCreateWindow(cs);
	cs.dwExStyle = 0;
	cs.style |= WS_CLIPCHILDREN|WS_CLIPSIBLINGS;
	return TRUE;
}

int CPreviewFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_paneManager.InstallDockingPanes(this);
	m_paneManager.SetTheme(xtpPaneThemeOffice);

	CMainFrame::m_arrManagers.Add(&m_paneManager);

	// Create docking panes.
	CXTPDockingPane* pwndPane1 = m_paneManager.CreatePane(
		1, CRect(0, 0,200, 160), xtpPaneDockBottom);

	pwndPane1->SetTitle(_T("Options"));
	pwndPane1->SetOptions(xtpPaneNoCloseable);

	const int  nIDs[] = {1};
	m_paneManager.SetIcons(IDB_PANE_ICONS, nIDs, 1);


	return 0;
}

BOOL CPreviewFrame::OnCreateClient(LPCREATESTRUCT /*lpcs*/, CCreateContext* /*pContext*/)
{
	m_wndSplitter.CreateStatic(this, 1, 2);
	m_wndSplitter.ModifyStyle(0, WS_CLIPSIBLINGS|WS_CLIPCHILDREN);
	m_wndSplitter.SetFullDrag(TRUE);


	CFrameWnd* pFrame = (CFrameWnd*)CTreeFrame::CreateObject();
	pFrame->CreateEx(NULL, NULL, NULL, WS_CHILD|WS_VISIBLE, CRect(0, 0, 0, 0), &m_wndSplitter, m_wndSplitter.IdFromRowCol(0, 0));


	pFrame = (CFrameWnd*)CListFrame::CreateObject();
	pFrame->CreateEx(NULL, NULL, NULL, WS_CHILD|WS_VISIBLE, CRect(0, 0, 0, 0), &m_wndSplitter, m_wndSplitter.IdFromRowCol(0, 1));

	m_wndSplitter.SetColumnInfo(0, 250, 0);
	return TRUE;
}



LRESULT CPreviewFrame::OnDockingPaneNotify(WPARAM wParam, LPARAM lParam)
{
	if (wParam == XTP_DPN_SHOWWINDOW)
	{
		// get a pointer to the docking pane being shown.
		CXTPDockingPane* pPane = (CXTPDockingPane*)lParam;
		if (!pPane->IsValid())
		{
			// create and attach the edit control for this pane.
			if (!::IsWindow(m_wndPane))
			{
				if (!m_wndPane.CreateEx(WS_EX_STATICEDGE, _T("EDIT"), _T(""),
					WS_CHILD|ES_AUTOVSCROLL|ES_MULTILINE, CRect(0, 0,200, 120), this, 0))
				{
					TRACE0( "Error creating pane edit control.\n" );
				}

			}

			pPane->Attach(&m_wndPane);
		}

		return TRUE; // handled
	}

	return FALSE;
}
