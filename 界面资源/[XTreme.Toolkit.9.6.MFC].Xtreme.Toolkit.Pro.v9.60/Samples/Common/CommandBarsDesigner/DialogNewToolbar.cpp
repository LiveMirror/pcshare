// DialogNewToolbar.cpp : implementation file
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
#include "commandbarsdesigner.h"
#include "DialogNewToolbar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogNewToolbar dialog


CDialogNewToolbar::CDialogNewToolbar(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogNewToolbar::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogNewToolbar)
	m_strCaption = _T("");
	m_strId = _T("");
	//}}AFX_DATA_INIT
	m_bGenerateId = TRUE;
}


void CDialogNewToolbar::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogNewToolbar)
	DDX_Text(pDX, IDC_EDIT_CAPTION, m_strCaption);
	DDX_Text(pDX, IDC_EDIT_ID, m_strId);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogNewToolbar, CDialog)
	//{{AFX_MSG_MAP(CDialogNewToolbar)
	ON_EN_CHANGE(IDC_EDIT_CAPTION, OnChangeEditCaption)
	ON_EN_CHANGE(IDC_EDIT_ID, OnChangeEditId)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogNewToolbar message handlers

void CDialogNewToolbar::OnChangeEditCaption()
{
	UpdateData();
	GenerateID();

}

void CDialogNewToolbar::OnChangeEditId()
{
	m_bGenerateId = FALSE;

}

void CDialogNewToolbar::GenerateID()
{
	if (m_bGenerateId)
	{
		CString strCaption = StripChars(m_strCaption);

		m_strId = _T("IDR");

		if (!strCaption.IsEmpty()) m_strId += _T("_") + strCaption;

		UpdateData(FALSE);
	}
}
void CDialogNewToolbar::OnOK()
{
	UpdateData();

	if (m_strCaption.IsEmpty())
	{
		GetDlgItem(IDC_EDIT_CAPTION)->SetFocus();
		return;
	}
	if (m_strId.IsEmpty())
	{
		GetDlgItem(IDC_EDIT_ID)->SetFocus();
		return;
	}

	CDialog::OnOK();
}
