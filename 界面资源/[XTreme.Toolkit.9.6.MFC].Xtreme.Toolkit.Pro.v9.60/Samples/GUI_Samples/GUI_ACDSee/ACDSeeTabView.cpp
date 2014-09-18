// ACDSeeTabView.cpp : implementation file
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
#include "GUI_ACDSee.h"
#include "ACDSeeTabView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CACDSeeTabView

IMPLEMENT_DYNCREATE(CACDSeeTabView, CView)

CACDSeeTabView::CACDSeeTabView()
{
}

CACDSeeTabView::~CACDSeeTabView()
{
}


BEGIN_MESSAGE_MAP(CACDSeeTabView, CView)
	//{{AFX_MSG_MAP(CACDSeeTabView)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CACDSeeTabView drawing

void CACDSeeTabView::OnDraw(CDC* /*pDC*/)
{
}

/////////////////////////////////////////////////////////////////////////////
// CACDSeeTabView diagnostics

#ifdef _DEBUG
void CACDSeeTabView::AssertValid() const
{
	CView::AssertValid();
}

void CACDSeeTabView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CACDSeeTabView message handlers

void CACDSeeTabView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	if (m_wndTabCtrl.GetSafeHwnd() != 0)
		return;

	m_wndTabCtrl.Create(TCS_OWNERDRAWFIXED|TCS_MULTILINE|WS_CHILD|WS_VISIBLE, CXTPClientRect(this), this, 0);

	m_wndImagList.Create(16, 16, ILC_COLOR32|ILC_MASK , 0, 1);
	CBitmap bmp;
	VERIFY(bmp.LoadBitmap(IDB_TABICONS));
	m_wndImagList.Add(&bmp, RGB(192, 192, 192));

	m_wndTabCtrl.SetImageList(&m_wndImagList);

	m_wndTabCtrl.InsertItem(0, _T("Folders"), 2);
	m_wndTabCtrl.InsertItem(1, _T("Albums"), 6);
	m_wndTabCtrl.InsertItem(2, _T("Favorities"), 5);

	m_wndTabCtrl.SetFont(XTPPaintManager()->GetRegularFont());
}

void CACDSeeTabView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	if (m_wndTabCtrl.GetSafeHwnd())
	{
		m_wndTabCtrl.MoveWindow(0, 0, cx, cy);
	}

}

BOOL CACDSeeTabView::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.style |= WS_CLIPCHILDREN|WS_CLIPSIBLINGS;

	return CView::PreCreateWindow(cs);
}
