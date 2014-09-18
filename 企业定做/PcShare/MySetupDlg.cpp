// MySetupDlg.cpp : implementation file
//

#include "stdafx.h"
#include "PcShare.h"
#include "MySetupDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMySetupDlg dialog

CMySetupDlg::CMySetupDlg()
: CDialog(CMySetupDlg::IDD, NULL)
{
	//{{AFX_DATA_INIT(CMySetupDlg)
	m_Port = _T("80");
	//}}AFX_DATA_INIT
}

void CMySetupDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMySetupDlg)
	DDX_Control(pDX, IDC_SPIN_PORT_MAIN, m_SpinPortMain);
	DDX_Text(pDX, IDC_EDIT_PORT_MAIN, m_Port);
	DDV_MaxChars(pDX, m_Port, 5);
	DDX_Text(pDX, IDC_EDIT_PASS, m_Pass);
	DDV_MaxChars(pDX, m_Port, 8);
	DDX_CBString(pDX, IDC_COMBO_SIZE, m_Size);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CMySetupDlg, CDialog)
	//{{AFX_MSG_MAP(CMySetupDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMySetupDlg message handlers

void CMySetupDlg::OnOK() 
{
	UpdateData();
	WritePrivateProfileString(PS_SHEZHI, _T("上线连接端口"), m_Port, GetIniFileName());
	WritePrivateProfileString(PS_SHEZHI, _T("上线连接密码"), m_Pass, GetIniFileName());
	WritePrivateProfileString(PS_SHEZHI, _T("视频监控尺寸"), m_Size, GetIniFileName());
	CDialog::OnOK();
}

BOOL CMySetupDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_SpinPortMain.SetRange32(1, 65534);

	TCHAR m_sPort[100] = {0};
	GetPrivateProfileString(PS_SHEZHI, _T("上线连接端口"), _T("80"), m_sPort, 99, GetIniFileName());
	m_Port = m_sPort;

	TCHAR m_sPass[100] = {0};
	GetPrivateProfileString(PS_SHEZHI, _T("上线连接密码"), _T(""), m_sPass, 99, GetIniFileName());
	m_Pass = m_sPass;

	TCHAR m_sSize[100] = {0};
	GetPrivateProfileString(PS_SHEZHI, _T("视频监控尺寸"), _T("320 x 240"), m_sSize, 99, GetIniFileName());
	m_Size = m_sSize;

	UpdateData(FALSE);
	return TRUE;  
}

void CMySetupDlg::OnCancel() 
{
	OnOK();
}
