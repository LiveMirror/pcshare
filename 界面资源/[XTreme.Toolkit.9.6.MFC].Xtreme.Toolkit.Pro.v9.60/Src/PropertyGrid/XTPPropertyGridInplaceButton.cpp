// XTPPropertyGridInplaceButton.cpp : implementation of the CXTPPropertyGridInplaceButton class.
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
#include "XTPVC80Helpers.h"
#include "XTPDrawHelpers.h"

#include "XTPPropertyGridInplaceEdit.h"
#include "XTPPropertyGridInplaceButton.h"
#include "XTPPropertyGridInplaceList.h"
#include "XTPPropertyGridItem.h"
#include "XTPPropertyGridInplaceButton.h"
#include "XTPPropertyGridInplaceEdit.h"
#include "XTPPropertyGrid.h"
#include "XTPPropertyGridPaintManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CXTColorPicker

CXTPPropertyGridInplaceButton::CXTPPropertyGridInplaceButton()
: m_bPressed(FALSE)
, m_bOver(FALSE)
, m_pGrid(0)
, m_pItem(0)
{

}

CXTPPropertyGridInplaceButton::~CXTPPropertyGridInplaceButton()
{

}

IMPLEMENT_DYNAMIC(CXTPPropertyGridInplaceButton, CStatic)

BEGIN_MESSAGE_MAP(CXTPPropertyGridInplaceButton, CStatic)
	//{{AFX_MSG_MAP(CXTPPropertyGridInplaceButton)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_CAPTURECHANGED()
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
	ON_WM_KEYDOWN()
	ON_WM_GETDLGCODE()
	ON_MESSAGE_VOID(WM_MOUSELEAVE, OnMouseLeave)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

UINT CXTPPropertyGridInplaceButton::OnGetDlgCode()
{
	return DLGC_WANTALLKEYS;
}


void CXTPPropertyGridInplaceButton::HideWindow()
{
	if (m_hWnd)
	{
		ShowWindow(SW_HIDE);
	}
}

void CXTPPropertyGridInplaceButton::Create(CXTPPropertyGridItem* pItem, CRect rect)
{
	ASSERT(pItem && pItem->GetGrid());
	
	int nWidth = GetSystemMetrics(SM_CXHTHUMB);

	m_pGrid      = pItem->GetGrid();
	rect.left    = rect.right - nWidth;
	rect.top -= 1;

	

	if (!m_hWnd)
	{
		CStatic::Create(_T(""), SS_NOTIFY|WS_CHILD, rect, m_pGrid);
	}

	SetWindowPos(0, rect.left, rect.top, rect.Width(), rect.Height(), SWP_NOZORDER|SWP_SHOWWINDOW);
	m_pItem = pItem;
}

/////////////////////////////////////////////////////////////////////////////
// CXTPPropertyGridInplaceButton message handlers

void CXTPPropertyGridInplaceButton::OnPaint()
{
	CPaintDC dc(this);

	CXTPPropertyGridView* pView = (CXTPPropertyGridView*)m_pGrid;
	if (pView)
	{
		pView->m_pGrid->GetPaintManager()->FillInplaceButton(&dc, this);
	}
}

void CXTPPropertyGridInplaceButton::OnLButtonDown(UINT, CPoint)
{
	m_bOver = m_bPressed = TRUE;
	Invalidate(FALSE);
	SetCapture();
}

void CXTPPropertyGridInplaceButton::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (m_bPressed)
	{
		m_bPressed = FALSE;
		Invalidate(FALSE);
		ReleaseCapture();
		if (m_bOver)
			m_pItem->OnInplaceButtonDown();
	}
	CStatic::OnLButtonUp(nFlags, point);
}

void CXTPPropertyGridInplaceButton::OnMouseMove(UINT nFlags, CPoint point)
{
	CXTPClientRect rect(this);
	if ((rect.PtInRect(point) && !m_bOver) ||
		(!rect.PtInRect(point) && m_bOver))
	{
		m_bOver = rect.PtInRect(point);
		Invalidate(FALSE);

		if (m_bOver)
		{
			TRACKMOUSEEVENT tme = { sizeof(TRACKMOUSEEVENT), TME_LEAVE, m_hWnd};
			_TrackMouseEvent(&tme);
		}
	}

	CStatic::OnMouseMove(nFlags, point);
}

void CXTPPropertyGridInplaceButton::OnMouseLeave()
{
	//reset mouse vars
	OnMouseMove(0,CPoint(-1, -1));
}


void CXTPPropertyGridInplaceButton::OnCaptureChanged(CWnd *pWnd)
{
	m_bPressed = FALSE;
	Invalidate(FALSE);

	CStatic::OnCaptureChanged(pWnd);
}

void CXTPPropertyGridInplaceButton::DestroyItem()
{
	// reset variables to defaults.
	m_bPressed = FALSE;
	m_bOver    = FALSE;
	m_pGrid    = NULL;
	m_pItem    = NULL;

	// destroy the window.
	//DestroyWindow( );
	if (::IsWindow(m_hWnd)) ShowWindow(SW_HIDE);
}

void CXTPPropertyGridInplaceButton::OnSetFocus(CWnd* pOldWnd)
{
	CStatic::OnSetFocus(pOldWnd);
	Invalidate(FALSE);

#ifdef XTP_SITENOTIFY_ONFOCUS
	XTP_SITENOTIFY_ONFOCUS(this, m_pGrid, TRUE)
#endif

}

void CXTPPropertyGridInplaceButton::OnKillFocus(CWnd* pNewWnd)
{
	CStatic::OnKillFocus(pNewWnd);
	if (m_pGrid)
		m_pGrid->Invalidate(FALSE);
	Invalidate(FALSE);

#ifdef XTP_SITENOTIFY_ONFOCUS
	XTP_SITENOTIFY_ONFOCUS(this, m_pGrid, FALSE)
#endif
}

void CXTPPropertyGridInplaceButton::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (m_pItem && (nChar == VK_SPACE || nChar == VK_DOWN || nChar == VK_RETURN))
	{
		m_pItem->OnInplaceButtonDown();
		return;
	}
	if (nChar == VK_TAB && m_pGrid)
	{
		((CXTPPropertyGrid*)m_pGrid->GetParent())->OnNavigate(
			xtpGridUIInplaceButton, GetKeyState(VK_SHIFT) >= 0, m_pItem);
		return;
	}

	CStatic::OnKeyDown(nChar, nRepCnt, nFlags);
}
