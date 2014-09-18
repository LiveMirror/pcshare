// PopoutFrame.cpp : implementation file
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
#include "PopoutFrame.h"
#include "MainFrm.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPopoutFrame

IMPLEMENT_DYNCREATE(CPopoutFrame, CFrameWnd)

CPopoutFrame::CPopoutFrame()
{
}

CPopoutFrame::~CPopoutFrame()
{
}

BEGIN_MESSAGE_MAP(CPopoutFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CPopoutFrame)
	ON_WM_CREATE()
	ON_COMMAND(ID_POPFRAME_EXIT, OnPopframeExit)
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CPopoutFrame message handlers

int CPopoutFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_wndView.Create(NULL, NULL, AFX_WS_DEFAULT_VIEW,
		CRect(0, 0, 0, 0), this, AFX_IDW_PANE_FIRST, NULL))
	{
		TRACE0("Failed to create view window\n");
		return -1;
	}
	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	if (!InitCommandBars())
		return -1;

	CXTPCommandBars* pCommandBars = GetCommandBars();

	CXTPCommandBar* pMenuBar = pCommandBars->SetMenu(_T("Menu Bar"), IDR_POPOUTFRAME);
	pMenuBar->SetFlags(xtpFlagIgnoreSetMenuMessage);


	CXTPToolBar* pCommandBar = (CXTPToolBar*)pCommandBars->Add(_T("Popout"), xtpBarTop);
	if (!pCommandBar ||
		!pCommandBar->LoadToolBar(IDR_POPOUT_TOOLBAR))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;
	}

	CXTPControlComboBox* pComboFind = (CXTPControlComboBox*)pCommandBar->GetControls()->Add(xtpControlComboBox, 0);
	pComboFind->SetDropDownListStyle();

	pComboFind->AddString(_T("str1"));
	pComboFind->AddString(_T("str2"));
	pComboFind->AddString(_T("str1"));
	pComboFind->AddString(_T("str2"));
	pComboFind->AddString(_T("str1"));
	pComboFind->AddString(_T("str2"));
	pComboFind->AddString(_T("str1"));
	pComboFind->AddString(_T("str2"));

	pComboFind->AddString(_T("str1"));
	pComboFind->AddString(_T("str2"));
	pComboFind->AddString(_T("str1"));
	pComboFind->AddString(_T("str2"));
	pComboFind->AddString(_T("str1"));
	pComboFind->AddString(_T("str2"));
	pComboFind->AddString(_T("str1"));
	pComboFind->AddString(_T("str2"));

	pComboFind->SetWidth(150);
	pComboFind->SetCaption(_T("Find"));
	pComboFind->SetStyle(xtpComboLabel);


	m_paneManager.InstallDockingPanes(this);

	CXTPDockingPane* pwndPaneFrame = m_paneManager.CreatePane(
		8, CRect(0, 0,200, 120), xtpPaneDockRight);
	pwndPaneFrame->SetTitle(_T("Empty Pane"));

	m_paneManager.SetTheme(xtpPaneThemeOffice);

	// Resize
	CRect r;
	GetWindowRect(&r);
	int cx = r.Width()/4;
	int cy = r.Height()/4;
	r.DeflateRect(cx, cy);
	MoveWindow(&r);

	return 0;
}

BOOL CPopoutFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;

	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	// Helps to reduce screen flicker.
	cs.lpszClass = AfxRegisterWndClass(0, NULL, NULL,
		AfxGetApp()->LoadIcon(IDR_MAINFRAME));

	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;

	cs.style |= WS_CLIPCHILDREN|WS_CLIPSIBLINGS;

	return TRUE;
}

void CPopoutFrame::OnPopframeExit()
{
	SendMessage(WM_CLOSE);
}

void CPopoutFrame::OnClose()
{
	CFrameWnd::OnClose();

	CMainFrame* pFrameWnd = (CMainFrame*)AfxGetMainWnd();
	if (pFrameWnd != NULL)
		pFrameWnd->m_pNewFrameWnd = NULL;
}
