// DockingContainersView.cpp : implementation of the CDockingContainersView class
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
#include "DockingContainers.h"

#include "DockingContainersDoc.h"
#include "DockingContainersView.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDockingContainersView

IMPLEMENT_DYNCREATE(CDockingContainersView, CView)

BEGIN_MESSAGE_MAP(CDockingContainersView, CView)
	//{{AFX_MSG_MAP(CDockingContainersView)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDockingContainersView construction/destruction

CDockingContainersView::CDockingContainersView()
{
	// TODO: add construction code here

}

CDockingContainersView::~CDockingContainersView()
{
}

BOOL CDockingContainersView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CDockingContainersView drawing

void CDockingContainersView::OnDraw(CDC* pDC)
{
	DrawLines(pDC, CXTPClientRect(this));
}
void CDockingContainersView::DrawLines(CDC* pDC, CRect rcView)
{
	CXTPClientRect rc(this);

	if (m_pointArray.GetSize() > 1)
	{
		pDC->MoveTo(m_pointArray[0].x * rcView.Width() / rc.Width(), m_pointArray[0].y * rcView.Height() / rc.Height());
		for (int i = 1; i < m_pointArray.GetSize(); i++)
		{
			pDC->LineTo(m_pointArray[i].x * rcView.Width() / rc.Width(), m_pointArray[i].y * rcView.Height() / rc.Height());
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// CDockingContainersView printing

BOOL CDockingContainersView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CDockingContainersView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CDockingContainersView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CDockingContainersView diagnostics

#ifdef _DEBUG
void CDockingContainersView::AssertValid() const
{
	CView::AssertValid();
}

void CDockingContainersView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CDockingContainersDoc* CDockingContainersView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDockingContainersDoc)));
	return (CDockingContainersDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDockingContainersView message handlers

void CDockingContainersView::OnLButtonDown(UINT nFlags, CPoint point)
{
	SetCapture();
	m_ptLast = point;

	m_pointArray.RemoveAll();
	m_pointArray.Add(m_ptLast);
	Invalidate(TRUE);

	CView::OnLButtonDown(nFlags, point);
}

void CDockingContainersView::OnMouseMove(UINT nFlags, CPoint point)
{
	if (GetCapture() == this)
	{
		CClientDC dc(this);

		dc.MoveTo(m_ptLast);
		dc.LineTo(point);

		m_pointArray.Add(point);

		m_ptLast = point;

		RefreshPanes();
	}

	CView::OnMouseMove(nFlags, point);
}

void CDockingContainersView::OnLButtonUp(UINT nFlags, CPoint point)
{
	ReleaseCapture();

	CView::OnLButtonUp(nFlags, point);
}
