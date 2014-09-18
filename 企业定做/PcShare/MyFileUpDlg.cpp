// MyFileUpDlg.cpp : implementation file
//

#include "stdafx.h"
#include "PcShare.h"
#include "MyFileUpDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyFileUpDlg dialog


CMyFileUpDlg::CMyFileUpDlg()
{
	//{{AFX_DATA_INIT(CMyFileUpDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	memset(&m_FileInfo, 0, sizeof(m_FileInfo));
	memset(m_Id, 0, sizeof(TCHAR) * 256);
	bIsUserCancel = FALSE;
}

CMyFileUpDlg::~CMyFileUpDlg()
{
}

void CMyFileUpDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMyFileUpDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	DDX_Control(pDX, IDC_ANIMATE_FILE, m_Animal);
	DDX_Control(pDX, IDC_PROGRESS_FILE, m_FileStep);
	DDX_Text(pDX, ID_FILE_LOCAL, m_LocalFile);
	DDX_Text(pDX, ID_FILE_REMOTE, m_RemoteFile);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMyFileUpDlg, CDialog)
	//{{AFX_MSG_MAP(CMyFileUpDlg)
	ON_WM_COPYDATA()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyFileUpDlg message handlers

BOOL CMyFileUpDlg::Create(CWnd* pParentWnd, LPCTSTR pId, LPINTERFILEINFO pInfo) 
{
	lstrcpy(m_Id, pId);
	memcpy(&m_FileInfo, pInfo, sizeof(INTERFILEINFO));
	return CDialog::Create(IDD, pParentWnd);
}

BOOL CMyFileUpDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_LocalFile = m_FileInfo.m_LocalFile;
	m_RemoteFile = m_FileInfo.m_RemoteFile;
	m_Animal.Open(IDR_AVI_FILE);

	m_FileStep.SetRange32(0, 100);

	UpdateData(FALSE);
	
	MYCHILDCMDINFO m_Info = {0};
	lstrcpy(m_Info.m_Id, m_Id);
	TCHAR nCWndName[256] = {0};
	wsprintf(nCWndName, _T("%s连接管理"), PS_TITLE);
	HWND hWnd = ::FindWindow(NULL, nCWndName);
	if(hWnd == NULL)
	{
		return FALSE;
	}
	
	HWND hMainListWnd = GetMainListWnd();
	
	m_Info.m_Info.m_Command = WM_CONNECT_UP_FILE;

	//取当前窗口
	m_Info.m_Info.m_Hwnd = GetSafeHwnd();
	
	//填充结构
	COPYDATASTRUCT ct = {0};
	ct.lpData = &m_Info;
	ct.cbData = sizeof(MYCHILDCMDINFO);
	ct.dwData = PS_LOCAL_FILETRAN;
	::SendMessage(hMainListWnd, WM_COPYDATA, 0, (LPARAM) &ct);
	
	return TRUE; 
}

BOOL CMyFileUpDlg::OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct) 
{
	LPTCPCONNECTINFO pInfo = (LPTCPCONNECTINFO) pCopyDataStruct->lpData;
	if(pCopyDataStruct->dwData == PS_SOCKET_CONNECT)
	{
		if(m_Tran.DuplicateHttpSocket(pInfo))
		{
			hDlFilethread = (HANDLE) _beginthread(UpLoadFileThread, 0, this);
		}
	}	
	return TRUE;
}

void CMyFileUpDlg::OnCancel() 
{
	bIsUserCancel = TRUE;
	m_Tran.Close();
	GetParent()->PostMessage(WM_DL_DELETE_VIEW, (WPARAM) this, NULL);
}

void CMyFileUpDlg::OnOK() 
{
	
}

void CMyFileUpDlg::UpLoadFileThread(LPVOID lPvoid)
{
	SetThreadAffinityMask(GetCurrentThread(), 0x0E);
	CMyFileUpDlg* pThis = (CMyFileUpDlg*) lPvoid;
	pThis->UpLoadFile();

	if(!pThis->bIsUserCancel)
	{
		//通知父窗口删除当前视
		pThis->GetParent()->PostMessage(WM_DL_DELETE_VIEW, (WPARAM) pThis, NULL);
	}
}

void CMyFileUpDlg::UpLoadFile()
{
	TCHAR m_LogsText[256];
	LARGE_INTEGER nTransLen = {0};

	HANDLE hFile = CreateFile(m_FileInfo.m_LocalFile, GENERIC_READ, 
			FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if(hFile == INVALID_HANDLE_VALUE)
	{
		wsprintf(m_LogsText, _T("无法打开本地文件[%s]，当前任务放弃"), m_FileInfo.m_LocalFile);
		MessageBox(m_LogsText);
		return;
	}

	GetFileSizeEx(hFile, &m_FileInfo.m_FileLen);

#ifdef UNICODE
	m_Tran.Convert2ClientANSI(m_FileInfo.m_LocalFile, wcslen(m_FileInfo.m_LocalFile));
	m_Tran.Convert2ClientANSI(m_FileInfo.m_RemoteFile, wcslen(m_FileInfo.m_RemoteFile));
#endif

	if(!m_Tran.TcpSendData(&m_FileInfo, sizeof(m_FileInfo)) || !m_Tran.TcpRecvData(&m_FileInfo, sizeof(m_FileInfo)))
	{
		CloseHandle(hFile);
		if(!bIsUserCancel)
		{
			MessageBox(_T("连接中断"));
		}
		return;
	}

		// 为文件传输做的Ansi到Unicode的转换 [9/19/2007 zhaiyinwei]
#ifdef UNICODE
	m_Tran.Convert2Unicode((char*)m_FileInfo.m_LocalFile,strlen((char*)m_FileInfo.m_LocalFile));
	m_Tran.Convert2Unicode((char*)m_FileInfo.m_RemoteFile,strlen((char*)m_FileInfo.m_RemoteFile));
#endif

	//查看返回码
	if(m_FileInfo.m_Response != 0)
	{
		switch(m_FileInfo.m_Response)
		{
			case DL_FILE_LOSE : 
				{
					wsprintf(m_LogsText, _T("无法创建远程文件[%s]，当前任务放弃"), m_FileInfo.m_RemoteFile);
					MessageBox(m_LogsText);
				}
				break;
			case DL_FILE_LEN_ERR :
				{
					wsprintf(m_LogsText, _T("远程文件长度异常[%s]，当前任务放弃"), m_FileInfo.m_RemoteFile);
					MessageBox(m_LogsText);
				}
				break;
			default :
				{
					MessageBox(_T("未知错误，，当前任务放弃"));
				}
				break;
		}

		CloseHandle(hFile);
		return;
	}

	//显示当前文件进度
	double c = 1.0 * m_FileInfo.m_FilePoint.QuadPart / m_FileInfo.m_FileLen.QuadPart * 100;
	m_FileStep.SetPos((int) c);

	nTransLen.QuadPart = m_FileInfo.m_FileLen.QuadPart - m_FileInfo.m_FilePoint.QuadPart;
	if(nTransLen.QuadPart == 0)
	{
		//无须下载
		CloseHandle(hFile);
		return;
	}

	//移动到断点位置
	SetFilePointerEx(hFile, m_FileInfo.m_FilePoint, NULL, FILE_BEGIN);

	//发送文件实体
	DWORD len = 0;
	BYTE pFileBuf[65535] = {0};
	DWORD nRecvLen = 0;
	while(nTransLen.QuadPart > 0)
	{
		len = DWORD (nTransLen.QuadPart > 65535 ? 65535 : nTransLen.QuadPart);
		ReadFile(hFile, pFileBuf, len, &nRecvLen, NULL);
		if(!m_Tran.TcpSendData(pFileBuf, len))
		{
			break;
		}

		nTransLen.QuadPart -= len;
		m_FileInfo.m_FilePoint.QuadPart += len;

		//显示当前文件进度
		double c = 1.0 * m_FileInfo.m_FilePoint.QuadPart / m_FileInfo.m_FileLen.QuadPart * 100;
		m_FileStep.SetPos((int) c);
	}

	//当前文件传输完成
	CloseHandle(hFile);
}
