// ACDSeeTheme.cpp
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
#include "ACDSeeTheme.h"
#include "resource.h"


#define CLR_TO_RGBQUAD(clr)     (RGB(GetBValue(clr), GetGValue(clr), GetRValue(clr)))

inline COLORREF GetNormColor(int r, int g, int b)
{
	return RGB(r > 255? 255: r, g > 255? 255: g, b > 255? 255: b);

}

HBITMAP AFXAPI LoadACDSeeBitmap(HINSTANCE hInst, HRSRC hRsrc, COLORREF clrAdjust)
{
	HGLOBAL hglb;
	if ((hglb = LoadResource(hInst, hRsrc)) == NULL)
		return NULL;

	LPBITMAPINFOHEADER lpBitmap = (LPBITMAPINFOHEADER)LockResource(hglb);
	if (lpBitmap == NULL)
		return NULL;

	// make copy of BITMAPINFOHEADER so we can modify the color table
	const int nColorTableSize = 16;
	UINT nSize = lpBitmap->biSize + nColorTableSize * sizeof(RGBQUAD);
	LPBITMAPINFOHEADER lpBitmapInfo = (LPBITMAPINFOHEADER)::malloc(nSize);
	if (lpBitmapInfo == NULL)
		return NULL;

	MEMCPY_S(lpBitmapInfo, lpBitmap, nSize);

	// color table is in RGBQUAD DIB format
	DWORD* pColorTable =
		(DWORD*)(((LPBYTE)lpBitmapInfo) + (UINT)lpBitmapInfo->biSize);

	clrAdjust = CLR_TO_RGBQUAD(clrAdjust);

	for (int iColor = 0; iColor < nColorTableSize; iColor++)
	{
		pColorTable[iColor]
			= GetNormColor(int(GetRValue(clrAdjust) * GetRValue(pColorTable[iColor]) / 192.75),
				int(GetGValue(clrAdjust) * GetGValue(pColorTable[iColor]) / 192.75),
				int(GetBValue(clrAdjust) * GetBValue(pColorTable[iColor]) / 192.75));
	}

	int nWidth = (int)lpBitmapInfo->biWidth;
	int nHeight = (int)lpBitmapInfo->biHeight;
	HDC hDCScreen = ::GetDC(NULL);
	HBITMAP hbm = ::CreateCompatibleBitmap(hDCScreen, nWidth, nHeight);

	if (hbm != NULL)
	{
		HDC hDCGlyphs = ::CreateCompatibleDC(hDCScreen);
		HBITMAP hbmOld = (HBITMAP)::SelectObject(hDCGlyphs, hbm);

		LPBYTE lpBits;
		lpBits = (LPBYTE)(lpBitmap + 1);
		lpBits += (1 << (lpBitmapInfo->biBitCount)) * sizeof(RGBQUAD);

		StretchDIBits(hDCGlyphs, 0, 0, nWidth, nHeight, 0, 0, nWidth, nHeight,
			lpBits, (LPBITMAPINFO)lpBitmapInfo, DIB_RGB_COLORS, SRCCOPY);
		SelectObject(hDCGlyphs, hbmOld);
		::DeleteDC(hDCGlyphs);
	}
	::ReleaseDC(NULL, hDCScreen);

	// free copy of bitmap info struct and resource itself
	::free(lpBitmapInfo);
	::FreeResource(hglb);

	return hbm;
}

HBITMAP CACDSeeTheme::m_hbmImageBackground = 0;
HBITMAP CACDSeeTheme::m_hbmImageFill = 0;

HBITMAP CACDSeeTheme::m_hbmTabCtrlBackground = 0;
HBITMAP CACDSeeTheme::m_hbmTabCtrlSelected = 0;


CACDSeeTheme::CACDSeeTheme()
{
	InitBitmaps();
}

void _DeleteObject(HGDIOBJ* pObject)
{
	ASSERT(pObject != NULL);
	if (*pObject != NULL)
	{
		DeleteObject(*pObject);
		*pObject = NULL;
	}
}


CACDSeeTheme::~CACDSeeTheme()
{
	_DeleteObject((HGDIOBJ*)&m_hbmImageBackground);
	_DeleteObject((HGDIOBJ*)&m_hbmImageFill);
	_DeleteObject((HGDIOBJ*)&m_hbmTabCtrlBackground);
	_DeleteObject((HGDIOBJ*)&m_hbmTabCtrlSelected);
}

void CACDSeeTheme::RefreshMetrics()
{
	CXTPDefaultTheme::RefreshMetrics();
	InitBitmaps();
}

BOOL CACDSeeTheme::InitBitmaps()
{
	_DeleteObject((HGDIOBJ*)&m_hbmImageBackground);
	_DeleteObject((HGDIOBJ*)&m_hbmImageFill);
	_DeleteObject((HGDIOBJ*)&m_hbmTabCtrlBackground);
	_DeleteObject((HGDIOBJ*)&m_hbmTabCtrlSelected);

	LPCTSTR lpszResourceName = MAKEINTRESOURCE(IDB_BACKGROUND);

	HINSTANCE hInstImageWell = AfxFindResourceHandle(lpszResourceName, RT_BITMAP);

	HRSRC hRsrcImageWell = ::FindResource(hInstImageWell, lpszResourceName, RT_BITMAP);
	if (hRsrcImageWell == NULL)
		return FALSE;

	// load the bitmap
	m_hbmImageBackground = LoadACDSeeBitmap(hInstImageWell, hRsrcImageWell, GetSysColor(COLOR_APPWORKSPACE));
	m_hbmImageFill = LoadACDSeeBitmap(hInstImageWell, hRsrcImageWell, GetSysColor(COLOR_3DFACE));

	hRsrcImageWell = ::FindResource(hInstImageWell, MAKEINTRESOURCE(IDB_TABCTRLBACK), RT_BITMAP);
	m_hbmTabCtrlBackground = LoadACDSeeBitmap(hInstImageWell, hRsrcImageWell, GetSysColor(COLOR_3DFACE));

	hRsrcImageWell = ::FindResource(hInstImageWell, MAKEINTRESOURCE(IDB_TABCTRLLIGHT), RT_BITMAP);
	m_hbmTabCtrlSelected = LoadACDSeeBitmap(hInstImageWell, hRsrcImageWell, GetSysColor(COLOR_3DFACE));

	return TRUE;
}


void CACDSeeTheme::_FillRect(CDC* pDC, CWnd* pWnd, HBITMAP hbm)
{
	CXTPCompatibleDC dc(pDC, CBitmap::FromHandle(hbm));

	CXTPClientRect r(pWnd);

	for (int x = 0; x  < r.Width(); x += 128)
	for (int y = 0; y  < r.Height(); y += 128)
		pDC->BitBlt(x, y, 128, 128, &dc, 0, 0, SRCCOPY);

}

void CACDSeeTheme::FillDockBar(CDC* pDC, CXTPDockBar* pBar)
{
	_FillRect(pDC, ((CWnd*)pBar), m_hbmImageFill);
}

void CACDSeeTheme::FillCommandBarEntry(CDC* pDC, CXTPCommandBar* pBar)
{
	CRect rc;
	pBar->GetClientRect(&rc);


	_FillRect(pDC, pBar, m_hbmImageFill);

	if (pBar->GetPosition() == xtpBarFloating || pBar->GetPosition() == xtpBarPopup)
	{
		Draw3dRect(pDC, rc, COLOR_3DFACE, 0);
		rc.DeflateRect(1, 1);
	}

	Draw3dRect(pDC, rc, COLOR_BTNHILIGHT, COLOR_3DSHADOW);
}
