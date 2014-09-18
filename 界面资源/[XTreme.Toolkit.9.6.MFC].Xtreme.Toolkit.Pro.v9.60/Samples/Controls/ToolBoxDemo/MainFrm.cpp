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
#include "ToolBoxDemo.h"

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
	ON_WM_CREATE()
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
	ON_COMMAND(XTP_ID_CUSTOMIZE, OnCustomize)

	ON_MESSAGE(XTWM_OUTBAR_NOTIFY, OnToolBoxNotify)
	ON_MESSAGE(XTPWM_DOCKINGPANE_NOTIFY, OnDockingPaneNotify)

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
	XTAuxData().bXPMode = TRUE;

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
		return -1;

	XTPPaintManager()->SetTheme(XTAuxData().bXPMode? xtpThemeOfficeXP: xtpThemeOffice2000);

	CXTPCommandBars* pCommandBars = GetCommandBars();

	pCommandBars->SetMenu(_T("Menu Bar"), IDR_MAINFRAME);



	CXTPToolBar* pCommandBar = (CXTPToolBar*)pCommandBars->Add(_T("Standard"), xtpBarTop);
	if (!pCommandBar ||
		!pCommandBar->LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;
	}

	// Initialize the toolbox.
	InitToolBoxBar();

	XTPDockingPaneManager()->InstallDockingPanes(this);
	XTPDockingPaneManager()->SetTheme(xtpPaneThemeOffice);

	XTPDockingPaneManager()->CreatePane(IDR_TOOLBOX, CRect(0, 0,200, 120), xtpPaneDockLeft);

	int nIDToolbox[] = {IDR_TOOLBOX};
	XTPDockingPaneManager()->SetIcons(IDB_PANE_ICON, nIDToolbox, 1, RGB(0, 255, 0));


	// Load the previous state for command bars.
	LoadCommandBars(_T("CommandBars"));

	return 0;
}

void CMainFrame::InitToolBoxBar()
{
	// create the image list used by the outloook bar.
	if ( !m_ilToolBox.Create( IDB_TOOLBOX, 16, 1, RGB( 0x00,0xff,0x00 ) ) )
	{
		TRACE0( "Failed to create image list.\n" );
		return;
	}

	// create the outlook bar control used by the docking window.
	if ( !m_wndToolBox.Create( WS_CHILD,
		CRect( 0,0,0,0 ), this, 0xff, OBS_XT_DEFAULT ) )
	{
		TRACE0( "Failed to create outlook bar control.\n" );
		return;
	}

	// set the image lists for the outlook bar.
	m_wndToolBox.SetImageList( &m_ilToolBox, OBS_XT_LARGEICON );
	m_wndToolBox.SetImageList( &m_ilToolBox, OBS_XT_SMALLICON );

	// Add the first folder to the outlook bar.
	int iFolder = m_wndToolBox.AddFolder(_T("Dialog Editor"), 0);

	// Add items to the folder, syntax is folder, index, text, image, and item data.
	m_wndToolBox.InsertItem( iFolder, 1,  _T( "Pointer" ),              0,  NULL );
	m_wndToolBox.InsertItem( iFolder, 2,  _T( "Button" ),               1,  NULL );
	m_wndToolBox.InsertItem( iFolder, 3,  _T( "Check Box" ),                2,  NULL );
	m_wndToolBox.InsertItem( iFolder, 4,  _T( "Edit Control" ),         3,  NULL );
	m_wndToolBox.InsertItem( iFolder, 5,  _T( "Combo Box" ),                4,  NULL );
	m_wndToolBox.InsertItem( iFolder, 6,  _T( "List Box" ),             5,  NULL );
	m_wndToolBox.InsertItem( iFolder, 7,  _T( "Group Box" ),                6,  NULL );
	m_wndToolBox.InsertItem( iFolder, 8,  _T( "Radio Button" ),         7,  NULL );
	m_wndToolBox.InsertItem( iFolder, 9,  _T( "Static Text" ),          8,  NULL );
	m_wndToolBox.InsertItem( iFolder, 10, _T( "Picture Control" ),      9,  NULL );
	m_wndToolBox.InsertItem( iFolder, 11, _T( "Horizontal Scroll Bar" ),    10, NULL );
	m_wndToolBox.InsertItem( iFolder, 12, _T( "Vertical Scroll Bar" ),  11, NULL );
	m_wndToolBox.InsertItem( iFolder, 13, _T( "Slider Control" ),       12, NULL );
	m_wndToolBox.InsertItem( iFolder, 14, _T( "Spin Control" ),         13, NULL );
	m_wndToolBox.InsertItem( iFolder, 15, _T( "Progress Control" ),     14, NULL );
	m_wndToolBox.InsertItem( iFolder, 16, _T( "Hot Key" ),              15, NULL );
	m_wndToolBox.InsertItem( iFolder, 17, _T( "List Control" ),         16, NULL );
	m_wndToolBox.InsertItem( iFolder, 18, _T( "Tree Control" ),         17, NULL );
	m_wndToolBox.InsertItem( iFolder, 19, _T( "Tab Control" ),          18, NULL );
	m_wndToolBox.InsertItem( iFolder, 20, _T( "Animation Control" ),        19, NULL );
	m_wndToolBox.InsertItem( iFolder, 21, _T( "Rich Edit 2.0 Control" ),    20, NULL );
	m_wndToolBox.InsertItem( iFolder, 22, _T( "Date Time Picker" ),     21, NULL );
	m_wndToolBox.InsertItem( iFolder, 23, _T( "Month Calendar Control" ),22, NULL );
	m_wndToolBox.InsertItem( iFolder, 24, _T( "IP Address Control" ),   23, NULL );
	m_wndToolBox.InsertItem( iFolder, 25, _T( "Extended Combo Box" ),   24, NULL );
	m_wndToolBox.InsertItem( iFolder, 26, _T( "Custom Control" ),       25, NULL );

	// Disable items
	m_wndToolBox.EnableItem( iFolder, 2, false );
	m_wndToolBox.EnableItem( iFolder, 3, false );
	m_wndToolBox.EnableItem( iFolder, 7, false );

	// add the second folder
	iFolder = m_wndToolBox.AddFolder(_T("Clipboard Ring"), 0);
	m_wndToolBox.InsertItem( iFolder, 1,  _T( "Pointer" ), 0,  NULL );

	// add the second folder
	iFolder = m_wndToolBox.AddFolder(_T("General"), 0);
	m_wndToolBox.InsertItem( iFolder, 1,  _T( "Pointer" ), 0,  NULL );

	// change the owner to CMainFrame.
	m_wndToolBox.SetOwner( this );
	m_wndToolBox.SetSelItem(0,0);



}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	cs.style |= WS_CLIPSIBLINGS|WS_CLIPCHILDREN;
	// Helps to reduce screen flicker.
	cs.lpszClass = AfxRegisterWndClass(0, NULL, NULL,
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

LRESULT CMainFrame::OnToolBoxNotify(WPARAM wParam, LPARAM lParam)
{
	int nBarAction = (int)wParam;

	// Cast the lParam to a XT_OUTBAR_INFO* struct pointer.
	XT_OUTBAR_INFO* pOBInfo = (XT_OUTBAR_INFO*)lParam;
	ASSERT(pOBInfo);

	switch (nBarAction)
	{
	case OBN_XT_ITEMCLICK:
	case OBN_XT_FOLDERCHANGE:
		{
			CView* pView = GetActiveView();
			if (pView != NULL)
			{
				pView->Invalidate(); // update display.
			}
		}
		break;

	case OBN_XT_ONLABELENDEDIT:
	case OBN_XT_ONGROUPENDEDIT:
	case OBN_XT_DRAGITEM:
	case OBN_XT_ITEMHOVER:
		break;

	case OBN_XT_DELETEITEM:
		if (m_wndToolBox.IsWindowVisible() && AfxMessageBox(_T("Are you sure you want to remove this folder shortcut?"),
			MB_ICONWARNING|MB_YESNO) == IDNO)
		{
			// The user selected No, return FALSE to abort the action.
			return FALSE;
		}
		break;

	case OBN_XT_DELETEFOLDER:
		if (m_wndToolBox.IsWindowVisible() && AfxMessageBox(_T("Are you sure you want to remove the specified folder?"),
			MB_ICONWARNING|MB_YESNO) == IDNO)
		{
			// The user selected No, return FALSE to abort the action.
			return FALSE;
		}
		break;
	}

	return TRUE;
}


LRESULT CMainFrame::OnDockingPaneNotify(WPARAM wParam, LPARAM lParam)
{
	if (wParam == XTP_DPN_SHOWWINDOW)
	{

		CXTPDockingPane* pPane = (CXTPDockingPane*)lParam;

		if (!pPane->IsValid())
		{
			pPane->Attach(&m_wndToolBox);
		}
		return TRUE;
	}
	return FALSE;


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

	// instantiate the customize dialog
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
