// DynamicHelpView.cpp : implementation file
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
#include "gui_visualstudio7.h"
#include "DynamicHelpView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDynamicHelpView

IMPLEMENT_DYNCREATE(CDynamicHelpView, CHtmlView)

CDynamicHelpView::CDynamicHelpView()
{
	//{{AFX_DATA_INIT(CDynamicHelpView)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CDynamicHelpView::~CDynamicHelpView()
{
}

void CDynamicHelpView::DoDataExchange(CDataExchange* pDX)
{
	CHtmlView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDynamicHelpView)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDynamicHelpView, CHtmlView)
	//{{AFX_MSG_MAP(CDynamicHelpView)
	//}}AFX_MSG_MAP
#if _MSC_VER >= 1200 // MFC 5.0
	ON_WM_NCPAINT()
	ON_WM_NCCALCSIZE()
#endif
	ON_WM_MOUSEACTIVATE()

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDynamicHelpView diagnostics

#ifdef _DEBUG
void CDynamicHelpView::AssertValid() const
{
	CHtmlView::AssertValid();
}

void CDynamicHelpView::Dump(CDumpContext& dc) const
{
	CHtmlView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDynamicHelpView message handlers

void CDynamicHelpView::OnInitialUpdate()
{
	// Use The LoadFromResource function
	// In the HTML File to locate resource in
	// your app add the file by name and in binary format
	// of type 2110 ( Magic Number for Microsoft)
	// HTML files are placed automatically under
	// category of type HTML
#if _MSC_VER >= 1200 // MFC 5.0
	LoadFromResource(IDR_DYNAMIC_HELP);
#endif
}

BOOL CDynamicHelpView::PreCreateWindow(CREATESTRUCT& cs)
{
	if (!CHtmlView::PreCreateWindow(cs))
		return FALSE;

	// TODO: Add your specialized code here and/or call the base class
	cs.style &= ~WS_BORDER;
	cs.dwExStyle = (cs.dwExStyle & ~WS_EX_CLIENTEDGE);

	return TRUE;
}

void CDynamicHelpView::OnNcPaint()
{
	CWindowDC dc(this);

	CRect rc;
	GetWindowRect(&rc);

	int cx = rc.Width();
	int cy = rc.Height();


	dc.Draw3dRect(0, 0, cx, cy, GetXtremeColor(COLOR_3DSHADOW), GetXtremeColor(COLOR_3DSHADOW));
}

void CDynamicHelpView::OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp)
{
	// adjust non-client area for border space
	lpncsp->rgrc[0].left   += 1;
	lpncsp->rgrc[0].top    += 1;
	lpncsp->rgrc[0].right  -= 1;
	lpncsp->rgrc[0].bottom -= 1;

	CHtmlView::OnNcCalcSize(bCalcValidRects, lpncsp);
}

int CDynamicHelpView::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message)
{
	return CWnd::OnMouseActivate(pDesktopWnd, nHitTest, message);
}
