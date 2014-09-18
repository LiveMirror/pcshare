// HistoryView.cpp : implementation file
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
#include "GUI_Explorer.h"
#include "HistoryView.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHistoryView

IMPLEMENT_DYNCREATE(CHistoryView, CView)

CHistoryView::CHistoryView()
{
}

CHistoryView::~CHistoryView()
{
}


BEGIN_MESSAGE_MAP(CHistoryView, CView)
	//{{AFX_MSG_MAP(CHistoryView)
	ON_WM_CREATE()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_CAPT_BUTTON, OnCaptionButton)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHistoryView drawing

void CHistoryView::OnDraw(CDC* pDC)
{
	// TODO: add draw code here
	CRect r;
	GetClientRect(&r);
	pDC->SetBkMode(TRANSPARENT);

	int x = r.Width()/2 -50;
	int y = r.Height()/2-25;

	CRect rcSquare(x,y,x+100,y+50);

	CFont* pOldFont = pDC->SelectObject(&XTAuxData().font);
	pDC->DrawEdge(&rcSquare, BDR_RAISEDOUTER, BF_RECT);
	pDC->DrawText(_T("History View"), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	pDC->SelectObject(pOldFont);
}

/////////////////////////////////////////////////////////////////////////////
// CHistoryView diagnostics

#ifdef _DEBUG
void CHistoryView::AssertValid() const
{
	CView::AssertValid();
}

void CHistoryView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CHistoryView message handlers

BOOL CHistoryView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Add your specialized code here and/or call the base class
	cs.dwExStyle |= WS_EX_STATICEDGE;
	cs.style |= WS_CLIPCHILDREN|WS_CLIPSIBLINGS;

	return CView::PreCreateWindow(cs);
}

int CHistoryView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// Create the image list used by frame buttons.
	m_ilButton.Create (XT_IDB_BTN_OUT, 16, 1, RGB(255,0,255));

	// Create the caption.
	if (!m_barCaption.Create(this, _T("History")))
	{
		TRACE0( "Unable to create caption.\n" );
		return -1;
	}

	// Create the caption button.
	if (!m_btnCaption.Create(NULL, WS_VISIBLE|BS_ICON|BS_OWNERDRAW|BS_CENTER|BS_VCENTER,
		CRect(0,0,0,0), this, IDC_CAPT_BUTTON))
	{
		TRACE0( "Unable to create caption button.\n" );
		return -1;
	}

	// set the caption buttons icon.
	m_btnCaption.SetIcon(CSize( 16, 15 ), m_ilButton.ExtractIcon(2));
	if (XTAuxData().bXPMode)
	{
		m_btnCaption.SetXButtonStyle(
			(m_btnCaption.GetXButtonStyle() | BS_XT_XPFLAT) & ~BS_XT_FLAT);
	}


	return 0;
}

void CHistoryView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	if( m_barCaption.GetSafeHwnd()) {
		m_barCaption.MoveWindow( 0, 0, cx, 19 );
	}

	if( m_btnCaption.GetSafeHwnd()) {
		m_btnCaption.MoveWindow( cx-18, 2, 16, 15 );
	}
}

void CHistoryView::OnCaptionButton()
{
	CMainFrame* pMainFrame = (CMainFrame*)GetParentFrame();
	ASSERT_VALID(pMainFrame);
	pMainFrame->GetSplitterWnd().HideColumn(0);
}
