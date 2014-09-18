// EmbeddedView.cpp : implementation file
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
#include "CommandBarsDesigner.h"
#include "EmbeddedView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEmbeddedView

CEmbeddedView::CEmbeddedView()
{
}

CEmbeddedView::~CEmbeddedView()
{
}


BEGIN_MESSAGE_MAP(CEmbeddedView, CWnd)
	//{{AFX_MSG_MAP(CEmbeddedView)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP

END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CEmbeddedView message handlers

BOOL CEmbeddedView::OnEraseBkgnd(CDC* /*pDC*/)
{
	return TRUE;
}

void CEmbeddedView::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	CXTPClientRect rc(this);

	CXTPBufferDC memDC(dc, rc);

	WORD HatchBits[8] = { 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

	CBitmap bm;
	bm.CreateBitmap(8,8,1,1, HatchBits);

	CBrush brush;
	brush.CreatePatternBrush(&bm);

	memDC.FillSolidRect(rc, GetXtremeColor(COLOR_3DFACE));
	memDC.FillRect(rc, &brush);

}
