// Themes.cpp
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

#include "StdAfx.h"
#include "Themes.h"

void CCustomMacTheme::_FillRect(CDC* pDC, CRect rc, int /*nTop*/)
{
	CXTPPenDC pen(*pDC, 0);
	for (int i = /*- nTop % 4*/0 ; i < rc.Height();)
	{
		pen.Color(RGB(251, 251, 251)); pDC->MoveTo(0, i); pDC->LineTo(rc.Width(), i++);
		pen.Color(RGB(253, 253, 253)); pDC->MoveTo(0, i); pDC->LineTo(rc.Width(), i++);
		pen.Color(RGB(251, 251, 251)); pDC->MoveTo(0, i); pDC->LineTo(rc.Width(), i++);
		pen.Color(RGB(239, 239, 239)); pDC->MoveTo(0, i); pDC->LineTo(rc.Width(), i++);

	}
}

void CCustomMacTheme::FillDockBar(CDC* pDC, CXTPDockBar* pBar)
{
	_FillRect(pDC, CXTPClientRect((CWnd*)pBar), CXTPWindowRect((CWnd*)pBar).top);
}
void CCustomMacTheme::FillStatusBar(CDC* pDC, CXTPStatusBar* pBar)
{
	_FillRect(pDC, CXTPClientRect((CWnd*)pBar), CXTPWindowRect((CWnd*)pBar).top);
}


void CCustomMacTheme::FillCommandBarEntry(CDC* pDC, CXTPCommandBar* pBar)
{
	CRect rc;
	pBar->GetClientRect(&rc);


	if (pBar->GetPosition() == xtpBarPopup)
	{
		pDC->FillSolidRect(rc, RGB(251, 251, 251));
		Draw3dRect(pDC, rc, COLOR_3DFACE, 0);
		rc.DeflateRect(1, 1);
	}
	else
	{
		_FillRect(pDC, rc, CXTPWindowRect((CWnd*)pBar).top);

		if (pBar->GetPosition() == xtpBarFloating )
		{
			Draw3dRect(pDC, rc, COLOR_3DFACE, 0);
			rc.DeflateRect(1, 1);
		}
	}
	Draw3dRect(pDC, rc, COLOR_BTNHILIGHT, COLOR_3DSHADOW);
}


void CCustomGrayTheme::FillCommandBarEntry(CDC* pDC, CXTPCommandBar* pBar)
{
	CRect rc;
	pBar->GetClientRect(&rc);
	pDC->FillSolidRect(rc, RGB(221, 221, 221));

	if (pBar->GetPosition() == xtpBarFloating || pBar->GetPosition() == xtpBarPopup)
	{
		Draw3dRect(pDC, rc, COLOR_3DFACE, 0);
		rc.DeflateRect(1, 1);
	}
	Draw3dRect(pDC, rc, COLOR_BTNHILIGHT, COLOR_3DSHADOW);
}

void CCustomGrayTheme::FillDockBar(CDC* pDC, CXTPDockBar* pBar)
{
	pDC->FillSolidRect(CXTPClientRect((CWnd*)pBar), RGB(221, 221, 221));
}
void CCustomGrayTheme::FillStatusBar(CDC* pDC, CXTPStatusBar* pBar)
{
	pDC->FillSolidRect(CXTPClientRect((CWnd*)pBar), RGB(221, 221, 221));
}

void CCustomGrayTheme::DrawRectangle(CDC* pDC, CRect rc, BOOL bSelected, BOOL bPressed, BOOL bEnabled, BOOL bChecked, BOOL bPopuped, BOOL /*bToolBar*/, XTPBarPosition /*barPosition = xtpBarPopup*/)
{
	if ((bSelected || bPressed || bPopuped) && bEnabled)
		pDC->FillSolidRect(rc, RGB(221, 221, 221));

	if (!bEnabled)
	{
		if ( bSelected == TRUE_KEYBOARD) Draw3dRect(pDC, rc, COLOR_3DSHADOW, COLOR_BTNHILIGHT);
	}
	else if (bChecked && !bSelected && !bPressed)
	{
		DrawShadedRect(pDC, rc);
		Draw3dRect(pDC, rc, COLOR_3DSHADOW, COLOR_BTNHILIGHT);
	}
	else if (bChecked ||bPopuped || bPressed == TRUE_KEYBOARD || (bSelected && bPressed)) Draw3dRect(pDC, rc, COLOR_3DSHADOW, COLOR_BTNHILIGHT);
	else if (bSelected || bPressed)  Draw3dRect(pDC, rc, COLOR_BTNHILIGHT, COLOR_3DSHADOW);
}


CSize CCustomGrayTheme::DrawCommandBarGripper(CDC* pDC, CXTPCommandBar* pBar, BOOL bDraw)
{
	CRect rc;
	pBar->GetClientRect(&rc);

	COLORREF clrGripper[] = {RGB(255, 255, 255), RGB(128, 128, 128), RGB(205, 205, 205)};
	CXTPPenDC pen(*pDC, 0);

	if (pBar->IsDialogBar())
	{
		CSize sz(8, max(22, m_nTextHeight + 4));

		if (pDC && bDraw)
		{
			for (int i = 3; i < sz.cy; i++)
			{
				pen.Color(clrGripper[i % 3]); pDC->MoveTo(3, i); pDC->LineTo(8, i);
			}
		}
		return sz;

	} else if (pBar->GetPosition() == xtpBarFloating)
	{
		ASSERT(pDC);
		CXTPFontDC font(pDC, &m_fontSmCaption);
		CSize sz = pDC->GetTextExtent(_T(" "));

		rc.SetRect(3, 3, rc.right - 3, 3 + max(15, sz.cy));
		if (pDC && bDraw)
		{
			pDC->FillSolidRect(rc, GetXtremeColor(COLOR_ACTIVECAPTION));
			pDC->SetTextColor(GetXtremeColor(COLOR_3DHIGHLIGHT));
			pDC->DrawText(pBar->GetTitle(), CRect(5, 3, rc.right - 2* rc.Size().cy, rc.bottom), DT_SINGLELINE | DT_VCENTER );
		}
		return rc.Size();

	}
	else if (pBar->GetPosition() == xtpBarRight || pBar->GetPosition() == xtpBarLeft)
	{
		if (pDC && bDraw)
		{
			//Draw3dRect(pDC, CRect(3, 3, rc.right - 3, 6), COLOR_BTNHILIGHT, COLOR_3DSHADOW);
			for (int i = 3; i < rc.right - 4; i++)
			{
				pen.Color(clrGripper[i % 3]); pDC->MoveTo(i, 3); pDC->LineTo(i, 8);
			}
		}
		return CSize(0, 8);
	}
	else if (pBar->GetPosition() == xtpBarTop || pBar->GetPosition() == xtpBarBottom)
	{
		if (pDC && bDraw)
		{
			for (int i = 3; i < rc.bottom - 4; i++)
			{
				pen.Color(clrGripper[i % 3]); pDC->MoveTo(3, i); pDC->LineTo(8, i);
			}
		}
		return CSize(8, 0);
	}
	return 0;

}


void CCustomGradientTheme::FillCommandBarEntry(CDC* pDC, CXTPCommandBar* pBar)
{
	if (pBar->GetPosition() == xtpBarPopup)
	{
		CRect rc;
		pBar->GetClientRect(&rc);
		XTPPaintManager()->GradientFill(pDC, rc, RGB(248, 242, 224), RGB(255, 255,255), FALSE);
		pDC->Draw3dRect(rc, RGB(160, 130, 35), RGB(160, 130, 35));
	}
	else if (pBar->GetPosition() == xtpBarFloating)
	{
		CXTPDefaultTheme::FillCommandBarEntry(pDC, pBar);
	}
	else
	{
		CWnd* pFrame = pBar->GetParentFrame(), *pWnd = pBar;
		CRect rc, rcParent, rcClient;
		pFrame->GetWindowRect(rcParent);
		pWnd->GetWindowRect(rc);

		pWnd->GetClientRect(rcClient);
		rcClient.TopLeft() += rcParent.TopLeft() -rc.TopLeft();
		rcClient.BottomRight() += rcParent.BottomRight() - rc.BottomRight();

		XTPPaintManager()->GradientFill(pDC, rcClient, RGB(248, 242, 224), RGB(255, 255,255), TRUE);
		pDC->Draw3dRect(CXTPClientRect(pWnd), RGB(255, 255, 255), RGB(248, 242, 224));

	}

}



void CCustomGradientTheme::DrawRectangle(CDC* pDC, CRect rc, BOOL bSelected, BOOL bPressed, BOOL bEnabled, BOOL bChecked, BOOL bPopuped, BOOL /*bToolBar*/, XTPBarPosition /*barPosition = xtpBarPopup*/)
{
	if (!bEnabled)
	{
		if ( bSelected == TRUE_KEYBOARD) Draw3dRect(pDC, rc, COLOR_3DSHADOW, COLOR_BTNHILIGHT);
	}
	else if (bChecked && !bSelected && !bPressed)
	{
		DrawShadedRect(pDC, rc);
		Draw3dRect(pDC, rc, COLOR_3DSHADOW, COLOR_BTNHILIGHT);
	}
	else if (bPopuped)
	{
		pDC->Draw3dRect(rc, RGB(160, 130, 35), RGB(160, 130, 35));
	}
	else if (bChecked || bPressed == TRUE_KEYBOARD || (bSelected && bPressed)) pDC->Draw3dRect(rc, RGB(160, 130, 35), RGB(255, 255, 255));
	else if (bSelected || bPressed)  pDC->Draw3dRect(rc, RGB(255, 255, 255), RGB(160, 130, 35));
}

void CCustomGradientTheme::AdjustExcludeRect(CRect& rc, CXTPControl* pControl, BOOL bVertical)
{
	CXTPCommandBar* pParent = pControl->GetParent();
	ASSERT(pParent);

	if (pControl->GetType() == xtpControlComboBox) return;

	if (pParent->GetType() != xtpBarTypePopup)
	{
		if (bVertical)
			rc.DeflateRect(-1, 0); else rc.DeflateRect(0, -1);
	} else rc.InflateRect(3, 0);

}



void CCustomBevelTheme::DrawUnpressedPushButton(CDC *pDC, const CRect &rect)
{
	pDC->RoundRect(rect, CPoint(6, 6));

	int nLeft = rect.left;
	int nTop = rect.top;
	int nRight = rect.right;
	int nBottom = rect.bottom;

	pDC->SelectObject(&m_penHilight);
	pDC->MoveTo(nLeft + 2, nBottom - 4);
	pDC->LineTo(nLeft + 2, nTop + 2);
	pDC->LineTo(nRight - 3, nTop + 2);
	pDC->SetPixel(nLeft + 3, nTop + 3, m_crHilight);

	pDC->SelectObject(&m_penLiteShadow);
	pDC->MoveTo(nLeft + 3, nBottom - 3);
	pDC->LineTo(nRight - 4, nBottom - 3);
	pDC->LineTo(nRight - 4, nBottom - 4);
	pDC->LineTo(nRight - 3, nBottom - 4);
	pDC->LineTo(nRight - 3, nTop + 2);

	pDC->SelectObject(&m_penShadow);
	pDC->MoveTo(nLeft + 3, nBottom - 2);
	pDC->LineTo(nRight - 3, nBottom - 2);
	pDC->LineTo(nRight - 3, nBottom - 3);
	pDC->LineTo(nRight - 2, nBottom - 3);
	pDC->LineTo(nRight - 2, nTop + 2);

	pDC->SetPixel(nLeft, nTop + 2, m_crDarkDkShadow);
	pDC->SetPixel(nLeft + 2, nTop, m_crDarkDkShadow);
	pDC->SetPixel(nRight - 3, nTop, m_crDarkDkShadow);
	pDC->SetPixel(nRight - 1, nTop + 2, m_crDarkDkShadow);
	pDC->SetPixel(nRight - 1, nBottom - 3, m_crDarkDkShadow);
	pDC->SetPixel(nRight - 3, nBottom - 1, m_crDarkDkShadow);
	pDC->SetPixel(nLeft + 2, nBottom - 1, m_crDarkDkShadow);
	pDC->SetPixel(nLeft, nBottom - 3, m_crDarkDkShadow);

	pDC->SetPixel(nLeft + 1, nTop + 2, m_crLiteShadow);
	pDC->SetPixel(nLeft + 2, nTop + 1, m_crLiteShadow);
	pDC->SetPixel(nRight - 3, nTop + 1, m_crLiteShadow);
	pDC->SetPixel(nRight - 2, nTop + 2, m_crLiteShadow);
	pDC->SetPixel(nLeft + 2, nBottom - 2, m_crLiteShadow);
	pDC->SetPixel(nLeft + 1, nBottom - 3, m_crLiteShadow);

	DrawCornerShadows(pDC, rect);
}   // DrawUnpressedPushButton

void CCustomBevelTheme::DrawCornerShadows(CDC *pDC, const CRect &rect)
{
	pDC->SetPixel(rect.left, rect.top + 2, m_crDarkDkShadow);
	pDC->SetPixel(rect.left + 2, rect.top, m_crDarkDkShadow);
	pDC->SetPixel(rect.right - 3, rect.top, m_crDarkDkShadow);
	pDC->SetPixel(rect.right - 1, rect.top + 2, m_crDarkDkShadow);
	pDC->SetPixel(rect.right - 1, rect.bottom - 3, m_crDarkDkShadow);
	pDC->SetPixel(rect.right - 3, rect.bottom - 1, m_crDarkDkShadow);
	pDC->SetPixel(rect.left + 2, rect.bottom - 1, m_crDarkDkShadow);
	pDC->SetPixel(rect.left, rect.bottom - 3, m_crDarkDkShadow);
}   // DrawCornerShadows

void CCustomBevelTheme::GetColors()
{
	m_crHilight = ::GetSysColor(COLOR_3DHILIGHT);
	m_crLiteFace = ::GetSysColor(COLOR_3DLIGHT);
	m_crFace = ::GetSysColor(COLOR_3DFACE);
	m_crShadow = ::GetSysColor(COLOR_3DSHADOW);
	m_crDkShadow = ::GetSysColor(COLOR_3DDKSHADOW);

	// Create some intermediate colors. This is done by averaging two colors.
	BYTE byRedFace = GetRValue(m_crFace);
	BYTE byGreenFace = GetGValue(m_crFace);
	BYTE byBlueFace = GetBValue(m_crFace);
	BYTE byRedShadow = GetRValue(m_crShadow);
	BYTE byGreenShadow = GetGValue(m_crShadow);
	BYTE byBlueShadow = GetBValue(m_crShadow);
	BYTE byRedDkShadow = GetRValue(m_crDkShadow);
	BYTE byGreenDkShadow = GetGValue(m_crDkShadow);
	BYTE byBlueDkShadow = GetBValue(m_crDkShadow);

	m_crLiteShadow = RGB(byRedFace + ((byRedShadow - byRedFace) >> 1),
		byGreenFace + ((byGreenShadow - byGreenFace) >> 1),
		byBlueFace + ((byBlueShadow - byBlueFace) >> 1));

	m_crDarkDkShadow = RGB(byRedShadow + ((byRedDkShadow - byRedShadow) >> 1),
		byGreenShadow + ((byGreenDkShadow - byGreenShadow) >> 1),
		byBlueShadow + ((byBlueDkShadow - byBlueShadow) >> 1));
}

void CCustomBevelTheme::CreatePens()
{
	DeletePens();

	m_penLiteShadow.CreatePen(PS_SOLID, 1, m_crLiteShadow);
	m_penShadow.CreatePen(PS_SOLID, 1, m_crShadow);
	m_penHilight.CreatePen(PS_SOLID, 1, m_crHilight);
}   // CreatePens


void CCustomBevelTheme::DeletePens()
{
	if (m_penHilight.m_hObject)
		m_penHilight.DeleteObject();
	if (m_penLiteShadow.m_hObject)
		m_penLiteShadow.DeleteObject();
	if (m_penShadow.m_hObject)
		m_penShadow.DeleteObject();
}   // DeletePens

CCustomBevelTheme::CCustomBevelTheme()
{
	RefreshMetrics();
}
void CCustomBevelTheme::RefreshMetrics()
{
	CXTPDefaultTheme::RefreshMetrics();
	GetColors();
	CreatePens();
}
void CCustomBevelTheme::DrawRectangle(CDC* pDC, CRect rc, BOOL bSelected, BOOL bPressed, BOOL bEnabled , BOOL bChecked , BOOL bPopuped , BOOL /*bToolBar*/, XTPBarPosition /*barPosition = xtpBarPopup*/)
{
	if (!bEnabled)
	{

		CBrush brFill(m_crFace);
		CBrush *pOldBrush   = (CBrush *)pDC->SelectObject(&brFill);
		CPen penFrame(PS_SOLID, 1, ::GetSysColor(COLOR_WINDOWFRAME));
		CPen *pOldPen = (CPen *)pDC->SelectObject(&m_penShadow);

		pDC->RoundRect(rc, CPoint(6, 6));

		pDC->SelectObject(pOldBrush);
		pDC->SelectObject(pOldPen);

	} else
	{
		COLORREF crBack = pDC->SetBkColor(GetSysColor(COLOR_3DFACE));
		COLORREF crText = pDC->SetTextColor(GetSysColor(COLOR_3DHIGHLIGHT));

		CBrush brFill;
		if (bChecked && !bPressed)
		{

			WORD bits[] = {
				0x0055, 0x00AA, 0x0055, 0x00AA,
					0x0055, 0x00AA, 0x0055, 0x00AA
			};

			CBitmap bitmap;
			bitmap.CreateBitmap( 8, 8, 1, 1, &bits );

			brFill.CreatePatternBrush( &bitmap );
		}
		else
		{
			brFill.CreateSolidBrush(bPressed || bPopuped ? m_crLiteShadow : m_crFace);
		}

		CBrush *pOldBrush   = (CBrush *)pDC->SelectObject(&brFill);
		CPen penFrame(PS_SOLID, 1, ::GetSysColor(COLOR_WINDOWFRAME));
		CPen *pOldPen = (CPen *)pDC->SelectObject(&penFrame);

		DrawUnpressedPushButton(pDC, rc);

		pDC->SelectObject(pOldBrush);
		pDC->SelectObject(pOldPen);

		if (bSelected == TRUE_KEYBOARD && !bPopuped)
		{
			rc.DeflateRect(2, 2);
			pDC->DrawFocusRect(rc);
		}

		pDC->SetBkColor(crBack);
		pDC->SetTextColor(crText);
	}
}
void CCustomBevelTheme::FillCommandBarEntry(CDC* pDC, CXTPCommandBar* pBar)
{
	if (pBar->GetPosition() == xtpBarPopup || pBar->GetPosition() == xtpBarFloating)
	{

		CRect rc;
		pBar->GetClientRect(&rc);
		pDC->FillSolidRect(rc, GetXtremeColor(COLOR_3DFACE));
		Draw3dRect(pDC, rc, COLOR_BTNTEXT, COLOR_BTNTEXT);
		rc.DeflateRect(1, 1);
		Draw3dRect(pDC, rc, COLOR_BTNHIGHLIGHT, COLOR_3DSHADOW);
	}
	else
	{
		CXTPDefaultTheme::FillCommandBarEntry(pDC, pBar);
	}
}

void CCustomBevelTheme::AdjustExcludeRect(CRect& rc, CXTPControl* pControl, BOOL bVertical)
{
	CXTPCommandBar* pParent = pControl->GetParent();
	ASSERT(pParent);

	if (pControl->GetType() == xtpControlComboBox) return;

	if (pParent->GetType() != xtpBarTypePopup)
	{
		if (bVertical)
			rc.DeflateRect(-1, 0); else rc.DeflateRect(0, -1);
	} else rc.InflateRect(-3, 3);

}


CCustomFancyTheme::CCustomFancyTheme()
{
	m_iconsInfo.bUseFadedIcons = FALSE;
	m_iconsInfo.bIconsWithShadow = FALSE;

	RefreshMetrics();
}


void CCustomFancyTheme::RefreshMetrics()
{
	CXTPOfficeTheme::RefreshMetrics();

	INT nElements[] =      { XPCOLOR_TOOLBAR_GRIPPER, XPCOLOR_SEPARATOR,  XPCOLOR_DISABLED,   XPCOLOR_MENUBAR_FACE, XPCOLOR_MENUBAR_BORDER, XPCOLOR_HIGHLIGHT,  XPCOLOR_HIGHLIGHT_BORDER, XPCOLOR_HIGHLIGHT_PUSHED_BORDER, XPCOLOR_HIGHLIGHT_CHECKED_BORDER, XPCOLOR_HIGHLIGHT_PUSHED, XPCOLOR_HIGHLIGHT_CHECKED, XPCOLOR_TOOLBAR_FACE, XPCOLOR_PUSHED_TEXT };
	COLORREF clrValues[] = { RGB(39, 65, 118),        RGB(106, 140, 203), RGB(109, 150, 208), RGB(255, 255, 220),   RGB(0, 45, 150),        RGB(255, 238, 194), RGB(0, 0, 128),           RGB(0, 0, 128),                  RGB(0, 0, 128),                   RGB(254, 128, 62),        RGB(255, 192, 111),         RGB(215, 241, 215),   RGB(255, 255, 255)};
	SetColors(sizeof(nElements)/sizeof(INT), nElements, clrValues);
}



void CCustomFancyTheme::FillCommandBarEntry(CDC* pDC, CXTPCommandBar* pBar)
{
	if (pBar->GetPosition() == xtpBarPopup)
	{
		CXTPOfficeTheme::FillCommandBarEntry(pDC, pBar);
	}
	else if (pBar->GetPosition() == xtpBarFloating)
	{
		CXTPOfficeTheme::FillCommandBarEntry(pDC, pBar);
	}
	else
	{
		CWnd* pFrame = pBar->GetParentFrame(), *pWnd = pBar;
		CRect rc, rcParent, rcClient;
		pFrame->GetWindowRect(rcParent);
		pWnd->GetWindowRect(rc);

		pWnd->GetClientRect(rcClient);
		rcClient.TopLeft() += rcParent.TopLeft() -rc.TopLeft();
		rcClient.BottomRight() += rcParent.BottomRight() - rc.BottomRight();

		XTPPaintManager()->GradientFill(pDC, rcClient, RGB(255, 220, 220), RGB(150, 254, 150), TRUE);
	}

}

COLORREF DarkenColor( long lScale, COLORREF lColor)
{
	long red   = MulDiv(GetRValue(lColor),(255-lScale),255);
	long green = MulDiv(GetGValue(lColor),(255-lScale),255);
	long blue  = MulDiv(GetBValue(lColor),(255-lScale),255);

	return RGB(red, green, blue);
}
COLORREF LightenColor( long lScale, COLORREF lColor)
{
	long R = MulDiv(255-GetRValue(lColor),lScale,255)+GetRValue(lColor);
	long G = MulDiv(255-GetGValue(lColor),lScale,255)+GetGValue(lColor);
	long B = MulDiv(255-GetBValue(lColor),lScale,255)+GetBValue(lColor);

	return RGB(R, G, B);
}

void CCustomIcyTheme::DrawPopupBarGripper(CDC* pDC, int x, int y, int cx, int cy, BOOL /*bExpanded*/)
{
	GradientFill(pDC, CRect(CPoint(x, y), CSize(cx, cy)), DarkenColor(30, GetXtremeColor(COLOR_3DFACE)), LightenColor(30, GetXtremeColor(COLOR_3DFACE)), TRUE);
}

CSize CCustomIcyTheme::DrawCommandBarGripper(CDC* pDC, CXTPCommandBar* pBar, BOOL bDraw)
{
	CRect rc;
	pBar->GetClientRect(&rc);

	if (pBar->IsDialogBar())
	{
		CSize sz(8, max(22, m_nTextHeight + 4));

		if (pDC && bDraw)
		{
			for (int y = 5; y < sz.cy - 2; y += 4)
			{
				pDC->FillSolidRect(CRect(4 + 2, y + 1, 6 + 2, y + 3), GetXtremeColor(COLOR_BTNHIGHLIGHT));
				pDC->FillSolidRect(CRect(3 + 2, y, 5 + 2, y + 2), DarkenColor(60, GetXtremeColor(COLOR_3DFACE)));
			}
		}
		return sz;

	} else if (pBar->GetPosition() == xtpBarFloating)
	{
		return CXTPOfficeTheme::DrawCommandBarGripper(pDC, pBar, bDraw);

	} else if (pBar->GetPosition() == xtpBarRight || pBar->GetPosition() == xtpBarLeft)
	{
		if (!pDC || !bDraw) return CSize(0, 8 + 1);
		for (int x = 5; x < rc.Width() - 8; x += 4)
		{
			pDC->FillSolidRect(CRect(x + 1, 4 + 3, x + 3, 6 + 3), GetXtremeColor(COLOR_BTNHIGHLIGHT));
			pDC->FillSolidRect(CRect(x, 3 + 3, x + 2, 5 + 3), DarkenColor(60, GetXtremeColor(COLOR_3DFACE)));
		}
	}
	else if (pBar->GetPosition() == xtpBarTop || pBar->GetPosition() == xtpBarBottom)
	{
		if (!pDC || !bDraw) return CSize(6 + 2 - 2, 0);
		for (int y = 5; y < rc.Height() - 8; y += 4)
		{
			pDC->FillSolidRect(CRect(4 + 2, y + 1, 6 + 2, y + 3), GetXtremeColor(COLOR_BTNHIGHLIGHT));
			pDC->FillSolidRect(CRect(3 + 2, y, 5 + 2, y + 2), DarkenColor(60, GetXtremeColor(COLOR_3DFACE)));
		}
	}
	return 0;
}

CSize CCustomIcyTheme::DrawCommandBarSeparator(CDC* pDC, CXTPCommandBar* pBar, CXTPControl* pControl, BOOL bDraw)
{
	if (!bDraw)
	{
		return CXTPOfficeTheme::DrawCommandBarSeparator(pDC, pBar, pControl, bDraw);
	}
	CRect rc;
	pBar->GetClientRect(&rc);
	CRect rcControl = pControl->GetRect();
	CRect rcRaw = pControl->GetRawRect();

	if (pBar->GetPosition() == xtpBarRight || pBar->GetPosition() == xtpBarLeft)
	{
		if (!pControl->GetWrap())
		{
			Line(pDC, rcRaw.left + 4, rcControl.top - 4, rcRaw.right - 4, rcControl.top - 4, XPCOLOR_3DSHADOW);
			Line(pDC, rcRaw.left + 5, rcControl.top - 3, rcRaw.right - 3, rcControl.top - 3, COLOR_3DHIGHLIGHT);
		}
		else
		{
			Line(pDC, rcRaw.right + 2, rcRaw.top + 4, rcRaw.right + 2, rcRaw.bottom, XPCOLOR_3DSHADOW);
			Line(pDC, rcRaw.right + 3, rcRaw.top + 5, rcRaw.right + 3, rcRaw.bottom, COLOR_3DHIGHLIGHT);
		}

	} else if (pBar->GetPosition() == xtpBarTop || pBar->GetPosition() == xtpBarBottom)
	{
		if (!pControl->GetWrap())
		{
			Line(pDC, rcControl.left - 4, rcRaw.top + 4, rcControl.left - 4, rcRaw.bottom - 4, XPCOLOR_3DSHADOW);
			Line(pDC, rcControl.left - 3, rcRaw.top + 5, rcControl.left - 3, rcRaw.bottom - 3, COLOR_3DHIGHLIGHT);
		}
		else
		{
			Line(pDC, rcRaw.left + 4, rcRaw.top - 4, rcRaw.right, rcRaw.top - 4, XPCOLOR_3DSHADOW);
			Line(pDC, rcRaw.left + 5, rcRaw.top - 3, rcRaw.right, rcRaw.top - 3, COLOR_3DHIGHLIGHT);
		}
	}
	else return CXTPOfficeTheme::DrawCommandBarSeparator(pDC, pBar, pControl, bDraw);

	return 0;
}


void CCustomIcyTheme::FillCommandBarEntry(CDC* pDC, CXTPCommandBar* pBar)
{
	CXTPClientRect rc((CWnd*)pBar);

	if (pBar->GetPosition() == xtpBarPopup)
	{

		Rectangle(pDC, rc, XPCOLOR_MENUBAR_BORDER, COLOR_3DFACE);

		if (pBar->GetType() == xtpBarTypePopup)
		{
			COLORREF color3DHilight = DarkenColor(30, GetXtremeColor(COLOR_3DFACE));
			COLORREF color3DShadow = LightenColor(30, GetXtremeColor(COLOR_3DFACE));

			for (int i = 0; i < pBar->GetControls()->GetCount(); i++)
			{
				CXTPControl* pControl = pBar->GetControl(i);
				if (pControl->IsVisible())
					XTPPaintManager()->GradientFill(pDC, pControl->GetRect(), color3DShadow, color3DHilight, FALSE);
			}
		}

		CXTPPopupBar* pPopupBar = DYNAMIC_DOWNCAST(CXTPPopupBar, pBar);
		FillIntersectRect(pDC, pPopupBar, GetXtremeColor(COLOR_3DFACE));

	}
	else if (pBar->GetPosition() == xtpBarFloating)
	{
		CXTPOfficeTheme::FillCommandBarEntry(pDC, pBar);
	}
	else if (pBar->GetPosition() == xtpBarTop || pBar->GetPosition() == xtpBarBottom )
	{
		COLORREF clr3DFace = GetXtremeColor(COLOR_3DFACE);
		pDC->FillSolidRect(rc, clr3DFace);

		CWnd* pWnd = pBar->GetParent();
		if (pWnd)
		{
			pDC->ExcludeClipRect(rc.left, rc.top, rc.left + 3, rc.bottom);
			XTPDrawHelpers()->ExcludeCorners(pDC, CRect(rc.left + 3, rc.top + 1, rc.right, rc.bottom - 1));
		}

		COLORREF color3DHilight = DarkenColor(30, GetXtremeColor(COLOR_3DFACE));
			COLORREF color3DShadow = LightenColor(30, GetXtremeColor(COLOR_3DFACE));

		GradientFill(pDC, rc, color3DShadow, color3DHilight, FALSE);

		pDC->FillSolidRect(rc.left + 5, rc.bottom - 1, rc.Width() - 7, 1, DarkenColor(60, GetXtremeColor(COLOR_3DFACE)));
		pDC->SelectClipRgn(0);
	}
	else if (pBar->GetPosition() == xtpBarLeft || pBar->GetPosition() == xtpBarRight )
	{
		COLORREF clr3DFace = GetXtremeColor(COLOR_3DFACE);
		pDC->FillSolidRect(rc, clr3DFace);

		CWnd* pWnd = pBar->GetParent();
		if (pWnd)
		{
			pDC->ExcludeClipRect(rc.left, rc.top, rc.right, rc.top + 3);
			XTPDrawHelpers()->ExcludeCorners(pDC, CRect(rc.left + 1, rc.top + 3, rc.right - 1, rc.bottom));
		}
		COLORREF color3DHilight = DarkenColor(30, GetXtremeColor(COLOR_3DFACE));
			COLORREF color3DShadow = LightenColor(30, GetXtremeColor(COLOR_3DFACE));


		GradientFill(pDC, rc, color3DShadow, color3DHilight, TRUE);
		pDC->FillSolidRect(rc.right - 1, rc.top + 5 , 1, rc.Height() - 7, DarkenColor(60, GetXtremeColor(COLOR_3DFACE)));

		pDC->SelectClipRgn(0);
	}
	else
	{
		CXTPOfficeTheme::FillCommandBarEntry(pDC, pBar);
	}


}

void CCustomFancyTheme::FillDockBar(CDC* pDC, CXTPDockBar* pBar)
{
		CWnd* pFrame = pBar->GetParentFrame(), *pWnd = pBar;
		CRect rc, rcParent, rcClient;
		pFrame->GetWindowRect(rcParent);
		pWnd->GetWindowRect(rc);

		pWnd->GetClientRect(rcClient);
		rcClient.TopLeft() += rcParent.TopLeft() -rc.TopLeft();
		rcClient.BottomRight() += rcParent.BottomRight() - rc.BottomRight();

		XTPPaintManager()->GradientFill(pDC, rcClient, RGB(255, 220, 220), RGB(150, 254, 150), TRUE);
}
