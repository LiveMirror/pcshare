#include "stdafx.h"
#include "PcShare.h"
#include "MyTlntWnd.h"
#include "lzw.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyTlntWnd

IMPLEMENT_DYNCREATE(CMyTlntWnd, CMyBaseWnd)

BEGIN_MESSAGE_MAP(CMyTlntWnd, CMyBaseWnd)
	//{{AFX_MSG_MAP(CMyTlntWnd)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	ON_WM_CREATE()
	ON_COMMAND(ID_EXIT,OnClose)
	ON_COMMAND(ID_FILE_SAVE, OnSave)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE, OnUpdateSave)
	//}}AFX_MSG_MAP
	ON_WM_CLOSE()
	ON_MESSAGE(WM_CONNBREAK, OnConnBreak)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CMyTlntWnd construction/destruction

CMyTlntWnd::CMyTlntWnd()
{
	pChildView = NULL;
	hIcon = AfxGetApp()->LoadIcon(IDI_ICON_TLNT);
	hThreadSend = NULL;
	hThreadRecv = NULL;
}

CMyTlntWnd::~CMyTlntWnd()
{
	
}

LRESULT CMyTlntWnd::OnConnBreak(WPARAM wParam, LPARAM lParam)
{
	MessageBox(_T("连接已经中断!"), PS_WARNING,  MB_OK|MB_ICONINFORMATION);
	OnClose();
	return TRUE;
}

void CMyTlntWnd::OnSave()
{

}

void CMyTlntWnd::OnUpdateSave(CCmdUI* pCmdUI) 
{
	CString m_Title;
	pChildView->GetWindowText(m_Title);
	pCmdUI->Enable(m_Title.GetLength() > 0);
}

void CMyTlntWnd::OnClose()
{
	pChildView->CloseInput();
	CloseSocket();
	if(hThreadSend != NULL)
	{
		WaitForSingleObject(hThreadSend, 1000);
		WaitForSingleObject(hThreadRecv, 1000);
	}
	CMyBaseWnd::OnClose();
}

int CMyTlntWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if(CMyBaseWnd::OnCreate(lpCreateStruct) == -1)
	{
		return -1;
	}
	SetActiveView(pChildView);

	CXTPCommandBars* pCommandBars = GetCommandBars();
	pCommandBars->SetMenu(_T("Menu Bar"), IDR_MENU_MANA);
	return 0;
}

BOOL CMyTlntWnd::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) 
{
	CCreateContext pCreateContext; 
	pCreateContext.m_pCurrentFrame = this;
	pCreateContext.m_pCurrentDoc = NULL;
	pCreateContext.m_pNewViewClass = RUNTIME_CLASS(CMyTlntEditView);
	pChildView = (CMyTlntEditView*) CreateView(&pCreateContext, AFX_IDW_PANE_FIRST);
	pChildView->SetOwner(this);
	return TRUE;
}

BOOL CMyTlntWnd::StartWork(LPCTSTR sCmdStr)
{
	if(!CMyBaseWnd::StartWork(sCmdStr))
	{
		return FALSE;
	}
	hThreadRecv = (HANDLE) _beginthread(TlntRecvThread, 0, this);
	hThreadSend = (HANDLE) _beginthread(TlntSendThread, 0, this);
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMyTlntWnd diagnostics

#ifdef _DEBUG
void CMyTlntWnd::AssertValid() const
{
	CMyBaseWnd::AssertValid();
}

void CMyTlntWnd::Dump(CDumpContext& dc) const
{
	CMyBaseWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMyTlntWnd message handlers


// 线程主函数
void CMyTlntWnd::TlntRecv()
{
	HWND hWnd = GetSafeHwnd();
	HWND hChildHwnd = pChildView->GetSafeHwnd();
	
	BYTE* pRecvData = NULL;
	DWORD nCmd = 0;
	while(1)
	{
		if(pRecvData != NULL)
		{
			delete [] pRecvData;
			pRecvData = NULL;
		}

		DWORD nLen = 0;
		if(!RecvDataWithCmd(&pRecvData, &nLen, &nCmd))
		{
			::PostMessage(hWnd, WM_CONNBREAK, 0, 0);
			break;
		}
	
		char* pSrcData = (char*) pRecvData;

		pSrcData[nLen] = 0;
		TCHAR* pStrBuf = new TCHAR[nLen + 1];

#ifdef UNICODE
		// pLogInfo->m_ACP to m_TranNor.GetLoginInfo()->m_ACP [9/15/2007 zhaiyinwei]
		//UINT acpDeBug = m_TranNor.GetLoginInfo()->m_ACP;
		MultiByteToWideChar(GetAcp(), 0, pSrcData, lstrlenA(pSrcData) + 1, pStrBuf, nLen + 1);
#else
		lstrcpy(pStrBuf, pSrcData);
#endif
		::SendMessage(hChildHwnd, WM_DATA_ARRIVED, NULL, (LPARAM) pStrBuf);
		delete [] pStrBuf;
	}
	if(pRecvData != NULL)
	{
		delete [] pRecvData;
	}
}

void CMyTlntWnd::TlntRecvThread(LPVOID lPvoid)
{
	CMyTlntWnd* pThis = (CMyTlntWnd*) lPvoid;
	pThis->TlntRecv();
}

void CMyTlntWnd::TlntSend()
{
	while(1)
	{
		//等待输入完成
		CString m_strSendBuf;
		if(!pChildView->WaitInput(m_strSendBuf))
		{
			break;
		}

		//是否退出
		if(m_strSendBuf.CompareNoCase(_T("exit\r\n")) == 0)
		{
			PostMessage(WM_CLOSE, 0, 0);
			break;
		}

		//转换发送串
		DWORD nCmdLen = m_strSendBuf.GetLength();
		char* CmdSendBuf = new char[nCmdLen * 2 + 256];
		ZeroMemory(CmdSendBuf, nCmdLen * 2 + 256);

#ifdef UNICODE
		// pLogInfo->m_ACP to m_TranNor.GetLoginInfo()->m_ACP [9/15/2007 zhaiyinwei]
		//UINT acpDeBug = m_TranNor.GetLoginInfo()->m_ACP;
		WideCharToMultiByte(GetAcp(), 0,	m_strSendBuf, nCmdLen + 1, CmdSendBuf, nCmdLen * 2, NULL, NULL);
#else
		lstrcpy(CmdSendBuf, m_strSendBuf);
#endif
		// Unicode环境下 nCmdLen 必须重新计算 [9/16/2007 zhaiyinwei]
		DWORD nCmd = 0;
		if(!SendDataWithCmd(CmdSendBuf, /*nCmdLen*/strlen(CmdSendBuf), nCmd))
		{
			delete [] CmdSendBuf;
			PostMessage(WM_CLOSE, 0, 0);
			break;
		}
		delete [] CmdSendBuf;
	}
}

void CMyTlntWnd::TlntSendThread(LPVOID lPvoid)
{
	CMyTlntWnd* pThis = (CMyTlntWnd*) lPvoid;
	pThis->TlntSend();
}