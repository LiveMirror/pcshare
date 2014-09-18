// MultiLanguageView.cpp : implementation of the CMultiLanguageView class
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
#include "MultiLanguage.h"

#include "MultiLanguageDoc.h"
#include "MultiLanguageView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMultiLanguageView

IMPLEMENT_DYNCREATE(CMultiLanguageView, CView)

BEGIN_MESSAGE_MAP(CMultiLanguageView, CView)
	//{{AFX_MSG_MAP(CMultiLanguageView)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMultiLanguageView construction/destruction

CMultiLanguageView::CMultiLanguageView()
{
	// TODO: add construction code here

}

CMultiLanguageView::~CMultiLanguageView()
{
}

BOOL CMultiLanguageView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CMultiLanguageView drawing

void CMultiLanguageView::OnDraw(CDC* pDC)
{
	CXTPFontDC fontDC(pDC, XTPPaintManager()->GetRegularBoldFont());
	pDC->TextOut(5,5, _T("Click the \"Language\" pull-down menu to see the translated text"));
	pDC->TextOut(5,20, _T("Also see \"Scribble\" multi-language project for localization sample."));
}

/////////////////////////////////////////////////////////////////////////////
// CMultiLanguageView printing

BOOL CMultiLanguageView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CMultiLanguageView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CMultiLanguageView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CMultiLanguageView diagnostics

#ifdef _DEBUG
void CMultiLanguageView::AssertValid() const
{
	CView::AssertValid();
}

void CMultiLanguageView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMultiLanguageDoc* CMultiLanguageView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMultiLanguageDoc)));
	return (CMultiLanguageDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMultiLanguageView message handlers
