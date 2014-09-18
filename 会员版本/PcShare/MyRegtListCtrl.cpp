// MyRegListCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "PcShare.h"
#include "MyRegtListCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyRegtListCtrl

IMPLEMENT_DYNCREATE(CMyRegtListCtrl, CListView)

CMyRegtListCtrl::CMyRegtListCtrl()
{
	
}

CMyRegtListCtrl::~CMyRegtListCtrl()
{
	
}


BEGIN_MESSAGE_MAP(CMyRegtListCtrl, CListView)
	//{{AFX_MSG_MAP(CMyRegtListCtrl)
	ON_WM_CREATE()
	ON_NOTIFY_REFLECT(NM_DBLCLK, OnDblclk)
	ON_NOTIFY_REFLECT(NM_RCLICK, OnRclick)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyRegtListCtrl drawing

void CMyRegtListCtrl::OnDraw(CDC* pDC)
{
//	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CMyRegtListCtrl diagnostics

#ifdef _DEBUG
void CMyRegtListCtrl::AssertValid() const
{
	CListView::AssertValid();
}

void CMyRegtListCtrl::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMyRegtListCtrl message handlers

int CMyRegtListCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if(CListView::OnCreate(lpCreateStruct) == -1)
	{
		return -1;
	}
	m_ListImageList.Create(16, 16,ILC_COLOR8|ILC_MASK,2,2);
	m_ListImageList.Add(AfxGetApp()->LoadIcon(IDI_ICON_REG_BIN));
	m_ListImageList.Add(AfxGetApp()->LoadIcon(IDI_ICON_REG_SZ));
	GetListCtrl().SetImageList(&m_ListImageList,LVSIL_SMALL);
	GetListCtrl().InsertColumn(0, _T("名称"), LVCFMT_LEFT, 120);
	GetListCtrl().InsertColumn(1, _T("类型"), LVCFMT_LEFT, 120);
	GetListCtrl().InsertColumn(2, _T("数值"), LVCFMT_LEFT, 240);
	GetListCtrl().GetHeaderCtrl()->ModifyStyle(HDS_BUTTONS ,HDS_HOTTRACK );
	return 0;
}

void CMyRegtListCtrl::OnDblclk(NMHDR* pNMHDR, LRESULT* pResult) 
{
	*pResult = 0;
	if(GetListCtrl().GetSelectedCount() > 0)
	{
		GetParentFrame()->PostMessage(WM_COMMAND,	(WPARAM) MAKEWPARAM(ID_REG_RIGHT_MODIFY, 1), NULL);
	}
}

void CMyRegtListCtrl::OnRclick(NMHDR* pNMHDR, LRESULT* pResult) 
{
	*pResult = 0;
	GetParentFrame()->PostMessage(WM_REGRIGHTRBCLICK, NULL,NULL);
}

BOOL CMyRegtListCtrl::PreCreateWindow(CREATESTRUCT& cs) 
{
	cs.style ^= LVS_REPORT|LVS_EDITLABELS|LVS_SINGLESEL;
	cs.style &= ~WS_BORDER;
	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	return CListView::PreCreateWindow(cs);
}

