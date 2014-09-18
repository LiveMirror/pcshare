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
#include "IntelligentMenus.h"
#include "ModernTheme.h"

#include "MainFrm.h"

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_COMMAND(ID_TOOLS_THEME_CUSTOMTHEME, OnToolsThemeCustomtheme)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_THEME_CUSTOMTHEME, OnUpdateToolsThemeCustomtheme)
	ON_COMMAND(ID_TOOLS_THEME_OFFICE2003THEME, OnToolsThemeOffice2003theme)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_THEME_OFFICE2003THEME, OnUpdateToolsThemeOffice2003theme)
	ON_COMMAND(ID_STOP, OnEmptyCommand)
	ON_COMMAND(ID_REFRESH, OnEmptyCommand)
	ON_COMMAND(ID_HOME, OnEmptyCommand)
	ON_COMMAND(ID_SEARCH, OnEmptyCommand)
	ON_COMMAND(ID_FAVORITES, OnEmptyCommand)
	ON_COMMAND(ID_FONT, OnEmptyCommand)
	ON_COMMAND(ID_FILE_EDIT, OnEmptyCommand)
	ON_COMMAND(ID_PROPERTIES, OnEmptyCommand)
	ON_COMMAND(ID_MUSIC, OnEmptyCommand)
	ON_COMMAND(ID_HISTORY, OnEmptyCommand)
	ON_COMMAND(ID_SENT, OnEmptyCommand)
	ON_COMMAND(ID_FULLSCREEN, OnEmptyCommand)
	ON_COMMAND(ID_DISCUSS, OnEmptyCommand)
	ON_COMMAND(ID_TOOLS_SYNCROMIZE , OnEmptyCommand)
	ON_COMMAND(ID_TOOLS_UPDATE, OnEmptyCommand)
	ON_COMMAND(ID_TOOLS_INTERNETOPTIONS, OnEmptyCommand)
	ON_COMMAND(ID_TOOLS_SHOWTRASHSCAN, OnEmptyCommand)
	ON_COMMAND(XTP_ID_CUSTOMIZE, OnCustomize)
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

static UINT buttons[] =
{
	ID_BACK,
	ID_FORWARD,
	ID_STOP,
	ID_REFRESH,
	ID_HOME,

	ID_SEARCH,
	ID_FAVORITES,
	ID_FILE_PRINT,

	ID_FONT,
	ID_FILE_EDIT,
	ID_PROPERTIES,
	ID_MUSIC,
	ID_HISTORY,
	ID_SENT,
	ID_FULLSCREEN,
	ID_DISCUSS
};

static UINT uHideCmds[] =
{
	ID_FILE_CLOSE,
	ID_FILE_SAVE,
	ID_FILE_SAVE_AS,
	ID_FILE_PRINT,
	ID_FILE_PRINT_PREVIEW,
	ID_FILE_PRINT_SETUP,
	ID_EDIT_CUT,
	ID_EDIT_COPY,
	ID_EDIT_PASTE,
	ID_EDIT_FIND,
	ID_VIEW_SOURCE,
	ID_FULLSCREEN,
	ID_TOOLS_UPDATE,
	ID_TOOLS_SHOWTRASHSCAN
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

	CXTPPaintManager::SetCustomTheme(new CModernTheme());

	XTP_COMMANDBARS_ICONSINFO* pIconsInfo = XTPPaintManager()->GetIconsInfo();
	pIconsInfo->bUseDisabledIcons = TRUE;
	pIconsInfo->bUseFadedIcons = FALSE;


	CXTPCommandBars* pCommandBars = GetCommandBars();

	CXTPCommandBar* pMenuBar = pCommandBars->SetMenu(_T("Menu Bar"), IDR_MAINFRAME);
	pMenuBar->SetFlags(xtpFlagIgnoreSetMenuMessage);


	CXTPToolBar* pStandardBar = (CXTPToolBar*)pCommandBars->Add(_T("Standard"), xtpBarTop);
	if (!pStandardBar ||
		!pStandardBar->LoadToolBar(IDR_TOOLBAR_NORMAL_SMALL))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;
	}

	if (XTPImageManager()->IsAlphaIconsSupported())
	{
		XTPImageManager()->SetIcons(IDR_TOOLBAR_NORMAL_SMALL_ALPHA, buttons, _countof(buttons), CSize(16, 16), xtpImageNormal);
		XTPImageManager()->SetIcons(IDR_TOOLBAR_HOT_SMALL_ALPHA, buttons, _countof(buttons), CSize(16, 16), xtpImageHot);

		XTPImageManager()->SetIcons(IDR_TOOLBAR_NORMAL_LARGE_ALPHA, buttons, _countof(buttons), CSize(24, 24), xtpImageNormal);
		XTPImageManager()->SetIcons(IDR_TOOLBAR_HOT_LARGE_ALPHA, buttons, _countof(buttons), CSize(24, 24), xtpImageHot);
	}
	else
	{
		XTPImageManager()->SetMaskColor(RGB(0, 0, 0));

		XTPImageManager()->SetIcons(IDR_TOOLBAR_NORMAL_SMALL, buttons, _countof(buttons), CSize(16, 16), xtpImageNormal);
		XTPImageManager()->SetIcons(IDR_TOOLBAR_HOT_SMALL, buttons, _countof(buttons), CSize(16, 16), xtpImageHot);

		XTPImageManager()->SetIcons(IDR_TOOLBAR_NORMAL_LARGE, buttons, _countof(buttons), CSize(24, 24), xtpImageNormal);
		XTPImageManager()->SetIcons(IDR_TOOLBAR_HOT_LARGE, buttons, _countof(buttons), CSize(24, 24), xtpImageHot);
	}

	CXTPCommandBarsOptions* pOptions = pCommandBars->GetCommandBarsOptions();
	pOptions->szLargeIcons = CSize(24, 24);
	pOptions->bLargeIcons = TRUE;


	pOptions->bAlwaysShowFullMenus = FALSE;
	pCommandBars->HideCommands(uHideCmds, _countof(uHideCmds));


	LoadCommandBars(_T("CommandBars"));

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
// CMainFrame message handlers

void CMainFrame::OnEmptyCommand()
{

}



void CMainFrame::OnCustomize()
{
	CXTPCustomizeSheet cs(GetCommandBars());

	CXTPCustomizeOptionsPage pageOptions(&cs);
	cs.AddPage(&pageOptions);

	CXTPCustomizeCommandsPage* pCommands = cs.GetCommandsPage();
	pCommands->AddCategories(IDR_MAINFRAME);
	pCommands->GetControls(_T("File"))->Add(xtpControlButton, ID_FILE_OPEN, _T(""), 1);

	pCommands->AddToolbarCategory(_T("Toolbar"), IDR_TOOLBAR_NORMAL_SMALL);

	pCommands->InsertAllCommandsCategory();
	pCommands->InsertNewMenuCategory();


	cs.DoModal();
}

void CMainFrame::OnToolsThemeCustomtheme()
{
	CXTPPaintManager::SetCustomTheme(new CModernTheme());
	XTP_COMMANDBARS_ICONSINFO* pIconsInfo = XTPPaintManager()->GetIconsInfo();
	pIconsInfo->bUseDisabledIcons = TRUE;
	pIconsInfo->bUseFadedIcons = FALSE;

	GetCommandBars()->RedrawCommandBars();
}

void CMainFrame::OnUpdateToolsThemeCustomtheme(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(XTPPaintManager()->GetCurrentTheme() != xtpThemeOffice2003);

}

void CMainFrame::OnToolsThemeOffice2003theme()
{
	CXTPPaintManager::SetTheme(xtpThemeOffice2003);
	XTP_COMMANDBARS_ICONSINFO* pIconsInfo = XTPPaintManager()->GetIconsInfo();
	pIconsInfo->bUseDisabledIcons = TRUE;

	GetCommandBars()->RedrawCommandBars();
}

void CMainFrame::OnUpdateToolsThemeOffice2003theme(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(XTPPaintManager()->GetCurrentTheme() == xtpThemeOffice2003);
}

void CMainFrame::OnClose()
{
	SaveCommandBars(_T("CommandBars"));

	CFrameWnd::OnClose();
}
