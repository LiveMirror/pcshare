// XTPShadowsManager.cpp : implementation of the CXTPShadowsManager class.
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
#include "XTPColorManager.h"

#include "XTPShadowsManager.h"
#include "XTPHookManager.h"
#include "XTPCommandBar.h"
#include "XTPPaintManager.h"
#include "XTPControlPopup.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CXTPShadowsManager CXTPShadowsManager::s_managerInstance;
BOOL CXTPShadowsManager::CShadowWnd::m_bAlphaShadow;

CXTPShadowsManager::CXTPShadowsManager(void)
{
	HMODULE hLib = GetModuleHandle(_T("USER32"));
	if (hLib)
	{
		UpdateLayeredWindow = (LPFNUPDATELAYEREDWINDOW) GetProcAddress(hLib, "UpdateLayeredWindow");
	}
	CShadowWnd::m_bAlphaShadow = AlphaShadow();
}


CXTPShadowsManager::~CXTPShadowsManager(void)
{
	ASSERT(m_lstShadow.IsEmpty());
}

#ifndef SPI_GETDROPSHADOW
#define SPI_GETDROPSHADOW     0x1024
#endif


void CXTPShadowsManager::SetShadow(CXTPCommandBar* pCommandBar, const CRect& rcExclude )
{
	if (pCommandBar->IsCustomizeMode())
		return;

	if (!pCommandBar->GetPaintManager()->IsOfficeLikeShadow())
	{
		if (!CShadowWnd::m_bAlphaShadow)
			return;

		BOOL bDropShadow = FALSE;
		SystemParametersInfo(SPI_GETDROPSHADOW, 0, &bDropShadow, 0);

		if (!bDropShadow)
			return;
	}

	CXTPWindowRect rc(pCommandBar);
	CreateShadow(TRUE, rc, rcExclude, pCommandBar);
	CreateShadow(FALSE, rc, rcExclude, pCommandBar);
}

void CXTPShadowsManager::SetShadow(CXTPControlPopup* pControl)
{
	if (pControl->GetParent()->IsCustomizeMode())
		return;

	if (pControl->GetPaintManager()->IsOfficeLikeShadow())
	{
		CRect rc = pControl->GetRect();
		((CWnd*)pControl->GetParent())->ClientToScreen(&rc);
		CXTPCommandBar* pCommandBar = pControl->GetCommandBar();

		CreateShadow(TRUE, rc, CXTPEmptyRect(), pCommandBar);
		CreateShadow(FALSE, rc, CXTPEmptyRect(), pCommandBar);
	}
}

void CXTPShadowsManager::RemoveShadow(CXTPCommandBar* pCommandBar)
{
	POSITION pos = m_lstShadow.GetHeadPosition();
	while (pos)
	{
		CShadowWnd* pShadow = m_lstShadow.GetNext(pos);

		if (pCommandBar == pShadow->m_pCommandBar)
		{
			XTPHookManager()->RemoveHook(pShadow->m_pCommandBar->GetSafeHwnd(), pShadow);
			DestroyShadow(pShadow);
		}
	}
}

CXTPShadowsManager::CShadowWnd* CXTPShadowsManager::CreateShadow(BOOL bHoriz, CRect rc, CRect rcExclude, CXTPCommandBar* pCommandBar)
{
	CShadowWnd* pShadow = new CShadowWnd();
	pShadow->m_bOfficeLikeShadow = pCommandBar->GetPaintManager()->IsOfficeLikeShadow();

	pShadow->m_pCommandBar = pCommandBar;
	pShadow->Create(bHoriz, rc);

	m_lstShadow.AddShadow(pShadow);



	if (pShadow->m_bOfficeLikeShadow)
	{
		pShadow->ExcludeRect(rcExclude);
	}

	POSITION pos = m_lstShadow.GetHeadPosition();
	while (pos)
	{
		CShadowWnd* pShadowPrev = m_lstShadow.GetNext(pos);
		if (pShadowPrev->m_pCommandBar)
		{
			pShadow->ExcludeRect(CXTPWindowRect(pShadowPrev->m_pCommandBar));
		}
	}


	pShadow->SetWindowPos(&CWnd::wndTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW | SWP_NOACTIVATE);

	XTPHookManager()->SetHook(pCommandBar->m_hWnd, pShadow);

	return pShadow;
}


void CXTPShadowsManager::DestroyShadow(CShadowWnd* pWnd)
{
	pWnd->ShowWindow(SW_HIDE);
	pWnd->m_pCommandBar = 0;
	m_lstShadow.RemoveShadow(pWnd);
	pWnd->DestroyWindow();
	delete pWnd;
}

CXTPShadowsManager::CShadowWnd::CShadowWnd()
{
	m_pCommandBar = 0;
}

int CXTPShadowsManager::CShadowWnd::OnHookMessage(HWND hWnd, UINT nMessage, WPARAM& wParam, LPARAM& /*lParam*/, LRESULT& /*lResult*/)
{
	if (nMessage == WM_SHOWWINDOW && wParam == FALSE)
	{
		ShowWindow(SW_HIDE);
	}
	if (nMessage == WM_SHOWWINDOW && wParam == TRUE)
	{
		ShowWindow(SW_SHOWNA);
	}
	if (nMessage == WM_DESTROY)
	{
		XTPHookManager()->RemoveHook(hWnd, this);
		XTPShadowsManager()->DestroyShadow(this);
	}
	return 0;
}

BOOL CXTPShadowsManager::CShadowWnd::Create(BOOL bHoriz, CRect rcWindow)
{
	CWnd* pSite = GetDesktopWindow();

	if (m_hWnd == 0)
	{
		int nFlags = WS_EX_TOOLWINDOW|WS_EX_TOPMOST;
		if (m_bAlphaShadow) nFlags |= 0x80000;

		CString strClass = ::AfxRegisterWndClass(0, ::LoadCursor(0, IDC_ARROW));
		if(!CreateEx(nFlags, strClass, 0, WS_POPUP, CXTPEmptyRect(), pSite,  0))
			return FALSE;
	}
	SetWindowRgn(NULL, FALSE);

	int nWidth = m_bAlphaShadow ? 4 : 2;

	m_bHoriz = bHoriz;

	CRect rcShadow;
	if (bHoriz) rcShadow.SetRect(rcWindow.left + nWidth, rcWindow.bottom, rcWindow.right + nWidth, rcWindow.bottom + nWidth);
		else rcShadow.SetRect(rcWindow.right, rcWindow.top + nWidth, rcWindow.right + nWidth, rcWindow.bottom);

	MoveWindow(rcShadow, FALSE);
	return TRUE;
}

BEGIN_MESSAGE_MAP(CXTPShadowsManager::CShadowWnd, CWnd)
	{ WM_ERASEBKGND, 0, 0, 0, AfxSig_bD, (AFX_PMSG)(AFX_PMSGW) (static_cast< BOOL (AFX_MSG_CALL CWnd::*)(CDC*) > (CXTPShadowsManager::CShadowWnd::OnEraseBkgnd)) },
	{ WM_PAINT, 0, 0, 0, AfxSig_vv, (AFX_PMSG)(AFX_PMSGW) (static_cast< void (AFX_MSG_CALL CWnd::*)(void) > (CXTPShadowsManager::CShadowWnd::OnPaint)) },
	{ WM_SIZE, 0, 0, 0, AfxSig_vwii, (AFX_PMSG)(AFX_PMSGW) (static_cast< void (AFX_MSG_CALL CWnd::*)(UINT, int, int) > (CXTPShadowsManager::CShadowWnd::OnSize)) },
	{ WM_NCHITTEST, 0, 0, 0, AfxSig_wp, (AFX_PMSG)(AFX_PMSGW) (static_cast< UINT (AFX_MSG_CALL CWnd::*)(CPoint) > (CXTPShadowsManager::CShadowWnd::OnNcHitTest)) },
END_MESSAGE_MAP()

UINT CXTPShadowsManager::CShadowWnd::OnNcHitTest(CPoint /*point*/)
{
	return (UINT)HTTRANSPARENT;
}

void CXTPShadowsManager::CShadowWnd::OnPaint()
{
	CPaintDC dc(this);
	CXTPClientRect rcClient(this);

	dc.FillSolidRect(rcClient, m_bAlphaShadow? 0: GetXtremeColor(COLOR_3DSHADOW));
}


BOOL CXTPShadowsManager::CShadowWnd::OnEraseBkgnd(CDC* /*pDC*/)
{
	return TRUE;
}

BOOL CXTPShadowsManager::CShadowWnd::ExcludeRect(CRect rcExclude)
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

void CXTPShadowsManager::CShadowWnd::LongShadow(CShadowList* pList)
{
	CXTPWindowRect rcWindow(this);

	POSITION pos = pList->GetHeadPosition();
	while(pos)
	{
		CShadowWnd* pWnd = pList->GetNext(pos);
		if (m_bHoriz == !pWnd->m_bHoriz)
		{
			CXTPWindowRect rc(pWnd);
			if (m_bHoriz == FALSE)
			{
				if (rcWindow.top == rc.bottom -1 && rcWindow.right < rc.right && rcWindow.right > rc.left)
				{
					rcWindow.top -= 2 * rc.Height();
					MoveWindow(rcWindow, FALSE);
				}
			} else
			{

				if (rcWindow.left == rc.right - 1 && rcWindow.top > rc.top && rcWindow.bottom < rc.bottom)
				{
					rcWindow.left -= 2 * rc.Width() + 1;
					MoveWindow(rcWindow, FALSE);
				}
			}
		}
	}
}

UINT CXTPShadowsManager::CShadowWnd::Factor(int& nRed, int& nGreen, int& nBlue, double dFactor)
{
	return (int(nRed * dFactor) << 16) + (int(nGreen * dFactor) << 8) + (int(nBlue * dFactor));
}


void CXTPShadowsManager::CShadowWnd::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	if (!cx || !cy || !m_bAlphaShadow || !XTPShadowsManager()->UpdateLayeredWindow || !m_pCommandBar)
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


	#define FOR(x, y) for (x = 0; x < 4; x++) for (y = 0; y < 4; y++)
	typedef unsigned int UINT32;

	COLORREF clrFactor = m_pCommandBar->GetPaintManager()->m_clrShadowFactor;


	int nRed = GetRValue(clrFactor), nGreen = GetGValue(clrFactor), nBlue = GetBValue(clrFactor);

	int nFactor = m_bOfficeLikeShadow? 1: 2;

	if (!m_bHoriz)
	{
		ASSERT( cx == 4);

		FOR(x, y) ((UINT32 *)pvBits)[3 - x + (cy - 1 - y)  * cx] = (UINT32) 3 * (x + 1) * (y + 1) * 0x1000000 * nFactor
			+ Factor(nRed, nGreen, nBlue, double((x + 1) * (y + 1))/20);

		for (x = 3; x >= 0; x--)
		{
			UINT nColor = 0x0F000000 * nFactor * ( 4 - x) + Factor(nRed, nGreen, nBlue, double(4 - x)/4);

			for (y = 0; y < cy - 4; y++) ((UINT32 *)pvBits)[x + y * cx] = nColor;
		}
	} else
	{
		ASSERT( cy == 4);

		FOR(x, y) ((UINT32 *)pvBits)[ x + y * cx] = (UINT32) 3 * (x + 1) * (y + 1) * 0x1000000 * nFactor
			+ Factor(nRed, nGreen, nBlue, double((x + 1) * (y + 1))/20);

		for (y = 0; y < 4; y++)
		{
			UINT nColor = 0x0F000000 * nFactor * ( y + 1) + Factor(nRed, nGreen, nBlue, double(y + 1)/4);

			for (x = 4; x < cx - 4; x++) ((UINT32 *)pvBits)[x + y * cx] = nColor;
		}

		FOR(x, y) ((UINT32 *)pvBits)[ cx - 1 - x + y * cx] = (UINT32) 3 * (x + 1) * (y + 1) * 0x1000000 * nFactor
			+ Factor(nRed, nGreen, nBlue, double((x + 1) * (y + 1))/20);
	}


	HBITMAP hOld = (HBITMAP)SelectObject(dc, hbitmap);

	SIZE sz = {cx, cy};

	XTPShadowsManager()->UpdateLayeredWindow((HWND)GetSafeHwnd(), (HDC)0, 0, &sz, dc.GetSafeHdc(), &pt, 0, &bf, 0x02);

	SelectObject(dc, hOld);
	DeleteObject(hbitmap);
	dc.DeleteDC();

}
