// XTPDockingPaneContext.cpp : implementation of the CXTPDockingPaneContext class.
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
#include "XTPTabManager.h"

#include "XTPDockingPaneContext.h"
#include "XTPDockingPane.h"
#include "XTPDockingPaneManager.h"
#include "XTPDockingPaneTabbedContainer.h"
#include "XTPDockingPaneResource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



void AFX_CDECL CXTPDockingPaneContext::AdjustRectangle(CRect& rect, CPoint pt)
{
	int nXOffset = (pt.x < rect.left) ? (pt.x - rect.left) :
					(pt.x > rect.right) ? (pt.x - rect.right) : 0;
	int nYOffset = (pt.y < rect.top) ? (pt.y - rect.top) :
					(pt.y > rect.bottom) ? (pt.y - rect.bottom) : 0;
	rect.OffsetRect(nXOffset, nYOffset);
}

typedef BOOL (WINAPI *PFNSETLAYEREDWINDOWATTRIBUTES) (HWND hwnd,COLORREF crKey,BYTE bAlpha,DWORD dwFlags);

#ifndef LWA_ALPHA
#define LWA_ALPHA               0x00000002
#endif

#ifndef WS_EX_LAYERED
#define WS_EX_LAYERED           0x00080000
#endif

/////////////////////////////////////////////////////////////////////////////
// CXTPDockingPaneContextAlphaWnd

CXTPDockingPaneContextAlphaWnd::CXTPDockingPaneContextAlphaWnd()
{
}

CXTPDockingPaneContextAlphaWnd::~CXTPDockingPaneContextAlphaWnd()
{
}


BEGIN_MESSAGE_MAP(CXTPDockingPaneContextAlphaWnd, CWnd)
	//{{AFX_MSG_MAP(CXTPDockingPaneContextAlphaWnd)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CXTPDockingPaneContextAlphaWnd message handlers

BOOL CXTPDockingPaneContextAlphaWnd::OnEraseBkgnd(CDC* /*pDC*/)
{
	return TRUE;
}

void CXTPDockingPaneContextAlphaWnd::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	CRect rc;
	GetClientRect(&rc);

	dc.FillSolidRect(rc, GetSysColor(COLOR_HIGHLIGHT));
}

//////////////////////////////////////////////////////////////////////////
// CXTPDockingPaneContextStickerWnd

CXTPDockingPaneContextStickerWnd::CXTPDockingPaneContextStickerWnd()
{
	m_typeSticker = m_selectedSticker = xtpPaneStickerNone;
}

CXTPDockingPaneContextStickerWnd::~CXTPDockingPaneContextStickerWnd()
{
}


BEGIN_MESSAGE_MAP(CXTPDockingPaneContextStickerWnd, CWnd)
	//{{AFX_MSG_MAP(CXTPDockingPaneContextStickerWnd)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CXTPDockingPaneContextAlphaWnd message handlers

BOOL CXTPDockingPaneContextStickerWnd::OnEraseBkgnd(CDC* /*pDC*/)
{
	return TRUE;
}

struct CXTPDockingPaneContextStickerWnd::SPRITEINFO
{
	SPRITEINFO(int x, int y, int left, int top, int cx, int cy)
	{
		ptDest = CPoint(x, y);
		rcSrc.SetRect(left, top, left + cx, top + cy);
	}
	CPoint ptDest;
	CRect rcSrc;
};

static CXTPDockingPaneContextStickerWnd::SPRITEINFO sprites[] = {
	CXTPDockingPaneContextStickerWnd::SPRITEINFO(25, 0, 0, 0, 43, 30),
	CXTPDockingPaneContextStickerWnd::SPRITEINFO(0, 25, 30, 33, 30, 43),
	CXTPDockingPaneContextStickerWnd::SPRITEINFO(25, 63, 43, 0, 43, 30),
	CXTPDockingPaneContextStickerWnd::SPRITEINFO(63, 25, 0, 33, 30, 43),
	CXTPDockingPaneContextStickerWnd::SPRITEINFO(30, 30, 86, 0, 33, 33),
	CXTPDockingPaneContextStickerWnd::SPRITEINFO(25, 0, 0, 76, 43, 30),
	CXTPDockingPaneContextStickerWnd::SPRITEINFO(0, 25, 90, 33, 30, 43),
	CXTPDockingPaneContextStickerWnd::SPRITEINFO(25, 63, 43, 76, 43, 30),
	CXTPDockingPaneContextStickerWnd::SPRITEINFO(63, 25, 60, 33, 30, 43),
	CXTPDockingPaneContextStickerWnd::SPRITEINFO(30, 30, 86, 76, 33, 33),
	CXTPDockingPaneContextStickerWnd::SPRITEINFO(25, 25, 0, 0, 43, 43),
};

void CXTPDockingPaneContextStickerWnd::DrawTransparent(CDC* pDC , const CPoint& ptDest, const CSize& sz, CBitmap* pBitmap)
{
	CImageList il;
	il.Create(sz.cx, sz.cy, ILC_COLOR24|ILC_MASK, 0, 1);
	il.Add(pBitmap, RGB(0, 0xFF, 0));

	il.Draw(pDC, 0, ptDest, ILD_NORMAL);
}

void CXTPDockingPaneContextStickerWnd::DrawSprite(CDC* pDC, UINT nID, CXTPDockingPaneContextStickerWnd::SPRITEINFO* pSpriteInfo, BOOL bClientBitmap)
{
	CBitmap bmp;
	VERIFY(bmp.LoadBitmap(nID));

	CSize sz(pSpriteInfo->rcSrc.Width(), pSpriteInfo->rcSrc.Height());

	CBitmap bmpSprite;
	bmpSprite.CreateCompatibleBitmap(pDC, sz.cx, sz.cy);

	if (bmpSprite.GetSafeHandle())
	{
		CXTPCompatibleDC dcSprite(pDC, &bmpSprite);
		CXTPCompatibleDC dc(pDC, &bmp);
		dcSprite.BitBlt(0, 0, sz.cx, sz.cy, &dc, pSpriteInfo->rcSrc.left, pSpriteInfo->rcSrc.top, SRCCOPY);
	}

	CPoint ptDest = bClientBitmap? pSpriteInfo->ptDest: CPoint(0, 0);

	DrawTransparent(pDC, ptDest, sz, &bmpSprite);
}


void CXTPDockingPaneContextStickerWnd::OnDraw(CDC* pDC)
{
	XTPCurrentSystemTheme theme = XTPColorManager()->GetCurrentSystemTheme();

	UINT nIDBitmap = theme == xtpSystemThemeOlive? XTP_IDB_DOCKINGPANE_STICKERS_OLIVE:
		theme == xtpSystemThemeSilver? XTP_IDB_DOCKINGPANE_STICKERS_SILVER: XTP_IDB_DOCKINGPANE_STICKERS_BLUE;

	BOOL bClient = (m_typeSticker & xtpPaneStickerClient) == xtpPaneStickerClient;

	if (bClient)
		DrawSprite(pDC, XTP_IDB_DOCKINGPANE_STICKER_CLIENT, &sprites[10]);
	if (m_typeSticker & xtpPaneStickerTop)
		DrawSprite(pDC, nIDBitmap, &sprites[m_selectedSticker == xtpPaneStickerTop? 5: 0], bClient);
	if (m_typeSticker & xtpPaneStickerLeft)
		DrawSprite(pDC, nIDBitmap, &sprites[m_selectedSticker == xtpPaneStickerLeft? 6: 1], bClient);
	if (m_typeSticker & xtpPaneStickerBottom)
		DrawSprite(pDC, nIDBitmap, &sprites[m_selectedSticker == xtpPaneStickerBottom? 7: 2], bClient);
	if (m_typeSticker & xtpPaneStickerRight)
		DrawSprite(pDC, nIDBitmap, &sprites[m_selectedSticker == xtpPaneStickerRight? 8: 3], bClient);
	if (m_typeSticker & xtpPaneStickerCenter)
		DrawSprite(pDC, nIDBitmap, &sprites[m_selectedSticker == xtpPaneStickerCenter? 9: 4]);
}


void CXTPDockingPaneContextStickerWnd::OnPaint()
{
	CPaintDC dcPaint(this); // device context for painting
	CXTPBufferDC dc(dcPaint, CXTPClientRect(this));

	OnDraw(&dc);
}

XTPDockingPaneStickerType CXTPDockingPaneContextStickerWnd::HitTest(CPoint pt)
{
	CXTPClientRect rc(this);
	ScreenToClient(&pt);

	if (!rc.PtInRect(pt))
		return xtpPaneStickerNone;

	CClientDC dcClient(this);

	CBitmap bmp;
	bmp.CreateCompatibleBitmap(&dcClient, rc.Width(), rc.Height());

	CXTPCompatibleDC dc(&dcClient, &bmp);
	dc.FillSolidRect(rc, 0);

	UINT nIDBitmap = XTP_IDB_DOCKINGPANE_STICKERS_BLUE;

	BOOL bClient = (m_typeSticker & xtpPaneStickerClient) == xtpPaneStickerClient;

	if (m_typeSticker & xtpPaneStickerTop) {
		DrawSprite(&dc, nIDBitmap, &sprites[0], bClient); if (dc.GetPixel(pt) != 0) return xtpPaneStickerTop;}
	if (m_typeSticker & xtpPaneStickerLeft) {
		DrawSprite(&dc, nIDBitmap, &sprites[1], bClient); if (dc.GetPixel(pt) != 0) return xtpPaneStickerLeft;}
	if (m_typeSticker & xtpPaneStickerBottom) {
		DrawSprite(&dc, nIDBitmap, &sprites[2], bClient); if (dc.GetPixel(pt) != 0) return xtpPaneStickerBottom;}
	if (m_typeSticker & xtpPaneStickerRight){
		DrawSprite(&dc, nIDBitmap, &sprites[3], bClient); if (dc.GetPixel(pt) != 0) return xtpPaneStickerRight;}
	if (m_typeSticker & xtpPaneStickerCenter) {
		DrawSprite(&dc, XTP_IDB_DOCKINGPANE_STICKER_CLIENT, &sprites[10]); if (dc.GetPixel(pt) != 0) return xtpPaneStickerCenter;}

	return xtpPaneStickerNone;
}

//////////////////////////////////////////////////////////////////////////
// CXTPDockingPaneContext

CXTPDockingPaneContext::CXTPDockingPaneContext()
{
	m_bUseAlphaContext = FALSE;
	m_bUseDockingStickers = FALSE;

	m_pfnSetLayeredWindowAttributes = NULL;

	HMODULE hLib = GetModuleHandle(_T("USER32"));
	if(hLib)
	{
		m_pfnSetLayeredWindowAttributes = (PVOID) ::GetProcAddress(hLib, "SetLayeredWindowAttributes");
	}
}

CXTPDockingPaneContext::~CXTPDockingPaneContext()
{

}


void CXTPDockingPaneContext::Drag(CXTPDockingPaneBase* pPane, CPoint pt)
{
	CRect rect = pPane->GetWindowRect();
	m_ptSticky = m_ptLast = pt;

	AdjustRectangle(rect, pt);

	m_pPane = pPane;
	m_rectDragFrame = rect;
	m_pDC = 0;
	m_pContainer = 0;
	m_bAttach = m_bAttachLast = FALSE;
	m_bFloatable = TRUE;

	m_bUseAlphaContext = m_pManager->IsAlphaDockingContext() && (m_pfnSetLayeredWindowAttributes != NULL);
	m_bUseDockingStickers = m_pManager->IsShowDockingContextStickers() && m_bUseAlphaContext;


	CXTPDockingPaneBaseList lst;
	pPane->FindPane(xtpPaneTypeDockingPane, &lst);
	POSITION pos = lst.GetHeadPosition();
	while (pos)
	{
		CXTPDockingPane* pPane = (CXTPDockingPane*)lst.GetNext(pos);

		if (m_pManager->_OnAction(xtpPaneActionFloating, pPane))
			m_bFloatable = FALSE;

		if (pPane->GetOptions() & xtpPaneNoFloatable)
			m_bFloatable = FALSE;
	}

	InitLoop();

	Track();
}


void CXTPDockingPaneContext::InitLoop()
{
	m_rectLast.SetRectEmpty();
	m_sizeLast.cx = m_sizeLast.cy = 0;
	m_bDitherLast = FALSE;
	m_rectStickerPane.SetRectEmpty();
	m_pStickerPane = NULL;

	if (!m_bUseAlphaContext)
	{
		// handle pending WM_PAINT messages
		MSG msg;
		while (::PeekMessage(&msg, NULL, WM_PAINT, WM_PAINT, PM_NOREMOVE))
		{
			if (!GetMessage(&msg, NULL, WM_PAINT, WM_PAINT))
				return;
			DispatchMessage(&msg);
		}

		// lock window update while dragging
		ASSERT(m_pDC == NULL);
		CWnd* pWnd = CWnd::GetDesktopWindow();
		if (pWnd->LockWindowUpdate())
			m_pDC = pWnd->GetDCEx(NULL, DCX_WINDOW|DCX_CACHE|DCX_LOCKWINDOWUPDATE);
		else
			m_pDC = pWnd->GetDCEx(NULL, DCX_WINDOW|DCX_CACHE);
		ASSERT(m_pDC != NULL);
	}
}
void CXTPDockingPaneContext::CancelLoop()
{
	ReleaseCapture();

	if (m_bUseAlphaContext)
	{
		m_wndContext.DestroyWindow();
		m_wndAttachedTab.DestroyWindow();

		DestroyDockingStickers();

		for (int i = 0; i < 5; i++)
		{
			if (m_rgnStickers[i].GetSafeHandle())
				m_rgnStickers[i].DeleteObject();
		}
	}
	else
	{
		DrawFocusRect(TRUE);    // gets rid of focus rect

		CWnd* pWnd = CWnd::GetDesktopWindow();
		pWnd->UnlockWindowUpdate();
		if (m_pDC != NULL)
		{
			pWnd->ReleaseDC(m_pDC);
			m_pDC = NULL;
		}
	}
}

void CXTPDockingPaneContext::_CreateRectangleRgn(CRgn& rgnResult, CRect rc)
{
	CRgn rgnOutside, rgnInside;
	rgnOutside.CreateRectRgnIndirect(&rc);
	CRect rect = rc;
	rect.DeflateRect(4, 4);
	rect.IntersectRect(rect, rc);
	rgnInside.CreateRectRgnIndirect(rect);
	rgnResult.CreateRectRgn(0, 0, 0, 0);
	rgnResult.CombineRgn(&rgnOutside, &rgnInside, RGN_XOR);
}

void CXTPDockingPaneContext::_CreateRgn(CRgn& rgnResult, CRect rc, BOOL bTabbedRgn, BOOL bRemove)
{
	if (bRemove)
	{
		rgnResult.CreateRectRgn(0, 0, 0, 0);
		return;
	}

	if (bTabbedRgn)
	{
		CSize szTab(min(50, rc.Width() - 5), min(20, rc.Height() / 2));
		CRect rcIntersect, rcTop(rc.left, rc.top, rc.right, rc.bottom - szTab.cy),
			rcBottom(rc.left + 5, rc.bottom - szTab.cy - 4, rc.left + 5 + szTab.cx, rc.bottom);

		CRgn rgnTop, rgnBottom, rgnIntersect;

		rcIntersect.IntersectRect(rcTop, rcBottom);
		rcIntersect.DeflateRect(4, 0);

		_CreateRectangleRgn(rgnTop, rcTop);
		_CreateRectangleRgn(rgnBottom, rcBottom);
		_CreateRectangleRgn(rgnIntersect, rcIntersect);

		rgnResult.CreateRectRgn(0, 0, 0, 0);
		rgnResult.CombineRgn(&rgnBottom, &rgnTop, RGN_OR);
		rgnResult.CombineRgn(&rgnResult, &rgnIntersect, RGN_XOR);
	}
	else
	{
		_CreateRectangleRgn(rgnResult, rc);
	}
}

void CXTPDockingPaneContext::DrawFocusRect(BOOL bRemoveRect)
{
	if (m_bUseAlphaContext)
	{
		ASSERT(bRemoveRect == FALSE);

		CRect rect = (m_pContainer == 0)? m_rectDragFrame: m_rectContainer;

		m_rectLast = rect;

		if (!m_bFloatable && m_pContainer == 0)
		{
			rect.SetRectEmpty();
		}

		if (m_bAttach != m_bAttachLast)
		{
			m_bAttachLast = m_bAttach;
			if (m_bAttach) CreateContextWindow(&m_wndAttachedTab); else m_wndAttachedTab.DestroyWindow();
		}

		if (m_bAttach)
		{
			CSize szTab(min(50, rect.Width() - 5), min(20, rect.Height() / 2));

			rect.bottom -= szTab.cy;
			CRect rcTab(rect.left + 5, rect.bottom, rect.left + 5 + szTab.cx, rect.bottom + szTab.cy);

			m_wndAttachedTab.SetWindowPos(0, rcTab.left, rcTab.top, rcTab.Width(), rcTab.Height(), SWP_NOZORDER|SWP_NOACTIVATE|SWP_SHOWWINDOW);
		}

		if (m_wndContext.GetSafeHwnd() == 0 || CXTPWindowRect(&m_wndContext).Size() != rect.Size() )
		{
			m_wndContext.DestroyWindow();
			CreateContextWindow(&m_wndContext);
		}

		m_wndContext.SetWindowPos(0, rect.left, rect.top, rect.Width(), rect.Height(), SWP_NOZORDER|SWP_NOACTIVATE|SWP_SHOWWINDOW);
	}
	else
	{
		ASSERT(m_pDC != NULL);
		// determine new rect and size
		CRect rect = (m_pContainer == 0)? m_rectDragFrame: m_rectContainer;

		if (!m_bFloatable && m_pContainer == 0)
			rect.SetRectEmpty();

		// first, determine the update region and select it
		CRgn rgnNew, rgnLast, rgnUpdate;

		_CreateRgn(rgnNew, rect, m_bAttach, bRemoveRect);
		_CreateRgn(rgnLast, m_rectLast, m_bAttachLast);

		rgnUpdate.CreateRectRgn(0, 0, 0, 0);
		rgnUpdate.CombineRgn(&rgnLast, &rgnNew, RGN_XOR);

		m_rectLast = rect;
		m_bAttachLast = m_bAttach;

		// draw into the update/new region
		m_pDC->SelectClipRgn(&rgnUpdate);
		m_pDC->GetClipBox(&rect);
		CBrush* pBrushOld = m_pDC->SelectObject(CDC::GetHalftoneBrush());
		m_pDC->PatBlt(rect.left, rect.top, rect.Width(), rect.Height(), PATINVERT);
		// cleanup DC

		if (pBrushOld != NULL)
			m_pDC->SelectObject(pBrushOld);

		m_pDC->SelectClipRgn(NULL);
	}
}

BOOL CXTPDockingPaneContext::CanDock(CRect rc, CPoint pt, CXTPDockingPaneBase* pPane, BOOL bInside)
{
	CRect rectContainer = pPane->GetWindowRect();
	int nSize = 20;

	if (rectContainer.Height() == 0 || rectContainer.Width() == 0)
		return FALSE;

	if (bInside && !rc.PtInRect(pt))
		return FALSE;

	if (!bInside && !CRect(rc.left - nSize, rc.top - nSize, rc.right + nSize, rc.bottom + nSize).PtInRect(pt))
		return FALSE;

	int nInside = bInside? 1: -1;
	BOOL bFound = FALSE;


	if (abs(pt.y - rc.top) < nSize && nInside * (pt.y - rc.top) >= 0)
	{
		rectContainer.SetRect(m_rectContainer.left, m_rectContainer.top, m_rectContainer.right, m_rectContainer.CenterPoint().y);
		m_containDirection = xtpPaneDockTop;
		nSize = abs(pt.y - rc.top);
		bFound = TRUE;
	}

	if (abs(rc.bottom - pt.y) < nSize && nInside * (rc.bottom - pt.y) >= 0)
	{
		rectContainer.SetRect(m_rectContainer.left, m_rectContainer.CenterPoint().y, m_rectContainer.right, m_rectContainer.bottom);
		m_containDirection = xtpPaneDockBottom;
		nSize = abs(rc.bottom - pt.y);
		bFound = TRUE;
	}

	if (abs(pt.x - rc.left) < nSize && nInside * (pt.x - rc.left) >= 0)
	{
		rectContainer.SetRect(m_rectContainer.left, m_rectContainer.top, m_rectContainer.CenterPoint().x, m_rectContainer.bottom);
		m_containDirection = xtpPaneDockLeft;
		nSize = abs(pt.x - rc.left);
		bFound = TRUE;
	}

	if (abs(rc.right - pt.x) < nSize && nInside * (rc.right- pt.x) >= 0)
	{
		rectContainer.SetRect(m_rectContainer.CenterPoint().x, m_rectContainer.top, m_rectContainer.right, m_rectContainer.bottom);
		m_containDirection = xtpPaneDockRight;
		nSize = abs(rc.right - pt.x);
		bFound = TRUE;
	}

	if (bFound)
	{
		if (!IsAllowDockingTo(pPane, m_containDirection))
			return FALSE;

		rectContainer = m_pManager->_CalculateResultDockingRect(m_pPane, m_containDirection, pPane);


		m_rectContainer = rectContainer;
		m_pContainer = pPane;
	}
	return bFound;
}

BOOL CXTPDockingPaneContext::IsAllowDockingTo(CXTPDockingPaneBase* pPane, XTPDockingPaneDirection direction)
{
	CXTPDockingPaneBaseList lst;
	m_pPane->FindPane(xtpPaneTypeDockingPane, &lst);
	POSITION pos = lst.GetHeadPosition();
	while (pos)
	{
		CXTPDockingPane* p = (CXTPDockingPane*)lst.GetNext(pos);

		if (m_pManager->_OnAction(xtpPaneActionDocking, p, pPane, direction))
		{
			return FALSE;
		}
	}
	return TRUE;
}

BOOL CXTPDockingPaneContext::IsAllowAttachTo(CXTPDockingPaneBase* pPane)
{

	CXTPDockingPaneBaseList lst;
	m_pPane->FindPane(xtpPaneTypeDockingPane, &lst);
	POSITION pos = lst.GetHeadPosition();
	while (pos)
	{
		CXTPDockingPane* p = (CXTPDockingPane*)lst.GetNext(pos);

		if (m_pManager->_OnAction(xtpPaneActionAttaching, p, pPane))
		{
			return FALSE;
		}
	}
	return TRUE;
}


void CXTPDockingPaneContext::FindContainer(CPoint pt)
{
	CXTPDockingPaneInfoList* pList = &m_pManager->GetPaneList();
	POSITION pos = pList->GetHeadPosition();
	while (pos)
	{
		CXTPDockingPaneBase* pPane = pList->GetNext(pos)->GetContainer();
		if (pPane == NULL)
			continue;

		ASSERT(pPane->GetType() == xtpPaneTypeTabbedContainer);

		if (pPane->GetContainer() == 0 || pPane->GetContainer()->GetType() != xtpPaneTypeSplitterContainer)
			continue;

		if (! (::GetWindowLong(((CXTPDockingPaneTabbedContainer*)pPane)->GetSafeHwnd(), GWL_STYLE) & WS_VISIBLE))
			continue;

		CRect rcClient = m_rectContainer = pPane->GetWindowRect();

		if (m_pPane->ContainPane(pPane))
			continue;

		if (((CXTPDockingPaneTabbedContainer*)pPane)->CanAttach(rcClient, pt))
		{
			if (IsAllowAttachTo(pPane))
			{
				m_pContainer = pPane;
				m_bAttach = TRUE;
				return;
			}
		}

		if (m_bUseDockingStickers)
		{
			if (rcClient.PtInRect(pt))
			{
				m_rectStickerPane = m_rectContainer;
				m_pStickerPane = pPane;
				return;
			}
		} else
		{
			if (CanDock(rcClient, pt, pPane))
				return;
		}
	}

	if (m_bUseDockingStickers)
	{
		if (m_rectStickerPane.IsRectEmpty())
		{
			CXTPDockingPaneBase* pPane = m_pManager->GetClientPane();
			if (pPane->GetWindowRect().PtInRect(pt))
			{
				m_rectStickerPane = pPane->GetWindowRect();
				m_pStickerPane = pPane;
				return;
			}

			pPane = m_pManager->GetTopPane();
			if (pPane->GetWindowRect().PtInRect(pt))
			{
				m_rectStickerPane = pPane->GetWindowRect();
			}
		}
	}
	else
	{
		CXTPDockingPaneBase* pPane = m_pManager->GetClientPane();
		m_rectContainer = pPane->GetWindowRect();
		if (CanDock(pPane->GetWindowRect(), pt, pPane))
			return;

		pPane = m_pManager->GetTopPane();
		m_rectContainer = pPane->GetWindowRect();
		if (CanDock(pPane->GetWindowRect(), pt, pPane, FALSE))
			return;
	}
}


void CXTPDockingPaneContext::Move(CPoint pt)
{
	CPoint ptOffset = pt - m_ptLast;

	CRect rectDragFrame(m_rectDragFrame);
	rectDragFrame.OffsetRect(ptOffset);

	if (CRect().IntersectRect(rectDragFrame, CXTPDrawHelpers::GetWorkArea(m_rectDragFrame)))
	{
		m_rectDragFrame = rectDragFrame;
	}


	m_pContainer = 0;
	m_bAttach = FALSE;
	m_rectStickerPane.SetRectEmpty();
	m_pStickerPane = NULL;

	if (GetKeyState(VK_CONTROL) >= 0)
		FindContainer(pt);

	m_ptLast = pt;

	if (m_bUseDockingStickers)
	{
		UpdateDockingStickers();

		BOOL bFound = FALSE;
		POSITION pos = m_lstStickers.GetTailPosition();
		while (pos)
		{

			CXTPDockingPaneContextStickerWnd* pSticker = m_lstStickers.GetPrev(pos);
			XTPDockingPaneStickerType selectedSticker = xtpPaneStickerNone;

			if (!bFound)
			{
				XTPDockingPaneStickerType ht = pSticker->HitTest(pt);
				if (ht != xtpPaneStickerNone)
				{
					if ((pSticker->m_typeSticker & xtpPaneStickerClient) == xtpPaneStickerClient)
					{
						m_pContainer = m_pStickerPane;
						m_rectContainer = m_rectStickerPane;
					}
					else
					{
						m_pContainer = m_pManager->GetTopPane();
						m_rectContainer = m_pContainer->GetWindowRect();
					}

					switch (ht)
					{
					case xtpPaneStickerTop:
						m_rectContainer.bottom = m_rectContainer.CenterPoint().y;
						m_containDirection = xtpPaneDockTop;
						break;
					case xtpPaneStickerBottom:
						m_rectContainer.top = m_rectContainer.CenterPoint().y;
						m_containDirection = xtpPaneDockBottom;
						break;
					case xtpPaneStickerLeft:
						m_rectContainer.right = m_rectContainer.CenterPoint().x;
						m_containDirection = xtpPaneDockLeft;
						break;
					case xtpPaneStickerRight:
						m_rectContainer.left = m_rectContainer.CenterPoint().x;
						m_containDirection = xtpPaneDockRight;
						break;
					case xtpPaneStickerCenter:
						m_bAttach = TRUE;
					}

					BOOL bAllow = TRUE;

					if ((ht != xtpPaneStickerCenter) &&  !IsAllowDockingTo(m_pContainer, m_containDirection))
					{
						m_pContainer = NULL;
						bAllow = FALSE;
					}

					if ((ht == xtpPaneStickerCenter) && !IsAllowAttachTo(m_pContainer))
					{
						m_pContainer = NULL;
						m_bAttach = FALSE;
						bAllow = FALSE;
					}

					if (bAllow)
					{
						if (ht != xtpPaneStickerCenter)
							m_rectContainer = m_pManager->_CalculateResultDockingRect(m_pPane, m_containDirection, m_pContainer);

						selectedSticker = ht;
					}

					bFound = TRUE;
				}
			}
			if (pSticker->m_selectedSticker != selectedSticker)
			{
				pSticker->m_selectedSticker = selectedSticker;
				pSticker->Invalidate(FALSE);
			}
		}
	}


	DrawFocusRect();
}

void CXTPDockingPaneContext::Track()
{
	// don't handle if capture already set
	if (::GetCapture() != NULL)
		return;

	CWnd* pCapture = m_pPane->m_pParentFrame;
	ASSERT(pCapture);
	// set capture to the window which received this message
	pCapture->SetCapture();
	ASSERT(pCapture == CWnd::GetCapture());

	BOOL bAccept = FALSE;
	// get messages until capture lost or cancelled/accepted
	while (CWnd::GetCapture() == pCapture)
	{
		MSG msg;

		if (!::GetMessage(&msg, NULL, 0, 0))
		{
			AfxPostQuitMessage((int)msg.wParam);
			break;
		}

		if (msg.message == WM_LBUTTONUP)
		{
			bAccept = TRUE;
			break;
		} else
		if (msg.message == WM_MOUSEMOVE )
		{
			CSize sz = m_ptSticky - CPoint(msg.pt);
			if (abs(sz.cx) > 4 || abs(sz.cy) > 4 )
			{
				Move(msg.pt);
				m_ptSticky = CPoint(0, 0);
			}
		} else

		if (msg.message == WM_KEYDOWN || msg.message == WM_KEYUP)
		{
			if (msg.wParam == VK_CONTROL)
			{
				Move(m_ptLast);
				m_ptSticky = CPoint(0, 0);
			}
			if (msg.wParam == VK_ESCAPE)
			{
				break;
			}
		} else
			DispatchMessage(&msg);
	}

	CancelLoop();

	if (bAccept && m_ptSticky == CPoint(0, 0))
	{
		XTPDockingPaneAction actionNotify = xtpPaneActionFloated;

		CXTPDockingPaneBaseList lst;
		m_pPane->FindPane(xtpPaneTypeDockingPane, &lst);

		if (m_bAttach)
		{
			ASSERT(m_pContainer->GetType() == xtpPaneTypeTabbedContainer);
			m_pManager->AttachPane(m_pPane, m_pContainer);
			actionNotify = xtpPaneActionAttached;
		}
		else
		{
			if (m_pContainer)
			{
				m_pManager->DockPane(m_pPane, m_pManager->GetRTLDirection(m_containDirection), m_pContainer);
				actionNotify = xtpPaneActionDocked;
			}
			else if (m_bFloatable)
			{
				if (m_rectDragFrame.top < 0)
					m_rectDragFrame.OffsetRect(0, -m_rectDragFrame.top);

				if (m_pPane->GetType() == xtpPaneTypeSplitterContainer)
					m_pPane->m_pParentFrame->MoveWindow(m_rectDragFrame);
				else
					m_pManager->FloatPane(m_pPane, m_rectDragFrame);
			} else
			{
				bAccept = FALSE;
			}
		}

		if (bAccept)
		{
			POSITION pos = lst.GetHeadPosition();
			while (pos)
			{
				CXTPDockingPane* pPane = (CXTPDockingPane*)lst.GetNext(pos);
				m_pManager->_OnAction(actionNotify, pPane);
			}
		}
	}
}


void CXTPDockingPaneContext::DestroyDockingStickers()
{
	while (!m_lstStickers.IsEmpty())
	{
		CWnd* pWnd = m_lstStickers.RemoveHead();
		pWnd->DestroyWindow();
		delete pWnd;
	}
}

void CXTPDockingPaneContext::IncludeRgnPart(CRgn* pRgn, int x1, int y, int x2)
{
	if (x1 < x2)
	{
		CRgn rgnExclude;
		rgnExclude.CreateRectRgn(x1, y, x2, y + 1);
		pRgn->CombineRgn(pRgn, &rgnExclude, RGN_OR);
	}
}

void CXTPDockingPaneContext::RegionFromBitmap(CRgn* pRgn, CDC* pDC, CRect rc)
{
	CSize sz = rc.Size();

	pRgn->CreateRectRgn(0, 0, 0, 0);

	for (int y = 0; y < sz.cy; y++)
	{
		int nStart = 0, x = 0;
		BOOL bTransparent = TRUE;

		while (x < sz.cx)
		{
			BOOL bTransparentPixel = pDC->GetPixel(x, y) == 0;

			if (bTransparent && !bTransparentPixel)
			{
				nStart = x;
				bTransparent = FALSE;
			}
			else if (!bTransparent && bTransparentPixel)
			{
				IncludeRgnPart(pRgn, nStart, y, x);
				bTransparent = TRUE;
			}
			x ++;
		}
		if (!bTransparent)
		{
			IncludeRgnPart(pRgn, nStart, y, x);
		}
	}
}

int CXTPDockingPaneContext::StickerToOffset(XTPDockingPaneStickerType sticker)
{
	if ((sticker & xtpPaneStickerClient) == xtpPaneStickerClient)   return 4;
	if (sticker == xtpPaneStickerTop) return 0;
	if (sticker == xtpPaneStickerLeft) return 1;
	if (sticker == xtpPaneStickerBottom) return 2;
	if (sticker == xtpPaneStickerRight) return 3;

	ASSERT(FALSE);
	return 0;
}

CXTPDockingPaneContextStickerWnd* CXTPDockingPaneContext::CreateNewSticker(CRect rc, XTPDockingPaneStickerType typeSticker)
{
	CXTPDockingPaneContextStickerWnd* pWnd = new CXTPDockingPaneContextStickerWnd();
	m_lstStickers.AddTail(pWnd);

	pWnd->CreateEx(WS_EX_TOOLWINDOW, AfxRegisterWndClass(NULL, ::LoadCursor(NULL, IDC_ARROW)),
		0, WS_POPUP, CRect(0, 0, 0, 0), m_pManager->GetSite(), 0);

	pWnd->m_typeSticker = typeSticker;

	CRgn& rgn = m_rgnStickers[StickerToOffset(typeSticker)];

	if (!rgn.GetSafeHandle())
	{
		CClientDC dcClient(pWnd);
		CBitmap bmp;
		bmp.CreateCompatibleBitmap(&dcClient, rc.Width(), rc.Height());

		if (bmp.GetSafeHandle())
		{
			CXTPCompatibleDC dc(&dcClient, &bmp);
			dc.FillSolidRect(rc, RGB(0, 0, 0));
			pWnd->OnDraw(&dc);

			RegionFromBitmap(&rgn, &dc, rc);
		}
	}

	HRGN hRgn = ::CreateRectRgn(0, 0, 0, 0);
	::CombineRgn(hRgn, (HRGN)rgn, NULL, RGN_COPY);

	pWnd->SetWindowRgn(hRgn, FALSE);
	pWnd->SetWindowPos(0, rc.left, rc.top, rc.Width(), rc.Height(), SWP_NOZORDER|SWP_NOACTIVATE|SWP_SHOWWINDOW);

	return pWnd;
}


void CXTPDockingPaneContext::UpdateDockingStickers()
{
	if (m_rectStickerPane.IsRectEmpty())
	{
		DestroyDockingStickers();
		return;
	}

	CSize sz(93, 93);
	CRect rc(m_rectStickerPane.CenterPoint(), sz);
	rc.OffsetRect(- sz.cx / 2, - sz.cy / 2);

	if (m_lstStickers.IsEmpty() ||
		((CXTPWindowRect(m_lstStickers.GetHead()) != rc) && (m_pStickerPane || m_lstStickers.GetCount() != 4)))
	{
		DestroyDockingStickers();

		if (m_pStickerPane)
		{
			CreateNewSticker(rc, m_pStickerPane == m_pManager->GetClientPane()? xtpPaneStickerClient: xtpPaneStickerPane);
		}

		CXTPDockingPaneBase* pPane = m_pManager->GetTopPane();
		CRect rcWindow = pPane->GetWindowRect();

		CreateNewSticker(CRect(CPoint(rcWindow.CenterPoint().x - 43 / 2, rcWindow.top + 16), CSize(43, 30)), xtpPaneStickerTop);
		CreateNewSticker(CRect(CPoint(rcWindow.left + 16, rcWindow.CenterPoint().y - 43/ 2), CSize(30, 43)), xtpPaneStickerLeft);

		CreateNewSticker(CRect(CPoint(rcWindow.CenterPoint().x - 43 / 2, rcWindow.bottom - 16 - 30), CSize(43, 30)), xtpPaneStickerBottom);
		CreateNewSticker(CRect(CPoint(rcWindow.right - 30 - 16, rcWindow.CenterPoint().y - 43/ 2), CSize(30, 43)), xtpPaneStickerRight);
	}

	ASSERT(m_lstStickers.GetCount() < 6);
}


void CXTPDockingPaneContext::CreateContextWindow(CXTPDockingPaneContextAlphaWnd* pWnd)
{
	ASSERT(m_bUseAlphaContext);

	if (pWnd->GetSafeHwnd())
		return;

	pWnd->CreateEx(WS_EX_LAYERED|WS_EX_TOPMOST|WS_EX_TOOLWINDOW, AfxRegisterWndClass(NULL, ::LoadCursor(NULL, IDC_ARROW)),
		0, WS_POPUP, CRect(0, 0, 0, 0), m_pManager->GetSite(), 0);

	if(m_pfnSetLayeredWindowAttributes)
	{
		((PFNSETLAYEREDWINDOWATTRIBUTES)m_pfnSetLayeredWindowAttributes)
			(pWnd->m_hWnd, 0, 100, LWA_ALPHA);
	}
}
