// StartPageView.cpp : implementation of the CStartPageView class
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
#include "GUI_VisualStudio7.h"
#include "StartPageView.h"
#include "MainFrm.h"
#include "ChildFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CGUI_VisualStudio7App theApp;

/////////////////////////////////////////////////////////////////////////////
// CStartPageView

IMPLEMENT_DYNCREATE(CStartPageView, CView)

BEGIN_MESSAGE_MAP(CStartPageView, CView)
	//{{AFX_MSG_MAP(CStartPageView)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStartPageView construction/destruction

CStartPageView::CStartPageView()
{
	// TODO: add construction code here
	m_strTitle = _T("Start Page");
}

CStartPageView::~CStartPageView()
{
}

BOOL CStartPageView::PreCreateWindow(CREATESTRUCT& cs)
{
	if (!CView::PreCreateWindow(cs))
		return FALSE;

	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CStartPageView drawing

void CStartPageView::OnDraw(CDC* pDC)
{
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
// CStartPageView printing

BOOL CStartPageView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CStartPageView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CStartPageView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CStartPageView diagnostics

#ifdef _DEBUG
void CStartPageView::AssertValid() const
{
	CView::AssertValid();
}

void CStartPageView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CStartPageView message handlers

BOOL CStartPageView::OnEraseBkgnd(CDC* pDC)
{
	UNREFERENCED_PARAMETER(pDC);
	return FALSE;
}

void CStartPageView::OnPaint()
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

void CStartPageView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	// Set the first time flag to false since we are open.
	theApp.m_bFirstTime = false;
	GetDocument()->SetTitle( m_strTitle );
}

void CStartPageView::OnDestroy()
{
	CView::OnDestroy();

	// Set the first time flag to true so we will open the next time
	// new document is selected.
	theApp.m_bFirstTime = true;
}
