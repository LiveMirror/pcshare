// ActivePaneViewView.cpp : implementation of the CActivePaneViewView class
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

#include "ActivePaneViewDoc.h"
#include "ActivePaneViewView.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CActivePaneViewView

IMPLEMENT_DYNCREATE(CActivePaneViewView, CViewDraw)

BEGIN_MESSAGE_MAP(CActivePaneViewView, CViewDraw)
	//{{AFX_MSG_MAP(CActivePaneViewView)
	ON_WM_NCCALCSIZE()
	ON_WM_NCPAINT()
	ON_COMMAND(ID_VIEW_THREE, OnViewThree)
	ON_UPDATE_COMMAND_UI(ID_VIEW_THREE, OnUpdateViewThree)
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CViewDraw::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CViewDraw::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CViewDraw::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CActivePaneViewView construction/destruction

CActivePaneViewView::CActivePaneViewView()
: m_bEnable(FALSE)
{
	// TODO: add construction code here
	m_strTitle = _T("Three...");
}

CActivePaneViewView::~CActivePaneViewView()
{
}

BOOL CActivePaneViewView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CViewDraw::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CActivePaneViewView drawing

void CActivePaneViewView::OnDraw(CDC* pDC)
{
	CViewDraw::OnDraw(pDC);
}

/////////////////////////////////////////////////////////////////////////////
// CActivePaneViewView printing

BOOL CActivePaneViewView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CActivePaneViewView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CActivePaneViewView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CActivePaneViewView diagnostics

#ifdef _DEBUG
void CActivePaneViewView::AssertValid() const
{
	CViewDraw::AssertValid();
}

void CActivePaneViewView::Dump(CDumpContext& dc) const
{
	CViewDraw::Dump(dc);
}

CActivePaneViewDoc* CActivePaneViewView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CActivePaneViewDoc)));
	return (CActivePaneViewDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CActivePaneViewView message handlers

void CActivePaneViewView::OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp)
{
	// bypass base class.
	CView::OnNcCalcSize(bCalcValidRects, lpncsp);
}

void CActivePaneViewView::OnNcPaint()
{
	// bypass base class.
	CView::OnNcPaint();
}

void CActivePaneViewView::OnViewThree()
{
	// TODO: Add your command handler code here
	AfxMessageBox(_T("SDI View - No. 3"));
}

void CActivePaneViewView::OnUpdateViewThree(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_bEnable);
}

void CActivePaneViewView::OnSetFocus(CWnd* pOldWnd)
{
	CViewDraw::OnSetFocus(pOldWnd);

	// TODO: Add your message handler code here
	m_bEnable = TRUE;
}

void CActivePaneViewView::OnKillFocus(CWnd* pNewWnd)
{
	CViewDraw::OnKillFocus(pNewWnd);

	// TODO: Add your message handler code here
	m_bEnable = FALSE;
}
