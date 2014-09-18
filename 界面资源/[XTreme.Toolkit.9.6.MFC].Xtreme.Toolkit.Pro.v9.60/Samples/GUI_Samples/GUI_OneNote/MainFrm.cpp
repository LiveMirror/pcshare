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
#include "GUI_OneNote.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define IDC_BUTTON_AUTORUN 300
/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_CLOSE()
	ON_COMMAND(ID_VIEW_TASKPANE, OnViewTaskPane)
	//}}AFX_MSG_MAP
	ON_XTP_CREATECONTROL()
	ON_XTP_CREATECOMMANDBAR()
	ON_COMMAND(XTP_ID_CUSTOMIZE, OnCustomize)

#ifdef _XTP_INCLUDE_DOCKINGPANE
	ON_MESSAGE(XTPWM_DOCKINGPANE_NOTIFY, OnDockingPaneNotify)
#endif

	ON_MESSAGE(WM_XTP_PRETRANSLATEMOUSEMSG, OnTabbarMouseMsg)


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

	CXTPCommandBar* pMenuBar = pCommandBars->SetMenu(_T("Menu Bar"), IDR_MAINFRAME);
	pMenuBar->SetFlags(xtpFlagIgnoreSetMenuMessage|xtpFlagHideMDIButtons);

	XTPImageManager()->SetMaskColor(RGB(128, 0, 128));

	CXTPToolBar* pCommandBar = (CXTPToolBar*)pCommandBars->Add(_T("Standard"), xtpBarTop);
	if (!pCommandBar ||
		!pCommandBar->LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;
	}

	CXTPToolBar* pFormatBar = (CXTPToolBar*)pCommandBars->Add(_T("Formatting"), xtpBarTop);
	if (!pFormatBar ||
		!pFormatBar->LoadToolBar(IDR_FORMATBAR))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;
	}

	if (XTPImageManager()->IsAlphaIconsSupported())
	{
		XTPImageManager()->SetIcons(IDR_MAINFRAME, IDR_MAINFRAME_ALPHA, xtpImageNormal);
		XTPImageManager()->SetIcons(IDR_EXTENDED, IDR_EXTENDED_ALPHA, xtpImageNormal);
		XTPImageManager()->SetIcons(IDR_FORMATBAR, IDR_FORMATBAR_ALPHA, xtpImageNormal);

		XTPPaintManager()->GetIconsInfo()->bUseDisabledIcons = TRUE;
		CXTPImageManager::m_dDisabledBrightnessFactor = 2.0;
		CXTPImageManager::m_dDisabledAlphaFactor = 4.0;

	} else
	{
		CXTPOffice2003Theme::LoadModernToolbarIcons(IDR_MAINFRAME);
		CXTPOffice2003Theme::LoadModernToolbarIcons(IDR_EXTENDED);
		CXTPOffice2003Theme::LoadModernToolbarIcons(IDR_FORMATBAR);
	}
	XTPImageManager()->SetIcons(IDR_COMMONBAR);
	XTPImageManager()->SetIcons(IDR_COLORS);


#ifdef _XTP_INCLUDE_DOCKINGPANE

	m_paneManager.InstallDockingPanes(this);
	m_paneManager.SetTheme(xtpPaneThemeOffice2003);

	// Create docking panes.
	m_paneManager.CreatePane(
		IDR_PANE_NEW, CRect(0, 0,200, 120), xtpPaneDockRight);

	CXTPDockingPane* pPaneSearch = m_paneManager.CreatePane(
		IDR_PANE_SEARCH, CRect(0, 0,200, 120), xtpPaneDockLeft);
	pPaneSearch->Hide();

	CXTPImageManagerIcon* pIcon = XTPImageManager()->GetImage(ID_FILE_NEW);
	ASSERT(pIcon);
	m_paneManager.SetIcon(IDR_PANE_NEW, pIcon->GetIcon());

	pIcon = XTPImageManager()->GetImage(ID_EDIT_FIND);
	ASSERT(pIcon);
	m_paneManager.SetIcon(IDR_PANE_SEARCH, pIcon->GetIcon());

	CXTPDockingPaneLayout layoutNormal(&m_paneManager);
	if (layoutNormal.Load(_T("NormalLayout")))
	{
		m_paneManager.SetLayout(&layoutNormal);
	}
#endif

	VERIFY(m_MTIClientWnd.Attach(this));
	m_MTIClientWnd.EnableToolTips();

	CXTPTabPaintManager* pTabPaintManager = new CXTPTabPaintManager();
	pTabPaintManager->SetAppearance(xtpTabAppearancePropertyPage2003);
	pTabPaintManager->m_bOneNoteColors = TRUE;
	pTabPaintManager->m_bHotTracking = TRUE;
	pTabPaintManager->m_bShowIcons = FALSE;
	pTabPaintManager->m_bBoldSelected = TRUE;
	pTabPaintManager->GetAppearanceSet()->m_rcHeaderMargin.SetRect(2, 6, 2, 0);
	pTabPaintManager->m_rcClientMargin.SetRect(2, 2, 2, 2);
	pTabPaintManager->m_rcButtonMargin.SetRect(4, 1, 4, 1);

	m_MTIClientWnd.SetPaintManager(pTabPaintManager);

	// Load the previous state for toolbars and menus.
	LoadCommandBars(_T("CommandBars"));


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

void CMainFrame::OnViewTaskPane()
{
#ifdef _XTP_INCLUDE_DOCKINGPANE
	m_paneManager.ShowPane(IDR_PANE_NEW);
#endif

}


int CMainFrame::OnCreateCommandBar(LPCREATEBARSTRUCT lpCreatePopup)
{
	if (lpCreatePopup->nID == ID_FORMAT_NOTEFLAGS_TODO ||
		(lpCreatePopup->bPopup && _tcscmp(lpCreatePopup->lpcstrCaption, _T("Note Flags")) == 0))
	{

		CXTPPopupBar* pPopupBar = CXTPPopupBar::CreatePopupBar(GetCommandBars());
		pPopupBar->SetTearOffPopup(_T("Note Flags"), 1000);
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

		if (lpCreateControl->nID == ID_EDIT_FONT && pToolBar->GetBarID() == IDR_FORMATBAR)
		{
			CXTPControlComboBox* pComboFont = new CXTPControlFontComboBox();
			pComboFont->SetDropDownListStyle();

			lpCreateControl->pControl = pComboFont;
			return TRUE;
		}

		if (lpCreateControl->nID == ID_EDIT_SIZE && pToolBar->GetBarID() == IDR_FORMATBAR)
		{
			CXTPControlComboBox* pComboSize = new CXTPControlSizeComboBox();
			pComboSize->SetDropDownListStyle();

			lpCreateControl->pControl = pComboSize;
			return TRUE;
		}
		if (lpCreateControl->nID == ID_FORMAT_NOTEFLAGS_TODO && pToolBar->GetBarID() == IDR_MAINFRAME)
		{
			lpCreateControl->controlType = xtpControlSplitButtonPopup;
			return TRUE;
		}


		if (lpCreateControl->nID == ID_FORMAT_FONTCOLOR && pToolBar->GetBarID() == IDR_FORMATBAR)
		{
			CXTPControlPopupColor* pPopupColor = new CXTPControlPopupColor();

			CXTPPopupBar* pColorBar = (CXTPPopupBar*)CXTPPopupToolBar::CreateObject();

			pColorBar->GetControls()->Add(new CXTPControlButtonColor(), XTP_IDS_AUTOMATIC);
			pColorBar->GetControls()->Add(new CXTPControlColorSelector(), ID_FORMAT_FONTCOLOR_SELECTOR);

			pPopupColor->SetCommandBar(pColorBar);
			pColorBar->EnableCustomization(FALSE);
			pColorBar->InternalRelease();

			lpCreateControl->pControl = pPopupColor;
			return TRUE;
		}
		if (lpCreateControl->nID == ID_FORMAT_HIGHLIGHT && pToolBar->GetBarID() == IDR_FORMATBAR)
		{
			CXTPControlPopupColor* pPopupColor = new CXTPControlPopupColor();

			CXTPPopupBar* pColorBar = (CXTPPopupBar*)CXTPPopupToolBar::CreateObject();

			pColorBar->GetControls()->Add(new CXTPControlButtonColor(), ID_FORMAT_HIGHLIGHT_NONE);
			pColorBar->GetControls()->Add(new CXTPControlColorSelector(), ID_FORMAT_HIGHLIGHT_SELECTOR);

			pPopupColor->SetCommandBar(pColorBar);
			pColorBar->EnableCustomization(FALSE);
			pColorBar->InternalRelease();

			lpCreateControl->pControl = pPopupColor;
			return TRUE;
		}



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
	pCommands->GetControls(_T("File"))->Add(xtpControlButton, ID_FILE_OPEN, _T(""), 1);

	pCommands->InsertAllCommandsCategory();
	pCommands->InsertBuiltInMenus(IDR_MAINFRAME);
	pCommands->InsertNewMenuCategory();

	cs.DoModal();
}

#ifdef _XTP_INCLUDE_CONTROLS

void CMainFrame::CreateTaskPanel()
{

	VERIFY(m_wndTaskPanel.Create(WS_CHILD|WS_CLIPSIBLINGS|WS_CLIPCHILDREN, CRect(0, 0, 0, 0), this, 0));
	m_wndTaskPanel.SetOwner(this);
	m_wndTaskPanel.SetTheme(xtpTaskPanelThemeOffice2003Plain);
	m_wndTaskPanel.SetExpandable(FALSE);
	m_wndTaskPanel.SetAnimation(xtpTaskPanelAnimationNo);

	CXTPTaskPanelGroupItem* pTextItem;

	CXTPTaskPanelGroup* pGroupNew = m_wndTaskPanel.AddGroup(ID_TASKGROUP_NEW);
	pGroupNew->GetInnerMargins().SetRect(0, 5, 0, 0);


		pGroupNew->AddLinkItem(ID_TASKITEM_BLANK, 0);
	pTextItem = pGroupNew->AddTextItem(_T("Add a new page to the current notebook. A new page is represented by a new tab at the end of the vertical series of tabs."));
	pTextItem->GetMargins().SetRect(20, 0, 0, 5);

	pGroupNew->AddLinkItem(ID_TASKITEM_SUBPAGE, 1);
	pTextItem = pGroupNew->AddTextItem(_T("Add a new page immediately following the current page and that shares the same title as the current page. A subpage is represented by a small tab to indicate that it is grouped with another page."));
	pTextItem->GetMargins().SetRect(20, 0, 0, 5);

	pGroupNew->AddLinkItem(ID_TASKITEM_SECTION, 2);
	pTextItem = pGroupNew->AddTextItem(_T("Create a section file in the current folder on your computer. Sections are represented by tabs across the top of the pages."));
	pTextItem->GetMargins().SetRect(20, 0, 0, 5);


	pGroupNew->AddLinkItem(ID_TASKITEM_QUICK, 3);
	pTextItem = pGroupNew->AddTextItem(_T("Open a small OneNote window where you can make notes that are automatically saved in the Quick notes section."));
	pTextItem->GetMargins().SetRect(20, 0, 0, 5);

	m_ilTaskIcons.Create(16, 16, ILC_COLOR24|ILC_MASK, 0, 1);
	CBitmap bmp;
	bmp.LoadBitmap(IDB_TASKPANEL_ICONS);
	m_ilTaskIcons.Add(&bmp, RGB(0, 0xFF, 0));

	m_wndTaskPanel.SetImageList(&m_ilTaskIcons);


	m_wndButtonAutoRun.Create(_T("Show at Startup"), WS_CHILD|BS_AUTOCHECKBOX|BS_NOTIFY   , CRect(0, 0, 300, 20), &m_wndTaskPanel, IDC_BUTTON_AUTORUN);
	m_wndButtonAutoRun.SetFont(XTPPaintManager()->GetRegularFont());
	CXTPTaskPanelGroupItem* pItemButton = pGroupNew->AddControlItem(m_wndButtonAutoRun);
	pItemButton->SetSize(CSize(0, 20));
	pItemButton->GetMargins().SetRect(0, 15, 0, 0);
	m_wndButtonAutoRun.SetItem(pItemButton);
	m_wndButtonAutoRun.SetCheck(1);

}

BEGIN_MESSAGE_MAP(CButtonEx, CButton)
	ON_WM_CTLCOLOR_REFLECT()
	ON_CONTROL_REFLECT(BN_CLICKED, OnAutoRun)
END_MESSAGE_MAP()

HBRUSH CButtonEx::CtlColor(CDC* pDC, UINT /*nCtlColor*/)
{
	if (m_pItem)
	{
		XTP_TASKPANEL_GROUPCOLORS* pColors = m_pItem->GetPaintManager()->GetGroupColors(m_pItem->GetItemGroup()->IsSpecialGroup());

		if (!m_brushBack.GetSafeHandle() || pColors->clrClient != m_clrBack)
		{
			m_brushBack.DeleteObject();
			m_brushBack.CreateSolidBrush(pColors->clrClient);
			m_clrBack = pColors->clrClient;
		}

		pDC->SetBkMode(TRANSPARENT);
		return m_brushBack;
	}

	return 0;
}

void CButtonEx::OnAutoRun()
{
	TRACE(_T("AutoRun clicked\n"));

}


#endif



LRESULT CMainFrame::OnDockingPaneNotify(WPARAM wParam, LPARAM lParam)
{
#ifdef _XTP_INCLUDE_DOCKINGPANE
	if (wParam == XTP_DPN_SHOWWINDOW)
	{
		CXTPDockingPane* pPane = (CXTPDockingPane*)lParam;

		if (!pPane->IsValid())
		{
			switch(pPane->GetID())
			{
				case IDR_PANE_NEW:


				#ifdef _XTP_INCLUDE_CONTROLS
					if (!m_wndTaskPanel.m_hWnd)
					{
						CreateTaskPanel();
					}
					pPane->Attach(&m_wndTaskPanel);

				#else

					if (!m_paneNew.m_hWnd)
					{
						m_paneNew.Create(_T("STATIC"), NULL, WS_CHILD, CRect(0, 0, 0, 0), this, 0);
					}
					pPane->Attach(&m_paneNew);

				#endif
					break;

				case IDR_PANE_SEARCH:
					if (!m_paneSeaarch.m_hWnd)
					{
						m_paneSeaarch.Create(_T("STATIC"), NULL, WS_CHILD, CRect(0, 0, 0, 0), this, 0);
					}
					pPane->Attach(&m_paneSeaarch);
					break;

				default:
					ASSERT(FALSE);
			}
		}

		return TRUE;

	}
#endif
	return FALSE;
}

void CMainFrame::OnDestroy()
{
	m_MTIClientWnd.Detach();

	CMDIFrameWnd::OnDestroy();
}

void CMainFrame::OnClose()
{
#ifdef _XTP_INCLUDE_DOCKINGPANE
	CXTPDockingPaneLayout layoutNormal(&m_paneManager);
	m_paneManager.GetLayout(&layoutNormal);
	layoutNormal.Save(_T("NormalLayout"));
#endif

	// Save the current state for toolbars and menus.
	SaveCommandBars(_T("CommandBars"));

	CMDIFrameWnd::OnClose();
}



LRESULT CMainFrame::OnTabbarMouseMsg(WPARAM wParam,LPARAM lParam)
{
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
	return FALSE;
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
