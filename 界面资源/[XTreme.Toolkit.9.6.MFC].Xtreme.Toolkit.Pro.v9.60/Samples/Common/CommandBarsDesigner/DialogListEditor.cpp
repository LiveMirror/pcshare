// DialogListEditor.cpp : implementation file
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
#include "DialogListEditor.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogListEditor dialog


CDialogListEditor::CDialogListEditor(CXTPControlComboBox* pCombo, CWnd* pParent /*=NULL*/)
	: CDialog(CDialogListEditor::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogListEditor)
	m_strList = _T("");
	//}}AFX_DATA_INIT
	m_pCombo = pCombo;
}


void CDialogListEditor::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogListEditor)
	DDX_Control(pDX, IDC_EDIT_LIST, m_wndList);
	DDX_Text(pDX, IDC_EDIT_LIST, m_strList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogListEditor, CDialog)
	//{{AFX_MSG_MAP(CDialogListEditor)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogListEditor message handlers

BOOL CDialogListEditor::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_strList = _T("");
	for (int i = 0; i < m_pCombo->GetCount(); i++)
	{
		CString str;
		m_pCombo->GetLBText(i, str);
		if (i > 0) str = _T("\r\n") + str;
		m_strList += str;
	}
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDialogListEditor::OnOK()
{
	UpdateData();
	m_pCombo->ResetContent();


	int nIndex = 0;
	do
	{
		nIndex = m_strList.Find(_T('\n'));
		if (nIndex != -1)
		{
			CString str = m_strList.Left(nIndex - 1);
			XTPStringDelete(m_strList, 0, nIndex + 1);
			if (!str.IsEmpty()) m_pCombo->AddString(str);
		} else
		{
			if (!m_strList.IsEmpty()) m_pCombo->AddString(m_strList);
		}
	} while (nIndex != -1);

	CDialog::OnOK();
}
