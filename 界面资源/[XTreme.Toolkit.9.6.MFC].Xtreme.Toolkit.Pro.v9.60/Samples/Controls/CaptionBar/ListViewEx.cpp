// ListViewEx.cpp : implementation of the CListViewEx class
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
#include "CaptionBar.h"

#include "ListViewExDoc.h"
#include "ListViewEx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CListViewEx

IMPLEMENT_DYNCREATE(CListViewEx, CListView)

BEGIN_MESSAGE_MAP(CListViewEx, CListView)
	//{{AFX_MSG_MAP(CListViewEx)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CListView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CListView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CListView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CListViewEx construction/destruction

CListViewEx::CListViewEx()
{
	// TODO: add construction code here
	m_ImageSmall.Create(16, 16, ILC_MASK|ILC_COLOR24, 2, 1);
	m_ImageLarge.Create(32, 32, ILC_MASK|ILC_COLOR24, 2, 1);

	HICON hIcon = AfxGetApp()->LoadIcon(IDR_CAPTIOTYPE);
	m_ImageSmall.Add(hIcon);
	m_ImageLarge.Add(hIcon);
}

CListViewEx::~CListViewEx()
{
}

BOOL CListViewEx::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CListView::PreCreateWindow( cs ))
		return FALSE;

	// Set the style for the tree control.
	cs.style |= LVS_REPORT | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;

	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CListViewEx drawing

void CListViewEx::OnDraw(CDC* )
{
}

void CListViewEx::OnInitialUpdate()
{
	CListView::OnInitialUpdate();


	// TODO: You may populate your ListView with items by directly accessing
	//  its list control through a call to GetListCtrl().


	// Add the parent item
	GetListCtrl().InsertColumn(0, _T("Column Text"), LVCFMT_LEFT, 200);

	GetListCtrl().SetImageList(&m_ImageSmall, LVSIL_SMALL);
	GetListCtrl().SetImageList(&m_ImageLarge, LVSIL_NORMAL);

	// Add children
	int i;
	for(i = 1; i < 5; i++ ) {
		GetListCtrl().InsertItem (0, _T("Child of Parent Item"));
	}

	HWND hWndHeader = GetListCtrl().GetDlgItem(0)->GetSafeHwnd();
	m_flatHeader.SubclassWindow (hWndHeader);
}

/////////////////////////////////////////////////////////////////////////////
// CListViewEx printing

BOOL CListViewEx::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CListViewEx::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CListViewEx::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CListViewEx diagnostics

#ifdef _DEBUG
void CListViewEx::AssertValid() const
{
	CListView::AssertValid();
}

void CListViewEx::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}

CListViewExDoc* CListViewEx::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CListViewExDoc)));
	return (CListViewExDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CListViewEx message handlers
void CListViewEx::OnStyleChanged(int /*nStyleType*/, LPSTYLESTRUCT /*lpStyleStruct*/)
{
	//TODO: add code to react to the user changing the view style of your window
}
