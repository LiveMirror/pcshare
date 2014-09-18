// MySelectTypeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "PcShare.h"
#include "MySelectTypeDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMySelectTypeDlg dialog


CMySelectTypeDlg::CMySelectTypeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMySelectTypeDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMySelectTypeDlg)
	m_LoginType = 0;
	//}}AFX_DATA_INIT
}


void CMySelectTypeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMySelectTypeDlg)
	DDX_Radio(pDX, IDC_RADIO_REMOTE, m_LoginType);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMySelectTypeDlg, CDialog)
	//{{AFX_MSG_MAP(CMySelectTypeDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMySelectTypeDlg message handlers
