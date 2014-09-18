// MyRegtWnd.cpp : implementation file
//

#include "stdafx.h"
#include "PcShare.h"
#include "MyRegtWnd.h"
#include "MyRegtEditValue.h"
#include "MyClientTitleDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyRegtWnd

IMPLEMENT_DYNCREATE(CMyRegtWnd, CMyBaseWnd)

CMyRegtWnd::CMyRegtWnd()
{
	pRegListView = NULL;
	pRegTreeView = NULL;
	hIcon = AfxGetApp()->LoadIcon(IDI_ICON_REGT);
}

CMyRegtWnd::~CMyRegtWnd()
{
}


BEGIN_MESSAGE_MAP(CMyRegtWnd, CMyBaseWnd)
	//{{AFX_MSG_MAP(CMyRegtWnd)
	ON_WM_CREATE()
	ON_COMMAND(ID_EXIT,OnClose)
	ON_COMMAND(ID_REG_RIGHT_DELETE, OnRegRightDelete)
	ON_UPDATE_COMMAND_UI(ID_REG_RIGHT_DELETE, OnUpdateRegRightDelete)
	ON_COMMAND(ID_REG_LEFT_DELETE, OnRegLeftDelete)
	ON_COMMAND(ID_REG_RIGHT_CREATE_STR, OnRegRightCreateStr)
	ON_COMMAND(ID_REG_RIGHT_CREATE_STR_EXT, OnRegRightCreateStrExt)
	ON_COMMAND(ID_REG_RIGHT_FLUSH, OnRegRightFlush)
	ON_COMMAND(ID_REG_RIGHT_MODIFY, OnRegRightModify)
	ON_UPDATE_COMMAND_UI(ID_REG_RIGHT_MODIFY, OnUpdateRegRightModify)
	ON_COMMAND(ID_REG_RIGHT_CREATE_INT, OnRegRightCreateInt)
	ON_MESSAGE(WM_REGLEFTDBCLICK,OnRegLeftDbClick)
	ON_MESSAGE(WM_REGLEFTRCLICK,OnRegLeftRClick)
	ON_MESSAGE(WM_REGRIGHTRBCLICK,OnRegRightRClick)
	ON_UPDATE_COMMAND_UI(IDS_UPDATEPANE, OnUpdatePane)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyRegtWnd message handlers

int CMyRegtWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMyBaseWnd::OnCreate(lpCreateStruct) == -1)
	{
		return -1;
	}
	SetActiveView(pRegListView);
	CXTPCommandBars* pCommandBars = GetCommandBars();
	pCommandBars->SetMenu(_T("Menu Bar"), IDR_MENU_MANA);
	return 0;
}

LRESULT CMyRegtWnd::OnRegLeftRClick(WPARAM wParam,LPARAM lParam)
{
	CTreeCtrl * pList = &pRegTreeView->GetTreeCtrl();
	HTREEITEM hItem = pList->GetSelectedItem();
	if(hItem == NULL || hItem == pList->GetRootItem())
	{
		return FALSE;
	}
	CPoint m_point;
	CMenu m_menu;
	m_menu.LoadMenu(IDR_MENU_REGT_LEFT);
	GetCursorPos(&m_point);
	SetForegroundWindow();   
	CXTPCommandBars::TrackPopupMenu(m_menu.GetSubMenu(0),TPM_LEFTALIGN|
		TPM_RIGHTBUTTON,m_point.x,m_point.y,this);
	m_menu.DestroyMenu();
	return TRUE;
}

void CMyRegtWnd::DisplayButton(CTreeCtrl *pTree,HTREEITEM hti,int nChild)
{
	TVITEM tvi;
	tvi.mask = TVIF_CHILDREN;
	tvi.hItem = hti;
	tvi.cChildren = nChild;
	pTree->SetItem(&tvi);
}

LRESULT CMyRegtWnd::OnRegLeftDbClick(WPARAM wParam,LPARAM lParam)
{
	HTREEITEM hItem = (HTREEITEM) wParam;
	if(hItem == NULL || hItem == pRegTreeView->GetTreeCtrl().GetRootItem() || !IsInit())
	{
		return FALSE;
	}

	PSREGINFO m_Item = {0};
	if(!GetMyRegLeftPath(hItem, m_Item.m_Key, m_Item.m_RootKey))
	{
		return FALSE;
	}

	//通信
	CMyWaitTransDlg Transdlg(this);
	Transdlg.m_Command = CLIENT_REG_QUERY;
	Transdlg.m_dTransLen = sizeof(PSREGINFO);
	CopyMemory(Transdlg.m_TransData, &m_Item, Transdlg.m_dTransLen);
	if(Transdlg.DoModal() == IDCANCEL)
	{
		PostMessage(WM_CLOSE, 0, 0);
		return FALSE;
	}
	if(Transdlg.m_Command == NOT_FIND_ANY_FILE)
	{
		pRegListView->GetListCtrl().DeleteAllItems();
		pRegListView->GetListCtrl().InsertItem(0, _T("(默认)"), 1);
		pRegListView->GetListCtrl().SetItemText(0, 1, _T("REG_SZ"));
		pRegListView->GetListCtrl().SetItemText(0, 2, _T("(数值未设置)"));
		DisplayButton(&pRegTreeView->GetTreeCtrl(), hItem, 0);
		return FALSE;
	}
	else if(Transdlg.m_Command != 0)
	{
		DisplayButton(&pRegTreeView->GetTreeCtrl(),hItem,0);
		return FALSE;
	}

	if(pRegTreeView->GetTreeCtrl().ItemHasChildren(hItem))
	{
	   HTREEITEM hNextItem;
	   HTREEITEM hChildItem = pRegTreeView->GetTreeCtrl().GetChildItem(hItem);
	   while (hChildItem != NULL)
	   {
		  hNextItem = pRegTreeView->GetTreeCtrl().GetNextItem(hChildItem, TVGN_NEXT);
		  pRegTreeView->GetTreeCtrl().DeleteItem(hChildItem);
		  hChildItem = hNextItem;
	   }
	}
	pRegListView->GetListCtrl().DeleteAllItems();

	LPREGINFO pItem = (LPREGINFO) Transdlg.m_TransData;
	TV_INSERTSTRUCT tvstruct;
	int nItem = 0;
	BOOL IsHasChild = FALSE;
	for(ULONG i = 0; i < (Transdlg.m_dTransLen / sizeof(REGINFO)); i++)
	{
		if(!pItem->m_IsKey)
		{
			if(pItem->m_NameLen == 0xffffffff)
			{
				nItem = pRegListView->GetListCtrl().InsertItem(
					pRegListView->GetListCtrl().GetItemCount(), _T("默认"), 1);
			}
			else
			{
				nItem = pRegListView->GetListCtrl().InsertItem(
					pRegListView->GetListCtrl().GetItemCount(), pItem->m_Name, 1);
			}

			if(pItem->m_Datalen == 0xffffffff)
			{
				pRegListView->GetListCtrl().SetItemText(nItem, 1, _T("REG_SZ"));
				pRegListView->GetListCtrl().SetItemText(nItem, 2, _T("数值未设置"));
				pItem ++;
				continue;
			}

			if(pItem->m_DataType == REG_SZ)
			{
				pRegListView->GetListCtrl().SetItemText(nItem, 1, _T("REG_SZ"));
				pItem->m_Data[pItem->m_Datalen] = 0;
				TCHAR m_TmpBuf[1024] = {0};
				lstrcpy(m_TmpBuf, (TCHAR*) pItem->m_Data);
				pRegListView->GetListCtrl().SetItemText(nItem, 2, m_TmpBuf);
			}
			else if(pItem->m_DataType == REG_MULTI_SZ)
			{
				pRegListView->GetListCtrl().SetItemText(nItem, 1, _T("REG_MULTI_SZ"));
				pItem->m_Data[pItem->m_Datalen] = 0;
				TCHAR m_TmpBuf[1024] = {0};
				lstrcpy(m_TmpBuf, (TCHAR*) pItem->m_Data);
				pRegListView->GetListCtrl().SetItemText(nItem, 2, m_TmpBuf);
			}
			else if(pItem->m_DataType == REG_BINARY)
			{
				pRegListView->GetListCtrl().SetItemText(nItem, 1, _T("REG_BINARY"));
				TCHAR m_TmpBuf[256] = {0};
				for(DWORD i = 0; i < pItem->m_Datalen; i++)
				{
					TCHAR m_Text[20] = {0};
					wsprintf(m_Text, _T("%02x "), (BYTE) pItem->m_Data[i]);
					lstrcat(m_TmpBuf, m_Text);
				}
				m_TmpBuf[lstrlen(m_TmpBuf) - 1] = 0;
				pRegListView->GetListCtrl().SetItemText(nItem, 2, m_TmpBuf);
				pItem ++;
				continue;
			}
			else if(pItem->m_DataType == REG_BINARY)
			{
				pRegListView->GetListCtrl().SetItemText(nItem, 1, _T("REG_BINARY"));
				TCHAR m_TmpBuf[256] = {0};
				for(DWORD i = 0; i < pItem->m_Datalen; i++)
				{
					TCHAR m_Text[20] = {0};
					wsprintf(m_Text, _T("%02x "), (BYTE) pItem->m_Data[i]);
					lstrcat(m_TmpBuf, m_Text);
				}
				m_TmpBuf[lstrlen(m_TmpBuf) - 1] = 0;
				pRegListView->GetListCtrl().SetItemText(nItem, 2, m_TmpBuf);
				pItem ++;
				continue;
			}
			else if(pItem->m_DataType == REG_DWORD)
			{
				pRegListView->GetListCtrl().SetItemText(nItem,1, _T("REG_DWORD"));

				TCHAR m_Tmpbuf[40] = {0};
				wsprintf(m_Tmpbuf, _T("0x%08x(%u)"), (UINT) *((DWORD*) pItem->m_Data),
					(UINT) *((DWORD*) pItem->m_Data));
				pRegListView->GetListCtrl().SetItemText(nItem, 2, m_Tmpbuf);
			}
			else if(pItem->m_DataType == REG_EXPAND_SZ)
			{
				pRegListView->GetListCtrl().SetItemText(nItem, 1, _T("REG_EXPAND_SZ"));
				pItem->m_Data[pItem->m_Datalen] = 0;
				TCHAR m_TmpBuf[1024] = {0};
				lstrcpy(m_TmpBuf, (TCHAR*) pItem->m_Data);
				pRegListView->GetListCtrl().SetItemText(nItem, 2, m_TmpBuf);
			}
			else
			{
				pItem ++;
				continue;
			}
		}
		else
		{
			tvstruct.hParent = hItem;
			tvstruct.hInsertAfter = TVI_LAST;
			tvstruct.item.iImage = 0;
			tvstruct.item.iSelectedImage = 1;
			tvstruct.item.pszText = pItem->m_Name;
			tvstruct.item.mask = TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_TEXT;
			HTREEITEM m_Parent = pRegTreeView->GetTreeCtrl().InsertItem(&tvstruct);
			DisplayButton(&pRegTreeView->GetTreeCtrl(),m_Parent,1);
			pRegTreeView->GetTreeCtrl().SetItemData(m_Parent,0);
			IsHasChild = TRUE;
		}
		pItem ++;
	}
	if(!IsHasChild)
	{
		DisplayButton(&pRegTreeView->GetTreeCtrl(), hItem, 0);
	}
	pRegTreeView->GetTreeCtrl().Expand(hItem,TVE_EXPAND);
	return TRUE;
}

LRESULT CMyRegtWnd::OnRegRightRClick(WPARAM wParam,LPARAM lParam)
{
	CPoint m_point;
	CMenu m_menu;

	if(pRegTreeView->GetTreeCtrl().GetSelectedItem()
		== pRegTreeView->GetTreeCtrl().GetRootItem() ||
		pRegTreeView->GetTreeCtrl().GetSelectedItem() == NULL)
	{
		return FALSE;
	}

	CListCtrl * pRightList = &pRegListView->GetListCtrl();
	if(pRightList->GetSelectedCount() > 0)
	{
		m_menu.LoadMenu(IDR_MENU_REGT_RIGHT);
	}
	else
	{
		m_menu.LoadMenu(IDR_MENU_REGT_RIGHT_EMPTY);
	}
	GetCursorPos(&m_point);
	SetForegroundWindow();   
	CXTPCommandBars::TrackPopupMenu(m_menu.GetSubMenu(0), TPM_LEFTALIGN|TPM_RIGHTBUTTON, m_point.x, m_point.y, this);
	m_menu.DestroyMenu();	
	return TRUE;
}

HKEY CMyRegtWnd::GetBootKey(TCHAR *KeyName)
{
	HKEY m_Rootkey = NULL;
	if(!StrCmpNI(KeyName, _T("HKEY_CLASSES_ROOT"), 17))
	{
		m_Rootkey = HKEY_CLASSES_ROOT;
	}
	else if(!StrCmpNI(KeyName, _T("HKEY_CURRENT_CONFIG"), 19))
	{
		m_Rootkey = HKEY_CURRENT_CONFIG;
	}
	else if(!StrCmpNI(KeyName, _T("HKEY_CURRENT_USER"), 17))
	{
		m_Rootkey = HKEY_CURRENT_USER ;
	}
	else if(!StrCmpNI(KeyName, _T("HKEY_LOCAL_MACHINE"), 18))
	{
		m_Rootkey = HKEY_LOCAL_MACHINE ;
	}
	else if(!StrCmpNI(KeyName, _T("HKEY_USERS"), 10))
	{
		m_Rootkey = HKEY_USERS ;
	}
	return m_Rootkey;
}

BOOL CMyRegtWnd::GetMyRegLeftPath(HTREEITEM hItem, TCHAR* Path, HKEY& Key)
{
	CTreeCtrl* pList = &pRegTreeView->GetTreeCtrl();
	if(hItem == NULL || hItem == pList->GetRootItem())
	{
		return FALSE;
	}
	
	TCHAR m_ItemName[512] = {0};
	TCHAR m_KeyName[2048] = {0};
	TCHAR m_TmpName[2048] = {0};
	HTREEITEM hCurItem = hItem;
	HTREEITEM hParent = NULL;

	while(1)
	{
		wsprintf(m_ItemName, _T("%s\\"), pList->GetItemText(hCurItem));
		wsprintf(m_TmpName, _T("%s%s"), m_ItemName, m_KeyName);
		lstrcpy(m_KeyName, m_TmpName);
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

	TCHAR* pDataStart = StrChr(m_KeyName, _T('\\'));
	if(pDataStart == NULL)
	{
		lstrcpy(Path, _T(""));
	}
	else
	{
		*pDataStart = 0;
		pDataStart++;
		lstrcpy(Path, pDataStart);
	}
	Key = GetBootKey(m_KeyName);
	return TRUE;
}

void CMyRegtWnd::OnRegRightNewValue(DWORD dType)
{
	PSREGINFO m_Item = {0};
	if(!GetMyRegLeftPath(pRegTreeView->GetTreeCtrl().GetSelectedItem(), m_Item.m_Key, m_Item.m_RootKey)) 
	{
		return;
	}
	m_Item.m_ItemType = dType;

	CMyRegtEditValue dlg(_T("新建键值"), _T(""), _T(""), m_Item.m_ItemType);
 	if(dlg.DoModal() != IDOK) 
	{
		return;
	}

	lstrcpy(m_Item.m_ItemName, dlg.m_ItemName);
	if(dType == REG_DWORD)
	{
		*((DWORD*) m_Item.m_ItemValue) = StrToInt(dlg.m_ItemValue);
		m_Item.m_ValueLen = sizeof(DWORD);
	}
	else
	{
		lstrcpy((TCHAR*) m_Item.m_ItemValue, dlg.m_ItemValue);
		m_Item.m_ValueLen = lstrlen((TCHAR*) m_Item.m_ItemValue) * sizeof(TCHAR);
	}
	CMyWaitTransDlg Transdlg(this);
	Transdlg.m_Command = CLIENT_REG_UPDATE_VALUE;
	Transdlg.m_dTransLen = sizeof(PSREGINFO);
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

	int nItem = 0;
	if(dType == REG_SZ)
	{
		nItem = pRegListView->GetListCtrl().InsertItem
			(pRegListView->GetListCtrl().GetItemCount(), dlg.m_ItemName, 1);
		pRegListView->GetListCtrl().SetItemText(nItem, 1, _T("REG_SZ"));
		pRegListView->GetListCtrl().SetItemText(nItem, 2, dlg.m_ItemValue);
	}
	else if(dType == REG_DWORD)
	{
		nItem = pRegListView->GetListCtrl().InsertItem
			(pRegListView->GetListCtrl().GetItemCount(), dlg.m_ItemName, 0);
		pRegListView->GetListCtrl().SetItemText(nItem, 1, _T("REG_DWORD"));

		CString m_Str;
		m_Str.Format(_T("0x%08x(%u)"), StrToInt(dlg.m_ItemValue), StrToInt(dlg.m_ItemValue));
		pRegListView->GetListCtrl().SetItemText(nItem, 2, m_Str);
	}
	else if(dType == REG_EXPAND_SZ)
	{
		nItem = pRegListView->GetListCtrl().InsertItem
			(pRegListView->GetListCtrl().GetItemCount(), dlg.m_ItemName, 1);
		pRegListView->GetListCtrl().SetItemText(nItem, 1, _T("REG_EXPAND_SZ"));
		pRegListView->GetListCtrl().SetItemText(nItem, 2, dlg.m_ItemValue);
	}
}

void CMyRegtWnd::OnRegRightDelete() 
{
	PSREGINFO m_Item = {0};
	if(!GetMyRegLeftPath(pRegTreeView->GetTreeCtrl().GetSelectedItem(), m_Item.m_Key, m_Item.m_RootKey)) 
	{
		return;
	}
	int nItem = pRegListView->GetListCtrl().GetNextItem(-1, LVNI_SELECTED);
	lstrcpy(m_Item.m_ItemName, pRegListView->GetListCtrl().GetItemText(nItem, 0));

	CMyWaitTransDlg Transdlg(this);
	Transdlg.m_Command = CLIENT_REG_DELETE_VALUE;
	Transdlg.m_dTransLen = sizeof(PSREGINFO);
	CopyMemory(Transdlg.m_TransData, &m_Item, Transdlg.m_dTransLen);
	if(Transdlg.DoModal() == IDCANCEL)
	{
		PostMessage(WM_CLOSE, 0, 0);
		return;
	}
	pRegListView->GetListCtrl().DeleteItem(nItem);
}

void CMyRegtWnd::OnUpdateRegRightDelete(CCmdUI* pCmdUI) 
{
	BOOL IsEnable = FALSE;
	int nItem = -1;
	CTreeCtrl * pList = &pRegTreeView->GetTreeCtrl();
	CListCtrl * pRightList = &pRegListView->GetListCtrl();
	HTREEITEM hItem = pList->GetSelectedItem();
	nItem = pRightList->GetNextItem(nItem,LVNI_SELECTED);
	if(hItem != NULL && hItem != pList->GetRootItem() &&
		pRightList->GetSelectedCount() == 1 &&
		pRightList->GetItemText(nItem,0) != _T("(默认)"))
		IsEnable = TRUE;
	pCmdUI->Enable(IsEnable);	
}

void CMyRegtWnd::OnRegLeftDelete() 
{
	HTREEITEM hItem = pRegTreeView->GetTreeCtrl().GetSelectedItem();
	PSREGINFO m_Item = {0};
	if(!GetMyRegLeftPath(hItem, m_Item.m_Key, m_Item.m_RootKey))
	{
		return;
	}

	CMyWaitTransDlg Transdlg(this);
	Transdlg.m_Command = CLIENT_REG_DELETE_KEY;
	Transdlg.m_dTransLen = sizeof(PSREGINFO);
	CopyMemory(Transdlg.m_TransData, &m_Item, Transdlg.m_dTransLen);
	if(Transdlg.DoModal() == IDCANCEL)
	{
		PostMessage(WM_CLOSE, 0, 0);
		return;
	}
	pRegTreeView->GetTreeCtrl().DeleteItem(hItem);
}

void CMyRegtWnd::OnRegRightCreateStr() 
{
	OnRegRightNewValue(REG_SZ);
}

void CMyRegtWnd::OnRegRightCreateStrExt() 
{
	OnRegRightNewValue(REG_EXPAND_SZ);
}

void CMyRegtWnd::OnRegRightCreateInt() 
{
	OnRegRightNewValue(REG_DWORD);
}

void CMyRegtWnd::OnRegRightFlush() 
{
	CTreeCtrl * pList = &pRegTreeView->GetTreeCtrl();
	HTREEITEM hItem = pList->GetSelectedItem();
	if(hItem == NULL) 
	{
		return;
	}
	PostMessage(WM_REGLEFTDBCLICK, (WPARAM) hItem, NULL);
}

void CMyRegtWnd::OnRegRightModify() 
{
 	PSREGINFO m_Item = {0};
 	if(!GetMyRegLeftPath(pRegTreeView->GetTreeCtrl().GetSelectedItem(), m_Item.m_Key, m_Item.m_RootKey))
	{
  		return;
	}
  
  	int nItem = pRegListView->GetListCtrl().GetNextItem(-1, LVNI_SELECTED);
   	lstrcpy(m_Item.m_ItemName, pRegListView->GetListCtrl().GetItemText(nItem, 0));
 
 	if(pRegListView->GetListCtrl().GetItemText(nItem,1) == _T("REG_DWORD"))
	{
  		m_Item.m_ItemType = REG_DWORD;
	}
  	else if(pRegListView->GetListCtrl().GetItemText(nItem,1) == _T("REG_SZ"))
	{
  		m_Item.m_ItemType = REG_SZ;
	}
	else if(pRegListView->GetListCtrl().GetItemText(nItem,1) == _T("REG_EXPAND_SZ"))
	{
  		m_Item.m_ItemType = REG_EXPAND_SZ;
	}
   
 	CMyRegtEditValue dlg(_T("编辑键值"), m_Item.m_ItemName, 
		pRegListView->GetListCtrl().GetItemText(nItem, 2), m_Item.m_ItemType);
 	if(dlg.DoModal() != IDOK) 
	{
		return;
	}
	if(StrCmp(m_Item.m_ItemName, _T("默认")) == 0)
	{
		lstrcpy(m_Item.m_ItemName, _T(""));
	}
	
	if(m_Item.m_ItemType == REG_DWORD)
	{
		*((DWORD*) m_Item.m_ItemValue) = StrToInt(dlg.m_ItemValue);
		m_Item.m_ValueLen = sizeof(DWORD);
	}
	else
	{
		lstrcpy((TCHAR*) m_Item.m_ItemValue, dlg.m_ItemValue);
		m_Item.m_ValueLen = lstrlen((TCHAR*) m_Item.m_ItemValue) * sizeof(TCHAR);
	}
  
	CMyWaitTransDlg Transdlg(this);
	Transdlg.m_dTransLen = sizeof(PSREGINFO);
	CopyMemory(Transdlg.m_TransData, &m_Item, Transdlg.m_dTransLen);
 	Transdlg.m_Command = CLIENT_REG_UPDATE_VALUE;
  	if(Transdlg.DoModal() == IDCANCEL)
	{
		PostMessage(WM_CLOSE, 0, 0);
		return;
	}
 	if(Transdlg.m_Command != 0)
 	{
  		return ;
 	}

 	if(m_Item.m_ItemType == REG_DWORD)
 	{
 		CString m_Str;
		m_Str.Format(_T("0x%08x(%u)"), StrToInt(dlg.m_ItemValue), StrToInt(dlg.m_ItemValue));
		pRegListView->GetListCtrl().SetItemText(nItem, 2, m_Str);
 	}
 	else
	{
 		pRegListView->GetListCtrl().SetItemText(nItem, 2, dlg.m_ItemValue);
	}
}

void CMyRegtWnd::OnUpdateRegRightModify(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(pRegListView->GetListCtrl().GetSelectedCount() == 1);	
}

BOOL CMyRegtWnd::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
	if(!m_WndSplitter.CreateStatic(this, 1, 2))
	{
		return FALSE;
	}

	CCreateContext m_CreateContext; 
	m_CreateContext.m_pCurrentFrame = this;
	m_CreateContext.m_pCurrentDoc = NULL;
	m_CreateContext.m_pNewDocTemplate = NULL;
	m_WndSplitter.CreateView(0, 0, RUNTIME_CLASS(CMyRegtTreeCtrl),
		CSize(0, 0), &m_CreateContext);
	m_WndSplitter.CreateView(0, 1, RUNTIME_CLASS(CMyRegtListCtrl),
		CSize(0, 0), &m_CreateContext);

	pRegTreeView = (CMyRegtTreeCtrl*) m_WndSplitter.GetPane(0,0);
	pRegListView = (CMyRegtListCtrl*) m_WndSplitter.GetPane(0,1);
	return TRUE;
}

void CMyRegtWnd::OnSize(UINT nType, int cx, int cy)
{
	CMyBaseWnd::OnSize(nType, cx, cy);
	if(m_WndSplitter.GetSafeHwnd())
	{
		m_WndSplitter.SetColumnInfo(0, cx * 1/4, 0);
		m_WndSplitter.RecalcLayout();
	}
}

void CMyRegtWnd::OnUpdatePane(CCmdUI* pCmdUI) 
{
	TCHAR m_Text[256] = {0};
	wsprintf(m_Text, _T("共有%d个对象"), pRegListView->GetListCtrl().GetItemCount());
	pCmdUI->SetText(m_Text);
}
