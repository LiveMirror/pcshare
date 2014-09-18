// NetServerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NetServer.h"
#include "NetServerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNetServerDlg dialog

CNetServerDlg::CNetServerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CNetServerDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNetServerDlg)
	m_Port = _T("");
	m_Pass = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	memset(m_IniFileName, 0, sizeof(m_IniFileName));
	GetIniFileName();
}

void CNetServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNetServerDlg)
	DDX_Text(pDX, IDC_EDIT_PORT, m_Port);
	DDV_MaxChars(pDX, m_Port, 5);
	DDX_Text(pDX, IDC_EDIT_PASS, m_Pass);
	DDV_MaxChars(pDX, m_Pass, 9);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CNetServerDlg, CDialog)
	//{{AFX_MSG_MAP(CNetServerDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDSTOP, OnStop)
	ON_BN_CLICKED(IDSTART, OnStart)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNetServerDlg message handlers

BOOL CNetServerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	
	TCHAR m_sPort[256] = {0};
	GetPrivateProfileString("setup", _T("port"), _T(""), m_sPort, 255, m_IniFileName);
	m_Port = m_sPort;

	TCHAR m_sPass[256] = {0};
	GetPrivateProfileString("setup", _T("pass"), _T(""), m_sPass, 255, m_IniFileName);
	m_Pass = m_sPass;

	UpdateData(FALSE);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CNetServerDlg::OnPaint() 
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
HCURSOR CNetServerDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CNetServerDlg::OnCancel() 
{
	CDialog::OnCancel();
}

void CNetServerDlg::GetIniFileName()
{
	GetModuleFileName(NULL, m_IniFileName, 500);
	TCHAR* pFind = StrRChr(m_IniFileName, NULL, _T('.'));
	if(pFind != NULL)
	{
		*(pFind + 1) = 0;
		lstrcat(m_IniFileName, "ini");
	}
}

void CNetServerDlg::OnOK() 
{
	
}

void CNetServerDlg::OnStart() 
{
	UpdateData();
	if(m_Port.IsEmpty())
	{
		return;
	}

	//保存数据
	WritePrivateProfileString("setup", _T("port"), m_Port, m_IniFileName);
	WritePrivateProfileString("setup", _T("pass"), m_Pass, m_IniFileName);
	if(!m_Socket.StartWork(m_Port, m_Pass))
	{
		MessageBox("端口打开失败！", "错误");
		return;
	}

	GetDlgItem(IDSTART)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_PORT)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_PASS)->EnableWindow(FALSE);
	GetDlgItem(IDSTOP)->EnableWindow(TRUE);
}

void CNetServerDlg::OnStop() 
{
	m_Socket.Close();
	GetDlgItem(IDSTART)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_PORT)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_PASS)->EnableWindow(TRUE);
	GetDlgItem(IDSTOP)->EnableWindow(FALSE);
}

