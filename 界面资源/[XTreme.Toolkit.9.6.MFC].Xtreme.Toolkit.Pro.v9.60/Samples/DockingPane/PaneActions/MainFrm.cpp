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
#include "PaneActions.h"

#include "MainFrm.h"

#include "PaneActionsDoc.h"
#include "PaneActionsView.h"

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
	ON_WM_CREATE()

	ON_COMMAND_RANGE(ID_ACTIONS_CLASSVIEW_CANCEL_ACTIONFLOATING, ID_ACTIONS_CLASSVIEW_CANCEL_ACTIONPINNING, OnActionsClassviewCancel)
	ON_UPDATE_COMMAND_UI_RANGE(ID_ACTIONS_CLASSVIEW_CANCEL_ACTIONFLOATING, ID_ACTIONS_CLASSVIEW_CANCEL_ACTIONPINNING, OnUpdateActionsClassviewCancel)
	ON_COMMAND_RANGE(ID_ACTIONS_CLASSVIEW_FLAGS_NOCLOSEABLE, ID_ACTIONS_CLASSVIEW_FLAGS_NOFLOATABLE, OnActionsClassviewFlags)
	ON_UPDATE_COMMAND_UI_RANGE(ID_ACTIONS_CLASSVIEW_FLAGS_NOCLOSEABLE, ID_ACTIONS_CLASSVIEW_FLAGS_NOFLOATABLE, OnUpdateActionsClassviewFlags)


	ON_COMMAND_RANGE(ID_ACTIONS_RESOURCEVIEW_CANCEL_ACTIONFLOATING, ID_ACTIONS_RESOURCEVIEW_CANCEL_ACTIONPINNING, OnActionsResourceviewCancel)
	ON_UPDATE_COMMAND_UI_RANGE(ID_ACTIONS_RESOURCEVIEW_CANCEL_ACTIONFLOATING, ID_ACTIONS_RESOURCEVIEW_CANCEL_ACTIONPINNING, OnUpdateActionsResourceviewCancel)
	ON_COMMAND_RANGE(ID_ACTIONS_RESOURCEVIEW_FLAGS_NOCLOSEABLE, ID_ACTIONS_RESOURCEVIEW_FLAGS_NOFLOATABLE, OnActionsResourceviewFlags)
	ON_UPDATE_COMMAND_UI_RANGE(ID_ACTIONS_RESOURCEVIEW_FLAGS_NOCLOSEABLE, ID_ACTIONS_RESOURCEVIEW_FLAGS_NOFLOATABLE, OnUpdateActionsResourceviewFlags)

	ON_COMMAND_RANGE(ID_ACTIONS_FILEVIEW_CANCEL_ACTIONFLOATING, ID_ACTIONS_FILEVIEW_CANCEL_ACTIONPINNING, OnActionsFileviewCancel)
	ON_UPDATE_COMMAND_UI_RANGE(ID_ACTIONS_FILEVIEW_CANCEL_ACTIONFLOATING, ID_ACTIONS_FILEVIEW_CANCEL_ACTIONPINNING, OnUpdateActionsFileviewCancel)
	ON_COMMAND_RANGE(ID_ACTIONS_FILEVIEW_FLAGS_NOCLOSEABLE, ID_ACTIONS_FILEVIEW_FLAGS_NOFLOATABLE, OnActionsFileviewFlags)
	ON_UPDATE_COMMAND_UI_RANGE(ID_ACTIONS_FILEVIEW_FLAGS_NOCLOSEABLE, ID_ACTIONS_FILEVIEW_FLAGS_NOFLOATABLE, OnUpdateActionsFileviewFlags)

	ON_COMMAND_RANGE(ID_ACTIONS_INFOVIEW_CANCEL_ACTIONFLOATING, ID_ACTIONS_INFOVIEW_CANCEL_ACTIONPINNING, OnActionsInfoviewCancel)
	ON_UPDATE_COMMAND_UI_RANGE(ID_ACTIONS_INFOVIEW_CANCEL_ACTIONFLOATING, ID_ACTIONS_INFOVIEW_CANCEL_ACTIONPINNING, OnUpdateActionsInfoviewCancel)
	ON_COMMAND_RANGE(ID_ACTIONS_INFOVIEW_FLAGS_NOCLOSEABLE, ID_ACTIONS_INFOVIEW_FLAGS_NOFLOATABLE, OnActionsInfoviewFlags)
	ON_UPDATE_COMMAND_UI_RANGE(ID_ACTIONS_INFOVIEW_FLAGS_NOCLOSEABLE, ID_ACTIONS_INFOVIEW_FLAGS_NOFLOATABLE, OnUpdateActionsInfoviewFlags)

	ON_COMMAND_RANGE(ID_ACTIONS_ALL_CANCEL_ACTIONFLOATING, ID_ACTIONS_ALL_CANCEL_ACTIONPINNING, OnActionsAllCancel)
	ON_UPDATE_COMMAND_UI_RANGE(ID_ACTIONS_ALL_CANCEL_ACTIONFLOATING, ID_ACTIONS_ALL_CANCEL_ACTIONPINNING, OnUpdateActionsAllCancel)
	ON_COMMAND_RANGE(ID_ACTIONS_ALL_FLAGS_NOCLOSEABLE, ID_ACTIONS_ALL_FLAGS_NOFLOATABLE, OnActionsAllFlags)
	ON_UPDATE_COMMAND_UI_RANGE(ID_ACTIONS_ALL_FLAGS_NOCLOSEABLE, ID_ACTIONS_ALL_FLAGS_NOFLOATABLE, OnUpdateActionsAllFlags)

	//}}AFX_MSG_MAP
	ON_MESSAGE(XTPWM_DOCKINGPANE_NOTIFY, OnDockingPaneNotify)
#ifdef _XTP_INCLUDE_COMMANDBARS
	ON_COMMAND(XTP_ID_CUSTOMIZE, OnCustomize)
	ON_WM_CLOSE()
#endif

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
	ZeroMemory(&m_bCancelAction, sizeof(m_bCancelAction));

}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

#ifndef _XTP_INCLUDE_COMMANDBARS

	// create and initialize all control bars prior to
	// creating docking panes...

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

	// TODO: Delete these three lines if you don't want the toolbar to be dockable
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);

	XTPColorManager()->DisableLunaColors();

#else

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	if (!InitCommandBars())
		return -1;

	XTPPaintManager()->SetTheme(xtpThemeOfficeXP);

	CXTPCommandBars* pCommandBars = GetCommandBars();

	CXTPCommandBar* pMenuBar = pCommandBars->SetMenu(_T("Menu Bar"), IDR_MAINFRAME);
	pMenuBar->SetFlags(xtpFlagAddMDISysPopup);


	CXTPToolBar* pCommandBar = (CXTPToolBar*)pCommandBars->Add(_T("Standard"), xtpBarTop);
	if (!pCommandBar ||
		!pCommandBar->LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;
	}

	// Load the previous state for toolbars and menus.
	LoadCommandBars(_T("CommandBars"));

#endif
	m_paneManager.InstallDockingPanes(this);
	m_paneManager.SetTheme(xtpPaneThemeGrippered);

	// Create docking panes.
	CXTPDockingPane* pPaneClassView = m_paneManager.CreatePane(
		IDR_PANE_CLASSVIEW, CRect(0, 0,250, 120), xtpPaneDockLeft);

	CXTPDockingPane* pPaneResourceView = m_paneManager.CreatePane(
		IDR_PANE_RESOURCEVIEW, CRect(0, 0,200, 120), xtpPaneDockBottom, pPaneClassView);

	CXTPDockingPane* pPaneFileView = m_paneManager.CreatePane(
		IDR_PANE_FILEVIEW, CRect(0, 0,200, 120), xtpPaneDockBottom, pPaneClassView);

	CXTPDockingPane* pPaneInfoView = m_paneManager.CreatePane(
		IDR_PANE_INFOVIEW, CRect(0, 0,200, 120), xtpPaneDockBottom, pPaneClassView);

	m_paneManager.AttachPane(pPaneResourceView, pPaneClassView);
	m_paneManager.AttachPane(pPaneFileView, pPaneClassView);
	m_paneManager.AttachPane(pPaneInfoView, pPaneClassView);

	pPaneClassView->Select();


	// Set the icons for the docking pane tabs.
	int nIDs[] = {IDR_PANE_CLASSVIEW, IDR_PANE_RESOURCEVIEW, IDR_PANE_FILEVIEW, IDR_PANE_INFOVIEW};
	m_paneManager.SetIcons(IDB_PANEICONS, nIDs, 4, RGB(128, 0, 128));

	m_paneManager.GetPaintManager()->GetTabPaintManager()->m_clientFrame = xtpTabFrameBorder;
	m_paneManager.GetPaintManager()->GetTabPaintManager()->m_rcControlMargin.SetRect(3, 3, 3, 3);



	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	cs.style |= WS_CLIPCHILDREN | WS_CLIPSIBLINGS;

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



LRESULT CMainFrame::OnDockingPaneNotify(WPARAM wParam, LPARAM lParam)
{
	if (wParam == XTP_DPN_SHOWWINDOW)
	{
		// get a pointer to the docking pane being shown.
		CXTPDockingPane* pPane = (CXTPDockingPane*)lParam;
		if (!pPane->IsValid())
		{
			int nIndex = pPane->GetID() - IDR_PANE_CLASSVIEW;
			ASSERT(nIndex >=0 &&  nIndex < sizeof(m_wndPanes) / sizeof(m_wndPanes[0]));

			// create and attach the edit control for this pane.
			if (!::IsWindow(m_wndPanes[nIndex].m_hWnd))
			{
				if (!m_wndPanes[nIndex].CreateEx(WS_EX_CLIENTEDGE, _T("EDIT"), _T(""),
					WS_CHILD|ES_AUTOVSCROLL|ES_MULTILINE, CRect(0, 0,200, 120), this, 0))
				{
					TRACE0( "Error creating pane edit control.\n" );
				}
			}

			pPane->Attach(&m_wndPanes[nIndex]);
		}

		return TRUE;
	}

	if (wParam == XTP_DPN_ACTION)
	{
		XTP_DOCKINGPANE_ACTION* pAction = (XTP_DOCKINGPANE_ACTION*)lParam;


		int nIndex = pAction->pPane->GetID() - IDR_PANE_CLASSVIEW;

		ASSERT(pAction->action < _countof(lpszActions));

		if (m_bCancelAction[nIndex][pAction->action])
		{
			pAction->bCancel = TRUE;
		}

		/*
		if (pAction->action == xtpPaneActionDocking)
		{
			if (pAction->pDockContainer->GetType() != xtpPaneTypeTabbedContainer && (pAction->dockDirection != xtpPaneDockLeft && pAction->dockDirection != xtpPaneDockRight))
			{
				pAction->bCancel = TRUE;
			}

		}*/

		((CPaneActionsView*)GetActiveView())->AddAction(pAction);

		return TRUE;
	}

	return FALSE;
}

//////////////////////////////////////////////////////////////////////////

void CMainFrame::OnActionsClassviewCancel(UINT nID)
{
	int nIndex = nID - ID_ACTIONS_CLASSVIEW_CANCEL_ACTIONFLOATING;
	m_bCancelAction[0][nIndex] = !m_bCancelAction[0][nIndex];

}

void CMainFrame::OnUpdateActionsClassviewCancel(CCmdUI* pCmdUI)
{
	int nIndex = pCmdUI->m_nID - ID_ACTIONS_CLASSVIEW_CANCEL_ACTIONFLOATING;
	pCmdUI->SetCheck(m_bCancelAction[0][nIndex]? TRUE: FALSE);

}

void CMainFrame::OnActionsClassviewFlags(UINT nID)
{
	int nFlag = nID == ID_ACTIONS_CLASSVIEW_FLAGS_NOCLOSEABLE? xtpPaneNoCloseable:
		nID == ID_ACTIONS_CLASSVIEW_FLAGS_NOHIDEABLE? xtpPaneNoHideable: xtpPaneNoFloatable;

	CXTPDockingPane* pPane = m_paneManager.FindPane(IDR_PANE_CLASSVIEW);

	DWORD dwOptions = pPane->GetOptions();
	if (dwOptions & nFlag) dwOptions &= ~ dwOptions; else dwOptions |= nFlag;
	pPane->SetOptions(dwOptions);


	m_paneManager.RedrawPanes();
}

void CMainFrame::OnUpdateActionsClassviewFlags(CCmdUI* pCmdUI)
{
	UINT nID = pCmdUI->m_nID;
	int nFlag = nID == ID_ACTIONS_CLASSVIEW_FLAGS_NOCLOSEABLE? xtpPaneNoCloseable:
		nID == ID_ACTIONS_CLASSVIEW_FLAGS_NOHIDEABLE? xtpPaneNoHideable: xtpPaneNoFloatable;

	pCmdUI->SetCheck(m_paneManager.FindPane(IDR_PANE_CLASSVIEW)->GetOptions() & nFlag? TRUE: FALSE);

}


//////////////////////////////////////////////////////////////////////////

void CMainFrame::OnActionsResourceviewCancel(UINT nID)
{
	int nIndex = nID - ID_ACTIONS_RESOURCEVIEW_CANCEL_ACTIONFLOATING;
	m_bCancelAction[1][nIndex] = !m_bCancelAction[1][nIndex];

}

void CMainFrame::OnUpdateActionsResourceviewCancel(CCmdUI* pCmdUI)
{
	int nIndex = pCmdUI->m_nID - ID_ACTIONS_RESOURCEVIEW_CANCEL_ACTIONFLOATING;
	pCmdUI->SetCheck(m_bCancelAction[1][nIndex]? TRUE: FALSE);

}

void CMainFrame::OnActionsResourceviewFlags(UINT nID)
{
	int nFlag = nID == ID_ACTIONS_RESOURCEVIEW_FLAGS_NOCLOSEABLE? xtpPaneNoCloseable:
		nID == ID_ACTIONS_RESOURCEVIEW_FLAGS_NOHIDEABLE? xtpPaneNoHideable: xtpPaneNoFloatable;

	CXTPDockingPane* pPane = m_paneManager.FindPane(IDR_PANE_RESOURCEVIEW);

	DWORD dwOptions = pPane->GetOptions();
	if (dwOptions & nFlag) dwOptions &= ~ dwOptions; else dwOptions |= nFlag;
	pPane->SetOptions(dwOptions);


	m_paneManager.RedrawPanes();
}

void CMainFrame::OnUpdateActionsResourceviewFlags(CCmdUI* pCmdUI)
{
	UINT nID = pCmdUI->m_nID;
	int nFlag = nID == ID_ACTIONS_RESOURCEVIEW_FLAGS_NOCLOSEABLE? xtpPaneNoCloseable:
		nID == ID_ACTIONS_RESOURCEVIEW_FLAGS_NOHIDEABLE? xtpPaneNoHideable: xtpPaneNoFloatable;

	pCmdUI->SetCheck(m_paneManager.FindPane(IDR_PANE_RESOURCEVIEW)->GetOptions() & nFlag? TRUE: FALSE);

}


//////////////////////////////////////////////////////////////////////////

void CMainFrame::OnActionsFileviewCancel(UINT nID)
{
	int nIndex = nID - ID_ACTIONS_FILEVIEW_CANCEL_ACTIONFLOATING;
	m_bCancelAction[2][nIndex] = !m_bCancelAction[2][nIndex];

}

void CMainFrame::OnUpdateActionsFileviewCancel(CCmdUI* pCmdUI)
{
	int nIndex = pCmdUI->m_nID - ID_ACTIONS_FILEVIEW_CANCEL_ACTIONFLOATING;
	pCmdUI->SetCheck(m_bCancelAction[2][nIndex]? TRUE: FALSE);

}

void CMainFrame::OnActionsFileviewFlags(UINT nID)
{
	int nFlag = nID == ID_ACTIONS_FILEVIEW_FLAGS_NOCLOSEABLE? xtpPaneNoCloseable:
		nID == ID_ACTIONS_FILEVIEW_FLAGS_NOHIDEABLE? xtpPaneNoHideable: xtpPaneNoFloatable;

	CXTPDockingPane* pPane = m_paneManager.FindPane(IDR_PANE_FILEVIEW);

	DWORD dwOptions = pPane->GetOptions();
	if (dwOptions & nFlag) dwOptions &= ~ dwOptions; else dwOptions |= nFlag;
	pPane->SetOptions(dwOptions);


	m_paneManager.RedrawPanes();
}

void CMainFrame::OnUpdateActionsFileviewFlags(CCmdUI* pCmdUI)
{
	UINT nID = pCmdUI->m_nID;
	int nFlag = nID == ID_ACTIONS_FILEVIEW_FLAGS_NOCLOSEABLE? xtpPaneNoCloseable:
		nID == ID_ACTIONS_FILEVIEW_FLAGS_NOHIDEABLE? xtpPaneNoHideable: xtpPaneNoFloatable;

	pCmdUI->SetCheck(m_paneManager.FindPane(IDR_PANE_FILEVIEW)->GetOptions() & nFlag? TRUE: FALSE);

}

//////////////////////////////////////////////////////////////////////////

void CMainFrame::OnActionsInfoviewCancel(UINT nID)
{
	int nIndex = nID - ID_ACTIONS_INFOVIEW_CANCEL_ACTIONFLOATING;
	m_bCancelAction[3][nIndex] = !m_bCancelAction[3][nIndex];

}

void CMainFrame::OnUpdateActionsInfoviewCancel(CCmdUI* pCmdUI)
{
	int nIndex = pCmdUI->m_nID - ID_ACTIONS_INFOVIEW_CANCEL_ACTIONFLOATING;
	pCmdUI->SetCheck(m_bCancelAction[3][nIndex]? TRUE: FALSE);

}

void CMainFrame::OnActionsInfoviewFlags(UINT nID)
{
	int nFlag = nID == ID_ACTIONS_INFOVIEW_FLAGS_NOCLOSEABLE? xtpPaneNoCloseable:
		nID == ID_ACTIONS_INFOVIEW_FLAGS_NOHIDEABLE? xtpPaneNoHideable: xtpPaneNoFloatable;

	CXTPDockingPane* pPane = m_paneManager.FindPane(IDR_PANE_INFOVIEW);

	DWORD dwOptions = pPane->GetOptions();
	if (dwOptions & nFlag) dwOptions &= ~ nFlag; else dwOptions |= nFlag;
	pPane->SetOptions(dwOptions);


	m_paneManager.RedrawPanes();
}

void CMainFrame::OnUpdateActionsInfoviewFlags(CCmdUI* pCmdUI)
{
	UINT nID = pCmdUI->m_nID;
	int nFlag = nID == ID_ACTIONS_INFOVIEW_FLAGS_NOCLOSEABLE? xtpPaneNoCloseable:
		nID == ID_ACTIONS_INFOVIEW_FLAGS_NOHIDEABLE? xtpPaneNoHideable: xtpPaneNoFloatable;

	pCmdUI->SetCheck(m_paneManager.FindPane(IDR_PANE_INFOVIEW)->GetOptions() & nFlag? TRUE: FALSE);

}


//////////////////////////////////////////////////////////////////////////

void CMainFrame::OnActionsAllCancel(UINT nID)
{
	int nIndex = nID - ID_ACTIONS_ALL_CANCEL_ACTIONFLOATING;

	BOOL bCancel = m_bCancelAction[0][nIndex];
	m_bCancelAction[0][nIndex] = !bCancel;
	m_bCancelAction[1][nIndex] = !bCancel;
	m_bCancelAction[2][nIndex] = !bCancel;
	m_bCancelAction[3][nIndex] = !bCancel;

}

void CMainFrame::OnUpdateActionsAllCancel(CCmdUI* pCmdUI)
{
	int nIndex = pCmdUI->m_nID - ID_ACTIONS_ALL_CANCEL_ACTIONFLOATING;
	pCmdUI->SetCheck(m_bCancelAction[0][nIndex] && m_bCancelAction[1][nIndex]
		&& m_bCancelAction[2][nIndex] && m_bCancelAction[3][nIndex]? TRUE: FALSE);

}

void CMainFrame::OnActionsAllFlags(UINT nID)
{
	int nFlag = nID == ID_ACTIONS_ALL_FLAGS_NOCLOSEABLE? xtpPaneNoCloseable:
		nID == ID_ACTIONS_ALL_FLAGS_NOHIDEABLE? xtpPaneNoHideable: xtpPaneNoFloatable;

	POSITION pos = m_paneManager.GetPaneList().GetHeadPosition();
	while (pos)
	{
		CXTPDockingPane* pPane = m_paneManager.GetPaneList().GetNext(pos);
		DWORD dwOptions = pPane->GetOptions();
		if (dwOptions & nFlag) dwOptions &= ~ nFlag; else dwOptions |= nFlag;
		pPane->SetOptions(dwOptions);
	}

	m_paneManager.RedrawPanes();
}

void CMainFrame::OnUpdateActionsAllFlags(CCmdUI* pCmdUI)
{
	UINT nID = pCmdUI->m_nID;
	int nFlag = nID == ID_ACTIONS_ALL_FLAGS_NOCLOSEABLE? xtpPaneNoCloseable:
		nID == ID_ACTIONS_ALL_FLAGS_NOHIDEABLE? xtpPaneNoHideable: xtpPaneNoFloatable;

	BOOL bSet = TRUE;

	POSITION pos = m_paneManager.GetPaneList().GetHeadPosition();
	while (pos)
	{
		if ((m_paneManager.GetPaneList().GetNext(pos)->GetOptions() & nFlag) == 0)
			bSet = FALSE;
	}

	pCmdUI->SetCheck(bSet);

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

void CMainFrame::OnClose()
{
	// Save the current state for toolbars and menus.
	SaveCommandBars(_T("CommandBars"));
	CFrameWnd::OnClose();
}

#endif
