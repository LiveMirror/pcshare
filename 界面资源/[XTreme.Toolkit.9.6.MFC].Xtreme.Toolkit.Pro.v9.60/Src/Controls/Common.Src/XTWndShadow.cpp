// XTWndShadow.cpp: implementation of the CXTWndShadow class.
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

#include "XTGlobal.h"
#include "XTWndHook.h"
#include "XTMemDC.h"
#include "XTWndShadow.h"
#include "XTThemeManager.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CXTShadowsManager CXTShadowsManager::m_instance;

#define  XT_WM_PARENTPOSCHANGED (WM_USER + 500)
#define  XT_WM_DESTROY          (WM_USER + 501)
#define  TID_DRAWSHADOW 1

void CXTShadowsManager::CShadowList::AddShadow(CXTShadowWnd* pShadow)
{
	pShadow->LongShadow(this);
	AddTail(pShadow);
}

void CXTShadowsManager::CShadowList::RemoveShadow(CXTShadowWnd* pShadow)
{
	POSITION pos = Find(pShadow);
	ASSERT(pos);
	RemoveAt(pos);
}

class CXTShadowHook : public CXTWndHook
{
public:
	CXTShadowHook(CXTShadowWnd* pShadow)
	{
		m_pShadow = pShadow;
	}
	LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
	{
		LRESULT nRes = CXTWndHook::WindowProc(message, wParam, lParam);
		m_pShadow->OnHookMessage(message, wParam, lParam);
		return nRes;
	}
	CXTShadowWnd* m_pShadow;
};

CXTShadowsManager::CXTShadowsManager(void)
{
	HMODULE hLib = GetModuleHandle(_T("USER32"));
	if (hLib)
	{
		UpdateLayeredWindow = (LPFNUPDATELAYEREDWINDOW) GetProcAddress(hLib, "UpdateLayeredWindow");
	}
}

CXTShadowsManager::~CXTShadowsManager(void)
{
	ASSERT(m_lstShadow.IsEmpty());

	while (!m_lstPool.IsEmpty())
	{
		delete m_lstPool.RemoveHead();
	}
}

void CXTShadowsManager::SetShadow(HWND hWnd, const CRect& rcExclude )
{
	CreateShadow(TRUE, hWnd, rcExclude);
	CreateShadow(FALSE, hWnd, rcExclude);
}


void CXTShadowsManager::SetShadow(CRect rcWnd, HWND hWnd)
{
	CreateShadow(TRUE, rcWnd, CXTPEmptyRect(), hWnd);
	CreateShadow(FALSE, rcWnd, CXTPEmptyRect(), hWnd);
}

CXTShadowWnd* CXTShadowsManager::CreateShadow(BOOL bHoriz, CRect rcWnd, CRect rcExclude, HWND hWnd)
{
	CXTShadowWnd* pShadow = m_lstPool.IsEmpty()?
		new CXTShadowWnd: m_lstPool.RemoveHead();

	ASSERT(pShadow->m_pHook == 0 && pShadow->m_hwndHook == 0);
	pShadow->m_hwndHook = hWnd;
	pShadow->m_rcExclude = rcExclude;

	pShadow->Create(bHoriz, rcWnd);

	pShadow->m_pHook = new CXTShadowHook(pShadow);
	pShadow->m_pHook->HookWindow(hWnd);


	m_lstShadow.AddShadow(pShadow);
	pShadow->ExcludeRect(rcExclude);

	::SetWindowLong(pShadow->m_hWnd, GWL_HWNDPARENT, 0);

	if (pShadow->m_bPseudoShadow)
	{
		pShadow->SetWindowPos(&CWnd::wndTop, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
		SAFE_DELETE(pShadow->m_pShadowDC);
		CWnd::GetDesktopWindow()->Invalidate(FALSE);
		pShadow->SetTimer(TID_DRAWSHADOW, 1, NULL);
	}
	else
	{
		pShadow->SetWindowPos(&CWnd::wndTop, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW | SWP_NOACTIVATE);
	}
	::SetWindowLongPtr(pShadow->m_hWnd, GWL_HWNDPARENT, (LONG_PTR)AfxGetMainWnd()->m_hWnd);

	return pShadow;
}

CXTShadowWnd* CXTShadowsManager::CreateShadow(BOOL bHoriz, HWND hWnd, CRect rcExclude)
{
	CXTPWindowRect rcWnd(hWnd);

	CXTShadowWnd* pShadow = CreateShadow(bHoriz, rcWnd, rcExclude, hWnd);
	pShadow->m_bAutoPosition = TRUE;

	return pShadow;

}

void CXTShadowsManager::DestroyShadow(CXTShadowWnd* pWnd)
{
	SAFE_DELETE(pWnd->m_pHook);
	SAFE_DELETE(pWnd->m_pShadowDC);
	pWnd->m_bAutoPosition = FALSE;
	pWnd->m_hwndHook = 0;

	if (pWnd->m_hWnd)
	{
		pWnd->ShowWindow(SW_HIDE);
	}

	m_lstShadow.RemoveShadow(pWnd);
	m_lstPool.AddTail(pWnd);
}

CXTShadowWnd::CXTShadowWnd()
{
	m_pHook = 0;
	m_bAutoPosition = FALSE;
	m_hwndHook = 0;
	m_pShadowDC = 0;
}

BEGIN_MESSAGE_MAP(CXTShadowWnd, CWnd)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_NCHITTEST()
	ON_MESSAGE_VOID(XT_WM_PARENTPOSCHANGED, OnParentPosChanged)
	ON_MESSAGE_VOID(XT_WM_DESTROY, OnSelfDestroy)
	ON_WM_TIMER()
END_MESSAGE_MAP()

int CXTShadowWnd::OnHookMessage(UINT nMessage, WPARAM& , LPARAM& lParam)
{
	if (nMessage == WM_DESTROY)
	{
		PostMessage(XT_WM_DESTROY);
	}
	if (nMessage == WM_WINDOWPOSCHANGED)
	{
		LPWINDOWPOS lpwp = (LPWINDOWPOS) lParam;
		if ((lpwp->flags & (SWP_NOMOVE | SWP_NOSIZE)) !=
				(SWP_NOMOVE | SWP_NOSIZE))
		{
			ASSERT(m_hwndHook);
			PostMessage(XT_WM_PARENTPOSCHANGED);
		}
	}
	return 0;
}

void CXTShadowWnd::OnSelfDestroy()
{
	XTShadowsManager()->DestroyShadow(this);
}
void CXTShadowWnd::OnParentPosChanged()
{
	CWnd* pWnd = CWnd::FromHandle(m_hwndHook);

	if (m_hwndHook && pWnd)
	{
		if (m_bAutoPosition)
		{

			int nWidth = m_nWidth;
			CXTPWindowRect rcWindow(pWnd);
			CRect rcShadow;
			if (m_bHoriz) rcShadow.SetRect(rcWindow.left + nWidth, rcWindow.bottom, rcWindow.right + nWidth, rcWindow.bottom + nWidth);
			else rcShadow.SetRect(rcWindow.right, rcWindow.top + nWidth, rcWindow.right + nWidth, rcWindow.bottom);


			CXTPWindowRect rc(this);
			if (rcShadow != rc)
			{
				MoveWindow(rcShadow, TRUE);
				LongShadow(&CXTShadowsManager::m_instance.m_lstShadow);
				SetWindowRgn(NULL, FALSE);
				ExcludeRect(m_rcExclude);

				if (m_bPseudoShadow)
				{
					SetWindowPos(0, 0, 0, 0, 0, SWP_HIDEWINDOW | (XTThemeManager()->GetTheme() != xtThemeDefault? 0: SWP_NOZORDER) | SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
					SAFE_DELETE(m_pShadowDC);
					GetDesktopWindow()->Invalidate(FALSE);
					SetTimer(TID_DRAWSHADOW, 1, NULL);
				}
			}

		}
		else
			LongShadow(&CXTShadowsManager::m_instance.m_lstShadow);
	}
}

void CXTShadowWnd::OnTimer(UINT_PTR nID)
{
	if (nID == TID_DRAWSHADOW)
	{
		KillTimer(TID_DRAWSHADOW);
		if (m_hwndHook && m_hWnd)
		{
			SetWindowPos(0, 0, 0, 0, 0, SWP_SHOWWINDOW | SWP_NOZORDER | SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
		}
		return;
	}
	CWnd::OnTimer(nID);

}

BOOL CXTShadowWnd::Create(BOOL bHoriz, CRect rcWindow)
{
	m_bAlphaShadow = XTShadowsManager()->AlphaShadow();
	m_bPseudoShadow = XTShadowsManager()->PseudoShadow();

	if (m_hWnd == 0)
	{
		int nFlags = 0;

#ifndef NOALPHASHADOW
		if (m_bAlphaShadow) nFlags = 0x80000;
#endif
		CString strClass = ::AfxRegisterWndClass(0, ::LoadCursor(0, IDC_ARROW));
		if(!CreateEx(WS_EX_TOOLWINDOW|nFlags, strClass, 0, WS_POPUP, CXTPEmptyRect(), AfxGetMainWnd(),  0))
			return FALSE;
	}

	SetWindowRgn(NULL, FALSE);

	m_nWidth = m_bAlphaShadow || m_bPseudoShadow ? 4 : 2;

	m_bHoriz = bHoriz;

	CRect rcShadow;
	if (rcWindow.IsRectEmpty())
		rcShadow.SetRectEmpty();
	else
	{
		if (bHoriz) rcShadow.SetRect(rcWindow.left + m_nWidth, rcWindow.bottom, rcWindow.right + m_nWidth, rcWindow.bottom + m_nWidth);
			else rcShadow.SetRect(rcWindow.right, rcWindow.top + m_nWidth, rcWindow.right + m_nWidth, rcWindow.bottom);
	}
	MoveWindow(rcShadow, FALSE);
	return TRUE;
}

UINT CXTShadowWnd::OnNcHitTest(CPoint)
{
	return (UINT)HTTRANSPARENT;
}

void CXTShadowWnd::OnPaint()
{
	CPaintDC dc(this);
	CXTPClientRect rcClient(this);

	if (m_bPseudoShadow) DrawPseudoShadow(&dc, rcClient);
	else dc.FillSolidRect(rcClient, m_bAlphaShadow? 0: GetXtremeColor(COLOR_3DSHADOW));
}

// utility functions
static inline int BoundValue(int iValue, int min, int max)
{
	return ((iValue > max) ? max : ((iValue < min) ? min : iValue));
}

static inline COLORREF AlphaPixel(COLORREF crPixel, int i)
{
	return RGB(
		BoundValue(GetRValue(crPixel) - i, 0, 255),
		BoundValue(GetGValue(crPixel) - i, 0, 255),
		BoundValue(GetBValue(crPixel) - i, 0, 255));
}

void CXTShadowWnd::DrawPseudoShadow(CDC* pDC, CRect& rcClient)
{
	if (!m_pShadowDC)
	{
		CreatePseudoShadow();
	}
	ASSERT(m_pShadowDC);
	pDC->BitBlt(0, 0, rcClient.Width(), rcClient.Height(), m_pShadowDC, 0, 0, SRCCOPY);
}

void CXTShadowWnd::ComputePseudoShadow(CDC* pDC, CRect& rcShadow)
{
	int x, y;

	if (!m_bHoriz)
	{
		ASSERT(rcShadow.Width() == 4);

		for (x = 0; x < 4; x++)
		{
			for (y = 0; y < 4; y++)
			{
				pDC->SetPixel(3 - x, y, AlphaPixel(pDC->GetPixel(3 - x, y), 3 * (x + 1) * (y + 1)));
			}
		}

		for (x = 0; x < 4; ++x)
		{
			for (y = 4; y < rcShadow.bottom; ++y)
			{
				pDC->SetPixel(x, y, AlphaPixel(pDC->GetPixel(x,y), 15 * (4 - x)));
			}
		}
	}
	else
	{
		ASSERT(rcShadow.Height() == 4);

		// draw shadow on left-bottom
		for (x = 0; x < 4; x++)
		{
			for (y = 0; y < 4; y++)
			{
				pDC->SetPixel(x, 3 - y, AlphaPixel(pDC->GetPixel(x, 3 - y), 3 * (x + 1) * (y + 1)));
			}
		}

		// draw shadow on bottom
		for (y = 0; y < 4; ++y)
		{
			for (x = 4; x < rcShadow.right - 4; x++)
				pDC->SetPixel(x, y, AlphaPixel(pDC->GetPixel(x, y), 15 * (4 - y)));
		}

		// draw shadow on right-bottom
		for (x = 0; x < 4; x++)
		{
			for (y = 0; y < 4; y++)
			{
				pDC->SetPixel(rcShadow.right - x - 1 , 3 - y, AlphaPixel(pDC->GetPixel(rcShadow.right - x - 1, 3 - y), 3 * (x + 1) * (y + 1)));
			}
		}
	}
}

void CXTShadowWnd::CreatePseudoShadow()
{
	ASSERT(!m_pShadowDC);
	SAFE_DELETE( m_pShadowDC );

	CXTPWindowRect rc(this);

	CWindowDC dcScreen(NULL); // Take snapshot of relevant screen areas
	m_pShadowDC = new CXTMemDC(&dcScreen, rc, (COLORREF)-1);
	m_pShadowDC->Discard();
	m_pShadowDC->FromDC();

	rc.OffsetRect(-rc.TopLeft());
	ComputePseudoShadow(m_pShadowDC, rc);
}

BOOL CXTShadowWnd::OnEraseBkgnd(CDC* )
{
	return TRUE;
}

BOOL CXTShadowWnd::ExcludeRect(CRect rcExclude)
{
	CXTPWindowRect rcWindow(this);
	CRect rcIntersect;
	if (rcIntersect.IntersectRect(rcWindow, rcExclude))
	{
		CXTPClientRect rcClient(this);
		HRGN hrgnClip = ::CreateRectRgnIndirect(&rcClient);

		rcIntersect.OffsetRect(-rcWindow.TopLeft());
		HRGN hrgnIntersect = ::CreateRectRgnIndirect(&rcIntersect);
		CombineRgn(hrgnClip, hrgnClip, hrgnIntersect, RGN_DIFF);
		DeleteObject(hrgnIntersect);

		if (!SetWindowRgn(hrgnClip, FALSE))
			DeleteObject(hrgnClip);
	}
	return TRUE;
}

void CXTShadowWnd::LongShadow(CXTShadowsManager::CShadowList* pList)
{
	CXTPWindowRect rcWindow(this);

	POSITION pos = pList->GetHeadPosition();
	while(pos)
	{
		CXTShadowWnd* pWnd = pList->GetNext(pos);
		if (m_bHoriz == !pWnd->m_bHoriz)
		{
			CXTPWindowRect rc(pWnd);
			if (m_bHoriz == FALSE)
			{
				if (rcWindow.top == rc.bottom -1 && rcWindow.right < rc.right && rcWindow.right > rc.left)
				{
					rcWindow.top -= 2 * rc.Height();
					MoveWindow(rcWindow);
				}
			} else
			{

				if (rcWindow.left == rc.right - 1 && rcWindow.top > rc.top && rcWindow.bottom < rc.bottom)
				{
					rcWindow.left -= 2 * rc.Width() + 1;
					MoveWindow(rcWindow);
				}
			}
		}
	}
}

void CXTShadowWnd::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	if (!cx || !cy || !m_bAlphaShadow || !XTShadowsManager()->UpdateLayeredWindow)
		return;

	BLENDFUNCTION bf;
	bf.BlendOp = AC_SRC_OVER;
	bf.BlendFlags = 0;
	bf.SourceConstantAlpha = 255;
	bf.AlphaFormat = 0x01;

	POINT pt = {0, 0};
	int x, y;
	HBITMAP hbitmap;
	BITMAPINFO bmi;

	CClientDC cDC(this);
	CDC dc;
	dc.CreateCompatibleDC(&cDC);

	// zero the memory for the bitmap info
	ZeroMemory(&bmi, sizeof(BITMAPINFO));

	bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmi.bmiHeader.biWidth = cx;
	bmi.bmiHeader.biHeight = cy;
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biBitCount = 32;         // four 8-bit components
	bmi.bmiHeader.biCompression = BI_RGB;
	bmi.bmiHeader.biSizeImage = cx * cy * 4;

	VOID *pvBits;

	// create our DIB section and select the bitmap into the dc
	hbitmap = CreateDIBSection(cDC, &bmi, DIB_RGB_COLORS, &pvBits, NULL, 0x0);

	if (pvBits == NULL)
		return;

	typedef unsigned int UINT32;

	UINT nColor = 0;

	if (!m_bHoriz)
	{
		ASSERT( cx == 4);

		for (x = 0; x < 4; x++)
		{
			for (y = 0; y < 4; y++)
			{
				int i = (3 - x + (cy - 1 - y) * cx);
				if (i > -1)
				{
					((UINT32*)pvBits)[i] = (UINT32)3 * (x + 1) * (y + 1) * 0x1000000;
				}
			}
		}

		for (x = 3; x >= 0; x--)
		{
			nColor += 0x0F000000;
			for (y = 0; y < cy - 4; y++)
			{
				int i = (x + y * cx);
				if (i > -1)
				{
					((UINT32*)pvBits)[i] = nColor;
				}
			}
		}
	}
	else
	{
		ASSERT( cy == 4);

		for (x = 0; x < 4; x++)
		{
			for (y = 0; y < 4; y++)
			{
				int i = (x + y * cx);
				if (i > -1)
				{
					((UINT32 *)pvBits)[i] = (UINT32)3 * (x + 1) * (y + 1) * 0x1000000;
				}
			}
		}

		for (y = 0; y < 4; y++)
		{
			nColor += 0x0F000000;
			for (x = 4; x < cx - 4; x++)
			{
				int i = (x + y * cx);
				if (i > -1)
				{
					((UINT32*)pvBits)[i] = nColor;
				}
			}
		}

		for (x = 0; x < 4; x++)
		{
			for (y = 0; y < 4; y++)
			{
				int i = (cx - 1 - x + y * cx);
				if (i > -1)
				{
					((UINT32 *)pvBits)[i] = (UINT32)3 * (x + 1) * (y + 1) * 0x1000000;
				}
			}
		}
	}

	HBITMAP hOld = (HBITMAP)SelectObject(dc, hbitmap);

	SIZE sz = {cx, cy};

	XTShadowsManager()->UpdateLayeredWindow((HWND)GetSafeHwnd(), (HDC)0, 0, &sz, dc.GetSafeHdc(), &pt, 0, &bf, 0x02);

	SelectObject(dc, hOld);
	DeleteObject(hbitmap);
	dc.DeleteDC();
}

// Uncomment this line to disable alpha shadow. DEBUG ONLY
// #define NOALPHASHADOW

BOOL CXTShadowsManager::AlphaShadow()
{
#ifdef NOALPHASHADOW
	return FALSE;
#endif
	return (UpdateLayeredWindow != NULL);
}
BOOL CXTShadowsManager::PseudoShadow()
{
	if (AlphaShadow())
		return FALSE;
	return !XTAuxData().bUseSolidShadows;
}

_XT_EXT_CLASS CXTShadowsManager* AFX_CDECL XTShadowsManager()
{
	return &CXTShadowsManager::m_instance;
}
