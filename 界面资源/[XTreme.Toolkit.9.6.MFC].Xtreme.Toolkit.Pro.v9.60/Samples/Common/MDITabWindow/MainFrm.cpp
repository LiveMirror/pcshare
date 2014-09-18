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
#include "MDITabWindow.h"

#include "MainFrm.h"

#pragma warning(disable:4510)
#pragma warning(disable:4610)

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



class CDockingFrameWnd : public CFrameWnd
{
public:
	CDockingFrameWnd(){};
	DECLARE_DYNAMIC(CDockingFrameWnd);
};
IMPLEMENT_DYNAMIC(CDockingFrameWnd, CFrameWnd);


CFrameWnd* CMainFrame::GetParentDockingFrame(CWnd* pWnd)
{
	if (pWnd)
	{
		return DYNAMIC_DOWNCAST(CDockingFrameWnd, pWnd->GetParentFrame());
	}
	return NULL;
}


/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_COMMAND(ID_VIEW_THEME_OFFICE2000, OnViewThemeOffice2000)
	ON_COMMAND(ID_VIEW_THEME_OFFICEXP, OnViewThemeOfficexp)
	ON_COMMAND(ID_VIEW_THEME_OFFICE2003, OnViewThemeOffice2003)
	ON_COMMAND(ID_VIEW_THEME_NATIVEXP, OnViewThemeNativexp)
	ON_UPDATE_COMMAND_UI(ID_VIEW_THEME_OFFICE2000, OnUpdateViewThemeOffice2000)
	ON_UPDATE_COMMAND_UI(ID_VIEW_THEME_OFFICEXP, OnUpdateViewThemeOfficexp)
	ON_UPDATE_COMMAND_UI(ID_VIEW_THEME_OFFICE2003, OnUpdateViewThemeOffice2003)
	ON_UPDATE_COMMAND_UI(ID_VIEW_THEME_NATIVEXP, OnUpdateViewThemeNativexp)
	ON_MESSAGE(WM_XTP_PRETRANSLATEMOUSEMSG, OnTabbarMouseMsg)
	ON_COMMAND(ID_TOGGLE_DOCK, OnToggleDock)
	ON_COMMAND(ID_WINDOW_CLOSE_ALL, OnWindowCloseAll)
	ON_UPDATE_COMMAND_UI(ID_WINDOW_CLOSE_ALL, OnUpdateWindowCloseAll)
	ON_COMMAND(ID_WINDOW_DOCKABLE, OnWindowDockable)
	ON_MESSAGE(XTPWM_DOCKINGPANE_NOTIFY, OnDockingPaneNotify)
	ON_UPDATE_COMMAND_UI(ID_WINDOW_DOCKABLE, OnUpdateWindowDockable)
	ON_COMMAND(ID_WINDOW_HIDE, OnWindowHide)
	ON_COMMAND(ID_WINDOW_HIDEALL, OnWindowHideall)
	ON_COMMAND(ID_WINDOW_AUTOHIDE, OnWindowAutohide)
	ON_UPDATE_COMMAND_UI(ID_WINDOW_AUTOHIDE, OnUpdateWindowAutohide)
	ON_UPDATE_COMMAND_UI(ID_WINDOW_HIDEALL, OnUpdateWindowHideall)
	ON_COMMAND(ID_TOGGLE_TABS, OnToggleTabs)
	ON_COMMAND(XTP_ID_CUSTOMIZE, OnCustomize)
	ON_WM_CLOSE()
	ON_UPDATE_COMMAND_UI(ID_TOGGLE_TABS, OnUpdateToggleTabs)
	//}}AFX_MSG_MAP
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
	m_nCount = 0;

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

	pCommandBars->SetMenu(_T("Menu Bar"), IDR_MAINFRAME);

	CXTPToolBar* pStandardBar = (CXTPToolBar*)pCommandBars->Add(_T("Standard"), xtpBarTop);
	if (!pStandardBar ||
		!pStandardBar->LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
	}

	XTPImageManager()->SetIcons(IDR_COLORS);

	m_paneManager.InstallDockingPanes(this);
	m_paneManager.SetTheme(xtpPaneThemeOffice);

	int nIDIcons[] = {1, 2};
	m_paneManager.SetIcons(IDB_BITMAP_ICONS, nIDIcons, 2, RGB(255, 255, 255));

	VERIFY(m_MTIClientWnd.Attach(this, TRUE));
	m_MTIClientWnd.EnableToolTips();

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


void CMainFrame::OnDestroy()
{
	CMDIFrameWnd::OnDestroy();

	m_MTIClientWnd.Detach();

}

void CMainFrame::OnViewThemeOffice2000()
{
	m_paneManager.SetTheme(xtpPaneThemeDefault);

	CXTPPaintManager::SetTheme(xtpThemeOffice2000);
	GetCommandBars()->RedrawCommandBars();
}

void CMainFrame::OnViewThemeOfficexp()
{
	m_paneManager.SetTheme(xtpPaneThemeOffice);

	CXTPPaintManager::SetTheme(xtpThemeOfficeXP);
	GetCommandBars()->RedrawCommandBars();
}

void CMainFrame::OnViewThemeOffice2003()
{
	m_paneManager.SetTheme(xtpPaneThemeOffice2003);

	CXTPPaintManager::SetTheme(xtpThemeOffice2003);
	GetCommandBars()->RedrawCommandBars();
}

void CMainFrame::OnViewThemeNativexp()
{
	m_paneManager.SetTheme(xtpPaneThemeDefault);

	CXTPPaintManager::SetTheme(xtpThemeNativeWinXP);
	GetCommandBars()->RedrawCommandBars();
}

void CMainFrame::OnUpdateViewThemeOffice2000(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(XTPPaintManager()->GetCurrentTheme() == xtpThemeOffice2000);
}

void CMainFrame::OnUpdateViewThemeOfficexp(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(XTPPaintManager()->GetCurrentTheme() == xtpThemeOfficeXP);
}

void CMainFrame::OnUpdateViewThemeOffice2003(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(XTPPaintManager()->GetCurrentTheme() == xtpThemeOffice2003);
}

void CMainFrame::OnUpdateViewThemeNativexp(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(XTPPaintManager()->GetCurrentTheme() == xtpThemeNativeWinXP);
}




LRESULT CMainFrame::OnTabbarMouseMsg(WPARAM wParam,LPARAM lParam)
{
	if (wParam != WM_RBUTTONDOWN)
		return FALSE;

	CPoint point = CPoint((DWORD)lParam);

	CXTPTabManagerItem* pItem =  m_MTIClientWnd.HitTest(point);
	if (pItem)
	{
		CFrameWnd* pFrame = DYNAMIC_DOWNCAST(CMDIChildWnd, CWnd::FromHandle(pItem->GetHandle()));
		if (!pFrame)
			return FALSE;

		MDIActivate(pFrame);
		pFrame->GetActiveView()->SetFocus();

		m_MTIClientWnd.Refresh();

		CMenu menu;
		VERIFY(menu.LoadMenu(IDR_POPUP_TABS));
		CMenu* pPopup = menu.GetSubMenu(0);


		m_MTIClientWnd.WorkspaceToScreen(&point);


		CXTPPopupBar* pPopupBar = (CXTPPopupBar*) CXTPPopupBar::CreatePopupBar(GetCommandBars());
		VERIFY(pPopupBar->LoadMenu(pPopup));
		pPopupBar->GetControls()->FindControl(xtpControlPopup, -1, TRUE, FALSE)->SetEnabled(XTPPaintManager()->GetCurrentTheme() == xtpThemeOffice2003);


		VERIFY(pPopupBar->Popup(point.x, point.y, NULL));

		while ( pPopupBar->IsTrackingMode() )
		{
			AfxGetApp()->PumpMessage();
		}
		pPopupBar->InternalRelease();


		m_MTIClientWnd.Refresh();
		return TRUE;
	}


	CMenu menuPopup;
	menuPopup.CreatePopupMenu();
	menuPopup.AppendMenu(MF_STRING, ID_WINDOW_CLOSE_ALL, _T("&Close All"));
	menuPopup.AppendMenu(MF_STRING, ID_TOGGLE_DOCK, _T("&Toggle Docking"));

	m_MTIClientWnd.WorkspaceToScreen(&point);
	CXTPCommandBars::TrackPopupMenu(&menuPopup, TPM_RIGHTBUTTON, point.x, point.y, AfxGetMainWnd());

	m_MTIClientWnd.Refresh();

	return TRUE;
}

void CMainFrame::OnToggleDock(void)
{
	if (m_MTIClientWnd.GetPaintManager()->GetPosition() == xtpTabPositionTop)
		m_MTIClientWnd.GetPaintManager()->SetPosition(xtpTabPositionBottom);
	else
		m_MTIClientWnd.GetPaintManager()->SetPosition(xtpTabPositionTop);

	m_MTIClientWnd.Refresh();
}

void CMainFrame::OnToggleTabs()
{
}

void CMainFrame::OnUpdateToggleTabs(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_MTIClientWnd.IsAttached());
}

void CMainFrame::OnWindowCloseAll()
{
	CXTPDockingPaneInfoList& lstPanes = m_paneManager.GetPaneList();
	POSITION pos = lstPanes.GetHeadPosition();
	while (pos)
	{
		CXTPDockingPane* pPane = lstPanes.GetNext(pos);
		m_paneManager.DestroyPane(pPane);
	}

	AfxGetApp()->CloseAllDocuments(FALSE);
}


void CMainFrame::ReparentAllChildren(CFrameWnd *pFrameOld, CFrameWnd* pFrameNew)
{
	CPtrList childList;
	CWnd* pChild;

	for (pChild = pFrameOld->GetTopWindow(); pChild != NULL; pChild = pChild->GetNextWindow(GW_HWNDNEXT))
		childList.AddTail(pChild);

	POSITION pos = childList.GetHeadPosition();
	while (pos != NULL)
	{
		pChild = (CWnd*)childList.GetNext(pos);
		pChild->SetParent(pFrameNew);
	}
}

BOOL CMainFrame::ActivateDockableFrame(CFrameWnd* pFrameNew)
{
	if(!pFrameNew) return FALSE;

	pFrameNew->SendMessage(WM_ACTIVATE, WA_ACTIVE);
	pFrameNew->ActivateFrame(-1);
	pFrameNew->OnUpdateFrameTitle(TRUE);
	return TRUE;
}

void CMainFrame::OnUpdateWindowDockable(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(GetParentDockingFrame(GetFocus()) !=NULL);
}


void CMainFrame::UnDockFrame(CFrameWnd* pFrameOld, CXTPDockingPane* pPane)
{
	class CFriendlyDocTemplate: private CMultiDocTemplate
	{
		friend class CMainFrame;
	};

	CDocument* pDocument = pFrameOld->GetActiveDocument();
	if (!pDocument)
		return;

	CFriendlyDocTemplate* pTemplate = (CFriendlyDocTemplate*)pDocument->GetDocTemplate();

	// Create frame without any child view
	CRuntimeClass* pViewClassSave = pTemplate->m_pViewClass;
	pTemplate->m_pViewClass = NULL;
	CFrameWnd* pFrameNew = pTemplate->CreateNewFrame(pDocument, NULL);
	pTemplate->m_pViewClass = pViewClassSave;

	ReparentAllChildren(pFrameOld, pFrameNew);

	pFrameOld->DestroyWindow();

	// activate the first view.
	CView* pView = NULL;
	if (pFrameNew->GetActiveView() == NULL)
	{
		CPtrList childList;
		CWnd* pChild = pFrameNew->GetTopWindow();
		while ( pChild != NULL && pView == NULL)
		{
			ASSERT_KINDOF(CWnd, pChild);
			if (pChild->IsKindOf(RUNTIME_CLASS(CView)))
			{
				pView = (CView*)pChild;
				pFrameNew->SetActiveView(pView,FALSE);
			}
			pChild = pChild->GetNextWindow(GW_HWNDNEXT);
		}
	}

	ActivateDockableFrame(pFrameNew);

	pFrameNew->ModifyStyleEx(WS_EX_CLIENTEDGE, 0, SWP_FRAMECHANGED);

	pPane->Detach();
	m_paneManager.ClosePane(pPane);
}

void CMainFrame::DockFrame(CFrameWnd* pFrameOld)
{

	CDocument* pDocument = pFrameOld->GetActiveDocument();

	if (!pDocument)
		return;

	CString strTitle = pDocument->GetTitle();

	CFrameWnd* pFrameNew = new CDockingFrameWnd;

	CCreateContext context;
	context.m_pCurrentDoc = pDocument;

	pFrameNew->Create(NULL, NULL, WS_CHILD, CRect(0, 0, 0, 0), this, NULL, 0, &context);

	ReparentAllChildren(pFrameOld, pFrameNew);

	pFrameOld->DestroyWindow();


	CView* pView = NULL;
	if (pFrameNew->GetActiveView() == NULL)
	{
		CPtrList childList;
		CWnd* pChild = pFrameNew->GetTopWindow();
		while ( pChild != NULL && pView == NULL)
		{
			ASSERT_KINDOF(CWnd, pChild);
			if (pChild->IsKindOf(RUNTIME_CLASS(CView)))
			{
				pView = (CView*)pChild;
				pFrameNew->SetActiveView(pView,FALSE);
			}
			pChild = pChild->GetNextWindow(GW_HWNDNEXT);
		}
	}

	ActivateDockableFrame(pFrameNew);

	pFrameNew->ModifyStyleEx(WS_EX_CLIENTEDGE, 0);

	int nID = ++m_nCount;

	CXTPDockingPane* pwndPane = m_paneManager.CreatePane(
		nID, CRect(0, 0,200, 120), (XTPDockingPaneDirection)(RAND_S()%4), m_paneManager.GetClientPane());

	pwndPane->SetTitle(strTitle);

	CString str;
	pDocument->GetDocTemplate()->GetDocString(str, CDocTemplate::docName);

	pwndPane->SetIconID(str == _T("Bounce")? 2: 1);


	m_mapPanes.SetAt(nID, pFrameNew);

	m_MTIClientWnd.UpdateContents();
}


void CMainFrame::OnWindowDockable()
{
	CFrameWnd* pFrame = GetParentDockingFrame(GetFocus());

	if (pFrame)
	{
		CXTPDockingPaneTabbedContainer* pContainer = (CXTPDockingPaneTabbedContainer*)pFrame->GetParent();
		CXTPDockingPane* pPane = pContainer->GetSelected();
		ASSERT(pPane);
		ASSERT(pPane->GetChild() == pFrame);

		UnDockFrame(pFrame, pPane);

	}
	else
	{
		CMDIChildWnd* pFrameOld = MDIGetActive();
		if (pFrameOld) DockFrame(pFrameOld);
	}
}


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
	if (wParam == XTP_DPN_RCLICK)
	{
		CXTPDockingPaneTabbedContainer* pContainer = (CXTPDockingPaneTabbedContainer*)lParam;

		if (!pContainer->GetSelected()) return TRUE;


		CPoint pos;
		GetCursorPos(&pos);

		CMenu menu;
		VERIFY(menu.LoadMenu(IDR_POPUP_PANES));
		CMenu* pPopup = menu.GetSubMenu(0);

		CXTPCommandBars::TrackPopupMenu(pPopup, TPM_RIGHTBUTTON, pos.x, pos.y, this);

		return TRUE;
	}

	return FALSE;
}



BOOL CMainFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	CFrameWnd* pFrame = GetParentDockingFrame(GetFocus());

	if (pFrame)
	{
		// let the view have first crack at the command
		if (pFrame->OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
			return TRUE;

		return CFrameWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
	}


	return CXTPMDIFrameWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

BOOL CMainFrame::OnCommand(WPARAM wParam, LPARAM lParam)
{
	CFrameWnd* pFrame = GetParentDockingFrame(GetFocus());

	if (pFrame)
	{
		// let the view have first crack at the command
		if (AfxCallWndProc(pFrame,
			pFrame->m_hWnd,
			WM_COMMAND, wParam, lParam) != 0)
			return TRUE; // handled by child
	}

	return CMDIFrameWnd::OnCommand(wParam, lParam);
}



void CMainFrame::OnWindowHide()
{
	CFrameWnd* pFrame = GetParentDockingFrame(GetFocus());

	if (pFrame)
	{
		CXTPDockingPaneTabbedContainer* pContainer = (CXTPDockingPaneTabbedContainer*)pFrame->GetParent();
		CXTPDockingPane* pPane = pContainer->GetSelected();

		pPane->Close();
	}
	else
	{
		CWnd* pActiveChildWnd = MDIGetActive(NULL);
		if (pActiveChildWnd)
			pActiveChildWnd->SendMessage(WM_COMMAND, ID_FILE_CLOSE);
	}
}

void CMainFrame::OnWindowHideall()
{
	CXTPDockingPaneInfoList& lstPanes = m_paneManager.GetPaneList();
	POSITION pos = lstPanes.GetHeadPosition();
	while (pos)
	{
		CXTPDockingPane* pPane = lstPanes.GetNext(pos);
		if (!pPane->IsClosed() && !pPane->IsHidden())
			m_paneManager.HidePane(pPane);
	}

}

void CMainFrame::OnWindowAutohide()
{
	CFrameWnd* pFrame = GetParentDockingFrame(GetFocus());

	if (pFrame)
	{
		CXTPDockingPaneTabbedContainer* pContainer = (CXTPDockingPaneTabbedContainer*)pFrame->GetParent();
		CXTPDockingPane* pPane = pContainer->GetSelected();

		pPane->Hide();
	}
}

void CMainFrame::OnUpdateWindowAutohide(CCmdUI* pCmdUI)
{

	pCmdUI->Enable(GetParentDockingFrame(GetFocus()) != NULL);

}
void CMainFrame::OnUpdateWindowCloseAll(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(MDIGetActive() != NULL);
}

void CMainFrame::OnUpdateWindowHideall(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_paneManager.GetPaneList().GetCount() > 0);

}

int CMainFrame::OnCreateControl(LPCREATECONTROLSTRUCT /*lpCreateControl*/)
{

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
