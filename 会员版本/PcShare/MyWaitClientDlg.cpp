// MyWaitClientDlg.cpp : implementation file
//

#include "stdafx.h"
#include "PcShare.h"
#include "MyWaitClientDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyWaitClientDlg dialog

CMyWaitClientDlg::CMyWaitClientDlg(CWnd* pParent, LPCTSTR sWndTitle, DWORD nCmd)
	: CDialog(CMyWaitClientDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMyWaitClientDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	lstrcpy(m_ExtCmdStr, sWndTitle);
	memset(&m_SocketInfo, 0, sizeof(m_SocketInfo));
	nSaveCmd = nCmd;
}

void CMyWaitClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMyWaitClientDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CMyWaitClientDlg, CDialog)
	//{{AFX_MSG_MAP(CMyWaitClientDlg)
	ON_WM_COPYDATA()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyWaitClientDlg message handlers

void CMyWaitClientDlg::OnCancel() 
{
	if(MessageBox(_T("确定要终止连接，会退出整个操作界面？"), _T("提示"), MB_OKCANCEL|MB_ICONQUESTION) != IDOK)
	{
		return;
	}
	CDialog::OnCancel();
}

void CMyWaitClientDlg::OnOK() 
{
	CDialog::OnOK();
}

BOOL CMyWaitClientDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	MoveWindow(0, 0, 0, 0);
	GetParent()->BringWindowToTop();
	CenterWindow();

	MYCHILDCMDINFO m_Info = {0};

	//ID
	TCHAR m_TmpStr[256] = {0};
	if(!GetStrValue(m_ExtCmdStr, _T("id"), m_TmpStr))
	{
		ASSERT(FALSE);
	}
	lstrcpy(m_Info.m_Id, m_TmpStr);

	//HWND
	if(!GetStrValue(m_ExtCmdStr, _T("hwnd"), m_TmpStr))
	{
		ASSERT(FALSE);
	}
	HWND hSendHwnd = (HWND) StrToInt(m_TmpStr);

	if(nSaveCmd == 0)
	{
		//CMD
		if(!GetStrValue(m_ExtCmdStr, _T("cmd"), m_TmpStr))
		{
			ASSERT(FALSE);
		}
		m_Info.m_Info.m_Command = StrToInt(m_TmpStr);
	}
	else
	{
		m_Info.m_Info.m_Command = nSaveCmd;
	}

	//取当前窗口
	m_Info.m_Info.m_Hwnd = GetSafeHwnd();
	
	//填充结构
	COPYDATASTRUCT ct = {0};
	ct.lpData = &m_Info;
	ct.cbData = sizeof(MYCHILDCMDINFO);
	ct.dwData = PS_LOCAL_SENDCMD;
	::SendMessage(hSendHwnd, WM_COPYDATA, 0, (LPARAM) &ct);
	return TRUE;  
}

BOOL CMyWaitClientDlg::OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct) 
{
	LPTCPCONNECTINFO pInfo = (LPTCPCONNECTINFO) pCopyDataStruct->lpData;
	if(pCopyDataStruct->dwData == PS_SOCKET_CONNECT)
	{
		memcpy(&m_SocketInfo, pInfo, sizeof(m_SocketInfo));
		PostMessage(WM_COMMAND, (WPARAM) IDOK, 0);
	}
	return CDialog::OnCopyData(pWnd, pCopyDataStruct);
}
