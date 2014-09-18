// StatusInfoDlg.cpp : implementation file
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
#include "statusbar.h"
#include "MainFrm.h"
#include "StatusInfoDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStatusInfoDlg dialog


CStatusInfoDlg::CStatusInfoDlg(
	CEdit         &wndEditCtrl,
	CAnimateCtrl  &wndAnimCtrl,
	CProgressCtrl &wndProgCtrl,
	CXTPStatusBarLogoPane   &wndLogoPane,
	CXTPStatusBarIconPane   &wndIconPane,
	CWnd* pParent /*=NULL*/)
: CDialog(CStatusInfoDlg::IDD, pParent)
, m_wndEditCtrl(wndEditCtrl)
, m_wndAnimCtrl(wndAnimCtrl)
, m_wndProgCtrl(wndProgCtrl)
, m_wndLogoPane(wndLogoPane)
, m_wndIconPane(wndIconPane)
{
	//{{AFX_DATA_INIT(CStatusInfoDlg)
	m_bAnim = FALSE;
	m_bEdit = FALSE;
	m_bLogo = FALSE;
	m_bProgress = FALSE;
	m_bIcon = FALSE;
	m_bEnableIcon = TRUE;
	m_bPlayAnim = TRUE;
	//}}AFX_DATA_INIT

	m_pMainFrame = (CMainFrame*)pParent;
}


void CStatusInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStatusInfoDlg)
	DDX_Control(pDX, IDC_CHK_PLAYANIM, m_chkPlayAnim);
	DDX_Control(pDX, IDC_CHK_ENABLEICON, m_chkEnableIcon);
	DDX_Control(pDX, IDC_BTN_TESPROG, m_btnTestProg);
	DDX_Check(pDX, IDC_CHK_ANIM, m_bAnim);
	DDX_Check(pDX, IDC_CHK_EDIT, m_bEdit);
	DDX_Check(pDX, IDC_CHK_LOGO, m_bLogo);
	DDX_Check(pDX, IDC_CHK_PROGRESS, m_bProgress);
	DDX_Check(pDX, IDC_CHK_ICON, m_bIcon);
	DDX_Check(pDX, IDC_CHK_ENABLEICON, m_bEnableIcon);
	DDX_Check(pDX, IDC_CHK_PLAYANIM, m_bPlayAnim);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CStatusInfoDlg, CDialog)
	//{{AFX_MSG_MAP(CStatusInfoDlg)
	ON_BN_CLICKED(IDC_CHK_ANIM, OnChkAnim)
	ON_BN_CLICKED(IDC_CHK_EDIT, OnChkEdit)
	ON_BN_CLICKED(IDC_CHK_LOGO, OnChkLogo)
	ON_BN_CLICKED(IDC_CHK_PROGRESS, OnChkProgress)
	ON_BN_CLICKED(IDC_BTN_TESPROG, OnBtnTesprog)
	ON_BN_CLICKED(IDC_CHK_ICON, OnChkIcon)
	ON_BN_CLICKED(IDC_CHK_ENABLEICON, OnChkEnableicon)
	ON_BN_CLICKED(IDC_CHK_PLAYANIM, OnChkPlayanim)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStatusInfoDlg message handlers

void CStatusInfoDlg::OnChkAnim()
{
	// TODO: Add your control notification handler code here
	UpdateData();
	m_chkPlayAnim.EnableWindow(m_bAnim);

	if (m_bAnim)
	{
		m_pMainFrame->AddAnimation();
		OnChkPlayanim();
	}
	else
		m_pMainFrame->RemoveAnimation();
}

void CStatusInfoDlg::OnChkEdit()
{
	// TODO: Add your control notification handler code here
	UpdateData();
	if (m_bEdit)
		m_pMainFrame->AddEdit();
	else
		m_pMainFrame->RemoveEdit();
}

void CStatusInfoDlg::OnChkLogo()
{
	// TODO: Add your control notification handler code here
	UpdateData();
	if (m_bLogo)
		m_pMainFrame->AddLogo();
	else
		m_pMainFrame->RemoveLogo();
}

void CStatusInfoDlg::OnChkProgress()
{
	// TODO: Add your control notification handler code here
	UpdateData();
	m_btnTestProg.EnableWindow(m_bProgress);

	if (m_bProgress)
		m_pMainFrame->AddProgress();
	else
		m_pMainFrame->RemoveProgress();
}

void CStatusInfoDlg::OnBtnTesprog()
{
	// TODO: Add your control notification handler code here
	m_pMainFrame->TestProgress();
}

BOOL CStatusInfoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO: Add extra initialization here
	m_btnTestProg.EnableWindow(m_bProgress);
	m_chkEnableIcon.EnableWindow(m_bIcon);
	m_chkPlayAnim.EnableWindow(m_bAnim);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CStatusInfoDlg::OnChkIcon()
{
	// TODO: Add your control notification handler code here
	UpdateData();
	m_chkEnableIcon.EnableWindow(m_bIcon);

	if (m_bIcon)
	{
		m_pMainFrame->AddIcon();
		OnChkEnableicon();
	}
	else
	{
		m_pMainFrame->RemoveIcon();
	}
}

void CStatusInfoDlg::OnChkEnableicon()
{
	// TODO: Add your control notification handler code here
	UpdateData();

	m_wndIconPane.EnableWindow(m_bEnableIcon);
	m_wndIconPane.Invalidate();
}

void CStatusInfoDlg::OnChkPlayanim()
{
	// TODO: Add your control notification handler code here
	UpdateData();

	if (m_bPlayAnim) {
		m_wndAnimCtrl.Play(0, (UINT)-1, (UINT)-1);
	}
	else {
		m_wndAnimCtrl.Stop();
	}
}
