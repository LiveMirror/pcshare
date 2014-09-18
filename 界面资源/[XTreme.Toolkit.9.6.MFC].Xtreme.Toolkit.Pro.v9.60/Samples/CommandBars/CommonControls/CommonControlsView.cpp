// CommonControlsView.cpp : implementation of the CCommonControlsView class
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
#include "CommonControls.h"

#include "CommonControlsDoc.h"
#include "CommonControlsView.h"
#include "MainFrm.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCommonControlsView

IMPLEMENT_DYNCREATE(CCommonControlsView, CView)

BEGIN_MESSAGE_MAP(CCommonControlsView, CView)
	//{{AFX_MSG_MAP(CCommonControlsView)
	ON_WM_HSCROLL()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)

	ON_UPDATE_COMMAND_UI(IDC_SLIDER, OnUpdateSlider)

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCommonControlsView construction/destruction

CCommonControlsView::CCommonControlsView()
{
	m_nGray = 0xFF; // Initialize the value used to create the background color.

}

CCommonControlsView::~CCommonControlsView()
{
}

BOOL CCommonControlsView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CCommonControlsView drawing

void CCommonControlsView::OnDraw(CDC* pDC)
{
	// This member function is called by the framework
	// to render an image of the document. The framework
	// calls this function to perform screen display, printing,
	// and print preview, and it passes a different
	// device context in each case. There is no default
	// implementation.

	// Get a pointer to the Document object.
	CCommonControlsDoc* pDoc = GetDocument();

	// Make certain that the Document pointer is valid.
	ASSERT_VALID(pDoc);

	// Draw the background of the window.
	// CXTPClientRect(this) returns the client area of the window.
	// RGB(m_nGray, m_nGray, m_nGray) creates a gray scale color for the window's background.
	pDC->FillSolidRect(CXTPClientRect(this), RGB(m_nGray, m_nGray, m_nGray));
}

BOOL CCommonControlsView::OnEraseBkgnd(CDC* /*pDC*/)
{
	// The framework calls this member function
	// when the CWnd object background needs erasing
	// (for example, when resized). It is called to
	// prepare an invalidated region for painting.

	// Return TRUE to prevent the background from being erased.
	// The OnDraw member function draws the background in this application.
	return TRUE;
}

void CCommonControlsView::OnPaint()
{
	// The framework calls this member
	// function when Windows or an application
	// makes a request to repaint a portion of
	// an application’s window. TheWM_PAINT message
	// is sent when the UpdateWindow or RedrawWindow
	// member function is called.

	// Create a CPaintDC object and prepares the
	// application window for painting.
	CPaintDC dc(this);

	// Create a CXTPBufferDC.
	// This is a memory DC that is used to prevent flickering.
	CXTPBufferDC dcMem(dc, CXTPClientRect(this));

	// Call OnDraw with the memory DC dcMem.
	OnDraw(&dcMem);
}


/////////////////////////////////////////////////////////////////////////////
// CCommonControlsView printing

BOOL CCommonControlsView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CCommonControlsView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CCommonControlsView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CCommonControlsView diagnostics

#ifdef _DEBUG
void CCommonControlsView::AssertValid() const
{
	CView::AssertValid();
}

void CCommonControlsView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CCommonControlsDoc* CCommonControlsView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CCommonControlsDoc)));
	return (CCommonControlsDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CCommonControlsView message handlers

void CCommonControlsView::OnUpdateSlider(CCmdUI* pCmdUI)
{
	// This function is called when the horizontal scroll
	// bar is scrolled on the Slider Control.

	// Enable the control.
	pCmdUI->Enable();

	// Use the DYNAMIC_DOWNCAST macro to cast the CWnd* pCmdUI->m_pOther into a CXTPCommandBar* object.
	CXTPCommandBar* pCommandBar = DYNAMIC_DOWNCAST(CXTPCommandBar, pCmdUI->m_pOther);
	if (!pCommandBar)
		return;

	// Use the DYNAMIC_DOWNCAST macro to cast the CSliderCtrl* control into a CXTPControlCustom* control.
	CXTPControlCustom* pControlSlider = DYNAMIC_DOWNCAST(CXTPControlCustom, pCommandBar->GetControl(pCmdUI->m_nIndex));
	if (!pControlSlider)
		return;

	// Use the DYNAMIC_DOWNCAST macro to cast the CWnd* control into a CSliderCtrl* control.
	CSliderCtrl* pSlider = DYNAMIC_DOWNCAST(CSliderCtrl, pControlSlider->GetControl());
	if (pSlider && pSlider->GetSafeHwnd())
	{
		int nMax = pSlider->GetRangeMax(); // Get the maximum range of the slider control
		int nPos = m_nGray * nMax / 0xFF;  // Determine the new position of the slider control's scroll bar.

		if (pSlider->GetPos() != nPos)
		{
			// If the new position of the scroll bar is
			// different than the current position
			// of the scroll bar then move the scroll
			// bar to the new position.
			pSlider->SetPos(nPos);
		}
	}
}

void CCommonControlsView::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// The framework calls this member function
	// when the user clicks a window's horizontal scroll bar.

	// Use the DYNAMIC_DOWNCAST macro to cast the
	// CScrollBar* object into a CSliderCtrl* object.
	CSliderCtrl* pSlider = DYNAMIC_DOWNCAST(CSliderCtrl, pScrollBar);
	if (pSlider != 0)
	{
		int nPos = pSlider->GetPos();      // Get the current position of the Slider control's scroll bar.
		int nMax = pSlider->GetRangeMax(); // Get the maximum range of the Sliders control's scroll bar.

		m_nGray = nPos * 0xFF / nMax;      // Calculate the value of m_nGray.

		// Invalidate the entire client area of the window.
		// This sends a WM_PAINT message causing the OnPaint member
		// function to be called.  Do not erase the background. The
		// FALSE argument tells MFC not to erase the background.
		Invalidate(FALSE);
	}

	// Let the base class handle the message.
	CView::OnHScroll(nSBCode, nPos, pScrollBar);
}
