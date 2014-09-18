// GUI_MsMoneyView.cpp : implementation of the CGUI_MsMoneyView class
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
#include "GUI_MsMoney.h"

#include "GUI_MsMoneyDoc.h"
#include "GUI_MsMoneyView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGUI_MsMoneyView

IMPLEMENT_DYNCREATE(CGUI_MsMoneyView, CView)

BEGIN_MESSAGE_MAP(CGUI_MsMoneyView, CView)
	//{{AFX_MSG_MAP(CGUI_MsMoneyView)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGUI_MsMoneyView construction/destruction

CGUI_MsMoneyView::CGUI_MsMoneyView()
{
	// TODO: add construction code here

}

CGUI_MsMoneyView::~CGUI_MsMoneyView()
{
}

BOOL CGUI_MsMoneyView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CGUI_MsMoneyView drawing

void CGUI_MsMoneyView::OnDraw(CDC* pDC)
{
	CGUI_MsMoneyDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// Display the start page bitmap...

	CBitmap bmpStartPage;
	if ( !bmpStartPage.LoadBitmap( IDB_STARTPAGE ) )
	{
		return;
	}

	CDC dcCompatible;
	if ( !dcCompatible.CreateCompatibleDC( pDC ) )
	{
		return;
	}

	CBitmap* pOld = dcCompatible.SelectObject( &bmpStartPage );

	BITMAP bmInfo;
	if ( bmpStartPage.GetObject( sizeof( bmInfo ), &bmInfo ) != 0 )
	{
		pDC->BitBlt( 0,0, bmInfo.bmWidth, bmInfo.bmHeight, &dcCompatible, 0,0, SRCCOPY );
	}

	dcCompatible.SelectObject( pOld );
}

/////////////////////////////////////////////////////////////////////////////
// CGUI_MsMoneyView printing

BOOL CGUI_MsMoneyView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CGUI_MsMoneyView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CGUI_MsMoneyView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CGUI_MsMoneyView diagnostics

#ifdef _DEBUG
void CGUI_MsMoneyView::AssertValid() const
{
	CView::AssertValid();
}

void CGUI_MsMoneyView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CGUI_MsMoneyDoc* CGUI_MsMoneyView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CGUI_MsMoneyDoc)));
	return (CGUI_MsMoneyDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CGUI_MsMoneyView message handlers

void CGUI_MsMoneyView::OnPaint()
{
	CPaintDC dc(this);

	// Get the client rect.
	CRect r;
	GetClientRect(&r);

	// Paint to a memory device context to help
	// eliminate screen flicker.
	CXTPBufferDC memDC(dc, r);
	memDC.FillSolidRect(r, GetSysColor(COLOR_WINDOW));

	OnPrepareDC(&memDC);
	OnDraw(&memDC);
}

BOOL CGUI_MsMoneyView::OnEraseBkgnd(CDC* /*pDC*/)
{
	return TRUE;
}
