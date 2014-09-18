// MyStartSocksDlg.cpp : implementation file
//

#include "stdafx.h"
#include "PcShare.h"
#include "MyStartSocksDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyStartSocksDlg dialog


CMyStartSocksDlg::CMyStartSocksDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMyStartSocksDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMyStartSocksDlg)
	m_Pass = _T("");
	m_Port = _T("1080");
	m_User = _T("");
	//}}AFX_DATA_INIT
}

void CMyStartSocksDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMyStartSocksDlg)
	DDX_Text(pDX, IDC_EDIT_PASS, m_Pass);
	DDV_MaxChars(pDX, m_Pass, 31);
	DDX_Text(pDX, IDC_EDIT_PORT, m_Port);
	DDV_MaxChars(pDX, m_Port, 5);
	DDX_Text(pDX, IDC_EDIT_USER, m_User);
	DDV_MaxChars(pDX, m_User, 63);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMyStartSocksDlg, CDialog)
	//{{AFX_MSG_MAP(CMyStartSocksDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyStartSocksDlg message handlers

void CMyStartSocksDlg::OnOK() 
{
	CDialog::OnOK();
}

void CMyStartSocksDlg::OnCancel() 
{
	
	CDialog::OnCancel();
}
