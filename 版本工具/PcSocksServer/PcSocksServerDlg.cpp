// PcSocksServerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "PcSocksServer.h"
#include "PcSocksServerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPcSocksServerDlg dialog

CPcSocksServerDlg::CPcSocksServerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPcSocksServerDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPcSocksServerDlg)
	m_Pass = _T("");
	m_Port = _T("");
	m_User = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	char m_ExeName[256] = {0};
	GetModuleFileName(NULL, m_ExeName, 255);
	m_IniFileName = m_ExeName;
	m_IniFileName.Replace(".exe", ".ini");
}

void CPcSocksServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPcSocksServerDlg)
	DDX_Text(pDX, IDC_EDIT_PASS, m_Pass);
	DDV_MaxChars(pDX, m_Pass, 31);
	DDX_Text(pDX, IDC_EDIT_PORT, m_Port);
	DDV_MaxChars(pDX, m_Port, 5);
	DDX_Text(pDX, IDC_EDIT_USER, m_User);
	DDV_MaxChars(pDX, m_User, 63);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CPcSocksServerDlg, CDialog)
	//{{AFX_MSG_MAP(CPcSocksServerDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDSTART, OnStart)
	ON_BN_CLICKED(IDSTOP, OnStop)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPcSocksServerDlg message handlers

BOOL CPcSocksServerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	TCHAR m_sPort[256] = {0};
	GetPrivateProfileString("setup", _T("port"), _T(""), m_sPort, 255, m_IniFileName);
	m_Port = m_sPort;

	TCHAR m_sPass[256] = {0};
	GetPrivateProfileString("setup", _T("pass"), _T(""), m_sPass, 255, m_IniFileName);
	m_Pass = m_sPass;

	TCHAR m_sUser[256] = {0};
	GetPrivateProfileString("setup", _T("pass"), _T(""), m_sUser, 255, m_IniFileName);
	m_User = m_sUser;
	
	UpdateData(FALSE);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CPcSocksServerDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CPcSocksServerDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CPcSocksServerDlg::OnOK() 
{
	
}

void CPcSocksServerDlg::OnCancel() 
{
	CDialog::OnCancel();
}

void CPcSocksServerDlg::OnStart() 
{
	UpdateData();
	if(m_Port.IsEmpty())
	{
		return;
	}

	MYSOCKSINFO m_SocksInfo = {0};
	m_SocksInfo.m_Port = StrToInt(m_Port);
	lstrcpy(m_SocksInfo.m_User, m_User);
	lstrcpy(m_SocksInfo.m_Pass, m_Pass);
	if(!m_Socks.StartWork(&m_SocksInfo))
	{
		MessageBox("打开端口失败！", "错误");
		return;
	}

	//保存信息
	WritePrivateProfileString("setup", _T("port"), m_Port, m_IniFileName);
	WritePrivateProfileString("setup", _T("user"), m_User, m_IniFileName);
	WritePrivateProfileString("setup", _T("pass"), m_Pass, m_IniFileName);

	GetDlgItem(IDSTART)->EnableWindow(FALSE);
	GetDlgItem(IDSTOP)->EnableWindow(TRUE);

	GetDlgItem(IDC_EDIT_PORT)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_USER)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_PASS)->EnableWindow(FALSE);
}

void CPcSocksServerDlg::OnStop() 
{
	m_Socks.StopWork();
	GetDlgItem(IDSTART)->EnableWindow(TRUE);
	GetDlgItem(IDSTOP)->EnableWindow(FALSE);

	GetDlgItem(IDC_EDIT_PORT)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_USER)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_PASS)->EnableWindow(TRUE);
}
