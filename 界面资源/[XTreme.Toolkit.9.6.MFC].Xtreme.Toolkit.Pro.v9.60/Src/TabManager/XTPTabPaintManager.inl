// XTPTabPaintManager.inl: inlines of the CXTPTabPaintManager class.
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

AFX_INLINE void LineEx(CDC* pDC, COLORREF clr, int x1, int y1, int x2, int y2)
{
	if (clr != XTP_TABMANAGER_COLOR_NONE)
	{
		CXTPPenDC pen(*pDC, clr);
		pDC->MoveTo(x1, y1);
		pDC->LineTo(x2, y2);
	}
}

AFX_INLINE void VerticalLine(CDC* pDC, int x, int y, int nLength, COLORREF clr)
{
	if (clr != XTP_TABMANAGER_COLOR_NONE)
		pDC->FillSolidRect(x, y, 1, nLength, clr);
}

AFX_INLINE void HorizontalLine(CDC* pDC, int x, int y, int nLength, COLORREF clr)
{
	if (clr != XTP_TABMANAGER_COLOR_NONE)
		pDC->FillSolidRect(x, y, nLength, 1, clr);
}
AFX_INLINE void Pixel(CDC* pDC, int x, int y, COLORREF clr)
{
	if (clr != XTP_TABMANAGER_COLOR_NONE)
		pDC->SetPixel(x, y, clr);
}
AFX_INLINE void Draw3dRect(CDC* pDC, CRect rc, COLORREF clrLight, COLORREF clrDark)
{
	if (clrLight != XTP_TABMANAGER_COLOR_NONE && clrDark != XTP_TABMANAGER_COLOR_NONE)
		pDC->Draw3dRect(rc, clrLight, clrDark);
}

AFX_INLINE BOOL IsHorizontalTabPosition(XTPTabPosition position)
{
	return (position == xtpTabPositionBottom || position == xtpTabPositionTop);
}

AFX_INLINE BOOL IsTopLeftTabPosition(XTPTabPosition position)
{
	return (position == xtpTabPositionTop || position == xtpTabPositionLeft);
}

AFX_INLINE void SwapColors(COLORREF& clrLight, COLORREF& clrDark)
{
	COLORREF clr(clrLight);
	clrLight = clrDark;
	clrDark = clr;
}