// PsProxyDlg.cpp : implementation file
//

#include "stdafx.h"
#include "PsProxy.h"
#include "PsProxyDlg.h"
#include "MySocket.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPsProxyDlg dialog

CPsProxyDlg::CPsProxyDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPsProxyDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPsProxyDlg)
	m_LocalPort = _T("");
	m_RemoteAddr = _T("");
	m_RemotePort = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	GetIniFileName();
}

void CPsProxyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPsProxyDlg)
	DDX_Text(pDX, IDC_EDIT_LOCAL_PORT, m_LocalPort);
	DDX_Text(pDX, IDC_EDIT_REMOTE_ADDR, m_RemoteAddr);
	DDX_Text(pDX, IDC_EDIT_REMOTE_PORT, m_RemotePort);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CPsProxyDlg, CDialog)
	//{{AFX_MSG_MAP(CPsProxyDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(ID_STOP, OnStop)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPsProxyDlg message handlers

BOOL CPsProxyDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	
	GetDlgItem(ID_STOP)->EnableWindow(FALSE);


	TCHAR m_sRemoteAddr[256] = {0};
	GetPrivateProfileString("setup", _T("remoteaddr"), _T(""), m_sRemoteAddr, 255, m_IniFileName);
	m_RemoteAddr = m_sRemoteAddr;

	TCHAR m_sRemotePort[256] = {0};
	GetPrivateProfileString("setup", _T("remoteport"), _T(""), m_sRemotePort, 255, m_IniFileName);
	m_RemotePort = m_sRemotePort;

	TCHAR m_sLocalPort[256] = {0};
	GetPrivateProfileString("setup", _T("localport"), _T(""), m_sLocalPort, 255, m_IniFileName);
	m_LocalPort = m_sLocalPort;

	UpdateData(FALSE);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CPsProxyDlg::OnPaint() 
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
HCURSOR CPsProxyDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CPsProxyDlg::GetIniFileName()
{
	GetModuleFileName(NULL, m_IniFileName, 500);
	TCHAR* pFind = StrRChr(m_IniFileName, NULL, _T('.'));
	if(pFind != NULL)
	{
		*(pFind + 1) = 0;
		lstrcat(m_IniFileName, "ini");
	}
}

void CPsProxyDlg::OnOK() 
{
	UpdateData();
	if(m_RemoteAddr.IsEmpty() || m_RemotePort.IsEmpty() || m_LocalPort.IsEmpty())
	{
		return;
	}

	WritePrivateProfileString("setup", _T("remoteaddr"), m_RemoteAddr, m_IniFileName);
	WritePrivateProfileString("setup", _T("remoteport"), m_RemotePort, m_IniFileName);
	WritePrivateProfileString("setup", _T("localport"), m_LocalPort, m_IniFileName);
	
	if(!m_Socket.StartWork(m_RemoteAddr, m_RemotePort, m_LocalPort))
	{
		MessageBox("无法打开本地端口", "错误");
		return;
	}

	GetDlgItem(IDOK)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_REMOTE_ADDR)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_REMOTE_PORT)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_LOCAL_PORT)->EnableWindow(FALSE);

	GetDlgItem(ID_STOP)->EnableWindow(TRUE);
}

void CPsProxyDlg::OnCancel() 
{
	OnStop();
	CDialog::OnCancel();
}

void CPsProxyDlg::OnStop() 
{
	m_Socket.Close();
	GetDlgItem(IDOK)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_REMOTE_ADDR)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_REMOTE_PORT)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_LOCAL_PORT)->EnableWindow(TRUE);

	GetDlgItem(ID_STOP)->EnableWindow(FALSE);
}
