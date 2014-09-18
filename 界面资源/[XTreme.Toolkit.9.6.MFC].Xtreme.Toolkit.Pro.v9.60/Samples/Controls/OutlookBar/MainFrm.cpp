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
#include "OutlookBar.h"

#include "MainFrm.h"
#include "LeftView.h"
#include "OutlookBarView.h"

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
	ON_COMMAND(ID_VIEW_XPMODE, OnViewXpmode)
	ON_UPDATE_COMMAND_UI(ID_VIEW_XPMODE, OnUpdateViewXpmode)
	ON_WM_CLOSE()
	ON_COMMAND(ID_VIEW_OFFICE2000, OnViewOffice2000)
	ON_UPDATE_COMMAND_UI(ID_VIEW_OFFICE2000, OnUpdateViewOffice2000)
	ON_COMMAND(ID_VIEW_OFFICE2003, OnViewOffice2003)
	ON_UPDATE_COMMAND_UI(ID_VIEW_OFFICE2003, OnUpdateViewOffice2003)
	//}}AFX_MSG_MAP
	ON_COMMAND(XTP_ID_CUSTOMIZE, OnCustomize)

	ON_MESSAGE(XTWM_OUTBAR_NOTIFY, OnOutbarNotify)
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
	// Enable/Disable XP GUI Mode
	CXTRegistryManager regMgr;
	m_nTheme = regMgr.GetProfileInt(_T("Settings"), _T("nTheme"), xtThemeOfficeXP);
}

CMainFrame::~CMainFrame()
{
	CXTRegistryManager regMgr;
	regMgr.WriteProfileInt(_T("Settings"), _T("nTheme"), m_nTheme);
}

void CMainFrame::SetTheme(XTThemeStyle nTheme)
{
	XTPPaintManager()->SetTheme(
		nTheme == xtThemeDefault? xtpThemeOffice2000:
		nTheme == xtThemeOfficeXP? xtpThemeOfficeXP: xtpThemeOffice2003);

	XTThemeManager()->SetTheme(nTheme);

	m_nTheme = nTheme;
	RedrawWindow( NULL, NULL,
		RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE | RDW_ALLCHILDREN );

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

	SetTheme((XTThemeStyle)m_nTheme);

	CXTPCommandBars* pCommandBars = GetCommandBars();

	pCommandBars->SetMenu(_T("Menu Bar"), IDR_MAINFRAME);


	CXTPToolBar* pCommandBar = (CXTPToolBar*)pCommandBars->Add(_T("Standard"), xtpBarTop);
	if (!pCommandBar ||
		!pCommandBar->LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;
	}

	// Load the previous state for command bars.
	LoadCommandBars(_T("CommandBars"));

	return 0;
}

BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT /*lpcs*/,
	CCreateContext* pContext)
{
	// create splitter window
	if (!m_wndSplitter.CreateStatic(this, 1, 2))
		return FALSE;

	// Here we create the outbar control using the splitter as its parent
	// and setting its id to the first pane.
	if (!m_wndOutlookBar.Create(WS_CHILD|WS_VISIBLE|WS_CLIPCHILDREN, CRect(0,0,0,0),
		&m_wndSplitter, m_wndSplitter.IdFromRowCol(0, 0), OBS_XT_DEFAULT|OBS_XT_ANIMATION/*|OBS_XT_SELHIGHLIGHT*/))
	{
		TRACE0("Failed to create outlook bar.");
		return FALSE;
	}

	// create the tree control.
	if (!m_wndTreeCtrl.Create(WS_BORDER|WS_CHILD|TVS_HASLINES|TVS_LINESATROOT|TVS_HASBUTTONS|TVS_SHOWSELALWAYS,
		CRect(0,0,0,0), &m_wndOutlookBar, IDC_TREE_CTRL))
	{
		TRACE0( "Unable to create tree control.\n" );
		return FALSE;
	}

	// create the tree control.
	// create the image lists used by the caption and tree controls.
	if (!InitializeImageLists())
		return FALSE;

	// fill the tree control.
	if (!InitilalizeTreeControl())
		return FALSE;

	m_wndTreeCtrl.SetOwner(this);

	if (!m_wndSplitter.CreateView(0, 1, RUNTIME_CLASS(COutlookBarView),
		CSize(100, 100), pContext))
	{
		m_wndSplitter.DestroyWindow();
		return FALSE;
	}

	// Set the background and text color of the outlook bar.
//  m_wndOutlookBar.SetBackColor(RGB(0x3a,0x6e,0xa5));
//  m_wndOutlookBar.SetTextColor(RGB(0xff,0xff,0xff));

	// Set the default sizes for the splitter panes.
	CRect r;
	GetClientRect(&r);
	m_wndSplitter.SetColumnInfo( 0, r.Width()/4, 0 );
	m_wndSplitter.RecalcLayout();

	// Add items to the outlook bar.
	InitializeOutlookBar();

	return TRUE;
}

static UINT nIcons[] =
{
	IDI_ICON1, IDI_ICON3,  IDI_ICON4,  IDI_ICON7,
	IDI_ICON8, IDI_ICON2,  IDI_ICON11, IDI_ICON6,
	IDI_ICON9, IDI_ICON10, IDI_ICON5
};

BOOL CMainFrame::InitilalizeTreeControl()
{
	// Set the tree controls image list.
	m_wndTreeCtrl.SetImageList (&m_ilTreeIcons, TVSIL_NORMAL);

	// TODO: Add items to the tree control.
	HTREEITEM hItem = m_wndTreeCtrl.InsertItem( _T("Outlook Today"), 0, 0);

	m_wndTreeCtrl.InsertItem (_T("Calendar"), 1, 1, hItem);
	m_wndTreeCtrl.InsertItem (_T("Contacts"), 2, 2, hItem);
	m_wndTreeCtrl.InsertItem (_T("Deleted Items"), 3, 3, hItem);
	m_wndTreeCtrl.InsertItem (_T("Drafts"), 4, 4, hItem);
	m_wndTreeCtrl.InsertItem (_T("Inbox"), 5, 5, hItem);
	m_wndTreeCtrl.InsertItem (_T("Journal"), 6, 6, hItem);
	m_wndTreeCtrl.InsertItem (_T("Notes"), 7, 7, hItem);
	m_wndTreeCtrl.InsertItem (_T("Outbox"), 8, 8, hItem);
	m_wndTreeCtrl.InsertItem (_T("Sent Items"), 9, 9, hItem);
	m_wndTreeCtrl.InsertItem (_T("Tasks"), 10, 10, hItem);

	m_wndTreeCtrl.SetItemBold (hItem, TRUE);
	m_wndTreeCtrl.Expand (hItem, TVE_EXPAND);
	m_wndTreeCtrl.SelectItem(hItem);

	return TRUE;
}

BOOL CMainFrame::InitializeImageLists()
{
	// Create the image list used by the caption button.
	if (!m_ilCaption.Create(XT_IDB_BTN_OUT, 16, 1, RGB(255,0,255)))
		return FALSE;

	// Create the image list used by the tree control.
	if (!m_ilTreeIcons.Create (16, 16, TRUE, 2, 1))
		return FALSE;

	// TODO: Load your icons that you want displayed in the tree
	// control here, then add them to the image list.
	int i;
	for (i = 0; i < _countof(nIcons); ++i)
	{
		HICON hIcon = AfxGetApp()->LoadIcon(nIcons[i]);
		ASSERT(hIcon);

		m_ilTreeIcons.Add(hIcon);
	}

	return TRUE;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;

	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	cs.lpszClass = AfxRegisterWndClass( 0, NULL, NULL,
		AfxGetApp()->LoadIcon(IDR_MAINFRAME));

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

void CMainFrame::InitializeOutlookBar()
{
	// Create the image lists used by the outlook bar.
	m_ImageSmall.Create (16, 16, ILC_COLOR16|ILC_MASK, 2, 1);
	m_ImageLarge.Create (32, 32, ILC_COLOR16|ILC_MASK, 2, 1);

	// initiailize the image lists.
	int i;
	for (i = 0; i < _countof(nIcons); ++i)
	{
		HICON hIcon = AfxGetApp()->LoadIcon(nIcons[i]);
		ASSERT(hIcon);

		m_ImageSmall.Add(hIcon);
		m_ImageLarge.Add(hIcon);
	}

	int iFolder; // index of the added folder

	// set the image lists for the outlook bar.
	m_wndOutlookBar.SetImageList(&m_ImageLarge, OBS_XT_LARGEICON);
	m_wndOutlookBar.SetImageList(&m_ImageSmall, OBS_XT_SMALLICON);

	// Add the first folder to the outlook bar.
	iFolder = m_wndOutlookBar.AddFolder(_T("Shortcuts 1"), 0);

	// Add items to the folder, syntax is folder, index, text, image, and item data.
	m_wndOutlookBar.InsertItem(iFolder, 1, _T("Item 1"), 0, NULL);
	m_wndOutlookBar.InsertItem(iFolder, 2, _T("Disabled Item 2"), 1, NULL);
	m_wndOutlookBar.InsertItem(iFolder, 3, _T("Disabled Item 3"), 2, NULL);
	m_wndOutlookBar.InsertItem(iFolder, 4, _T("Item 4"), 3, NULL);
	m_wndOutlookBar.InsertItem(iFolder, 5, _T("Item 5"), 4, NULL);
	m_wndOutlookBar.InsertItem(iFolder, 6, _T("Item 6"), 5, NULL);

	// Add the second folder to the outlook bar.
	iFolder = m_wndOutlookBar.AddFolder(_T("Shortcuts 2"), 1);

	// Add items to the folder, syntax is folder, index, text, image, and item data.
	m_wndOutlookBar.InsertItem(iFolder, 1, _T("Item 1"), 0, NULL);
	m_wndOutlookBar.InsertItem(iFolder, 2, _T("Item 2"), 1, NULL);

	// Add the tree control to the outlook bar.
	iFolder = m_wndOutlookBar.AddFolderBar(_T("Tree Control"), &m_wndTreeCtrl );

	// Set the default font used by the outlook bar.
	m_wndOutlookBar.SetFontX(&XTAuxData().font);

	// We want to receive notification messages.
	m_wndOutlookBar.SetOwner(this);

	// Select the first folder in the bar.
	m_wndOutlookBar.SetSelFolder(iFolder);

	// Disable item 2 and 3
	m_wndOutlookBar.EnableItem( 0, 1, false );
	m_wndOutlookBar.EnableItem( 0, 2, false );
}

LRESULT CMainFrame::OnOutbarNotify(WPARAM wParam, LPARAM lParam)
{
	int nBarAction = (int)wParam;

	// Cast the lParam to a XT_OUTBAR_INFO* struct pointer.
	XT_OUTBAR_INFO* pOBInfo = (XT_OUTBAR_INFO*)lParam;
	ASSERT(pOBInfo);

	switch (nBarAction)
	{
	case OBN_XT_ITEMCLICK:
		{
		CString strMessage;
		strMessage.Format( _T("Item selected: %d, Name: %s.\n"), pOBInfo->nIndex, pOBInfo->lpszText);
		AfxMessageBox(strMessage);
		break;
		}

	case OBN_XT_FOLDERCHANGE:
		TRACE2( "Folder selected: %d, Name: %s.\n", pOBInfo->nIndex, pOBInfo->lpszText);
		break;

	case OBN_XT_ONLABELENDEDIT:
		TRACE2( "Item edited: %d, New name: %s.\n", pOBInfo->nIndex, pOBInfo->lpszText);
		break;

	case OBN_XT_ONGROUPENDEDIT:
		TRACE2( "Folder edited: %d, New name: %s.\n", pOBInfo->nIndex, pOBInfo->lpszText);
		break;

	case OBN_XT_DRAGITEM:
		TRACE3( "Dragging From: %d, To: %d, Name: %s.\n", pOBInfo->nDragFrom, pOBInfo->nDragTo, pOBInfo->lpszText);
		break;

	case OBN_XT_ITEMHOVER:
		TRACE2( "Hovering Item: %d, Name: %s.\n", pOBInfo->nIndex, pOBInfo->lpszText);
		break;

	case OBN_XT_DELETEITEM:
		if (m_wndOutlookBar.IsWindowVisible() && AfxMessageBox(_T("Are you sure you want to remove this folder shortcut?"),
			MB_ICONWARNING|MB_YESNO) == IDNO)
		{
			// The user selected No, return FALSE to abort the action.
			return FALSE;
		}
		TRACE2( "Item deleted: %d, Name: %s.\n", pOBInfo->nIndex, pOBInfo->lpszText);
		break;

	case OBN_XT_DELETEFOLDER:
		if (m_wndOutlookBar.IsWindowVisible() && AfxMessageBox(_T("Are you sure you want to remove the specified folder?"),
			MB_ICONWARNING|MB_YESNO) == IDNO)
		{
			// The user selected No, return FALSE to abort the action.
			return FALSE;
		}
		TRACE2( "Folder deleted: %d, Name: %s.\n", pOBInfo->nIndex, pOBInfo->lpszText);
		break;
	}

	return TRUE;
}




BOOL CMainFrame::LoadFrame(UINT nIDResource, DWORD dwDefaultStyle, CWnd* pParentWnd, CCreateContext* pContext)
{
	if ( !CFrameWnd::LoadFrame(nIDResource, dwDefaultStyle, pParentWnd, pContext) )
	{
		return FALSE;
	}



	return TRUE;
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


void CMainFrame::OnViewXpmode()
{
	SetTheme(xtThemeOfficeXP);
}

void CMainFrame::OnUpdateViewXpmode(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_nTheme == xtThemeOfficeXP? TRUE: FALSE);
}

void CMainFrame::OnViewOffice2000()
{
	SetTheme(xtThemeDefault);

}

void CMainFrame::OnUpdateViewOffice2000(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_nTheme == xtThemeDefault? TRUE: FALSE);

}

void CMainFrame::OnViewOffice2003()
{
	SetTheme(xtThemeOffice2003);

}

void CMainFrame::OnUpdateViewOffice2003(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_nTheme == xtThemeOffice2003? TRUE: FALSE);
}
