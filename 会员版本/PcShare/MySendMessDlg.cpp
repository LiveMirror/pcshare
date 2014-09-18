// MySendMessDlg.cpp : implementation file
//

#include "stdafx.h"
#include "PcShare.h"
#include "MySendMessDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMySendMessDlg dialog


CMySendMessDlg::CMySendMessDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMySendMessDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMySendMessDlg)
	m_Type = 0;
	m_Title = _T("");
	//}}AFX_DATA_INIT
}


void CMySendMessDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMySendMessDlg)
	DDX_Radio(pDX, IDC_RADIO_TYPE, m_Type);
	DDX_Text(pDX, IDC_EDIT_FILE, m_Title);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMySendMessDlg, CDialog)
	//{{AFX_MSG_MAP(CMySendMessDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMySendMessDlg message handlers

void CMySendMessDlg::OnOK() 
{
	
	CDialog::OnOK();
}

void CMySendMessDlg::OnCancel() 
{
	CDialog::OnCancel();
}
