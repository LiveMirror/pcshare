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
#include "TrayIconDemo.h"

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
	ON_COMMAND(IDR_RESTORE, OnRestore)
	ON_COMMAND(ID_VIEW_XPMODE, OnViewXpmode)
	ON_UPDATE_COMMAND_UI(ID_VIEW_XPMODE, OnUpdateViewXpmode)
	ON_COMMAND(ID_PROPERTIES, OnProperties)
	ON_COMMAND(ID_APP_EXIT, OnAppExit)
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
	ON_COMMAND(XTP_ID_CUSTOMIZE, OnCustomize)

	ON_MESSAGE(TIN_XT_TRAYICON, OnTrayIconNotify)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

static UINT icoArray[] =
{
	IDI_ICON2,
	IDI_ICON3,
	IDI_ICON4,
	IDI_ICON5
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// Enable/Disable XP GUI Mode
	CXTRegistryManager regMgr;
	XTAuxData().bXPMode = regMgr.GetProfileInt(_T("Settings"), _T("bXPMode"), TRUE);
}

CMainFrame::~CMainFrame()
{
	CXTRegistryManager regMgr;
	regMgr.WriteProfileInt(_T("Settings"), _T("bXPMode"), XTAuxData().bXPMode);
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

	XTPPaintManager()->SetTheme(XTAuxData().bXPMode? xtpThemeOffice2003: xtpThemeNativeWinXP);

	CXTPCommandBars* pCommandBars = GetCommandBars();

	pCommandBars->SetMenu(_T("Menu Bar"), IDR_MAINFRAME);


	CXTPToolBar* pCommandBar = (CXTPToolBar*)pCommandBars->Add(_T("Standard"), xtpBarTop);
	if (!pCommandBar ||
		!pCommandBar->LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;
	}
	((CXTPControlButton*)pCommandBar->GetControls()->GetAt(8))->SetStyle(xtpButtonIconAndCaption);


	// Create the tray icon.
	if (!m_TrayIcon.Create(
		_T("Power Tools for MFC!"), // Toolktip text
		this,                       // Parent window
		IDI_TRAYICON,               // Icon resource ID
		IDR_POPUP_TRAY,             // Resource ID of popup menu
		IDR_RESTORE,                // Default menu item for popup menu
		false))                     // True if default menu item is located by position
	{
		TRACE0("Failed to create tray icon\n");
		return -1;
	}

	m_TrayIcon.SetAnimationIcons(
		icoArray, _countof(icoArray));

	// Display the properties dialog.
	::PostMessage(m_hWnd, WM_COMMAND, ID_PROPERTIES, 0);

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

void CMainFrame::OnRestore()
{
	m_propDlg.MinMaxWindow();
}

void CMainFrame::OnViewXpmode()
{
	XTAuxData().bXPMode = !XTAuxData().bXPMode;
	XTPPaintManager()->SetTheme(XTAuxData().bXPMode? xtpThemeOffice2003: xtpThemeNativeWinXP);

	RedrawWindow( NULL, NULL,
		RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE | RDW_ALLCHILDREN );
}

void CMainFrame::OnUpdateViewXpmode(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(XTAuxData().bXPMode);
}

LRESULT CMainFrame::OnTrayIconNotify(WPARAM /*wParam*/, LPARAM lParam)
{
	//UINT uID       = (UINT)wParam;  // resource ID of the tray icon.
	UINT uMouseMsg = (UINT)lParam;  // mouse message that was sent.


	// We can let the tray icon control handle our context menu and
	// mouse double click events, but we want handle our balloon tip
	// notifications, so we will return 1 to let the tray icon control
	// know that we have handled these messages already...

	switch( uMouseMsg )
	{

	// Sent when the balloon is shown (balloons are queued).
	case NIN_BALLOONSHOW:
		{
		}
		return 1;

	// Sent when the balloon disappears—for example, when the
	// icon is deleted. This message is not sent if the balloon
	// is dismissed because of a timeout or a mouse click.
	case NIN_BALLOONHIDE:
		{
		}
		return 1;

	// Sent when the balloon is dismissed because of a timeout.
	case NIN_BALLOONTIMEOUT:
		{
		}
		return 1;

	// Sent when the balloon is dismissed because of a mouse click.
	case NIN_BALLOONUSERCLICK:
		{
			CXTHyperLink hyperlink;
			hyperlink.GotoURL(_T("http://www.codejock.com/"));
		}
		return 1;


	case WM_RBUTTONUP:
		{
			CMenu menu;
			if (!menu.LoadMenu(IDR_POPUP_TRAY)) {
				return 0;
			}

			CMenu* pSubMenu = menu.GetSubMenu(0);
			if (pSubMenu == NULL) {
				return 0;
			}

			::SetMenuDefaultItem(pSubMenu->m_hMenu, IDR_RESTORE, FALSE);

			// Display the menu at the current mouse location. There's a "bug"
			// (Microsoft calls it a feature) in Windows 95 that requires calling
			// SetForegroundWindow. To find out more, search for Q135788 in MSDN.
			//
			CPoint pos;
			GetCursorPos(&pos);
			::SetForegroundWindow(m_hWnd);

			CXTPCommandBars::TrackPopupMenu(pSubMenu, 0, pos.x, pos.y,this);

			::PostMessage(m_hWnd, WM_NULL, 0, 0);

			menu.DestroyMenu();
		}
		return 1;
	}

	return 0;
}

void CMainFrame::OnProperties()
{
	// Display the properties dialog.
	m_propDlg.DoModal(this);
}

void CMainFrame::OnAppExit()
{
	// if the properties dialog is open, close it...
	if (::IsWindow(m_propDlg.m_hWnd)) {
		::SendMessage(m_propDlg.m_hWnd, WM_COMMAND, IDOK, 0);
	}

	// post a message to ourself to close
	::PostMessage(m_hWnd, WM_CLOSE, 0, 0);
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
