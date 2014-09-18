// XTPDockContext.cpp : implementation of the CXTPDockContext class.
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

#include "XTPDockContext.h"
#include "XTPToolBar.h"
#include "XTPCommandBars.h"
#include "XTPMouseManager.h"
#include "XTPDockBar.h"
#include "XTPDialogBar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


void CXTPDockContext::AdjustRectangle(CRect& rect, CPoint pt)
{
	int nXOffset = (pt.x < rect.left) ? (pt.x - rect.left) :
					(pt.x > rect.right) ? (pt.x - rect.right) : 0;
	int nYOffset = (pt.y < rect.top) ? (pt.y - rect.top) :
					(pt.y > rect.bottom) ? (pt.y - rect.bottom) : 0;
	rect.OffsetRect(nXOffset, nYOffset);
}


CXTPDockContext::CXTPDockContext(CXTPToolBar* pBar)
{
	m_pBar = pBar;
	m_pDockSite = pBar->GetFrameSite();
	ASSERT(m_pDockSite);

	m_uMRUDockPosition = xtpBarTop;
	m_rectMRUDockPos.SetRectEmpty();
	m_ptMRUFloatPos.y = m_ptMRUFloatPos.x = 0;

	ASSERT(m_pDockSite->IsFrameWnd());

}

CXTPDockContext::~CXTPDockContext(void)
{
}

void CXTPDockContext::StartDrag(CPoint pt)
{
	m_nHitTest = HTCLIENT;

	CRect rectMRUDockPos = m_rectMRUDockPos;
	XTPBarPosition uMRUDockPosition = m_uMRUDockPosition;
	BOOL bFloating = m_pBar->m_barPosition == xtpBarFloating;

	CXTPWindowRect rect(m_pBar);
	m_ptLast = pt;

	AdjustRectangle(rect, pt);
	m_rectDragFrame = m_rectDragDock = rect;

	SetCursor(LoadCursor(NULL, IDC_SIZEALL));

	XTPMouseManager()->LockMouseMove();

	Track();

	XTPMouseManager()->UnlockMouseMove();

	if (m_pBar->m_barPosition == xtpBarFloating)
	{
		m_ptMRUFloatPos = m_rectDragFrame.TopLeft();

		if (!bFloating)
		{
			m_rectMRUDockPos = rectMRUDockPos;
			m_uMRUDockPosition = uMRUDockPosition;
		}
	}
}

void CXTPDockContext::StartResize(int nHitTest, CPoint pt)
{
	m_nHitTest = nHitTest;
	m_ptLast = pt;

	m_pBar->GetWindowRect(m_rectDragFrame);

	Track();

	m_pBar->GetWindowRect(m_rectDragFrame);

	if (m_pBar->m_barPosition == xtpBarFloating)
		m_ptMRUFloatPos = m_rectDragFrame.TopLeft();

	m_pBar->m_nMRUWidth = m_rectDragFrame.Width();

}


void CXTPDockContext::Track()
{
	// don't handle if capture already set
	if (::GetCapture() != NULL)
		return;

	// set capture to the window which received this message
	m_pBar->SetCapture();
	ASSERT(m_pBar == CWnd::GetCapture());

	CPoint pt(0, 0);
	// get messages until capture lost or cancelled/accepted
	while (CWnd::GetCapture() == m_pBar)
	{
		MSG msg;

		while (::PeekMessage(&msg, NULL, WM_PAINT, WM_PAINT, PM_NOREMOVE))
		{
			if (!GetMessage(&msg, NULL, WM_PAINT, WM_PAINT))
				break;
			DispatchMessage(&msg);
		}

		if (!::GetMessage(&msg, NULL, 0, 0))
		{
			AfxPostQuitMessage((int)msg.wParam);
			break;
		}

		if (msg.message == WM_LBUTTONUP)
			break;
		else if (msg.message == WM_MOUSEMOVE && pt != msg.pt)
		{
			pt = msg.pt;
			if (m_nHitTest == HTCLIENT) Move(msg.pt);
				else if (m_pBar->IsDialogBar()) Resize(msg.pt); else Stretch(msg.pt);
		}
		else if (msg.message == WM_KEYDOWN)
		{
			if (msg.wParam == VK_ESCAPE)
			{
				break;
			}
		} else
			DispatchMessage(&msg);


		if (m_pDockSite->m_nIdleFlags & CFrameWnd::idleLayout)
		{
			m_pDockSite->RecalcLayout(FALSE);
			m_pDockSite->m_nIdleFlags &= ~CFrameWnd::idleLayout;
		}
	}

	ReleaseCapture();

	CXTPCommandBars* pCommandBars = m_pBar->GetCommandBars();
	if (pCommandBars)
	{
		pCommandBars->GetCommandBarsOptions()->bDirtyState = TRUE;
	}
}

void CXTPDockContext::Move(CPoint pt)
{
	CPoint ptOffset = pt - m_ptLast;
	CXTPCommandBars* pCommandBars = m_pBar->GetCommandBars();
	ASSERT(pCommandBars);

	m_rectDragFrame.OffsetRect(ptOffset);
	m_rectDragDock.OffsetRect(ptOffset);

	DWORD dwFlags = m_pBar->GetFlags();

	BOOL bDocked = FALSE;

	CXTPDockBar* pDockBar = pCommandBars->CanDock(pt, m_pBar->m_pDockBar);
	if (pDockBar)
	{
		XTPBarPosition position = pDockBar->GetPosition();
		if ((position == xtpBarTop && dwFlags & xtpFlagAlignTop) ||
			(position == xtpBarLeft && dwFlags & xtpFlagAlignLeft) ||
			(position == xtpBarRight && dwFlags & xtpFlagAlignRight) ||
			(position == xtpBarBottom && dwFlags & xtpFlagAlignBottom))
		{
			CXTPDockBar* pOldDockBar = m_pBar->m_pDockBar;
			pCommandBars->DockCommandBar(m_pBar, &m_rectDragDock, pDockBar);

			if (pDockBar != pOldDockBar || m_pBar->GetPosition() == xtpBarFloating)
			{
				m_pDockSite->RecalcLayout(FALSE);
				m_pBar->GetWindowRect(m_rectDragDock);
			}

			bDocked = TRUE;
		}
	}

	if (!bDocked && (dwFlags & xtpFlagFloating))
	{
		if (m_pBar->GetPosition() != xtpBarFloating)
		{
			pCommandBars->FloatCommandBar(m_pBar);
			CSize sz = m_pBar->CalcDynamicLayout(0, LM_HORZ | LM_MRUWIDTH | LM_COMMIT);
			m_rectDragFrame = CRect(m_rectDragFrame.TopLeft(), sz);
			AdjustRectangle(m_rectDragFrame, pt);
		}
		m_pBar->MoveWindow(m_rectDragFrame);
	}

	m_ptLast = pt;


}

void CXTPDockContext::Resize(CPoint pt)
{
	CPoint ptOffset = pt - m_ptLast;
	ASSERT(m_pBar->IsDialogBar());

	if (m_nHitTest == HTLEFT || m_nHitTest == HTTOPLEFT || m_nHitTest == HTBOTTOMLEFT)
		m_rectDragFrame.left += ptOffset.x;

	if (m_nHitTest == HTRIGHT || m_nHitTest == HTTOPRIGHT || m_nHitTest == HTBOTTOMRIGHT)
		m_rectDragFrame.right += ptOffset.x;

	if (m_nHitTest == HTTOP || m_nHitTest == HTTOPRIGHT || m_nHitTest == HTTOPLEFT)
		m_rectDragFrame.top += ptOffset.y;

	if (m_nHitTest == HTBOTTOM|| m_nHitTest == HTBOTTOMRIGHT || m_nHitTest == HTBOTTOMLEFT)
		m_rectDragFrame.bottom += ptOffset.y;

	CRect rectTemp;
	m_pBar->GetWindowRect(rectTemp);

	if (rectTemp.Size() != m_rectDragFrame.Size())
	{
		if (CRect().IntersectRect(CXTPDrawHelpers::GetWorkArea(m_pBar), m_rectDragFrame))
		{
			((CXTPDialogBar*)m_pBar)->OnResize(m_rectDragFrame, m_nHitTest);
			m_pBar->Invalidate(FALSE);
		}
	}


	m_ptLast = pt;
}

void CXTPDockContext::Stretch(CPoint pt)
{
	CPoint ptOffset = pt - m_ptLast;

	DWORD dwMode = LM_HORZ | LM_COMMIT;
	int nLength = 0;

	if (m_nHitTest == HTLEFT || m_nHitTest == HTRIGHT)
	{
		if (m_nHitTest == HTLEFT) m_rectDragFrame.left += ptOffset.x; else m_rectDragFrame.right += ptOffset.x;
		nLength = m_rectDragFrame.Width();
	} else
	{
		dwMode |= LM_LENGTHY;
		if (m_nHitTest == HTTOP) m_rectDragFrame.top += ptOffset.y; else m_rectDragFrame.bottom += ptOffset.y;
		nLength = m_rectDragFrame.Height();
	}
	nLength = (nLength >= 0) ? nLength : 0;

	CSize size = m_pBar->CalcDynamicLayout(nLength, dwMode);

	CRect rectTemp;
	m_pBar->GetWindowRect(rectTemp);

	if (rectTemp.Size() != size)
	{
		if (m_nHitTest == HTRIGHT || m_nHitTest == HTBOTTOM)
		{
			rectTemp.BottomRight() = rectTemp.TopLeft() + size;
		}
		else if (m_nHitTest == HTLEFT)
		{
			rectTemp.bottom = rectTemp.top + size.cy;
			rectTemp.left = rectTemp.right - size.cx;

		} else if (m_nHitTest == HTTOP)
		{
			rectTemp.top = rectTemp.bottom - size.cy;
			rectTemp.right = rectTemp.left + size.cx;
		}

		if (CRect().IntersectRect(CXTPDrawHelpers::GetWorkArea(m_pBar), rectTemp))
		{
			m_pBar->MoveWindow(rectTemp);
			m_pBar->Invalidate(FALSE);
		}
	}
	m_ptLast = pt;
}


BOOL CXTPDockContext::ToggleDocking()
{
	CXTPCommandBars* pCommandBars = m_pBar->GetCommandBars();

	if (m_pBar->GetPosition() == xtpBarFloating)
	{
		// Dock it only if is allowed to be docked
		if (!(m_pBar->m_dwFlags & xtpFlagAlignAny))
			return FALSE;

		CRect rect = m_rectMRUDockPos;
		CXTPDockBar* pDockBar = pCommandBars->GetDockBar(m_uMRUDockPosition);
		pDockBar->ClientToScreen(&rect);

		// dock it at the specified position, RecalcLayout will snap
		pCommandBars->DockCommandBar(m_pBar,  &rect, pDockBar);
		m_pDockSite->RecalcLayout();

	} else
	{
		if (!(m_pBar->m_dwFlags & xtpFlagFloating))
			return FALSE;

		if (!pCommandBars->GetCommandBarsOptions()->bDblClickFloat)
			return FALSE;

		CPoint ptFloat = m_ptMRUFloatPos;
		if (ptFloat.x <= 0 || ptFloat.y <= 0)
		{
			ptFloat = m_rectMRUDockPos.TopLeft();
			m_pBar->GetParent()->ClientToScreen(&ptFloat);
		}
		pCommandBars->FloatCommandBar(m_pBar);
		CSize sz = m_pBar->CalcDynamicLayout(0, LM_HORZ | LM_MRUWIDTH | LM_COMMIT);
		m_pBar->MoveWindow(CRect(ptFloat, sz));
	}

	pCommandBars->GetCommandBarsOptions()->bDirtyState = TRUE;
	return TRUE;
}
