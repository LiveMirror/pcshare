// filenewd.cpp : implementation file
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
#include "filenewd.h"
#include "helpids.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

DWORD const CFileNewDialog::m_nHelpIDs[] =
{
	IDC_DATEDIALOG_LIST, IDH_WORDPAD_FILENEW_DOC,
	0, 0
};

/////////////////////////////////////////////////////////////////////////////
// CFileNewDialog dialog

CFileNewDialog::CFileNewDialog(CWnd* pParent /*=NULL*/)
	: CCSDialog(CFileNewDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFileNewDialog)
	m_nSel = -1;
	//}}AFX_DATA_INIT
}


void CFileNewDialog::DoDataExchange(CDataExchange* pDX)
{
	CCSDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFileNewDialog)
	DDX_Control(pDX, IDC_DATEDIALOG_LIST, m_listbox);
	DDX_LBIndex(pDX, IDC_DATEDIALOG_LIST, m_nSel);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFileNewDialog, CCSDialog)
	//{{AFX_MSG_MAP(CFileNewDialog)
	ON_LBN_DBLCLK(IDC_DATEDIALOG_LIST, OnDblclkDatedialogList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CFileNewDialog message handlers

BOOL CFileNewDialog::OnInitDialog()
{
	CCSDialog::OnInitDialog();

	CString str;
	VERIFY(str.LoadString(IDS_WORD6_DOCUMENT));
	m_listbox.AddString(str);
	VERIFY(str.LoadString(IDS_RTF_DOCUMENT));
	m_listbox.AddString(str);
	VERIFY(str.LoadString(IDS_TEXT_DOCUMENT));
	m_listbox.AddString(str);
	m_listbox.SetCurSel(0);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CFileNewDialog::OnDblclkDatedialogList()
{
	OnOK();
}
