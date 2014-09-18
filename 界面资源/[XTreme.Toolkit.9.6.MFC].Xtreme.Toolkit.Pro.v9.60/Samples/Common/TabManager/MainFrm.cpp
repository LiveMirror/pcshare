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
#include "TabManager.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_WM_CLOSE()
	ON_COMMAND(ID_VIEW_SWITCH_THEME, OnSwitchTheme)
	ON_COMMAND(ID_VIEW_THEME_OFFICE2000, OnViewThemeOffice2000)
	ON_COMMAND(ID_VIEW_THEME_OFFICEXP, OnViewThemeOfficexp)
	ON_COMMAND(ID_VIEW_THEME_OFFICE2003, OnViewThemeOffice2003)
	ON_COMMAND(ID_VIEW_THEME_NATIVEXP, OnViewThemeNativexp)
	ON_COMMAND(ID_VIEW_THEME_WHIDBEY, OnViewThemeWhidbey)
	ON_UPDATE_COMMAND_UI(ID_VIEW_THEME_OFFICE2000, OnUpdateViewThemeOffice2000)
	ON_UPDATE_COMMAND_UI(ID_VIEW_THEME_OFFICEXP, OnUpdateViewThemeOfficexp)
	ON_UPDATE_COMMAND_UI(ID_VIEW_THEME_OFFICE2003, OnUpdateViewThemeOffice2003)
	ON_UPDATE_COMMAND_UI(ID_VIEW_THEME_NATIVEXP, OnUpdateViewThemeNativexp)
	ON_UPDATE_COMMAND_UI(ID_VIEW_THEME_WHIDBEY, OnUpdateViewThemeWhidbey)
	//}}AFX_MSG_MAP
	ON_COMMAND(XTP_ID_CUSTOMIZE, OnCustomize)
	ON_MESSAGE(XTPWM_DOCKINGPANE_NOTIFY, OnDockingPaneNotify)
	ON_XTP_CREATECONTROL()

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
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// Create Status bar.
	// Important: All control bars including the Status Bar
	// must be created before CommandBars....
	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	// Initialize the command bars
	if (!InitCommandBars())
		return -1;

	// Get a pointer to the command bars object.
	CXTPCommandBars* pCommandBars = GetCommandBars();
	if(pCommandBars == NULL)
	{
		TRACE0("Failed to create command bars object.\n");
		return -1;      // fail to create
	}

	// Add the menu bar
	CXTPCommandBar* pMenuBar = pCommandBars->SetMenu(
		_T("Menu Bar"), IDR_MAINFRAME);
	if(pMenuBar == NULL)
	{
		TRACE0("Failed to create menu bar.\n");
		return -1;      // fail to create
	}

	// Create ToolBar
	CXTPToolBar* pToolBar = (CXTPToolBar*)
		pCommandBars->Add(_T("Standard"), xtpBarTop);
	if (!pToolBar || !pToolBar->LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;
	}



	m_MTIClientWnd.Attach(this, TRUE);

	m_paneManager.InstallDockingPanes(this);
	m_paneManager.SetTheme(xtpPaneThemeDefault);

	// Create docking panes.
	CXTPDockingPane* pwndPane1 = m_paneManager.CreatePane(
		IDR_PANE1, CRect(0, 0,200, 120), xtpPaneDockLeft);
	CXTPDockingPane* pwndPane2 = m_paneManager.CreatePane(
		IDR_PANE2, CRect(0, 0,200, 120), xtpPaneDockTop, pwndPane1);
	CXTPDockingPane* pwndPane3 = m_paneManager.CreatePane(
		IDR_PANE3, CRect(0, 0,200, 120), xtpPaneDockLeft, pwndPane2);
	m_paneManager.AttachPane(pwndPane2, pwndPane1);
	m_paneManager.AttachPane(pwndPane3, pwndPane1);

	CXTPDockingPane* pwndPane4 = m_paneManager.CreatePane(
		IDR_PANE4, CRect(0, 0,200, 120), xtpPaneDockRight);
	CXTPDockingPane* pwndPane5 = m_paneManager.CreatePane(
		IDR_PANE5, CRect(0, 0,200, 120), xtpPaneDockBottom, pwndPane4);
	CXTPDockingPane* pwndPane6 = m_paneManager.CreatePane(
		IDR_PANE6, CRect(0, 0,200, 120), xtpPaneDockBottom, pwndPane5);
	pwndPane6->SetIconID(IDR_PANE1);

	m_paneManager.AttachPane(pwndPane5, pwndPane4);
	m_paneManager.AttachPane(pwndPane6, pwndPane4);
	m_paneManager.HidePane(pwndPane6);

	// Set the icons for the docking pane tabs.
	int nIDs1[] = {IDR_PANE1, IDR_PANE2, IDR_PANE3, IDR_PANE4, IDR_PANE5};
	m_paneManager.SetIcons(IDB_BITMAP1, nIDs1, 5, RGB(0, 255, 0));

	m_wndProperties.Create(IDD_DIALOG_PROPERTIES, this);
	m_wndProperties.CenterWindow(this);
	m_wndProperties.ShowWindow(SW_SHOW);

	// Load the previous state for command bars.
	LoadCommandBars(_T("CommandBars"));

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CMDIFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return TRUE;
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

LRESULT CMainFrame::OnDockingPaneNotify(WPARAM wParam, LPARAM lParam)
{
	if (wParam == XTP_DPN_SHOWWINDOW)
	{
		// get a pointer to the docking pane being shown.
		CXTPDockingPane* pPane = (CXTPDockingPane*)lParam;
		if (!pPane->IsValid())
		{
			int nIndex = pPane->GetID() - IDR_PANE1;
			ASSERT(nIndex >=0 &&  nIndex < sizeof(m_wndPanes) / sizeof(m_wndPanes[0]));

			// create and attach the edit control for this pane.
			if (!::IsWindow(m_wndPanes[nIndex].m_hWnd))
			{
				if (!m_wndPanes[nIndex].CreateEx(WS_EX_CLIENTEDGE, _T("EDIT"), _T(""),
					WS_CHILD|ES_AUTOVSCROLL|ES_MULTILINE, CRect(0, 0,200, 120), this, 0))
				{
					TRACE0( "Error creating pane edit control.\n" );
				}

				//AdjustStyle(GetDockingPaneManager()->GetCurrentTheme());
			}

			pPane->Attach(&m_wndPanes[nIndex]);
		}

		return TRUE; // handled
	}
	return FALSE;
}

void CMainFrame::OnThemeChanged()
{
	GetCommandBars()->RedrawCommandBars();
	m_paneManager.RedrawPanes();
	m_MTIClientWnd.UpdateContents();

	m_wndProperties.UpdateAll();
}

void CMainFrame::OnSwitchTheme()
{
	switch(XTPPaintManager()->GetCurrentTheme())
	{
	case xtpThemeOffice2000:
		OnViewThemeOfficexp();
		break;
	case xtpThemeOfficeXP:
		OnViewThemeOffice2003();
		break;
	case xtpThemeOffice2003:
		OnViewThemeNativexp();
		break;
	case xtpThemeNativeWinXP:
		OnViewThemeWhidbey();
		break;
	case xtpThemeWhidbey:
		OnViewThemeOffice2000();
		break;
	}

}

void CMainFrame::OnViewThemeOffice2000()
{
	CXTPPaintManager::SetTheme(xtpThemeOffice2000);
	m_paneManager.SetTheme(xtpPaneThemeDefault);
	OnThemeChanged();
}

void CMainFrame::OnViewThemeOfficexp()
{
	CXTPPaintManager::SetTheme(xtpThemeOfficeXP);
	m_paneManager.SetTheme(xtpPaneThemeOffice);
	OnThemeChanged();
}

void CMainFrame::OnViewThemeOffice2003()
{
	CXTPPaintManager::SetTheme(xtpThemeOffice2003);
	m_paneManager.SetTheme(xtpPaneThemeOffice2003);
	OnThemeChanged();
}

void CMainFrame::OnViewThemeNativexp()
{
	CXTPPaintManager::SetTheme(xtpThemeNativeWinXP);
	m_paneManager.SetTheme(xtpPaneThemeNativeWinXP);
	OnThemeChanged();
}

void CMainFrame::OnViewThemeWhidbey()
{
	CXTPPaintManager::SetTheme(xtpThemeWhidbey);
	m_paneManager.SetTheme(xtpPaneThemeWhidbey);
	OnThemeChanged();
}

void CMainFrame::OnUpdateViewThemeOffice2000(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(XTPPaintManager()->GetCurrentTheme() == xtpThemeOffice2000);
}

void CMainFrame::OnUpdateViewThemeOfficexp(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(XTPPaintManager()->GetCurrentTheme() == xtpThemeOfficeXP);
}

void CMainFrame::OnUpdateViewThemeOffice2003(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(XTPPaintManager()->GetCurrentTheme() == xtpThemeOffice2003);
}

void CMainFrame::OnUpdateViewThemeNativexp(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(XTPPaintManager()->GetCurrentTheme() == xtpThemeNativeWinXP);
}

void CMainFrame::OnUpdateViewThemeWhidbey(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(XTPPaintManager()->GetCurrentTheme() == xtpThemeWhidbey);
}


int CMainFrame::OnCreateControl(LPCREATECONTROLSTRUCT lpCreateControl)
{
	if (lpCreateControl->bToolBar)
	{
		CXTPToolBar* pToolBar = DYNAMIC_DOWNCAST(CXTPToolBar, lpCreateControl->pCommandBar);
		if (!pToolBar)
			return FALSE;

		if (lpCreateControl->nID == ID_VIEW_SWITCH_THEME)
		{
			lpCreateControl->controlType = xtpControlSplitButtonPopup;
			lpCreateControl->buttonStyle = xtpButtonIconAndCaption;
			return TRUE;
		}
	}
	return FALSE;
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
	pPageCommands->AddCategories(IDR_TABMANTYPE);

	// initialize the commands page page.
	pPageCommands->InsertAllCommandsCategory();
	pPageCommands->InsertBuiltInMenus(IDR_TABMANTYPE);
	pPageCommands->InsertNewMenuCategory();

	// display the customize dialog.
	dlg.DoModal();
}



void CMainFrame::OnSetPreviewMode(BOOL bPreview, CPrintPreviewState* pState)
{
// Toggle CommandBars
	GetCommandBars()->OnSetPreviewMode(bPreview);

// Toggle Tab Client
	m_MTIClientWnd.ShowWorkspace(!bPreview);

// Toggle Docking Panes.
	m_paneManager.OnSetPreviewMode(bPreview);


	CMDIFrameWnd::OnSetPreviewMode(bPreview, pState);
}
