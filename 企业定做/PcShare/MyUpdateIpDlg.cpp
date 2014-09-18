// MyFtpIp.cpp : implementation file
//

#include "stdafx.h"
#include "PcShare.h"
#include "MyUpdateIpDlg.h"
#include <Wininet.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyUpdateIpDlg dialog


CMyUpdateIpDlg::CMyUpdateIpDlg() : CDialog(CMyUpdateIpDlg::IDD, NULL)
{
	//{{AFX_DATA_INIT(CMyUpdateIpDlg)
	m_FtpServer = _T("");
	m_Pass = _T("");
	m_Path = _T("/");
	m_User = _T("");
	m_Port = 80;
	m_FtpPort = 21;
	//}}AFX_DATA_INIT
}

void CMyUpdateIpDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMyUpdateIpDlg)
	DDX_Control(pDX, IDC_COMBO_IPLIST, m_IpList);
	DDX_Text(pDX, IDC_EDIT_ADDR_FTP, m_FtpServer);
	DDX_Text(pDX, IDC_EDIT_PASS, m_Pass);
	DDX_Text(pDX, IDC_EDIT_PATH, m_Path);
	DDX_Text(pDX, IDC_EDIT_USER, m_User);
	DDX_Text(pDX, IDC_EDIT_PORT2, m_Port);
	DDX_Text(pDX, IDC_EDIT_PORT_FTP, m_FtpPort);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CMyUpdateIpDlg, CDialog)
	//{{AFX_MSG_MAP(CMyUpdateIpDlg)
	ON_BN_CLICKED(IDCANCEL, OnCancel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyUpdateIpDlg message handlers

BOOL UpDataServerInfo(TCHAR *Ip,WORD Port,TCHAR *UserName,
					  TCHAR* UserPwd,TCHAR *LocalFile,
					  TCHAR *RemoteFile)
{
	HINTERNET Ie = InternetOpen(NULL, INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
	if(Ie == NULL)
	{
		return FALSE;
	}
	HINTERNET Hc = InternetConnect( Ie, Ip, Port, UserName, UserPwd, INTERNET_SERVICE_FTP, 0, 0);	
	if(Hc == NULL)
	{
		InternetCloseHandle(Ie);
		return FALSE;
	}
	BOOL bRet = FtpPutFile(Hc, LocalFile, RemoteFile, FTP_TRANSFER_TYPE_ASCII, 0);
	InternetCloseHandle(Ie);
	InternetCloseHandle(Hc);
	return bRet;
}

void CMyUpdateIpDlg::FlushThread(LPVOID lPvoid)
{
	CMyUpdateIpDlg* pThis = (CMyUpdateIpDlg*) lPvoid;

	//取本机IP和端口
	TCHAR m_Text[512] = {0};
	TCHAR m_IpStr[512] = {0};
	pThis->m_IpList.GetWindowText(m_Text, 512);
	wsprintf(m_IpStr, _T("%s:%d"), m_Text, pThis->m_Port);

	char m_Ip[1024] = {0};
#ifdef UNICODE
	WideCharToMultiByte(CP_ACP, 0,	m_IpStr, lstrlen(m_IpStr) + 1, m_Ip, 1024, NULL, NULL);
#else
	lstrcpy(m_Ip, m_IpStr);
#endif

	//写文件
	TCHAR m_IpPath[256] = _T("ip.txt");
	GetSysFileName(m_IpPath);

	CFile m_File;
	if(!m_File.Open(m_IpPath, CFile::modeCreate|CFile::modeWrite))
	{
		pThis->MessageBox(_T("无法生成IP文件"), PS_WARNING, MB_OKCANCEL|MB_ICONWARNING);
		pThis->GetDlgItem(IDOK)->EnableWindow(TRUE);
		return;
	}
	m_File.Write(m_Ip, lstrlen(m_IpStr));
	m_File.Close();

	//目标文件名称
	if(!UpDataServerInfo(pThis->m_FtpServer.LockBuffer(),
						 pThis->m_FtpPort,
						 pThis->m_User.LockBuffer(),
						 pThis->m_Pass.LockBuffer(),
						 m_IpPath,
						 pThis->m_Path.LockBuffer()))
	{
		//上传文件失败
		pThis->MessageBox(_T("上传文件失败"), PS_WARNING, MB_OKCANCEL|MB_ICONWARNING);
		pThis->GetDlgItem(IDOK)->EnableWindow(TRUE);
		return;
	}

	pThis->GetDlgItem(IDOK)->EnableWindow(TRUE);
	pThis->MessageBox(_T("刷新IP成功"), _T("刷新IP"));
}

BOOL CMyUpdateIpDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	TCHAR m_TmpAddrList[1024] = {0};
	lstrcpy(m_TmpAddrList, GetIpList());
	TCHAR* pStart = m_TmpAddrList;
	while(1)
	{
		TCHAR* pFind = StrChr(pStart, _T('-'));
		if(pFind == NULL)
		{
			m_IpList.AddString(pStart);
			break;
		}
		*pFind = 0;
		m_IpList.AddString(pStart);
		pStart = pFind + 1;
	}
	m_IpList.SetCurSel(0);

	TCHAR m_sFtpServer[256] = {0};
	GetPrivateProfileString(PS_SHEZHI, _T("FTP服务器"),	_T(""), m_sFtpServer, 250, GetIniFileName());
	m_FtpServer = m_sFtpServer;

	TCHAR m_sPath[256] = {0};
	GetPrivateProfileString(PS_SHEZHI, _T("FTP绝对路径"), _T("/ip.rar"), m_sPath, 250, GetIniFileName());
	m_Path = m_sPath;

	TCHAR m_sUser[256] = {0};
	GetPrivateProfileString(PS_SHEZHI, _T("FTP用户名"), _T(""), m_sUser, 250, GetIniFileName());
	m_User = m_sUser;

	TCHAR m_sPass[25] = {0};
	if(GetPrivateProfileStruct(PS_SHEZHI, _T("FTP用户密码"), m_sPass, 24, GetIniFileName()))
	{
		m_Pass = m_sPass;
	}
	
	UpdateData(FALSE);

	return TRUE;  
}

void CMyUpdateIpDlg::OnCancel() 
{
	CDialog::OnCancel();
}

void CMyUpdateIpDlg::OnOK() 
{
	UpdateData();	
	WritePrivateProfileString(PS_SHEZHI, _T("FTP服务器"), m_FtpServer,GetIniFileName());
	WritePrivateProfileString(PS_SHEZHI, _T("FTP绝对路径"),	m_Path,GetIniFileName());
	WritePrivateProfileString(PS_SHEZHI, _T("FTP用户名"), m_User,GetIniFileName());
	TCHAR m_sPass[25] = {0};
	lstrcpy(m_sPass, m_Pass);
	WritePrivateProfileStruct(PS_SHEZHI, _T("FTP用户密码"), m_sPass, 24, GetIniFileName());
	GetDlgItem(IDOK)->EnableWindow(FALSE);
	_beginthread(FlushThread, 0, this);
}

