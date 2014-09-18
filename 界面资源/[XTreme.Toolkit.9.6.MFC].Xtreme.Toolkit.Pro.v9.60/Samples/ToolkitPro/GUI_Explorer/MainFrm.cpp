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
#include "GUI_Explorer.h"

#include "MainFrm.h"
#include "GUI_ExplorerTree.h"
#include "GUI_ExplorerView.h"
#include "HistoryView.h"
#include "SearchView.h"
#include "InternetView.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifndef AFX_ID_VIEW_MINIMUM
#define AFX_ID_VIEW_MINIMUM              ID_VIEW_SMALLICON
#endif

#ifndef AFX_ID_VIEW_MAXIMUM
#define AFX_ID_VIEW_MAXIMUM              ID_VIEW_BYNAME
#endif

LPCTSTR lpszProfileName = _T("CommandBars (v2.0)");


/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_COMMAND(ID_BACK, OnBack)
	ON_COMMAND(ID_FORWARD, OnForward)
	ON_COMMAND(ID_UP, OnUp)
	ON_COMMAND(ID_VIEW_SEARCH, OnSearch)
	ON_COMMAND(ID_VIEW_FOLDERS, OnFolders)
	ON_COMMAND(ID_VIEW_HISTORY, OnHistory)
	ON_COMMAND(ID_MOVETO, OnMoveto)
	ON_COMMAND(ID_COPYTO, OnCopyto)
	ON_COMMAND(ID_DELETE, OnDelete)
	ON_COMMAND(ID_UNDO, OnUndo)
	ON_COMMAND(ID_VIEWS, OnViews)
	ON_UPDATE_COMMAND_UI(ID_VIEW_FOLDERS, OnUpdateFolders)
	ON_UPDATE_COMMAND_UI(ID_VIEW_SEARCH, OnUpdateSearch)
	ON_UPDATE_COMMAND_UI(ID_VIEW_HISTORY, OnUpdateHistory)
	ON_COMMAND(ID_VIEW_XPMODE, OnViewXpmode)
	ON_UPDATE_COMMAND_UI(ID_VIEW_XPMODE, OnUpdateViewXpmode)
	ON_WM_CLOSE()
	ON_UPDATE_COMMAND_UI(ID_BACK, OnUpdateBack)
	ON_UPDATE_COMMAND_UI(ID_FORWARD, OnUpdateForward)
	ON_UPDATE_COMMAND_UI(ID_UP, OnUpdateUp)
	ON_UPDATE_COMMAND_UI(ID_MOVETO, OnUpdateMoveto)
	ON_UPDATE_COMMAND_UI(ID_COPYTO, OnUpdateCopyto)
	ON_UPDATE_COMMAND_UI(ID_DELETE, OnUpdateDelete)
	ON_UPDATE_COMMAND_UI(ID_UNDO, OnUpdateUndo)
	ON_COMMAND(ID_GO, OnGo)
	//}}AFX_MSG_MAP

	ON_UPDATE_COMMAND_UI_RANGE(AFX_ID_VIEW_MINIMUM, AFX_ID_VIEW_MAXIMUM, OnUpdateViewStyles)
	ON_COMMAND_RANGE(AFX_ID_VIEW_MINIMUM, AFX_ID_VIEW_MAXIMUM, OnViewStyle)

	ON_MESSAGE(XTWM_SHELL_NOTIFY, OnUpdateShell)
	ON_COMMAND(IDOK, OnAddressBarEnter)
	ON_CBN_SELENDOK(IDC_ADDRESSBAR, OnSelEndOkAddressBar)

	ON_COMMAND(XTP_ID_CUSTOMIZE, OnCustomize)
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
	// TODO: add member initialization code here
	m_pSearchView  = NULL;
	m_pHistoryView = NULL;
	m_pTreeView    = NULL;
	m_pFolderView  = NULL;
	m_pINetView    = NULL;
	m_pListCtrl    = NULL;
	m_pTreeCtrl    = NULL;

	// Enable/Disable XP GUI Mode
	CXTRegistryManager regMgr;
	XTAuxData().bXPMode = regMgr.GetProfileInt(_T("Settings"), _T("bXPMode"), FALSE);

}

CMainFrame::~CMainFrame()
{
	CXTRegistryManager regMgr;
	regMgr.WriteProfileInt(_T("Settings"), _T("bXPMode"), XTAuxData().bXPMode);
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
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

	CXTPMenuBar* pMenuBar = pCommandBars->SetMenu(_T("Menu Bar"), IDR_MAINFRAME);


	CXTPToolBar* pStandardBar = (CXTPToolBar*)pCommandBars->Add(_T("Standard"), xtpBarTop);
	if (!pStandardBar ||
		!pStandardBar->LoadToolBar(IDR_EXPLORER_BAR))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;
	}

	CXTPToolBar* pGoBar = (CXTPToolBar*)pCommandBars->Add(_T("Go"), xtpBarTop);
	if (!pGoBar ||
		!pGoBar->LoadToolBar(IDR_GO_BAR))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;
	}
	pGoBar->SetButtonSize(CSize(25, 25));

	InitializeAddressBar();

	m_wndLogo.Create(NULL, WS_CHILD|WS_VISIBLE|SS_BITMAP|SS_CENTERIMAGE, CRect(0, 0, 138, 23), this);
	VERIFY(m_bmpLogo.LoadBitmap(IDB_LOGO));
	m_wndLogo.SetBitmap(m_bmpLogo);

	if (!m_wndReBar.Create(this) ||
		!m_wndReBar.AddToolBar(pMenuBar) ||
		!m_wndReBar.AddBar(&m_wndLogo, 0, 0, RBBS_NOGRIPPER | RBBS_FIXEDBMP | RBBS_FIXEDSIZE) ||
		!m_wndReBar.AddToolBar(pStandardBar, RBBS_GRIPPERALWAYS | RBBS_BREAK) ||
		!m_wndReBar.AddBar(&m_wndAddressBar, _T("Address"), NULL, RBBS_GRIPPERALWAYS | RBBS_BREAK) ||
		!m_wndReBar.AddToolBar(pGoBar) )
	{
		TRACE0("Failed to create rebar\n");
		return -1;      // fail to create
	}

	// Set sizes for logo band.
	REBARBANDINFO rbbi;
	rbbi.cbSize     = sizeof(rbbi);
	rbbi.fMask      = RBBIM_CHILDSIZE|RBBIM_IDEALSIZE|RBBIM_SIZE|RBBIM_ID;
	rbbi.cx         =
	rbbi.cxIdeal    =
	rbbi.cxMinChild = 138;
	rbbi.cyMinChild = 23;
	rbbi.wID = IDB_LOGO;

	m_wndReBar.SetBandInfo(1, &rbbi);


	XTPImageManager()->SetIcons(IDR_GO_BAR, IDB_GO_BAR_HOT, xtpImageHot);

	XTAuxData().bXPMode = !XTAuxData().bXPMode;
	OnViewXpmode();


	LoadCommandBars(lpszProfileName);
	m_wndReBar.LoadState(lpszProfileName);

	return 0;
}

BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT /*lpcs*/,
	CCreateContext* pContext)
{
	// create splitter window
	if (!m_wndSplitter.CreateStatic(this, 1, 2))
		return FALSE;

	if (!m_wndSplitter.CreateView(0, 0, RUNTIME_CLASS(CGUI_ExplorerTree),
		CSize(300, 100), pContext))
	{
		m_wndSplitter.DestroyWindow();
		return FALSE;
	}

	if (!m_wndSplitter.CreateView(0, 1, RUNTIME_CLASS(CGUI_ExplorerView),
		CSize(100, 100), pContext))
	{
		m_wndSplitter.DestroyWindow();
		return FALSE;
	}

	m_wndSplitter.ShowTopBorder(false, 3);
	m_wndSplitter.SetSplitterStyle (XT_SPLIT_DOTTRACKER);

	return TRUE;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;

	cs.lpszClass = AfxRegisterWndClass( 0, NULL, NULL,
		AfxGetApp()->LoadIcon(IDR_MAINFRAME));

	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	cs.style &= ~FWS_ADDTOTITLE;

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

void CMainFrame::OnUpdateViewStyles(CCmdUI* pCmdUI)
{
	// TODO: customize or extend this code to handle choices on the
	// View menu.

	// if the right-hand pane hasn't been created or isn't a view,
	// disable commands in our range

	if (m_pListCtrl == NULL)
		pCmdUI->Enable(FALSE);
	else
	{
		DWORD dwStyle = m_pListCtrl->GetStyle() & LVS_TYPEMASK;

		// if the command is ID_VIEW_LINEUP, only enable command
		// when we're in LVS_ICON or LVS_SMALLICON mode

		if (pCmdUI->m_nID == ID_VIEW_LINEUP)
		{
			if (dwStyle == LVS_ICON || dwStyle == LVS_SMALLICON)
				pCmdUI->Enable();
			else
				pCmdUI->Enable(FALSE);
		}
		else
		{
			// otherwise, use dots to reflect the style of the view
			pCmdUI->Enable();
			BOOL bChecked = FALSE;

			switch (pCmdUI->m_nID)
			{
			case ID_VIEW_DETAILS:
				bChecked = (dwStyle == LVS_REPORT);
				break;

			case ID_VIEW_SMALLICON:
				bChecked = (dwStyle == LVS_SMALLICON);
				break;

			case ID_VIEW_LARGEICON:
				bChecked = (dwStyle == LVS_ICON);
				break;

			case ID_VIEW_LIST:
				bChecked = (dwStyle == LVS_LIST);
				break;

			default:
				bChecked = FALSE;
				break;
			}

			pCmdUI->SetRadio(bChecked ? 1 : 0);
		}
	}
}


void CMainFrame::OnViewStyle(UINT nCommandID)
{
	// TODO: customize or extend this code to handle choices on the
	// View menu.

	// if the right-hand pane has been created and is a CGUI_ExplorerView,
	// process the menu commands...
	if (m_pListCtrl != NULL)
	{
		DWORD dwStyle = (DWORD)-1;

		switch (nCommandID)
		{
		case ID_VIEW_LINEUP:
			{
				// ask the list control to snap to grid
				m_pListCtrl->Arrange(LVA_SNAPTOGRID);
			}
			break;

		// other commands change the style on the list control
		case ID_VIEW_DETAILS:
			dwStyle = LVS_REPORT;
			break;

		case ID_VIEW_SMALLICON:
			dwStyle = LVS_SMALLICON;
			break;

		case ID_VIEW_LARGEICON:
			dwStyle = LVS_ICON;
			break;

		case ID_VIEW_LIST:
			dwStyle = LVS_LIST;
			break;
		}

		// change the style; window will repaint automatically
		if (dwStyle != -1)
			m_pListCtrl->ModifyStyle(LVS_TYPEMASK, dwStyle);
	}
}

void CMainFrame::OnBack()
{
	// TODO: Add your command handler code here

}

void CMainFrame::OnUpdateBack(CCmdUI* pCmdUI)
{
	// display disabled image list.
	pCmdUI->Enable(FALSE);
}

void CMainFrame::OnForward()
{
	// TODO: Add your command handler code here

}

void CMainFrame::OnUpdateForward(CCmdUI* pCmdUI)
{
	// display disabled image list.
	pCmdUI->Enable(FALSE);
}

void CMainFrame::OnUp()
{
	// TODO: Add your command handler code here
	if ( m_pTreeView )
	{
		m_pTreeView->SelectParentItem();
	}
}

void CMainFrame::OnUpdateUp(CCmdUI* /*pCmdUI*/)
{

}

void CMainFrame::OnSearch()
{
	if (m_wndSplitter.GetColumnCount() == 1)
	{
		m_wndSplitter.ShowColumn();
		if (m_wndSplitter.GetPane(0,0)->IsKindOf(RUNTIME_CLASS(CSearchView)))
		{
			return;
		}
	}
	if (!m_wndSplitter.GetPane(0,0)->IsKindOf(RUNTIME_CLASS(CSearchView)))
	{
		if (m_pSearchView == NULL)
		{
			if (m_wndSplitter.GetPane(0,0)->IsKindOf(
				RUNTIME_CLASS(CHistoryView)))
			{
				m_pHistoryView = (CHistoryView*)m_wndSplitter.ReplaceView(
					0,0,RUNTIME_CLASS(CSearchView));
			}
			else
			{
				m_pTreeView  = (CGUI_ExplorerTree*)m_wndSplitter.ReplaceView(
					0,0,RUNTIME_CLASS(CSearchView));
			}
		}
		else
		{
			if (m_wndSplitter.GetPane(0,0)->IsKindOf(
				RUNTIME_CLASS(CHistoryView)))
			{
				m_pHistoryView = (CHistoryView*)m_wndSplitter.ReplaceView(
					0, 0, (CView*)m_pSearchView);
			}
			else
			{
				m_pTreeView = (CGUI_ExplorerTree*)m_wndSplitter.ReplaceView(
					0, 0, (CView*)m_pSearchView);
			}
		}
	}
	else
	{
		m_wndSplitter.HideColumn(0);
	}
}

void CMainFrame::OnFolders()
{
	if (m_wndSplitter.GetColumnCount() == 1)
	{
		m_wndSplitter.ShowColumn();
		if (m_wndSplitter.GetPane(0,0)->IsKindOf(RUNTIME_CLASS(CGUI_ExplorerTree)))
		{
			return;
		}
	}
	if (!m_wndSplitter.GetPane(0,0)->IsKindOf(RUNTIME_CLASS(CGUI_ExplorerTree)))
	{
		if (m_pTreeView == NULL)
		{
			if (m_wndSplitter.GetPane(0,0)->IsKindOf(
				RUNTIME_CLASS(CHistoryView)))
			{
				m_pHistoryView = (CHistoryView*)m_wndSplitter.ReplaceView(
					0,0,RUNTIME_CLASS(CGUI_ExplorerTree));
			}
			else
			{
				m_pSearchView  = (CSearchView*)m_wndSplitter.ReplaceView(
					0,0,RUNTIME_CLASS(CGUI_ExplorerTree));
			}
		}
		else
		{
			if (m_wndSplitter.GetPane(0,0)->IsKindOf(
				RUNTIME_CLASS(CHistoryView)))
			{
				m_pHistoryView = (CHistoryView*)m_wndSplitter.ReplaceView(
					0, 0, (CView*)m_pTreeView);
			}
			else
			{
				m_pSearchView = (CSearchView*)m_wndSplitter.ReplaceView(
					0, 0, (CView*)m_pTreeView);
			}
		}
	}
	else
	{
		m_wndSplitter.HideColumn(0);
	}
}

void CMainFrame::OnHistory()
{
	if (m_wndSplitter.GetColumnCount() == 1)
	{
		m_wndSplitter.ShowColumn();
		if (m_wndSplitter.GetPane(0,0)->IsKindOf(RUNTIME_CLASS(CHistoryView)))
		{
			return;
		}
	}
	if (!m_wndSplitter.GetPane(0,0)->IsKindOf(RUNTIME_CLASS(CHistoryView)))
	{
		if (m_pHistoryView == NULL)
		{
			if (m_wndSplitter.GetPane(0,0)->IsKindOf(
				RUNTIME_CLASS(CGUI_ExplorerTree)))
			{
				m_pTreeView = (CGUI_ExplorerTree*)m_wndSplitter.ReplaceView(
					0,0,RUNTIME_CLASS(CHistoryView));
			}
			else
			{
				m_pSearchView  = (CSearchView*)m_wndSplitter.ReplaceView(
					0,0,RUNTIME_CLASS(CHistoryView));
			}
		}
		else
		{
			if (m_wndSplitter.GetPane(0,0)->IsKindOf(
				RUNTIME_CLASS(CGUI_ExplorerTree)))
			{
				m_pTreeView = (CGUI_ExplorerTree*)m_wndSplitter.ReplaceView(
					0, 0, (CView*)m_pHistoryView);
			}
			else
			{
				m_pSearchView = (CSearchView*)m_wndSplitter.ReplaceView(
					0, 0, (CView*)m_pHistoryView);
			}
		}
	}
	else
	{
		m_wndSplitter.HideColumn(0);
	}
}

void CMainFrame::OnMoveto()
{
	// TODO: Add your command handler code here

}

void CMainFrame::OnUpdateMoveto(CCmdUI* pCmdUI)
{
	// display disabled image list.
	pCmdUI->Enable(FALSE);
}

void CMainFrame::OnCopyto()
{
	// TODO: Add your command handler code here

}

void CMainFrame::OnUpdateCopyto(CCmdUI* pCmdUI)
{
	// display disabled image list.
	pCmdUI->Enable(FALSE);
}

void CMainFrame::OnDelete()
{
	// TODO: Add your command handler code here

}

void CMainFrame::OnUpdateDelete(CCmdUI* pCmdUI)
{
	// display disabled image list.
	pCmdUI->Enable(FALSE);
}

void CMainFrame::OnUndo()
{
	// TODO: Add your command handler code here

}

void CMainFrame::OnUpdateUndo(CCmdUI* pCmdUI)
{
	// display disabled image list.
	pCmdUI->Enable(FALSE);
}

void CMainFrame::OnViews()
{
	DWORD dwStyle = m_pListCtrl->GetStyle();

	switch (dwStyle & (LVS_REPORT | LVS_SMALLICON | LVS_ICON | LVS_LIST))
	{
	case LVS_ICON:      OnViewStyle( ID_VIEW_SMALLICON ); break;
	case LVS_SMALLICON: OnViewStyle( ID_VIEW_LIST );      break;
	case LVS_LIST:      OnViewStyle( ID_VIEW_DETAILS );   break;
	case LVS_REPORT:    OnViewStyle( ID_VIEW_LARGEICON ); break;
	}
}

void CMainFrame::OnUpdateFolders(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_wndSplitter.GetPane(0,0)->IsKindOf(
		RUNTIME_CLASS(CGUI_ExplorerTree)));
}

void CMainFrame::OnUpdateSearch(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_wndSplitter.GetPane(0,0)->IsKindOf(
		RUNTIME_CLASS(CSearchView)));
}

void CMainFrame::OnUpdateHistory(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_wndSplitter.GetPane(0,0)->IsKindOf(
		RUNTIME_CLASS(CHistoryView)));
}


void CMainFrame::InitializeAddressBar()
{
	// create a combo box for the address bar
	if (!m_wndAddressBar.Create(CBS_DROPDOWN | WS_CHILD,
		CRect(0,0,200,400), this, IDC_ADDRESSBAR))
	{
		TRACE0("Failed to create combobox\n");
		return;      // fail to create
	}

	// set the font for the combo and initialize the toolbar.
	m_wndAddressBar.SetFont(&XTAuxData().font);

	m_pTreeView   = (CGUI_ExplorerTree*)m_wndSplitter.GetPane(0,0);
	m_pFolderView = (CGUI_ExplorerView*)m_wndSplitter.GetPane(0,1);

	m_pTreeCtrl = &m_pTreeView->GetTreeCtrl();
	m_pListCtrl = &m_pFolderView->GetListCtrl();

	m_pListCtrl->AssociateTree (m_pTreeCtrl);
	m_pTreeCtrl->AssociateList (m_pListCtrl);
	m_pTreeCtrl->AssociateCombo(&m_wndAddressBar);
}

LRESULT CMainFrame::OnUpdateShell(WPARAM wParam, LPARAM /*lParam*/)
{
	switch (wParam)
	{
	case SHN_XT_INETFOLDER:
		{
			int iCount = m_wndSplitter.GetColumnCount( );
			CWnd* pWnd = m_wndSplitter.GetPane( 0,iCount-1 );

			if ( pWnd && pWnd->IsKindOf( RUNTIME_CLASS( CInternetView ) ) )
				break;

			if ( m_pINetView == NULL )
			{
				m_pFolderView = ( CGUI_ExplorerView* )m_wndSplitter.ReplaceView(
					0,iCount-1, RUNTIME_CLASS( CInternetView ) );
			}
			else
			{
				m_pFolderView = ( CGUI_ExplorerView* )m_wndSplitter.ReplaceView(
					0,iCount-1, ( CView* )m_pINetView );
			}
		}
		break;

	default:
		{
			int iCount = m_wndSplitter.GetColumnCount( );
			CWnd* pWnd = m_wndSplitter.GetPane( 0,iCount-1 );

			if ( pWnd && pWnd->IsKindOf( RUNTIME_CLASS( CGUI_ExplorerView ) ) )
				break;

			m_pINetView = ( CInternetView* )m_wndSplitter.ReplaceView(
				0,1, ( CView* )m_pFolderView );
		}
		break;
	}

	return 0;
}


void CMainFrame::OnViewXpmode()
{
	XTAuxData().bXPMode = !XTAuxData().bXPMode;

	CXTCaptionButton& captBtn = m_pTreeView->m_wndCaption.GetCaptionButton();

	if (XTAuxData().bXPMode)
	{
		captBtn.SetXButtonStyle(
			(captBtn.GetXButtonStyle() | BS_XT_XPFLAT) & ~BS_XT_FLAT);
	}
	else
	{
		captBtn.SetXButtonStyle(
			(captBtn.GetXButtonStyle() | BS_XT_FLAT) & ~BS_XT_XPFLAT);
	}

	if (XTAuxData().bXPMode)
		CXTPPaintManager::SetCustomTheme(new CXTPReBarPaintManager<CXTPWhidbeyTheme>);
	else
		CXTPPaintManager::SetCustomTheme(new CXTPReBarPaintManager<CXTPNativeXPTheme>);

	XTPPaintManager()->GetIconsInfo()->bUseDisabledIcons = TRUE;
	XTPPaintManager()->GetIconsInfo()->bUseFadedIcons = FALSE;
	XTPPaintManager()->GetIconsInfo()->bIconsWithShadow = FALSE;


	RedrawWindow(NULL, NULL,
		RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE | RDW_ALLCHILDREN);

	GetCommandBars()->RedrawCommandBars();
}

void CMainFrame::OnUpdateViewXpmode(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(XTAuxData().bXPMode);
}

void CMainFrame::OnClose()
{

	SaveCommandBars(lpszProfileName);
	m_wndReBar.SaveState(lpszProfileName);

	// save frame window size and position.
	m_wndPosition.SaveWindowPos(this);

	CFrameWnd::OnClose();
}

BOOL CMainFrame::ShowWindowEx(int nCmdShow)
{
	ASSERT_VALID(this);

	// restore saved rebar state.
	//m_wndReBar.LoadState(_T("Rebar State"));

	// restore frame window size and position.
	m_wndPosition.LoadWindowPos(this);
	nCmdShow = m_wndPosition.showCmd;

	return ShowWindow(SW_SHOW);
}

void CMainFrame::OnAddressBarEnter()
{
	CString strNewPath;
	m_wndAddressBar.GetWindowText(strNewPath);

	if (strNewPath.GetLength())
	{
		m_pTreeCtrl->TunnelTree(strNewPath);
	}
}

void CMainFrame::OnSelEndOkAddressBar()
{
	if (m_pTreeCtrl != NULL)
	{
		HTREEITEM hItem = (HTREEITEM)m_wndAddressBar.GetItemData(
			m_wndAddressBar.GetCurSel());

		if (hItem != NULL) {
			m_pTreeCtrl->SelectItem(hItem);
		}
	}
}


void CMainFrame::OnGo()
{
	// TODO: Add your command handler code here
	OnAddressBarEnter();
}


int CMainFrame::OnCreateControl(LPCREATECONTROLSTRUCT lpCreateControl)
{
	if (lpCreateControl->nID == ID_VIEWS)
	{
		lpCreateControl->controlType = xtpControlSplitButtonPopup;
		return TRUE;
	}

	if (lpCreateControl->nID == ID_VIEWS)
	{
		lpCreateControl->controlType = xtpControlSplitButtonPopup;
		return TRUE;
	}

	switch (lpCreateControl->nID)
	{
		case ID_BACK:
		case ID_VIEW_SEARCH:
		case ID_VIEW_FOLDERS:
		case ID_VIEW_HISTORY:
		case ID_GO:
			lpCreateControl->buttonStyle = xtpButtonIconAndCaption;
			return TRUE;
	}

	return FALSE;
}

void CMainFrame::OnCustomize()
{
	CXTPCustomizeSheet cs(GetCommandBars());

	CXTPCustomizeOptionsPage pageOptions(&cs);
	cs.AddPage(&pageOptions);

	CXTPCustomizeCommandsPage* pCommands = cs.GetCommandsPage();
	pCommands->AddCategories(IDR_MAINFRAME);

	pCommands->InsertAllCommandsCategory();
	pCommands->InsertBuiltInMenus(IDR_MAINFRAME);
	pCommands->InsertNewMenuCategory();

	cs.DoModal();
}
