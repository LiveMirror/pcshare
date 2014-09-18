// docopt.cpp : implementation file
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
#include "wordpad.h"
#include "docopt.h"
#include "helpids.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDocOptPage property page

const DWORD CDocOptPage::m_nHelpIDs[] =
{
	IDC_BOX, IDH_COMM_GROUPBOX,
	IDC_WRAP_NONE, IDH_WORDPAD_WRAP_NO,
	IDC_WRAP_WINDOW, IDH_WORDPAD_WRAP_WINDOW,
	IDC_WRAP_RULER, IDH_WORDPAD_WRAP_RULER,
	IDC_BOXT, IDH_COMM_GROUPBOX,
	IDC_CHECK_TOOLBAR, IDH_WORDPAD_CHECK_TOOLBAR,
	IDC_CHECK_FORMATBAR, IDH_WORDPAD_CHECK_FORMATBAR,
	IDC_CHECK_STATUSBAR, IDH_WORDPAD_CHECK_STATUSBAR,
	IDC_CHECK_RULERBAR, IDH_WORDPAD_CHECK_RULERBAR,
	0, 0
};

CDocOptPage::CDocOptPage() : CCSPropertyPage(CDocOptPage::IDD)
{
	//{{AFX_DATA_INIT(CDocOptPage)
	m_nWordWrap = -1;
	m_bFormatBar = FALSE;
	m_bRulerBar = FALSE;
	m_bStatusBar = FALSE;
	m_bToolBar = FALSE;
	//}}AFX_DATA_INIT
}

CDocOptPage::CDocOptPage(UINT nIDCaption) :
	CCSPropertyPage(CDocOptPage::IDD, nIDCaption)
{
	m_nWordWrap = -1;
	m_bFormatBar = FALSE;
	m_bRulerBar = FALSE;
	m_bStatusBar = FALSE;
	m_bToolBar = FALSE;
}

CDocOptPage::~CDocOptPage()
{
}

void CDocOptPage::DoDataExchange(CDataExchange* pDX)
{
	CCSPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDocOptPage)
	DDX_Radio(pDX, IDC_WRAP_NONE, m_nWordWrap);
	DDX_Check(pDX, IDC_CHECK_FORMATBAR, m_bFormatBar);
	DDX_Check(pDX, IDC_CHECK_RULERBAR, m_bRulerBar);
	DDX_Check(pDX, IDC_CHECK_STATUSBAR, m_bStatusBar);
	DDX_Check(pDX, IDC_CHECK_TOOLBAR, m_bToolBar);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDocOptPage, CCSPropertyPage)
	//{{AFX_MSG_MAP(CDocOptPage)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CDocOptPage message handlers

/////////////////////////////////////////////////////////////////////////////
// CEmbeddedOptPage property page

CEmbeddedOptPage::CEmbeddedOptPage() : CDocOptPage(IDS_EMBEDDED_OPTIONS)
{
}

BOOL CEmbeddedOptPage::OnInitDialog()
{
	BOOL b = CDocOptPage::OnInitDialog();
	GetDlgItem(IDC_CHECK_STATUSBAR)->ShowWindow(SW_HIDE);
	return b;
}
