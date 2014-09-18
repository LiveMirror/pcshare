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
#include "MDIMenus.h"

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
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	ON_WM_CREATE()
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP

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
	// TODO: add member initialization code here

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


	CXTPCommandBars* pCommandBars = GetCommandBars();

	CXTPMenuBar* pMenuBar = pCommandBars->SetMenu(_T("Menu Bar"), IDR_MAINFRAME);
	pMenuBar->SetFlags(xtpFlagAddMDISysPopup);

	CXTPToolBar* pStandardBar = (CXTPToolBar*)pCommandBars->Add(_T("Standard"), xtpBarTop);
	if (!pStandardBar ||
		!pStandardBar->LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;
	}

	XTPColorManager()->DisableLunaColors(TRUE);
	CXTPPaintManager::SetTheme(xtpThemeOffice2003);


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




int CMainFrame::OnCreateControl(LPCREATECONTROLSTRUCT lpCreateControl)
{
	CXTPMenuBar* pMenuBar = DYNAMIC_DOWNCAST(CXTPMenuBar, lpCreateControl->pCommandBar);
	if (pMenuBar)
	{
		ASSERT(lpCreateControl->pMenu);
		CMenu* pPopupMenu = lpCreateControl->pMenu->GetSubMenu(lpCreateControl->nIndex);

		if (lpCreateControl->strCaption == _T("&Window"))
		{
			CXTPControlPopup* pControlPopup = CXTPControlPopup::CreateControlPopup(xtpControlPopup);

			// Exclude only IDR_MAINFRAME template
			pControlPopup->ExcludeDocTemplate(IDR_MAINFRAME);

			pControlPopup->SetCommandBar(pPopupMenu);


			lpCreateControl->pControl = pControlPopup;
			return TRUE;
		}

		if (lpCreateControl->strCaption == _T("&Edit"))
		{
			CXTPControlPopup* pControlPopup = CXTPControlPopup::CreateControlPopup(xtpControlPopup);

			// Present only in IDR_MDIMENUTYPE2 and IDR_MDIMENUTYPE3 template
			pControlPopup->AssignDocTemplate(IDR_MDIMENUTYPE2);
			pControlPopup->AssignDocTemplate(IDR_MDIMENUTYPE3);

			pControlPopup->SetCommandBar(pPopupMenu);

			lpCreateControl->pControl = pControlPopup;
			return TRUE;
		}
		if (lpCreateControl->strCaption == _T("&Color"))
		{
			CXTPControlPopup* pControlPopup = CXTPControlPopup::CreateControlPopup(xtpControlPopup);

			// Present only in IDR_MDIMENUTYPE1 template
			pControlPopup->AssignDocTemplate(IDR_MDIMENUTYPE1);

			pControlPopup->SetCommandBar(pPopupMenu);

			lpCreateControl->pControl = pControlPopup;
			return TRUE;
		}

	}

	if (lpCreateControl->nID == ID_GOTO_URL)
	{
		CXTPControlComboBox* pComboUrl = (CXTPControlComboBox*)CXTPControlComboBox::CreateObject();

		pComboUrl->AssignDocTemplate(IDR_MDIMENUTYPE3);

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

void CMainFrame::OnClose()
{
	SaveCommandBars(_T("CommandBars2"));

	CFrameWnd::OnClose();
}
