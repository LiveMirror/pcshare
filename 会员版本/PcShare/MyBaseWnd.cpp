// MyBaseWnd.cpp : implementation file
//

#include "stdafx.h"
#include "PcShare.h"
#include "MyBaseWnd.h"
#include "MyWaitClientDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyBaseWnd

IMPLEMENT_DYNCREATE(CMyBaseWnd, CFrameWnd)

CMyBaseWnd::CMyBaseWnd()
{
	ZeroMemory(m_ExtCmdStr, sizeof(m_ExtCmdStr));
	m_CurSorStatus = FALSE;
	hIcon = NULL;
	m_IsInit = FALSE;
}

CMyBaseWnd::~CMyBaseWnd()
{
	
}

BEGIN_MESSAGE_MAP(CMyBaseWnd, CFrameWnd)
	//{{AFX_MSG_MAP(CMyBaseWnd)
	ON_WM_CLOSE()
	ON_WM_SETCURSOR()
	ON_WM_CREATE()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_SETMYCURSOR, OnSetMyCursor)
END_MESSAGE_MAP()


static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	IDS_UPDATEPANE,
};

/////////////////////////////////////////////////////////////////////////////
// CMyBaseWnd message handlers


BOOL CMyBaseWnd::StartWork(LPCTSTR sCmdStr)
{
	lstrcpy(m_ExtCmdStr, sCmdStr);

	CString	m_BaseTitle;
	GetWindowText(m_BaseTitle);
	CString m_Title = m_BaseTitle;
	m_Title += _T(" - 按ESC键可终止操作，退出等待");
	SetWindowText(m_Title);
	PostMessage(WM_SETMYCURSOR, (WPARAM) TRUE, 0);
	CMyWaitClientDlg dlg(this, sCmdStr, 0);
	if(dlg.DoModal() != IDOK)
	{
		PostMessage(WM_CLOSE, 0, 0);
		return FALSE;
	}
	if(!m_Tran.DuplicateHttpSocket(&dlg.m_SocketInfo))
	{
		return FALSE;
	}
	SetWindowText(m_BaseTitle);
	PostMessage(WM_SETMYCURSOR, (WPARAM) FALSE, 0);
	m_IsInit = TRUE;
	return TRUE;
}

BOOL CMyBaseWnd::StartWorkWithCmd(LPCTSTR sCmdStr, DWORD nCmd, LPTCPCONNECTINFO pSocketInfo)
{
	lstrcpy(m_ExtCmdStr, sCmdStr);
	CString	m_BaseTitle;
	GetWindowText(m_BaseTitle);
	CString m_Title = m_BaseTitle;
	m_Title += _T(" - 按ESC键可终止操作，退出等待");
	SetWindowText(m_Title);
	PostMessage(WM_SETMYCURSOR, (WPARAM) TRUE, 0);

	CMyWaitClientDlg dlg(this, sCmdStr, nCmd);
	if(dlg.DoModal() != IDOK)
	{
		TRACE(_T("dlg.DoModal() != IDOK\n"));
		PostMessage(WM_CLOSE, 0, 0);
		return FALSE;
	}

	memcpy(pSocketInfo, &dlg.m_SocketInfo, sizeof(TCPCONNECTINFO));
	SetWindowText(m_BaseTitle);
	PostMessage(WM_SETMYCURSOR, (WPARAM) FALSE, 0);
	return TRUE;
}

void CMyBaseWnd::OnClose() 
{
	if(m_IsInit)
	{
		m_Tran.Close();
	}
	CFrameWnd::OnClose();
}

void CMyBaseWnd::CloseSocket()
{
	if(m_IsInit)
	{
		m_Tran.Close();
	}
}

BOOL CMyBaseWnd::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	if(m_CurSorStatus)
	{
		SetCursor(GetWaitCursor());
		return TRUE;
	}
	return CFrameWnd::OnSetCursor(pWnd, nHitTest, message);
}

LRESULT CMyBaseWnd::OnSetMyCursor(WPARAM wParam, LPARAM lParam)
{
	if(wParam)
	{
		m_CurSorStatus = TRUE;
		PostMessage(WM_SETCURSOR, 0, 0);
	}
	else
	{
		m_CurSorStatus = FALSE;
		PostMessage(WM_SETCURSOR, 0, 0);
	}
	return TRUE;
}

int CMyBaseWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	SetThreadAffinityMask(GetCurrentThread(), 0x01);

	if(CFrameWnd::OnCreate(lpCreateStruct) == -1)
	{
		return -1;
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	if(!InitCommandBars())
	{
		return -1;
	}

	CXTPPaintManager* pTheme = new CXTPWhidbeyTheme();
	XTP_COMMANDBARS_ICONSINFO* pIconsInfo = pTheme->GetIconsInfo();
	pIconsInfo->bUseDisabledIcons = TRUE;
	pIconsInfo->bIconsWithShadow = TRUE;
	XTPPaintManager()->SetCustomTheme(pTheme);
	GetCommandBars()->RedrawCommandBars();
	CenterWindow();
	return 0;
}

BOOL CMyBaseWnd::PreCreateWindow(CREATESTRUCT& cs) 
{
	if(!CFrameWnd::PreCreateWindow(cs))
	{
		return FALSE;
	}
	cs.style ^= WS_MAXIMIZE;   
	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	cs.lpszClass = AfxRegisterWndClass(
		CS_VREDRAW | CS_HREDRAW,
		::LoadCursor(NULL, IDC_ARROW),
		(HBRUSH) ::GetStockObject(WHITE_BRUSH),
		hIcon);
	return TRUE;
}

HCURSOR GetWaitCursor()
{
	static HCURSOR m_WaitCur = NULL;
	if(m_WaitCur == NULL)
	{
		m_WaitCur = LoadCursor(NULL, MAKEINTRESOURCE(IDC_WAIT));
	}
	return m_WaitCur;
}

void CMyBaseWnd::OnSize(UINT nType, int cx, int cy) 
{
	CFrameWnd::OnSize(nType, cx, cy);
	if(m_wndStatusBar.GetSafeHwnd())
	{
		UINT nID = 0, nStyle = 0;
		int Width = 0;
		m_wndStatusBar.GetPaneInfo(1, nID, nStyle, Width);
		Width = cx / 4;
		m_wndStatusBar.SetPaneInfo(1, nID, nStyle, Width);
	}
}

BOOL CMyBaseWnd::TranData(LPVOID InData, DWORD InLen, LPVOID OutData, DWORD OutLen)
{
	if(!m_IsInit)
	{
		return FALSE;
	}
	return m_Tran.TranData(InData, InLen, OutData, OutLen);
}

BOOL CMyBaseWnd::SendDataWithCmd(LPVOID pSendData, DWORD dSendLen, DWORD nCmd)
{
	if(!m_IsInit)
	{
		return FALSE;
	}
	return m_Tran.SendDataWithCmd(pSendData, dSendLen, nCmd);
}

BOOL CMyBaseWnd::RecvDataWithCmd(LPBYTE* pRecvData, DWORD* dRecvLen, DWORD* dCmd)
{
	if(!m_IsInit)
	{
		return FALSE;
	}
	return m_Tran.RecvDataWithCmd(pRecvData, dRecvLen, dCmd);
}

UINT CMyBaseWnd::GetAcp()
{
	return m_Tran.GetLoginInfo()->m_ACP;
}

BOOL CMyBaseWnd::IsInit()
{
	return m_IsInit;
}

