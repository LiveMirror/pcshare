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
#include "TaskPanel.h"

#include "MainFrm.h"

#include "SlidersTheme.h"

#include "SearchDlg.h"

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
	ON_COMMAND(ID_THEME_OFFICE2000, OnThemeOffice2000)
	ON_UPDATE_COMMAND_UI(ID_THEME_OFFICE2000, OnUpdateThemeOffice2000)
	ON_COMMAND(ID_THEME_OFFICE2003, OnThemeOffice2003)
	ON_UPDATE_COMMAND_UI(ID_THEME_OFFICE2003, OnUpdateThemeOffice2003)
	ON_COMMAND(ID_THEME_NATIVEWINXP, OnThemeNativewinxp)
	ON_UPDATE_COMMAND_UI(ID_THEME_NATIVEWINXP, OnUpdateThemeNativewinxp)
	ON_COMMAND(ID_THEME_OFFICE2000PLAIN, OnThemeOffice2000plain)
	ON_UPDATE_COMMAND_UI(ID_THEME_OFFICE2000PLAIN, OnUpdateThemeOffice2000plain)
	ON_COMMAND(ID_THEME_OFFICEXPPLAIN, OnThemeOfficexpplain)
	ON_UPDATE_COMMAND_UI(ID_THEME_OFFICEXPPLAIN, OnUpdateThemeOfficexpplain)
	ON_COMMAND(ID_THEME_OFFICE2003PLAIN, OnThemeOffice2003plain)
	ON_UPDATE_COMMAND_UI(ID_THEME_OFFICE2003PLAIN, OnUpdateThemeOffice2003plain)
	ON_COMMAND(ID_THEME_NATIVEWINXPPLAIN, OnThemeNativewinxpplain)
	ON_UPDATE_COMMAND_UI(ID_THEME_NATIVEWINXPPLAIN, OnUpdateThemeNativewinxpplain)
	ON_COMMAND(ID_TASKPANEL_ANIMATION, OnTaskpanelAnimation)
	ON_UPDATE_COMMAND_UI(ID_TASKPANEL_ANIMATION, OnUpdateTaskpanelAnimation)
	ON_COMMAND(ID_VIEW_SEARCH, OnViewSearch)
	ON_COMMAND(ID_THEME_CUSTOM, OnThemeCustom)
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
	ON_COMMAND(XTP_ID_CUSTOMIZE, OnCustomize)

	ON_MESSAGE(XTPWM_TASKPANEL_NOTIFY, OnTaskPanelNotify)

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
	pStandardBar->GetControls()->FindControl(ID_VIEW_SEARCH)->SetStyle(xtpButtonIconAndCaption);
	pStandardBar->GetControls()->CreateOriginalControls();

	// Load the previous state for command bars.
	LoadCommandBars(_T("CommandBars"));

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

BOOL CMainFrame::CreateTaskPanel()
{

	if (!m_wndTaskPanel.Create(WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS|WS_CLIPCHILDREN, CRect(0, 0, 0, 0), &m_wndSplitter, m_wndSplitter.IdFromRowCol(0, 0)))
		return FALSE;

	m_wndTaskPanel.SetOwner(this);

	m_wndSplitter.SetColumnInfo(0, 210, 0);


	CXTPTaskPanelGroup* pGroupSystem = m_wndTaskPanel.AddGroup(ID_TASKGROUP_SYSTEM);

	pGroupSystem->AddLinkItem(ID_TASKITEM_HIDECONTENTS, 0);
	pGroupSystem->AddLinkItem(ID_TASKITEM_ADDORREMOVE, 1);
	pGroupSystem->AddLinkItem(ID_TASKITEM_SEARCH, 2);

	pGroupSystem->SetSpecialGroup();

	CXTPTaskPanelGroup* pGroupFiles = m_wndTaskPanel.AddGroup(ID_TASKGROUP_FILES);

	pGroupFiles->AddLinkItem(ID_TASKITEM_NEWFOLDER, 3);
	pGroupFiles->AddLinkItem(ID_TASKITEM_PUBLISH, 4);
	pGroupFiles->AddLinkItem(ID_TASKITEM_SHARE, 5);


	CXTPTaskPanelGroup* pGroupOtherPlaces = m_wndTaskPanel.AddGroup(ID_TASKGROUP_OTHERPLACES);

	pGroupOtherPlaces->AddLinkItem(ID_TASKITEM_MYCOMPUTER, 6);
	pGroupOtherPlaces->AddLinkItem(ID_TASKITEM_MYDOCUMENTS, 7);
	pGroupOtherPlaces->AddLinkItem(ID_TASKITEM_SHAREDDOCUMENTS, 8);
	pGroupOtherPlaces->AddLinkItem(ID_TASKITEM_MYNETWORKPLACES, 9);

	//pGroupOtherPlaces->ShowCaption(FALSE);
	//pGroupOtherPlaces->SetExpanded(FALSE);
	//pGroupOtherPlaces->SetExpandable(FALSE);

	CXTPTaskPanelGroup* pGroupDetails = m_wndTaskPanel.AddGroup(ID_TASKGROUP_DETAILS);

	CXTPTaskPanelGroupItem* pItemDisk = pGroupDetails->AddTextItem(_T("Local Disk (C:)"));
	pItemDisk->SetBold(TRUE);
	pItemDisk->GetMargins().SetRectEmpty();

	pGroupDetails->AddTextItem(_T("Local Disk"));
	pGroupDetails->AddTextItem(_T("File Sytem: FAT32"));
	pGroupDetails->AddTextItem(_T("Free Space: 23,3 GB"));
	pGroupDetails->AddTextItem(_T("Total Size: 76,2 GB"));

	m_wndTaskPanel.GetImageManager()->SetIcons(XTPImageManager()->IsAlphaIconsSupported()?
		IDB_TASKICONS_ALPHA: IDB_TASKICONS_NORMAL, 0, 0, CSize(16, 16));

	return TRUE;
}


BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT /*lpcs*/, CCreateContext* pContext)
{
	m_wndSplitter.CreateStatic(this, 1, 2);

	if (!CreateTaskPanel())
		return FALSE;

	m_wndSplitter.CreateView(0, 1, pContext->m_pNewViewClass, CSize(300, 0), pContext);

	m_wndSplitter.ShowTopBorder(FALSE, 0);
	return TRUE;
}


void CMainFrame::SetTheme(XTPPaintTheme commandBarsTheme, XTPTaskPanelPaintTheme taskPanelTheme)
{
	XTPPaintManager()->SetTheme(commandBarsTheme);

	m_wndTaskPanel.SetTheme(taskPanelTheme);

	GetCommandBars()->RedrawCommandBars();
	RedrawWindow(0, 0, RDW_INVALIDATE|RDW_UPDATENOW|RDW_ERASE|RDW_ALLCHILDREN);
}

void CMainFrame::OnThemeOffice2000()
{
	SetTheme(xtpThemeOffice2000, xtpTaskPanelThemeOffice2000);
}

void CMainFrame::OnUpdateThemeOffice2000(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_wndTaskPanel.GetCurrentTheme() == xtpTaskPanelThemeOffice2000);

}

void CMainFrame::OnThemeOffice2003()
{
	SetTheme(xtpThemeOffice2003, xtpTaskPanelThemeOffice2003);
}

void CMainFrame::OnUpdateThemeOffice2003(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_wndTaskPanel.GetCurrentTheme() == xtpTaskPanelThemeOffice2003);

}

void CMainFrame::OnThemeNativewinxp()
{
	SetTheme(xtpThemeNativeWinXP, xtpTaskPanelThemeNativeWinXP);
}

void CMainFrame::OnUpdateThemeNativewinxp(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_wndTaskPanel.GetCurrentTheme() == xtpTaskPanelThemeNativeWinXP);

}

void CMainFrame::OnThemeOffice2000plain()
{
	SetTheme(xtpThemeOffice2000, xtpTaskPanelThemeOffice2000Plain);
}

void CMainFrame::OnUpdateThemeOffice2000plain(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_wndTaskPanel.GetCurrentTheme() == xtpTaskPanelThemeOffice2000Plain);

}

void CMainFrame::OnThemeOfficexpplain()
{
	SetTheme(xtpThemeOfficeXP, xtpTaskPanelThemeOfficeXPPlain);

}

void CMainFrame::OnUpdateThemeOfficexpplain(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_wndTaskPanel.GetCurrentTheme() == xtpTaskPanelThemeOfficeXPPlain);

}

void CMainFrame::OnThemeOffice2003plain()
{
	SetTheme(xtpThemeOffice2003, xtpTaskPanelThemeOffice2003Plain);
}

void CMainFrame::OnUpdateThemeOffice2003plain(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_wndTaskPanel.GetCurrentTheme() == xtpTaskPanelThemeOffice2003Plain);

}

void CMainFrame::OnThemeNativewinxpplain()
{
	SetTheme(xtpThemeNativeWinXP, xtpTaskPanelThemeNativeWinXPPlain);

}

void CMainFrame::OnUpdateThemeNativewinxpplain(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_wndTaskPanel.GetCurrentTheme() == xtpTaskPanelThemeNativeWinXPPlain);

}

void CMainFrame::OnTaskpanelAnimation()
{
	m_wndTaskPanel.SetAnimation(m_wndTaskPanel.GetAnimation() == xtpTaskPanelAnimationNo? xtpTaskPanelAnimationYes: xtpTaskPanelAnimationNo);

}

void CMainFrame::OnUpdateTaskpanelAnimation(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_wndTaskPanel.GetAnimation() == xtpTaskPanelAnimationYes);

}


LRESULT CMainFrame::OnTaskPanelNotify(WPARAM wParam, LPARAM lParam)
{
	switch(wParam) {
	case XTP_TPN_CLICK:
		{
			CXTPTaskPanelGroupItem* pItem = (CXTPTaskPanelGroupItem*)lParam;
			TRACE(_T("Click Event: pItem.Caption = %s, pItem.ID = %i\n"), pItem->GetCaption(), pItem->GetID());
		}
		break;

	case XTP_TPN_RCLICK:
		{
			CXTPTaskPanelItem* pItem = (CXTPTaskPanelItem*)lParam;
			TRACE(_T("RClick Event: pItem.Caption = %s, pItem.ID = %i\n"), pItem->GetCaption(), pItem->GetID());

		}
		break;
	}

	return 0;
}

void CMainFrame::OnViewSearch()
{
	CSearchDlg sd;
	sd.DoModal();

}

void CMainFrame::OnThemeCustom()
{
	XTPPaintManager()->SetTheme(xtpThemeOfficeXP);

	m_wndTaskPanel.SetCustomTheme(new CSlidersTheme);

	GetCommandBars()->RedrawCommandBars();
	RedrawWindow(0, 0, RDW_INVALIDATE|RDW_UPDATENOW|RDW_ERASE|RDW_ALLCHILDREN);

}

void CMainFrame::OnClose()
{
	// Save the current state for command bars.
	SaveCommandBars(_T("CommandBars"));
	// TODO: Add your message handler code here and/or call default

	CFrameWnd::OnClose();
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
	pPageCommands->AddCategories(IDR_MAINFRAME);

	// initialize the commands page page.
	pPageCommands->InsertAllCommandsCategory();
	pPageCommands->InsertBuiltInMenus(IDR_MAINFRAME);
	pPageCommands->InsertNewMenuCategory();

	// display the customize dialog.
	dlg.DoModal();
}
