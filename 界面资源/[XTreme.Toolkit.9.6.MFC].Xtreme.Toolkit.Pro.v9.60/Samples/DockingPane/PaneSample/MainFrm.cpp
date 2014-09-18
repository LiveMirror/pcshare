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
#include "Pane.h"

#include "MainFrm.h"

//#define VISIO_EDIT_BACKGROUND
//#define VISIO_OFFICE2003_THEME
BOOL m_bMinMaxSample = FALSE;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace XTPDockingPanePaintThemes;

class CCustomTheme :  public CXTPDockingPaneGripperedTheme
{
	virtual void DrawCaption(CDC& dc,CXTPDockingPaneTabbedContainer* pPane, CRect rc)
	{
		CRect rcCaption(0, 2, rc.right, m_nTitleHeight + 1);

		// Drawing Frame
		dc.FillSolidRect(rcCaption, GetSysColor(COLOR_3DFACE));
		dc.Draw3dRect(1, rc.top + 1, rc.Width() -1, rcCaption.bottom + 1, GetSysColor(COLOR_WINDOW), GetSysColor(COLOR_WINDOW));
		dc.Draw3dRect(0, rc.top, rc.Width() - 1 , rcCaption.bottom + 2, GetSysColor(COLOR_3DSHADOW), GetSysColor(COLOR_3DSHADOW));

		COLORREF clrText = GetSysColor(COLOR_BTNTEXT);

		rcCaption.right -= 3;
		DrawCaptionButtons(&dc, pPane->GetCaptionButtons(), rcCaption, clrText, 14, 2);

		// Drawing Title
		if (pPane->GetSelected())
		{
			CRect rcText(rcCaption.left + 4, rcCaption.top + 1, rcCaption.right, rcCaption.bottom);

			dc.SetTextColor(clrText);
			CXTPFontDC font(&dc, &m_fntTitle);
			dc.DrawText(pPane->GetSelected()->GetTitle(), rcText, DT_SINGLELINE|DT_END_ELLIPSIS|DT_VCENTER);
		}
	}

};





#ifdef _XTP_INCLUDE_COMMANDBARS

class CDoubleGripperTheme : public CXTPDefaultTheme
{
	virtual CSize DrawCommandBarGripper(
		CDC* pDC, CXTPCommandBar* pBar, BOOL bDraw)
	{
		// If Toolbar is vertical docked
		if (pBar->GetPosition() == xtpBarRight ||
			pBar->GetPosition() == xtpBarLeft)
		{
			if (bDraw)
			{
				CXTPClientRect rc(pBar);
				Draw3dRect(pDC, CRect(3, 3, rc.right - 3, 6), COLOR_BTNHILIGHT, COLOR_3DSHADOW);
				Draw3dRect(pDC, CRect(3, 7, rc.right - 3, 10), COLOR_BTNHILIGHT, COLOR_3DSHADOW);
			}
			return CSize(0, 10);
		}
		// if Toolbar is horizontal  docked
		else if (pBar->GetPosition() == xtpBarTop ||
			pBar->GetPosition() == xtpBarBottom)
		{
			CXTPClientRect rc(pBar);
			if (bDraw)
			{
				Draw3dRect(pDC, CRect(3, 3, 6, rc.bottom - 3), COLOR_BTNHILIGHT, COLOR_3DSHADOW);
				Draw3dRect(pDC, CRect(7, 3, 10, rc.bottom - 3), COLOR_BTNHILIGHT, COLOR_3DSHADOW);
			}
			return CSize(10, 0);
		}
		else return CXTPDefaultTheme::DrawCommandBarGripper(pDC, pBar, bDraw);
	}
};
#endif


// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWnd)
	ON_WM_CREATE()
	ON_COMMAND_RANGE(IDR_PANE1, IDR_PANE10, OnShowPane)

	ON_COMMAND(ID_THEME_DEFAULT, OnThemeDefault)
	ON_COMMAND(ID_THEME_OFFICE, OnThemeOffice)
	ON_COMMAND(ID_THEME_WHIDBEY, OnThemeWhidbey)
	ON_COMMAND(ID_THEME_SHORTCUTBAR2003, OnThemeShortcutBar2003)
	ON_COMMAND(ID_THEME_OFFICE2003, OnThemeOffice2003)
	ON_COMMAND(ID_THEME_WINXP, OnThemeWinXP)
	ON_COMMAND(ID_THEME_GRIPPERED, OnThemeGrippered)
	ON_COMMAND(ID_THEME_EXPLORER, OnThemeExplorer)
	
	ON_UPDATE_COMMAND_UI_RANGE(ID_THEME_DEFAULT, ID_THEME_EXPLORER, OnUpdateTheme)

	ON_MESSAGE(XTPWM_DOCKINGPANE_NOTIFY, OnDockingPaneNotify)
	ON_COMMAND(ID_LAYOUT_EDITOR, OnLayoutEditor)
	ON_UPDATE_COMMAND_UI(ID_LAYOUT_EDITOR, OnUpdateLayoutEditor)
	ON_COMMAND(ID_LAYOUT_RUN, OnLayoutRun)
	ON_UPDATE_COMMAND_UI(ID_LAYOUT_RUN, OnUpdateLayoutRun)
	ON_WM_DESTROY()

	ON_COMMAND(ID_THEME_VISIO, OnThemeVisio)
	ON_COMMAND(ID_THEME_VISIO_OFFICE2000, OnThemeVisio2000)
	ON_COMMAND(ID_THEME_VISIO_OFFICE2003, OnThemeVisio2003)
	ON_COMMAND(ID_THEME_CUSTOM, OnThemeCustom)
	ON_COMMAND(ID_THEME_CUSTOM2, OnThemeCustom2)
	ON_COMMAND(ID_THEME_CUSTOM3, OnThemeCustom3)
	ON_COMMAND(ID_THEME_CUSTOM4, OnThemeCustom4)


	ON_COMMAND(ID_LAYOUT_LOADFROMFILE, OnLoadFromFile)
	ON_COMMAND(ID_LAYOUT_SAVETOFILE, OnSaveToFile)
	ON_COMMAND(ID_LAYOUT_LOADFROMRESOURCE, OnLoadFromResource)

	ON_COMMAND(ID_DOCKINGPANE_HIDECLIENT, OnHideClient)
	ON_UPDATE_COMMAND_UI(ID_DOCKINGPANE_HIDECLIENT, OnUpdateHideClient)

	ON_COMMAND(ID_DOCKINGPANE_USESPLITTERTRACKER, OnUseSplitterTracker)
	ON_UPDATE_COMMAND_UI(ID_DOCKINGPANE_USESPLITTERTRACKER, OnUpdateUseSplitterTracker)

	ON_COMMAND(ID_DOCKINGPANE_THEMEDFLOATINGFRAMES, OnThemedFloatingFrames)
	ON_UPDATE_COMMAND_UI(ID_DOCKINGPANE_THEMEDFLOATINGFRAMES, OnUpdateThemedFloatingFrames)

	ON_COMMAND(ID_DOCKINGCONTEXT_ALPHACONTEXT, OnAlphaContext)
	ON_UPDATE_COMMAND_UI(ID_DOCKINGCONTEXT_ALPHACONTEXT, OnUpdateAlphaContext)

	ON_COMMAND(ID_DOCKINGCONTEXT_DOCKINGSTICKERS, OnDockingStickers)
	ON_UPDATE_COMMAND_UI(ID_DOCKINGCONTEXT_DOCKINGSTICKERS, OnUpdateDockingStickers)

	ON_COMMAND(ID_DOCKINGPANE_MINMAXSAMPLE, OnMinMaxSamlpe)
	ON_UPDATE_COMMAND_UI(ID_DOCKINGPANE_MINMAXSAMPLE, OnUpdateMinMaxSamlpe)
	ON_WM_GETMINMAXINFO()

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


// CMainFrame construction/destruction

CMainFrame::CMainFrame()
: m_pEditorLayout(NULL)
, m_pRunLayout(NULL)
{
	m_themeSelected = xtpPaneThemeCustom;
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	LOGFONT lfIcon;
	VERIFY( ::SystemParametersInfo( SPI_GETICONTITLELOGFONT, sizeof( lfIcon ), &lfIcon, 0 ) );
	m_fntEdit.CreateFontIndirect(&lfIcon);


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

	// Initialize the docking pane manager and set the
	// initial them for the docking panes.  Do this only after all
	// control bars objects have been created and docked.
	GetDockingPaneManager()->InstallDockingPanes(this);
	SetDockingPaneTheme(xtpPaneThemeOffice2003);
	GetDockingPaneManager()->SetFloatingFrameCaption(_T("Panes"));
	m_paneManager.SetThemedFloatingFrames(TRUE);

	// Create docking panes.
	CXTPDockingPane* pwndPane1 = GetDockingPaneManager()->CreatePane(
		IDR_PANE1, CRect(0, 0,200, 120), xtpPaneDockTop);
	GetDockingPaneManager()->CreatePane(
		IDR_PANE2, CRect(0, 0,200, 120), xtpPaneDockLeft, pwndPane1);
	CXTPDockingPane* pwndPane3 = GetDockingPaneManager()->CreatePane(
		IDR_PANE3, CRect(0, 0,200, 120), xtpPaneDockLeft);
	CXTPDockingPane* pwndPane4 = GetDockingPaneManager()->CreatePane(
		IDR_PANE4, CRect(0, 0,200, 120), xtpPaneDockTop, pwndPane3);
	CXTPDockingPane* pwndPane5 = GetDockingPaneManager()->CreatePane(
		IDR_PANE5, CRect(0, 0,200, 120), xtpPaneDockLeft, pwndPane4);
	CXTPDockingPane* pwndPane6 = GetDockingPaneManager()->CreatePane(
		IDR_PANE6, CRect(0, 0,200, 120), xtpPaneDockTop, pwndPane5);
	CXTPDockingPane* pwndPane7 = GetDockingPaneManager()->CreatePane(
		IDR_PANE7, CRect(0, 0,200, 120), xtpPaneDockTop, pwndPane6);
	CXTPDockingPane* pwndPane8 = GetDockingPaneManager()->CreatePane(
		IDR_PANE8, CRect(0, 0,200, 120), xtpPaneDockRight);
	CXTPDockingPane* pwndPane9 = GetDockingPaneManager()->CreatePane(
		IDR_PANE9, CRect(0, 0,200, 120), xtpPaneDockBottom, pwndPane8);
	CXTPDockingPane* pwndPane10 = GetDockingPaneManager()->CreatePane(
		IDR_PANE10, CRect(0, 0,200, 120), xtpPaneDockBottom, pwndPane9);

	// Create the layout manager for run mode.  Layouts define how
	// the panes will appear when docked, you can define multiple
	// layouts.

	// If "RunLayout" layout does not exist, save the current configuration
	// as "RunLayout", otherwise load previous state.
	m_pRunLayout = GetDockingPaneManager()->CreateLayout();
	if (!m_pRunLayout->Load(_T("RunLayout"))) {
		GetDockingPaneManager()->GetLayout(m_pRunLayout);
	}

	// Initialize the default layout for "EditorLayout".
	GetDockingPaneManager()->AttachPane(
		pwndPane9, pwndPane8);
	GetDockingPaneManager()->AttachPane(
		pwndPane10, pwndPane8);
	GetDockingPaneManager()->AttachPane(
		pwndPane6, pwndPane1);
	GetDockingPaneManager()->AttachPane(
		pwndPane4, pwndPane7);
	GetDockingPaneManager()->HidePane(
		pwndPane7);

	// If "EditorLayout" layout does not exist, save the current configuration
	// as "EditorLayout", otherwise load previous state and set it as the active
	// layout.
	m_pEditorLayout = GetDockingPaneManager()->CreateLayout();

	if (m_pEditorLayout->Load(_T("EditorLayout"))) {
		GetDockingPaneManager()->SetLayout(m_pEditorLayout);
	}
	else {
		GetDockingPaneManager()->GetLayout(m_pEditorLayout);
	}

	// Get a pointer to IDR_PANE1 and set the title.
	CXTPDockingPane* pPane = GetDockingPaneManager()->FindPane(IDR_PANE1);
	ASSERT(pPane);
	if (pPane) {
		pPane->SetTitle(_T("Pane 1 - Long Title\nPane 1"));
	}

	// Get a pointer to IDR_PANE2 and set the title.
	pPane = GetDockingPaneManager()->FindPane(IDR_PANE2);
	ASSERT(pPane);
	if (pPane) {
		pPane->SetTitle(_T("Pane 2 - No Closeable\nPane 2"));
		pPane->SetOptions(xtpPaneNoCloseable);
	}

	// Get a pointer to IDR_PANE3 and set the title.
	pPane = GetDockingPaneManager()->FindPane(IDR_PANE3);
	ASSERT(pPane);
	if (pPane) {
		pPane->SetTitle(_T("Pane 3 - No Hideable\nPane 3"));
		pPane->SetOptions(xtpPaneNoHideable);
	}


	m_bEditor = TRUE;

	// Set the icons for the docking pane tabs.
	int nIDs1[] = {IDR_PANE1, IDR_PANE2, IDR_PANE3, IDR_PANE4, IDR_PANE5};
	GetDockingPaneManager()->SetIcons(IDB_BITMAP1, nIDs1, 5, RGB(0, 255, 0));

	int nIDs2[] = {IDR_PANE6, IDR_PANE7, IDR_PANE8, IDR_PANE9, IDR_PANE10};
	GetDockingPaneManager()->SetIcons(IDB_BITMAP2, nIDs2, 5, RGB(0, 255, 0));

	OnThemeOffice2003();
	
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
			CEditPane& wndEdit = m_wndPanes[nIndex];

			// create and attach the edit control for this pane.
			if (!::IsWindow(wndEdit.m_hWnd))
			{
				if (!wndEdit.CreateEx(WS_EX_STATICEDGE, _T("EDIT"), _T(""),
					WS_CHILD|ES_AUTOVSCROLL|ES_MULTILINE, CRect(0, 0,200, 120), this, 0))
				{
					TRACE0( "Error creating pane edit control.\n" );
				}
				wndEdit.SetFont(&m_fntEdit);

				AdjustStyle(GetDockingPaneManager()->GetCurrentTheme());
			}

			pPane->Attach(&wndEdit);
			wndEdit.m_pPane = pPane;
		}

		return TRUE; // handled
	}
	if (wParam == XTP_DPN_RCLICK)
	{
		CPoint pos;
		GetCursorPos(&pos);

		CMenu menu;
		VERIFY(menu.LoadMenu(IDR_POPUP_PANES));
		CMenu* pPopup = menu.GetSubMenu(0);
		TrackPopupMenu(pPopup->GetSafeHmenu(), TPM_RIGHTBUTTON, pos.x, pos.y, 0, AfxGetMainWnd()->GetSafeHwnd(), 0);

		return TRUE;
	}

	return FALSE;
}

void CMainFrame::AdjustStyle(XTPDockingPanePaintTheme theme)
{
	BOOL bBorder = FALSE, bStaticEdge = FALSE, bClientEdge = FALSE;
	switch(theme)
	{
		case xtpPaneThemeOffice: bStaticEdge = TRUE;    break;
		case xtpPaneThemeDefault: bBorder = TRUE; break;
		case xtpPaneThemeGrippered: bClientEdge = TRUE; break;
	}

	for (int i = 0; i < sizeof(m_wndPanes) / sizeof(m_wndPanes[0]); i++)
	{
		if (!m_wndPanes[i].m_hWnd)
		{
			if (!m_wndPanes[i].CreateEx(WS_EX_STATICEDGE, _T("EDIT"), _T(""),
				WS_CHILD|ES_AUTOVSCROLL|ES_MULTILINE, CRect(0, 0,0, 0), this, 0))
			{
				TRACE0( "Error creating pane edit control.\n" );
			}
			m_wndPanes[i].SetFont(&m_fntEdit);

		}
		m_wndPanes[i].ModifyStyle(bBorder? 0: WS_BORDER, bBorder? WS_BORDER: 0, SWP_FRAMECHANGED|SWP_DRAWFRAME);
		m_wndPanes[i].ModifyStyleEx(bStaticEdge? 0: WS_EX_STATICEDGE, bStaticEdge? WS_EX_STATICEDGE: 0, SWP_FRAMECHANGED|SWP_DRAWFRAME );
		m_wndPanes[i].ModifyStyleEx(bClientEdge? 0: WS_EX_CLIENTEDGE, bClientEdge? WS_EX_CLIENTEDGE: 0, SWP_FRAMECHANGED|SWP_DRAWFRAME );
	}
}
// CMainFrame message handlers

void CMainFrame::OnShowPane(UINT nID)
{
	GetDockingPaneManager()->ShowPane(nID);
}


void CMainFrame::SwitchLayout()
{
	if (m_bMinMaxSample)
		OnMinMaxSamlpe();

	if (m_bEditor)
	{
		GetDockingPaneManager()->GetLayout(m_pEditorLayout);
		GetDockingPaneManager()->SetLayout(m_pRunLayout);
	} else
	{
		GetDockingPaneManager()->GetLayout(m_pRunLayout);
		GetDockingPaneManager()->SetLayout(m_pEditorLayout);
	}
	m_bEditor = !m_bEditor;
}

void CMainFrame::OnLayoutEditor()
{
	if (!m_bEditor) SwitchLayout();

}

void CMainFrame::OnLayoutRun()
{
	if (m_bEditor) SwitchLayout();
}

void CMainFrame::OnUpdateLayoutEditor(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_bEditor);
}

void CMainFrame::OnUpdateLayoutRun(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(!m_bEditor);
}

void CMainFrame::OnDestroy()
{
	if (m_bMinMaxSample)
	{
		OnMinMaxSamlpe();
	}

	CMDIFrameWnd::OnDestroy();

	GetDockingPaneManager()->GetLayout(m_bEditor? m_pEditorLayout: m_pRunLayout);

	m_pEditorLayout->Save(_T("EditorLayout"));
	m_pRunLayout->Save(_T("RunLayout"));


	delete m_pEditorLayout;
	delete m_pRunLayout;
}

void CMainFrame::OnThemeDefault()
{
	AdjustStyle(xtpPaneThemeDefault);
	SetDockingPaneTheme(xtpPaneThemeDefault);

#ifdef _XTP_INCLUDE_COMMANDBARS
	CXTPPaintManager::SetTheme(xtpThemeOffice2000);
	GetCommandBars()->RedrawCommandBars();
#endif
}
void CMainFrame::OnThemeOffice()
{
	AdjustStyle(xtpPaneThemeOffice);
	SetDockingPaneTheme(xtpPaneThemeOffice);

#ifdef _XTP_INCLUDE_COMMANDBARS
	CXTPPaintManager::SetTheme(xtpThemeOfficeXP);
	GetCommandBars()->RedrawCommandBars();
#endif

}

void CMainFrame::OnThemeWhidbey()
{
	AdjustStyle(xtpPaneThemeOffice);
	SetDockingPaneTheme(xtpPaneThemeWhidbey);

#ifdef _XTP_INCLUDE_COMMANDBARS
	CXTPPaintManager::SetTheme(xtpThemeWhidbey);
	GetCommandBars()->RedrawCommandBars();
#endif

}

void CMainFrame::OnThemeExplorer()
{
	AdjustStyle(xtpPaneThemeGrippered);
	SetDockingPaneTheme(xtpPaneThemeExplorer);

#ifdef _XTP_INCLUDE_COMMANDBARS
	CXTPPaintManager::SetTheme(xtpThemeNativeWinXP);
	GetCommandBars()->RedrawCommandBars();
#endif

}
void CMainFrame::OnThemeOffice2003()
{
	AdjustStyle(xtpPaneThemeOffice);
	SetDockingPaneTheme(xtpPaneThemeOffice2003);

#ifdef _XTP_INCLUDE_COMMANDBARS
	CXTPPaintManager::SetTheme(xtpThemeOffice2003);
	GetCommandBars()->RedrawCommandBars();
#endif

}

void CMainFrame::SetDockingPaneTheme(XTPDockingPanePaintTheme theme)
{
	GetDockingPaneManager()->SetTheme(theme);
	m_themeSelected = theme;
}

void CMainFrame::OnThemeShortcutBar2003()
{
	AdjustStyle(xtpPaneThemeOffice);
	SetDockingPaneTheme(xtpPaneThemeShortcutBar2003);
	

#ifdef _XTP_INCLUDE_COMMANDBARS
	CXTPPaintManager::SetTheme(xtpThemeOffice2003);
	GetCommandBars()->RedrawCommandBars();
#endif	
}

void CMainFrame::OnThemeWinXP()
{
	AdjustStyle(xtpPaneThemeGrippered);
	SetDockingPaneTheme(xtpPaneThemeNativeWinXP);

#ifdef _XTP_INCLUDE_COMMANDBARS
	CXTPPaintManager::SetTheme(xtpThemeNativeWinXP);
	GetCommandBars()->RedrawCommandBars();
#endif

}



void CMainFrame::OnThemeGrippered()
{
	AdjustStyle(xtpPaneThemeGrippered);
	SetDockingPaneTheme(xtpPaneThemeGrippered);
	GetDockingPaneManager()->GetPaintManager()->GetTabPaintManager()->m_clientFrame = xtpTabFrameBorder;

#ifdef _XTP_INCLUDE_COMMANDBARS
	CXTPPaintManager::SetCustomTheme(new CDoubleGripperTheme);
	GetCommandBars()->RedrawCommandBars();
#endif

}

void CMainFrame::OnThemeVisio()
{
	AdjustStyle(xtpPaneThemeVisio);
	SetDockingPaneTheme(xtpPaneThemeVisio);
#ifdef _XTP_INCLUDE_COMMANDBARS
	CXTPPaintManager::SetTheme(xtpThemeOfficeXP);
	GetCommandBars()->RedrawCommandBars();
#endif

}

void CMainFrame::OnThemeVisio2003()
{
	AdjustStyle(xtpPaneThemeVisio);
	SetDockingPaneTheme(xtpPaneThemeOffice2003);
	GetDockingPaneManager()->GetPaintManager()->GetTabPaintManager()->SetAppearance(xtpTabAppearanceVisio);
	GetDockingPaneManager()->GetPaintManager()->GetTabPaintManager()->SetColor(xtpTabColorOffice2003);
	GetDockingPaneManager()->GetPaintManager()->GetTabPaintManager()->m_bHotTracking = TRUE;

	GetDockingPaneManager()->GetPaintManager()->GetPanelPaintManager()->SetColor(xtpTabColorOffice2003);


#ifdef _XTP_INCLUDE_COMMANDBARS
	CXTPPaintManager::SetTheme(xtpThemeOffice2003);
	GetCommandBars()->RedrawCommandBars();
#endif

}

void CMainFrame::OnThemeVisio2000()
{
	SetDockingPaneTheme(xtpPaneThemeDefault);
	GetDockingPaneManager()->GetPaintManager()->GetTabPaintManager()->SetAppearance(xtpTabAppearanceVisio);

	AdjustStyle(xtpPaneThemeVisio);

#ifdef _XTP_INCLUDE_COMMANDBARS
	CXTPPaintManager::SetTheme(xtpThemeOffice2000);
	GetCommandBars()->RedrawCommandBars();
#endif
}


void CMainFrame::OnThemeCustom()
{
	AdjustStyle(xtpPaneThemeCustom);
	GetDockingPaneManager()->SetCustomTheme(new CCustomTheme());

#ifdef _XTP_INCLUDE_COMMANDBARS
	CXTPPaintManager::SetTheme(xtpThemeOfficeXP);
	GetCommandBars()->RedrawCommandBars();
#endif

}

void CMainFrame::OnThemeCustom2()
{
	AdjustStyle(xtpPaneThemeGrippered);
	GetDockingPaneManager()->SetCustomTheme(new CXTPDockingPaneDefaultTheme());
	CXTPTabPaintManager* pTabPaintManager = GetDockingPaneManager()->GetPaintManager()->GetTabPaintManager();

	pTabPaintManager->SetPosition(xtpTabPositionTop);
	pTabPaintManager->m_clientFrame = xtpTabFrameBorder;


#ifdef _XTP_INCLUDE_COMMANDBARS
	CXTPPaintManager::SetCustomTheme(new CDoubleGripperTheme);
	GetCommandBars()->RedrawCommandBars();
#endif

}

void CMainFrame::OnThemeCustom3()
{
	AdjustStyle(xtpPaneThemeOffice);

	GetDockingPaneManager()->SetCustomTheme(new CXTPDockingPaneOfficeTheme());
	GetDockingPaneManager()->GetPaintManager()->GetTabPaintManager()->SetAppearance(xtpTabAppearanceStateButtons);
	GetDockingPaneManager()->GetPaintManager()->GetPanelPaintManager()->SetAppearance(xtpTabAppearanceStateButtons);

#ifdef _XTP_INCLUDE_COMMANDBARS
	CXTPPaintManager::SetTheme(xtpThemeOfficeXP);
	GetCommandBars()->RedrawCommandBars();
#endif

}

void CMainFrame::OnThemeCustom4()
{
	AdjustStyle(xtpPaneThemeVisio);
	GetDockingPaneManager()->SetCustomTheme(new CXTPDockingPaneOfficeTheme());

	CXTPTabPaintManager* pTabPaintManager = GetDockingPaneManager()->GetPaintManager()->GetTabPaintManager();
	pTabPaintManager->SetAppearance(xtpTabAppearanceExcel);
	pTabPaintManager->m_clientFrame = xtpTabFrameBorder;

	pTabPaintManager = GetDockingPaneManager()->GetPaintManager()->GetPanelPaintManager();
	pTabPaintManager->SetAppearance(xtpTabAppearanceExcel);


#ifdef _XTP_INCLUDE_COMMANDBARS
	CXTPPaintManager::SetTheme(xtpThemeOfficeXP);
	GetCommandBars()->RedrawCommandBars();
#endif

}

void CMainFrame::OnUpdateTheme(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_themeSelected == XTPDockingPanePaintTheme(pCmdUI->m_nID - ID_THEME_DEFAULT));
}


void CMainFrame::OnHideClient()
{
	GetDockingPaneManager()->HideClient(!GetDockingPaneManager()->IsClientHidden());
}

void CMainFrame::OnUpdateHideClient(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(GetDockingPaneManager()->IsClientHidden()? TRUE: FALSE);
}

void CMainFrame::OnUseSplitterTracker()
{
	GetDockingPaneManager()->UseSplitterTracker(!GetDockingPaneManager()->IsSplitterTrackerUsed());
}

void CMainFrame::OnUpdateUseSplitterTracker(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(GetDockingPaneManager()->IsSplitterTrackerUsed()? TRUE: FALSE);
}

void CMainFrame::OnThemedFloatingFrames()
{
	m_paneManager.SetThemedFloatingFrames(!m_paneManager.IsThemedFloatingFrames());
}
void CMainFrame::OnUpdateThemedFloatingFrames(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_paneManager.IsThemedFloatingFrames()? TRUE: FALSE);
}

void CMainFrame::OnAlphaContext()
{
	m_paneManager.SetAlphaDockingContext(!m_paneManager.IsAlphaDockingContext());

}
void CMainFrame::OnUpdateAlphaContext(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_paneManager.IsAlphaDockingContext()? TRUE: FALSE);
}

void CMainFrame::OnDockingStickers()
{
	m_paneManager.SetShowDockingContextStickers(!m_paneManager.IsShowDockingContextStickers());
}
void CMainFrame::OnUpdateDockingStickers(CCmdUI *pCmdUI)
{
	if (m_paneManager.IsAlphaDockingContext())
	{
		pCmdUI->SetCheck(m_paneManager.IsShowDockingContextStickers()? TRUE: FALSE);
	}
	else
	{
		pCmdUI->Enable(FALSE);
	}
}

void CMainFrame::OnMinMaxSamlpe()
{
	m_bMinMaxSample = !m_bMinMaxSample;

	if (m_bMinMaxSample)
	{
		CXTPDockingPane* pPane = m_paneManager.FindPane(IDR_PANE1);
		ASSERT(pPane);
		pPane->SetMinTrackSize(CSize(100, 100));
		pPane->SetMaxTrackSize(CSize(150, 150));

		pPane = m_paneManager.FindPane(IDR_PANE2);
		ASSERT(pPane);
		pPane->SetMinTrackSize(CSize(130, 130));
		pPane->SetMaxTrackSize(CSize(130, 130));
	}
	else
	{
		CXTPDockingPane* pPane = m_paneManager.FindPane(IDR_PANE1);
		ASSERT(pPane);
		pPane->SetMinTrackSize(CSize(0, 0));
		pPane->SetMaxTrackSize(CSize(32000, 32000));

		pPane = m_paneManager.FindPane(IDR_PANE2);
		ASSERT(pPane);
		pPane->SetMinTrackSize(CSize(0, 0));
		pPane->SetMaxTrackSize(CSize(32000, 32000));
	}

	for (int i = 0; i < sizeof(m_wndPanes) / sizeof(m_wndPanes[0]); i++)
	{
		if (m_wndPanes[i].m_hWnd)
		{
			m_wndPanes[i].OnSize(0, 0, 0);
			m_wndPanes[i].RedrawWindow();

		}
	}
	GetDockingPaneManager()->RedrawPanes();
}

void CMainFrame::OnUpdateMinMaxSamlpe(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_bMinMaxSample);
}





void CMainFrame::OnLoadFromFile()
{
	CXTPDockingPaneLayout* pLayout = m_bEditor? m_pEditorLayout: m_pRunLayout;

	CString strFilter = _T("XML Document(*.xml)|*.xml|All files (*.*)|*.*||");
	CFileDialog fd(TRUE, _T("xml"), NULL, OFN_HIDEREADONLY|OFN_FILEMUSTEXIST, strFilter);
	if (fd.DoModal() == IDOK)
	{
#ifndef _XTP_EXCLUDE_XML
		if (pLayout->LoadFromFile(fd.GetPathName(), _T("Common")))
		{
			GetDockingPaneManager()->SetLayout(pLayout);
		}
#else
		CFile file(fd.GetPathName(), CFile::modeRead);
		CArchive ar(&file, CArchive::load);

		pLayout->Serialize(ar);
		if (pLayout->IsValid())
		{
			GetDockingPaneManager()->SetLayout(pLayout);
		}

#endif
	}
}

void CMainFrame::OnSaveToFile()
{
	CXTPDockingPaneLayout* pLayout = m_bEditor? m_pEditorLayout: m_pRunLayout;

	CString strFilter = _T("XML Document(*.xml)|*.xml|All files (*.*)|*.*||");
	CFileDialog fd(FALSE, _T("xml"), NULL, OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, strFilter);
	if (fd.DoModal() == IDOK)
	{
#ifndef _XTP_EXCLUDE_XML

		GetDockingPaneManager()->GetLayout(pLayout);
		pLayout->SaveToFile(fd.GetPathName(), _T("Common"));
#else
		GetDockingPaneManager()->GetLayout(pLayout);
		CFile file(fd.GetPathName(), CFile::modeCreate|CFile::modeWrite);
		CArchive ar(&file, CArchive::store);

		pLayout->Serialize(ar);

#endif
	}

}

void CMainFrame::OnLoadFromResource()
{

#ifndef _XTP_EXCLUDE_XML

	LPCTSTR lpszResourceName = MAKEINTRESOURCE(IDR_SPIRAL);

	HINSTANCE hInst = AfxGetInstanceHandle();
	HRSRC hRsrc = ::FindResource(hInst, lpszResourceName, RT_HTML);
	if (hRsrc == NULL)
		return ;

	HGLOBAL hGlobal = LoadResource(hInst, hRsrc);
	if (hGlobal == NULL)
		return ;

	LPCSTR pData = (LPCSTR)LockResource(hGlobal);
	if (pData == NULL)
		return ;


	CXTPDOMDocumentPtr xmlDocPtr = 0;
	CXTPDOMElementPtr xmlRootPtr = 0;

	VERIFY(SUCCEEDED(xmlDocPtr.CreateInstance(CLSID_XTPDOMDocument)));

	VARIANT_BOOL bResult;
	HRESULT hr = xmlDocPtr->loadXML(_bstr_t(pData), &bResult);
	if (SUCCEEDED(hr) && (bResult != VARIANT_FALSE))
	{

		xmlDocPtr->get_documentElement(&xmlRootPtr);
		VERIFY(xmlRootPtr != 0);

		CXTPDockingPaneLayout* pLayout = m_bEditor? m_pEditorLayout: m_pRunLayout;

		if (pLayout->LoadFromNode(xmlRootPtr, _T("Common")))
		{
			GetDockingPaneManager()->SetLayout(pLayout);
		}
	}

	UnlockResource(hGlobal);
	FreeResource(hGlobal);

#endif

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
		pCommands->AddCategories(IDR_PaneTYPE);

		// Use the command bar manager to initialize the
		// customize dialog.
		pCommands->InsertAllCommandsCategory();
		pCommands->InsertBuiltInMenus(IDR_PaneTYPE);
		pCommands->InsertNewMenuCategory();

		// Dispaly the dialog.
		dlg.DoModal();
	}
}

void CMainFrame::OnClose()
{
	// Save the current state for toolbars and menus.
	SaveCommandBars(_T("CommandBars"));
	CMDIFrameWnd::OnClose();
}

#endif


void CMainFrame::OnGetMinMaxInfo(LPMINMAXINFO pMinMaxInfo)
{
	CXTPMDIFrameWnd::OnGetMinMaxInfo(pMinMaxInfo);

	if (m_bMinMaxSample)
	{
		CXTPWindowRect rcWindow(this);

		CRect rcTopPane= m_paneManager.GetTopPane()->GetWindowRect();

		CSize szBorder = rcWindow.Size() - rcTopPane.Size();

		MINMAXINFO mmi;
		m_paneManager.GetTopPane()->GetMinMaxInfo(&mmi);

		pMinMaxInfo->ptMinTrackSize = CPoint(mmi.ptMinTrackSize) + szBorder;
	}
}


//////////////////////////////////////////////////////////////////////////
// CEditPane

CEditPane::CEditPane()
{
	m_pPane = NULL;
}

LRESULT CEditPane::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == WM_NCPAINT && GetExStyle() & WS_EX_STATICEDGE)
	{
		CRect rc;
		GetWindowRect(&rc);
		CWindowDC dc(this);
		rc.OffsetRect(-rc.TopLeft());
		dc.Draw3dRect(rc, GetSysColor(XPCOLOR_3DSHADOW), GetSysColor(XPCOLOR_3DSHADOW));
		return TRUE;
	}
	return CXTPEdit::WindowProc(message, wParam, lParam);
}


BEGIN_MESSAGE_MAP(CEditPane, CXTPEdit)
	ON_WM_CTLCOLOR_REFLECT()
	ON_WM_SIZE()
END_MESSAGE_MAP()

HBRUSH CEditPane::CtlColor(CDC* pDC, UINT /*nCtlColor*/)
{
	static CBrush brush(RGB(0xFF, 230, 191));

	pDC->SetBkMode(TRANSPARENT);

	if (m_pPane && m_bMinMaxSample)
	{
		MINMAXINFO mmi;
		m_pPane->GetMinMaxInfo(&mmi);

		if (mmi.ptMinTrackSize.x > 0)
		{
			return brush;
		}
	}
	return GetSysColorBrush(COLOR_WINDOW);
}

void CEditPane::OnSize(UINT nType, int cx, int cy)
{
	CEdit::OnSize(nType, cx, cy);

	if (m_pPane && (m_bMinMaxSample || (cx == 0 && cy == 0)))
	{
		MINMAXINFO mmi;
		m_pPane->GetMinMaxInfo(&mmi);

		CString strInfo;

		if (mmi.ptMinTrackSize.x > 0)
		{
			CXTPWindowRect rc(this);

			strInfo.Format(_T("Tracking Size:\r\nMin (%i, %i)\r\nMax (%i, %i)\r\nCurrent (%i, %i)"),
				mmi.ptMinTrackSize.x, mmi.ptMinTrackSize.y, mmi.ptMaxTrackSize.x, mmi.ptMaxTrackSize.y, rc.Width(), rc.Height());
		}
		SetWindowText(strInfo);
	}
}


void CMainFrame::OnSetPreviewMode(BOOL bPreview, CPrintPreviewState* pState)
{

#ifdef _XTP_INCLUDE_COMMANDBARS
	// Toggle CommandBars
	GetCommandBars()->OnSetPreviewMode(bPreview);
#endif


// Toggle Docking Panes.
	m_paneManager.OnSetPreviewMode(bPreview);


	CMDIFrameWnd::OnSetPreviewMode(bPreview, pState);
}