// MySendLinkDlg.cpp : implementation file
//

#include "stdafx.h"
#include "PcShare.h"
#include "MySendLinkDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMySendLinkDlg dialog


CMySendLinkDlg::CMySendLinkDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMySendLinkDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMySendLinkDlg)
	m_Title = _T("");
	m_Type = 0;
	//}}AFX_DATA_INIT
}


void CMySendLinkDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMySendLinkDlg)
	DDX_Text(pDX, IDC_EDIT_FILE, m_Title);
	DDV_MaxChars(pDX, m_Title, 510);
	DDX_Radio(pDX, IDC_RADIO_TYPE, m_Type);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMySendLinkDlg, CDialog)
	//{{AFX_MSG_MAP(CMySendLinkDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMySendLinkDlg message handlers
