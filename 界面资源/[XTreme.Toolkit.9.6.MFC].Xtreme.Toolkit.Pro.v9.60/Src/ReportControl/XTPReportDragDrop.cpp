// CXTPReportDragDrop.cpp : implementation of the CXTPReportHeaderDropWnd class.
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
#include "XTPDrawHelpers.h"

#include "XTPReportDefines.h"
#include "XTPReportHeader.h"
#include "XTPReportPaintManager.h"
#include "XTPReportColumn.h"
#include "XTPReportDragDrop.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CXTPReportHeaderDragWnd

CXTPReportHeaderDragWnd::CXTPReportHeaderDragWnd()
{
	m_pHeader = NULL;
	m_pPaintManager = NULL;
	m_pColumn = NULL;
}

CXTPReportHeaderDragWnd::~CXTPReportHeaderDragWnd()
{
}


BEGIN_MESSAGE_MAP(CXTPReportHeaderDragWnd, CWnd)
	//{{AFX_MSG_MAP(CXTPReportHeaderDragWnd)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CXTPReportHeaderDragWnd message handlers

BOOL CXTPReportHeaderDragWnd::Create(CRect rect, CXTPReportHeader* pHeader, CXTPReportPaintManager* pPaintManager, CXTPReportColumn* pColumn)
{
	m_pHeader = pHeader;
	m_pPaintManager = pPaintManager;
	m_pColumn = pColumn;
	if (rect.Height() == 0)
		rect.bottom = rect.top + pPaintManager->GetHeaderHeight();

	DWORD dwStyle = WS_POPUP|WS_DISABLED;
	DWORD dwExStyle = WS_EX_TOOLWINDOW|WS_EX_TOPMOST;

	return CreateEx(dwExStyle, AfxRegisterWndClass(0, LoadCursor(0, IDC_ARROW)), NULL, dwStyle,
		rect.left, rect.top, rect.Width(), rect.Height(),
		NULL, NULL, NULL );
}


void CXTPReportHeaderDragWnd::OnPaint()
{
	CPaintDC dc(this);
	CXTPClientRect rc(this);
	CXTPBufferDC memDC(dc, rc);
	OnDraw(&memDC, rc);
}

BOOL CXTPReportHeaderDragWnd::OnEraseBkgnd(CDC* pDC)
{
	UNREFERENCED_PARAMETER(pDC);
	return TRUE;
}

void CXTPReportHeaderDragWnd::OnDraw(CDC* pDC, CRect rcClient)
{
	if (!m_pPaintManager)
		return;

	// draw background
	m_pPaintManager->FillHeaderControl(pDC, rcClient);
	// draw column header
	m_pPaintManager->DrawColumn(pDC, m_pColumn, m_pHeader, rcClient, TRUE);
}

void CXTPReportHeaderDragWnd::PostNcDestroy()
{
	CWnd::PostNcDestroy();
	delete this;
}

/////////////////////////////////////////////////////////////////////////////
// CXTPReportHeaderDropWnd

CXTPReportHeaderDropWnd::CXTPReportHeaderDropWnd(COLORREF crColor)
{
	m_brush.CreateSolidBrush(crColor);

	// Register the window class if it has not already been registered.
	//WNDCLASS wndclass;
	//HINSTANCE hInst = AfxGetInstanceHandle();

/*  if(!(::GetClassInfo(hInst, XTPDROPWND_CLASSNAME, &wndclass)))
	{
		// otherwise we need to register a new class
		wndclass.style = CS_SAVEBITS ;
		wndclass.lpfnWndProc = ::DefWindowProc;
		wndclass.cbClsExtra = wndclass.cbWndExtra = 0;
		wndclass.hInstance = hInst;
		wndclass.hIcon = NULL;
		wndclass.hCursor = LoadCursor( hInst, IDC_ARROW );
		wndclass.hbrBackground = (HBRUSH)m_brush;
		wndclass.lpszMenuName = NULL;
		wndclass.lpszClassName = XTPDROPWND_CLASSNAME;
		if (!AfxRegisterClass(&wndclass))
			AfxThrowResourceException();
	}*/
}

CXTPReportHeaderDropWnd::~CXTPReportHeaderDropWnd()
{
}


BEGIN_MESSAGE_MAP(CXTPReportHeaderDropWnd, CWnd)
	//{{AFX_MSG_MAP(CXTPReportHeaderDropWnd)
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CXTPReportHeaderDropWnd message handlers

BOOL CXTPReportHeaderDropWnd::Create(int nHeight)
{
	m_nHeight = nHeight + 20;

	DWORD dwStyle = WS_POPUP|WS_DISABLED;
	DWORD dwExStyle = WS_EX_TOOLWINDOW ;

	BOOL bResult = CreateEx(dwExStyle, AfxRegisterWndClass(0, LoadCursor(0, IDC_ARROW)), NULL, dwStyle,
		0, 0, 12, m_nHeight,
		NULL, NULL, NULL );

	CRgn rgn1, rgn2;
	POINT ptArrow[7];

	ptArrow[0].x = 8;   ptArrow[0].y = 0;
	ptArrow[1].x = 8;   ptArrow[1].y = 4;
	ptArrow[2].x = 11;  ptArrow[2].y = 4;
	ptArrow[3].x = 6;   ptArrow[3].y = 9;
	ptArrow[4].x = 1;   ptArrow[4].y = 4;
	ptArrow[5].x = 4;   ptArrow[5].y = 4;
	ptArrow[6].x = 4;   ptArrow[6].y = 0;
	rgn1.CreatePolygonRgn(ptArrow, 7, ALTERNATE);


	ptArrow[0].x = 4;   ptArrow[0].y = m_nHeight;
	ptArrow[1].x = 4;   ptArrow[1].y = m_nHeight-4;
	ptArrow[2].x = 0;   ptArrow[2].y = m_nHeight-4;
	ptArrow[3].x = 6;   ptArrow[3].y = m_nHeight-10;
	ptArrow[4].x = 12;  ptArrow[4].y = m_nHeight-4;
	ptArrow[5].x = 8;   ptArrow[5].y = m_nHeight-4;
	ptArrow[6].x = 8;   ptArrow[6].y = m_nHeight;
	rgn2.CreatePolygonRgn(ptArrow, 7, ALTERNATE);

	m_rgn.CreateRectRgn(0, 0, 12, nHeight);
	m_rgn.CombineRgn(&rgn1, &rgn2, RGN_OR);
	SetWindowRgn(m_rgn, FALSE);

	rgn1.DeleteObject();
	rgn2.DeleteObject();

	return bResult;
}


void CXTPReportHeaderDropWnd::PostNcDestroy()
{
	m_rgn.DeleteObject();

	CWnd::PostNcDestroy();
	delete this;
}

BOOL CXTPReportHeaderDropWnd::OnEraseBkgnd(CDC* pDC)
{
	pDC->FillRect(CRect(0, 0, 12, m_nHeight), &m_brush);
	return TRUE;
}

void CXTPReportHeaderDropWnd::SetWindowPos(int x, int y)
{
	CWnd::SetWindowPos( &wndTop,
		x - 6, y - (m_nHeight / 2), 0, 0,
		SWP_NOSIZE|SWP_SHOWWINDOW|SWP_NOACTIVATE);
}
