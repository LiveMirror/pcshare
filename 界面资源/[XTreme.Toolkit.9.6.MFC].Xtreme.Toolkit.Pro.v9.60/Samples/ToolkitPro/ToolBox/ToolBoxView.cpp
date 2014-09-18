// ToolBoxView.cpp : implementation of the CToolBoxView class
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
#include "ToolBox.h"

#include "ToolBoxDoc.h"
#include "ToolBoxView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CToolBoxView

IMPLEMENT_DYNCREATE(CToolBoxView, CView)

BEGIN_MESSAGE_MAP(CToolBoxView, CView)
	//{{AFX_MSG_MAP(CToolBoxView)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CToolBoxView construction/destruction

CToolBoxView::CToolBoxView()
{
	// TODO: add construction code here

}

CToolBoxView::~CToolBoxView()
{
}

BOOL CToolBoxView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CToolBoxView drawing

void CToolBoxView::OnDraw(CDC*)
{
}

/////////////////////////////////////////////////////////////////////////////
// CToolBoxView diagnostics

#ifdef _DEBUG
void CToolBoxView::AssertValid() const
{
	CView::AssertValid();
}

void CToolBoxView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CToolBoxDoc* CToolBoxView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CToolBoxDoc)));
	return (CToolBoxDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CToolBoxView message handlers

int CToolBoxView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;


	m_wndPropertySheet.Create(WS_VISIBLE|WS_CHILD|SS_NOTIFY|WS_CLIPCHILDREN|WS_CLIPSIBLINGS,
		CRect(0, 0, 0, 0), this, 0);

	m_wndPropertySheet.GetPaintManager()->SetAppearance(xtpTabAppearanceExcel);

	m_wndPageLookAndBehaviour.Create(CPageLookAndBehaviour::IDD, &m_wndPropertySheet);
	m_wndPageDragAndDrop.Create(CPageDragAndDrop::IDD, &m_wndPropertySheet);

	m_wndPropertySheet.InsertItem(0, _T("Look And Behaviour"), m_wndPageLookAndBehaviour);
	m_wndPropertySheet.InsertItem(1, _T("Drag And Drop"), m_wndPageDragAndDrop);


	return 0;
}

void CToolBoxView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	if (m_wndPropertySheet.GetSafeHwnd())
	{
		m_wndPropertySheet.MoveWindow(0, 0, cx, cy);
	}

}

BOOL CToolBoxView::OnEraseBkgnd(CDC* )
{
	return TRUE;
}
