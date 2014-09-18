// PaneOptionsView.cpp : implementation of the CPaneOptionsView class
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
#include "PaneOptions.h"

#include "PaneOptionsDoc.h"
#include "PaneOptionsView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPaneOptionsView

IMPLEMENT_DYNCREATE(CPaneOptionsView, CView)

BEGIN_MESSAGE_MAP(CPaneOptionsView, CView)
	//{{AFX_MSG_MAP(CPaneOptionsView)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPaneOptionsView construction/destruction

CPaneOptionsView::CPaneOptionsView()
{
	// TODO: add construction code here

}

CPaneOptionsView::~CPaneOptionsView()
{
}

BOOL CPaneOptionsView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CPaneOptionsView drawing

void CPaneOptionsView::OnDraw(CDC* pDC)
{
	pDC->FillSolidRect(CXTPClientRect(this), RGB(0, 166, 166));
}

/////////////////////////////////////////////////////////////////////////////
// CPaneOptionsView printing

BOOL CPaneOptionsView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CPaneOptionsView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CPaneOptionsView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CPaneOptionsView diagnostics

#ifdef _DEBUG
void CPaneOptionsView::AssertValid() const
{
	CView::AssertValid();
}

void CPaneOptionsView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CPaneOptionsView message handlers

BOOL CPaneOptionsView::OnEraseBkgnd(CDC* /*pDC*/)
{
	return TRUE;
}

void CPaneOptionsView::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	dc.FillSolidRect(CXTPClientRect(this), RGB(0, 166, 166));

	// Do not call CView::OnPaint() for painting messages
}
