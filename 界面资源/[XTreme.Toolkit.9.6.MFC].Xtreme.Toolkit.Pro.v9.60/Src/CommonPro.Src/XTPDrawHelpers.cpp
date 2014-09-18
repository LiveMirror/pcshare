// XTPDrawHelpers.cpp: implementation of the CXTPDrawHelpers class.
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
#include "uxtheme.h"
#include "schemadef.h"
#include "tmschema.h"
#include "XTPWinThemeWrapper.h"
#include "XTPWinThemeWrapper.h"
#include "XTPColorManager.h"
#include "XTPDrawHelpers.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

HHOOK CXTPMouseMonitor::m_hHookMouse = 0;
CWnd* CXTPMouseMonitor::m_pWndMonitor = 0;

//////////////////////////////////////////////////////////////////////////
// CXTPMouseMonitor
//////////////////////////////////////////////////////////////////////////

void CXTPMouseMonitor::SetupHook(CWnd* pWndMonitor)
{
	if (pWndMonitor && m_hHookMouse == 0)
	{
		m_hHookMouse = SetWindowsHookEx(WH_MOUSE, MouseProc, 0, GetCurrentThreadId ());
	}
	if (!pWndMonitor && m_hHookMouse)
	{
		UnhookWindowsHookEx(m_hHookMouse);
		m_hHookMouse = 0;
	}
	m_pWndMonitor = pWndMonitor;
}

LRESULT CALLBACK CXTPMouseMonitor::MouseProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode != HC_ACTION || !m_pWndMonitor)
		return CallNextHookEx(m_hHookMouse, nCode, wParam, lParam);

	CXTPWindowRect rc(m_pWndMonitor);

	if (!rc.PtInRect(((PMOUSEHOOKSTRUCT)lParam)->pt))
	{
		switch(wParam)
		{
		case WM_LBUTTONDOWN:
		case WM_NCLBUTTONDOWN:
		case WM_RBUTTONDOWN:
		case WM_NCRBUTTONDOWN:
		case WM_MBUTTONDOWN:
		case WM_NCMBUTTONDOWN:
		case WM_LBUTTONUP:
		case WM_NCLBUTTONUP:
			m_pWndMonitor->GetOwner()->SetFocus();
			return TRUE;
		}
	}

	return CallNextHookEx(m_hHookMouse, nCode, wParam, lParam);
}


//===========================================================================
// CXTPClientCursorPos class
//===========================================================================

CXTPTransparentBitmap::CXTPTransparentBitmap(HBITMAP hBitmap)
: m_hBitmap(hBitmap)
{
}

// Not foolproof, but works 99% of the time :).  Assumes the top
// left pixel is the transparent color.

COLORREF CXTPTransparentBitmap::GetTransparentColor() const
{
	CBitmap* pBitmap = CBitmap::FromHandle(m_hBitmap);
	if (pBitmap != NULL)
	{
		CXTPCompatibleDC dc(NULL, pBitmap);
		return dc.GetPixel(0,0);
	}
	return (COLORREF)-1;
}


HICON CXTPTransparentBitmap::ConvertToIcon() const
{
	if (m_hBitmap == NULL)
		return NULL;

	COLORREF crTransparent = GetTransparentColor();

	BITMAP bmp;
	if (!::GetObject(m_hBitmap, sizeof(BITMAP), &bmp))
		return NULL;

	if (bmp.bmHeight == 0 || bmp.bmWidth == 0)
		return NULL;

	CImageList il;
	il.Create(bmp.bmWidth, bmp.bmHeight, ILC_COLOR24|ILC_MASK, 0, 1);
	il.Add(CBitmap::FromHandle(m_hBitmap), crTransparent);

	ASSERT(il.GetImageCount() == 1);

	return il.ExtractIcon(0);
}

//===========================================================================
// CXTPClientCursorPos class
//===========================================================================

CXTPClientCursorPos::CXTPClientCursorPos(CWnd* pWnd)
{
	GetCursorPos(this);
	pWnd->ScreenToClient(this);
}

//===========================================================================
// CXTPEmptyRect class
//===========================================================================

CXTPEmptyRect::CXTPEmptyRect()
{
	SetRectEmpty();
}

//===========================================================================
// CXTPWindowRect class
//===========================================================================

CXTPWindowRect::CXTPWindowRect(HWND hWnd)
{
	if (::IsWindow(hWnd))
		::GetWindowRect(hWnd, this);
	else
		SetRectEmpty();
}

CXTPWindowRect::CXTPWindowRect(const CWnd* pWnd)
{
	if (::IsWindow(pWnd->GetSafeHwnd()))
		::GetWindowRect(pWnd->GetSafeHwnd(), this);
	else
		SetRectEmpty();
}

//===========================================================================
// CXTPClientRect class
//===========================================================================

CXTPClientRect::CXTPClientRect(HWND hWnd)
{
	if (::IsWindow(hWnd))
		::GetClientRect(hWnd, this);
	else
		SetRectEmpty();
}

CXTPClientRect::CXTPClientRect(const CWnd* pWnd)
{
	if (::IsWindow(pWnd->GetSafeHwnd()))
		::GetClientRect(pWnd->GetSafeHwnd(), this);
	else
		SetRectEmpty();
}

//===========================================================================
// CXTPBufferDC class
//===========================================================================

CXTPBufferDC::CXTPBufferDC(HDC hDestDC, const CRect rcPaint) : m_hDestDC (hDestDC)
{
	m_rect = rcPaint;
	Attach (::CreateCompatibleDC (m_hDestDC));
	m_bitmap.Attach (::CreateCompatibleBitmap(
		m_hDestDC, m_rect.right, m_rect.bottom));
	m_hOldBitmap = ::SelectObject (m_hDC, m_bitmap);
}

CXTPBufferDC::~CXTPBufferDC()
{
	::BitBlt (m_hDestDC, m_rect.left, m_rect.top, m_rect.Width(),
		m_rect.Height(), m_hDC, m_rect.left, m_rect.top, SRCCOPY);
	::SelectObject (m_hDC, m_hOldBitmap);
}

CDC* CXTPBufferDC::GetDestDC()
{
	return CDC::FromHandle(m_hDestDC);
}

//===========================================================================
// CXTPFontDC class
//===========================================================================

CXTPFontDC::CXTPFontDC(CDC* pDC, CFont* pFont)
: m_hDC(pDC->GetSafeHdc())
{
	m_hOldFont = SelectObject(m_hDC, pFont->GetSafeHandle());
}

CXTPFontDC::~CXTPFontDC()
{
	::SelectObject(m_hDC, m_hOldFont);
}

void CXTPFontDC::SetFont(CFont* pFont)
{
	::SelectObject(m_hDC, m_hOldFont);
	m_hOldFont = SelectObject(m_hDC, pFont->GetSafeHandle());
}

//===========================================================================
// CXTPPenDC class
//===========================================================================

CXTPPenDC::CXTPPenDC(HDC hDC, COLORREF crColor)
: m_hDC (hDC)
{
	VERIFY(m_pen.CreatePen (PS_SOLID, 1, crColor));
	m_hOldPen = (HPEN)::SelectObject (m_hDC, m_pen);
}

CXTPPenDC::~CXTPPenDC ()
{
	::SelectObject (m_hDC, m_hOldPen);
}

void CXTPPenDC::Color(COLORREF crColor)
{
	::SelectObject (m_hDC, m_hOldPen);
	VERIFY(m_pen.DeleteObject());
	VERIFY(m_pen.CreatePen (PS_SOLID, 1, crColor));
	m_hOldPen = (HPEN)::SelectObject (m_hDC, m_pen);
}

COLORREF CXTPPenDC::Color()
{
	LOGPEN logPen;
	m_pen.GetLogPen(&logPen);
	return logPen.lopnColor;
}

//===========================================================================
// CXTPBrushDC class
//===========================================================================

CXTPBrushDC::CXTPBrushDC(HDC hDC, COLORREF crColor)
: m_hDC (hDC)
{
	VERIFY(m_brush.CreateSolidBrush (crColor));
	m_hOldBrush = (HBRUSH)::SelectObject (m_hDC, m_brush);
}

CXTPBrushDC::~CXTPBrushDC()
{
	::SelectObject(m_hDC, m_hOldBrush);
}

void CXTPBrushDC::Color(COLORREF crColor)
{
	::SelectObject(m_hDC, m_hOldBrush);
	VERIFY(m_brush.DeleteObject());
	VERIFY(m_brush.CreateSolidBrush(crColor));
	m_hOldBrush = (HBRUSH)::SelectObject (m_hDC, m_brush);
}

//===========================================================================
// CXTPCompatibleDC class
//===========================================================================

CXTPCompatibleDC::CXTPCompatibleDC(CDC* pDC, CBitmap* pBitmap)
{
	CreateCompatibleDC(pDC);
	m_pBmp = SelectObject(pBitmap);
}

CXTPCompatibleDC::~CXTPCompatibleDC()
{
	SelectObject(m_pBmp);
	DeleteDC();
}



//===========================================================================
// CXTPSplitterTracker class
//===========================================================================
CXTPSplitterTracker::CXTPSplitterTracker(BOOL bSolid /*= FALSE*/)
{
	m_bSolid = bSolid;
	m_rcBoundRect.SetRectEmpty();
}


void CXTPSplitterTracker::OnInvertTracker(CRect rect)
{
	ASSERT(!rect.IsRectEmpty());

	if (m_bSolid)
	{
		m_pDC->InvertRect(rect);
	}
	else
	{
		CBrush* pDitherBrush = CDC::GetHalftoneBrush();
		CBrush* pBrush = (CBrush*)m_pDC->SelectObject(pDitherBrush);

		m_pDC->PatBlt(rect.left, rect.top, rect.Width(), rect.Height(), PATINVERT);
		m_pDC->SelectObject(pBrush);
	}
}

BOOL CXTPSplitterTracker::Track(CWnd* pTrackWnd, CRect rcAvail, CRect& rectTracker, CPoint point, BOOL bHoriz)
{
	pTrackWnd->SetCapture();
	m_pDC = 0;

	CWnd* pWnd = CWnd::GetDesktopWindow();
	if (pWnd->LockWindowUpdate())
		m_pDC = pWnd->GetDCEx(NULL, DCX_WINDOW|DCX_CACHE|DCX_LOCKWINDOWUPDATE);
	else
		m_pDC = pWnd->GetDCEx(NULL, DCX_WINDOW|DCX_CACHE);
	ASSERT(m_pDC != NULL);

	CPoint ptOffset = bHoriz? CPoint(rectTracker.left - point.x, 0):
		CPoint(0, rectTracker.top - point.y);

	OnInvertTracker(rectTracker);

	if (!m_rcBoundRect.IsRectEmpty())
		OnInvertTracker(m_rcBoundRect);

	BOOL bAccept = FALSE;
	while (CWnd::GetCapture() == pTrackWnd)
	{
		MSG msg;
		if (!GetMessage(&msg, NULL, 0, 0))
			break;

		if (msg.message == WM_MOUSEMOVE)
		{
			point = CPoint(msg.lParam);
			pTrackWnd->ClientToScreen(&point);
			point += ptOffset;

			point.x = max(min(point.x, rcAvail.right), rcAvail.left);
			point.y = max(min(point.y, rcAvail.bottom), rcAvail.top);

			if (bHoriz)
			{
				if (rectTracker.left != point.x)
				{
					OnInvertTracker(rectTracker);
					rectTracker.OffsetRect(point.x - rectTracker.left, 0);
					OnInvertTracker(rectTracker);
				}

			} else
			{
				if (rectTracker.top != point.y)
				{
					OnInvertTracker(rectTracker);
					rectTracker.OffsetRect(0, point.y - rectTracker.top);
					OnInvertTracker(rectTracker);
				}
			}
		}
		else if (msg.message == WM_KEYDOWN &&  msg.wParam == VK_ESCAPE) break;
		else if (msg.message ==WM_LBUTTONUP)
		{
			bAccept = TRUE;
			break;
		}
		else  ::DispatchMessage(&msg);
	}

	if (!m_rcBoundRect.IsRectEmpty())
		OnInvertTracker(m_rcBoundRect);

	OnInvertTracker(rectTracker);

	if (CWnd::GetCapture() == pTrackWnd) ReleaseCapture();
	pWnd = CWnd::GetDesktopWindow();
	pWnd->UnlockWindowUpdate();
	if (m_pDC != NULL)
	{
		pWnd->ReleaseDC(m_pDC);
		m_pDC = NULL;
	}

	return bAccept;
}
//===========================================================================
// CXTPDrawHelpers class
//===========================================================================

CXTPDrawHelpers CXTPDrawHelpers::m_instance; // singleton

CXTPDrawHelpers::CXTPDrawHelpers()
	: m_hMsImgDll(NULL)
{
	m_pfnFastGradientFill = 0;
	m_hMsImgDll = ::LoadLibrary( _T("msimg32.dll") );

	if (m_hMsImgDll)
	{
		m_pfnFastGradientFill = (PFNGRADIENTFILL)GetProcAddress(m_hMsImgDll, "GradientFill");
	}
}

CXTPDrawHelpers::~CXTPDrawHelpers()
{
	if (m_hMsImgDll != NULL)
	{
		::FreeLibrary( m_hMsImgDll );
	}
}

BOOL CXTPDrawHelpers::GradientFill(HDC hdc, PTRIVERTEX pVertex, ULONG dwNumVertex, PVOID pMesh, ULONG dwNumMesh, ULONG dwMode)
{
	if ( m_pfnFastGradientFill )
	{
		return (*m_pfnFastGradientFill)(hdc, pVertex, dwNumVertex, pMesh, dwNumMesh, dwMode);
	}

	return FALSE;
}

void CXTPDrawHelpers::GradientFillSlow(CDC* pDC, LPRECT lpRect, COLORREF crFrom, COLORREF crTo, BOOL bHorz)
{
	int cx = max(1, lpRect->right - lpRect->left);
	int cy = max(1, lpRect->bottom - lpRect->top);

	CRect rc;
	pDC->GetClipBox(&rc);

	if (rc.IsRectEmpty())
		rc = *lpRect;
	else
		rc.IntersectRect(rc, lpRect);

	if (bHorz)
	{
		for (int nX = rc.left; nX < rc.right; nX++)
		{
			pDC->FillSolidRect(nX, rc.top, 1, rc.Height(), BlendColors(
				crFrom, crTo, (float)(1.0 - ((nX - lpRect->left) / (float)cx))));
		}
	}
	else
	{
		for (int nY = rc.top; nY < rc.bottom; nY++)
		{
			pDC->FillSolidRect(rc.left, nY, rc.Width(), 1, BlendColors(
				crFrom, crTo, (float)(1.0 - ((nY - lpRect->top)) / (float)cy)));
		}
	}
}

void CXTPDrawHelpers::GradientFillFast(CDC* pDC, LPRECT lpRect, COLORREF crFrom, COLORREF crTo, BOOL bHorz)
{
	TRIVERTEX vert[2];
	vert[0].x      = lpRect->left;
	vert[0].y      = lpRect->top;
	vert[0].Red    = (COLOR16)(GetRValue(crFrom) << 8);
	vert[0].Green  = (COLOR16)(GetGValue(crFrom) << 8);
	vert[0].Blue   = (COLOR16)(GetBValue(crFrom) << 8);
	vert[0].Alpha  = 0x0000;

	vert[1].x      = lpRect->right;
	vert[1].y      = lpRect->bottom;
	vert[1].Red    = (COLOR16)(GetRValue(crTo) << 8);
	vert[1].Green  = (COLOR16)(GetGValue(crTo) << 8);
	vert[1].Blue   = (COLOR16)(GetBValue(crTo) << 8);
	vert[1].Alpha  = 0x0000;

	GRADIENT_RECT gRect = { 0, 1 };

	GradientFill(*pDC, vert, 2, &gRect, 1, bHorz ? GRADIENT_FILL_RECT_H : GRADIENT_FILL_RECT_V);
}

void CXTPDrawHelpers::GradientFill(CDC* pDC, LPRECT lpRect, COLORREF crFrom, COLORREF crTo, BOOL bHorz)
{
	if (!lpRect)
		return;

	if (::IsRectEmpty(lpRect))
		return;

	if (IsLowResolution(pDC->GetSafeHdc()))
	{
		pDC->FillSolidRect(lpRect, crFrom);
	}
	else if (crFrom == crTo)
	{
		pDC->FillSolidRect(lpRect, crFrom);
	}
	else if (m_pfnFastGradientFill != NULL)
	{
		GradientFillFast(pDC, lpRect, crFrom, crTo, bHorz);
	}
	else
	{
		GradientFillSlow(pDC, lpRect, crFrom, crTo, bHorz);
	}
}

void CXTPDrawHelpers::GradientFill(CDC* pDC, LPRECT lpRect, CXTPPaintManagerColorGradient& grc, BOOL bHorz)
{
	GradientFill(pDC, lpRect, grc.clrLight, grc.clrDark, bHorz);
}

void CXTPDrawHelpers::ExcludeCorners(CDC* pDC, CRect rc)
{
	pDC->ExcludeClipRect(rc.left, rc.top, rc.left + 1, rc.top + 1);
	pDC->ExcludeClipRect(rc.right - 1, rc.top, rc.right, rc.top + 1);
	pDC->ExcludeClipRect(rc.left, rc.bottom - 1, rc.left + 1, rc.bottom + 1);
	pDC->ExcludeClipRect(rc.right - 1, rc.bottom - 1, rc.right, rc.bottom + 1);
}

void CXTPDrawHelpers::BlurPoints(CDC* pDC, LPPOINT pts, int nCount)
{
	for (int i = 0; i < nCount; i += 2)
	{
		CPoint ptBlur = pts[i];
		CPoint ptDirection(pts[i].x + pts[i + 1].x, pts[i].y + pts[i + 1].y);

		COLORREF clrBlur = pDC->GetPixel(ptDirection);
		COLORREF clrDirection = pDC->GetPixel(ptBlur);

		pDC->SetPixel(ptBlur, RGB(
			(GetRValue(clrBlur) + GetRValue(clrDirection)) / 2,
			(GetGValue(clrBlur) + GetGValue(clrDirection)) / 2,
			(GetBValue(clrBlur) + GetBValue(clrDirection)) / 2));
	}
}

COLORREF CXTPDrawHelpers::BlendColors(COLORREF crA, COLORREF crB, float fAmountA)
{
	BYTE btR = (BYTE)min(255, (int)(GetRValue(crA) * fAmountA + GetRValue(crB) * (1.0f - fAmountA)));
	BYTE btG = (BYTE)min(255, (int)(GetGValue(crA) * fAmountA + GetGValue(crB) * (1.0f - fAmountA)));
	BYTE btB = (BYTE)min(255, (int)(GetBValue(crA) * fAmountA + GetBValue(crB) * (1.0f - fAmountA)));

	return RGB(btR, btG, btB);
}

static int CALLBACK XTPEnumFontFamExProc(ENUMLOGFONTEX* pelf, NEWTEXTMETRICEX* /*lpntm*/, int /*FontType*/, LPVOID pThis)
{
	CString strFontName = *( CString* )pThis;
	CString strFaceName = pelf->elfLogFont.lfFaceName;

	if ( strFaceName.CompareNoCase( strFontName ) == 0 )
		return 0;

	return 1;
}

BOOL CXTPDrawHelpers::FontExists(CString& strFaceName)
{
	// Enumerate all styles and charsets of all fonts:
	LOGFONT lfEnum;
	::ZeroMemory(&lfEnum, sizeof(LOGFONT));

	lfEnum.lfFaceName[ 0 ] = 0;
	lfEnum.lfCharSet = DEFAULT_CHARSET;

	CWindowDC dc( NULL );

	return  ::EnumFontFamiliesEx(dc.m_hDC, &lfEnum, ( FONTENUMPROC )
		XTPEnumFontFamExProc, ( LPARAM )&strFaceName, 0 ) == 0;
}

CString CXTPDrawHelpers::GetDefaultFontName()
{
	LOGFONT lfFont;
	::GetObject(::GetStockObject(DEFAULT_GUI_FONT), sizeof(LOGFONT), &lfFont);

	return CString(lfFont.lfFaceName);
}

CString CXTPDrawHelpers::GetVerticalFontName(BOOL bUseOfficeFont)
{
	LOGFONT lfFont;
	::GetObject(::GetStockObject(DEFAULT_GUI_FONT), sizeof(LOGFONT), &lfFont);
	bool bUseSystemFont = lfFont.lfCharSet > SYMBOL_CHARSET;

	if (bUseSystemFont)
	{
		OSVERSIONINFO osvi;
		osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
		VERIFY(GetVersionEx(&osvi));

		if (!((osvi.dwPlatformId == VER_PLATFORM_WIN32_NT) && (osvi.dwMajorVersion > 4)))
			bUseSystemFont = FALSE;
	}


	CString strOfficeFont = _T("Tahoma");
	CString strVerticalFaceName = _T("Arial");

	if (bUseSystemFont || !FontExists(strVerticalFaceName))
	{
		strVerticalFaceName = lfFont.lfFaceName;
	}
	else if (bUseOfficeFont && !bUseSystemFont && FontExists(strOfficeFont) )
	{
		strVerticalFaceName = strOfficeFont;
	}

	return strVerticalFaceName;
}

void CXTPDrawHelpers::Triangle(CDC* pDC, CPoint pt0, CPoint pt1, CPoint pt2, COLORREF clr)
{
	CXTPPenDC pen (*pDC, clr);
	CXTPBrushDC brush (*pDC, clr);

	Triangle(pDC, pt0, pt1, pt2);
}

BOOL CXTPDrawHelpers::IsLowResolution(HDC hDC/* = 0*/)
{
	if (hDC)
	{
		int nColors = ::GetDeviceCaps(hDC, BITSPIXEL);
		return (nColors > 0 && nColors <= 8);
	}
	hDC = ::GetDC(::GetDesktopWindow());
	if (hDC)
	{
		int nColors = ::GetDeviceCaps(hDC, BITSPIXEL);
		::ReleaseDC(::GetDesktopWindow(), hDC);
		return (nColors > 0 && nColors <= 8);
	}
	return FALSE;
}

XTPCurrentSystemTheme CXTPDrawHelpers::GetCurrentSystemTheme()
{
	return XTPColorManager()->GetCurrentSystemTheme();
}


//=============================================================================
// Multi-Monitor API
//=============================================================================

class CXTPMultiMonitor
{
protected:
	DECLARE_HANDLE(HMONITOR);

protected:
	typedef struct tagMONITORINFO
	{
		DWORD   cbSize;
		RECT    rcMonitor;
		RECT    rcWork;
		DWORD   dwFlags;
	} MONITORINFO, *LPMONITORINFO;

protected:
	CXTPMultiMonitor();

protected:
	BOOL InitMultipleMonitorStubs();

	CRect GetWorkArea(HMONITOR hMonitor)
	{
		if (hMonitor && m_pfnGetMonitorInfo)
		{
			ASSERT(sizeof(MONITORINFO) == 40);

			MONITORINFO info;
			info.cbSize = sizeof(MONITORINFO);

			if (m_pfnGetMonitorInfo(hMonitor, (MONITORINFO*)&info))
				return info.rcWork;
		}

		RECT rcWork;
		::SystemParametersInfo(SPI_GETWORKAREA, NULL, &rcWork, 0);
		return rcWork;
	}

public:

	CRect GetWorkArea(HWND hWnd) {
		return GetWorkArea(InitMultipleMonitorStubs()? m_pfnMonitorFromWindow(hWnd, 0): 0);
	}
	CRect GetWorkArea(POINT ptScreenCoords) {
		return GetWorkArea(InitMultipleMonitorStubs()? m_pfnMonitorFromPoint(ptScreenCoords, 0): 0);
	}
	CRect GetWorkArea(LPCRECT lprcScreenCoords) {
		return GetWorkArea(InitMultipleMonitorStubs()? m_pfnMonitorFromRect(lprcScreenCoords, 0): 0);
	}



protected:
	int      (WINAPI* m_pfnGetSystemMetrics)(int);
	HMONITOR (WINAPI* m_pfnMonitorFromWindow)(HWND, DWORD);
	HMONITOR (WINAPI* m_pfnMonitorFromRect)(LPCRECT, DWORD);
	HMONITOR (WINAPI* m_pfnMonitorFromPoint)(POINT, DWORD);
	BOOL     (WINAPI* m_pfnGetMonitorInfo)(HMONITOR, LPMONITORINFO);
	BOOL     m_fMultiMonInitDone;

public:
	static CXTPMultiMonitor s_monInstance;
};

CXTPMultiMonitor CXTPMultiMonitor::s_monInstance;

CXTPMultiMonitor::CXTPMultiMonitor()
{
	m_fMultiMonInitDone = FALSE;
}


BOOL CXTPMultiMonitor::InitMultipleMonitorStubs()
{
	if (m_fMultiMonInitDone)
	{
		return m_pfnGetMonitorInfo != NULL;
	}

#ifdef UNICODE
	OSVERSIONINFO osvi = {0};
	osvi.dwOSVersionInfoSize = sizeof(osvi);
	GetVersionEx(&osvi);
	BOOL fMultimonPlatformNT = (VER_PLATFORM_WIN32_NT == osvi.dwPlatformId);
#endif

	HMODULE hUser32 = GetModuleHandle(TEXT("USER32"));
	if (hUser32 &&
		(*(FARPROC*)&m_pfnGetSystemMetrics    = GetProcAddress(hUser32,"GetSystemMetrics")) != NULL &&
		(*(FARPROC*)&m_pfnMonitorFromWindow   = GetProcAddress(hUser32,"MonitorFromWindow")) != NULL &&
		(*(FARPROC*)&m_pfnMonitorFromRect     = GetProcAddress(hUser32,"MonitorFromRect")) != NULL &&
		(*(FARPROC*)&m_pfnMonitorFromPoint    = GetProcAddress(hUser32,"MonitorFromPoint")) != NULL &&
#ifdef UNICODE
		(*(FARPROC*)&m_pfnGetMonitorInfo      = fMultimonPlatformNT ? GetProcAddress(hUser32,"GetMonitorInfoW") :
		GetProcAddress(hUser32,"GetMonitorInfoA")) != NULL
#else
		(*(FARPROC*)&m_pfnGetMonitorInfo      = GetProcAddress(hUser32,"GetMonitorInfoA")) != NULL
#endif
		)
	{
		m_fMultiMonInitDone = TRUE;
		return TRUE;
	}
	else
	{
		m_pfnGetSystemMetrics    = NULL;
		m_pfnMonitorFromWindow   = NULL;
		m_pfnMonitorFromRect     = NULL;
		m_pfnMonitorFromPoint    = NULL;
		m_pfnGetMonitorInfo      = NULL;

		m_fMultiMonInitDone = TRUE;
		return FALSE;
	}
}


CRect CXTPDrawHelpers::GetWorkArea(const CRect& rect)
{
	return CXTPMultiMonitor::s_monInstance.GetWorkArea(rect);
}

CRect CXTPDrawHelpers::GetWorkArea(const CWnd* pWnd)
{
	ASSERT_VALID(pWnd);

	return CXTPMultiMonitor::s_monInstance.GetWorkArea(pWnd->GetSafeHwnd());
}

CRect CXTPDrawHelpers::GetWorkArea()
{
	POINT point;
	::GetCursorPos( &point );

	return CXTPMultiMonitor::s_monInstance.GetWorkArea(point);
}
