// MyClientTitleDlg.cpp : implementation file
//

#include "stdafx.h"
#include "PcShare.h"
#include "MyClientTitleDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyClientTitleDlg dialog


CMyClientTitleDlg::CMyClientTitleDlg(LPCTSTR Text, LPCTSTR Title, LPCTSTR Value)
	: CDialog(CMyClientTitleDlg::IDD, NULL)
{
	//{{AFX_DATA_INIT(CMyClientTitleDlg)
	m_Value = Value;
	m_Text = Text;
	//}}AFX_DATA_INIT
	m_Title = Title;
}


void CMyClientTitleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMyClientTitleDlg)
	DDX_Text(pDX, IDC_EDIT_VALUE, m_Value);
	DDV_MaxChars(pDX, m_Title, 250);
	DDX_Text(pDX, IDC_STATIC_TEXT, m_Text);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CMyClientTitleDlg, CDialog)
	//{{AFX_MSG_MAP(CMyClientTitleDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyClientTitleDlg message handlers


BOOL CMyClientTitleDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	SetWindowText(m_Title);	
	return TRUE;  
}

