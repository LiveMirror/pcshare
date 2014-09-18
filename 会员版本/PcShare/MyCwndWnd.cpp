// MyCwndWnd.cpp : implementation file
//

#include "stdafx.h"
#include "PcShare.h"
#include "MyCwndWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyCwndWnd

IMPLEMENT_DYNCREATE(CMyCwndWnd, CMyBaseWnd)

CMyCwndWnd::CMyCwndWnd()
{
	pCwndListView = NULL;
	hIcon = AfxGetApp()->LoadIcon(IDI_ICON_CWND);
}

CMyCwndWnd::~CMyCwndWnd()
{
}


BEGIN_MESSAGE_MAP(CMyCwndWnd, CMyBaseWnd)
	//{{AFX_MSG_MAP(CMyCwndWnd)
	ON_WM_CREATE()
	ON_UPDATE_COMMAND_UI(IDS_UPDATEPANE, OnUpdatePane)
	ON_COMMAND(ID_CWND_MAX, OnCwndMax)
	ON_UPDATE_COMMAND_UI(ID_CWND_MAX, OnUpdateCwndMax)
	ON_COMMAND(ID_CWND_MIN, OnCwndMin)
	ON_UPDATE_COMMAND_UI(ID_CWND_MIN, OnUpdateCwndMin)
	ON_COMMAND(ID_CWND_HIDE, OnCwndHide)
	ON_UPDATE_COMMAND_UI(ID_CWND_HIDE, OnUpdateCwndHide)
	ON_COMMAND(ID_CWND_SHOW, OnCwndShow)
	ON_UPDATE_COMMAND_UI(ID_CWND_SHOW, OnUpdateCwndShow)
	ON_COMMAND(ID_CWND_CLOSE, OnCwndClose)
	ON_UPDATE_COMMAND_UI(ID_CWND_CLOSE, OnUpdateCwndClose)
	ON_COMMAND(ID_CWND_FLUSH, OnCwndFlush)
	ON_COMMAND(ID_EXIT,OnClose)
	ON_WM_CONTEXTMENU()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyCwndWnd message handlers
int CMyCwndWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMyBaseWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	SetActiveView(pCwndListView);

	CXTPCommandBars* pCommandBars = GetCommandBars();
	pCommandBars->SetMenu(_T("Menu Bar"), IDR_MENU_MANA);

	return 0;
}

BOOL CMyCwndWnd::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
	CCreateContext pCreateContext; 
	pCreateContext.m_pCurrentFrame = this;
	pCreateContext.m_pCurrentDoc = NULL;
	pCreateContext.m_pNewViewClass = RUNTIME_CLASS(CMyCwndListView);
	pCwndListView = (CMyCwndListView *) CreateView(&pCreateContext,AFX_IDW_PANE_FIRST);
	return TRUE;
}

void CMyCwndWnd::OnCwndMax()
{
	OnCwndCtrl(SW_SHOWMAXIMIZED);
}

void CMyCwndWnd::OnUpdateCwndMax(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(pCwndListView->GetListCtrl().GetSelectedCount() == 1);
}

void CMyCwndWnd::OnCwndMin()
{
	OnCwndCtrl(SW_SHOWMINIMIZED);
}

void CMyCwndWnd::OnUpdateCwndMin(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(pCwndListView->GetListCtrl().GetSelectedCount() == 1);
}

void CMyCwndWnd::OnCwndHide()
{
	OnCwndCtrl(SW_HIDE);
}

void CMyCwndWnd::OnUpdateCwndHide(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(pCwndListView->GetListCtrl().GetSelectedCount() == 1);
}

void CMyCwndWnd::OnCwndShow()
{
	OnCwndCtrl(SW_SHOW);
}

void CMyCwndWnd::OnUpdateCwndShow(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(pCwndListView->GetListCtrl().GetSelectedCount() == 1);
}

void CMyCwndWnd::OnCwndClose()
{
	OnCwndCtrl(WM_CLOSE);
}

void CMyCwndWnd::OnUpdateCwndClose(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(pCwndListView->GetListCtrl().GetSelectedCount() == 1);
}

void CMyCwndWnd::OnCwndCtrl(UINT nType) 
{
	int nItem = pCwndListView->GetListCtrl().GetNextItem(-1, LVNI_SELECTED);
	WINDOWITEM m_Item = {0};
	m_Item.m_hWnd = (HWND) StrToInt(pCwndListView->GetListCtrl().GetItemText(nItem, 0));
	m_Item.m_CtrlType = nType;

	CMyWaitTransDlg Transdlg(this);
	Transdlg.m_dTransLen = sizeof(WINDOWITEM);
	Transdlg.m_Command = CLIENT_CONTROL_WINDOWS;
	CopyMemory(Transdlg.m_TransData, &m_Item, sizeof(WINDOWITEM));

	if(Transdlg.DoModal() == IDCANCEL)
	{
		PostMessage(WM_CLOSE, 0, 0);
		return;
	}
	if(Transdlg.m_Command != 0)
	{
		return;
	}
	if(nType == WM_CLOSE)
	{
		pCwndListView->GetListCtrl().DeleteItem(nItem);
	}
}

void CMyCwndWnd::OnCwndFlush() 
{
	CMyWaitTransDlg Transdlg(this);
	Transdlg.m_dTransLen = 0;
	Transdlg.m_Command = CLIENT_ENUM_WINDOWS;
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
	pCwndListView->GetListCtrl().DeleteAllItems();
	LPWINDOWITEM pItem = (LPWINDOWITEM) Transdlg.m_TransData;
	for(DWORD i = 0; i < (dwFileSize / sizeof(WINDOWITEM)); i++)
	{
		pCwndListView->InsertMyItem(pItem);
		pItem ++;
	}
}

void CMyCwndWnd::OnUpdatePane(CCmdUI* pCmdUI) 
{
	TCHAR m_Text[256] = {0};
	wsprintf(m_Text, _T("共有%d个窗口"), pCwndListView->GetListCtrl().GetItemCount());
	pCmdUI->SetText(m_Text);
}

BOOL CMyCwndWnd::StartWork(LPCTSTR sCmdStr)
{
	pCwndListView->ReSizeWnd();
	if(!CMyBaseWnd::StartWork(sCmdStr))
	{
		return FALSE;
	}
	OnCwndFlush();
	return TRUE;
}

void CMyCwndWnd::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	if(pWnd->GetSafeHwnd() == pCwndListView->GetSafeHwnd())
	{
		CPoint m_point;
 		CMenu m_menu;
 		m_menu.LoadMenu(IDR_MENU_CWND_LIST);
 		GetCursorPos(&m_point);
 		SetForegroundWindow();   
 		CXTPCommandBars::TrackPopupMenu(m_menu.GetSubMenu(0), TPM_LEFTALIGN|
 			TPM_RIGHTBUTTON, m_point.x, m_point.y, this);
 		m_menu.DestroyMenu();	
	}
}