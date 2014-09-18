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
#include "ReBarSample.h"

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
	ON_WM_CLOSE()
	ON_COMMAND(ID_THEME_OFFICE, OnThemeOffice)
	ON_UPDATE_COMMAND_UI(ID_THEME_OFFICE, OnUpdateThemeOffice)
	ON_COMMAND(ID_THEME_STANDARD, OnThemeStandard)
	ON_UPDATE_COMMAND_UI(ID_THEME_STANDARD, OnUpdateThemeStandard)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
	ON_COMMAND_RANGE(ID_WEBBROWSER_STOP, ID_WEBBROWSER_NEXT, OnEmptyCommandRange)
	ON_XTP_CREATECONTROL()
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
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!InitCommandBars(/*RUNTIME_CLASS(CReBarCommandBars)*/))
		return -1;

	CXTPCommandBars* pCommandBars = GetCommandBars();

	CXTPPaintManager::SetCustomTheme(new CXTPReBarPaintManager<CXTPOfficeTheme>());
	m_bOfficeXP = TRUE;


	CXTPToolBar* pMenuBar = pCommandBars->SetMenu(_T("Menu Bar"), IDR_MAINFRAME); ;
	if (!pMenuBar)
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	CXTPToolBar* pToolBar = pCommandBars->Add(_T("Standard"), xtpBarTop);
	if (!pToolBar ||
		!pToolBar->LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}


	CXTPToolBar* pWebBar = pCommandBars->Add(_T("Web"), xtpBarTop);
	if (!pWebBar ||
		!pWebBar->LoadToolBar(IDR_TOOLBAR_WEB))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}
	CXTPToolBar* pThemeBar = pCommandBars->Add(_T("Theme"), xtpBarTop);
	if (!pThemeBar ||
		!pThemeBar->LoadToolBar(IDR_TOOLBAR_THEME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	///

	if (!m_wndReBar.Create(this) ||
		!m_wndReBar.AddToolBar(pMenuBar) ||
		!m_wndReBar.AddToolBar(pToolBar, RBBS_GRIPPERALWAYS|RBBS_BREAK) ||
		!m_wndReBar.AddToolBar(pThemeBar) ||
		!m_wndReBar.AddToolBar(pWebBar)
		)
	{
		TRACE0("Failed to create rebar\n");
		return -1;      // fail to create
	}

	pMenuBar->SetFlags(xtpFlagAddMDISysPopup);

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	LoadCommandBars(_T("CommandBars"));
	m_wndReBar.LoadState( _T("RebarState2") );

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


void CMainFrame::OnEmptyCommandRange(UINT)
{

}

int CMainFrame::OnCreateControl(LPCREATECONTROLSTRUCT lpCreateControl)
{
	if (lpCreateControl->bToolBar)
	{
		CXTPToolBar* pToolBar = DYNAMIC_DOWNCAST(CXTPToolBar, lpCreateControl->pCommandBar);
		if (!pToolBar)
			return FALSE;

		if (lpCreateControl->nID == ID_GOTO_URL && pToolBar->GetBarID() == IDR_TOOLBAR_WEB)
		{
			CXTPControlComboBox* pComboUrl = (CXTPControlComboBox*)CXTPControlComboBox::CreateObject();
			pComboUrl->AddString(_T("http://www.codejock.com"));
			pComboUrl->AddString(_T("ms-help://MS.VSCC/vsccCommon/html/vsccCommon_Start_Page.htm"));
			pComboUrl->SetWidth(200);
			pComboUrl->SetDropDownWidth(300);
			pComboUrl->SetDropDownListStyle();
			pComboUrl->SetFlags(xtpFlagManualUpdate);
			pComboUrl->SetEditText(_T("http://www.codejock.com"));

			lpCreateControl->pControl = pComboUrl;
			return TRUE;
		}


	}
	return FALSE;

}

void CMainFrame::OnClose()
{
	SaveCommandBars(_T("CommandBars"));
	m_wndReBar.SaveState( _T("RebarState2") );

	CMDIFrameWnd::OnClose();
}

void CMainFrame::OnCustomize()
{
	CXTPCustomizeSheet cs(GetCommandBars());

	CXTPCustomizeOptionsPage pageOptions(&cs);
	cs.AddPage(&pageOptions);

	CXTPCustomizeCommandsPage* pCommands = cs.GetCommandsPage();
	pCommands->AddCategories(IDR_REBARSTYPE, TRUE);

	pCommands->InsertAllCommandsCategory();
	pCommands->InsertBuiltInMenus(IDR_REBARSTYPE);
	pCommands->InsertNewMenuCategory();

	cs.DoModal();
}

void CMainFrame::OnThemeOffice()
{
	m_bOfficeXP = TRUE;
	CXTPPaintManager::SetCustomTheme(new CXTPReBarPaintManager<CXTPOfficeTheme>());
	GetCommandBars()->RedrawCommandBars();
}

void CMainFrame::OnUpdateThemeOffice(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_bOfficeXP);

}

void CMainFrame::OnThemeStandard()
{
	m_bOfficeXP = FALSE;
	CXTPPaintManager::SetCustomTheme(new CXTPReBarPaintManager<CXTPNativeXPTheme>());
	GetCommandBars()->RedrawCommandBars();

}

void CMainFrame::OnUpdateThemeStandard(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(!m_bOfficeXP);

}

void CMainFrame::OnDestroy()
{
	CMDIFrameWnd::OnDestroy();

}
