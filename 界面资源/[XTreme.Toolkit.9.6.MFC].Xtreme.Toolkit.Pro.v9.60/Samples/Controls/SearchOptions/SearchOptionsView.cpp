// SearchOptionsView.cpp : implementation of the CSearchOptionsView class
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
#include "SearchOptions.h"

#include "SearchOptionsDoc.h"
#include "SearchOptionsView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSearchOptionsView

IMPLEMENT_DYNCREATE(CSearchOptionsView, CListView)

BEGIN_MESSAGE_MAP(CSearchOptionsView, CListView)
	//{{AFX_MSG_MAP(CSearchOptionsView)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CListView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CListView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CListView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSearchOptionsView construction/destruction

CSearchOptionsView::CSearchOptionsView()
: m_bFirstTime( true )
{
	// TODO: add construction code here

}

CSearchOptionsView::~CSearchOptionsView()
{
}

BOOL CSearchOptionsView::PreCreateWindow(CREATESTRUCT& cs)
{
	if (!CListView::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_STATICEDGE;
	cs.style |= WS_CLIPCHILDREN | WS_CLIPSIBLINGS | LVS_REPORT | LVS_NOSORTHEADER;

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CSearchOptionsView drawing

void CSearchOptionsView::OnDraw(CDC* /*pDC*/)
{
}

void CSearchOptionsView::OnInitialUpdate()
{
	CListView::OnInitialUpdate();

	if ( m_bFirstTime )
	{
		CListCtrl& refCtrl = GetListCtrl();

		refCtrl.InsertColumn(0, _T("Column Text"), LVCFMT_LEFT, 250);
		refCtrl.InsertItem(0, _T("Item Text"));

		m_bFirstTime = false;
	}

	// TODO: You may populate your ListView with items by directly accessing
	//  its list control through a call to GetListCtrl().
}

/////////////////////////////////////////////////////////////////////////////
// CSearchOptionsView printing

BOOL CSearchOptionsView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CSearchOptionsView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CSearchOptionsView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CSearchOptionsView diagnostics

#ifdef _DEBUG
void CSearchOptionsView::AssertValid() const
{
	CListView::AssertValid();
}

void CSearchOptionsView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}

CSearchOptionsDoc* CSearchOptionsView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSearchOptionsDoc)));
	return (CSearchOptionsDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CSearchOptionsView message handlers
void CSearchOptionsView::OnStyleChanged(int /*nStyleType*/, LPSTYLESTRUCT /*lpStyleStruct*/)
{
	//TODO: add code to react to the user changing the view style of your window
}
