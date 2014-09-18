// TrayIconDlgDlg.cpp : implementation file
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
#include "TrayIconDlg.h"
#include "TrayIconDlgDlg.h"
#include "AboutDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTrayIconDlgDlg dialog

CTrayIconDlgDlg::CTrayIconDlgDlg(CWnd* pParentWnd /*=NULL*/)
	: CXTResizeDialog(CTrayIconDlgDlg::IDD, pParentWnd)
{
	//{{AFX_DATA_INIT(CTrayIconDlgDlg)
	m_bAnimateIcon = FALSE;
	m_bHideIcon = FALSE;
	m_bShowIcon = TRUE;
	m_strBalloonMsg = _T("");
	m_strBalloonTitle = _T("");
	m_iTimeOut = 10;
	m_strToolTip = _T("Power Tools for MFC!");
	m_iBalloonIcon = 1;
	//}}AFX_DATA_INIT

	m_strBalloonMsg.LoadString(IDS_BALLOON_MESSAGE);
	m_strBalloonTitle.LoadString(IDS_BALLOON_TITLE);

	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_bMinimized = false;

	// Enable/Disable XP GUI Mode
	XTAuxData().bXPMode = TRUE;
}


void CTrayIconDlgDlg::DoDataExchange(CDataExchange* pDX)
{
	CXTResizeDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTrayIconDlgDlg)
	DDX_Control(pDX, IDC_TEXT_INFO, m_txtInfo);
	DDX_Control(pDX, IDC_TXT_TOOLTIP, m_txtToolTip);
	DDX_Control(pDX, IDC_TXT_TIMEOUT, m_txtTimeOut);
	DDX_Control(pDX, IDC_TXT_BALLOONTITLE, m_txtBalloonTitle);
	DDX_Control(pDX, IDC_TXT_BALLOONMSG, m_txtBalloonMsg);
	DDX_Control(pDX, IDC_TXT_BALLOONICON, m_txtBalloonIcon);
	DDX_Control(pDX, IDC_EDIT_TOOLTIP, m_editTooltip);
	DDX_Control(pDX, IDC_EDIT_TIMEOUT, m_editTimeout);
	DDX_Control(pDX, IDC_EDIT_BALLOONTITLE, m_editBalloonTitle);
	DDX_Control(pDX, IDC_COMBO_BALLOONICON, m_comboBalloonIcon);
	DDX_Control(pDX, IDC_CHK_SHOWICON, m_chkShowIcon);
	DDX_Control(pDX, IDC_CHK_HIDEICON, m_chkHideIcon);
	DDX_Control(pDX, IDC_CHK_ANIMATEICON, m_chkAnimateIcon);
	DDX_Control(pDX, IDC_BTN_TOOLTIP, m_btnTooltip);
	DDX_Control(pDX, IDC_BTN_SHOWBALLOON, m_btnShowBalloon);
	DDX_Control(pDX, IDC_BTN_MINIMIZETRAY, m_btnMinimizeToTray);
	DDX_Control(pDX, IDC_TXT_BALLOONTIP, m_txtBalloonTip);
	DDX_Control(pDX, IDC_GBOX_BALLOONTIP, m_gboxBalloonTip);
	DDX_Check(pDX, IDC_CHK_ANIMATEICON, m_bAnimateIcon);
	DDX_Check(pDX, IDC_CHK_HIDEICON, m_bHideIcon);
	DDX_Check(pDX, IDC_CHK_SHOWICON, m_bShowIcon);
	DDX_Text(pDX, IDC_EDIT_BALLOONMSG, m_strBalloonMsg);
	DDV_MaxChars(pDX, m_strBalloonMsg, 256);
	DDX_Text(pDX, IDC_EDIT_BALLOONTITLE, m_strBalloonTitle);
	DDX_Text(pDX, IDC_EDIT_TIMEOUT, m_iTimeOut);
	DDX_Text(pDX, IDC_EDIT_TOOLTIP, m_strToolTip);
	DDX_CBIndex(pDX, IDC_COMBO_BALLOONICON, m_iBalloonIcon);
	DDX_Control(pDX, IDC_EDIT_BALLOONMSG, m_editBalloonMsg);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTrayIconDlgDlg, CXTResizeDialog)
	//{{AFX_MSG_MAP(CTrayIconDlgDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_CHK_SHOWICON, OnChkShowicon)
	ON_BN_CLICKED(IDC_CHK_HIDEICON, OnChkHideicon)
	ON_BN_CLICKED(IDC_CHK_ANIMATEICON, OnChkAnimateicon)
	ON_EN_CHANGE(IDC_EDIT_TOOLTIP, OnChangeEditTooltip)
	ON_BN_CLICKED(IDC_BTN_TOOLTIP, OnBtnTooltip)
	ON_EN_CHANGE(IDC_EDIT_BALLOONTITLE, OnChangeEditBalloontitle)
	ON_EN_CHANGE(IDC_EDIT_TIMEOUT, OnChangeEditTimeout)
	ON_EN_CHANGE(IDC_EDIT_BALLOONMSG, OnChangeEditBalloonmsg)
	ON_CBN_SELENDOK(IDC_COMBO_BALLOONICON, OnSelendokComboBalloonicon)
	ON_BN_CLICKED(IDC_BTN_SHOWBALLOON, OnBtnShowBalloon)
	ON_BN_CLICKED(IDC_BTN_MINIMIZETRAY, OnBtnMinimizetray)
	ON_COMMAND(IDR_RESTORE, OnRestore)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTrayIconDlgDlg message handlers

static UINT icoArray[] =
{
	IDI_ICON2,
	IDI_ICON3,
	IDI_ICON4,
	IDI_ICON5
};

BOOL CTrayIconDlgDlg::OnInitDialog()
{
	CXTResizeDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.
	SetIcon(m_hIcon, TRUE);         // Set big icon
	SetIcon(m_hIcon, FALSE);        // Set small icon

	// Set control resizing.
	SetResize(IDC_EDIT_TOOLTIP,      SZ_TOP_LEFT,     SZ_TOP_RIGHT);
	SetResize(IDC_BTN_TOOLTIP,       SZ_TOP_RIGHT,    SZ_TOP_RIGHT);
	SetResize(IDC_BTN_MINIMIZETRAY,  SZ_TOP_RIGHT,    SZ_TOP_RIGHT);
	SetResize(IDOK,                  SZ_TOP_RIGHT,    SZ_TOP_RIGHT);
	SetResize(IDC_TEXT_INFO,         SZ_TOP_LEFT,     SZ_TOP_RIGHT);
	SetResize(IDC_GBOX_FEATURES,     SZ_TOP_LEFT,     SZ_TOP_RIGHT);
	SetResize(IDC_GBOX_BALLOONTIP,   SZ_TOP_LEFT,     SZ_TOP_RIGHT);
	SetResize(IDC_EDIT_BALLOONTITLE, SZ_TOP_LEFT,     SZ_TOP_RIGHT);
	SetResize(IDC_TXT_TIMEOUT,       SZ_TOP_RIGHT,    SZ_TOP_RIGHT);
	SetResize(IDC_EDIT_TIMEOUT,      SZ_TOP_RIGHT,    SZ_TOP_RIGHT);
	SetResize(IDC_EDIT_BALLOONMSG,   SZ_TOP_LEFT,     SZ_BOTTOM_RIGHT);
	SetResize(IDC_TXT_BALLOONICON,   SZ_BOTTOM_LEFT,  SZ_BOTTOM_LEFT);
	SetResize(IDC_COMBO_BALLOONICON, SZ_BOTTOM_LEFT,  SZ_BOTTOM_RIGHT);
	SetResize(IDC_BTN_SHOWBALLOON,   SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);

	// Load window placement
	LoadPlacement(_T("CTrayIconDlgDlg"));

	m_txtBalloonTip.SetFont(&XTAuxData().fontBold);
	EnableControls();

	// display the balloon tooltip.
	OnBtnShowBalloon();

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

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTrayIconDlgDlg::OnDestroy()
{
	// Save window placement
	SavePlacement(_T("CTrayIconDlgDlg"));

	CXTResizeDialog::OnDestroy();
}

void CTrayIconDlgDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CXTResizeDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CTrayIconDlgDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CXTResizeDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CTrayIconDlgDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CTrayIconDlgDlg::OnChkShowicon()
{
	UpdateData();

	if (m_bShowIcon) {
		m_TrayIcon.AddIcon();
	}
	else {
		m_TrayIcon.RemoveIcon();
	}
}

void CTrayIconDlgDlg::OnChkHideicon()
{
	UpdateData();

	if (m_bHideIcon) {
		m_TrayIcon.HideIcon();
	}
	else {
		m_TrayIcon.ShowIcon();
	}
}

void CTrayIconDlgDlg::OnChkAnimateicon()
{
	UpdateData();

	if (m_bAnimateIcon) {
		m_TrayIcon.StartAnimation();
	}
	else {
		m_TrayIcon.StopAnimation();
	}
}

void CTrayIconDlgDlg::OnChangeEditTooltip()
{
	UpdateData();
}

void CTrayIconDlgDlg::OnBtnTooltip()
{
	UpdateData();

	m_TrayIcon.SetTooltipText(m_strToolTip);
}

void CTrayIconDlgDlg::OnChangeEditBalloontitle()
{
	UpdateData();
}

void CTrayIconDlgDlg::OnChangeEditTimeout()
{
	UpdateData();
}

void CTrayIconDlgDlg::OnChangeEditBalloonmsg()
{
	UpdateData();
}

void CTrayIconDlgDlg::OnSelendokComboBalloonicon()
{
	UpdateData();
}

UINT balloonIcon[] =
{
	NIIF_ERROR,  // Error icon.
	NIIF_INFO,   // Information icon.
	NIIF_NONE,   // No icon.
	NIIF_WARNING // Warning icon.
};


void CTrayIconDlgDlg::OnBtnShowBalloon()
{
	UpdateData();

	m_TrayIcon.ShowBalloonTip(m_strBalloonMsg, m_strBalloonTitle,
		balloonIcon[m_iBalloonIcon], m_iTimeOut);

}

void CTrayIconDlgDlg::MinMaxWindow()
{
	m_bMinimized = !m_bMinimized;

	if (m_bMinimized)
	{
		m_TrayIcon.MinimizeToTray(this);
		m_btnMinimizeToTray.SetWindowText(_T("&Maximize from Tray..."));
	}
	else
	{
		m_TrayIcon.MaximizeFromTray(this);
		m_btnMinimizeToTray.SetWindowText(_T("&Minimize to Tray..."));
	}
}

void CTrayIconDlgDlg::OnBtnMinimizetray()
{
	MinMaxWindow();
}

void CTrayIconDlgDlg::EnableControls()
{
	bool bEnable = XTOSVersionInfo()->IsWin2KOrGreater();

	// Requirements:
	// Windows 2000 or later, VC6 with platform SDK or
	// later to use these features.

	m_chkHideIcon.      EnableWindow(bEnable);
	m_txtBalloonTip.    EnableWindow(bEnable);
	m_txtBalloonTitle.  EnableWindow(bEnable);
	m_editBalloonTitle. EnableWindow(bEnable);
	m_txtTimeOut.       EnableWindow(bEnable);
	m_editTimeout.      EnableWindow(bEnable);
	m_txtBalloonMsg.    EnableWindow(bEnable);
	m_editBalloonMsg.   EnableWindow(bEnable);
	m_txtBalloonIcon.   EnableWindow(bEnable);
	m_comboBalloonIcon. EnableWindow(bEnable);
	m_btnShowBalloon.   EnableWindow(bEnable);

	m_editBalloonMsg.SetBackColor(bEnable ?
		GetXtremeColor(COLOR_WINDOW) : GetXtremeColor(COLOR_3DFACE));
}

void CTrayIconDlgDlg::OnRestore()
{
	MinMaxWindow();
}
