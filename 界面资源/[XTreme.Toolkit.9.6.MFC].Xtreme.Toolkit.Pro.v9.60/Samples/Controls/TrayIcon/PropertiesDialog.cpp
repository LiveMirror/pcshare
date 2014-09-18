// PropertiesDialog.cpp : implementation file
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
#include "PropertiesDialog.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

bool CPropertiesDialog::m_bMinimized = false;
CPropertiesDialog *CPropertiesDialog::m_pInstance = NULL;

/////////////////////////////////////////////////////////////////////////////
// CPropertiesDialog dialog

CPropertiesDialog::CPropertiesDialog(CWnd* pParentWnd /*=NULL*/)
	: CXTResizeDialog(CPropertiesDialog::IDD, pParentWnd)
{
	//{{AFX_DATA_INIT(CPropertiesDialog)
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

	m_pParentWnd = pParentWnd;

	m_pInstance = this;
}


void CPropertiesDialog::DoDataExchange(CDataExchange* pDX)
{
	CXTResizeDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropertiesDialog)
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


BEGIN_MESSAGE_MAP(CPropertiesDialog, CXTResizeDialog)
	//{{AFX_MSG_MAP(CPropertiesDialog)
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
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropertiesDialog message handlers

BOOL CPropertiesDialog::OnInitDialog()
{
	CXTResizeDialog::OnInitDialog();

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
	LoadPlacement(_T("CPropertiesDialog"));

	m_txtBalloonTip.SetFont(&XTAuxData().fontBold);
	EnableControls();

	// display the balloon tooltip.
	OnBtnShowBalloon();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPropertiesDialog::OnDestroy()
{
	// Save window placement
	SavePlacement(_T("CPropertiesDialog"));

	m_pInstance = NULL;

	CXTResizeDialog::OnDestroy();
}

void CPropertiesDialog::OnChkShowicon()
{
	UpdateData();

	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	ASSERT_VALID(pMainFrame);

	if (m_bShowIcon) {
		pMainFrame->m_TrayIcon.AddIcon();
	}
	else {
		pMainFrame->m_TrayIcon.RemoveIcon();
	}
}

void CPropertiesDialog::OnChkHideicon()
{
	UpdateData();

	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	ASSERT_VALID(pMainFrame);

	if (m_bHideIcon) {
		pMainFrame->m_TrayIcon.HideIcon();
	}
	else {
		pMainFrame->m_TrayIcon.ShowIcon();
	}
}

void CPropertiesDialog::OnChkAnimateicon()
{
	UpdateData();

	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	ASSERT_VALID(pMainFrame);

	if (m_bAnimateIcon) {
		pMainFrame->m_TrayIcon.StartAnimation();
	}
	else {
		pMainFrame->m_TrayIcon.StopAnimation();
	}
}

void CPropertiesDialog::OnChangeEditTooltip()
{
	UpdateData();
}

void CPropertiesDialog::OnBtnTooltip()
{
	UpdateData();

	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	ASSERT_VALID(pMainFrame);

	pMainFrame->m_TrayIcon.SetTooltipText(m_strToolTip);
}

void CPropertiesDialog::OnChangeEditBalloontitle()
{
	UpdateData();
}

void CPropertiesDialog::OnChangeEditTimeout()
{
	UpdateData();
}

void CPropertiesDialog::OnChangeEditBalloonmsg()
{
	UpdateData();
}

void CPropertiesDialog::OnSelendokComboBalloonicon()
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

void CPropertiesDialog::OnBtnShowBalloon()
{
	UpdateData();

	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	ASSERT_VALID(pMainFrame);

	pMainFrame->m_TrayIcon.ShowBalloonTip(m_strBalloonMsg, m_strBalloonTitle,
		balloonIcon[m_iBalloonIcon], m_iTimeOut);

}

void CPropertiesDialog::MinMaxWindow()
{
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	ASSERT_VALID(pMainFrame);

	m_bMinimized = !m_bMinimized;

	if (m_bMinimized)
	{
		pMainFrame->m_TrayIcon.MinimizeToTray(pMainFrame);
		if (m_pInstance)
		{
			m_pInstance->m_btnMinimizeToTray.SetWindowText(
				_T("&Maximize from Tray..."));
		}
	}
	else
	{
		pMainFrame->m_TrayIcon.MaximizeFromTray(pMainFrame);
		if (m_pInstance)
		{
			m_pInstance->m_btnMinimizeToTray.SetWindowText(_T("&Minimize to Tray..."));
		}
	}
}

void CPropertiesDialog::OnBtnMinimizetray()
{
	MinMaxWindow();
}

void CPropertiesDialog::EnableControls()
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

INT_PTR CPropertiesDialog::DoModal(CWnd* pParentWnd)
{
	m_pParentWnd = pParentWnd;
	return CXTResizeDialog::DoModal();
}
