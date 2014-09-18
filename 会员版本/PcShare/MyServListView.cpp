// MyServicesView.cpp : implementation file
//

#include "stdafx.h"
#include "PcShare.h"
#include "MyServListView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyServListView

IMPLEMENT_DYNCREATE(CMyServListView, CXTListView)

CMyServListView::CMyServListView()
{
	
}

CMyServListView::~CMyServListView()
{
}


BEGIN_MESSAGE_MAP(CMyServListView, CXTListView)
	//{{AFX_MSG_MAP(CMyServListView)
	ON_WM_DESTROY()
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_NOTIFY_REFLECT(NM_DBLCLK, OnDblclk)
	//}}AFX_MSG_MAP

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyServListView drawing

void CMyServListView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CMyServListView diagnostics

#ifdef _DEBUG
void CMyServListView::AssertValid() const
{
	CXTListView::AssertValid();
}

void CMyServListView::Dump(CDumpContext& dc) const
{
	CXTListView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMyServListView message handlers

int CMyServListView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CXTListView::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_imagelist.Create(16, 16,ILC_COLOR8|ILC_MASK,1,1);
	m_imagelist.Add(AfxGetApp()->LoadIcon(IDI_ICON_SERVICE));
	GetListCtrl().SetImageList(&m_imagelist,LVSIL_SMALL);
	GetListCtrl().SetExtendedStyle(LVS_EX_FULLROWSELECT);
	GetListCtrl().InsertColumn(0, _T("名称"), LVCFMT_LEFT, 140);
	GetListCtrl().InsertColumn(1, _T("描述"), LVCFMT_LEFT, 240);
	GetListCtrl().InsertColumn(2, _T("状态"), LVCFMT_LEFT, 80);
	GetListCtrl().InsertColumn(3, _T("启动类别"), LVCFMT_LEFT, 100);
	GetListCtrl().InsertColumn(4, _T("登录身份"), LVCFMT_LEFT, 100);

	SubclassHeader();
	m_flatHeader.ThawAllColumns();
	m_flatHeader.ShowSortArrow(TRUE);
	EnableUserSortColor(true);

	return 0;
}

BOOL CMyServListView::PreCreateWindow(CREATESTRUCT& cs) 
{
	cs.style ^= LVS_REPORT|LVS_SINGLESEL;
	cs.style &= ~WS_BORDER;
	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	cs.dwExStyle ^= WS_EX_STATICEDGE;
	return CXTListView::PreCreateWindow(cs);
}

bool CMyServListView::SortList(int , bool )
{
	CXTSortClass csc (&GetListCtrl(), m_nSortedCol);
	csc.Sort (m_bAscending, xtSortString);
	return true;
}

void CMyServListView::DeleteAllItems()
{
	for(int i = 0; i < GetListCtrl().GetItemCount(); i++)
	{
		delete (LPMYSERVICES) GetListCtrl().GetItemData(i);
	}
	GetListCtrl().DeleteAllItems();
}

void CMyServListView::InsertItem(LPMYSERVICES pServices)
{
	LPMYSERVICES pItem = new MYSERVICES;
	*pItem = *pServices;
	int nItem = GetListCtrl().InsertItem(GetListCtrl().GetItemCount(), pItem->m_Disp, 0);
	GetListCtrl().SetItemText(nItem, 1, pItem->lpDescribe);
	DWORD m_Status = *((DWORD*) pServices->m_Status);
	switch(m_Status)
	{
		case SERVICE_STOPPED : GetListCtrl().SetItemText(nItem, 2, _T("已停止")); break;
		case SERVICE_RUNNING : GetListCtrl().SetItemText(nItem, 2, _T("已运行")); break;
		case SERVICE_PAUSED : GetListCtrl().SetItemText(nItem, 2, _T("暂停")); break;
		case SERVICE_START_PENDING : GetListCtrl().SetItemText(nItem, 2, _T("正在启动")); break;
		default				 : GetListCtrl().SetItemText(nItem, 2, _T("状态未定")); break;
	}

	if(pItem->dwStartType == SERVICE_AUTO_START)
	{
		GetListCtrl().SetItemText(nItem, 3, _T("自动"));
	}
	else if(pItem->dwStartType == SERVICE_DEMAND_START)
	{
		GetListCtrl().SetItemText(nItem, 3, _T("手动"));
	}
	else
	{
		GetListCtrl().SetItemText(nItem, 3, _T("已禁用"));
	}
	GetListCtrl().SetItemText(nItem, 4, pItem->lpServiceStartName);
	GetListCtrl().SetItemData(nItem, (DWORD) pItem);
}	

void CMyServListView::OnDestroy() 
{
	DeleteAllItems();
	CXTListView::OnDestroy();
}

void CMyServListView::OnDblclk(NMHDR* pNMHDR, LRESULT* pResult) 
{
	*pResult = 0;
	if(GetListCtrl().GetSelectedCount() == 1)
	{
		*pResult = 0;
		GetParentFrame()->PostMessage(WM_COMMAND, (WPARAM) MAKEWPARAM(ID_SERVICES_ATTRIB,1), 0);
	}
}

void CMyServListView::UpDateItem(int nItem,LPMYSERVICES pServices)
{
	GetListCtrl().SetItemText(nItem,1,pServices->lpDescribe);
	DWORD m_Status = *((DWORD*) pServices->m_Status);
	switch(m_Status)
	{
		case SERVICE_STOPPED : GetListCtrl().SetItemText(nItem, 2, _T("已停止")); break;
		case SERVICE_RUNNING : GetListCtrl().SetItemText(nItem, 2, _T("已运行")); break;
		case SERVICE_PAUSED : GetListCtrl().SetItemText(nItem, 2, _T("暂停")); break;
		case SERVICE_START_PENDING : GetListCtrl().SetItemText(nItem, 2, _T("正在启动")); break;
		default				 : GetListCtrl().SetItemText(nItem, 2, _T("状态未定")); break;
	}
	if(pServices->dwStartType == SERVICE_AUTO_START)
	{
		GetListCtrl().SetItemText(nItem, 3, _T("自动"));
	}
	else if(pServices->dwStartType == SERVICE_DEMAND_START)
	{
		GetListCtrl().SetItemText(nItem, 3, _T("手动"));
	}
	else
	{
		GetListCtrl().SetItemText(nItem, 3, _T("已禁用"));
	}
	GetListCtrl().SetItemText(nItem,4,pServices->lpServiceStartName);
}

void CMyServListView::ReStart()
{
	DeleteAllItems();
}

void CMyServListView::ReSizeWnd()
{
	CRect rect;
	GetClientRect(&rect);
	if(rect.Width() > 0)
	{
		GetListCtrl().SetColumnWidth(0, rect.Width() / 4);
		GetListCtrl().SetColumnWidth(1, rect.Width() / 3);
		GetListCtrl().SetColumnWidth(2, rect.Width() / 12);
		GetListCtrl().SetColumnWidth(3, rect.Width() / 10);
		GetListCtrl().SetColumnWidth(4, 
			rect.Width()  - rect.Width() / 4 - rect.Width() / 3 - rect.Width() / 12 - rect.Width() / 10 - 30);
	}
}

void CMyServListView::OnSize(UINT nType, int cx, int cy) 
{
	CXTListView::OnSize(nType, cx, cy);
}