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
#include "DynamicPanes.h"

#include "MainFrm.h"
#include "SimpleForm.h"
#include "SimpleListView.h"


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
	ON_COMMAND(ID_MANAGER_CREATEFORM, OnManagerCreateform)
	ON_MESSAGE(XTPWM_DOCKINGPANE_NOTIFY, OnDockingPaneNotify)
	ON_COMMAND(ID_MANAGER_CREATEEDIT, OnManagerCreateedit)
	ON_COMMAND(ID_MANAGER_CREATELISTCONTROL, OnManagerCreatelistcontrol)
	ON_COMMAND(ID_MANAGER_CREATE3INROW, OnManagerCreate3inrow)
	ON_COMMAND(ID_MANAGER_CREATE3INCOLUMN, OnManagerCreate3incolumn)
	ON_COMMAND(ID_MANAGER_CREATE3INWINDOW, OnManagerCreate3inwindow)
	ON_COMMAND(ID_MANAGER_HIDEALL, OnManagerHideall)
	ON_COMMAND(ID_MANAGER_SHOWALL, OnManagerShowall)
	ON_COMMAND(ID_MANAGER_DESTOYALL, OnManagerDestoyall)
	ON_UPDATE_COMMAND_UI(ID_MANAGER_DESTOYALL, OnUpdateManagerAll)
	ON_COMMAND(ID_POPUP_CLOSE, OnPopupClose)
	ON_COMMAND(ID_POPUP_DESTROY, OnPopupDestroy)
	ON_COMMAND(ID_POPUP_HIDE, OnPopupHide)
	ON_UPDATE_COMMAND_UI(ID_POPUP_HIDE, OnUpdatePopupHide)
	ON_COMMAND(ID_TOOLS_ALLOWCONTEXTMENU, OnToolsAllowcontextmenu)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_ALLOWCONTEXTMENU, OnUpdateToolsAllowcontextmenu)
	ON_COMMAND(ID_TOOLS_FORBIDEDITCLOSING, OnToolsForbideditclosing)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_FORBIDEDITCLOSING, OnUpdateToolsForbideditclosing)
	ON_COMMAND(ID_TOOLS_FORBIDFORMCLOSING, OnToolsForbidformclosing)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_FORBIDFORMCLOSING, OnUpdateToolsForbidformclosing)
	ON_COMMAND(ID_TOOLS_FORBIDLISTCLOSING, OnToolsForbidlistclosing)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_FORBIDLISTCLOSING, OnUpdateToolsForbidlistclosing)
	ON_COMMAND(ID_TOOLS_THEME_OFFICE2000, OnToolsThemeOffice2000)
	ON_COMMAND(ID_TOOLS_THEME_OFFICE2003, OnToolsThemeOffice2003)
	ON_COMMAND(ID_TOOLS_THEME_OFFICEXP, OnToolsThemeOfficexp)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_THEME_OFFICE2000, OnUpdateToolsThemeOffice2000)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_THEME_OFFICE2003, OnUpdateToolsThemeOffice2003)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_THEME_OFFICEXP, OnUpdateToolsThemeOfficexp)
	ON_UPDATE_COMMAND_UI(ID_MANAGER_SHOWALL, OnUpdateManagerAll)
	ON_UPDATE_COMMAND_UI(ID_MANAGER_HIDEALL, OnUpdateManagerAll)
	//}}AFX_MSG_MAP
#ifdef _XTP_INCLUDE_COMMANDBARS
	ON_WM_CLOSE()
	ON_COMMAND(XTP_ID_CUSTOMIZE, OnCustomize)
#endif

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
	m_nCount = 0;
	m_bContextMenu = TRUE;

	m_bForbidEditClosing = m_bForbidFormClosing = m_bForbidListClosing = FALSE;
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

#ifndef _XTP_INCLUDE_COMMANDBARS

	if (!m_wndToolBar.Create(this, TBSTYLE_FLAT| WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
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

	// TODO: Delete these three lines if you don't want the toolbar to
	//  be dockable
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);

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

	CXTPPaintManager::SetTheme(xtpThemeOfficeXP);

	CXTPCommandBars* pCommandBars = GetCommandBars();

	pCommandBars->SetMenu(_T("Menu Bar"), IDR_MAINFRAME);

	CXTPToolBar* pStandardBar = (CXTPToolBar*)pCommandBars->Add(_T("Standard"), xtpBarTop);
	if (!pStandardBar ||
		!pStandardBar->LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;
	}

	// Load the previous state for command bars.
	LoadCommandBars(_T("CommandBars"));

#endif

	m_paneManager.InstallDockingPanes(this);
	m_paneManager.SetTheme(xtpPaneThemeOffice);

	int nIDIcons[] = {1, 2, 3, 4, 5, 6};
	m_paneManager.SetIcons(IDB_BITMAP_ICONS, nIDIcons, 7, RGB(255, 0, 255));

#ifdef _XTP_INCLUDE_COMMANDBARS
	XTPPaintManager()->RefreshMetrics();
#endif

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
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



LRESULT CMainFrame::OnDockingPaneNotify(WPARAM wParam, LPARAM lParam)
{
	if (wParam == XTP_DPN_SHOWWINDOW)
	{
		// get a pointer to the docking pane being shown.
		CXTPDockingPane* pPane = (CXTPDockingPane*)lParam;
		if (!pPane->IsValid())
		{
			CWnd* pWnd = NULL;
			if (m_mapPanes.Lookup(pPane->GetID(), pWnd))
			{
				pPane->Attach(pWnd);
			}
		}

		return TRUE; // handled
	}
	if (wParam == XTP_DPN_RCLICK && m_bContextMenu)
	{
		m_pPopupPane = 0;
		CXTPDockingPaneBase* pContainer = (CXTPDockingPaneBase*)lParam;
		if (pContainer->GetType() == xtpPaneTypeTabbedContainer)
			m_pPopupPane = ((CXTPDockingPaneTabbedContainer*)pContainer)->GetSelected();
		else if (pContainer->GetType() == xtpPaneTypeDockingPane)
			m_pPopupPane = (CXTPDockingPane*)pContainer;

		if (!m_pPopupPane) return TRUE;


		CPoint pos;
		GetCursorPos(&pos);

		CMenu menu;
		VERIFY(menu.LoadMenu(IDR_POPUP_PANES));
		CMenu* pPopup = menu.GetSubMenu(0);

	#ifndef _XTP_INCLUDE_COMMANDBARS
		TrackPopupMenu(pPopup->GetSafeHmenu(), TPM_RIGHTBUTTON, pos.x, pos.y, 0, GetSafeHwnd(), 0);
	#else
		CXTPCommandBars::TrackPopupMenu(pPopup, TPM_RIGHTBUTTON, pos.x, pos.y, this);
	#endif

		return TRUE;
	}

	if (wParam == XTP_DPN_CLOSEPANE)
	{
		CXTPDockingPane* pPane = (CXTPDockingPane*)lParam;

		CWnd* pWnd =NULL;

		if (!m_mapPanes.Lookup(pPane->GetID(), pWnd))
			return FALSE;

		CFrameWnd* pFrame = DYNAMIC_DOWNCAST(CFrameWnd, pWnd);
		if (pFrame)
		{
			CView* pView = (CView*)pFrame->GetWindow(GW_CHILD);
			if (pView && m_bForbidEditClosing && pView->IsKindOf(RUNTIME_CLASS(CEditView)))
				return XTP_ACTION_NOCLOSE;
			if (pView && m_bForbidListClosing && pView->IsKindOf(RUNTIME_CLASS(CSimpleListView)))
				return XTP_ACTION_NOCLOSE;
			if (pView && m_bForbidFormClosing && pView->IsKindOf(RUNTIME_CLASS(CSimpleForm)))
				return XTP_ACTION_NOCLOSE;


		}
		return TRUE;
	}

	return FALSE;
}


CXTPDockingPane* CMainFrame::CreatePane(CRuntimeClass* pNewViewClass, CString strFormat, XTPDockingPaneDirection direction, CXTPDockingPane* pNeighbour)
{
	int nID = ++m_nCount;

	CXTPDockingPane* pwndPane = m_paneManager.CreatePane(
		nID, CRect(0, 0,200, 120), direction, pNeighbour);

	CString strTitle;
	strTitle.Format(strFormat, nID);
	pwndPane->SetTitle(strTitle);
	pwndPane->SetIconID(nID % 6 + 1);

	CFrameWnd* pFrame = new CFrameWnd;

	CCreateContext context;
	context.m_pNewViewClass = pNewViewClass;
	context.m_pCurrentDoc = GetActiveView()->GetDocument();

	pFrame->Create(NULL, NULL, WS_CHILD|WS_VISIBLE|WS_CLIPCHILDREN|WS_CLIPSIBLINGS, CRect(0, 0, 0, 0), this, NULL, 0, &context);
	pFrame->ModifyStyleEx(WS_EX_CLIENTEDGE, 0);
	pFrame->SendMessageToDescendants(WM_INITIALUPDATE, 0, 0, TRUE, TRUE);

	m_mapPanes.SetAt(nID, pFrame);

	return pwndPane;
}

void CMainFrame::OnManagerCreateform()
{
	CreatePane(RUNTIME_CLASS(CSimpleForm), _T("Form %i"), xtpPaneDockRight);
}


void CMainFrame::OnManagerCreateedit()
{
	CreatePane(RUNTIME_CLASS(CEditView), _T("Edit %i"), xtpPaneDockBottom);
}

void CMainFrame::OnManagerCreatelistcontrol()
{
	CreatePane(RUNTIME_CLASS(CSimpleListView), _T("List %i"), xtpPaneDockLeft);
}

void CMainFrame::OnManagerCreate3inrow()
{
	CXTPDockingPane* pwndPaneForm = CreatePane(RUNTIME_CLASS(CSimpleForm), _T("Form %i"), xtpPaneDockBottom);
	CXTPDockingPane* pwndPaneEdit = CreatePane(RUNTIME_CLASS(CEditView), _T("Edit %i"), xtpPaneDockRight, pwndPaneForm);
	CreatePane(RUNTIME_CLASS(CSimpleListView), _T("List %i"), xtpPaneDockRight, pwndPaneEdit);
}

void CMainFrame::OnManagerCreate3incolumn()
{
	CXTPDockingPane* pwndPaneForm = CreatePane(RUNTIME_CLASS(CSimpleForm), _T("Form %i"), xtpPaneDockLeft);
	CXTPDockingPane* pwndPaneEdit = CreatePane(RUNTIME_CLASS(CEditView), _T("Edit %i"), xtpPaneDockBottom, pwndPaneForm);
	CreatePane(RUNTIME_CLASS(CSimpleListView), _T("List %i"), xtpPaneDockBottom, pwndPaneEdit);
}

void CMainFrame::OnManagerCreate3inwindow()
{
	CXTPDockingPane* pwndPaneForm = CreatePane(RUNTIME_CLASS(CSimpleForm), _T("Form %i"), xtpPaneDockRight);
	CXTPDockingPane* pwndPaneEdit = CreatePane(RUNTIME_CLASS(CEditView), _T("Edit %i"), xtpPaneDockRight);
	CXTPDockingPane* pwndPaneList = CreatePane(RUNTIME_CLASS(CSimpleListView), _T("List %i"), xtpPaneDockRight);

	m_paneManager.AttachPane(pwndPaneEdit, pwndPaneForm);
	m_paneManager.AttachPane(pwndPaneList, pwndPaneForm);
}

void CMainFrame::OnManagerHideall()
{
	CXTPDockingPaneInfoList& lstPanes = m_paneManager.GetPaneList();
	POSITION pos = lstPanes.GetHeadPosition();
	while (pos)
	{
		CXTPDockingPane* pPane = lstPanes.GetNext(pos);
		if (!pPane->IsHidden())
			pPane->Close();
	}
}

void CMainFrame::OnManagerShowall()
{
	CXTPDockingPaneInfoList& lstPanes = m_paneManager.GetPaneList();
	POSITION pos = lstPanes.GetHeadPosition();
	while (pos)
	{
		CXTPDockingPane* pPane = lstPanes.GetNext(pos);
		if (pPane->IsClosed())
			m_paneManager.ShowPane(pPane);
	}
}

void CMainFrame::OnManagerDestoyall()
{
	CXTPDockingPaneInfoList& lstPanes = m_paneManager.GetPaneList();
	POSITION pos = lstPanes.GetHeadPosition();
	while (pos)
	{
		CXTPDockingPane* pPane = lstPanes.GetNext(pos);
		m_paneManager.DestroyPane(pPane);
	}
}

void CMainFrame::OnUpdateManagerAll(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_paneManager.GetPaneList().GetCount() > 0);
}

void CMainFrame::OnPopupClose()
{
	m_pPopupPane->Close();
}

void CMainFrame::OnPopupDestroy()
{
	m_paneManager.DestroyPane(m_pPopupPane);
}

void CMainFrame::OnPopupHide()
{
	if (!m_pPopupPane->IsHidden())
	{	
		m_pPopupPane->Hide();
	} else
	{
		m_pPopupPane->Select();
		m_paneManager.ToggleDocking(m_pPopupPane);
	}

}

void CMainFrame::OnUpdatePopupHide(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_pPopupPane->IsHidden()? TRUE: FALSE);

}

void CMainFrame::OnToolsAllowcontextmenu()
{
	m_bContextMenu = !m_bContextMenu;

}

void CMainFrame::OnUpdateToolsAllowcontextmenu(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_bContextMenu);

}

void CMainFrame::OnToolsForbideditclosing()
{
	m_bForbidEditClosing = !m_bForbidEditClosing;

}

void CMainFrame::OnUpdateToolsForbideditclosing(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_bForbidEditClosing);
}

void CMainFrame::OnToolsForbidformclosing()
{
	m_bForbidFormClosing = !m_bForbidFormClosing;

}

void CMainFrame::OnUpdateToolsForbidformclosing(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_bForbidFormClosing);
}

void CMainFrame::OnToolsForbidlistclosing()
{
	m_bForbidListClosing = !m_bForbidListClosing;

}

void CMainFrame::OnUpdateToolsForbidlistclosing(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_bForbidListClosing);
}

void CMainFrame::OnToolsThemeOffice2000()
{
	m_paneManager.SetTheme(xtpPaneThemeDefault);

#ifdef _XTP_INCLUDE_COMMANDBARS
	CXTPPaintManager::SetTheme(xtpThemeOffice2000);
	GetCommandBars()->RedrawCommandBars();
#endif
}

void CMainFrame::OnToolsThemeOffice2003()
{
	m_paneManager.SetTheme(xtpPaneThemeOffice2003);


#ifdef _XTP_INCLUDE_COMMANDBARS

	CXTPPaintManager::SetTheme(xtpThemeOffice2003);
	GetCommandBars()->RedrawCommandBars();
#endif
}

void CMainFrame::OnToolsThemeOfficexp()
{
	m_paneManager.SetTheme(xtpPaneThemeOffice);

#ifdef _XTP_INCLUDE_COMMANDBARS
	CXTPPaintManager::SetTheme(xtpThemeOfficeXP);
	GetCommandBars()->RedrawCommandBars();
#endif
}

void CMainFrame::OnUpdateToolsThemeOffice2000(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_paneManager.GetCurrentTheme() == xtpPaneThemeDefault? TRUE: FALSE);

}

void CMainFrame::OnUpdateToolsThemeOffice2003(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_paneManager.GetCurrentTheme() == xtpPaneThemeOffice2003? TRUE: FALSE);

}

void CMainFrame::OnUpdateToolsThemeOfficexp(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_paneManager.GetCurrentTheme() == xtpPaneThemeOffice? TRUE: FALSE);
}

#ifdef _XTP_INCLUDE_COMMANDBARS

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

#endif
