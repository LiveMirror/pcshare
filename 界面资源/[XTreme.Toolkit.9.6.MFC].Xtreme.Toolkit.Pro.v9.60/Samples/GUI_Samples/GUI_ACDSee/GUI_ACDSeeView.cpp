// GUI_ACDSeeView.cpp : implementation of the CGUI_ACDSeeView class
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

#include "GUI_ACDSeeDoc.h"
#include "GUI_ACDSeeView.h"

#include "ACDSeeTheme.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGUI_ACDSeeView

IMPLEMENT_DYNCREATE(CGUI_ACDSeeView, CView)

BEGIN_MESSAGE_MAP(CGUI_ACDSeeView, CView)
	//{{AFX_MSG_MAP(CGUI_ACDSeeView)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)

	ON_COMMAND(ID_EDIT_CUT, OnEmptyCommand)
	ON_COMMAND(ID_EDIT_PASTE, OnEmptyCommand)
	ON_COMMAND(ID_EDIT_COPY, OnEmptyCommand)

	ON_COMMAND_RANGE(ID_BUTTON32771, ID_BUTTON32788, OnEmptyCommandRange)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGUI_ACDSeeView construction/destruction

CGUI_ACDSeeView::CGUI_ACDSeeView()
{
//  LoadBitmap(MAKEINTRESOURCE(IDB_BACKGROUND));

}

CGUI_ACDSeeView::~CGUI_ACDSeeView()
{
}

BOOL CGUI_ACDSeeView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}



/////////////////////////////////////////////////////////////////////////////
// CGUI_ACDSeeView drawing

void CGUI_ACDSeeView::OnDraw(CDC* pDC)
{
	CGUI_ACDSeeDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here

	CXTPCompatibleDC dc(pDC, CBitmap::FromHandle(CACDSeeTheme::m_hbmImageBackground));

	CRect r;
	GetClientRect(&r);

	for (int x = 0; x  < r.Width(); x += 128)
	for (int y = 0; y  < r.Height(); y += 128)
		pDC->BitBlt(x, y, 128, 128, &dc, 0, 0, SRCCOPY);
}

/////////////////////////////////////////////////////////////////////////////
// CGUI_ACDSeeView printing

BOOL CGUI_ACDSeeView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CGUI_ACDSeeView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CGUI_ACDSeeView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CGUI_ACDSeeView diagnostics

#ifdef _DEBUG
void CGUI_ACDSeeView::AssertValid() const
{
	CView::AssertValid();
}

void CGUI_ACDSeeView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CGUI_ACDSeeDoc* CGUI_ACDSeeView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CGUI_ACDSeeDoc)));
	return (CGUI_ACDSeeDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CGUI_ACDSeeView message handlers

void CGUI_ACDSeeView::OnPaint()
{

	CPaintDC dc(this);
	// Get the client rect.
	CRect r;
	GetClientRect(&r);

	// Paint to a memory device context to help
	// eliminate screen flicker.
	CXTPBufferDC memDC(dc, r);

	OnPrepareDC(&memDC);
	OnDraw(&memDC);
}

BOOL CGUI_ACDSeeView::OnEraseBkgnd(CDC* pDC)
{
	UNREFERENCED_PARAMETER(pDC);
	return FALSE;
}


void CGUI_ACDSeeView::OnEmptyCommand()
{

}

void CGUI_ACDSeeView::OnEmptyCommandRange(UINT)
{

}
