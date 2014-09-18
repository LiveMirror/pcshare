// MyServWnd.cpp : implementation file
//

#include "stdafx.h"
#include "PcShare.h"
#include "MyServWnd.h"
#include "MyServDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyServWnd

IMPLEMENT_DYNCREATE(CMyServWnd, CMyBaseWnd)

CMyServWnd::CMyServWnd()
{
	pServicesView = NULL;
	hIcon = AfxGetApp()->LoadIcon(IDI_ICON_SERV);
}

CMyServWnd::~CMyServWnd()
{
}


BEGIN_MESSAGE_MAP(CMyServWnd, CMyBaseWnd)
	//{{AFX_MSG_MAP(CMyServWnd)
	ON_WM_CREATE()
	ON_COMMAND(ID_SERVICES_ENUM, OnEnumServices)
	ON_COMMAND(ID_SERVICES_START, OnServicesStart)
	ON_COMMAND(ID_SERVICES_STOP, OnServicesStop)
	ON_UPDATE_COMMAND_UI(ID_SERVICES_START, OnUpdateServicesStart)
	ON_UPDATE_COMMAND_UI(ID_SERVICES_STOP, OnUpdateServicesStop)
	ON_COMMAND(ID_SERVICES_ATTRIB, OnServicesAttrib)
	ON_UPDATE_COMMAND_UI(ID_SERVICES_ATTRIB, OnUpdateServicesAttrib)
	ON_COMMAND(ID_SERVICES_DELETE, OnServicesDelete)
	ON_UPDATE_COMMAND_UI(ID_SERVICES_DELETE, OnUpdateServicesDelete)
	ON_COMMAND(ID_EXIT,OnClose)
	ON_UPDATE_COMMAND_UI(IDS_UPDATEPANE, OnUpdatePane)
	ON_WM_CONTEXTMENU()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyServWnd message handlers

int CMyServWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMyBaseWnd::OnCreate(lpCreateStruct) == -1)
	{
		return -1;
	}
	SetActiveView(pServicesView);
	
	CXTPCommandBars* pCommandBars = GetCommandBars();
	pCommandBars->SetMenu(_T("Menu Bar"), IDR_MENU_MANA);

	return 0;
}

BOOL CMyServWnd::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
	CCreateContext pCreateContext; 
	pCreateContext.m_pCurrentFrame = this;
	pCreateContext.m_pCurrentDoc = NULL;
	pCreateContext.m_pNewViewClass = RUNTIME_CLASS(CMyServListView);
	pServicesView = (CMyServListView *) CreateView(&pCreateContext,AFX_IDW_PANE_FIRST);
	return TRUE;
}

void CMyServWnd::OnUpdatePane(CCmdUI* pCmdUI) 
{
	TCHAR m_Text[256] = {0};
	wsprintf(m_Text, _T("共有%d个服务"), pServicesView->GetListCtrl().GetItemCount());
	pCmdUI->SetText(m_Text);
}

void CMyServWnd::OnEnumServices()
{
	CMyWaitTransDlg Transdlg(this);
	Transdlg.m_dTransLen = 0;
	Transdlg.m_Command = CLIENT_ENUM_SERVICES;
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
	BYTE * m_DesBuf = (LPBYTE) Transdlg.m_TransData;
	pServicesView->DeleteAllItems();
	LPMYSERVICES pData = (LPMYSERVICES) m_DesBuf;
	DWORD m_Status = 0;
	for(DWORD i = 0; i < dwFileSize/sizeof(MYSERVICES);i++)
	{
		pServicesView->InsertItem(&pData[i]);
	}
}

void CMyServWnd::OnServicesStart() 
{
	MyControlServices(0);
}

void CMyServWnd::OnServicesStop() 
{
	MyControlServices(1);
}

void CMyServWnd::OnServicesDelete() 
{
	if(MessageBox(_T("真要删除？"), _T("删除服务"), MB_OKCANCEL|MB_ICONQUESTION) != IDOK)
	{
		return;
	}

	int nItem = pServicesView->GetListCtrl().GetNextItem(-1, LVNI_SELECTED);
	if(nItem == -1) 
	{
		return;
	}
	LPMYSERVICES pServices = (LPMYSERVICES) pServicesView->GetListCtrl().GetItemData(nItem);
	CONTROLSERVICES m_Item = {0};
	lstrcpy(m_Item.m_Name, pServices->m_Name);
	
	CMyWaitTransDlg Transdlg(this);
	Transdlg.m_dTransLen = sizeof(CONTROLSERVICES);
	Transdlg.m_Command = CLIENT_DELETE_SERVICE;
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

	delete (LPMYSERVICES) pServicesView->GetListCtrl().GetItemData(nItem);
	pServicesView->GetListCtrl().DeleteItem(nItem);
}

void CMyServWnd::OnUpdateServicesDelete(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(pServicesView->GetListCtrl().GetSelectedCount() == 1);
}

void CMyServWnd::MyControlServices(BYTE flag)
{
	int nItem = pServicesView->GetListCtrl().GetNextItem(-1,LVNI_SELECTED);
	if(nItem == -1)	
	{
		return;
	}
	
	LPMYSERVICES pServices = (LPMYSERVICES) pServicesView->GetListCtrl().GetItemData(nItem);
	CONTROLSERVICES m_Item = {0};
	lstrcpy(m_Item.m_Name, pServices->m_Name);
	m_Item.m_Flag = flag;

	CMyWaitTransDlg Transdlg(this);
	Transdlg.m_Command = CLIENT_CONTROL_SERVICES;
	Transdlg.m_dTransLen = sizeof(CONTROLSERVICES);
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
	switch(flag)
	{
		case 0 : pServicesView->GetListCtrl().SetItemText(nItem, 2, _T("已运行")); break;
		case 1 : pServicesView->GetListCtrl().SetItemText(nItem, 2, _T("已停止")); break;
		case 2 : pServicesView->GetListCtrl().DeleteItem(nItem); break;
		default: break;
	}
}

void CMyServWnd::OnUpdateServicesStart(CCmdUI* pCmdUI) 
{
	BOOL IsEnable = FALSE;
	if(pServicesView->GetListCtrl().GetSelectedCount() == 1)
	{
		int nItem = pServicesView->GetListCtrl().GetNextItem(-1, LVNI_SELECTED);
		if(pServicesView->GetListCtrl().GetItemText(nItem,2) == _T("已停止"))
		{
			IsEnable = TRUE;
		}
	}
	pCmdUI->Enable(IsEnable	);
}

void CMyServWnd::OnUpdateServicesStop(CCmdUI* pCmdUI) 
{
	BOOL IsEnable = FALSE;
	if(pServicesView->GetListCtrl().GetSelectedCount() == 1)
	{
		int nItem = pServicesView->GetListCtrl().GetNextItem(-1,LVNI_SELECTED);
		if(pServicesView->GetListCtrl().GetItemText(nItem,2) == _T("已运行"))
		{
			IsEnable = TRUE;
		}
	}
	pCmdUI->Enable(IsEnable);
}

void CMyServWnd::OnServicesAttrib() 
{
	int nItem = pServicesView->GetListCtrl().GetNextItem(-1,LVNI_SELECTED);
	if(nItem == -1) return;
	LPMYSERVICES pServices = (LPMYSERVICES) pServicesView->GetListCtrl().GetItemData(nItem);

	CMyServDlg dlgServ(pServices);
	if(dlgServ.DoModal() != IDOK || !dlgServ.m_IsModify)
	{
		return;
	}

	CMyWaitTransDlg Transdlg(this);
	LPMYSERVICES pTmpServices = (LPMYSERVICES) Transdlg.m_TransData;
	memcpy(pTmpServices, pServices, sizeof(MYSERVICES));
	pTmpServices->dwStartType = dlgServ.m_Type;
	Transdlg.m_dTransLen = sizeof(MYSERVICES);
	Transdlg.m_Command = CLIENT_CONFIG_SERVICES;
	if(Transdlg.DoModal() == IDCANCEL)
	{
		PostMessage(WM_CLOSE, 0, 0);
		return;
	}
	if(Transdlg.m_Command != 0)
	{
		return;
	}
	pServices->dwStartType = dlgServ.m_Type;
	pServicesView->UpDateItem(nItem, pServices);
}

void CMyServWnd::OnUpdateServicesAttrib(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(pServicesView->GetListCtrl().GetSelectedCount() == 1);
}

BOOL CMyServWnd::StartWork(LPCTSTR sCmdStr)
{
	pServicesView->ReSizeWnd();
	if(!CMyBaseWnd::StartWork(sCmdStr))
	{
		return FALSE;
	}
	OnEnumServices();
	return TRUE;
}

void CMyServWnd::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	if(pWnd->GetSafeHwnd() == pServicesView->GetSafeHwnd())
	{
		CPoint m_point;
 		CMenu m_menu;
 		m_menu.LoadMenu(IDR_MENU_SERVICES);
 		GetCursorPos(&m_point);
 		SetForegroundWindow();   
 		CXTPCommandBars::TrackPopupMenu(m_menu.GetSubMenu(0), TPM_LEFTALIGN|
 			TPM_RIGHTBUTTON,m_point.x,m_point.y,this);
 		m_menu.DestroyMenu();	
	}
}
