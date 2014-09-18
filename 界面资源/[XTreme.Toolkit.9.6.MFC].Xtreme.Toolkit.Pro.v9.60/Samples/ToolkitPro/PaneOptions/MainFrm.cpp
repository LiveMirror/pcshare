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
#include "PaneOptions.h"

#include "PreviewFrame.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CArray<CXTPDockingPaneManager*, CXTPDockingPaneManager*> CMainFrame::m_arrManagers;
/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_COMMAND(ID_VIEW_OFFICEXP, OnViewOfficexp)
	ON_UPDATE_COMMAND_UI(ID_VIEW_OFFICEXP, OnUpdateViewOfficexp)
	ON_COMMAND(ID_VIEW_OFFICE2003, OnViewOffice2003)
	ON_UPDATE_COMMAND_UI(ID_VIEW_OFFICE2003, OnUpdateViewOffice2003)
	//}}AFX_MSG_MAP

	ON_XTP_CREATECONTROL()
	ON_COMMAND(ID_HELP_SEARCH, OnEmptyCommand)
	ON_COMMAND(ID_HELP_HELP, OnEmptyCommand)
	ON_COMMAND(ID_HELP_USING, OnEmptyCommand)
	ON_COMMAND(ID_BUILD_COMPILE, OnEmptyCommand)
	ON_COMMAND(ID_BUILD_DEBUG, OnEmptyCommand)
	ON_COMMAND(ID_BUILD_RUN, OnEmptyCommand)

	ON_XTP_EXECUTE(ID_BUILD_CONFIGURATION, OnBuildConfiguration)
	ON_UPDATE_COMMAND_UI(ID_BUILD_CONFIGURATION, OnUpdateBuildConfiguration)

	ON_COMMAND(XTP_ID_CUSTOMIZE, OnCustomize)

	ON_MESSAGE(XTPWM_DOCKINGPANE_NOTIFY, OnDockingPaneNotify)


END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

#define PANEID_NAVIGATION 10

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	m_nState = 0;
	XTAuxData().bXPMode = TRUE;
}

CMainFrame::~CMainFrame()
{
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

	CXTPPaintManager::SetTheme(xtpThemeOfficeXP);

	CXTPCommandBars* pCommandBars = GetCommandBars();

	pCommandBars->SetMenu(_T("Menu Bar"), IDR_MAINFRAME);

	XTPImageManager()->SetMaskColor(RGB(0, 255, 0));

	CXTPToolBar* pStandardBar = pCommandBars->Add(_T("Standard"), xtpBarTop);
	if (!pStandardBar ||
		!pStandardBar->LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
	}

	CXTPToolBar* pBuildBar = pCommandBars->Add(_T("Build Bar"), xtpBarTop);
	if (!pBuildBar ||
		!pBuildBar->LoadToolBar(IDR_BUILD_BAR))
	{
		TRACE0("Failed to create toolbar\n");
	}
	CXTPToolBar* pHelpBar = (CXTPToolBar*)pCommandBars->Add(_T("Help Bar"), xtpBarTop);
	if (!pHelpBar ||
		!pHelpBar->LoadToolBar(IDR_HELP_BAR))
	{
		TRACE0("Failed to create toolbar\n");
	}

	DockRightOf(pBuildBar, pStandardBar);
	DockRightOf(pHelpBar, pBuildBar);


	m_paneManager.InstallDockingPanes(this);
	m_paneManager.SetTheme(xtpPaneThemeOffice);

	m_arrManagers.Add(&m_paneManager);

	// Create docking panes.
	CXTPDockingPane* pwndPane1 = m_paneManager.CreatePane(
		PANEID_NAVIGATION, CRect(0, 0,200, 120), xtpPaneDockLeft);

	pwndPane1->SetTitle(_T("Project Navigation"));

	const int  nIDs[] = {PANEID_NAVIGATION};
	m_paneManager.SetIcons(IDB_PANE_ICONS, nIDs, 1);


	m_ilTreeIcons.Create(16, 16, ILC_MASK|ILC_COLOR8, 0, 1);

	CBitmap bmp;
	bmp.LoadBitmap(IDR_TREE_ICONS);

	m_ilTreeIcons.Add(&bmp, RGB(255, 0, 255));

	return 0;
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

BOOL CMainFrame::InitializeCaptionBar()
{
	if (!m_wndCaption.Create(&m_wndSplitterCaption, _T("Caption Text"), NULL,
		WS_VISIBLE|SS_CENTER|SS_CENTERIMAGE, CRect(0,0,0,0), m_wndSplitterCaption.IdFromRowCol(0, 0)))
	{
		TRACE0("Failed to caption window.\n");
		return FALSE;
	}

	HICON hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	// set the caption colors.
	m_wndCaption.SetCaptionColors (GetXtremeColor(COLOR_3DFACE),
		GetXtremeColor(COLOR_3DSHADOW), GetXtremeColor(COLOR_WINDOW) );

	LOGFONT lf;
	XTAuxData().fontBold.GetLogFont(&lf);
	lf.lfHeight = 18;
	m_fontCaption.CreateFontIndirect(&lf);

	// set the font and initial icon.
	m_wndCaption.ModifyCaptionStyle(0,
		&m_fontCaption, NULL, hIcon);

	// Sizing for splitter
	m_wndSplitterCaption.SetRowInfo (0, 22, 0);

	return TRUE;
}

BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT /*lpcs*/, CCreateContext* /*pContext*/)
{
	// TODO: Add your specialized code here and/or call the base class
	m_wndSplitterCaption.CreateStatic(this, 2, 1);
	m_wndSplitterCaption.ModifyStyle(0, WS_CLIPCHILDREN|WS_CLIPSIBLINGS);


	if (!InitializeCaptionBar())
		return FALSE;


	m_pPreviewFrame = (CPreviewFrame*)CPreviewFrame::CreateObject();
	m_pPreviewFrame->CreateEx(NULL, NULL, NULL, WS_CHILD|WS_VISIBLE, CRect(0, 0, 0, 0), &m_wndSplitterCaption, m_wndSplitterCaption.IdFromRowCol(1, 0));

	m_wndSplitterCaption.SetSplitterStyle(XT_SPLIT_NOFULLDRAG | XT_SPLIT_NOSIZE | XT_SPLIT_NOBORDER);

	return TRUE;
}



int CMainFrame::OnCreateControl(LPCREATECONTROLSTRUCT lpCreateControl)
{
	if (lpCreateControl->bToolBar)
	{
		CXTPToolBar* pToolBar = DYNAMIC_DOWNCAST(CXTPToolBar, lpCreateControl->pCommandBar);
		if (!pToolBar)
			return FALSE;

		if (lpCreateControl->nID == ID_BUILD_CONFIGURATION && pToolBar->GetBarID() == IDR_BUILD_BAR)
		{
			CXTPControlComboBox* pComboState = (CXTPControlComboBox*)CXTPControlComboBox::CreateObject();
			pComboState->AddString(_T("Debug"));
			pComboState->AddString(_T("Release"));
			pComboState->AddString(_T("Configuration Manager"));
			pComboState->SetDropDownWidth(150);
			lpCreateControl->pControl = pComboState;
			return TRUE;
		}
	}
	return FALSE;

}
void CMainFrame::OnEmptyCommand()
{

}

void CMainFrame::OnBuildConfiguration(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMXTPCONTROL* tagNMCONTROL = (NMXTPCONTROL*)pNMHDR;

	CXTPControlComboBox* pControl = (CXTPControlComboBox*)tagNMCONTROL->pControl;
	if (pControl->GetType() == xtpControlComboBox)
	{
		int nState = pControl->GetCurSel();
		if (nState != 2)
		{
			m_nState = nState;
		}
		else
		{
			MessageBox(_T("Configuration Manager"));
			pControl->SetCurSel(m_nState);
		}
		*pResult = TRUE; // Handled
	}
}

void CMainFrame::OnUpdateBuildConfiguration(CCmdUI* pCmdUI)
{
	CXTPCommandBar* pToolBar = (CXTPToolBar*)pCmdUI->m_pOther;
	if (pToolBar && !XTPMouseManager()->IsTrackedLock())
	{
		CXTPControlComboBox* pStateCombo = (CXTPControlComboBox*)pToolBar->GetControls()->GetAt(pCmdUI->m_nIndex);

		if (pStateCombo->GetType() == xtpControlComboBox)
		{
			pStateCombo->SetCurSel(m_nState);
		}
	}

	pCmdUI->Enable(TRUE);
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



LRESULT CMainFrame::OnDockingPaneNotify(WPARAM wParam, LPARAM lParam)
{
	if (wParam == XTP_DPN_SHOWWINDOW)
	{
		// get a pointer to the docking pane being shown.
		CXTPDockingPane* pPane = (CXTPDockingPane*)lParam;
		ASSERT(pPane->GetID() == PANEID_NAVIGATION);

		if (!pPane->IsValid())
		{
			// create and attach the edit control for this pane.
			if (!::IsWindow(m_wndTreeCtrl.m_hWnd))
			{
				// create the tree control.
				if (!m_wndTreeCtrl.Create (WS_CLIPCHILDREN|WS_CLIPSIBLINGS|WS_VISIBLE|TVS_HASLINES|TVS_LINESATROOT|TVS_HASBUTTONS|TVS_SHOWSELALWAYS,
					CRect(0,0,0,0), this, 101))
				{
					TRACE0( "Unable to create tree control.\n" );
					return -1;
				}

				m_wndTreeCtrl.SetImageList(&m_ilTreeIcons, TVSIL_NORMAL);


				m_wndTreeCtrl.InsertItem(_T("General"), 0, 0);
				HTREEITEM hItem = m_wndTreeCtrl.InsertItem(_T("Standard Resource"), 0, 0);
				m_wndTreeCtrl.InsertItem(_T("Custom Resource"), 0, 0);
				m_wndTreeCtrl.InsertItem(_T("User Interface"), 0, 0);

				m_wndTreeCtrl.InsertItem(_T("File System"), 6, 6, hItem);
				m_wndTreeCtrl.InsertItem(_T("Perfomance"), 5, 5, hItem);
				m_wndTreeCtrl.InsertItem(_T("COM+"), 1, 1, hItem);
				m_wndTreeCtrl.InsertItem(_T("System Tools"), 3, 3, hItem);
				m_wndTreeCtrl.InsertItem(_T("Local Users"), 4, 4, hItem);
				m_wndTreeCtrl.InsertItem(_T("Controls Services"), 2, 2, hItem);

				m_wndTreeCtrl.Expand (hItem, TVE_EXPAND);
			}

			pPane->Attach(&m_wndTreeCtrl);
		}

		return TRUE; // handled
	}


	return FALSE;
}

void CMainFrame::OnViewOfficexp()
{
	for (int i = 0; i < m_arrManagers.GetSize(); i++) m_arrManagers[i]->SetTheme(xtpPaneThemeOffice);

	m_wndCaption.SetCaptionColors (GetXtremeColor(COLOR_3DFACE),
		GetXtremeColor(COLOR_3DSHADOW), GetXtremeColor(COLOR_WINDOW) );

	XTThemeManager()->SetTheme(xtThemeOfficeXP);

	CXTPPaintManager::SetTheme(xtpThemeOfficeXP);
	RedrawWindow(0, 0, RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE|RDW_ALLCHILDREN);
}

void CMainFrame::OnUpdateViewOfficexp(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(XTPPaintManager()->GetCurrentTheme() == xtpThemeOfficeXP );
}

void CMainFrame::OnViewOffice2003()
{
	for (int i = 0; i < m_arrManagers.GetSize(); i++) m_arrManagers[i]->SetTheme(xtpPaneThemeOffice2003);

	XTThemeManager()->SetTheme(xtThemeOffice2003);

	CXTPPaintManager::SetTheme(xtpThemeOffice2003);
	RedrawWindow(0, 0, RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE|RDW_ALLCHILDREN);
}

void CMainFrame::OnUpdateViewOffice2003(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(XTPPaintManager()->GetCurrentTheme() == xtpThemeOffice2003);
}
