// XTPOffice2003Theme.cpp : implementation of the CXTPOffice2003Theme class.
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
#include "Tmschema.h"
#include "XTPVC80Helpers.h"
#include "XTPImageManager.h"
#include "XTPDrawHelpers.h"

#include "XTPCommandBarsResource.h"
#include "XTPPaintManager.h"
#include "XTPControlButton.h"
#include "XTPControlPopup.h"
#include "XTPColorManager.h"
#include "XTPToolBar.h"
#include "XTPPopupBar.h"
#include "XTPControlComboBox.h"
#include "XTPControls.h"
#include "XTPShadowsManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace XTPPaintThemes;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


CXTPOffice2003Theme::CXTPOffice2003Theme()
{
	m_iconsInfo.bUseFadedIcons = FALSE;
	m_iconsInfo.bIconsWithShadow = FALSE;

	RefreshMetrics();
}

CXTPOffice2003Theme::~CXTPOffice2003Theme()
{

}





void CXTPOffice2003Theme::RefreshMetrics()
{
	CXTPOfficeTheme::RefreshMetrics();

	HWND hWnd = AfxGetMainWnd()? AfxGetMainWnd()->GetSafeHwnd(): 0;
	m_themeStatusBar.OpenThemeData(hWnd, L"STATUS");


	m_clrMenuExpandedDark = GetXtremeColor(XPCOLOR_MENUBAR_EXPANDED);
	m_clrMenuExpandedLight = XTPColorManager()->LightColor(GetXtremeColor(COLOR_WINDOW), m_clrMenuExpandedDark, 550);

	m_clrCommandBarLight = m_clrDockBarLight = XTPColorManager()->LightColor(GetXtremeColor(COLOR_3DFACE), GetXtremeColor(COLOR_WINDOW), 0xcd);
	m_clrCommandBarDark = m_clrDockBarDark = GetXtremeColor(COLOR_3DFACE);
	m_crlToolbarShadow = GetXtremeColor(XPCOLOR_TOOLBAR_FACE);
	m_crlFloatingGripper = GetXtremeColor(COLOR_3DSHADOW);
	m_clrPopupDark = GetXtremeColor(XPCOLOR_MENUBAR_FACE);
	m_clrPopupLight = XTPColorManager()->LightColor(GetXtremeColor(COLOR_WINDOW), GetXtremeColor(COLOR_3DFACE), 550);

	m_crlExpandNormalDark = GetXtremeColor(COLOR_3DSHADOW);
	m_crlExpandNormalLight = XTPColorManager()->LightColor(GetXtremeColor(XPCOLOR_3DFACE), m_crlExpandNormalDark, 51);

	m_bLunaTheme = FALSE;

	m_clrCircleLight = GetXtremeColor(XPCOLOR_MENUBAR_FACE);
	m_clrCircleDark = XTPColorManager()->LightColor(GetXtremeColor(COLOR_WINDOW), GetXtremeColor(COLOR_3DFACE), 550);

	m_clrStatusPane = XTPColorManager()->LightColor(GetXtremeColor(COLOR_3DFACE), GetXtremeColor(COLOR_WINDOW), 70);


	m_arrColor[XPCOLOR_SPLITTER_FACE].SetStandardValue(XTPColorManager()->grcDockBar.clrDark);



	switch (GetCurrentSystemTheme())
	{
	case xtpSystemThemeBlue:
		{
			m_clrDockBarDark = RGB(158, 190, 245);
			m_clrDockBarLight = RGB(196, 218, 250);
			m_crlToolbarShadow = RGB(59, 97, 156);
			m_clrCommandBarDark = RGB(129, 169, 226);
			m_clrCommandBarLight = RGB(221, 236, 254);
			m_crlFloatingGripper = RGB(42, 102, 201);
			m_clrPopupDark = RGB(227, 239, 255);
			m_clrPopupLight = RGB(147, 181, 231);
			m_crlExpandNormalDark  = RGB(0, 53, 145);
			m_crlExpandNormalLight  = RGB(117, 166, 241);

			m_clrMenuExpandedDark = RGB(121, 161, 220);
			m_clrMenuExpandedLight = RGB(203, 221, 246);

			m_clrCircleLight = 0xFEEFE3;
			m_clrCircleDark = 0xE2A981;

			m_clrStatusPane = RGB(211, 211, 211);
			m_clrShadowFactor = RGB(0x16, 0x1E, 0x36);

			m_arrColor[XPCOLOR_EDITCTRLBORDER].SetStandardValue(RGB(203, 225, 252));
			m_arrColor[XPCOLOR_3DFACE].SetStandardValue(RGB(216, 231, 252));
			m_arrColor[XPCOLOR_3DSHADOW].SetStandardValue(RGB(158, 190, 245));
			m_arrColor[COLOR_APPWORKSPACE].SetStandardValue(RGB(144, 153, 174));
			m_arrColor[XPCOLOR_LABEL].SetStandardValue(RGB(189, 211, 247));

			INT nElements[] =      { XPCOLOR_TOOLBAR_GRIPPER, XPCOLOR_SEPARATOR,  XPCOLOR_DISABLED,   XPCOLOR_MENUBAR_FACE, XPCOLOR_MENUBAR_BORDER, XPCOLOR_HIGHLIGHT,  XPCOLOR_HIGHLIGHT_BORDER, XPCOLOR_HIGHLIGHT_PUSHED_BORDER, XPCOLOR_HIGHLIGHT_CHECKED_BORDER, XPCOLOR_HIGHLIGHT_PUSHED, XPCOLOR_HIGHLIGHT_CHECKED, XPCOLOR_TOOLBAR_FACE, XPCOLOR_PUSHED_TEXT };
			COLORREF clrValues[] = { RGB(39, 65, 118),        RGB(106, 140, 203), RGB(109, 150, 208), RGB(246, 246, 246),   RGB(0, 45, 150),        RGB(255, 238, 194), RGB(0, 0, 128),           RGB(0, 0, 128),                  RGB(0, 0, 128),                   RGB(254, 128, 62),        RGB(255, 192, 111),         RGB(158, 190, 245),   0};
			SetColors(sizeof(nElements)/sizeof(INT), nElements, clrValues);

			m_bLunaTheme = TRUE;
		}
		break;

	case xtpSystemThemeOlive:
		{
			m_clrDockBarDark = RGB(217, 217, 167);
			m_clrDockBarLight = RGB(242, 241, 228);
			m_crlToolbarShadow = RGB(96, 128, 88);

			m_clrCommandBarDark = RGB(183, 198, 145);
			m_clrCommandBarLight = RGB(244, 247, 222);
			m_crlFloatingGripper = RGB(116, 134, 94);
			m_clrPopupLight = RGB(194, 206, 159);
			m_clrPopupDark = RGB(236, 240, 213);
			m_crlExpandNormalDark  = RGB(96, 119, 107);
			m_crlExpandNormalLight  = RGB(176, 194, 140);

			m_clrMenuExpandedDark = RGB(164, 180, 120);
			m_clrMenuExpandedLight = RGB(230, 230, 209);

			m_clrCircleLight = 0xD5F0EC;
			m_clrCircleDark = 0x93C6B8;

			m_clrStatusPane = RGB(211, 211, 211);
			m_clrShadowFactor = RGB(0x1F, 0x22, 0x19);

			m_arrColor[XPCOLOR_EDITCTRLBORDER].SetStandardValue(RGB(216, 227, 182));
			m_arrColor[XPCOLOR_3DFACE].SetStandardValue(RGB(226, 231, 191));
			m_arrColor[XPCOLOR_3DSHADOW].SetStandardValue(RGB(171, 192, 138));
			m_arrColor[COLOR_APPWORKSPACE].SetStandardValue(RGB(151, 160, 123));
			m_arrColor[XPCOLOR_LABEL].SetStandardValue(RGB(222, 227, 189));


			INT nElements[] =      { XPCOLOR_TOOLBAR_GRIPPER, XPCOLOR_SEPARATOR,  XPCOLOR_DISABLED,   XPCOLOR_MENUBAR_FACE, XPCOLOR_MENUBAR_BORDER, XPCOLOR_HIGHLIGHT,  XPCOLOR_HIGHLIGHT_BORDER, XPCOLOR_HIGHLIGHT_CHECKED_BORDER, XPCOLOR_HIGHLIGHT_PUSHED_BORDER, XPCOLOR_HIGHLIGHT_PUSHED,  XPCOLOR_HIGHLIGHT_CHECKED, XPCOLOR_TOOLBAR_FACE, XPCOLOR_PUSHED_TEXT };
			COLORREF clrValues[] = { RGB(81, 94, 51),         RGB(96, 128, 88),    RGB(159, 174, 122), RGB(244, 244, 238),   RGB(117, 141, 94),      RGB(255, 238, 194), RGB(63, 93, 56),          RGB(63, 93, 56),                  RGB(63, 93, 56),                 RGB(254, 128, 62),          RGB(255, 192, 111),        RGB(217, 217, 167),   0};
			SetColors(sizeof(nElements)/sizeof(INT), nElements, clrValues);

			m_bLunaTheme = TRUE;
		}
		break;

	case xtpSystemThemeSilver:
		{
			m_clrDockBarDark = RGB(215, 215, 229);
			m_clrDockBarLight = RGB(243, 243, 247);
			m_crlToolbarShadow = RGB(124, 124, 148);

			m_clrCommandBarDark = RGB(153, 151, 181);
			m_clrCommandBarLight = RGB(243, 244, 250);
			m_crlFloatingGripper = RGB(122, 121, 153);
			m_clrPopupLight = RGB(186, 185, 205);
			m_clrPopupDark = RGB(233, 231, 241);
			m_crlExpandNormalDark  = RGB(118, 116, 146);
			m_crlExpandNormalLight  = RGB(179, 178, 200);

			m_clrMenuExpandedDark = RGB(133, 131, 162);
			m_clrMenuExpandedLight = RGB(215, 215, 226);

			m_clrCircleLight = 0xF1E8E8;
			m_clrCircleDark = 0xC5AEB0;

			m_clrStatusPane = RGB(236, 234, 218);
			m_clrShadowFactor = RGB(0x24, 0x20, 0x29);


			m_arrColor[XPCOLOR_EDITCTRLBORDER].SetStandardValue(RGB(214, 211, 231));
			m_arrColor[XPCOLOR_3DFACE].SetStandardValue(RGB(223, 223, 234));
			m_arrColor[XPCOLOR_3DSHADOW].SetStandardValue(RGB(177, 176, 195));
			m_arrColor[COLOR_APPWORKSPACE].SetStandardValue(RGB(155, 154, 179));
			m_arrColor[XPCOLOR_LABEL].SetStandardValue(RGB(214, 215, 231));

			INT nElements[] =      { XPCOLOR_TOOLBAR_GRIPPER, XPCOLOR_SEPARATOR,  XPCOLOR_DISABLED,   XPCOLOR_MENUBAR_FACE, XPCOLOR_MENUBAR_BORDER, XPCOLOR_HIGHLIGHT,  XPCOLOR_HIGHLIGHT_BORDER, XPCOLOR_HIGHLIGHT_CHECKED_BORDER, XPCOLOR_HIGHLIGHT_PUSHED_BORDER, XPCOLOR_HIGHLIGHT_PUSHED,  XPCOLOR_HIGHLIGHT_CHECKED, XPCOLOR_TOOLBAR_FACE, XPCOLOR_PUSHED_TEXT };
			COLORREF clrValues[] = { RGB(84, 84, 117),        RGB(110, 109, 143), RGB(168, 167, 190), RGB(253, 250, 255),   RGB(124, 124, 148),     RGB(255, 238, 194), RGB(75, 75, 111),          RGB(75, 75, 111),                  RGB(75, 75, 111),              RGB(254, 128, 62),         RGB(255, 192, 111),        RGB(215, 215, 229),   0};
			SetColors(sizeof(nElements)/sizeof(INT), nElements, clrValues);

			m_bLunaTheme = TRUE;
		}
	}



	CreateGradientCircle();
}



void CXTPOffice2003Theme::FillDockBar(CDC* pDC, CXTPDockBar* pBar)
{
	if (((CWnd*)pBar)->GetStyle() & CBRS_RIGHT)
		pDC->FillSolidRect(CXTPClientRect((CWnd*)pBar), m_clrDockBarLight);
	else
	{
		CXTPClientRect rc((CWnd*)pBar);
		rc.right = rc.left + max(rc.Width(), GetSystemMetrics(SM_CXFULLSCREEN) /2);
		GradientFill(pDC, rc, m_clrDockBarDark, m_clrDockBarLight, TRUE);
	}
}
void CXTPOffice2003Theme::ExcludeCorners(CDC* pDC, CRect rc)
{
	pDC->ExcludeClipRect(rc.left, rc.top, rc.left + 1, rc.top + 1);
	pDC->ExcludeClipRect(rc.right - 1, rc.top, rc.right, rc.top + 1);
	pDC->ExcludeClipRect(rc.left, rc.bottom - 1, rc.left + 1, rc.bottom + 1);
	pDC->ExcludeClipRect(rc.right - 1, rc.bottom - 1, rc.right, rc.bottom + 1);
}

void CXTPOffice2003Theme::DrawPopupBarGripper(CDC* pDC, int x, int y, int cx, int cy, BOOL bExpanded)
{
	if (!bExpanded)
	{
		GradientFill(pDC, CRect(CPoint(x, y), CSize(cx, cy)), m_clrCommandBarLight, m_clrCommandBarDark, TRUE);
	} else
	{
		GradientFill(pDC, CRect(CPoint(x, y), CSize(cx, cy)), m_clrMenuExpandedLight, m_clrMenuExpandedDark, TRUE);
	}
}


void CXTPOffice2003Theme::FillCommandBarEntry(CDC* pDC, CXTPCommandBar* pBar)
{
	CXTPClientRect rc((CWnd*)pBar);

	if (pBar->GetType() == xtpBarTypeMenuBar)
	{
		CXTPClientRect rcFill((CWnd*)pBar);
		rcFill.right = rcFill.left + max(rcFill.Width(), GetSystemMetrics(SM_CXFULLSCREEN) /2);
		GradientFill(pDC, rcFill, m_clrDockBarDark, m_clrDockBarLight, TRUE);

		if (pBar->GetPosition() == xtpBarFloating)
		{
			COLORREF clr = GetXtremeColor(XPCOLOR_MENUBAR_BORDER);
			pDC->Draw3dRect(rc, clr, clr);
			rc.DeflateRect(1, 1);
			pDC->Draw3dRect(rc, clr, clr);

			pDC->SetPixel(2, 2, clr);
			pDC->SetPixel(rc.right - 2, 2, clr);
			pDC->SetPixel(2, rc.bottom - 2, clr);
			pDC->SetPixel(rc.right - 2, rc.bottom - 2, clr);
		}
	}
	else if (pBar->GetPosition() == xtpBarTop || pBar->GetPosition() == xtpBarBottom )
	{
		COLORREF clr3DFace = GetXtremeColor(COLOR_3DFACE);
		pDC->FillSolidRect(rc, clr3DFace);

		CWnd* pWnd = pBar->GetParent();
		if (pWnd)
		{
			CXTPWindowRect rcDockBar((CWnd*)pWnd);
			pBar->ScreenToClient(rcDockBar);
			rcDockBar.right = rcDockBar.left + GetSystemMetrics(SM_CXFULLSCREEN);
			GradientFill(pDC, rcDockBar, m_clrDockBarDark, m_clrDockBarLight, TRUE);

			pDC->ExcludeClipRect(rc.left, rc.top, rc.left + 3, rc.bottom);
			ExcludeCorners(pDC, CRect(rc.left + 3, rc.top + 1, rc.right, rc.bottom - 1));
		}

		CRect rcFill = rc; rcFill.top += 1;

		int nFirst = pBar->GetControls()->GetNext(-1, +1, FALSE, TRUE);
		if (nFirst != -1)
		{
			CRect rcRaw = pBar->GetControls()->GetAt(nFirst)->GetRawRect();
			rcFill.bottom = rcRaw.bottom + 1;
		}
		GradientFill(pDC, rcFill, m_clrCommandBarLight, m_clrCommandBarDark, FALSE);
		pDC->FillSolidRect(rcFill.left, rcFill.bottom,  rcFill.Width(), rc.Height() - rcFill.Height() - 2, m_clrCommandBarDark);

		pDC->FillSolidRect(rc.left + 5, rc.bottom - 1, rc.Width() - 7, 1, m_crlToolbarShadow);

		pDC->SelectClipRgn(0);
	}
	else if (pBar->GetPosition() == xtpBarLeft || pBar->GetPosition() == xtpBarRight )
	{
		COLORREF clr3DFace = GetXtremeColor(COLOR_3DFACE);
		pDC->FillSolidRect(rc, clr3DFace);

		CWnd* pWnd = pBar->GetParent();
		if (pWnd)
		{
			if (pWnd->GetStyle() & CBRS_RIGHT)
				pDC->FillSolidRect(rc, m_clrDockBarLight);
			else
			{
				CXTPWindowRect rcDockBar((CWnd*)pWnd);
				pBar->ScreenToClient(rcDockBar);
				rcDockBar.right = rcDockBar.left + GetSystemMetrics(SM_CXFULLSCREEN);
				GradientFill(pDC, rcDockBar, m_clrDockBarDark, m_clrDockBarLight, TRUE);
			}

			pDC->ExcludeClipRect(rc.left, rc.top, rc.right, rc.top + 3);
			ExcludeCorners(pDC, CRect(rc.left + 1, rc.top + 3, rc.right - 1, rc.bottom));
		}

		CRect rcFill = rc; rcFill.left += 1;

		int nFirst = pBar->GetControls()->GetNext(-1, +1, FALSE, TRUE);
		if (nFirst != -1)
		{
			CRect rcRaw = pBar->GetControls()->GetAt(nFirst)->GetRawRect();
			rcFill.right = rcRaw.right + 1;
		}
		GradientFill(pDC, rcFill, m_clrCommandBarLight, m_clrCommandBarDark, TRUE);
		pDC->FillSolidRect(rc.right - 1, rc.top + 5 , 1, rc.Height() - 7, m_crlToolbarShadow);

		pDC->SelectClipRgn(0);
	}
	else if (pBar->GetPosition() == xtpBarPopup)
	{
		Rectangle(pDC, rc, XPCOLOR_MENUBAR_BORDER, XPCOLOR_MENUBAR_FACE);

		if (pBar->GetType() == xtpBarTypePopup)
			DrawPopupBarGripper(pDC, 1, 2, GetGripperWidth(pBar), rc.bottom - 4);

		CXTPPopupBar* pPopupBar = DYNAMIC_DOWNCAST(CXTPPopupBar, pBar);
		FillIntersectRect(pDC, pPopupBar, GetXtremeColor(XPCOLOR_MENUBAR_FACE));
	}
	else
	CXTPOfficeTheme::FillCommandBarEntry(pDC, pBar);
}

CSize CXTPOffice2003Theme::DrawCommandBarSeparator(CDC* pDC, CXTPCommandBar* pBar, CXTPControl* pControl, BOOL bDraw)
{
	if (!bDraw)
	{
		return CXTPOfficeTheme::DrawCommandBarSeparator(pDC, pBar, pControl, bDraw);
	}
	CRect rc;
	pBar->GetClientRect(&rc);
	CRect rcControl = pControl->GetRect();
	CXTPPenDC pen (*pDC, GetXtremeColor(XPCOLOR_SEPARATOR));
	CRect rcRaw = pControl->GetRawRect();

	if (pBar->GetPosition() == xtpBarRight || pBar->GetPosition() == xtpBarLeft)
	{
		if (!pControl->GetWrap())
		{
			Line(pDC, rcRaw.left + 4, rcControl.top - 4, rcRaw.right - 4, rcControl.top - 4, XPCOLOR_SEPARATOR);
			Line(pDC, rcRaw.left + 5, rcControl.top - 3, rcRaw.right - 3, rcControl.top - 3, COLOR_3DHIGHLIGHT);
		}
		else
		{
			Line(pDC, rcRaw.right + 2, rcRaw.top + 4, rcRaw.right + 2, rcRaw.bottom, XPCOLOR_SEPARATOR);
			Line(pDC, rcRaw.right + 3, rcRaw.top + 5, rcRaw.right + 3, rcRaw.bottom, COLOR_3DHIGHLIGHT);
		}

	} else if (pBar->GetPosition() == xtpBarTop || pBar->GetPosition() == xtpBarBottom)
	{
		if (!pControl->GetWrap())
		{
			Line(pDC, rcControl.left - 4, rcRaw.top + 4, rcControl.left - 4, rcRaw.bottom - 4, XPCOLOR_SEPARATOR);
			Line(pDC, rcControl.left - 3, rcRaw.top + 5, rcControl.left - 3, rcRaw.bottom - 3, COLOR_3DHIGHLIGHT);
		}
		else
		{
			Line(pDC, rcRaw.left + 4, rcRaw.top - 4, rcRaw.right, rcRaw.top - 4, XPCOLOR_SEPARATOR);
			Line(pDC, rcRaw.left + 5, rcRaw.top - 3, rcRaw.right, rcRaw.top - 3, COLOR_3DHIGHLIGHT);
		}
	}
	else return CXTPOfficeTheme::DrawCommandBarSeparator(pDC, pBar, pControl, bDraw);

	return 0;
}

CSize CXTPOffice2003Theme::DrawDialogBarGripper(CDC* pDC, CXTPCommandBar* pBar, BOOL bDraw)
{
	CSize sz(8, max(25, m_nTextHeight + 6));

	if (pDC && bDraw)
	{
		CRect rcGripper;
		pBar->GetClientRect(&rcGripper);

		rcGripper.DeflateRect(3, 3);
		rcGripper.bottom = rcGripper.top + sz.cy;

		GradientFill(pDC, rcGripper, m_clrCommandBarLight, m_clrCommandBarDark, FALSE);


		for (int y = rcGripper.top + 4; y < rcGripper.bottom - 5; y += 4)
		{
			pDC->FillSolidRect(CRect(4 + 2, y + 1, 6 + 2, y + 3), GetXtremeColor(COLOR_BTNHIGHLIGHT));
			pDC->FillSolidRect(CRect(3 + 2, y, 5 + 2, y + 2), GetXtremeColor(XPCOLOR_TOOLBAR_GRIPPER));
		}
	}

	return sz;
}

CSize CXTPOffice2003Theme::DrawCommandBarGripper(CDC* pDC, CXTPCommandBar* pBar, BOOL bDraw)
{
	if (pBar->IsDialogBar())
		return DrawDialogBarGripper(pDC, pBar, bDraw);

	CRect rc;
	pBar->GetClientRect(&rc);

	if (pBar->GetPosition() == xtpBarFloating)
	{
		ASSERT(pDC);
		CXTPFontDC font(pDC, &m_fontSmCaption);
		CSize sz = pDC->GetTextExtent(_T(" "));

		rc.SetRect(3, 3, rc.right - 3, 3 + max(15, sz.cy));
		if (pDC && bDraw)
		{
			pDC->FillSolidRect(rc, m_crlFloatingGripper);
			pDC->SetTextColor(GetXtremeColor(COLOR_3DHIGHLIGHT));
			pDC->DrawText(pBar->GetTitle(), CRect(5, 3, rc.right - 2* rc.Size().cy, rc.bottom), DT_SINGLELINE | DT_VCENTER );
		}
		return rc.Size();

	} else if (pBar->GetPosition() == xtpBarRight || pBar->GetPosition() == xtpBarLeft)
	{
		if (!pDC || !bDraw) return CSize(0, 8 + 1);
		for (int x = 5; x < rc.Width() - 8; x += 4)
		{
			pDC->FillSolidRect(CRect(x + 1, 4 + 3, x + 3, 6 + 3), GetXtremeColor(COLOR_BTNHIGHLIGHT));
			pDC->FillSolidRect(CRect(x, 3 + 3, x + 2, 5 + 3), GetXtremeColor(XPCOLOR_TOOLBAR_GRIPPER));
		}
	}
	else if (pBar->GetPosition() == xtpBarTop || pBar->GetPosition() == xtpBarBottom)
	{
		if (!pDC || !bDraw) return CSize(6 + 2 - 2, 0);
		for (int y = 5; y < rc.Height() - 8; y += 4)
		{
			pDC->FillSolidRect(CRect(4 + 2, y + 1, 6 + 2, y + 3), GetXtremeColor(COLOR_BTNHIGHLIGHT));
			pDC->FillSolidRect(CRect(3 + 2, y, 5 + 2, y + 2), GetXtremeColor(XPCOLOR_TOOLBAR_GRIPPER));
		}
	}
	return 0;

}

void CXTPOffice2003Theme::RectangleEx(CDC* pDC, CRect rc, int nColorBorder, BOOL bHoriz, COLORREF clrLight, COLORREF clrDark)
{
	GradientFill(pDC, rc, clrLight, clrDark, bHoriz);
	Draw3dRect(pDC, rc, nColorBorder, nColorBorder);
}

void CXTPOffice2003Theme::DrawRectangle(CDC* pDC, CRect rc, BOOL bSelected, BOOL bPressed, BOOL bEnabled, BOOL bChecked, BOOL bPopuped, BOOL bToolBar, XTPBarPosition barPosition /*= xtpBarPopup*/)
{
	if (bPopuped)
	{
		GradientFill(pDC, rc, m_clrPopupDark, m_clrPopupLight, FALSE);
		Draw3dRect(pDC, rc, XPCOLOR_MENUBAR_BORDER, XPCOLOR_MENUBAR_BORDER);
	}
	else if (m_bLunaTheme && bToolBar && IsDockingPosition(barPosition))
	{
		BOOL bHoriz = IsVerticalPosition(barPosition);
		if (!bEnabled)
		{
			if ( bSelected == TRUE_KEYBOARD)
				RectangleEx(pDC, rc, XPCOLOR_HIGHLIGHT_BORDER, bHoriz, RGB(255, 242, 200), RGB(255, 212, 151));
			return;
		}
		else if (bChecked && !bSelected && !bPressed) RectangleEx(pDC, rc, XPCOLOR_HIGHLIGHT_PUSHED_BORDER, bHoriz, RGB(255, 213, 140), RGB(255, 173, 85));
		else if (bChecked && bSelected && !bPressed) RectangleEx(pDC, rc, XPCOLOR_HIGHLIGHT_PUSHED_BORDER, bHoriz, RGB(254, 142, 75), RGB(255, 207, 139));
		else if (bPressed == TRUE_KEYBOARD || (bSelected && bPressed))  RectangleEx(pDC, rc, XPCOLOR_HIGHLIGHT_PUSHED_BORDER, bHoriz, RGB(254, 142, 75), RGB(255, 207, 139));
		else if (bSelected || bPressed)  RectangleEx(pDC, rc, XPCOLOR_HIGHLIGHT_BORDER, bHoriz, RGB(255, 242, 200), RGB(255, 212, 151));

	}
	else CXTPOfficeTheme::DrawRectangle(pDC, rc, bSelected, bPressed, bEnabled, bChecked, bPopuped, bToolBar, barPosition);
}

void CXTPOffice2003Theme::DrawExpandSymbols(CDC* pDC, BOOL bVertical, CRect rc, BOOL bHiddenExists, COLORREF clr)
{
	if (!bVertical)
	{
		CPoint pt = CPoint(rc.left + rc.Width()/2, rc.bottom - 6);
		CXTPPenDC pen(*pDC, clr);
		Line(pDC, CPoint(pt.x -2 , pt.y  - 4), CPoint(pt.x + 3, pt.y - 4));
		Triangle(pDC, CPoint(pt.x -2 , pt.y - 1), CPoint(pt.x + 2, pt.y - 1), CPoint (pt.x, pt.y + 1), clr);

		if (bHiddenExists)
		{
			Triangle(pDC, CPoint(rc.left + 3, rc.top + 5), CPoint(rc.left + 4, rc.top + 6), CPoint(rc.left + 3, rc.top + 7), clr);
			Triangle(pDC, CPoint(rc.left + 7, rc.top + 5), CPoint(rc.left + 8, rc.top + 6), CPoint(rc.left + 7, rc.top + 7), clr);
		}
	} else
	{
		CPoint pt = CPoint(rc.right - 5, rc.CenterPoint().y);
		CXTPPenDC pen(*pDC, clr);
		Line(pDC, CPoint(pt.x - 5 , pt.y  - 2), CPoint(pt.x - 5, pt.y + 3));

		Triangle(pDC, CPoint(pt.x , pt.y), CPoint(pt.x - 2, pt.y - 2), CPoint (pt.x - 2, pt.y + 2), clr);

		if (bHiddenExists)
		{
			Triangle(pDC, CPoint(rc.left + 5, rc.top + 3), CPoint(rc.left + 6, rc.top + 4), CPoint(rc.left + 7, rc.top + 3), clr);
			Triangle(pDC, CPoint(rc.left + 5, rc.top + 7), CPoint(rc.left + 6, rc.top + 8), CPoint(rc.left + 7, rc.top + 7), clr);
		}
	}
}

CSize CXTPOffice2003Theme::DrawSpecialControl(CDC* pDC, XTPSpecialControl controlType, CXTPControl* pButton, CXTPCommandBar* pBar, BOOL bDraw, LPVOID lpParam)
{
	if (controlType == xtpButtonExpandToolbar)
	{
		if (!bDraw)
		{
			CSize szBar = *(CSize*)lpParam;
			if (!IsVerticalPosition(pBar->GetPosition()))
				pButton->SetRect(CRect(szBar.cx - (11 + 1), 1, szBar.cx - 0, szBar.cy - 0));
			else
				pButton->SetRect(CRect(0, szBar.cy - (11 + 1), szBar.cx - 0, szBar.cy - 0));
		}
		else
		{
			CRect rc = pButton->GetRect();
			if (!IsVerticalPosition(pBar->GetPosition()))
			{
				pDC->ExcludeClipRect(rc.right - 1, rc.top, rc.right, rc.top + 1);
				pDC->ExcludeClipRect(rc.right - 1, rc.bottom - 1, rc.right, rc.bottom);
				pDC->ExcludeClipRect(rc.left, rc.top + 1, rc.left + 1, rc.bottom - 1);
			} else
			{
				pDC->ExcludeClipRect(rc.left, rc.bottom - 1, rc.left + 1, rc.bottom);
				pDC->ExcludeClipRect(rc.right - 1, rc.bottom - 1, rc.right, rc.bottom);
				pDC->ExcludeClipRect(rc.left + 1, rc.top, rc.right - 1, rc.top + 1);
			}

			if (((CXTPControlPopup*)pButton)->GetPopup())
			{
				if (m_bLunaTheme) GradientFill(pDC, rc, RGB(254, 142, 75), RGB(255, 207, 139), IsVerticalPosition(pBar->GetPosition()));
				else pDC->FillSolidRect(rc, GetXtremeColor(XPCOLOR_HIGHLIGHT_CHECKED));
			}
			else if (pButton->GetSelected())
			{
				if (m_bLunaTheme) GradientFill(pDC, rc, RGB(255, 242, 200), RGB(255, 212, 151), IsVerticalPosition(pBar->GetPosition()));
				else pDC->FillSolidRect(rc, GetXtremeColor(XPCOLOR_HIGHLIGHT));
			}
			else
			{
				GradientFill(pDC, rc, m_crlExpandNormalLight, m_crlExpandNormalDark, IsVerticalPosition(pBar->GetPosition()));
			}

			COLORREF clrText = GetXtremeColor((((CXTPControlPopup*)pButton)->GetPopup())? XPCOLOR_TOOLBAR_TEXT: pButton->GetSelected()? XPCOLOR_HIGHLIGHT_TEXT: XPCOLOR_TOOLBAR_TEXT);
			BOOL bHiddenExists = *(BOOL*)lpParam;

			DrawExpandSymbols(pDC, IsVerticalPosition(pBar->GetPosition()), CRect(CPoint(rc.left + 1, rc.top + 1), rc.Size()) , bHiddenExists, GetXtremeColor(COLOR_BTNHIGHLIGHT));
			DrawExpandSymbols(pDC, IsVerticalPosition(pBar->GetPosition()), rc, bHiddenExists, clrText);

			pDC->SelectClipRgn(0);
		}
		return 0;
	}
	if (controlType == xtpButtonExpandMenu)
	{
		if (bDraw)
		{
			CXTPPaintManager::DrawControlEntry(pDC, pButton);
			CRect rcButton = pButton->GetRect();
			int nCenter = rcButton.CenterPoint().x;

			m_ilGradientCircle.Draw(pDC, 0, CPoint(nCenter - 7, rcButton.top + 1), ILD_NORMAL);

			int nTop = rcButton.top + 5;
			pDC->MoveTo(nCenter - 2, nTop); pDC->LineTo(nCenter, nTop + 2);     pDC->LineTo(nCenter + 3, nTop - 1);
			pDC->MoveTo(nCenter - 2, nTop + 1); pDC->LineTo(nCenter, nTop + 3);     pDC->LineTo(nCenter + 3, nTop);

			pDC->MoveTo(nCenter - 2, nTop + 4); pDC->LineTo(nCenter, nTop + 6);     pDC->LineTo(nCenter + 3, nTop + 3);
			pDC->MoveTo(nCenter - 2, nTop + 5); pDC->LineTo(nCenter, nTop + 7);     pDC->LineTo(nCenter + 3, nTop + 4);

		}
		return CSize(0, 18);
	}

	return CXTPOfficeTheme::DrawSpecialControl(pDC, controlType, pButton, pBar, bDraw, lpParam);
}


void CXTPOffice2003Theme::CreateGradientCircle()
{

	const BYTE clrMask[16*16] = {
		0xFF, 0xFF, 0xFF, 0xFF, 0xFC, 0xEA, 0xC6, 0xB4, 0xB4, 0xB4, 0xC6, 0xFC, 0xFF, 0xFF, 0xFF, 0xFF,
		0xFF, 0xFF, 0xFF, 0xEA, 0xEA, 0xEA, 0xEA, 0xC6, 0xB4, 0x90, 0x7E, 0x7E, 0xC6, 0xFF, 0xFF, 0xFF,
		0xFF, 0xFF, 0xEA, 0xFC, 0xFC, 0xEA, 0xEA, 0xC6, 0xB4, 0x90, 0x7E, 0x6C, 0x5A, 0x90, 0xFF, 0xFF,
		0xFF, 0xEA, 0xFC, 0xFC, 0xFC, 0xEA, 0xEA, 0xC6, 0xB4, 0x90, 0x7E, 0x6C, 0x5A, 0x36, 0x7E, 0xFF,
		0xEA, 0xFC, 0xFC, 0xFC, 0xFC, 0xEA, 0xEA, 0xC6, 0xB4, 0x90, 0x7E, 0x6C, 0x5A, 0x36, 0x24, 0xEA,
		0xEA, 0xFC, 0xFC, 0xFC, 0xFC, 0xEA, 0xEA, 0xC6, 0xB4, 0x90, 0x7E, 0x6C, 0x48, 0x36, 0x24, 0x5A,
		0xEA, 0xEA, 0xEA, 0xEA, 0xEA, 0xEA, 0xC6, 0xB4, 0x90, 0x90, 0x7E, 0x6C, 0x48, 0x24, 0x12, 0x24,
		0xC6, 0xEA, 0xEA, 0xEA, 0xC6, 0xC6, 0xC6, 0xB4, 0x90, 0x7E, 0x7E, 0x48, 0x36, 0x24, 0x12, 0x12,
		0xC6, 0xC6, 0xC6, 0xC6, 0xC6, 0xB4, 0xB4, 0x90, 0x7E, 0x7E, 0x6C, 0x48, 0x36, 0x12, 0x12, 0x00,
		0xC6, 0xB4, 0xB4, 0xB4, 0xB4, 0xB4, 0x90, 0x7E, 0x7E, 0x6C, 0x5A, 0x36, 0x24, 0x12, 0x00, 0x00,
		0xC6, 0x90, 0x90, 0x90, 0x90, 0x90, 0x7E, 0x7E, 0x6C, 0x6C, 0x48, 0x36, 0x24, 0x12, 0x00, 0x36,
		0xFC, 0x90, 0x7E, 0x7E, 0x7E, 0x7E, 0x6C, 0x6C, 0x5A, 0x5A, 0x36, 0x24, 0x12, 0x00, 0x00, 0xB4,
		0xFF, 0xC6, 0x6C, 0x6C, 0x6C, 0x6C, 0x5A, 0x5A, 0x5A, 0x48, 0x36, 0x24, 0x12, 0x00, 0x48, 0xFF,
		0xFF, 0xFF, 0xB4, 0x6C, 0x5A, 0x5A, 0x5A, 0x48, 0x48, 0x36, 0x24, 0x12, 0x00, 0x36, 0xFF, 0xFF,
		0xFF, 0xFF, 0xFF, 0xC6, 0x5A, 0x48, 0x48, 0x48, 0x36, 0x24, 0x24, 0x12, 0x6C, 0xFF, 0xFF, 0xFF,
		0xFF, 0xFF, 0xFF, 0xFF, 0xFC, 0xB4, 0x6C, 0x48, 0x36, 0x5A, 0x7E, 0xEA, 0xFF, 0xFF, 0xFF, 0xFF
	};

	m_ilGradientCircle.DeleteImageList();

	COLORREF clrLight = m_clrCircleLight;
	COLORREF clrDark = m_clrCircleDark;

	CWindowDC dc(CWnd::GetDesktopWindow());

	CDC dcCircle;
	dcCircle.CreateCompatibleDC(&dc);
	CBitmap bmp;
	VERIFY(bmp.CreateCompatibleBitmap(&dc, 16, 16));
	CBitmap* pOldBitmap = (CBitmap*)dcCircle.SelectObject(&bmp);

	for (int i = 0; i < 16 * 16; i++)
	{
		int n = clrMask[i];

		int nRed = GetRValue(clrDark) - n * (GetRValue(clrDark) - GetRValue(clrLight)) / 0xFF;
		int nGreen = GetGValue(clrDark) - n * ( GetGValue(clrDark) - GetGValue(clrLight)) / 0xFF;
		int nBlue = GetBValue(clrDark) - n * ( GetBValue(clrDark) - GetBValue(clrLight)) / 0xFF;
		dcCircle.SetPixel(i / 16, i % 16, n == 0xFF? RGB(0, 0xFF, 0): RGB(nRed, nGreen, nBlue));
	}

	dcCircle.SelectObject(pOldBitmap);

	m_ilGradientCircle.Create(16, 16, ILC_COLOR24|ILC_MASK, 1, 0);
	m_ilGradientCircle.Add(&bmp, RGB(0, 0xFF, 0));

	ASSERT(m_ilGradientCircle.GetImageCount() == 1);

}


#define RGB_TO_RGBQUAD(r,g,b)   (RGB(b,g,r))


HBITMAP CXTPOffice2003Theme::GetModernBitmap(UINT nIDResource)
{
	const static struct
	{
		DWORD rgbqFrom;
		DWORD rgbqTo;
	} modernColorMap[] =
	{
		{ RGB_TO_RGBQUAD(0, 128, 0),  RGB_TO_RGBQUAD(0, 151, 94) },
		{ RGB_TO_RGBQUAD(255, 0, 0),  RGB_TO_RGBQUAD(255, 87, 42) },
		{ RGB_TO_RGBQUAD(128, 0, 0),  RGB_TO_RGBQUAD(213, 79, 74) },
		{ RGB_TO_RGBQUAD(0, 0, 255),  RGB_TO_RGBQUAD(58, 123, 252) },
		{ RGB_TO_RGBQUAD(0, 0, 128),  RGB_TO_RGBQUAD(1, 81, 205) },
		{ RGB_TO_RGBQUAD(128, 128, 128),  RGB_TO_RGBQUAD(149, 147, 145) },
		{ RGB_TO_RGBQUAD(128, 128, 0),  RGB_TO_RGBQUAD(228, 167, 8) },
		{ RGB_TO_RGBQUAD(192, 192, 192),  RGB_TO_RGBQUAD(205, 198, 187) },
		{ RGB_TO_RGBQUAD(255, 255, 0),  RGB_TO_RGBQUAD(255, 218, 33) }
	};


	LPCTSTR lpszResourceName = MAKEINTRESOURCE(nIDResource);
	ASSERT(lpszResourceName != NULL);

	// determine location of the bitmap in resource fork
	HINSTANCE hInst = AfxFindResourceHandle(lpszResourceName, RT_BITMAP);
	HRSRC hRsrc = ::FindResource(hInst, lpszResourceName, RT_BITMAP);
	if (hRsrc == NULL)
		return FALSE;


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

	COLORREF clrMask = XTPImageManager()->GetMaskColor();

	for (int iColor = 0; iColor < nColorTableSize; iColor++)
	{
		// look for matching RGBQUAD color in original
		for (int i = 0; i < _countof(modernColorMap); i++)
		{
			if (pColorTable[iColor] == modernColorMap[i].rgbqFrom
				&& clrMask != pColorTable[iColor])
			{
				pColorTable[iColor] = modernColorMap[i].rgbqTo;
				break;
			}
		}
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

BOOL CXTPOffice2003Theme::LoadModernToolbarIcons(UINT nIDResource)
{

	HBITMAP hbm = GetModernBitmap(nIDResource);
	if (!hbm)
		return FALSE;

	CBitmap bmp;
	bmp.Attach(hbm);

	return XTPImageManager()->SetIcons(nIDResource, bmp);
}

BOOL CXTPOffice2003Theme::LoadModernToolbarIcons(UINT nIDResource, UINT* pCommands, int nCount, CSize szIcon)
{
	HBITMAP hbm = GetModernBitmap(nIDResource);
	if (!hbm)
		return FALSE;

	CBitmap bmp;
	bmp.Attach(hbm);

	return XTPImageManager()->SetIcons(bmp, pCommands, nCount, szIcon);
}



CRect CXTPOffice2003Theme::GetCommandBarBorders(CXTPCommandBar* pBar)
{
	if (pBar->GetPosition() == xtpBarTop || pBar->GetPosition() == xtpBarBottom)
		return CRect(4, 2, 2, 2);
	else return CXTPOfficeTheme::GetCommandBarBorders(pBar);
}

CSize CXTPOffice2003Theme::DrawTearOffGripper(CDC* pDC, CRect rcGripper, BOOL bSelected, BOOL bDraw)
{
	if (bDraw)
	{
		CRect rc(1, 1, rcGripper.right + 1, rcGripper.Height() + 1);
		pDC->FillSolidRect(rc, GetXtremeColor(XPCOLOR_MENUBAR_FACE));
		rc.DeflateRect(1, 1);
		int nLeft = rc.CenterPoint().x - min(32, rcGripper.Width() /2 - 10);
		int nRight = rc.CenterPoint().x + min(32, rcGripper.Width() /2 - 10);

		if (bSelected)
			Rectangle(pDC, rc, XPCOLOR_HIGHLIGHT_BORDER, XPCOLOR_HIGHLIGHT);
		else
			pDC->FillSolidRect(rc, GetXtremeColor(XPCOLOR_TOOLBAR_FACE));

		for (int x = nLeft; x < nRight; x += 4)
		{
			pDC->FillSolidRect(CRect(x + 1, 4 + 1, x + 3, 6 + 1), GetXtremeColor(COLOR_BTNHIGHLIGHT));
			pDC->FillSolidRect(CRect(x, 3 + 1, x + 2, 5 + 1), GetXtremeColor(XPCOLOR_TOOLBAR_GRIPPER));
		}
	}

	return CSize(0, 9);
}

void CXTPOffice2003Theme::DrawStatusBarGripper(CDC* pDC, CRect rcClient)
{
	if (m_themeStatusBar.IsAppThemed())
	{
		CRect rcGripper(rcClient.right - 18, rcClient.top, rcClient.right, rcClient.bottom);
		m_themeStatusBar.DrawThemeBackground(pDC->GetSafeHdc(), SP_GRIPPER, 0, &rcGripper, 0);

	} else
	{
		CXTPPenDC penBorder(*pDC, GetXtremeColor(COLOR_3DSHADOW));
		int i;
		for (i = 0; i < 3; i++ )
		{
			pDC->MoveTo(rcClient.Width() - i * 5, rcClient.Height() - 1);
			pDC->LineTo(rcClient.Width(), rcClient.Height() - 1 - i * 5);

			pDC->MoveTo(rcClient.Width() - 1 - i * 5, rcClient.Height() - 1);
			pDC->LineTo(rcClient.Width(), rcClient.Height() - 2 - i * 5);

			pDC->MoveTo(rcClient.Width() - 2 - i * 5, rcClient.Height() - 1);
			pDC->LineTo(rcClient.Width(), rcClient.Height() - 3 - i * 5);
		}

		CXTPPenDC penBorderWhite(*pDC, GetXtremeColor(COLOR_BTNHIGHLIGHT));
		for (i = 0; i < 3; i++ )
		{
			pDC->MoveTo(rcClient.Width() - 3 - i * 5, rcClient.Height() - 1);
			pDC->LineTo(rcClient.Width(), rcClient.Height() - 4 - i * 5);
		}

	}
}

void CXTPOffice2003Theme::DrawStatusBarPaneBorder(CDC* pDC, CRect rc, BOOL /*bGripperPane*/)
{
	pDC->Draw3dRect(rc, m_clrStatusPane, m_clrStatusPane);
}


CXTPWhidbeyTheme::CXTPWhidbeyTheme()
{
	m_systemTheme = xtpSystemThemeUnknown;
	RefreshMetrics();
}

CXTPWhidbeyTheme::~CXTPWhidbeyTheme()
{

}

void CXTPWhidbeyTheme::RefreshMetrics()
{
	CXTPOffice2003Theme::RefreshMetrics();

	switch (XTPColorManager()->GetCurrentSystemTheme())
	{
	case xtpSystemThemeBlue:
		m_clrCommandBarDark = RGB(199, 198, 176);
		m_crlToolbarShadow = RGB(146, 146, 118);
		break;
	case xtpSystemThemeSilver:
		m_clrCommandBarDark = RGB(173, 172, 196);
		m_clrCommandBarLight = RGB(245, 245, 252);
		m_crlToolbarShadow = RGB(124, 124, 148);
	}
}
