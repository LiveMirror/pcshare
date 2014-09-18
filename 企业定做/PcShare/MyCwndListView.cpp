// MyCwndListView.cpp : implementation file
//

#include "stdafx.h"
#include "PcShare.h"
#include "MyCwndListView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyCwndListView

IMPLEMENT_DYNCREATE(CMyCwndListView, CXTListView)

CMyCwndListView::CMyCwndListView()
{
}

CMyCwndListView::~CMyCwndListView()
{
}


BEGIN_MESSAGE_MAP(CMyCwndListView, CXTListView)
	//{{AFX_MSG_MAP(CMyCwndListView)
	ON_WM_CREATE()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyCwndListView drawing

void CMyCwndListView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CMyCwndListView diagnostics

#ifdef _DEBUG
void CMyCwndListView::AssertValid() const
{
	CXTListView::AssertValid();
}

void CMyCwndListView::Dump(CDumpContext& dc) const
{
	CXTListView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMyCwndListView message handlers

int CMyCwndListView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CXTListView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	m_imagelist.Create(16, 16,ILC_COLOR8|ILC_MASK,1,1);
	m_imagelist.Add(AfxGetApp()->LoadIcon(IDI_ICON_CWND));
	GetListCtrl().SetImageList(&m_imagelist,LVSIL_SMALL);
	GetListCtrl().SetExtendedStyle(LVS_EX_FULLROWSELECT);
	GetListCtrl().InsertColumn(0, _T("窗口句柄"), LVCFMT_LEFT, 80);
	GetListCtrl().InsertColumn(1, _T("窗口标题"), LVCFMT_LEFT, 80);
	GetListCtrl().InsertColumn(2, _T("窗口进程"), LVCFMT_LEFT, 80);
	
	SubclassHeader();
	m_flatHeader.ThawAllColumns();
	m_flatHeader.ShowSortArrow(TRUE);
	EnableUserSortColor(true);
	return 0;
}

bool CMyCwndListView::SortList(int , bool )
{
	CXTSortClass csc (&GetListCtrl(), m_nSortedCol);
	csc.Sort (m_bAscending, xtSortString);
	return true;
}

void CMyCwndListView::OnSize(UINT nType, int cx, int cy) 
{
	CXTListView::OnSize(nType, cx, cy);
}

void CMyCwndListView::ReSizeWnd()
{
	CRect rect;
	GetClientRect(&rect);
	if(rect.Width() > 0)
	{
		GetListCtrl().SetColumnWidth(0, 80);
		GetListCtrl().SetColumnWidth(1, (rect.Width() - 80) / 2);
		GetListCtrl().SetColumnWidth(2, (rect.Width() - 80) / 2 - 30);
	}
}

void CMyCwndListView::InsertMyItem(LPWINDOWITEM pItem)
{
	TCHAR m_TmpBuf[2048] = {0};
	int i = GetListCtrl().GetItemCount();

	wsprintf(m_TmpBuf, _T("%d"), pItem->m_hWnd);
	GetListCtrl().InsertItem(i, m_TmpBuf);

	GetListCtrl().SetItemText(i, 1, pItem->m_WndName);
	GetListCtrl().SetItemText(i, 2, pItem->m_ProcName);
}

BOOL CMyCwndListView::PreCreateWindow(CREATESTRUCT& cs) 
{
	cs.style ^= LVS_REPORT|LVS_SINGLESEL;
	cs.style &= ~WS_BORDER;
	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	cs.dwExStyle ^= WS_EX_STATICEDGE;
	return CXTListView::PreCreateWindow(cs);
}
