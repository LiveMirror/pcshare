// MainFrm.cpp : implementation of the CMainFrame class
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
#include "StatusBar.h"

#include "MainFrm.h"
#include "StatusInfoDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define WM_ADDSTATUSITEMS   (WM_USER+1)

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_COMMAND(ID_ADDITEMS, OnAdditems)

	ON_UPDATE_COMMAND_UI_RANGE(ID_VIEW_THEME_OFFICEXP, ID_VIEW_THEME_OFFICE2003, OnUpdateViewTheme)
	ON_COMMAND_RANGE(ID_VIEW_THEME_OFFICEXP, ID_VIEW_THEME_OFFICE2003, OnViewTheme)

	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
	ON_COMMAND(XTP_ID_CUSTOMIZE, OnCustomize)

	ON_MESSAGE_VOID(WM_ADDSTATUSITEMS, OnAdditems)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	m_nTheme = AfxGetApp()->GetProfileInt(_T("Settings"), _T("nTheme"), xtpThemeOffice2003);
}

CMainFrame::~CMainFrame()
{
	AfxGetApp()->WriteProfileInt(_T("Settings"), _T("nTheme"), m_nTheme);
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

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

	pCommandBars->SetMenu(_T("Menu Bar"), IDR_MAINFRAME);



	CXTPToolBar* pCommandBar = (CXTPToolBar*)pCommandBars->Add(_T("Standard"), xtpBarTop);
	if (!pCommandBar ||
		!pCommandBar->LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;
	}

	pCommandBar->GetControls()->GetAt(8)->SetStyle(xtpButtonIconAndCaption);
	pCommandBar->GetControls()->CreateOriginalControls();


	// display the add status items dialog
	PostMessage(WM_ADDSTATUSITEMS);

	// Load the previous state for command bars.
	LoadCommandBars(_T("CommandBars"));

	SetTheme(m_nTheme);

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CMDIFrameWnd::PreCreateWindow(cs) )
		return FALSE;

	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	cs.lpszClass = AfxRegisterWndClass( 0, NULL, NULL,
		AfxGetApp()->LoadIcon(IDR_MAINFRAME));

	return TRUE;
}

void CMainFrame::ResizeToolbar()
{
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CMDIFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers

void CMainFrame::AddAnimation()
{
	int iIndex = m_wndStatusBar.GetPaneCount();

	if (!m_wndAnimCtrl.Create(WS_CHILD|WS_VISIBLE|ACS_CENTER,
		CRect(0,0,0,0), &m_wndStatusBar, 0))
	{
		TRACE0("Failed to create avi control.\n");
		return;
	}

	m_wndAnimCtrl.Open(IDR_HEARTBEAT);

	// add the indicator to the status bar.
	m_wndStatusBar.AddIndicator(ID_INDICATOR_ANIM, iIndex);

	// Initialize the pane info and add the control.
	int nIndex = m_wndStatusBar.CommandToIndex(ID_INDICATOR_ANIM);
	ASSERT (nIndex != -1);

	m_wndStatusBar.SetPaneWidth(nIndex, 14);
	m_wndStatusBar.SetPaneStyle(nIndex, m_wndStatusBar.GetPaneStyle(nIndex) | SBPS_POPOUT);
	m_wndStatusBar.AddControl(&m_wndAnimCtrl, ID_INDICATOR_ANIM, FALSE);
}

void CMainFrame::RemoveAnimation()
{
	m_wndStatusBar.RemoveIndicator(ID_INDICATOR_ANIM);
}

void CMainFrame::AddIcon()
{
	int iIndex = m_wndStatusBar.GetPaneCount();

	if (!m_wndIconPane.Create(NULL, &m_wndStatusBar))
	{
		TRACE0("Failed to create icon pane.\n");
		return;
	}

	// add the indicator to the status bar.
	m_wndStatusBar.AddIndicator(ID_INDICATOR_ICON, iIndex);

	// Initialize the pane info and add the control.
	int nIndex = m_wndStatusBar.CommandToIndex(ID_INDICATOR_ICON);
	ASSERT (nIndex != -1);

	m_wndIconPane.SetPaneIcon(IDR_STATUS_ICON);
	m_wndStatusBar.SetPaneWidth(nIndex, 32);
	m_wndStatusBar.AddControl(&m_wndIconPane, ID_INDICATOR_ICON, FALSE);
}

void CMainFrame::RemoveIcon()
{
	m_wndStatusBar.RemoveIndicator(ID_INDICATOR_ICON);
}

void CMainFrame::AddEdit()
{
	int iIndex = m_wndStatusBar.GetPaneCount();

	// Create the edit control and add it to the status bar
	if (!m_wndEditCtrl.CreateEx(WS_EX_STATICEDGE, _T("edit"), _T("Hello World"),
		WS_CHILD|WS_VISIBLE, CRect(0,0,0,0), &m_wndStatusBar, 0))
	{
		TRACE0("Failed to create edit control.\n");
		return;
	}

	m_wndEditCtrl.SetFont(m_wndStatusBar.GetFont());

	// add the indicator to the status bar.
	m_wndStatusBar.AddIndicator(ID_INDICATOR_EDIT, iIndex);

	// Initialize the pane info and add the control.
	int nIndex = m_wndStatusBar.CommandToIndex(ID_INDICATOR_EDIT);
	ASSERT (nIndex != -1);

	m_wndStatusBar.SetPaneWidth(nIndex, 75);
	m_wndStatusBar.SetPaneStyle(nIndex, m_wndStatusBar.GetPaneStyle(nIndex) | SBPS_NOBORDERS);
	m_wndStatusBar.AddControl(&m_wndEditCtrl, ID_INDICATOR_EDIT, FALSE);
}

void CMainFrame::RemoveEdit()
{
	m_wndStatusBar.RemoveIndicator(ID_INDICATOR_EDIT);
}

void CMainFrame::AddLogo()
{
	if (!m_wndLogoPane.Create(_T("Codejock Software"), &m_wndStatusBar))
	{
		TRACE0("Failed to create logo control.\n");
		return;
	}

	int nWidth = m_wndLogoPane.GetTextSize().cx;

	// add the indicator to the status bar.
	m_wndStatusBar.AddIndicator(ID_INDICATOR_LOGO, 0);

	// Initialize the pane info and add the control.
	int nIndex = m_wndStatusBar.CommandToIndex(ID_INDICATOR_LOGO);
	ASSERT (nIndex != -1);

	m_wndStatusBar.SetPaneWidth(nIndex, nWidth);
	m_wndStatusBar.SetPaneStyle(nIndex, m_wndStatusBar.GetPaneStyle(nIndex) | SBPS_POPOUT);
	m_wndStatusBar.AddControl(&m_wndLogoPane, ID_INDICATOR_LOGO, FALSE);
}

void CMainFrame::RemoveLogo()
{
	m_wndStatusBar.RemoveIndicator(ID_INDICATOR_LOGO);
}

void CMainFrame::AddProgress()
{
	int iIndex = m_wndStatusBar.GetPaneCount();

	// Create the edit control and add it to the status bar
	if (!m_wndProgCtrl.Create(WS_CHILD|WS_VISIBLE|PBS_SMOOTH,
		CRect(0,0,0,0), &m_wndStatusBar, 0))
	{
		TRACE0("Failed to create edit control.\n");
		return;
	}

	// add the indicator to the status bar.
	m_wndStatusBar.AddIndicator(ID_INDICATOR_PROG, iIndex);

	// Initialize the pane info and add the control.
	int nIndex = m_wndStatusBar.CommandToIndex(ID_INDICATOR_PROG);
	ASSERT (nIndex != -1);

	m_wndStatusBar.SetPaneWidth(nIndex, 150);
	m_wndStatusBar.SetPaneStyle(nIndex, m_wndStatusBar.GetPaneStyle(nIndex) | SBPS_NOBORDERS);
	m_wndStatusBar.AddControl(&m_wndProgCtrl, ID_INDICATOR_PROG, FALSE);

	// initialize progress control.
	m_wndProgCtrl.SetRange (0, 5000);
	m_wndProgCtrl.SetPos (0);
	m_wndProgCtrl.SetStep (1);
}

void CMainFrame::RemoveProgress()
{
	m_wndStatusBar.RemoveIndicator(ID_INDICATOR_PROG);
}

void CMainFrame::TestProgress()
{
	m_wndProgCtrl.SetPos (0);

	int i;
	for (i = 0; i <= 5000; i++)
	{
		m_wndProgCtrl.StepIt();
	}
}

void CMainFrame::OnAdditems()
{
	static CStatusInfoDlg dlg(
		m_wndEditCtrl,
		m_wndAnimCtrl,
		m_wndProgCtrl,
		m_wndLogoPane,
		m_wndIconPane,
		this);

	dlg.DoModal();
}

void CMainFrame::SetTheme(int nTheme)
{
	m_nTheme = nTheme;

	XTPPaintManager()->SetTheme((XTPPaintTheme)nTheme);

	GetCommandBars()->RedrawCommandBars();

	RedrawWindow( NULL, NULL,
		RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE | RDW_ALLCHILDREN );
}

void CMainFrame::OnViewTheme(UINT nID)
{
	SetTheme(nID - ID_VIEW_THEME_OFFICEXP);
}

void CMainFrame::OnUpdateViewTheme(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_nTheme == (int)pCmdUI->m_nID - ID_VIEW_THEME_OFFICEXP? TRUE: FALSE);
}

void CMainFrame::OnClose()
{
	// Save the current state for command bars.
	SaveCommandBars(_T("CommandBars"));
	// TODO: Add your message handler code here and/or call default

	CMDIFrameWnd::OnClose();
}

void CMainFrame::OnCustomize()
{
	// get a pointer to the command bars object.
	CXTPCommandBars* pCommandBars = GetCommandBars();
	if (pCommandBars == NULL)
		return;

	// instanciate the customize dialog
	CXTPCustomizeSheet dlg(pCommandBars);

	// add the options page to the customize dialog.
	CXTPCustomizeOptionsPage pageOptions(&dlg);
	dlg.AddPage(&pageOptions);

	// add the commands page to the customize dialog.
	CXTPCustomizeCommandsPage* pPageCommands = dlg.GetCommandsPage();
	pPageCommands->AddCategories(IDR_STATUSTYPE);

	// initialize the commands page page.
	pPageCommands->InsertAllCommandsCategory();
	pPageCommands->InsertBuiltInMenus(IDR_STATUSTYPE);
	pPageCommands->InsertNewMenuCategory();

	// display the customize dialog.
	dlg.DoModal();
}
