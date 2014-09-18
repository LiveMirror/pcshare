// NewToolbar.cpp : implementation file
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
#include "CheckListBox.h"
#include "NewToolbar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNewToolbar dialog


CNewToolbar::CNewToolbar(LPCTSTR lpszToolbar, LPCTSTR lpszTitle, CWnd* pParent /*=NULL*/)
	: CXTResizeDialog(CNewToolbar::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNewToolbar)
	m_strToolbar = lpszToolbar;
	//}}AFX_DATA_INIT
	m_strDlgTitle = lpszTitle;
}


void CNewToolbar::DoDataExchange(CDataExchange* pDX)
{
	CXTResizeDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNewToolbar)
	DDX_Control(pDX, IDOK, m_btnOk);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	DDX_Control(pDX, IDC_EDIT_TOOLBAR, m_editToolbar);
	DDX_Text(pDX, IDC_EDIT_TOOLBAR, m_strToolbar);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CNewToolbar, CXTResizeDialog)
	//{{AFX_MSG_MAP(CNewToolbar)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNewToolbar message handlers

BOOL CNewToolbar::OnInitDialog()
{
	CXTResizeDialog::OnInitDialog();

	// Set focus to the edit control and select
	// its contents.
	m_editToolbar.SetFocus();
	m_editToolbar.SetSel(0,-1);
	SetWindowText(m_strDlgTitle);

	// Set control resizing.
	SetResize(IDC_EDIT_TOOLBAR, SZ_TOP_LEFT, SZ_TOP_RIGHT);
	SetResize(IDOK, SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDCANCEL, SZ_TOP_RIGHT, SZ_TOP_RIGHT);

	// prevent vertical resizing.
	SetFlag(xtResizeNoVertical);

	// Load window placement
	LoadPlacement(_T("CNewToolbar"));

	m_btnOk.SetXButtonStyle(BS_XT_SEMIFLAT);
	m_btnCancel.SetXButtonStyle(BS_XT_SEMIFLAT);

	return FALSE;   // return TRUE unless you set the focus to a control
	                // EXCEPTION: OCX Property Pages should return FALSE
}

void CNewToolbar::OnDestroy()
{
	// Save window placement
	SavePlacement(_T("CNewToolbar"));

	CXTResizeDialog::OnDestroy();
}
