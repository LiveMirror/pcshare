// PreviewPane.cpp : implementation file
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
#include "GUI_Outlook.h"
#include "PreviewPane.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define CAPT_MAX_HEIGHT 35
#define CAPT_MIN_HEIGHT 5

#define TEXT_BOLD_LEFT  4
#define TEXT_NORM_LEFT  75
#define TEXT_LINE_ONE   4
#define TEXT_LINE_TWO   18

/////////////////////////////////////////////////////////////////////////////
// CPreviewPane

IMPLEMENT_DYNCREATE(CPreviewPane, CView)

CPreviewPane::CPreviewPane()
{
	m_strTitle1 = _T("Message:");
	m_strDescp1 = _T("Some words about this message");
	m_strTitle2 = _T("Content:");
	m_strDescp2 = _T("This message contains nothing special.");

	m_strViewMessage = _T("This area can be used for preview info...");
	m_bMax = TRUE;
}

CPreviewPane::~CPreviewPane()
{

}

BEGIN_MESSAGE_MAP(CPreviewPane, CView)
	//{{AFX_MSG_MAP(CPreviewPane)
	ON_WM_LBUTTONDBLCLK()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPreviewPane drawing

void CPreviewPane::OnDraw(CDC* pDC)
{
	// TODO: add draw code here
	GetWindowRect(&m_rcCaption);
	ScreenToClient(&m_rcCaption);

	pDC->FillSolidRect(m_rcCaption, GetXtremeColor(COLOR_WINDOW));
	m_rcView.CopyRect(m_rcCaption);

	m_rcCaption.bottom = m_bMax ? CAPT_MAX_HEIGHT : CAPT_MIN_HEIGHT;
	m_rcView.top = m_rcCaption.bottom;

	// Default header text color
	COLORREF clrText = GetXtremeColor(COLOR_BTNTEXT);

	XTPPaintTheme theme = XTPPaintManager()->GetCurrentTheme();
	if (theme == xtpThemeOffice2003)
	{
		if ( m_richEditCtrl.m_hWnd == ::GetFocus( ) )
		{
			XTPDrawHelpers()->GradientFill(pDC, m_rcCaption, XTPColorManager()->grcCaption.clrDark,XTPColorManager()->grcCaption.clrLight, FALSE);
			clrText = GetXtremeColor(COLOR_WINDOW);
		}
		else
		{
			XTPDrawHelpers()->GradientFill(pDC, m_rcCaption, XTPColorManager()->grcToolBar.clrDark, XTPColorManager()->grcToolBar.clrLight, FALSE);
		}
		pDC->FillSolidRect(m_rcCaption.left, m_rcCaption.bottom - 1, m_rcCaption.right, 1, GetXtremeColor(XPCOLOR_FRAME));
	}
	else
	{
		// Test to see if the window has the focus and draw the header accordingly
		if ( m_richEditCtrl.m_hWnd == ::GetFocus( ) )
		{
			pDC->FillSolidRect(m_rcCaption, GetXtremeColor(COLOR_HIGHLIGHT));
			clrText = GetXtremeColor(COLOR_WINDOW);
		}
		else
		{
			pDC->FillSolidRect(m_rcCaption, XTPPaintManager()->GetXtremeColor(XPCOLOR_3DFACE));
		}
		CRect rc(m_rcCaption);
		rc.DeflateRect(1, 1, theme == xtpThemeNativeWinXP? 0: 1, 0);
		pDC->Draw3dRect(rc, GetXtremeColor(COLOR_3DHILIGHT), GetStaticFrameColor(TRUE));
	}



	pDC->SetBkMode(TRANSPARENT);

	if (m_bMax)
	{
		// Set text color
		COLORREF clrOldText = pDC->SetTextColor(clrText);
		pDC->SelectObject(&XTAuxData().fontBold);

		pDC->TextOut(TEXT_BOLD_LEFT, TEXT_LINE_ONE, m_strTitle1);
		pDC->TextOut(TEXT_BOLD_LEFT, TEXT_LINE_TWO, m_strTitle2);

		pDC->SelectObject(&XTAuxData().font);
		pDC->TextOut(TEXT_NORM_LEFT, TEXT_LINE_ONE, m_strDescp1);
		pDC->TextOut(TEXT_NORM_LEFT, TEXT_LINE_TWO, m_strDescp2);

		// Return original color
		pDC->SetTextColor(clrOldText);
	}
}

/////////////////////////////////////////////////////////////////////////////
// CPreviewPane diagnostics

#ifdef _DEBUG
void CPreviewPane::AssertValid() const
{
	CView::AssertValid();
}

void CPreviewPane::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CPreviewPane message handlers

void CPreviewPane::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// Toggle caption.
	if (m_rcCaption.PtInRect(point))
	{
		m_bMax = !m_bMax;
		Invalidate();
		ResizeEditCtrl();
	}

	CView::OnLButtonDblClk(nFlags, point);
}

BOOL CPreviewPane::OnEraseBkgnd(CDC* pDC)
{
	UNUSED_ALWAYS(pDC);
	return TRUE;
}

void CPreviewPane::OnPaint()
{
	CPaintDC dc(this);

	// Get the client rect.
	CRect rectClient;
	GetClientRect(&rectClient);

	CXTMemDC memDC(&dc, rectClient, GetXtremeColor(COLOR_WINDOW));
	OnDraw(&memDC);
}

int CPreviewPane::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// Create the rich edit control that will be used in the view.
	if (!m_richEditCtrl.Create(WS_CHILD | WS_VISIBLE | ES_LEFT | ES_MULTILINE | ES_WANTRETURN,
		CRect(0,0,0,0), this, 0 ))
		return -1;

	// Set the font for the edit control.
	m_richEditCtrl.SetFont( &XTAuxData().font );

	// Set the text for the edit control.
	m_richEditCtrl.SetWindowText( m_strViewMessage );

	return 0;
}

void CPreviewPane::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	ResizeEditCtrl();
}

void CPreviewPane::ResizeEditCtrl()
{
	if (::IsWindow( m_richEditCtrl.m_hWnd ))
	{
		CRect rc;
		GetClientRect( &rc );

		int x = 0;
		int y = ( m_bMax ? CAPT_MAX_HEIGHT : CAPT_MIN_HEIGHT );

		m_richEditCtrl.MoveWindow(x, y, rc.Width(), rc.Height()-y);
	}
}

/////////////////////////////////////////////////////////////////////////////
// CPreviewEdit

BEGIN_MESSAGE_MAP(CPreviewEdit, CRichEditCtrl)
	//{{AFX_MSG_MAP(CPreviewEdit)
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CPreviewEdit::OnSetFocus(CWnd* pOldWnd)
{
	CRichEditCtrl::OnSetFocus(pOldWnd);
	GetParent()->InvalidateRect(NULL);
}

void CPreviewEdit::OnKillFocus(CWnd* pNewWnd)
{
	CRichEditCtrl::OnKillFocus(pNewWnd);
	GetParent()->InvalidateRect(NULL);
}
