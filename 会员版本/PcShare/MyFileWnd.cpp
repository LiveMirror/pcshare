// MainFrame.cpp : implementation file
//

#include "stdafx.h"
#include "PcShare.h"
#include "MyFileWnd.h"
#include "MyClientTitleDlg.h"
#include "MyAttribDlg.h"

#include "MyFileDlDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyFileWnd

IMPLEMENT_DYNCREATE(CMyFileWnd, CMyBaseWnd)

BEGIN_MESSAGE_MAP(CMyFileWnd, CMyBaseWnd)
	//{{AFX_MSG_MAP(CMyFileWnd)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_COMMAND(ID_DIR_FLUSH, OnDirFlush)
	ON_UPDATE_COMMAND_UI(ID_DIR_FLUSH, OnUpdateDirFlush)
	ON_COMMAND(ID_FILE_MANA_PREV, OnDirPrev)
	ON_UPDATE_COMMAND_UI(ID_FILE_MANA_PREV, OnUpdateDirPrev)
	ON_COMMAND(ID_DIR_RIGHT_DELETE, OnDirRightDelete)
	ON_UPDATE_COMMAND_UI(ID_DIR_RIGHT_DELETE, OnUpdateDirRightDelete)
	ON_COMMAND(ID_DIR_RIGHT_EXEC, OnDirRightExec)
	ON_UPDATE_COMMAND_UI(ID_DIR_RIGHT_EXEC, OnUpdateDirRightExec)
	ON_COMMAND(ID_DIR_RIGHT_OPEN, OnDirRightOpen)
	ON_UPDATE_COMMAND_UI(ID_DIR_RIGHT_OPEN, OnUpdateDirRightOpen)
	ON_COMMAND(ID_DIR_RIGHT_RENAME, OnDirRightRename)
	ON_UPDATE_COMMAND_UI(ID_DIR_RIGHT_RENAME, OnUpdateDirRightRename)
	ON_COMMAND(ID_DIR_RIGHT_SELALL, OnDirRightSelall)
	ON_UPDATE_COMMAND_UI(ID_DIR_RIGHT_SELALL, OnUpdateDirRightSelall)
	ON_COMMAND(ID_DIR_CREATE, OnDirCreate)
	ON_UPDATE_COMMAND_UI(ID_DIR_CREATE, OnUpdateDirCreate)
	ON_COMMAND(ID_DIR_RIGHT_DOWNLOAD, OnDirRightDownload)
	ON_UPDATE_COMMAND_UI(ID_DIR_RIGHT_DOWNLOAD, OnUpdateDirRightDownload)
	ON_COMMAND(ID_DIR_RIGHT_UPLOAD, OnDirRightUpload)
	ON_COMMAND(ID_DIR_RIGHT_ATTRIB, OnDirRightAttrib)
	ON_UPDATE_COMMAND_UI(ID_DIR_RIGHT_ATTRIB, OnUpdateDirRightAttrib)
	ON_COMMAND(ID_DIR_FIND_FILE, OnDirFindFile)
	ON_UPDATE_COMMAND_UI(ID_DIR_FIND_FILE, OnUpdateDirFindFile)
	ON_COMMAND(ID_EXIT,OnClose)
	ON_UPDATE_COMMAND_UI(IDS_UPDATEPANE, OnUpdatePane)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_DIRRIGHTRCLICK, OnDirRightRClick)
	ON_MESSAGE(WM_TREESELCHANGED, OnTreeSelChanged)
	ON_MESSAGE(WM_FINDFILE, OnFindFile)
	ON_MESSAGE(WM_DL_DELETE_VIEW, OnDlDeleteView)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyFileWnd message handlers

CMyFileWnd::CMyFileWnd()
{
	m_IsFindFile = FALSE;
	pDirListView = NULL;
	pDirTreeView = NULL;
	pDirFindView = NULL;
	hIcon = AfxGetApp()->LoadIcon(IDI_ICON_FILE);
}

CMyFileWnd::~CMyFileWnd()
{
	
}

void CMyFileWnd::OnClose() 
{
	while(m_DlgList.GetCount() > 0)
	{
		CMyFileDlDlg* pWnd = (CMyFileDlDlg*) m_DlgList.GetHead();
		if(pWnd->GetSafeHwnd())
		{
			pWnd->DestroyWindow();
		}
		delete pWnd;
		m_DlgList.RemoveHead();
	}
	CMyBaseWnd::OnClose();
}

LRESULT CMyFileWnd::OnDlDeleteView(WPARAM wParam,LPARAM lParam)
{
	CMyFileDlDlg* pWnd = (CMyFileDlDlg*) wParam;
	pWnd->DestroyWindow();
	delete pWnd;

	POSITION pos1, pos2;
	for(pos1 = m_DlgList.GetHeadPosition(); ( pos2 = pos1 ) != NULL;)
	{
		if(pWnd == m_DlgList.GetNext(pos1))
		{
			m_DlgList.RemoveAt(pos2);
		}
	}
	return TRUE;
}

void CMyFileWnd::OnUpdatePane(CCmdUI* pCmdUI) 
{
	TCHAR m_Text[256] = {0};
	wsprintf(m_Text, _T("共有%d个对象"), pDirListView->GetListCtrl().GetItemCount());
	pCmdUI->SetText(m_Text);
}

BOOL CMyFileWnd::PreCreateWindow(CREATESTRUCT& cs) 
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
		::AfxGetApp()->LoadIcon(IDI_ICON_FILE));
	return TRUE;
}

int CMyFileWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if(CMyBaseWnd::OnCreate(lpCreateStruct) == -1)
	{
		return -1;
	}
	
	if(!m_IsFindFile)
	{
		if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_TOOLBAR_FILE))
		{
			TRACE0("Failed to create toolbar\n");
			return -1;      // fail to create
		}

		SetActiveView(pDirListView);
		m_ToolBarList.Create(16, 16, ILC_COLOR24 | ILC_MASK, 10, 10);
		m_ToolBarList.Add(IDB_BITMAP_FILE_PREV);
		m_ToolBarList.Add(IDB_BITMAP_FILE_REFRESH);
		m_ToolBarList.Add(IDB_BITMAP_FILE_SEARCH);
		m_ToolBarList.Add(IDB_BITMAP_FILE_DL);
		m_ToolBarList.Add(IDB_BITMAP_FILE_DELETE);

		m_wndToolBar.GetToolBarCtrl().SetImageList(&m_ToolBarList);

		m_wndToolBar.SetButtonText(0, _T("后退"));// "*" to _T("*") [9/15/2007 zhaiyinwei]
		m_wndToolBar.SetButtonText(1, _T("刷新"));// "*" to _T("*") [9/15/2007 zhaiyinwei]
		m_wndToolBar.SetButtonText(2, _T("查找"));// "*" to _T("*") [9/15/2007 zhaiyinwei]
		m_wndToolBar.SetButtonText(3, _T("下载"));// "*" to _T("*") [9/15/2007 zhaiyinwei]
		m_wndToolBar.SetButtonText(4, _T("删除"));// "*" to _T("*") [9/15/2007 zhaiyinwei]
	
		m_wndToolBar.SetButtonStyle(0, TBBS_AUTOSIZE);
		m_wndToolBar.SetButtonStyle(1, TBBS_AUTOSIZE);
		m_wndToolBar.SetButtonStyle(2, TBBS_AUTOSIZE);
		m_wndToolBar.SetButtonStyle(3, TBBS_AUTOSIZE);
		m_wndToolBar.SetButtonStyle(4, TBBS_AUTOSIZE);
	
		m_wndToolBar.GetToolBarCtrl().SetButtonSize(CSize(16, 16 + 19));
		m_wndToolBar.GetToolBarCtrl().SetBitmapSize(CSize(16, 16));
		m_wndToolBar.GetToolBarCtrl().SetDisabledImageList(&m_ToolBarList);
	}
	else
	{
		if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_TOOLBAR_FIND))
		{
			TRACE0("Failed to create toolbar\n");
			return -1;      // fail to create
		}

		SetActiveView(pDirFindView);
		m_ToolBarList.Create(16, 16, ILC_COLOR24 | ILC_MASK, 10, 10);
		m_ToolBarList.Add(IDB_BITMAP_FILE_REFRESH);
		m_ToolBarList.Add(IDB_BITMAP_FILE_DL);
		m_ToolBarList.Add(IDB_BITMAP_FILE_DELETE);

		m_wndToolBar.GetToolBarCtrl().SetImageList(&m_ToolBarList);
		
		m_wndToolBar.SetButtonText(0, _T("刷新"));// "*" to _T("*") [9/15/2007 zhaiyinwei]
		m_wndToolBar.SetButtonText(1, _T("下载"));// "*" to _T("*") [9/15/2007 zhaiyinwei]
		m_wndToolBar.SetButtonText(2, _T("删除"));// "*" to _T("*") [9/15/2007 zhaiyinwei]
	
		m_wndToolBar.SetButtonStyle(0, TBBS_AUTOSIZE);
		m_wndToolBar.SetButtonStyle(1, TBBS_AUTOSIZE);
		m_wndToolBar.SetButtonStyle(2, TBBS_AUTOSIZE);
	
		m_wndToolBar.GetToolBarCtrl().SetButtonSize(CSize(16, 16 + 19));
		m_wndToolBar.GetToolBarCtrl().SetBitmapSize(CSize(16, 16));
		m_wndToolBar.GetToolBarCtrl().SetDisabledImageList(&m_ToolBarList);
	
		pDirListView->SetMyCoulmn(FALSE);
	}
	return 0;
}

// CMyFileWnd 消息处理程序

BOOL CMyFileWnd::GetMyDirLeftPath(HTREEITEM hItem, TCHAR* Path)
{
	CTreeCtrl* pList = &pDirTreeView->GetTreeCtrl();
	if(hItem == NULL) 
	{
		return FALSE;
	}
	if(hItem == pList->GetRootItem())
	{
		lstrcpy(Path, _T(""));
		return TRUE;
	}

	TCHAR m_ItemName[512] = {0};
	TCHAR m_KeyName[2048] = {0};
	TCHAR m_TmpName[2048] = {0};
	HTREEITEM hCurItem = hItem;
	HTREEITEM hParent = NULL;

	while(1)
	{
		wsprintf(m_ItemName, _T("%s\\"), pList->GetItemText(hCurItem));
		wsprintf(m_TmpName, _T("%s%s"), m_ItemName,m_KeyName);
		lstrcpy(m_KeyName,m_TmpName);
		hCurItem = pList->GetParentItem(hCurItem);
		if(hCurItem == pList->GetRootItem())
		{
			break;
		}
	}
	if(m_KeyName[lstrlen(m_KeyName) - 1] == _T('\\'))
	{
		m_KeyName[lstrlen(m_KeyName) - 1] = 0;
	}
	lstrcpy(Path, m_KeyName);
	return TRUE;
}

LRESULT CMyFileWnd::OnTreeSelChanged(WPARAM wParam, LPARAM lParam)
{
	//检查数据正确性
	HTREEITEM hItem = (HTREEITEM) wParam;
	if(hItem == NULL || !IsInit())
	{
		return FALSE;
	}

	//磁盘列表
	if(hItem == pDirTreeView->GetTreeCtrl().GetRootItem())
	{
		GetDiskInfo(hItem);
		return TRUE;
	}

	PATHFILEITEM m_FileItem = {0};	
	if(!GetMyDirLeftPath(hItem, m_FileItem.m_FileName))
	{
		return FALSE;
	}

	CMyWaitTransDlg Transdlg(this);
	Transdlg.m_Command = CLIENT_DIR_LIST;
	Transdlg.m_dTransLen = sizeof(PATHFILEITEM);
	CopyMemory(Transdlg.m_TransData, &m_FileItem, Transdlg.m_dTransLen);
	if(Transdlg.DoModal() == IDCANCEL)
	{
		PostMessage(WM_CLOSE, 0, 0);
		return FALSE;
	}

	//清空当前列表
	pDirListView->SetMyCoulmn(FALSE);
	DeleteTreeChild(hItem);
	if(Transdlg.m_Command != 0)
	{
		DisplayButton(&pDirTreeView->GetTreeCtrl(), hItem, 0);
		return FALSE;
	}

	//显示列表
	LPDIRFILELIST pDirListItem = (LPDIRFILELIST) Transdlg.m_TransData;
	BOOL IsHasChild = FALSE;
	for(DWORD i = 0; i < Transdlg.m_dTransLen / sizeof(DIRFILELIST); i++)
	{
		TCHAR* pItemName = StrRChr(pDirListItem->m_ItemPath, NULL, _T('\\'));
		if(pItemName == NULL) 
		{
			continue;
		}
		pItemName ++;

		if(pDirListItem->m_FileData.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)
		{
			TV_INSERTSTRUCT tvstruct = {0};
			tvstruct.hParent = hItem;
			tvstruct.hInsertAfter = TVI_LAST;
			tvstruct.item.iImage = 5;
			tvstruct.item.iSelectedImage = 6;
			tvstruct.item.pszText = pItemName;
			tvstruct.item.mask = TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_TEXT;
			HTREEITEM hItem = pDirTreeView->GetTreeCtrl().InsertItem(&tvstruct);
			DisplayButton(&pDirTreeView->GetTreeCtrl(),hItem,1);
			pDirListView->SendMessage(WM_INSERTMYITEM,(WPARAM) pDirListItem,NULL);
			IsHasChild = TRUE;
		}
		pDirListItem ++;
	}

	pDirListItem = (LPDIRFILELIST) Transdlg.m_TransData;
	for(DWORD j = 0; j < Transdlg.m_dTransLen / sizeof(DIRFILELIST); j++)
	{
		if(!(pDirListItem->m_FileData.dwFileAttributes &FILE_ATTRIBUTE_DIRECTORY))
		{
			pDirListView->SendMessage(WM_INSERTMYITEM, (WPARAM) pDirListItem,NULL);
		}
		pDirListItem ++;
	}

	if(!IsHasChild)
	{
		DisplayButton(&pDirTreeView->GetTreeCtrl(), hItem, 0);
	}
	pDirTreeView->GetTreeCtrl().Expand(hItem, TVE_EXPAND);
	return TRUE;
}

LRESULT CMyFileWnd::OnFindFile(WPARAM wParam, LPARAM lParam)
{
	CMyWaitTransDlg Transdlg(this);
	CopyMemory(Transdlg.m_TransData, (LPVOID) wParam, sizeof(PATHFILEITEM));
	Transdlg.m_Command = CLIENT_FIND_FILE;
	Transdlg.m_dTransLen = sizeof(PATHFILEITEM);
	if(Transdlg.DoModal() == IDCANCEL)
	{
		PostMessage(WM_CLOSE, 0, 0);
		return FALSE;
	}

	//清空当前列表
	pDirListView->SetMyCoulmn(FALSE);
	if(Transdlg.m_Command != 0)
	{
		return FALSE;
	}

	pDirListView->GetListCtrl().DeleteAllItems();

	//显示列表
	LPDIRFILELIST pDirListItem = (LPDIRFILELIST) Transdlg.m_TransData;
	for(DWORD j = 0; j < Transdlg.m_dTransLen / sizeof(DIRFILELIST); j++)
	{
		pDirListView->SendMessage(WM_INSERTMYITEM, (WPARAM) pDirListItem, NULL);
		pDirListItem ++;
	}
	return TRUE;
}

BOOL CMyFileWnd::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) 
{
	if(!m_WndSplitter.CreateStatic(this, 1, 2))
	{
		return FALSE;
	}

	if(!m_IsFindFile)
	{
		if(!m_WndSplitter.CreateView(0, 1, RUNTIME_CLASS(CMyDirListView), CSize(0, 0), pContext) || 
		!m_WndSplitter.CreateView(0, 0, RUNTIME_CLASS(CMyDirTreeView), CSize(0, 0), pContext))
		{
			m_WndSplitter.DestroyWindow();
			return FALSE;
		}
		pDirListView = (CMyDirListView*) m_WndSplitter.GetPane(0, 1);
		pDirTreeView = (CMyDirTreeView*) m_WndSplitter.GetPane(0, 0);
	}
	else
	{
		if(!m_WndSplitter.CreateView(0, 1, RUNTIME_CLASS(CMyDirListView), CSize(0, 0), pContext) || 
		!m_WndSplitter.CreateView(0, 0, RUNTIME_CLASS(CMyFileFindView), CSize(0, 0), pContext))
		{
			m_WndSplitter.DestroyWindow();
			return FALSE;
		}
		pDirListView = (CMyDirListView*) m_WndSplitter.GetPane(0, 1);
		pDirFindView = (CMyFileFindView*) m_WndSplitter.GetPane(0, 0);
	}
	return TRUE;
}

void CMyFileWnd::OnSize(UINT nType, int cx, int cy) 
{
	CMyBaseWnd::OnSize(nType, cx, cy);
	if(m_WndSplitter.GetSafeHwnd())
	{
		if(m_IsFindFile)
		{
			m_WndSplitter.SetColumnInfo(0, 210,10);
			m_WndSplitter.RecalcLayout();
		}
		else
		{
			m_WndSplitter.SetColumnInfo(0, cx * 1/4,10);
			m_WndSplitter.RecalcLayout();
		}
	}
}

void CMyFileWnd::OnDirFlush() 
{
	if(!m_IsFindFile)
	{
		HTREEITEM hItem = pDirTreeView->GetTreeCtrl().GetSelectedItem();
		if(hItem == NULL) 
		{
			return;
		}
		PostMessage(WM_TREESELCHANGED, (WPARAM) hItem, NULL);
	}
	else
	{
		pDirFindView->OnFileFind();
	}
}

void CMyFileWnd::OnUpdateDirFlush(CCmdUI* pCmdUI) 
{
	if(m_IsFindFile)
	{
		pDirFindView->UpdateData();
		pCmdUI->Enable(!pDirFindView->m_Dir.IsEmpty());
	}
}

void CMyFileWnd::OnDirPrev() 
{
	HTREEITEM hItem = pDirTreeView->GetTreeCtrl().GetSelectedItem();
	if(hItem == NULL) 
	{
		return;
	}
	HTREEITEM hParent = pDirTreeView->GetTreeCtrl().GetParentItem(hItem);
	if(hParent == NULL) 
	{
		return;
	}
	pDirTreeView->GetTreeCtrl().SelectItem(hParent);
}

void CMyFileWnd::OnUpdateDirPrev(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(pDirTreeView->GetTreeCtrl().GetSelectedItem() != pDirTreeView->GetTreeCtrl().GetRootItem());
}

void CMyFileWnd::OnUpdateDirRightExec(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(pDirListView->GetListCtrl().GetSelectedCount() == 1);
}

void CMyFileWnd::OnDirRightExec() 
{
	CMyWaitTransDlg Transdlg(this);

	//取远程目录
	PATHFILEITEM m_Item = {0};
	int nItem = pDirListView->GetListCtrl().GetNextItem(-1, LVNI_SELECTED);
	lstrcpy(m_Item.m_FileName, pDirListView->GetListCtrl().GetItemText(nItem, 4));

	//取文件名称
	lstrcat(m_Item.m_FileName, _T("\\"));
	lstrcat(m_Item.m_FileName, pDirListView->GetListCtrl().GetItemText(nItem, 0));

	//取运行参数和运行方式
	Transdlg.m_dTransLen = sizeof(PATHFILEITEM);
	CopyMemory(Transdlg.m_TransData, &m_Item, Transdlg.m_dTransLen);
	Transdlg.m_Command = CLIENT_EXEC_FILE;
	if(Transdlg.DoModal() == IDCANCEL)
	{
		PostMessage(WM_CLOSE, 0, 0);
		return;
	}
	if(Transdlg.m_Command != 0)
	{
		return;
	}

	//发送命令到客户机
	MessageBox(_T("远程运行程序成功!"), PS_CAOZUO, MB_OK|MB_ICONINFORMATION);
}

LRESULT CMyFileWnd::OnDirRightRClick(WPARAM wParam,LPARAM lParam)
{
	CMenu m_menu;
	if(pDirListView->GetListCtrl().GetSelectedCount() > 0)
	{
		if(pDirListView->GetListCtrl().GetSelectedCount() == 1)
		{
			int nItem = pDirListView->GetListCtrl().GetNextItem(-1, LVNI_SELECTED);
			if(pDirListView->GetListCtrl().GetItemText(nItem, 1) == _T("文件"))
			{
				//文件
				m_menu.LoadMenu(IDR_MENU_DIR_RIGHT);
				CPoint m_point;
				GetCursorPos(&m_point);
				SetForegroundWindow();   
				CXTPCommandBars::TrackPopupMenu(m_menu.GetSubMenu(0), TPM_LEFTALIGN|
					TPM_RIGHTBUTTON,m_point.x,m_point.y,this);
				m_menu.DestroyMenu();
				return TRUE;
			}
			else if(pDirListView->GetListCtrl().GetItemText(nItem, 1) == _T("文件夹"))
			{
				//目录
				m_menu.CreatePopupMenu();
				m_menu.AppendMenu(MF_STRING,ID_DIR_RIGHT_OPEN, _T("打开"));
				m_menu.AppendMenu(MF_STRING,ID_DIR_RIGHT_DOWNLOAD, _T("下载"));
				m_menu.AppendMenu(MF_STRING,ID_DIR_RIGHT_DELETE, _T("删除"));
				m_menu.AppendMenu(MF_STRING,ID_DIR_RIGHT_RENAME, _T("重命名"));
				m_menu.AppendMenu(MF_STRING,ID_DIR_FIND_FILE, _T("搜索文件"));
				m_menu.AppendMenu(MF_SEPARATOR);
				m_menu.AppendMenu(MF_STRING,ID_DIR_RIGHT_ATTRIB, _T("属性"));
			}
			else
			{
				//磁盘
				m_menu.CreatePopupMenu();
				m_menu.AppendMenu(MF_STRING,ID_DIR_RIGHT_OPEN, _T("打开"));
				m_menu.AppendMenu(MF_STRING,ID_DIR_FIND_FILE, _T("搜索文件"));
				m_menu.AppendMenu(MF_SEPARATOR);
				m_menu.AppendMenu(MF_STRING,ID_DIR_RIGHT_ATTRIB, _T("属性"));
			}
		}
		else
		{
			//多选
			if(m_IsFindFile || (pDirTreeView != NULL && 
				pDirTreeView->GetTreeCtrl().GetSelectedItem() != pDirTreeView->GetTreeCtrl().GetRootItem()))
			{
				//非磁盘
				m_menu.CreatePopupMenu();
				m_menu.AppendMenu(MF_STRING,ID_DIR_RIGHT_DELETE, _T("删除"));
				m_menu.AppendMenu(MF_STRING,ID_DIR_RIGHT_DOWNLOAD, _T("下载"));
			}
			else 
			{
				return FALSE;
			}
		}
	}
	else
	{
		if(m_IsFindFile)
		{
			m_menu.CreatePopupMenu();
			m_menu.AppendMenu(MF_STRING,ID_DIR_FLUSH, _T("刷新"));
		}
		else
		{
			HTREEITEM hItem = pDirTreeView->GetTreeCtrl().GetSelectedItem();
			if(hItem != pDirTreeView->GetTreeCtrl().GetRootItem())
			{
				m_menu.LoadMenu(IDR_MENU_DIR_RIGHT_EMPTY);
				CPoint m_point;
				GetCursorPos(&m_point);
				SetForegroundWindow();   
				CXTPCommandBars::TrackPopupMenu(m_menu.GetSubMenu(0), TPM_LEFTALIGN|TPM_RIGHTBUTTON,m_point.x,m_point.y,this);
				m_menu.DestroyMenu();
				return TRUE;
			}
			else
			{
				m_menu.CreatePopupMenu();
				m_menu.AppendMenu(MF_STRING,ID_DIR_FLUSH, _T("刷新"));
			}
		}
	}
	CPoint m_point;
	GetCursorPos(&m_point);
	SetForegroundWindow();   
	CXTPCommandBars::TrackPopupMenu(&m_menu, TPM_LEFTALIGN|TPM_RIGHTBUTTON,m_point.x,m_point.y,this);
	m_menu.DestroyMenu();
	return TRUE;
}

void CMyFileWnd::OnDirRightOpen() 
{
	int nItem = pDirListView->GetListCtrl().GetNextItem(-1,LVNI_SELECTED);
	if(pDirListView->GetListCtrl().GetItemText(nItem, 1) == _T("文件"))	//文件
	{
		PATHFILEITEM m_FileItem = {0};
		wsprintf(m_FileItem.m_FileName, _T("%s\\%s"),
			pDirListView->GetListCtrl().GetItemText(nItem, 4),
			pDirListView->GetListCtrl().GetItemText(nItem, 0));

		CMyWaitTransDlg Transdlg(this);
		Transdlg.m_Command = CLIENT_FILE_INFO;
		CopyMemory(Transdlg.m_TransData, &m_FileItem, sizeof(PATHFILEITEM));
		Transdlg.m_dTransLen = sizeof(PATHFILEITEM);
		if(Transdlg.DoModal() == IDCANCEL)
		{
			PostMessage(WM_CLOSE, 0, 0);
			return;
		}
		if(Transdlg.m_Command != 0)
		{
			return;
		}

		LPFILEINFO pFileStatus = (LPFILEINFO) Transdlg.m_TransData;

		INTERFILEINFO m_FileInfo;

		TCHAR m_TempPath[256] = {0};
		GetTempPath(200,m_TempPath);

		wsprintf(m_FileInfo.m_LocalFile, _T("%s%s"), m_TempPath,
			pDirListView->GetListCtrl().GetItemText(nItem, 0));

		wsprintf(m_FileInfo.m_RemoteFile, _T("%s\\%s"),
			pDirListView->GetListCtrl().GetItemText(nItem, 4),
			pDirListView->GetListCtrl().GetItemText(nItem, 0));

		//取文件长度
		m_FileInfo.m_FileLen.HighPart = pFileStatus->m_FileData.nFileSizeHigh;
		m_FileInfo.m_FileLen.LowPart = pFileStatus->m_FileData.nFileSizeLow;

		TCHAR m_Id[256] = {0};

		//唯一标识
		if(!GetStrValue(m_ExtCmdStr, _T("id"), m_Id))
		{
			MessageBox(_T("系统错误"), _T("提示"));
			return ;
		}

		m_FileInfo.m_IsExec = TRUE;
		m_FileInfo.m_IsUpLoad = FALSE;
		CMyFileDlDlg* pDlg = new CMyFileDlDlg;
		m_DlgList.AddTail(pDlg);
		if(!pDlg->Create(this, m_Id, &m_FileInfo))
		{
			delete pDlg;
			MessageBox(_T("系统错误"), _T("提示"));
			return ;
		}
		pDlg->ShowWindow(SW_SHOW);
	}
	else //目录或者磁盘
	{
		TCHAR m_TmpItemName[256] = {0};
		TCHAR m_ItemName[256] = {0};
		lstrcpy(m_ItemName,(LPCTSTR) pDirListView->GetListCtrl().GetItemText(nItem,0));
		HTREEITEM hItem = pDirTreeView->GetTreeCtrl().GetSelectedItem();
		if (pDirTreeView->GetTreeCtrl().ItemHasChildren(hItem))
		{
		   HTREEITEM hNextItem;
		   HTREEITEM hChildItem = pDirTreeView->GetTreeCtrl().GetChildItem(hItem);
		   while (hChildItem != NULL)
		   {
			   hNextItem = pDirTreeView->GetTreeCtrl().GetNextItem(hChildItem,TVGN_NEXT);
			   lstrcpy(m_TmpItemName,pDirTreeView->GetTreeCtrl().GetItemText(hChildItem));
			   if(!StrCmp(m_TmpItemName,m_ItemName))
			   {
				   pDirTreeView->GetTreeCtrl().SelectItem(hChildItem);
				   break;
			   }
			   hChildItem = hNextItem;
		   }
		}
	}
}

void CMyFileWnd::OnUpdateDirRightOpen(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(pDirListView->GetListCtrl().GetSelectedCount() == 1);
}

void CMyFileWnd::OnDirRightRename() 
{
	int nItem = pDirListView->GetListCtrl().GetNextItem(-1,LVNI_SELECTED);

	TCHAR m_FileName[256] = {0};
	lstrcpy(m_FileName, pDirListView->GetListCtrl().GetItemText(nItem,0));
	CMyClientTitleDlg dlg(_T("新名称"), _T("更改名称"), m_FileName);
	if(dlg.DoModal() != IDOK) 
	{
		return;
	}

	RENAMEFILEITEM m_ReNameItem = {0};
	wsprintf(m_ReNameItem.m_OldFileName, _T("%s\\%s"),
		pDirListView->GetListCtrl().GetItemText(nItem,4),
		pDirListView->GetListCtrl().GetItemText(nItem,0));

	wsprintf(m_ReNameItem.m_NewFileName, _T("%s\\%s"),
		pDirListView->GetListCtrl().GetItemText(nItem,4), dlg.m_Value);

	CMyWaitTransDlg Transdlg(this);
	CopyMemory(Transdlg.m_TransData, &m_ReNameItem, sizeof(RENAMEFILEITEM));
	Transdlg.m_dTransLen = sizeof(RENAMEFILEITEM);
	Transdlg.m_Command = CLIENT_RENAME;
	if(Transdlg.DoModal() == IDCANCEL)
	{
		PostMessage(WM_CLOSE, 0, 0);
		return;
	}
	if(Transdlg.m_Command != 0)
	{
		return;
	}
	if(pDirListView->GetListCtrl().GetItemText(nItem, 1) == _T("文件夹"))
	{
		//查看是否是目录
		HTREEITEM hItem = pDirTreeView->GetTreeCtrl().GetSelectedItem();
		if (pDirTreeView->GetTreeCtrl().ItemHasChildren(hItem))
		{
		   HTREEITEM hNextItem;
		   HTREEITEM hChildItem = pDirTreeView->GetTreeCtrl().GetChildItem(hItem);
		   while (hChildItem != NULL)
		   {
			  hNextItem = pDirTreeView->GetTreeCtrl().GetNextItem(hChildItem,TVGN_NEXT);
			  if(pDirTreeView->GetTreeCtrl().GetItemText(hChildItem) == dlg.m_Value)
			   {
				  pDirTreeView->GetTreeCtrl().SetItemText(hChildItem,
					  pDirListView->GetListCtrl().GetItemText(nItem,0));
				  break;
			   }
			  hChildItem = hNextItem;
		   }
		}
	}
	pDirListView->GetListCtrl().SetItemText(nItem,0, dlg.m_Value);
}


void CMyFileWnd::OnUpdateDirRightDelete(CCmdUI* pCmdUI) 
{
	BOOL IsEnable = FALSE;
	if(pDirListView->GetListCtrl().GetSelectedCount() > 0)
	{
		int nItem = pDirListView->GetListCtrl().GetNextItem(-1,LVNI_SELECTED);
		if(pDirListView->GetListCtrl().GetItemText(nItem, 1) == _T("文件夹") ||
			pDirListView->GetListCtrl().GetItemText(nItem, 1) == _T("文件"))
		{
			IsEnable = TRUE;
		}
	}
	pCmdUI->Enable(IsEnable);
}

void CMyFileWnd::OnUpdateDirRightRename(CCmdUI* pCmdUI) 
{
	BOOL IsEnable = FALSE;
	if(pDirListView->GetListCtrl().GetSelectedCount() == 1)
	{
		int nItem = pDirListView->GetListCtrl().GetNextItem(-1,LVNI_SELECTED);
		if(pDirListView->GetListCtrl().GetItemText(nItem, 1) == _T("文件夹") ||
			pDirListView->GetListCtrl().GetItemText(nItem, 1) == _T("文件"))
		{
			IsEnable = TRUE;
		}
	}
	pCmdUI->Enable(IsEnable);
}

void CMyFileWnd::OnDirCreate() 
{
	TCHAR m_RemotePath[2048] = {0};
	GetMyDirLeftPath(pDirTreeView->GetTreeCtrl().GetSelectedItem(),m_RemotePath);

	CMyClientTitleDlg dlg(_T("创建目录"), _T("目录名称"), _T("新建目录"));
	if(dlg.DoModal() != IDOK)
	{
		return;
	}

	PATHFILEITEM m_PathFileItem = {0};
	wsprintf(m_PathFileItem.m_FileName,  _T("%s\\%s"), m_RemotePath, dlg.m_Value);

	CMyWaitTransDlg Transdlg(this);
	Transdlg.m_Command = CLIENT_CREATE_DIR;
	CopyMemory(Transdlg.m_TransData, &m_PathFileItem, sizeof(m_PathFileItem));
	Transdlg.m_dTransLen = sizeof(m_PathFileItem);
	if(Transdlg.DoModal() == IDCANCEL)
	{
		PostMessage(WM_CLOSE, 0, 0);
		return;
	}
	if(Transdlg.m_Command != 0)
	{
		return;
	}

	DIRFILELIST m_DirList = {0};
	m_DirList.m_FileData.dwFileAttributes ^= FILE_ATTRIBUTE_DIRECTORY;
	wsprintf(m_DirList.m_ItemPath, _T("%s"), m_PathFileItem.m_FileName);
	int nItem = 0;
	pDirListView->SendMessage(WM_INSERTMYITEM, (WPARAM) &m_DirList, (LPARAM) &nItem);
	if(nItem == -1) 
	{
		return;
	}
	
	TCHAR m_Name[256] = {0};
	lstrcpy(m_Name, dlg.m_Value);
	TV_INSERTSTRUCT tvstruct;
	tvstruct.hParent = pDirTreeView->GetTreeCtrl().GetSelectedItem();
	tvstruct.hInsertAfter = TVI_LAST;
	tvstruct.item.iImage = 2;
	tvstruct.item.iSelectedImage = 7;
	tvstruct.item.pszText = m_Name;
	tvstruct.item.mask = TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_TEXT;
	HTREEITEM hItem = pDirTreeView->GetTreeCtrl().InsertItem(&tvstruct);	
	pDirListView->GetListCtrl().SetItemState(nItem, LVIS_SELECTED, LVIS_SELECTED);
}

void CMyFileWnd::OnDirRightAttrib() 
{
	int nItem = pDirListView->GetListCtrl().GetNextItem(-1, LVNI_SELECTED);
	
	PATHFILEITEM m_FileItem = {0};
	wsprintf(m_FileItem.m_FileName, _T("%s\\%s"),
		pDirListView->GetListCtrl().GetItemText(nItem, 4),
		pDirListView->GetListCtrl().GetItemText(nItem, 0));

	CMyWaitTransDlg Transdlg(this);
	if(pDirListView->GetListCtrl().GetItemText(nItem, 1) == _T("文件夹"))
	{
		Transdlg.m_Command = CLIENT_DIR_INFO;
	}
	else if(pDirListView->GetListCtrl().GetItemText(nItem, 1) == _T("文件"))
	{
		Transdlg.m_Command = CLIENT_FILE_INFO;
	}
	else
	{
		wsprintf(m_FileItem.m_FileName, _T("%s"), pDirListView->GetListCtrl().GetItemText(nItem, 0));
		Transdlg.m_Command = CLIENT_DISK_INFO;
	}

	CopyMemory(Transdlg.m_TransData, &m_FileItem, sizeof(PATHFILEITEM));
	Transdlg.m_dTransLen = sizeof(PATHFILEITEM);
	if(Transdlg.DoModal() == IDCANCEL)
	{
		PostMessage(WM_CLOSE, 0, 0);
		return;
	}
	if(Transdlg.m_Command != 0)
	{
		return;
	}

	if(pDirListView->GetListCtrl().GetItemText(nItem, 1) == _T("文件夹"))
	{
		CMyAttribDlg dlg(this, *((LPDIRINFO) Transdlg.m_TransData));
		dlg.DoModal();
	}
	else if(pDirListView->GetListCtrl().GetItemText(nItem, 1) == _T("文件"))
	{
		CMyAttribDlg dlg(this, *((LPFILEINFO) Transdlg.m_TransData));
		dlg.DoModal();
	}
	else
	{
		CMyAttribDlg dlg(this, *((LPDISKINFO) Transdlg.m_TransData));
		dlg.DoModal();
	}
}

void CMyFileWnd::OnUpdateDirRightAttrib(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(pDirListView->GetListCtrl().GetSelectedCount() == 1);
}

void CMyFileWnd::OnDirRightDelete() 
{
	TCHAR m_PathName[1024] = {0};
	lstrcpy(m_PathName, pDirListView->GetListCtrl().GetItemText(0, 4));

	int m_Count = pDirListView->GetListCtrl().GetSelectedCount();
	if(m_Count <= 0)
	{
		return;
	}
	if(m_Count * sizeof(DIRFILELIST) > TRAN_DATA_LEN)
	{
		MessageBox(_T("选择操作的条目太多，请分批删除！"), PS_WARNING, MB_OKCANCEL|MB_ICONQUESTION);
		return;
	}
	if(MessageBox(_T("真要删除?"), _T("删除"), MB_OKCANCEL|MB_ICONQUESTION) != IDOK)
	{
		return;
	}

	CMyWaitTransDlg Transdlg(this);
	int  nItem = -1;
	LPDIRFILELIST pItemData = (LPDIRFILELIST) Transdlg.m_TransData;
	Transdlg.m_dTransLen = 0;
	for(int i = 0; i < m_Count; i++)
	{
		memset(pItemData, 0, sizeof(DIRFILELIST));
		nItem = pDirListView->GetListCtrl().GetNextItem(nItem, LVNI_SELECTED);
		wsprintf(pItemData->m_ItemPath, _T("%s\\%s"), m_PathName,
			pDirListView->GetListCtrl().GetItemText(nItem,0));
		Transdlg.m_dTransLen += sizeof(DIRFILELIST);
		pItemData++;
	}
	Transdlg.m_Command = CLIENT_FILE_DELETE;
	if(Transdlg.DoModal() == IDCANCEL)
	{
		PostMessage(WM_CLOSE, 0, 0);
		return;
	}
	if(Transdlg.m_Command != 0)
	{
		if(m_Count != 1)
		{
			OnDirFlush();
		}
		return;
	}
	nItem = -1;
	for(i = 0; i < m_Count; i++)
	{
		//删除目录,删除树项目
		nItem = pDirListView->GetListCtrl().GetNextItem(nItem, LVNI_SELECTED);
		if(pDirListView->GetListCtrl().GetItemText(nItem, 1) == _T("文件夹"))
		{
			HTREEITEM hNextItem;
			HTREEITEM hItem = pDirTreeView->GetTreeCtrl().GetSelectedItem();
			HTREEITEM hChildItem = pDirTreeView->GetTreeCtrl().GetChildItem(hItem);
			while (hChildItem != NULL)
			{
				hNextItem = pDirTreeView->GetTreeCtrl().GetNextItem(hChildItem, TVGN_NEXT);
				if(pDirTreeView->GetTreeCtrl().GetItemText(hChildItem) ==
					pDirListView->GetListCtrl().GetItemText(nItem,0))
				{
					pDirTreeView->GetTreeCtrl().DeleteItem(hChildItem);
					break;
				}
				hChildItem = hNextItem;
			}
		}
	}
	for(i = 0; i < m_Count; i++)
	{
		nItem = pDirListView->GetListCtrl().GetNextItem(-1, LVNI_SELECTED);
		pDirListView->GetListCtrl().DeleteItem(nItem);
	}
}

void CMyFileWnd::OnDirRightDownload() 
{
	//本地路径
	TCHAR m_LocalPath[2048] = {0};
	CXTBrowseDialog dlg;
	dlg.SetTitle(_T("下载到指定目录"));
	if(dlg.DoModal() != IDOK)
	{
		return;
	}
	wsprintf(m_LocalPath, _T("%s"), dlg.GetSelPath());
	if(m_LocalPath[lstrlen(m_LocalPath) - 1] == _T('\\'))
	{
		m_LocalPath[lstrlen(m_LocalPath) - 1] = 0;
	}

	TCHAR m_RemotePath[2048] = {0};
	int nItem = pDirListView->GetListCtrl().GetNextItem(-1, LVNI_SELECTED);
	lstrcpy(m_RemotePath, pDirListView->GetListCtrl().GetItemText(nItem, 4));

	int m_Count = pDirListView->GetListCtrl().GetSelectedCount();
	if(m_Count <= 0)
	{
		return;
	}
	if(m_Count * sizeof(DIRFILELIST) > TRAN_DATA_LEN)
	{
		MessageBox(_T("选择操作的条目太多，请分批进行！"), PS_WARNING, MB_OKCANCEL|MB_ICONQUESTION);
		return;
	}

	CMyWaitTransDlg Transdlg(this);
	nItem = -1;
	LPDIRFILELIST pItemData = (LPDIRFILELIST) Transdlg.m_TransData;
	Transdlg.m_dTransLen = 0;
	for(int i = 0; i < m_Count; i++)
	{
		memset(pItemData, 0, sizeof(DIRFILELIST));
		nItem = pDirListView->GetListCtrl().GetNextItem(nItem, LVNI_SELECTED);
		wsprintf(pItemData->m_ItemPath, _T("%s\\%s"), m_RemotePath,
			pDirListView->GetListCtrl().GetItemText(nItem, 0));
		if(pDirListView->GetListCtrl().GetItemText(nItem, 1) == _T("文件夹"))
		{
			pItemData->m_FileData.dwFileAttributes = 1;
		}
		else
		{
			pItemData->m_FileData.dwFileAttributes = 0;
		}
		Transdlg.m_dTransLen += sizeof(DIRFILELIST);
		pItemData++;
	}
	Transdlg.m_Command = CLIENT_DOWN_FILE_LIST;
	if(Transdlg.DoModal() == IDCANCEL)
	{
		PostMessage(WM_CLOSE, 0, 0);
		return;
	}
	if(Transdlg.m_Command != 0)
	{
		return;
	}

	//查找主窗口
	TCHAR nCWndName[256] = {0};
	wsprintf(nCWndName, _T("%s下载管理"), PS_TITLE);
	HWND hWnd = ::FindWindow(NULL, nCWndName);
	if(hWnd == NULL)
	{
		MessageBox(_T("系统错误"), _T("提示"));
		return ;
	}

	MYINSERTDLFILEINFO m_InsertDlFileInfo = {0};

	//唯一标识
	if(!GetStrValue(m_ExtCmdStr, _T("id"), m_InsertDlFileInfo.m_Id))
	{
		MessageBox(_T("系统错误"), _T("提示"));
		return ;
	}

	//肉机名称
	if(!GetStrValue(m_ExtCmdStr, _T("clienttitie"), m_InsertDlFileInfo.m_Title))
	{
		MessageBox(_T("系统错误"), _T("提示"));
		return ;
	}

	//目录
	lstrcpy(m_InsertDlFileInfo.m_LocalPath, m_LocalPath);
	lstrcpy(m_InsertDlFileInfo.m_RemotePath, m_RemotePath);

	memcpy(Transdlg.m_TransData + Transdlg.m_dTransLen, &m_InsertDlFileInfo, sizeof(MYINSERTDLFILEINFO));

	//填充结构
	COPYDATASTRUCT ct = {0};
	ct.lpData = Transdlg.m_TransData;
	ct.cbData = Transdlg.m_dTransLen + sizeof(MYINSERTDLFILEINFO);
	ct.dwData = PS_LOCAL_FILETRAN;
	if(!::SendMessage(hWnd, WM_COPYDATA, 0, (LPARAM) &ct))
	{
		MessageBox(_T("系统错误"), _T("提示"));
	}
}

void CMyFileWnd::OnUpdateDirRightDownload(CCmdUI* pCmdUI) 
{
	BOOL IsEnable = FALSE;
	if(pDirListView->GetListCtrl().GetSelectedCount() > 0)
	{
		int nItem = pDirListView->GetListCtrl().GetNextItem(-1,LVNI_SELECTED);
		if(pDirListView->GetListCtrl().GetItemText(nItem, 1) == _T("文件夹") ||
			pDirListView->GetListCtrl().GetItemText(nItem, 1) == _T("文件"))
		{
			IsEnable = TRUE;
		}
	}
	pCmdUI->Enable(IsEnable);
}

void CMyFileWnd::OnDirFindFile()
{
	TCHAR m_ItemName[256] = {0};
	int nItem = pDirListView->GetListCtrl().GetNextItem(-1,LVNI_SELECTED);
	if(pDirListView->m_IsDiskList)
	{
		wsprintf(m_ItemName, _T("%s"),
			pDirListView->GetListCtrl().GetItemText(nItem,0));
	}
	else
	{
		wsprintf(m_ItemName, _T("%s\\%s"),
			pDirListView->GetListCtrl().GetItemText(nItem,4),
			pDirListView->GetListCtrl().GetItemText(nItem,0));
	}

	CString m_WndText = m_ExtCmdStr;
	m_WndText.Replace(_T("文件管理"), _T("文件搜索"));
	m_WndText += _T("searchpath=");
	m_WndText += m_ItemName;
	m_WndText += ";";

	//启动新的子进程
	TCHAR m_lpCmdLine[512] = {0};
	GetModuleFileName(NULL, m_lpCmdLine, 500);
	lstrcat(m_lpCmdLine, _T(" "));
	lstrcat(m_lpCmdLine, m_WndText);
	ShowWindow(SW_SHOWMAXIMIZED);

	if(!IsMulitProcess())
	{
		StartMoudle(m_WndText);
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

void CMyFileWnd::OnDirRightUpload() 
{
	OPENFILENAME OpenFileName = {0};
	TCHAR szFile[MAX_PATH] = {0};
	TCHAR szFileName[MAX_PATH] = {0};
  	TCHAR szFilter[1024] = {0};
	lstrcpy(szFilter, _T("所有文件(*.*)"));
	lstrcpy(szFilter + lstrlen(szFilter) + 1, _T("*.*"));
	OpenFileName.lStructSize       = sizeof(OPENFILENAME);
    OpenFileName.hwndOwner         = GetSafeHwnd();
	OpenFileName.lpstrFilter       = szFilter;
    OpenFileName.lpstrCustomFilter = NULL;
    OpenFileName.nMaxCustFilter    = 0;
    OpenFileName.nFilterIndex      = 0;
    OpenFileName.lpstrFile         = szFile;
    OpenFileName.nMaxFile          = sizeof(szFile);
    OpenFileName.lpstrFileTitle    = szFileName;
    OpenFileName.nMaxFileTitle     = sizeof(szFileName);
    OpenFileName.lpstrInitialDir   = NULL;
    OpenFileName.lpstrTitle        = _T("选择上传到客户机器的文件");
    OpenFileName.nFileOffset       = 0;
    OpenFileName.nFileExtension    = 0;
    OpenFileName.lpstrDefExt       = NULL;
    OpenFileName.Flags             = OFN_EXPLORER | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
	if(!GetOpenFileName(&OpenFileName))
	{
		return;
	}

	WIN32_FILE_ATTRIBUTE_DATA m_FileData = {0};
	if(!GetFileAttributesEx(szFile, GetFileExInfoStandard, &m_FileData))
	{
		MessageBox(_T("无法打开文件!"), _T("错误"));
		return;
	}

	TCHAR m_RemotePath[2048] = {0};
	GetMyDirLeftPath(pDirTreeView->GetTreeCtrl().GetSelectedItem(),m_RemotePath);

	INTERFILEINFO m_FileInfo;
	lstrcpy(m_FileInfo.m_LocalFile,szFile);
	wsprintf(m_FileInfo.m_RemoteFile, _T("%s\\%s"), m_RemotePath, szFileName);

	//取文件长度
	m_FileInfo.m_FileLen.HighPart = m_FileData.nFileSizeHigh;
	m_FileInfo.m_FileLen.LowPart = m_FileData.nFileSizeLow;

	TCHAR m_Id[256] = {0};

	//唯一标识
	if(!GetStrValue(m_ExtCmdStr, _T("id"), m_Id))
	{
		MessageBox(_T("系统错误"), _T("提示"));
		return ;
	}

	m_FileInfo.m_IsExec = TRUE;
	m_FileInfo.m_IsUpLoad = TRUE;

	CMyFileDlDlg* pDlg = new CMyFileDlDlg;
	m_DlgList.AddTail(pDlg);
	if(!pDlg->Create(this, m_Id, &m_FileInfo))
	{
		delete pDlg;
		MessageBox(_T("系统错误"), _T("提示"));
		return ;
	}

	pDlg->ShowWindow(SW_SHOW);
		
	//查看是否已经存在
	TCHAR* pFind = StrRChr(m_FileInfo.m_RemoteFile, NULL, _T('\\'));
	if(pFind == NULL)
	{
		pFind = m_FileInfo.m_RemoteFile;
	}
	int nCount = pDirListView->GetListCtrl().GetItemCount();
	for(int i = 0; i < nCount; i++)
	{
		if(pDirListView->GetListCtrl().GetItemText(i, 0) == (pFind + 1))
		{
			return;
		}
	}

	DIRFILELIST m_Item = {0};
	lstrcpy(m_Item.m_ItemPath, m_FileInfo.m_RemoteFile);
	memcpy(&m_Item.m_FileData, &m_FileData, sizeof(WIN32_FILE_ATTRIBUTE_DATA));
	pDirListView->SendMessage(WM_INSERTMYITEM, (WPARAM) &m_Item, NULL); 
}

void CMyFileWnd::OnDirRightSelall() 
{
	for(int i = 0; i < pDirListView->GetListCtrl().GetItemCount(); i++)
	{
		pDirListView->GetListCtrl().SetItemState(i,LVIS_SELECTED,LVIS_SELECTED);
	}
}

void CMyFileWnd::OnUpdateDirRightSelall(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(pDirListView->GetListCtrl().GetItemCount() > 0);
}

void CMyFileWnd::OnUpdateDirFindFile(CCmdUI* pCmdUI) 
{
	BOOL bIsEnable = FALSE;
	if(pDirListView->GetListCtrl().GetSelectedCount() == 1)
	{
		int nItem = pDirListView->GetListCtrl().GetNextItem(-1,LVNI_SELECTED);
		if(pDirListView->GetListCtrl().GetItemText(nItem, 1) != _T("文件"))
		{
			bIsEnable = TRUE;
		}
	}
	pCmdUI->Enable(bIsEnable);
}

void CMyFileWnd::OnUpdateDirCreate(CCmdUI* pCmdUI) 
{
	BOOL IsEnable = FALSE;
	if(pDirListView->GetListCtrl().GetSelectedCount() == 0)
	{
		if(pDirTreeView->GetTreeCtrl().GetSelectedItem() 
			!= pDirTreeView->GetTreeCtrl().GetRootItem())
		{
			IsEnable = TRUE;
		}
	}
	pCmdUI->Enable(IsEnable);
}

void CMyFileWnd::DisplayButton(CTreeCtrl *pTree,HTREEITEM hti,int nChild)
{
	TVITEM tvi;
	tvi.mask = TVIF_CHILDREN;
	tvi.hItem = hti;
	tvi.cChildren = nChild;
	pTree->SetItem(&tvi);
}

void CMyFileWnd::GetDiskInfo(HTREEITEM hItem)
{
	//取客户磁盘信息列表
	CMyWaitTransDlg Transdlg(this);
	Transdlg.m_Command = CLIENT_DISK_LIST;
	Transdlg.m_dTransLen = 0;
	if(Transdlg.DoModal() == IDCANCEL)
	{
		PostMessage(WM_CLOSE, 0, 0);
		return;
	}
	if(Transdlg.m_Command != 0)
	{
		return;
	}

	//增加新项目
	pDirListView->SetMyCoulmn(TRUE);
	DeleteTreeChild(hItem);

	int		m_Image = 0;
	TCHAR	m_ItemType[20] = {0};
	LPDISKINFO pItem = (LPDISKINFO) Transdlg.m_TransData;
	for(DWORD i = 0; i < Transdlg.m_dTransLen / sizeof(DISKINFO); i++)
	{
		//图标和驱动器名称
		if(pItem->m_Name[0] == _T('A') || pItem->m_Name[0] == _T('B'))
		{
			m_Image = 0;
			lstrcpy(m_ItemType, _T("软驱"));
		}
		else if(pItem->m_DeviceType == DRIVE_CDROM)
		{
			m_Image = 1;
			lstrcpy(m_ItemType, _T("光驱"));
		}
		else if(pItem->m_DeviceType == DRIVE_REMOVABLE)
		{
			m_Image = 4;
			lstrcpy(m_ItemType, _T("可移动驱动器"));
		}
		else if(pItem->m_DeviceType == DRIVE_REMOTE)
		{
			m_Image = 3;
			lstrcpy(m_ItemType, _T("远程磁盘"));
		}
		else 
		{
			m_Image = 2;
			lstrcpy(m_ItemType, _T("本地磁盘"));
		}
	
		pItem->m_Name[2] = 0x00;

		//增加树控
		TV_INSERTSTRUCT tvstruct;
		tvstruct.hParent = hItem;
		tvstruct.hInsertAfter = TVI_LAST;
		tvstruct.item.iImage = m_Image;
		tvstruct.item.iSelectedImage = m_Image;
		tvstruct.item.pszText = pItem->m_Name;
		tvstruct.item.mask = TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_TEXT;
		HTREEITEM hcItem = pDirTreeView->GetTreeCtrl().InsertItem(&tvstruct);	
		DisplayButton(&pDirTreeView->GetTreeCtrl(),hcItem,1);

		//名称
		int nItem = pDirListView->GetListCtrl().InsertItem(
			pDirListView->GetListCtrl().GetItemCount(), pItem->m_Name, m_Image);

		//类型
		pDirListView->GetListCtrl().SetItemText(nItem, 1, m_ItemType);

		//总空间大小
		TCHAR m_Size[256];
		StrFormatByteSize64(pItem->TotalNumberOfBytes, m_Size, 256);
		pDirListView->GetListCtrl().SetItemText(nItem, 2, m_Size);

		//剩余空间大小
		StrFormatByteSize64(pItem->TotalNumberOfFreeBytes, m_Size, 256);
		pDirListView->GetListCtrl().SetItemText(nItem, 3, m_Size);
		pDirListView->GetListCtrl().SetItemData(nItem, nItem);

		pItem++;
	}
	pDirTreeView->GetTreeCtrl().Expand(hItem, TVE_EXPAND);
}

void CMyFileWnd::DeleteTreeChild(HTREEITEM hItem)
{
	if(pDirTreeView->GetTreeCtrl().ItemHasChildren(hItem))
	{
	   HTREEITEM hNextItem = NULL;
	   HTREEITEM hChildItem = pDirTreeView->GetTreeCtrl().GetChildItem(hItem);
	   while (hChildItem != NULL)
	   {
		  hNextItem = pDirTreeView->GetTreeCtrl().GetNextItem(hChildItem,TVGN_NEXT);
		  pDirTreeView->GetTreeCtrl().DeleteItem(hChildItem);
		  hChildItem = hNextItem;
	   }
	}
}

BOOL CMyFileWnd::StartWork(LPCTSTR sCmdStr)
{
	if(m_IsFindFile)
	{
		pDirListView->SetMyCoulmn(FALSE);
		TCHAR m_FindPath[256] = {0};
		if(!GetStrValue(sCmdStr, _T("searchpath"), m_FindPath))
		{
			ASSERT(FALSE);
		}
		pDirFindView->SetPath(m_FindPath);
	}
	else
	{
		pDirListView->SetMyCoulmn(TRUE);
	}

	if(!CMyBaseWnd::StartWork(sCmdStr))
	{
		return FALSE;
	}

	if(!m_IsFindFile)
	{
		PostMessage(WM_TREESELCHANGED, (WPARAM) pDirTreeView->GetTreeCtrl().GetRootItem(), NULL);
	}
	return TRUE;
}

