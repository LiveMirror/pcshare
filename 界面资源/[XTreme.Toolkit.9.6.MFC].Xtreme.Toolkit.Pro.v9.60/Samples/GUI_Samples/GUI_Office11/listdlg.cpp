// listdlg.cpp : Defines the class behaviors for the application.
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
#include "resource.h"
#include "listdlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

BEGIN_MESSAGE_MAP(CListDlg, CDialog)
	//{{AFX_MSG_MAP(CListDlg)
	ON_LBN_DBLCLK(IDC_LISTDIALOG_LIST, OnOK)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CListDlg::CListDlg(UINT idStrDlgTitle, UINT idStrListTitle,
	const CStringList& listItems, int nDefSel) : CDialog(CListDlg::IDD),
	m_listItems(listItems)
{
	VERIFY(m_strDlgTitle.LoadString(idStrDlgTitle));
	VERIFY(m_strListTitle.LoadString(idStrListTitle));
	m_nSelection = nDefSel;
}

BOOL CListDlg::OnInitDialog()
{
	SetWindowText(m_strDlgTitle);
// fix this
	SetDlgItemText(IDC_STATIC_HEADING, m_strListTitle);
	CListBox* pListBox = (CListBox*)GetDlgItem(IDC_LISTDIALOG_LIST);
	ASSERT(pListBox != NULL);
	// fill with document templates in list

	POSITION pos = m_listItems.GetHeadPosition();
	while (pos != NULL)
	{
		if ( pListBox->AddString(m_listItems.GetNext(pos)) == -1)
			return FALSE;
	}
	pListBox->SetCurSel(m_nSelection);

	return CDialog::OnInitDialog();
}

void CListDlg::OnOK()
{
	CListBox* pListBox = (CListBox*)GetDlgItem(IDC_LISTDIALOG_LIST);
	ASSERT(pListBox != NULL);
	m_nSelection = pListBox->GetCurSel();
	CDialog::OnOK();
}
