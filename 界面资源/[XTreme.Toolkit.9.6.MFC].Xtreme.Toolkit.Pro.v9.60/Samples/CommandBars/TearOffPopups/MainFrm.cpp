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
#include "TearOffPopups.h"

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
	ON_COMMAND(ID_THEMES_OFFICE2000, OnThemesOffice2000)
	ON_UPDATE_COMMAND_UI(ID_THEMES_OFFICE2000, OnUpdateThemesOffice2000)
	ON_COMMAND(ID_THEMES_OFFICEXP, OnThemesOfficexp)
	ON_UPDATE_COMMAND_UI(ID_THEMES_OFFICEXP, OnUpdateThemesOfficexp)
	ON_COMMAND(ID_THEMES_OFFICE2003, OnThemesOffice2003)
	ON_UPDATE_COMMAND_UI(ID_THEMES_OFFICE2003, OnUpdateThemesOffice2003)
	ON_COMMAND(ID_THEMES_NATIVEXPTHEME, OnThemesNativexptheme)
	ON_UPDATE_COMMAND_UI(ID_THEMES_NATIVEXPTHEME, OnUpdateThemesNativexptheme)
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP

	ON_XTP_CREATECOMMANDBAR()
	ON_XTP_CREATECONTROL()

	ON_COMMAND(XTP_ID_CUSTOMIZE, OnCustomize)

	ON_COMMAND_RANGE(ID_BDR_OUTSIDE, ID_BDR_INSIDE_VERT, OnBorders)
	ON_UPDATE_COMMAND_UI_RANGE(ID_BDR_OUTSIDE, ID_BDR_INSIDE_VERT, OnUpdateBorders)

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
	m_dwBorders = 0;

}

CMainFrame::~CMainFrame()
{
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

	CXTPPaintManager::SetTheme(xtpThemeOfficeXP);

	CXTPCommandBars* pCommandBars = GetCommandBars();

	CXTPCommandBar* pMenuBar = pCommandBars->SetMenu(_T("Menu Bar"), IDR_TEAROFTYPE);
	pMenuBar->SetFlags(xtpFlagIgnoreSetMenuMessage|xtpFlagAddMDISysPopup);

	CXTPToolBar* pStandardBar = (CXTPToolBar*)pCommandBars->Add(_T("Standard"), xtpBarTop);
	if (!pStandardBar ||
		!pStandardBar->LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;
	}

	CXTPToolBar* pBordersBar = (CXTPToolBar*)pCommandBars->Add(_T("Borders"), xtpBarTop);
	if (!pBordersBar ||
		!pBordersBar->LoadToolBar(IDR_BORDERS))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;
	}


	XTPImageManager()->SetIcons(IDR_TOOLBAR_ICONS);

	LoadCommandBars(_T("CommandBars2"));

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


int CMainFrame::OnCreateCommandBar(LPCREATEBARSTRUCT lpCreatePopup)
{
	if (lpCreatePopup->bTearOffBar && lpCreatePopup->nID == 1005)
	{
		CXTPToolBar* pToolBar = DYNAMIC_DOWNCAST(CXTPToolBar, lpCreatePopup->pCommandBar);
		ASSERT(pToolBar);
		pToolBar->EnableDocking(xtpFlagFloating);
		pToolBar->EnableCustomization(FALSE);
		return TRUE;
	}

	if (lpCreatePopup->bPopup && _tcscmp(lpCreatePopup->lpcstrCaption, _T("Find")) == 0)
	{

		CXTPPopupBar* pPopupBar = CXTPPopupBar::CreatePopupBar(GetCommandBars());
		pPopupBar->SetTearOffPopup(_T("Find Bar"), 1000);
		lpCreatePopup->pCommandBar = pPopupBar;
		return TRUE;
	}

	if (lpCreatePopup->bPopup && _tcscmp(lpCreatePopup->lpcstrCaption, _T("Styles")) == 0)
	{
		CXTPPopupBar* pPopupBar = CXTPPopupBar::CreatePopupBar(GetCommandBars());
		pPopupBar->SetTearOffPopup(_T("Styles Bar"), 1001);
		lpCreatePopup->pCommandBar = pPopupBar;
		return TRUE;
	}
	if (lpCreatePopup->bPopup && _tcscmp(lpCreatePopup->lpcstrCaption, _T("Themes")) == 0)
	{
		CXTPPopupBar* pPopupBar = CXTPPopupBar::CreatePopupBar(GetCommandBars());
		pPopupBar->SetTearOffPopup(_T("Themes Bar"), 1003);
		lpCreatePopup->pCommandBar = pPopupBar;
		return TRUE;
	}
	if (lpCreatePopup->bPopup && lpCreatePopup->nID == ID_BDR_NONE)
	{
		CXTPPopupToolBar* pPopupBar = CXTPPopupToolBar::CreatePopupToolBar(GetCommandBars());

		pPopupBar->LoadToolBar(IDR_BORDERS);
		pPopupBar->SetWidth(23 * 5);
		pPopupBar->SetTearOffPopup(_T("Borders"), IDR_BORDERS, 23 * 5);

		lpCreatePopup->pCommandBar = pPopupBar;
		return TRUE;
	}

	return FALSE;
}


int CMainFrame::OnCreateControl(LPCREATECONTROLSTRUCT lpCreateControl)
{
	if (lpCreateControl->bToolBar)
	{
		CXTPToolBar* pToolBar = DYNAMIC_DOWNCAST(CXTPToolBar, lpCreateControl->pCommandBar);
		if (!pToolBar)
			return FALSE;

		if (lpCreateControl->nID == ID_BDR_NONE && pToolBar->GetBarID() == IDR_MAINFRAME)
		{
			lpCreateControl->controlType = xtpControlSplitButtonPopup;
			return TRUE;
		}
		if (lpCreateControl->nID == ID_BUTTON_TEXT && pToolBar->GetBarID() == IDR_MAINFRAME)
		{
			CXTPControlPopupColor* pPopupColor = new CXTPControlPopupColor();

			CXTPPopupBar* pColorBar = CXTPPopupToolBar::CreatePopupToolBar(GetCommandBars());
			pPopupColor->SetCommandBar(pColorBar);

			pColorBar->GetControls()->Add(new CXTPControlButtonColor(), XTP_IDS_AUTOMATIC);
			pColorBar->GetControls()->Add(new CXTPControlColorSelector(), ID_SELECTOR_TEXT);
			pColorBar->GetControls()->Add(new CXTPControlButtonColor(), XTP_IDS_MORE_COLORS);

			pColorBar->SetTearOffPopup(_T("Text Color"), 1005, 0);
			pColorBar->EnableCustomization(FALSE);

			pColorBar->InternalRelease();

			lpCreateControl->pControl = pPopupColor;

			return TRUE;
		}
	}
	return FALSE;
}

void CMainFrame::SwitchBorders(DWORD dwState)
{
	if (m_dwBorders & dwState) m_dwBorders &= ~dwState; else m_dwBorders |= dwState;

}


void CMainFrame::OnBorders(UINT nID)
{
	switch (nID)
	{
		case ID_BDR_TOP: SwitchBorders(borderTop); break;
		case ID_BDR_LEFT: SwitchBorders(borderLeft); break;
		case ID_BDR_RIGHT: SwitchBorders(borderRight); break;
		case ID_BDR_BOTTOM: SwitchBorders(borderBottom); break;
		case ID_BDR_NONE: m_dwBorders = 0; break;
		case ID_BDR_OUTSIDE: SwitchBorders(borderOutside); break;
		case ID_BDR_ALL: SwitchBorders(borderAll); break;
		case ID_BDR_INSIDE_VERT: SwitchBorders(borderInsideVert); break;
		case ID_BDR_INSIDE_HORZ: SwitchBorders(borderInsideHorz); break;
		case ID_BDR_INSIDE: SwitchBorders(borderInside); break;
	}

}

void CMainFrame::OnUpdateBorders(CCmdUI* pCmdUI)
{
	switch (pCmdUI->m_nID)
	{
		case ID_BDR_TOP: pCmdUI->SetCheck(m_dwBorders & borderTop? TRUE: FALSE); break;
		case ID_BDR_LEFT: pCmdUI->SetCheck(m_dwBorders & borderLeft? TRUE: FALSE); break;
		case ID_BDR_RIGHT: pCmdUI->SetCheck(m_dwBorders & borderRight? TRUE: FALSE); break;
		case ID_BDR_BOTTOM: pCmdUI->SetCheck(m_dwBorders & borderBottom? TRUE: FALSE); break;
		case ID_BDR_OUTSIDE : pCmdUI->SetCheck((m_dwBorders & borderOutside) == borderOutside? TRUE: FALSE); break;
		case ID_BDR_ALL : pCmdUI->SetCheck((m_dwBorders & borderAll) == borderAll? TRUE: FALSE); break;
		case ID_BDR_INSIDE_VERT: pCmdUI->SetCheck(m_dwBorders & borderInsideVert? TRUE: FALSE); break;
		case ID_BDR_INSIDE_HORZ: pCmdUI->SetCheck(m_dwBorders & borderInsideHorz? TRUE: FALSE); break;
		case ID_BDR_INSIDE: pCmdUI->SetCheck((m_dwBorders & borderInside) == borderInside? TRUE: FALSE); break;
	}
}

void CMainFrame::OnCustomize()
{
	CXTPCustomizeSheet cs(GetCommandBars());

	CXTPCustomizeOptionsPage pageOptions(&cs);
	cs.AddPage(&pageOptions);

	CXTPCustomizeCommandsPage* pCommands = cs.GetCommandsPage();
	pCommands->AddCategories(IDR_TEAROFTYPE);
	pCommands->AddToolbarCategory(_T("Borders"), IDR_BORDERS);

	pCommands->InsertAllCommandsCategory();
	pCommands->InsertNewMenuCategory();


	cs.DoModal();
}

void CMainFrame::OnThemesOffice2000()
{
	CXTPPaintManager::SetTheme(xtpThemeOffice2000);
	GetCommandBars()->RedrawCommandBars();
}

void CMainFrame::OnUpdateThemesOffice2000(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(XTPPaintManager()->GetCurrentTheme() == xtpThemeOffice2000);

}

void CMainFrame::OnThemesOfficexp()
{
	CXTPPaintManager::SetTheme(xtpThemeOfficeXP);
	GetCommandBars()->RedrawCommandBars();
}

void CMainFrame::OnUpdateThemesOfficexp(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(XTPPaintManager()->GetCurrentTheme() == xtpThemeOfficeXP);

}

void CMainFrame::OnThemesOffice2003()
{
	CXTPPaintManager::SetTheme(xtpThemeOffice2003);
	GetCommandBars()->RedrawCommandBars();
}

void CMainFrame::OnUpdateThemesOffice2003(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(XTPPaintManager()->GetCurrentTheme() == xtpThemeOffice2003);
}

void CMainFrame::OnThemesNativexptheme()
{
	CXTPPaintManager::SetTheme(xtpThemeNativeWinXP);
	GetCommandBars()->RedrawCommandBars();
}

void CMainFrame::OnUpdateThemesNativexptheme(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(XTPPaintManager()->GetCurrentTheme() == xtpThemeNativeWinXP);
}

void CMainFrame::OnClose()
{
	SaveCommandBars(_T("CommandBars2"));

	CMDIFrameWnd::OnClose();
}
