// MyRegTreeCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "PcShare.h"
#include "MyRegtTreeCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyRegtTreeCtrl

IMPLEMENT_DYNCREATE(CMyRegtTreeCtrl, CTreeView)

CMyRegtTreeCtrl::CMyRegtTreeCtrl()
{
	
}

CMyRegtTreeCtrl::~CMyRegtTreeCtrl()
{
	
}


BEGIN_MESSAGE_MAP(CMyRegtTreeCtrl, CTreeView)
	//{{AFX_MSG_MAP(CMyRegtTreeCtrl)
	ON_WM_CREATE()
	ON_NOTIFY_REFLECT(TVN_SELCHANGED, OnSelchanged)
	ON_NOTIFY_REFLECT(NM_RCLICK, OnRclick)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyRegtTreeCtrl drawing

void CMyRegtTreeCtrl::OnDraw(CDC* pDC)
{
//	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CMyRegtTreeCtrl diagnostics

#ifdef _DEBUG
void CMyRegtTreeCtrl::AssertValid() const
{
	CTreeView::AssertValid();
}

void CMyRegtTreeCtrl::Dump(CDumpContext& dc) const
{
	CTreeView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMyRegtTreeCtrl message handlers

int CMyRegtTreeCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CTreeView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	m_TreeImageList.Create(16, 16,ILC_COLOR8|ILC_MASK ,3,3);
	m_TreeImageList.Add(AfxGetApp()->LoadIcon(IDI_ICON_DIR_NORMAL));
	m_TreeImageList.Add(AfxGetApp()->LoadIcon(IDI_ICON_DIR_OPEN));
	m_TreeImageList.Add(AfxGetApp()->LoadIcon(IDI_ICON_COMPUTER));
	GetTreeCtrl().SetImageList(&m_TreeImageList, TVSIL_NORMAL);

	GetTreeCtrl().ModifyStyle(NULL,TVS_HASLINES|TVS_LINESATROOT|TVS_HASBUTTONS|TVS_SHOWSELALWAYS);
	ReStart();
	return 0;
}

void CMyRegtTreeCtrl::ReStart()
{
	GetTreeCtrl().DeleteAllItems();

	HTREEITEM m_Parent = NULL;

	TV_INSERTSTRUCT tvstruct;
	tvstruct.hParent = TVI_ROOT;
	tvstruct.hInsertAfter = TVI_LAST;
	tvstruct.item.iImage = 2;
	tvstruct.item.iSelectedImage = 2;
	tvstruct.item.pszText = _T("ÎÒµÄµçÄÔ");
	tvstruct.item.mask = TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_TEXT;
	HTREEITEM m_RootItem = GetTreeCtrl().InsertItem(&tvstruct);

	tvstruct.hParent = m_RootItem;
	tvstruct.hInsertAfter = TVI_LAST;
	tvstruct.item.iImage = 0;
	tvstruct.item.iSelectedImage = 1;
	tvstruct.item.pszText = _T("HKEY_CLASSES_ROOT");
	tvstruct.item.mask = TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_TEXT;
	m_Parent = GetTreeCtrl().InsertItem(&tvstruct);
	GetTreeCtrl().SetItemData(m_Parent,0);

	TVITEM tvi;
	tvi.mask = TVIF_CHILDREN;
	tvi.hItem = m_Parent;
	tvi.cChildren = 1;
	GetTreeCtrl().SetItem(&tvi);

	tvstruct.hParent = m_RootItem;
	tvstruct.hInsertAfter = TVI_LAST;
	tvstruct.item.iImage = 0;
	tvstruct.item.iSelectedImage = 1;
	tvstruct.item.pszText = _T("HKEY_CURRENT_USER");
	tvstruct.item.mask = TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_TEXT;
	m_Parent = GetTreeCtrl().InsertItem(&tvstruct);
	GetTreeCtrl().SetItemData(m_Parent,0);

	tvi.hItem = m_Parent;
	GetTreeCtrl().SetItem(&tvi);

	tvstruct.hParent = m_RootItem;
	tvstruct.hInsertAfter = TVI_LAST;
	tvstruct.item.iImage = 0;
	tvstruct.item.iSelectedImage = 1;
	tvstruct.item.pszText = _T("HKEY_LOCAL_MACHINE");
	tvstruct.item.mask = TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_TEXT;
	m_Parent = GetTreeCtrl().InsertItem(&tvstruct);
	GetTreeCtrl().SetItemData(m_Parent,0);

	tvi.hItem = m_Parent;
	GetTreeCtrl().SetItem(&tvi);

	tvstruct.hParent = m_RootItem;
	tvstruct.hInsertAfter = TVI_LAST;
	tvstruct.item.iImage = 0;
	tvstruct.item.iSelectedImage = 1;
	tvstruct.item.pszText = _T("HKEY_USERS");
	tvstruct.item.mask = TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_TEXT;
	m_Parent = GetTreeCtrl().InsertItem(&tvstruct);
	GetTreeCtrl().SetItemData(m_Parent,0);

	tvi.hItem = m_Parent;
	GetTreeCtrl().SetItem(&tvi);

	tvstruct.hParent = m_RootItem;
	tvstruct.hInsertAfter = TVI_LAST;
	tvstruct.item.iImage = 0;
	tvstruct.item.iSelectedImage = 1;
	tvstruct.item.pszText = _T("HKEY_CURRENT_CONFIG");
	tvstruct.item.mask = TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_TEXT;
	m_Parent = GetTreeCtrl().InsertItem(&tvstruct);
	GetTreeCtrl().SetItemData(m_Parent,0);

	tvi.hItem = m_Parent;
	GetTreeCtrl().SetItem(&tvi);
	GetTreeCtrl().Expand(m_RootItem,TVE_EXPAND);
}

void CMyRegtTreeCtrl::OnSelchanged(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	GetParentFrame()->PostMessage(WM_REGLEFTDBCLICK, (WPARAM) pNMTreeView->itemNew.hItem, TRUE);	
	*pResult = 0;
}

void CMyRegtTreeCtrl::OnRclick(NMHDR* pNMHDR, LRESULT* pResult) 
{
	*pResult = 0;
	if(GetTreeCtrl().GetSelectedItem() != NULL)
	{
		GetParentFrame()->PostMessage(WM_REGLEFTRCLICK,NULL,NULL);	
	}
}	








