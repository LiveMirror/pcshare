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
#include "ToolBox.h"

#include "MainFrm.h"
#include "ToolBoxView.h"


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
	ON_MESSAGE(XTPWM_TASKPANEL_NOTIFY, OnTaskPanelNotify)
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

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
// get path of executable
	TCHAR szBuff[_MAX_PATH];
	VERIFY(::GetModuleFileName(AfxGetInstanceHandle(), szBuff, _MAX_PATH));

	LPTSTR lpszExt = _tcsrchr(szBuff, '.');
	lstrcpy(lpszExt, _T(".xml"));

	m_strIniFileName = szBuff;

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

	CXTPPaintManager::SetTheme(xtpThemeOfficeXP);

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


	CXTPPropExchangeXMLNode px(TRUE, 0, _T("Settings"));
	if (px.LoadFromFile(m_strIniFileName))
	{
		CXTPPropExchangeSection pxTaskPanel(px.GetSection(_T("TaskPanel")));

		m_wndTaskPanel.GetGroups()->DoPropExchange(&pxTaskPanel);

		CXTPPropExchangeSection pxCommandBars(px.GetSection(_T("CommandBars")));
		GetCommandBars()->DoPropExchange(&pxCommandBars, NULL);
	}
	else
	{
		ResetToolboxItems();
	}

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	cs.style |= WS_CLIPCHILDREN|WS_CLIPSIBLINGS;

	cs.cx = 690;
	cs.cy = 500;

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


CXTPTaskPanelGroup* CMainFrame::CreateToolboxGroup(UINT nID)
{
	CXTPTaskPanelGroup* pFolder = m_wndTaskPanel.AddGroup(nID);

	CXTPTaskPanelGroupItem* pPointer = pFolder->AddLinkItem(ID_TOOLBOXITEM_POINTER, 0);
	pPointer->SetItemSelected(TRUE);
	pPointer->AllowDrag(FALSE);
	pPointer->AllowDrop(FALSE);

	return pFolder;
}

void CMainFrame::ResetToolboxItems()
{
	m_wndTaskPanel.GetGroups()->Clear(FALSE);

	CXTPTaskPanelGroup* pFolderPropertyPanes = CreateToolboxGroup(ID_TOOLBOXFOLDER_PROPERTYPANES);

	CXTPTaskPanelGroup* pFolderData = CreateToolboxGroup(ID_TOOLBOXFOLDER_DATA);
	pFolderData->AddLinkItem(ID_TOOLBOXITEM_DATASET                 ,26);
	pFolderData->AddLinkItem(ID_TOOLBOXITEM_OLEDBDATAADAPTER        ,27);
	pFolderData->AddLinkItem(ID_TOOLBOXITEM_OLEDBCONNECTION         ,28);
	pFolderData->AddLinkItem(ID_TOOLBOXITEM_OLEDBCOMMAND            ,29);
	pFolderData->AddLinkItem(ID_TOOLBOXITEM_SQLDATAADAPTER          ,30);
	pFolderData->AddLinkItem(ID_TOOLBOXITEM_SQLCONNECTION           ,31);
	pFolderData->AddLinkItem(ID_TOOLBOXITEM_SQLCOMMAND              ,32);
	pFolderData->AddLinkItem(ID_TOOLBOXITEM_DATAVIEW                ,33);

	CXTPTaskPanelGroup* pFolderComponents = CreateToolboxGroup(ID_TOOLBOXFOLDER_COMPONENTS);
	pFolderComponents->AddLinkItem(ID_TOOLBOXITEM_FILESYSTEMWATCHER     ,34);
	pFolderComponents->AddLinkItem(ID_TOOLBOXITEM_EVENTLOG              ,35);
	pFolderComponents->AddLinkItem(ID_TOOLBOXITEM_DIRECTORYENTRY        ,36);
	pFolderComponents->AddLinkItem(ID_TOOLBOXITEM_DIRECTORYSEARCHER     ,37);
	pFolderComponents->AddLinkItem(ID_TOOLBOXITEM_MESSAGEQUEUE          ,38);
	pFolderComponents->AddLinkItem(ID_TOOLBOXITEM_PERFORMANCECOUNTER    ,39);
	pFolderComponents->AddLinkItem(ID_TOOLBOXITEM_PROCESS               ,40);
	pFolderComponents->AddLinkItem(ID_TOOLBOXITEM_SERVICECONTROLLER     ,41);
	pFolderComponents->AddLinkItem(ID_TOOLBOXITEM_TIMER                 ,42);


	CXTPTaskPanelGroup* pFolderWindowsForms = CreateToolboxGroup(ID_TOOLBOXFOLDER_WINDOWSFORMS);
	pFolderWindowsForms->AddLinkItem(ID_TOOLBOXITEM_BUTTON,             1);
	pFolderWindowsForms->AddLinkItem(ID_TOOLBOXITEM_CHECK_BOX,              2);
	pFolderWindowsForms->AddLinkItem(ID_TOOLBOXITEM_EDIT_CONTROL,           3);
	pFolderWindowsForms->AddLinkItem(ID_TOOLBOXITEM_COMBO_BOX,              4);
	pFolderWindowsForms->AddLinkItem(ID_TOOLBOXITEM_LIST_BOX,               5);
	pFolderWindowsForms->AddLinkItem(ID_TOOLBOXITEM_GROUP_BOX,              6);
	pFolderWindowsForms->AddLinkItem(ID_TOOLBOXITEM_RADIO_BUTTON,           7);
	pFolderWindowsForms->AddLinkItem(ID_TOOLBOXITEM_STATIC_TEXT,            8);
	pFolderWindowsForms->AddLinkItem(ID_TOOLBOXITEM_PICTURE_CONTROL,        9);
	pFolderWindowsForms->AddLinkItem(ID_TOOLBOXITEM_HORIZONTAL_SCROLL_BAR,  10);
	pFolderWindowsForms->AddLinkItem(ID_TOOLBOXITEM_VERTICAL_SCROLL_BAR,    11);
	pFolderWindowsForms->AddLinkItem(ID_TOOLBOXITEM_SLIDER_CONTROL,     12);
	pFolderWindowsForms->AddLinkItem(ID_TOOLBOXITEM_SPIN_CONTROL,           13);
	pFolderWindowsForms->AddLinkItem(ID_TOOLBOXITEM_PROGRESS_CONTROL,       14);
	pFolderWindowsForms->AddLinkItem(ID_TOOLBOXITEM_HOT_KEY,                15);
	pFolderWindowsForms->AddLinkItem(ID_TOOLBOXITEM_LIST_CONTROL,           16);
	pFolderWindowsForms->AddLinkItem(ID_TOOLBOXITEM_TREE_CONTROL,           17);
	pFolderWindowsForms->AddLinkItem(ID_TOOLBOXITEM_TAB_CONTROL,            18);
	pFolderWindowsForms->AddLinkItem(ID_TOOLBOXITEM_ANIMATION_CONTROL,      19);
	pFolderWindowsForms->AddLinkItem(ID_TOOLBOXITEM_RICH_EDIT_CONTROL,      20);
	pFolderWindowsForms->AddLinkItem(ID_TOOLBOXITEM_DATE_TIME_PICKER,       21);
	pFolderWindowsForms->AddLinkItem(ID_TOOLBOXITEM_MONTH_CALENDAR_CONTROL,22);
	pFolderWindowsForms->AddLinkItem(ID_TOOLBOXITEM_IP_ADDRESS_CONTROL, 23);
	pFolderWindowsForms->AddLinkItem(ID_TOOLBOXITEM_EXTENDED_COMBO_BOX, 24);
	pFolderWindowsForms->AddLinkItem(ID_TOOLBOXITEM_CUSTOM_CONTROL,     25);

	CreateToolboxGroup(ID_TOOLBOXFOLDER_CLIPBOARDRING);
	CreateToolboxGroup(ID_TOOLBOXFOLDER_GENERAL);

	pFolderPropertyPanes->SetExpanded(TRUE);

}

BOOL CMainFrame::CreateTaskPanel()
{

	if (!m_wndTaskPanel.Create(WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS|WS_CLIPCHILDREN, CRect(0, 0, 0, 0), &m_wndSplitter, m_wndSplitter.IdFromRowCol(0, 0)))
		return FALSE;

	m_wndTaskPanel.SetOwner(this);

	m_wndTaskPanel.GetImageManager()->SetIcons(IDB_TOOLBOXICONS, 0, 0, CSize(16, 16));
	m_wndTaskPanel.SetBehaviour(xtpTaskPanelBehaviourToolbox);
	m_wndTaskPanel.SetTheme(xtpTaskPanelThemeToolbox);
	m_wndTaskPanel.SetSelectItemOnFocus(TRUE);
	m_wndTaskPanel.AllowDrag(TRUE);


	m_wndSplitter.SetColumnInfo(0, 210, 0);




	return TRUE;
}

BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT /*lpcs*/, CCreateContext* pContext)
{
	m_wndSplitter.CreateStatic(this, 1, 2);

	if (!CreateTaskPanel())
		return FALSE;

	m_wndSplitter.CreateView(0, 1, pContext->m_pNewViewClass, CSize(300, 0), pContext);
	SetActiveView((CView*)m_wndSplitter.GetDlgItem(m_wndSplitter.IdFromRowCol(0, 1)));

	return TRUE;

}

BOOL CMainFrame::IsToggleButtons()
{
	return ((CToolBoxView*)GetActiveView())->m_wndPageLookAndBehaviour.m_bToggleButtons;
}

LRESULT CMainFrame::OnTaskPanelNotify(WPARAM wParam, LPARAM lParam)
{
	switch(wParam)
	{
		case XTP_TPN_CLICK:
			{
				CXTPTaskPanelGroupItem* pItem = (CXTPTaskPanelGroupItem*)lParam;
				TRACE(_T("Click Event: pItem.Caption = %s, pItem.ID = %i\n"), pItem->GetCaption(), pItem->GetID());

				if (IsToggleButtons())
				{
					pItem->SetItemSelected(!pItem->IsItemSelected());
				}
			}
			break;

		case XTP_TPN_RCLICK:

			OnTaskPanelRButtonDown((CXTPTaskPanelItem*)lParam);
			return TRUE;

	}
	return 0;
}

void CMainFrame::OnRenameTaskPanelItem(CXTPTaskPanelItem* pItem)
{
	CXTPTaskPanelGroup* pGroup = pItem->GetItemGroup();

	if (pGroup->GetItemLayout() == xtpTaskItemLayoutImages)
		pGroup->SetItemLayout(xtpTaskItemLayoutDefault);

	m_wndTaskPanel.RenameItem(pItem);
}


void CMainFrame::OnTaskPanelRButtonDown(CXTPTaskPanelItem* pItem)
{
	CPoint pt;
	GetCursorPos(&pt);

	CXTPTaskPanelGroup* pGroup = pItem->GetItemGroup();

	if (pItem->GetID() == ID_TOOLBOXITEM_POINTER)
	{
		pItem = pGroup;

		pt = pGroup->GetCaptionRect().CenterPoint();
		pt.Offset(0, -m_wndTaskPanel.GetScrollOffset());

		m_wndTaskPanel.ClientToScreen(&pt);
	}

	BOOL bGroup = pItem->GetType() == xtpTaskItemTypeGroup;


	ASSERT(pGroup);

	CMenu menu;
	menu.LoadMenu(IDR_POPUP_TOOLBOX);

	CMenu* pMenuPopup = menu.GetSubMenu(bGroup? 1: 0);

	if (pGroup->GetItemLayout() != xtpTaskItemLayoutImages)
		pMenuPopup->CheckMenuItem(ID_TOOLBOX_LISTVIEW, MF_CHECKED|MF_BYCOMMAND);

	CXTPTaskPanelItems* pItems = pItem->GetParentItems();

	if (pItem->GetIndex() == 0)
		pMenuPopup->EnableMenuItem(ID_TOOLBOX_MOVEUP, MF_DISABLED|MF_BYCOMMAND);

	if (pItem->GetIndex() == pItems->GetCount() - 1)
		pMenuPopup->EnableMenuItem(ID_TOOLBOX_MOVEDOWN, MF_DISABLED|MF_BYCOMMAND);

	if (!::IsClipboardFormatAvailable(CXTPTaskPanel::GetClipboardFormat()))
		pMenuPopup->EnableMenuItem(ID_EDIT_PASTE, MF_DISABLED|MF_BYCOMMAND);


	int nReturn = CXTPCommandBars::TrackPopupMenu(pMenuPopup, TPM_LEFTALIGN|TPM_RETURNCMD|TPM_NONOTIFY, pt.x, pt.y, this);

	switch (nReturn)
	{
		case ID_TOOLBOX_RENAME:
			OnRenameTaskPanelItem(pItem);
			break;

		case ID_TOOLBOX_ADDTAB:
			{
				CXTPTaskPanelGroup* pGroup = CreateToolboxGroup(0);
				m_wndTaskPanel.RenameItem(pGroup);
			}
			break;

		case ID_TOOLBOX_LISTVIEW:
			pGroup->SetItemLayout(pGroup->GetItemLayout() == xtpTaskItemLayoutImages?
				xtpTaskItemLayoutDefault: xtpTaskItemLayoutImages);
			break;

		case ID_TOOLBOX_MOVEDOWN:
		case ID_TOOLBOX_MOVEUP:
			{
				CXTPTaskPanelItems* pItems = pItem->GetParentItems();
				pItems->Move(pItem, pItem->GetIndex() + (nReturn == ID_TOOLBOX_MOVEDOWN? +1: -1));
			}
			break;

		case ID_EDIT_COPY:
			pItem->CopyToClipboard();
			break;

		case ID_EDIT_PASTE:
			{
				CXTPTaskPanelItem* pItem = CXTPTaskPanelItem::PasteFromClipboard();
				if (pItem)
				{
					CXTPTaskPanelItems* pItems = pGroup->GetItems();
					pItems->InsertAt(pItems->GetCount(), pItem);
				}
			}
			break;

		case ID_EDIT_CUT:
			pItem->CopyToClipboard();

		case ID_TOOLBOX_DELETE:
			pItem->Remove();
			break;
	}
}

BOOL CMainFrame::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_F2 && GetFocus() == &m_wndTaskPanel)
	{
		CXTPTaskPanelItem* pItem = m_wndTaskPanel.GetFocusedItem();
		if (pItem)
		{
			OnRenameTaskPanelItem(pItem);
			return TRUE;
		}
	}

	return CFrameWnd::PreTranslateMessage(pMsg);
}

void CMainFrame::OnClose()
{
	CXTPPropExchangeXMLNode px(FALSE, 0, _T("Settings"));

	CXTPPropExchangeSection pxTaskPanel(px.GetSection(_T("TaskPanel")));
	m_wndTaskPanel.GetGroups()->DoPropExchange(&pxTaskPanel);

	CXTPPropExchangeSection pxCommandBars(px.GetSection(_T("CommandBars")));
	GetCommandBars()->DoPropExchange(&pxCommandBars);

	px.SaveToFile(m_strIniFileName);

	CFrameWnd::OnClose();
}
