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
#include "Advanced.h"

#include "MainFrm.h"
#include "PopoutFrame.h"

#include "HeroesView.h"
#include "DockingFrame.h"
#include "AdvancedDialog.h"

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
	ON_COMMAND(ID_WINDOW_NEWPOPOUT, OnWindowNewpopout)
	ON_UPDATE_COMMAND_UI(ID_WINDOW_NEWPOPOUT, OnUpdateWindowNewpopout)
	ON_WM_CLOSE()
	ON_MESSAGE(XTPWM_DOCKINGPANE_NOTIFY, OnDockingPaneNotify)
	ON_COMMAND(ID_WINDOW_DIALOG, OnWindowDialog)
	//}}AFX_MSG_MAP
	ON_COMMAND(XTP_ID_CUSTOMIZE, OnCustomize)

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
	m_pHeroesView = 0;
	m_pNewFrameWnd = 0;
	m_pDockingFrame = 0;

}

CMainFrame::~CMainFrame()
{
	if (m_pHeroesView)
		delete m_pHeroesView;
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


	XTPImageManager()->SetMaskColor(CXTPImageManager::GetBitmapMaskColor(IDR_MAINFRAME));

	CXTPPaintManager::SetTheme(xtpThemeOfficeXP);

	CXTPCommandBars* pCommandBars = GetCommandBars();

	CXTPCommandBar* pMenuBar = pCommandBars->SetMenu(_T("Menu Bar"), IDR_ADVANCTYPE);
	pMenuBar->SetFlags(xtpFlagIgnoreSetMenuMessage);


	CXTPToolBar* pCommandBar = (CXTPToolBar*)pCommandBars->Add(_T("Standard"), xtpBarTop);
	if (!pCommandBar ||
		!pCommandBar->LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;
	}

	((CXTPControlButton*)(pCommandBar->GetControls()->FindControl(ID_WINDOW_DIALOG)))->SetStyle(xtpButtonCaption);
	((CXTPControlButton*)(pCommandBar->GetControls()->FindControl(ID_WINDOW_NEWPOPOUT)))->SetStyle(xtpButtonCaption);


	GetDockingPaneManager()->InstallDockingPanes(this);
	GetDockingPaneManager()->SetTheme(xtpPaneThemeOffice);

	CXTPDockingPane* pwndPaneFrame = GetDockingPaneManager()->CreatePane(
		2, CRect(0, 0,200, 120), xtpPaneDockRight);
	pwndPaneFrame->SetTitle(_T("Docking Frame\nFrame"));

	CXTPDockingPane* pwndPaneEdit = GetDockingPaneManager()->CreatePane(
		3, CRect(0, 0,200, 120), xtpPaneDockBottom);
	pwndPaneEdit->SetTitle(_T("Edit Control\nEdit"));

	CXTPDockingPane* pwndPaneHeroes = GetDockingPaneManager()->CreatePane(
		1, CRect(0, 0,200, 120), xtpPaneDockLeft);
	pwndPaneHeroes->SetTitle(_T("Super Heroes\nHeroes"));

	const int nIDTabs[] = {1, 2, 3};
	GetDockingPaneManager()->SetIcons(IDB_BITMAP_ICONS, nIDTabs, 3);

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
			if (pPane->GetID() == 1)
			{
				if (m_pHeroesView == 0)
				{
					m_pHeroesView = new  CHeroesView();
					m_pHeroesView->Create(_T("STATIC"), NULL, WS_CHILD, CRect(0, 0, 0, 0), this, 0);
					((CHeroesView*)m_pHeroesView)->OnInitialUpdate();
				}

				pPane->Attach(m_pHeroesView);
			}
			if (pPane->GetID() == 2)
			{
				if (m_pDockingFrame == 0)
				{
					m_pDockingFrame = (CDockingFrame*)CDockingFrame::CreateObject();
					m_pDockingFrame->Create(NULL, NULL, WS_CHILD|WS_CLIPCHILDREN|WS_CLIPSIBLINGS, CRect(0, 0, 0, 0), this, 0);
					m_pDockingFrame->ModifyStyleEx(WS_EX_CLIENTEDGE, 0);


				}

				pPane->Attach(m_pDockingFrame);
			}
			if (pPane->GetID() == 3)
			{
				if (!m_wndEdit.GetSafeHwnd())
				{
					VERIFY(m_wndEdit.CreateEx(WS_EX_STATICEDGE, _T("EDIT"), _T(""),
						WS_CHILD|ES_AUTOVSCROLL|ES_MULTILINE, CRect(0, 0,200, 120), this, 0));
					m_wndEdit.SetFont(CFont::FromHandle((HFONT)GetStockObject(DEFAULT_GUI_FONT)));

				}
				pPane->Attach(&m_wndEdit);
			}

		}

		return TRUE; // handled
	}
	return FALSE;
}
void CMainFrame::OnWindowNewpopout()
{
	if (m_pNewFrameWnd)
	{
		m_pNewFrameWnd->BringWindowToTop();
		return;
	}

	m_pNewFrameWnd = new CPopoutFrame;

	m_pNewFrameWnd->LoadFrame(IDR_MAINFRAME,
		WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, NULL, NULL);

	// The one and only window has been initialized, so show and update it.
	m_pNewFrameWnd->ShowWindow(SW_SHOW);
	m_pNewFrameWnd->UpdateWindow();
}

void CMainFrame::OnUpdateWindowNewpopout(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_pNewFrameWnd && ::IsWindow(m_pNewFrameWnd->m_hWnd));
}

void CMainFrame::OnClose()
{
	// Save the current state for command bars.
	SaveCommandBars(_T("CommandBars"));

	if (m_pNewFrameWnd)
		m_pNewFrameWnd->SendMessage(WM_CLOSE);

	CMDIFrameWnd::OnClose();
}

void CMainFrame::OnWindowDialog()
{
	CAdvancedDialog ad;
	ad.DoModal();

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
	pPageCommands->AddCategories(IDR_ADVANCTYPE);

	// initialize the commands page page.
	pPageCommands->InsertAllCommandsCategory();
	pPageCommands->InsertBuiltInMenus(IDR_ADVANCTYPE);
	pPageCommands->InsertNewMenuCategory();

	// display the customize dialog.
	dlg.DoModal();
}


void CMainFrame::OnSetPreviewMode(BOOL bPreview, CPrintPreviewState* pState)
{
// Toggle CommandBars
	GetCommandBars()->OnSetPreviewMode(bPreview);

// Toggle Docking Panes.
	m_paneManager.OnSetPreviewMode(bPreview);


	CMDIFrameWnd::OnSetPreviewMode(bPreview, pState);
}
