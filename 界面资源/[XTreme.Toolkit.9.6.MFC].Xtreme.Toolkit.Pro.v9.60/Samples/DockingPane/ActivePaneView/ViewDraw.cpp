// ViewDraw.cpp : implementation file
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
#include "ActivePaneView.h"
#include "ViewDraw.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CViewDraw

IMPLEMENT_DYNCREATE(CViewDraw, CView)

CViewDraw::CViewDraw()
{
}

CViewDraw::~CViewDraw()
{
}


BEGIN_MESSAGE_MAP(CViewDraw, CView)
	//{{AFX_MSG_MAP(CViewDraw)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_NCPAINT()
	ON_WM_NCCALCSIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CViewDraw drawing

void CViewDraw::OnDraw(CDC* pDC)
{
	// draw view text.
	CXTPFontDC dc(pDC, &theApp.m_font);

	CRect rect;
	pDC->GetClipBox(&rect);
	pDC->DrawText(m_strTitle, &rect, DT_SINGLELINE|DT_CENTER|DT_VCENTER);
}

/////////////////////////////////////////////////////////////////////////////
// CViewDraw diagnostics

#ifdef _DEBUG
void CViewDraw::AssertValid() const
{
	CView::AssertValid();
}

void CViewDraw::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CViewDraw message handlers

BOOL CViewDraw::OnEraseBkgnd(CDC* pDC)
{
	UNUSED_ALWAYS(pDC);
	return FALSE;
}

void CViewDraw::OnPaint()
{
	CPaintDC dc(this);

	// Get the client rect.
	CXTPClientRect rect(this);

	// Paint to a memory device context to reduce screen flicker.
	CXTPBufferDC memDC(dc.m_hDC, rect);
	memDC.FillSolidRect(&rect, ::GetSysColor(COLOR_WINDOW));

	OnPrepareDC(&memDC);
	OnDraw(&memDC);
}

void CViewDraw::OnNcPaint()
{
	CWindowDC dc(this);

	CXTPWindowRect rect(this);

	int cx = rect.Width();
	int cy = rect.Height();

	const COLORREF clrFrame =  ::GetSysColor(COLOR_3DSHADOW);
	dc.Draw3dRect(0, 0, cx, cy, clrFrame, clrFrame);
}

void CViewDraw::OnNcCalcSize(BOOL /*bCalcValidRects*/, NCCALCSIZE_PARAMS FAR* lpncsp)
{
	// adjust non-client area for border space
	lpncsp->rgrc[0].left   += 1;
	lpncsp->rgrc[0].top    += 1;
	lpncsp->rgrc[0].right  -= 1;
	lpncsp->rgrc[0].bottom -= 1;
}
