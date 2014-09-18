// MyProcWnd.cpp : implementation file
//

#include "stdafx.h"
#include "PcShare.h"
#include "MyProcWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyProcWnd

IMPLEMENT_DYNCREATE(CMyProcWnd, CMyBaseWnd)

CMyProcWnd::CMyProcWnd()
{
	pProcessListView = NULL;
	hIcon = AfxGetApp()->LoadIcon(IDI_ICON_PROC);
}

CMyProcWnd::~CMyProcWnd()
{
}


BEGIN_MESSAGE_MAP(CMyProcWnd, CMyBaseWnd)
	//{{AFX_MSG_MAP(CMyProcWnd)
	ON_WM_CREATE()
	ON_UPDATE_COMMAND_UI(IDS_UPDATEPANE, OnUpdatePane)
	ON_COMMAND(ID_PROCESS_KILL, OnProcessKill)
	ON_UPDATE_COMMAND_UI(ID_PROCESS_KILL, OnUpdateProcessKill)
	ON_COMMAND(ID_PROCESS_FLUSH, OnProcessFlush)
	ON_COMMAND(ID_EXIT,OnClose)
	ON_WM_CONTEXTMENU()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyProcWnd message handlers
int CMyProcWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMyBaseWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	SetActiveView(pProcessListView);
	CXTPCommandBars* pCommandBars = GetCommandBars();
	pCommandBars->SetMenu(_T("Menu Bar"), IDR_MENU_MANA);
	return 0;
}

BOOL CMyProcWnd::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
	CCreateContext pCreateContext; 
	pCreateContext.m_pCurrentFrame = this;
	pCreateContext.m_pCurrentDoc = NULL;
	pCreateContext.m_pNewViewClass = RUNTIME_CLASS(CMyProcListView);
	pProcessListView = (CMyProcListView *) CreateView(&pCreateContext,AFX_IDW_PANE_FIRST);
	return TRUE;
}

void CMyProcWnd::OnProcessKill() 
{
	int nItem = pProcessListView->GetListCtrl().GetNextItem(-1, LVNI_SELECTED);
	PATHFILEITEM m_Item = {0};
	lstrcpy(m_Item.m_FileName, pProcessListView->GetListCtrl().GetItemText(nItem, 0));
	
	CMyWaitTransDlg Transdlg(this);
	Transdlg.m_dTransLen = sizeof(PATHFILEITEM);
	Transdlg.m_Command = CLIENT_PROCESS_KILL;
	CopyMemory(Transdlg.m_TransData, &m_Item, Transdlg.m_dTransLen);
	if(Transdlg.DoModal() == IDCANCEL)
	{
		PostMessage(WM_CLOSE, 0, 0);
		return;
	}
	if(Transdlg.m_Command != 0)
	{
		return;
	}
	pProcessListView->GetListCtrl().DeleteItem(nItem);
}

void CMyProcWnd::OnUpdateProcessKill(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(pProcessListView->GetListCtrl().GetSelectedCount() == 1);
}

void CMyProcWnd::OnProcessFlush() 
{
	CMyWaitTransDlg Transdlg(this);
	Transdlg.m_dTransLen = 0;
	Transdlg.m_Command = CLIENT_PROCESS_LIST;
	if(Transdlg.DoModal() == IDCANCEL)
	{
		PostMessage(WM_CLOSE, 0, 0);
		return;
	}
	if(Transdlg.m_Command != 0)
	{
		return;
	}

	DWORD dwFileSize = Transdlg.m_dTransLen;
	pProcessListView->GetListCtrl().DeleteAllItems();
	LPPROCESSLIST pItem = (LPPROCESSLIST) Transdlg.m_TransData;
	for(DWORD i = 0; i < (dwFileSize / sizeof(PROCESSLIST)); i++)
	{
		pProcessListView->InsertMyItem(pItem);
		pItem ++;
	}
}

void CMyProcWnd::OnUpdatePane(CCmdUI* pCmdUI) 
{
	TCHAR m_Text[256] = {0};
	wsprintf(m_Text, _T("共有%d个进程"), pProcessListView->GetListCtrl().GetItemCount());
	pCmdUI->SetText(m_Text);
}

BOOL CMyProcWnd::StartWork(LPCTSTR sCmdStr)
{
	pProcessListView->ReSizeWnd();
	if(!CMyBaseWnd::StartWork(sCmdStr))
	{
		return FALSE;
	}
	OnProcessFlush();
	return TRUE;
}

void CMyProcWnd::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	if(pWnd->GetSafeHwnd() == pProcessListView->GetSafeHwnd())
	{
		CPoint m_point;
 		CMenu m_menu;
 		m_menu.LoadMenu(IDR_MENU_PROC_LIST);
 		GetCursorPos(&m_point);
 		SetForegroundWindow();   
 		CXTPCommandBars::TrackPopupMenu(m_menu.GetSubMenu(0), TPM_LEFTALIGN|
 			TPM_RIGHTBUTTON,m_point.x,m_point.y,this);
 		m_menu.DestroyMenu();	
	}
}
