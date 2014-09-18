// MyProcessListView.cpp : implementation file
//

#include "stdafx.h"
#include "PcShare.h"
#include "MyProcListView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyProcListView

IMPLEMENT_DYNCREATE(CMyProcListView, CXTListView)

CMyProcListView::CMyProcListView()
{
}

CMyProcListView::~CMyProcListView()
{
	
}


BEGIN_MESSAGE_MAP(CMyProcListView, CXTListView)
	//{{AFX_MSG_MAP(CMyProcListView)
	ON_WM_CREATE()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyProcListView drawing

void CMyProcListView::OnDraw(CDC* pDC)
{
//	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CMyProcListView diagnostics

#ifdef _DEBUG
void CMyProcListView::AssertValid() const
{
	CXTListView::AssertValid();
}

void CMyProcListView::Dump(CDumpContext& dc) const
{
	CXTListView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMyProcListView message handlers

int CMyProcListView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CXTListView::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_imagelist.Create(16, 16,ILC_COLOR8|ILC_MASK,1,1);
	m_imagelist.Add(AfxGetApp()->LoadIcon(IDI_ICON_PROC));
	GetListCtrl().SetImageList(&m_imagelist,LVSIL_SMALL);

	GetListCtrl().SetExtendedStyle(LVS_EX_FULLROWSELECT);
	GetListCtrl().InsertColumn(0, _T("进程ID"), LVCFMT_LEFT, 80);
	GetListCtrl().InsertColumn(1, _T("线程数量"), LVCFMT_LEFT, 80);
	GetListCtrl().InsertColumn(2, _T("进程级别"), LVCFMT_LEFT, 80);
	GetListCtrl().InsertColumn(3, _T("执行文件"), LVCFMT_LEFT, 80);

	SubclassHeader();
	m_flatHeader.ThawAllColumns();
	m_flatHeader.ShowSortArrow(TRUE);
	EnableUserSortColor(true);
	return 0;
}

bool CMyProcListView::SortList(int , bool )
{
	CXTSortClass csc (&GetListCtrl(), m_nSortedCol);
	csc.Sort (m_bAscending, xtSortString);
	return true;
}

void CMyProcListView::InsertMyItem(LPPROCESSLIST pItem)
{
	TCHAR m_TmpBuf[2048] = {0};
	int i = GetListCtrl().GetItemCount();

	wsprintf(m_TmpBuf, _T("%d"), pItem->th32ProcessID);
	GetListCtrl().InsertItem(i, m_TmpBuf);

	wsprintf(m_TmpBuf, _T("%d"), pItem->cntThreads);
	GetListCtrl().SetItemText(i, 1, m_TmpBuf);

	wsprintf(m_TmpBuf, _T("%d"), pItem->pcPriClassBase);
	GetListCtrl().SetItemText(i, 2, m_TmpBuf);

	GetListCtrl().SetItemText(i, 3, pItem->szExeFile);
}

BOOL CMyProcListView::PreCreateWindow(CREATESTRUCT& cs) 
{
	cs.style ^= LVS_REPORT|LVS_SINGLESEL;
	cs.style &= ~WS_BORDER;
	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	cs.dwExStyle ^= WS_EX_STATICEDGE;
	return CXTListView::PreCreateWindow(cs);
}

void CMyProcListView::OnSize(UINT nType, int cx, int cy) 
{
	CXTListView::OnSize(nType, cx, cy);
}

void CMyProcListView::ReSizeWnd()
{
	CRect rect;
	GetClientRect(&rect);
	if(rect.Width() > 0)
	{
		GetListCtrl().SetColumnWidth(0, 80);
		GetListCtrl().SetColumnWidth(1, 60);
		GetListCtrl().SetColumnWidth(2, 60);
		GetListCtrl().SetColumnWidth(3, rect.Width() - 200 - 30);
	}
}

