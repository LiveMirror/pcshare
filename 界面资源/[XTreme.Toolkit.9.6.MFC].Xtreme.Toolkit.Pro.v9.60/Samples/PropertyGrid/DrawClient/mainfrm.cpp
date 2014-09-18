// mainfrm.cpp : implementation of the CMainFrame class
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
#include "drawcli.h"

#include "mainfrm.h"
#include "SplitFrm.h"
#include "DrawVw.h"
#include "DrawDoc.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_COMMAND(ID_PROPERTIES, OnProperties)
	//}}AFX_MSG_MAP

#ifdef _XTP_INCLUDE_DOCKINGPANE
	ON_MESSAGE(XTPWM_DOCKINGPANE_NOTIFY, OnDockingPaneNotify)
#endif
	ON_MESSAGE(XTPWM_PROPERTYGRID_NOTIFY, OnGridNotify)

#ifdef _XTP_INCLUDE_COMMANDBARS
	ON_WM_CLOSE()
	ON_COMMAND(XTP_ID_CUSTOMIZE, OnCustomize)
	ON_MESSAGE(WM_XTP_PRETRANSLATEMOUSEMSG, OnTabbarMouseMsg)
#endif

END_MESSAGE_MAP()

static UINT BASED_CODE indicators[] =
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
	m_pView = 0;
	m_bInUpdate = FALSE;
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{

	if (CMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

#ifdef _XTP_INCLUDE_CONTROLS
	XTThemeManager()->SetTheme(xtThemeOffice2003);
#endif

#ifndef _XTP_INCLUDE_COMMANDBARS

	if (!m_wndToolBar.Create(this) ||
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

	// TODO: Remove this if you don't want tool tips
	m_wndToolBar.SetBarStyle(m_wndToolBar.GetBarStyle() |
		CBRS_TOOLTIPS | CBRS_FLYBY);

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

	CXTPPaintManager::SetTheme(xtpThemeOffice2003);

	CXTPCommandBars* pCommandBars = GetCommandBars();

	CXTPCommandBar* pMenuBar = pCommandBars->SetMenu(_T("Menu Bar"), IDR_DRAWCLTYPE);
	pMenuBar->SetFlags(xtpFlagIgnoreSetMenuMessage);




	CXTPToolBar* pCommandBar = (CXTPToolBar*)pCommandBars->Add(_T("Standard"), xtpBarTop);
	if (!pCommandBar ||
		!pCommandBar->LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;
	}

	CXTPToolBar* pOrderBar = (CXTPToolBar*)pCommandBars->Add(_T("Order"), xtpBarBottom);
	if (!pOrderBar ||
		!pOrderBar->LoadToolBar(IDR_TOOLBAR_ORDER))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;
	}

	for (int i = 0; i < pOrderBar->GetControls()->GetCount(); i++)
		((CXTPControlButton*)pOrderBar->GetControl(i))->SetStyle(xtpButtonIconAndCaption);

	VERIFY(m_MTIClientWnd.Attach(this));
	XTPImageManager()->SetIcons(IDR_COLORS);

	CXTPOffice2003Theme::LoadModernToolbarIcons(IDR_MAINFRAME);
	CXTPOffice2003Theme::LoadModernToolbarIcons(IDR_TOOLBAR_ORDER);


#endif


#ifdef _XTP_INCLUDE_DOCKINGPANE
	GetDockingPaneManager()->InstallDockingPanes(this);

#ifdef _XTP_INCLUDE_COMMANDBARS
	GetDockingPaneManager()->SetTheme(xtpPaneThemeOffice2003);
#endif

	int nID = 1;
	CXTPDockingPane* pPropertyPane = GetDockingPaneManager()->CreatePane(nID, CRect(0, 0,200, 120), xtpPaneDockRight);
	pPropertyPane->SetTitle(_T("Properties"));

	GetDockingPaneManager()->SetIcons(IDB_PANEICONS, &nID, 1);

#else
	EnableDocking(CBRS_ALIGN_ANY);
	if (!m_wndPropertyBar.Create(this))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}
	m_wndPropertyBar.EnableDocking(CBRS_ALIGN_RIGHT|CBRS_ALIGN_LEFT);
	DockControlBar(&m_wndPropertyBar, AFX_IDW_DOCKBAR_RIGHT);
	DelayRecalcLayout();

#endif

#ifdef _XTP_INCLUDE_COMMANDBARS
	XTPPaintManager()->RefreshMetrics();

	// Load the previous state for command bars.
	LoadCommandBars(_T("CommandBars"));
#endif



	return 0;
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

#ifdef _XTP_INCLUDE_COMMANDBARS
	m_MTIClientWnd.Detach();
#endif
}


LRESULT CMainFrame::OnDockingPaneNotify(WPARAM wParam, LPARAM lParam)
{
#ifdef _XTP_INCLUDE_DOCKINGPANE

	if (wParam == XTP_DPN_SHOWWINDOW)
	{
		CXTPDockingPane* pPane = (CXTPDockingPane*)lParam;

		if (!pPane->IsValid())
		{
			ASSERT (pPane->GetID() == 1);

			if (!m_panePropertyGrid.m_hWnd)
			{
				m_panePropertyGrid.Create(_T("STATIC"), NULL, WS_CHILD|WS_CLIPCHILDREN|WS_CLIPSIBLINGS, CRect(0, 0, 0, 0), this, 0);

			}
			pPane->Attach(&m_panePropertyGrid);
		}

		return TRUE;
	}
#endif
	return FALSE;
}

void CMainFrame::OnProperties()
{
#ifdef _XTP_INCLUDE_DOCKINGPANE
	GetDockingPaneManager()->ShowPane(1);
#endif
}

void CMainFrame::UpdatePropertyGridContent(CDrawView* /*pView*/)
{
	CXTPPropertyGrid& wndPropertyGrid = GetPropertyGrid(this);
	if (!wndPropertyGrid.m_hWnd) return;

	if (m_bInUpdate)
		return;

	m_bInUpdate = TRUE;



	wndPropertyGrid.BeginUpdate(m_stateExpanding);

	CFrameWnd* pFrame = MDIGetActive();
	if (!pFrame)
	{
		m_pView = NULL;
		wndPropertyGrid.EndUpdate(m_stateExpanding);
		m_bInUpdate = FALSE;
		return;
	}
	CDrawView* pView = DYNAMIC_DOWNCAST(CDrawView, pFrame->GetActiveView());
	m_pView = pView;

	if (!pView)
	{
		wndPropertyGrid.EndUpdate(m_stateExpanding);
		m_bInUpdate = FALSE;
		return;
	}

	CDrawObjList* pObjects = &pView->m_selection;

	CXTPPropertyGridItem* pCategoryObjects = wndPropertyGrid.AddCategory(ID_CATEGORY_OBJECTS);
	CXTPPropertyGridItem* pItemObjectsCount = pCategoryObjects->AddChildItem(new CXTPPropertyGridItemNumber(_T("Objects Count"), (long)pView->GetDocument()->GetObjects()->GetCount()));
	pItemObjectsCount->SetReadOnly(TRUE);
	pCategoryObjects->Expand();


	CXTPPropertyGridItem* pCategorySelected = wndPropertyGrid.AddCategory(ID_CATEGORY_SELECTED);

	CXTPPropertyGridItem* pItemSelectedCount =  pCategorySelected->AddChildItem(new CXTPPropertyGridItemNumber(_T("Selected Count"), (long)pObjects->GetCount()));
	pItemSelectedCount->SetReadOnly(TRUE);
	if (pObjects->GetCount() > 0)
	{
		CDrawObj* pSelected = pObjects->GetHead();

		pCategorySelected->AddChildItem(new CXTPPropertyGridItemColor(ID_ITEM_LINECOLOR, pSelected->m_logpen.lopnColor));
		pCategorySelected->AddChildItem(new CXTPPropertyGridItemColor(ID_ITEM_FILLCOLOR, pSelected->m_logbrush.lbColor));

		if (pObjects->GetCount() == 1)
		{

			pCategorySelected->AddChildItem(new CXTPPropertyGridItemBool(ID_ITEM_SOLID, pSelected->m_bBrush));

			int nPenSize = pSelected->m_bPen ? pSelected->m_logpen.lopnWidth.x : 0;
			pCategorySelected->AddChildItem(new CXTPPropertyGridItemNumber(ID_ITEM_PENSIZE, nPenSize));
		}
	}


	pCategorySelected->Expand();

	if (pView->GetDocument()->m_pSummInfo)
	{

		CXTPPropertyGridItem* pCategorySummary = wndPropertyGrid.AddCategory(ID_CATEGORY_SUMMARY);



		pCategorySummary->AddChildItem(new CXTPPropertyGridItem(ID_ITEM_AUTHOR,
			pView->GetDocument()->m_pSummInfo->GetAuthor()));

		pCategorySummary->AddChildItem(new CXTPPropertyGridItem(ID_ITEM_TITLE,
			pView->GetDocument()->m_pSummInfo->GetTitle()));
		pCategorySummary->AddChildItem(new CXTPPropertyGridItem(ID_ITEM_SUBJECT,
			pView->GetDocument()->m_pSummInfo->GetSubject()));

		CXTPPropertyGridItem* pItemCreated =  pCategorySummary->AddChildItem(new CXTPPropertyGridItem(_T("Created"),
			pView->GetDocument()->m_pSummInfo->GetCreateDate()));
		pItemCreated->SetReadOnly(TRUE);

		pCategorySummary->Expand();

	}

	if (pObjects->GetCount() > 0)
	{
		wndPropertyGrid.GetVerbs()->Add(_T("Shape Properties"), 1);
		wndPropertyGrid.GetVerbs()->Add(_T("Paper Color"), 2);
	}


	wndPropertyGrid.EndUpdate(m_stateExpanding);
	m_bInUpdate = FALSE;
}


LRESULT CMainFrame::OnGridNotify(WPARAM wParam, LPARAM lParam)
{
	if (wParam == XTP_PGN_VERB_CLICK)
	{
		CXTPPropertyGridVerb* pVerb = (CXTPPropertyGridVerb*)lParam;
		if (pVerb->GetID() == 1) OnCommand(ID_EDIT_PROPERTIES, 0);
		if (pVerb->GetID() == 2) OnCommand(ID_VIEW_PAPERCOLOR, 0);
		return TRUE;
	}
	if (wParam == XTP_PGN_ITEMVALUE_CHANGED && m_pView)
	{
		CXTPPropertyGridItem* pItem = (CXTPPropertyGridItem*)lParam;

		switch (pItem->GetID())
		{
			case ID_ITEM_AUTHOR: m_pView->GetDocument()->m_pSummInfo->SetAuthor(pItem->GetValue()); break;
			case ID_ITEM_SUBJECT: m_pView->GetDocument()->m_pSummInfo->SetSubject(pItem->GetValue()); break;
			case ID_ITEM_TITLE: m_pView->GetDocument()->m_pSummInfo->SetTitle(pItem->GetValue()); break;
			case ID_ITEM_PENSIZE:
				{
					CDrawObj* pSelected = m_pView->m_selection.GetHead();

					int nPenSize = min(100, ((CXTPPropertyGridItemNumber*)pItem)->GetNumber());
					if (nPenSize > 0)
					{
						pSelected->m_logpen.lopnWidth.x = nPenSize;
						pSelected->m_logpen.lopnWidth.y = nPenSize;
					}
					pSelected->Invalidate();
					pSelected->m_pDocument->SetModifiedFlag();
				}
				break;
			case ID_ITEM_SOLID:
				{
					CDrawObj* pSelected = m_pView->m_selection.GetHead();

					BOOL bSolid = min(100, ((CXTPPropertyGridItemBool*)pItem)->GetBool());
					pSelected->m_bBrush = bSolid;
					pSelected->Invalidate();
					pSelected->m_pDocument->SetModifiedFlag();
				}
				break;

			case ID_ITEM_LINECOLOR:
				{
					COLORREF color = ((CXTPPropertyGridItemColor*)pItem)->GetColor();

					POSITION pos = m_pView->m_selection.GetHeadPosition();
					while (pos != NULL)
					{
						CDrawObj* pObj = m_pView->m_selection.GetNext(pos);
						pObj->SetLineColor(color);
					}
				}
				break;
			case ID_ITEM_FILLCOLOR:
				{
					COLORREF color = ((CXTPPropertyGridItemColor*)pItem)->GetColor();

					POSITION pos = m_pView->m_selection.GetHeadPosition();
					while (pos != NULL)
					{
						CDrawObj* pObj = m_pView->m_selection.GetNext(pos);
						pObj->SetFillColor(color);
					}
				}
				break;

			default:
				TRACE(_T("Value Changed. Caption = %s, ID = %i, Value = %s\n"), pItem->GetCaption(), pItem->GetID(), pItem->GetValue());

		}


	}
	return 0;
}



BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
	return CMDIFrameWnd::OnCreateClient(lpcs, pContext);
}



LRESULT CMainFrame::OnTabbarMouseMsg(WPARAM wParam,LPARAM lParam)
{
#ifdef _XTP_INCLUDE_COMMANDBARS

	if (wParam != WM_RBUTTONDOWN)
		return FALSE;

	CPoint point = CPoint((DWORD)lParam);
	CXTPTabManagerItem* pItem =  m_MTIClientWnd.HitTest(point);

	if (pItem)
	{
		CWnd* pFrame = CWnd::FromHandle(pItem->GetHandle());
		MDIActivate(pFrame);

		m_MTIClientWnd.Refresh();

		CMenu menu;
		VERIFY(menu.LoadMenu(IDR_POPUP_TABS));
		CMenu* pPopup = menu.GetSubMenu(0);


		m_MTIClientWnd.WorkspaceToScreen(&point);

		CXTPCommandBars::TrackPopupMenu(pPopup, 0, point.x, point.y, this);

		m_MTIClientWnd.Refresh();

		return TRUE;
	}
#endif
	return FALSE;
}

#ifdef _XTP_INCLUDE_COMMANDBARS

void CMainFrame::OnClose()
{
	// Save the current state for command bars.
	SaveCommandBars(_T("CommandBars"));
	// TODO: Add your message handler code here and/or call default

	CMDIFrameWnd::OnClose();
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
	pPageCommands->AddCategories(IDR_DRAWCLTYPE);

	// initialize the commands page page.
	pPageCommands->InsertAllCommandsCategory();
	pPageCommands->InsertBuiltInMenus(IDR_DRAWCLTYPE);
	pPageCommands->InsertNewMenuCategory();

	// display the customize dialog.
	dlg.DoModal();
}


void CMainFrame::OnSetPreviewMode(BOOL bPreview, CPrintPreviewState* pState)
{
// Toggle CommandBars
	GetCommandBars()->OnSetPreviewMode(bPreview);

#ifdef _XTP_INCLUDE_DOCKINGPANE
// Toggle Docking Panes.
	m_paneManager.OnSetPreviewMode(bPreview);
#endif

// Toggle Tab Client
	m_MTIClientWnd.ShowWorkspace(!bPreview);

	CMDIFrameWnd::OnSetPreviewMode(bPreview, pState);
}

#endif
