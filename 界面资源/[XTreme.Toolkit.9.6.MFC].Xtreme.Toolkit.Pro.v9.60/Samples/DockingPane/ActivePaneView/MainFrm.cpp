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
#include "ActivePaneView.h"

#include "MainFrm.h"
#include "ViewOne.h"
#include "ViewTwo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	ON_WM_CREATE()
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
#ifdef _XTP_INCLUDE_COMMANDBARS
	ON_COMMAND(XTP_ID_CUSTOMIZE, OnCustomize)
#endif//_XTP_INCLUDE_COMMANDBARS
	ON_MESSAGE(XTPWM_DOCKINGPANE_NOTIFY, OnDockingPaneNotify)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

static UINT uHideCmds[] =
{
	ID_FILE_PRINT,
	ID_FILE_PRINT_PREVIEW,
};
/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
: m_pViewOne(NULL)
, m_pViewTwo(NULL)
, m_pActivePane(NULL)
{
	// TODO: add member initialization code here

}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

#ifndef _XTP_INCLUDE_COMMANDBARS

	if (!m_wndToolBar.Create(this, TBSTYLE_FLAT|WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	// TODO: Delete these three lines if you don't want the toolbar to
	//  be dockable
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);

#else

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

	// Set Office 2003 Theme
	CXTPPaintManager::SetTheme(xtpThemeOfficeXP);

	// Hide array of commands
	pCommandBars->HideCommands(uHideCmds, _countof(uHideCmds));

	// Set "Always Show Full Menus" option to the FALSE
	pCommandBars->GetCommandBarsOptions()->bAlwaysShowFullMenus = FALSE;

	// Load the previous state for toolbars and menus.
	LoadCommandBars(_T("CommandBars"));

#endif//_XTP_INCLUDE_COMMANDBARS

	// Initialize the docking pane manager and set the
	// initial them for the docking panes.  Do this only after all
	// control bars objects have been created and docked.
	m_paneManager.InstallDockingPanes(this);
	// Set Office 2003 Theme
	m_paneManager.SetTheme(xtpPaneThemeOffice);

	// Create docking panes.
	CXTPDockingPane* pwndPane1 = m_paneManager.CreatePane(
		IDR_PANE_OPTIONS, CRect(0, 0,200, 120), xtpPaneDockLeft);
	CXTPDockingPane* pwndPane2 = m_paneManager.CreatePane(
		IDR_PANE_PROPERTIES, CRect(0, 0,200, 120), xtpPaneDockBottom, pwndPane1);
	m_paneManager.CreatePane(
		IDR_PANE_DIALOG, CRect(0, 0,200, 120), xtpPaneDockBottom, pwndPane2);

	// Set the icons for the docking pane tabs.
	int nIDIcons[] = {IDR_PANE_OPTIONS, IDR_PANE_PROPERTIES, IDR_PANE_DIALOG};
		m_paneManager.SetIcons(IDB_BITMAP_ICONS, nIDIcons,
		_countof(nIDIcons), RGB(0, 255, 0));

	// Load the previous state for docking panes.
	CXTPDockingPaneLayout layoutNormal(&m_paneManager);
	if (layoutNormal.Load(_T("NormalLayout")))
	{
		m_paneManager.SetLayout(&layoutNormal);
	}

	return 0;
}

void CMainFrame::OnClose()
{
#ifdef _XTP_INCLUDE_COMMANDBARS

	// Save the current state for toolbars and menus.
	SaveCommandBars(_T("CommandBars"));

#endif//_XTP_INCLUDE_COMMANDBARS

	// Save the current state for docking panes.
	CXTPDockingPaneLayout layoutNormal(&m_paneManager);
	m_paneManager.GetLayout(&layoutNormal);
	layoutNormal.Save(_T("NormalLayout"));

	CFrameWnd::OnClose();
}

#ifdef _XTP_INCLUDE_COMMANDBARS

void CMainFrame::OnCustomize()
{
	// Get a pointer to the command bars object.
	CXTPCommandBars* pCommandBars = GetCommandBars();
	if(pCommandBars != NULL)
	{
		// Instanciate the customize dialog object.
		CXTPCustomizeSheet dlg(pCommandBars);

		// Add the options page to the customize dialog.
		CXTPCustomizeOptionsPage pageOptions(&dlg);
		dlg.AddPage(&pageOptions);

		// Add the commands page to the customize dialog.
		CXTPCustomizeCommandsPage* pCommands = dlg.GetCommandsPage();
		pCommands->AddCategories(IDR_MAINFRAME);

		// Use the command bar manager to initialize the
		// customize dialog.
		pCommands->InsertAllCommandsCategory();
		pCommands->InsertBuiltInMenus(IDR_MAINFRAME);
		pCommands->InsertNewMenuCategory();

		// Dispaly the dialog.
		dlg.DoModal();
	}
}

#endif//_XTP_INCLUDE_COMMANDBARS

LRESULT CMainFrame::OnDockingPaneNotify(WPARAM wParam, LPARAM lParam)
{
	if (wParam == XTP_DPN_SHOWWINDOW)
	{
		CXTPDockingPane* pPane = (CXTPDockingPane*)lParam;

		if (!pPane->IsValid())
		{
			switch (pPane->GetID())
			{
			case IDR_PANE_DIALOG:
				if (!::IsWindow(m_dlgPane.m_hWnd))
				{
					m_dlgPane.Create(IDD_PANEDIALOG, this);
				}
				pPane->Attach(&m_dlgPane);
				break;

			case IDR_PANE_OPTIONS:
				{
					if (!m_pViewOne)
					{
						m_pViewOne = DYNAMIC_DOWNCAST(CViewOne,
							pPane->AttachView(this, RUNTIME_CLASS(CViewOne)));
					}
					else
					{
						pPane->Attach(m_pViewOne->GetParent());
					}
					break;
				}
			case IDR_PANE_PROPERTIES:
				{
					if (!m_pViewTwo)
					{
						m_pViewTwo = DYNAMIC_DOWNCAST(CViewTwo,
							pPane->AttachView(this, RUNTIME_CLASS(CViewTwo)));
					} 
					else
					{
						pPane->Attach(m_pViewTwo->GetParent());
					}
					break;
				}
			}
		}
		return TRUE;
	}
	return FALSE;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	cs.style |= WS_CLIPCHILDREN|WS_CLIPSIBLINGS;
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers

// The following code will ensure that messages are correctly routed to
// the docking pane child views.

CXTPDockingPane* CMainFrame::GetActivePane(BOOL bFindActive/*=TRUE*/)
{
	// if the pane manager has not been created yet return NULL.
	if (!::IsWindow(m_paneManager.m_hWnd))
		return NULL;

	// do not search for the active pane.
	if (bFindActive == FALSE)
		return m_pActivePane;

	// get the list of docking panes from the pane manager and
	// search for the active pane.
	CXTPDockingPaneInfoList& paneList = m_paneManager.GetPaneList();
	for (POSITION pos = paneList.GetHeadPosition(); pos;)
	{
		// the pane is active if the child view has focus.
		XTP_DOCKINGPANE_INFO& dpi = paneList.GetNext(pos);
		if (dpi.pPane != NULL && dpi.pPane->IsFocus())
		{
			// save a pointer for the active pane.
			m_pActivePane = dpi.pPane;
			return dpi.pPane;
		}
	}

	return NULL;
}

CWnd* CMainFrame::GetActivePaneView(BOOL bFindActive/*=TRUE*/)
{
	CXTPDockingPane* pActivePane = GetActivePane(bFindActive);
	if (pActivePane)
		return pActivePane->GetChild();
	return NULL;
}

BOOL CMainFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	// Use GetActivePane(TRUE) to search the pane manager's list for the pane
	// with a view that has input focus.  If one is found, give the view an
	// opportunity to handle the OnCmdMsg first before we pass it to the base
	// class.

	CWnd* pView = GetActivePaneView(TRUE);
	if (pView != NULL && pView->OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
	{
		return TRUE; // handled.
	}

	return CFrameWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

BOOL CMainFrame::OnCommand(WPARAM wParam, LPARAM lParam)
{
	// Since we are only handling a command we do not want to search for a new
	// active pane, but use the existing one.  Use GetActivePane(FALSE) to
	// retrieve the view for pane that is currently active.  If one is found,
	// give the view an opportunity to handle the OnCommand first before we
	// pass it to the base class.

	CWnd* pView = GetActivePaneView(FALSE);
	if (pView != NULL && AfxCallWndProc(
		pView, pView->m_hWnd, WM_COMMAND, wParam, lParam) != 0)
	{
		return TRUE; // handled.
	}

	return CFrameWnd::OnCommand(wParam, lParam);
}


void CMainFrame::OnSetPreviewMode(BOOL bPreview, CPrintPreviewState* pState)
{
#ifdef _XTP_INCLUDE_COMMANDBARS
// Toggle CommandBars
	GetCommandBars()->OnSetPreviewMode(bPreview);
#endif

// Toggle Docking Panes.
	m_paneManager.OnSetPreviewMode(bPreview);


	CFrameWnd::OnSetPreviewMode(bPreview, pState);
}
