// MyOpenUrlDlg.cpp : implementation file
//

#include "stdafx.h"
#include "PcShare.h"
#include "MyOpenUrlDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyOpenUrlDlg dialog


CMyOpenUrlDlg::CMyOpenUrlDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMyOpenUrlDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMyOpenUrlDlg)
	m_Url = _T("");
	//}}AFX_DATA_INIT
}


void CMyOpenUrlDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMyOpenUrlDlg)
	DDX_Text(pDX, IDC_EDIT_FILE, m_Url);
	DDV_MaxChars(pDX, m_Url, 255);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMyOpenUrlDlg, CDialog)
	//{{AFX_MSG_MAP(CMyOpenUrlDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyOpenUrlDlg message handlers

