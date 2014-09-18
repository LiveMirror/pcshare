// MyFrameWnd.cpp : implementation file
//

#include "stdafx.h"
#include "PcShare.h"
#include "MyFramWnd.h"
#include "MyWaitClientDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyFramWnd

IMPLEMENT_DYNCREATE(CMyFramWnd, CMyBaseWnd)

CMyFramWnd::CMyFramWnd()
{
	pFrameView = NULL;
	m_IsEnableInput = FALSE;
	hIcon = AfxGetApp()->LoadIcon(IDI_ICON_FRAM);

	ZeroMemory(m_ExtCmdStr, sizeof(m_ExtCmdStr));
	m_CurSorStatus = FALSE;
}

CMyFramWnd::~CMyFramWnd()
{

}

BEGIN_MESSAGE_MAP(CMyFramWnd, CMyBaseWnd)
	//{{AFX_MSG_MAP(CMyFramWnd)
	ON_WM_CREATE()
	ON_COMMAND(ID_FRAME_SEND_CTRL, OnSendCtrl)
	ON_UPDATE_COMMAND_UI(ID_FRAME_SEND_CTRL, OnUpdateSendCtrl)
	ON_COMMAND(ID_FRAM_SEND_MOUSE, OnSendMouse)
	ON_UPDATE_COMMAND_UI(ID_FRAM_SEND_MOUSE, OnUpdateSendMouse)
	ON_COMMAND(ID_FRAME_SAVE_IMAGE, OnSaveImage)
	ON_UPDATE_COMMAND_UI(ID_FRAME_SAVE_IMAGE, OnUpdateSaveImage)
	ON_UPDATE_COMMAND_UI(IDS_UPDATEPANE, OnUpdatePane)
	ON_COMMAND(ID_RECORD_STOP, OnRecordStop)
	ON_UPDATE_COMMAND_UI(ID_RECORD_STOP, OnUpdateRecordStop)
	ON_COMMAND(ID_RECORD_START, OnRecordStart)
	ON_UPDATE_COMMAND_UI(ID_RECORD_START, OnUpdateRecordStart)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_SETMYCURSOR, OnSetMyCursor)
	ON_COMMAND(ID_EXIT, OnClose)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyFramWnd message handlers

void CMyFramWnd::OnRecordStart()
{
	pFrameView->Start();
}

void CMyFramWnd::OnUpdateRecordStart(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(pFrameView->IsCanRecord());
}

void CMyFramWnd::OnRecordStop()
{
	pFrameView->Stop();
}

void CMyFramWnd::OnUpdateRecordStop(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(pFrameView->IsRecord());
}

void CMyFramWnd::OnSendCtrl()
{
	m_IsEnableInput = TRUE;
	pFrameView->SetInputEnable(m_IsEnableInput);
	pFrameView->SendCtrlInfo();
}

void CMyFramWnd::OnUpdateSendCtrl(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(pFrameView->IsInit());
}

void CMyFramWnd::OnSendMouse()
{
	if(m_IsEnableInput)
	{
		m_IsEnableInput = FALSE;
	}
	else
	{
		m_IsEnableInput = TRUE;
	}
	pFrameView->SetInputEnable(m_IsEnableInput);
}

void CMyFramWnd::OnUpdateSendMouse(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(pFrameView->IsInit());
	pCmdUI->SetCheck(m_IsEnableInput);
}

void CMyFramWnd::OnSaveImage()
{
	pFrameView->Save();
}

void CMyFramWnd::OnUpdateSaveImage(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(pFrameView->IsInit());
}

int CMyFramWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	SetThreadAffinityMask(GetCurrentThread(), 0x01);
	if(CMyBaseWnd::OnCreate(lpCreateStruct) == -1)
	{
		return -1;
	}
	SetActiveView(pFrameView);

	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT|TBSTYLE_LIST, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC|WS_EX_RTLREADING   ) ||
		!m_wndToolBar.LoadToolBar(IDR_TOOLBAR_FRAM))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	m_ToolBarList.Create(16, 16, ILC_COLOR24|ILC_MASK, 10, 10);
	m_ToolBarList.Add(IDB_BITMAP_PLAY);
	m_ToolBarList.Add(IDB_BITMAP_STOP);
	m_ToolBarList.Add(IDB_BITMAP_SAVE);
	m_ToolBarList.Add(IDB_BITMAP_VIDEO_RECORDE_START);
	m_ToolBarList.Add(IDB_BITMAP_VIDEO_RECORDE_STOP);

	m_wndToolBar.GetToolBarCtrl().SetImageList(&m_ToolBarList);
	m_wndToolBar.SetButtonStyle(0, TBBS_AUTOSIZE);
	m_wndToolBar.SetButtonStyle(1, TBBS_AUTOSIZE);
	m_wndToolBar.SetButtonStyle(2, TBBS_AUTOSIZE);
	m_wndToolBar.SetButtonStyle(3, TBBS_AUTOSIZE);
	m_wndToolBar.SetButtonStyle(4, TBBS_AUTOSIZE);
	m_wndToolBar.SetButtonText(0,_T("控制鼠标和键盘输入"));// "*" to _T("*") [9/15/2007 zhaiyinwei]
	m_wndToolBar.SetButtonText(1,_T("发送Ctrl+Alt+Del"));// "*" to _T("*") [9/15/2007 zhaiyinwei]
	m_wndToolBar.SetButtonText(2,_T("保存当前屏幕内容"));// "*" to _T("*") [9/15/2007 zhaiyinwei]
	m_wndToolBar.SetButtonText(3,_T("开始录像"));// "*" to _T("*") [9/15/2007 zhaiyinwei]
	m_wndToolBar.SetButtonText(4,_T("停止录像"));// "*" to _T("*") [9/15/2007 zhaiyinwei]
	m_wndToolBar.GetToolBarCtrl().SetDisabledImageList(&m_ToolBarList);
	CenterWindow();
	return 0;
}

BOOL CMyFramWnd::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) 
{
	CCreateContext pCreateContext; 
	pCreateContext.m_pCurrentFrame = this;
	pCreateContext.m_pCurrentDoc = NULL;
	pCreateContext.m_pNewViewClass = RUNTIME_CLASS(CMyMultView);
	pFrameView = (CMyMultView*) CreateView(&pCreateContext,AFX_IDW_PANE_FIRST);
	return TRUE;
}

BOOL CMyFramWnd::StartWork(LPCTSTR sCmdStr)
{
	TCPCONNECTINFO	m_SocketInfo = {0};
	if(!CMyBaseWnd::StartWorkWithCmd(sCmdStr, WM_CONNECT_FRAM, &m_SocketInfo))
	{
		return FALSE;
	}
	pFrameView->StartWork(m_SocketInfo, TRUE);
	return TRUE;
}

void CMyFramWnd::OnUpdatePane(CCmdUI* pCmdUI) 
{
	if(pFrameView != NULL)
	{
		pCmdUI->SetText(pFrameView->GetXy());
	}
}

BOOL CMyFramWnd::PreCreateWindow(CREATESTRUCT& cs) 
{
	if(!CMyBaseWnd::PreCreateWindow(cs))
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

