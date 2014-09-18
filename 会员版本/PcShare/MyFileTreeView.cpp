// MyDirTreeView.cpp : implementation file
//

#include "stdafx.h"
#include "PcShare.h"
#include "MyFileTreeView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyDirTreeView

IMPLEMENT_DYNCREATE(CMyDirTreeView, CTreeView)

CMyDirTreeView::CMyDirTreeView()
{

}

CMyDirTreeView::~CMyDirTreeView()
{
}

BEGIN_MESSAGE_MAP(CMyDirTreeView, CTreeView)
	//{{AFX_MSG_MAP(CMyDirTreeView)
	ON_WM_CREATE()
	ON_NOTIFY_REFLECT(TVN_SELCHANGED, OnSelchanged)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyDirTreeView drawing

void CMyDirTreeView::OnDraw(CDC* pDC)
{
//	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CMyDirTreeView diagnostics

#ifdef _DEBUG
void CMyDirTreeView::AssertValid() const
{
	CTreeView::AssertValid();
}

void CMyDirTreeView::Dump(CDumpContext& dc) const
{
	CTreeView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMyDirTreeView message handlers

int CMyDirTreeView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if(CTreeView::OnCreate(lpCreateStruct) == -1)
	{
		return -1;
	}
	
	m_imagelist.Create(16, 16,ILC_COLOR8|ILC_MASK, 7, 100);
	m_imagelist.Add(AfxGetApp()->LoadIcon(IDI_ICON_DISK_SOFT));
	m_imagelist.Add(AfxGetApp()->LoadIcon(IDI_ICON_DISK_CD));
	m_imagelist.Add(AfxGetApp()->LoadIcon(IDI_ICON_DISK_HARD));
	m_imagelist.Add(AfxGetApp()->LoadIcon(IDI_ICON_DISK_NET));
	m_imagelist.Add(AfxGetApp()->LoadIcon(IDI_ICON_DISK_REMOTE));
	m_imagelist.Add(AfxGetApp()->LoadIcon(IDI_ICON_DIR_NORMAL));
	m_imagelist.Add(AfxGetApp()->LoadIcon(IDI_ICON_DIR_OPEN));
	m_imagelist.Add(AfxGetApp()->LoadIcon(IDI_ICON_COMPUTER));
	GetTreeCtrl().SetImageList(&m_imagelist,TVSIL_NORMAL);
	GetTreeCtrl().ModifyStyle(NULL,TVS_HASLINES|TVS_LINESATROOT|TVS_HASBUTTONS|TVS_SHOWSELALWAYS);

	TV_INSERTSTRUCT tvstruct;
	tvstruct.hParent = TVI_ROOT;
	tvstruct.hInsertAfter = TVI_LAST;
	tvstruct.item.iImage = 7;
	tvstruct.item.iSelectedImage = 7;
	tvstruct.item.pszText = _T("ÎÒµÄµçÄÔ");
	tvstruct.item.mask = TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_TEXT;
	HTREEITEM m_RootItem = GetTreeCtrl().InsertItem(&tvstruct);	
	GetTreeCtrl().SelectItem(m_RootItem);
	GetTreeCtrl().SetItemData(m_RootItem,0);

	TVITEM tvi;
	tvi.mask = TVIF_CHILDREN;
	tvi.hItem = m_RootItem;
	tvi.cChildren = 1;
	GetTreeCtrl().SetItem(&tvi);
	GetTreeCtrl().SelectItem(m_RootItem);
	return 0;
}

void CMyDirTreeView::OnSelchanged(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	GetParentFrame()->PostMessage(WM_TREESELCHANGED, (WPARAM) pNMTreeView->itemNew.hItem, 0);	
	*pResult = 0;
}
