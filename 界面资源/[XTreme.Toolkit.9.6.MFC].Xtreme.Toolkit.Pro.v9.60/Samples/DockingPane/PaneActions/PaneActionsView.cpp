// PaneActionsView.cpp : implementation of the CPaneActionsView class
//
// This file is a part of the XTREME TOOLKIT PRO MFC class library.
// ©1998-2005 Codejock Software, All Rights Reserved.
//
// THIS SOURCE FILE IS THE PROPERTY OF CODEJOCK SOFTWARE AND IS NOT TO BE
// RE-DISTRIBUTED BY ANY MEANS WHATSOEVER WITHOUT THE EXPRESSED WRITTEN
// CONSENT OF CODEJOCK SOFTWARE.
//
// THIS SOURCE CODE CAN ONLY BE USED UNDER THE TERMS AND CONDITIONS OUTLINED
// IN THE XTREME TOOLKIT PRO LICENSE AGREEMENT. CODEJOCK SOFTWARE GRANTS TO
// YOU (ONE SOFTWARE DEVELOPER) THE LIMITED RIGHT TO USE THIS SOFTWARE ON A
// SINGLE COMPUTER.
//
// CONTACT INFORMATION:
// support@codejock.com
// http://www.codejock.com
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PaneActions.h"

#include "PaneActionsDoc.h"
#include "PaneActionsView.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPaneActionsView

IMPLEMENT_DYNCREATE(CPaneActionsView, CListView)

BEGIN_MESSAGE_MAP(CPaneActionsView, CListView)
	//{{AFX_MSG_MAP(CPaneActionsView)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPaneActionsView construction/destruction

CPaneActionsView::CPaneActionsView()
{
	// TODO: add construction code here

}

CPaneActionsView::~CPaneActionsView()
{
}

BOOL CPaneActionsView::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.style |= WS_CLIPCHILDREN|WS_CLIPSIBLINGS;

	return CListView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CPaneActionsView drawing

void CPaneActionsView::OnDraw(CDC* /*pDC*/)
{
}

/////////////////////////////////////////////////////////////////////////////
// CPaneActionsView diagnostics

#ifdef _DEBUG
void CPaneActionsView::AssertValid() const
{
	CListView::AssertValid();
}

void CPaneActionsView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}

CPaneActionsDoc* CPaneActionsView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CPaneActionsDoc)));
	return (CPaneActionsDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CPaneActionsView message handlers

void CPaneActionsView::OnInitialUpdate()
{
	CListView::OnInitialUpdate();

	CListCtrl& wndList = GetListCtrl();

	wndList.DeleteAllItems();

}

int CPaneActionsView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CListView::OnCreate(lpCreateStruct) == -1)
		return -1;

	CListCtrl& wndList = GetListCtrl();
	wndList.ModifyStyle(0, LVS_REPORT);

	wndList.InsertColumn(0, _T("Action"), LVCFMT_LEFT, 300);
	wndList.InsertColumn(1, _T("Pane"), LVCFMT_LEFT, 100);
	wndList.InsertColumn(2, _T("Cancelable"), LVCFMT_CENTER, 100);
	wndList.InsertColumn(3, _T("Canceled"), LVCFMT_CENTER, 100);

	return 0;
}


void CPaneActionsView::AddAction(XTP_DOCKINGPANE_ACTION* pAction)
{
	CListCtrl& wndList = GetListCtrl();

	CString strAction = pAction->action < _countof(lpszActions)? lpszActions[pAction->action]: _T("Unknown");

	int nIndex = wndList.InsertItem(wndList.GetItemCount(), strAction, -1);

	wndList.SetItem(nIndex, 1, LVIF_TEXT, pAction->pPane->GetTitle(), 0, 0, 0, 0);

	BOOL bCancelable = pAction->action % 2 == 0;
	wndList.SetItem(nIndex, 2, LVIF_TEXT, bCancelable? _T("true"): _T("false"), 0, 0, 0, 0);

	if (bCancelable)
	wndList.SetItem(nIndex, 3, LVIF_TEXT, pAction->bCancel? _T("true"): _T("false"), 0, 0, 0, 0);
}
