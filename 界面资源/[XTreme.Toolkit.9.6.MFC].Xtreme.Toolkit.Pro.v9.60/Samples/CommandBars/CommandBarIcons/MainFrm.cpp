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
#include "CommandBarIcons.h"

#include "MainFrm.h"

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
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
	ON_COMMAND(XTP_ID_CUSTOMIZE, OnCustomize)

	ON_COMMAND(ID_PROJECT_ADDFILES, OnEmptyCommand)
	ON_COMMAND(ID_PROJECT_IMPORTFILES, OnEmptyCommand)
	ON_COMMAND(ID_PROJECT_EXPORTFILES, OnEmptyCommand)
	ON_COMMAND(ID_PROJECT_UPDATEALL, OnEmptyCommand)
	ON_COMMAND(ID_PROJECT_IMPORTFILES, OnEmptyCommand)
	ON_COMMAND(ID_PROJECT_BUILD, OnEmptyCommand)
	ON_COMMAND(ID_PROJECT_BUILDALL, OnEmptyCommand)
	ON_COMMAND(ID_PROJECT_CHECK, OnEmptyCommand)
	ON_COMMAND(ID_BUTTON_ADD, OnEmptyCommand)
	ON_COMMAND(ID_BUTTON_DELETE, OnEmptyCommand)

	ON_COMMAND(ID_FILE_NEW_ARCHIVE, OnEmptyCommand)
	ON_COMMAND(ID_FILE_OPEN_ARCHIVE, OnEmptyCommand)
	ON_COMMAND(ID_FILE_FAVORITES, OnEmptyCommand)
	ON_COMMAND(ID_FILE_CLOSE_ARCHIVE, OnEmptyCommand)
	ON_COMMAND(ID_FILE_WIZARD, OnEmptyCommand)
	ON_COMMAND(ID_FILE_PROPERTIES, OnEmptyCommand)

	ON_COMMAND(ID_THEME_OFFICE2000, OnSwitchOffice2000)
	ON_COMMAND(ID_THEME_OFFICEXP, OnSwitchOfficeXP)
	ON_COMMAND(ID_THEME_OFFICE2003, OnSwitchOffice2003)
	ON_COMMAND(ID_THEME_NATIVEXP, OnSwitchNativeXP)

	ON_UPDATE_COMMAND_UI(ID_THEME_OFFICE2000, OnUpdateSwitchOffice2000)
	ON_UPDATE_COMMAND_UI(ID_THEME_OFFICEXP, OnUpdateSwitchOfficeXP)
	ON_UPDATE_COMMAND_UI(ID_THEME_OFFICE2003, OnUpdateSwitchOffice2003)
	ON_UPDATE_COMMAND_UI(ID_THEME_NATIVEXP, OnUpdateSwitchNativeXP)

	ON_COMMAND(ID_PROPERTIES, OnProperties)
	ON_UPDATE_COMMAND_UI(ID_PROPERTIES, OnUpdateProperties)

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

}

CMainFrame::~CMainFrame()
{
}

void CMainFrame::OnEmptyCommand()
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

	CXTPPaintManager::SetTheme(xtpThemeNativeWinXP);
	//CXTPPaintManager::SetTheme(xtpThemeOfficeXP);

	XTP_COMMANDBARS_ICONSINFO* pIconsInfo = XTPPaintManager()->GetIconsInfo();
	pIconsInfo->bUseDisabledIcons = TRUE;
	pIconsInfo->bIconsWithShadow = FALSE;

	CXTPCommandBars* pCommandBars = GetCommandBars();

	CXTPCommandBar* pMenuBar = pCommandBars->SetMenu(_T("Menu Bar"), IDR_MAINFRAME);
	pMenuBar->SetFlags(xtpFlagIgnoreSetMenuMessage);

	CXTPToolBar* pStandardBar = (CXTPToolBar*)pCommandBars->Add(_T("Standard"), xtpBarTop);
	if (!pStandardBar ||
		!pStandardBar->LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;
	}
	CXTPToolBar* pThemesBar = (CXTPToolBar*)pCommandBars->Add(_T("Themes Bar"), xtpBarTop);
	if (!pThemesBar ||
		!pThemesBar->LoadToolBar(IDR_TOOLBAR_THEMES))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;
	}

	XTPImageManager()->SetIcons(IDR_MENU_ICONS);
	XTPImageManager()->SetIcons(IDR_MAINFRAME_LARGE);

	m_wndProperties.Create(IDD_DIALOG_PROPERTIES, this);
	m_wndProperties.CenterWindow(this);
	m_wndProperties.ShowWindow(SW_SHOW);

	// Load the previous state for command bars.
	LoadCommandBars(_T("CommandBars"));

	CXTPCommandBarsOptions* pOptions = pCommandBars->GetCommandBarsOptions();
	pOptions->szLargeIcons = CSize (24, 24);
	pOptions->bLargeIcons = FALSE;

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
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
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers.

void CMainFrame::OnProperties()
{
	if (m_wndProperties.GetSafeHwnd() && m_wndProperties.IsWindowVisible())
	{
		m_wndProperties.ShowWindow(SW_HIDE);
	} 
	else
	{
		if (!m_wndProperties.GetSafeHwnd())
		{
			m_wndProperties.Create(IDD_DIALOG_PROPERTIES, this);
			m_wndProperties.UpdateAll();
		}
		m_wndProperties.ShowWindow(SW_SHOW);
	}
}

void CMainFrame::OnUpdateProperties(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_wndProperties.GetSafeHwnd() && m_wndProperties.IsWindowVisible());
}

void CMainFrame::SwitchTheme(XTPPaintTheme paintTheme)
{
	CXTPPaintManager::SetTheme(paintTheme);

	XTP_COMMANDBARS_ICONSINFO* pIconsInfo = XTPPaintManager()->GetIconsInfo();
	pIconsInfo->bUseDisabledIcons = TRUE;

	RecalcLayout(FALSE);
	GetCommandBars()->RedrawCommandBars();
	RedrawWindow();

	if (m_wndProperties.GetSafeHwnd() && m_wndProperties.IsWindowVisible())
	{
		m_wndProperties.UpdateAll();
	}
}

void CMainFrame::OnSwitchOffice2000()
{
	SwitchTheme(xtpThemeOffice2000);
}

void CMainFrame::OnSwitchOfficeXP()
{
	SwitchTheme(xtpThemeOfficeXP);
}

void CMainFrame::OnSwitchOffice2003()
{
	SwitchTheme(xtpThemeOffice2003);
}

void CMainFrame::OnSwitchNativeXP()
{
	SwitchTheme(xtpThemeNativeWinXP);
}

void CMainFrame::OnUpdateSwitchOffice2000(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(XTPPaintManager()->GetCurrentTheme() == xtpThemeOffice2000);
}

void CMainFrame::OnUpdateSwitchOfficeXP(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(XTPPaintManager()->GetCurrentTheme() == xtpThemeOfficeXP);
}

void CMainFrame::OnUpdateSwitchOffice2003(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(XTPPaintManager()->GetCurrentTheme() == xtpThemeOffice2003);
}

void CMainFrame::OnUpdateSwitchNativeXP(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(XTPPaintManager()->GetCurrentTheme() == xtpThemeNativeWinXP);
}

int CMainFrame::OnCreateControl(LPCREATECONTROLSTRUCT lpCreateControl)
{
	if (lpCreateControl->bToolBar && lpCreateControl->nID >= ID_THEME_OFFICE2000 && lpCreateControl->nID <= ID_THEME_NATIVEXP)
	{
		CXTPControlButton* pButton = (CXTPControlButton*)CXTPControlButton::CreateObject();
		lpCreateControl->pControl = pButton;
		pButton->SetID(lpCreateControl->nID);
		pButton->SetStyle(xtpButtonCaption);
		return TRUE;
	}
	if (lpCreateControl->bToolBar &&
		(lpCreateControl->nID == ID_PROPERTIES || lpCreateControl->nID == ID_BUTTON_ADD || lpCreateControl->nID == ID_BUTTON_DELETE))
	{
		CXTPControlButton* pButton = (CXTPControlButton*)CXTPControlButton::CreateObject();
		lpCreateControl->pControl = pButton;
		pButton->SetID(lpCreateControl->nID);
		pButton->SetStyle(xtpButtonIconAndCaption);
		return TRUE;
	}
	return FALSE;
}

void CMainFrame::OnClose()
{
	// Save the current state for command bars.
	SaveCommandBars(_T("CommandBars"));

	CFrameWnd::OnClose();
}

void CMainFrame::OnCustomize()
{
	// Get a pointer to the command bars object.
	CXTPCommandBars* pCommandBars = GetCommandBars();
	if (pCommandBars == NULL)
		return;

	// Instantiate the customize dialog.
	CXTPCustomizeSheet dlg(pCommandBars);

	// Add the options page to the customize dialog.
	CXTPCustomizeOptionsPage pageOptions(&dlg);
	dlg.AddPage(&pageOptions);

	// Add the commands page to the customize dialog.
	CXTPCustomizeCommandsPage* pPageCommands = dlg.GetCommandsPage();
	pPageCommands->AddCategories(IDR_MAINFRAME);

	// Initialize the commands page page.
	pPageCommands->InsertAllCommandsCategory();
	pPageCommands->InsertBuiltInMenus(IDR_MAINFRAME);
	pPageCommands->InsertNewMenuCategory();

	// Display the customize dialog.
	dlg.DoModal();
}
