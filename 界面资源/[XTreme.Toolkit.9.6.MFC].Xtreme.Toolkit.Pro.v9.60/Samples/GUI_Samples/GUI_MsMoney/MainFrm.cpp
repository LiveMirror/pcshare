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
#include "GUI_MsMoney.h"
#include "MsMoneyTheme.h"

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
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
	ON_COMMAND_RANGE(ID_FILE_NEW_NEWACCOUNT, ID_SHOPPING_BANKINGCENTER, OnEmptyCommandRange)
	ON_COMMAND(ID_NAVIGATE_BACK, OnEmptyCommand)

	ON_COMMAND(ID_PORTFOLIO, OnGreenTheme)
	ON_COMMAND(ID_BILLSDEPOSITS, OnBlueTheme)
	ON_COMMAND(ID_REPORTS, OnRedTheme)
	ON_COMMAND(ID_ACCOUNTLIST, OnBlueTheme)
	ON_COMMAND(ID_CATEGORIES, OnGreenTheme)
	ON_COMMAND(ID_BUDGET, OnRedTheme)


	ON_XTP_CREATECOMMANDBAR()

END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

void SetIcons(UINT nNormal, UINT nHot, UINT nDisabled)
{
	CBitmap bmpNormal;
	VERIFY(bmpNormal.LoadBitmap(nNormal));
	XTPImageManager()->SetIcons(IDR_MAINFRAME, bmpNormal);

	CBitmap bmpHot;
	VERIFY(bmpHot.LoadBitmap(nHot));
	XTPImageManager()->SetIcons(IDR_MAINFRAME, bmpHot, xtpImageHot);

	CBitmap bmpDisabled;
	VERIFY(bmpDisabled.LoadBitmap(nDisabled));
	XTPImageManager()->SetIcons(IDR_MAINFRAME, bmpDisabled, xtpImageDisabled);

}

void CMainFrame::OnGreenTheme()
{
	SetIcons(IDR_MAINFRAME_GREEN, IDR_MAINFRAME_GREEN_HOT, IDR_MAINFRAME_GREEN_DISABLED);
	CXTPPaintManager::SetCustomTheme(new CMsMoneyTheme(RGB(152, 185, 158), RGB(108, 156, 120), RGB(136, 174, 144), RGB(93, 145, 106)));
}

void CMainFrame::OnRedTheme()
{
	SetIcons(IDR_MAINFRAME_RED, IDR_MAINFRAME_RED_HOT, IDR_MAINFRAME_RED_DISABLED);
	CXTPPaintManager::SetCustomTheme(new CMsMoneyTheme(RGB(201, 160, 156), RGB(179, 120, 116), RGB(193, 145, 141), RGB(171, 106, 102)));
}


void CMainFrame::OnBlueTheme()
{
	SetIcons(IDR_MAINFRAME, IDR_MAINFRAME_HOT, IDR_MAINFRAME_DISABLED);
	CXTPPaintManager::SetCustomTheme(new CMsMoneyTheme(RGB(140, 163, 196), RGB(91, 125, 173), RGB(122, 149, 187), RGB(74, 111, 165)));
}


void CMainFrame::OnEmptyCommand()
{
}

void CMainFrame::OnEmptyCommandRange(UINT)
{
}
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
	{
		return -1;      // fail to create
	}

	CXTPCommandBars* pCommandBars = GetCommandBars();

	pCommandBars->SetMenu(_T("Menu Bar"), IDR_MAINFRAME);

	XTPImageManager()->SetMaskColor(RGB(255, 0, 255));

	CXTPToolBar* pCommandBar = (CXTPToolBar*)pCommandBars->Add(_T("Standard"), xtpBarTop);
	if (!pCommandBar ||
		!pCommandBar->LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
	}
	pCommandBar->EnableDocking(xtpFlagAlignAny | xtpFlagFloating | xtpFlagStretched | xtpFlagHideWrap);

	CXTPPaintManager::SetCustomTheme(new CMsMoneyTheme(RGB(140, 163, 196), RGB(91, 125, 173), RGB(122, 149, 187), RGB(74, 111, 165)));
	//CXTPPaintManager::SetCustomTheme(new CMsMoneyTheme(RGB(152, 185, 158), RGB(108, 156, 120), RGB(136, 174, 144), RGB(93, 145, 106)));

	SetIcons(IDR_MAINFRAME, IDR_MAINFRAME_HOT, IDR_MAINFRAME_DISABLED);

	GetCommandBars()->GetCommandBarsOptions()->szIcons = CSize(24, 22);
	GetCommandBars()->GetCommandBarsOptions()->bShowExpandButtonAlways = FALSE;

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

int CMainFrame::OnCreateCommandBar(LPCREATEBARSTRUCT lpCreatePopup)
{
	if (lpCreatePopup->bExpandBar)
	{
		lpCreatePopup->pCommandBar = CXTPPopupBar::CreatePopupBar(GetCommandBars());
		return TRUE;
	}
	return FALSE;
}
