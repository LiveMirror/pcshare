// XTButtonTheme.cpp: implementation of the CXTButtonTheme class.
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
#include "XTPImageManager.h"
#include "XTPWinThemeWrapper.h"
#include "XTFunctions.h"
#include "XTThemeManager.h"
#include "XTButtonTheme.h"
#include "XTButtonThemeOffice.h"
#include "XTDefines.h"
#include "XTButton.h"
#include "XTHelpers.h"
#include "XTUtil.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_THEME_FACTORY(CXTButtonTheme, CXTButtonThemeOfficeXP, CXTButtonThemeOffice2003)

//===========================================================================
// CXTButtonTheme class
//===========================================================================

CXTButtonTheme::CXTButtonTheme()
: m_bShowIcon(TRUE)
, m_bOffsetHiliteText(TRUE)
, m_pFont(NULL)
{
}

CXTButtonTheme::~CXTButtonTheme()
{

}

void CXTButtonTheme::RefreshMetrics()
{
	XTPColorManager()->RefreshColors();
	// background colors.
	m_crBack.SetStandardValue(GetXtremeColor(COLOR_BTNFACE));

	// text colors.
	m_crText.SetStandardValue(GetXtremeColor(COLOR_BTNTEXT));
	m_crTextDisabled.SetStandardValue(GetXtremeColor(COLOR_GRAYTEXT));

	// border colors.
	m_crBorderHilite.SetStandardValue(GetXtremeColor(COLOR_BTNFACE));
	m_crBorderShadow.SetStandardValue(GetXtremeColor(COLOR_3DSHADOW));
	m_crBorder3DHilite.SetStandardValue(GetXtremeColor(COLOR_3DHILIGHT));
	m_crBorder3DShadow.SetStandardValue(GetXtremeColor(COLOR_3DDKSHADOW));
}

CFont* CXTButtonTheme::GetThemeFont(CXTButton* pButton) const
{
	if (m_pFont && m_pFont->GetSafeHandle())
		return m_pFont;

	if (pButton)
		return pButton->GetFont();

	return NULL;
}

void CXTButtonTheme::SetThemeFont(CFont* pFont)
{
	if (pFont && pFont->GetSafeHandle())
		m_pFont = pFont;
}

void CXTButtonTheme::SetAlternateColors(COLORREF clr3DFace, COLORREF clr3DHilight, COLORREF clr3DShadow, COLORREF clrBtnText)
{
	SetColorFace(clr3DFace);
	SetColorText(clrBtnText);
	SetColorHilite(clr3DHilight);
	SetColorShadow(clr3DShadow);
}

void CXTButtonTheme::SetColorFace(COLORREF clrFace)
{
	m_crBack.SetCustomValue(clrFace);
}

COLORREF CXTButtonTheme::GetColorFace()
{
	return m_crBack;
}

void CXTButtonTheme::SetColorHilite(COLORREF clrHilite)
{
	m_crBorderHilite.SetCustomValue(clrHilite);
}

void CXTButtonTheme::SetColorShadow(COLORREF clrShadow)
{
	m_crBorderShadow.SetCustomValue(clrShadow);
}

void CXTButtonTheme::SetColorText(COLORREF clrText)
{
	m_crText.SetCustomValue(clrText);
}

void CXTButtonTheme::OffsetPoint(CPoint& point, CSize size, CXTButton* pButton)
{
	CXTPClientRect rcClient(pButton);

	if (pButton->GetXButtonStyle() & BS_XT_TWOROWS)
	{
		point.x = __max(0,((rcClient.Width()-size.cx)/2));
		point.y = __max(0,((rcClient.Height()-size.cy)/2));
	}
	else
	{
		point.x = pButton->GetBorderGap();
		point.y = __max(0,((rcClient.Height()-size.cy)/2));
	}
}

CPoint CXTButtonTheme::GetTextPosition(UINT nState, CRect& rcItem, CSize& sizeText, CXTButton* pButton)
{
	UNREFERENCED_PARAMETER(nState);

	CRect rcText = rcItem;
	rcText.DeflateRect(2,0);

	CPoint point = rcText.TopLeft();

	if (pButton->GetUserPosition())
	{
		point = pButton->GetTextPoint();
	}
	else
	{
		OffsetPoint(point, sizeText, pButton);
		point.x = pButton->GetBorderGap();

		if (pButton->GetXButtonStyle() & BS_XT_TWOROWS)
		{
			point.y += (pButton->GetImageSize().cy/2)+(pButton->GetImageGap()/2);
		}

		switch (pButton->GetStyle() & (BS_CENTER|BS_LEFT|BS_RIGHT))
		{
		case BS_LEFT:
			if (!(pButton->GetXButtonStyle() & BS_XT_TWOROWS))
			{
				point.x += pButton->GetImageSize().cx + pButton->GetImageGap();
			}
			break;

		case BS_RIGHT:
			point.x = rcItem.right - pButton->GetBorderGap() - sizeText.cx;
			break;

		default:
			if (!(pButton->GetXButtonStyle() & BS_XT_TWOROWS))
			{
				point.x += pButton->GetImageSize().cx;
			}
			point.x += (rcItem.right - point.x - sizeText.cx - pButton->GetBorderGap()) / 2;
			break;
		}
	}

	return point;
}

COLORREF CXTButtonTheme::GetTextColor(UINT nState, CXTButton* pButton)
{
	UNREFERENCED_PARAMETER(pButton);

	if (nState & ODS_DISABLED)
	{
		return m_crTextDisabled;
	}
	return m_crText;
}

void CXTButtonTheme::DrawButtonText(CDC* pDC, UINT nState, CRect& rcItem, CXTButton* pButton)
{
	CString strText;
	pButton->GetWindowText(strText);

	// if the string is empty just return.
	if (strText.IsEmpty())
		return;

	// Remove ampersand
	XTFuncStripMnemonics(strText);

	// select font into device context.
	CXTPFontDC fontDC(pDC, GetThemeFont(pButton));

	// get the text size.
	CSize sizeText = pDC->GetTextExtent(strText);

	CPoint pt = GetTextPosition(nState, rcItem, sizeText, pButton);

	BOOL  bSelected = pButton->GetChecked() || (nState & ODS_SELECTED);

	if(bSelected && m_bOffsetHiliteText)
		pt.Offset(1,1);

	// Set the draw state flags.
	DWORD dwFlags = DST_PREFIXTEXT;

	if ((nState & ODS_DISABLED) == 0)
		dwFlags |= DSS_NORMAL;

	pDC->SetTextColor(GetTextColor(nState, pButton));

	// draw the text, and select the original font.
	pButton->GetWindowText(strText);
	pDC->SetTextAlign(TA_LEFT);
	pDC->DrawState( pt, sizeText, strText, dwFlags, TRUE, strText.GetLength(), (HBRUSH)NULL );
}

CPoint CXTButtonTheme::CalculateImagePosition(CDC* pDC, UINT nState,
	CRect& rcItem, bool bHasPushedImage, CXTButton* pButton)
{
	UNREFERENCED_PARAMETER(bHasPushedImage);
	UNREFERENCED_PARAMETER(nState);

	// Get the window text.
	CString strText;
	pButton->GetWindowText(strText);

	// Remove ampersand
	XTFuncStripMnemonics(strText);

	CSize   sizeText = pDC->GetTextExtent(strText);
	CPoint  point    = rcItem.TopLeft();

	DWORD dwStyle = pButton->GetStyle();

	if (pButton->GetUserPosition())
	{
		point = pButton->GetImagePoint();
	}
	else
	{
		OffsetPoint(point, pButton->GetImageSize(), pButton);
		point.x = pButton->GetBorderGap();

		if (pButton->GetXButtonStyle() & BS_XT_TWOROWS)
		{
			if (!strText.IsEmpty())
			{
				point.y -= ((sizeText.cy/2)+(pButton->GetImageGap()/2));
			}

			// horz. alignment - only for two-row mode, otherwise always
			// on left edge
			switch (pButton->GetStyle() & (BS_CENTER|BS_LEFT|BS_RIGHT))
			{
			case BS_LEFT:
				break;
			case BS_RIGHT:
				point.x = rcItem.right - pButton->GetBorderGap() - pButton->GetImageSize().cx;
				break;
			default:
				point.x = rcItem.left + __max(0,((rcItem.Width()-pButton->GetImageSize().cx)/2));
				break;
			}
		}
		else
		{
			if (strText.IsEmpty())
			{
				if ((dwStyle & BS_CENTER) == BS_CENTER)
				{
					int cx = rcItem.Width();
					if (cx >= pButton->GetImageSize().cx)
					{
						point.x = (cx - pButton->GetImageSize().cx)/2;
					}
				}
				if  ((dwStyle & BS_VCENTER) == BS_VCENTER)
				{
					int cy = rcItem.Height();
					if (cy >= pButton->GetImageSize().cy)
					{
						point.y = (cy - pButton->GetImageSize().cy)/2;
					}
				}
			}
		}
	}

	return point;
}

void CXTButtonTheme::DrawButtonIcon(CDC* pDC, UINT nState, CRect& rcItem, CXTButton* pButton)
{
	if (pButton == NULL || m_bShowIcon == FALSE)
		return;

	CXTPImageManagerIcon* pIcon = pButton->GetIcon();
	if (pIcon == NULL)
		return;

	CPoint pt = CalculateImagePosition(
		pDC, nState, rcItem, (pButton->GetSelectedIcon() != NULL), pButton);

	BOOL  bSelected = pButton->GetChecked() || (nState & ODS_SELECTED);

	if( bSelected )
		pt.Offset(1,1);

	if (nState & ODS_DISABLED)
	{
		pIcon->Draw(pDC, pt, pIcon->GetDisabledIcon());
	}
	else if (pButton->GetHilite() || bSelected)
	{
		pIcon->Draw(pDC, pt, pIcon->GetHotIcon());
	}
	else
	{
		pIcon->Draw(pDC, pt, pIcon->GetIcon());
	}
}

BOOL CXTButtonTheme::UseWinXPThemes(CXTButton* pButton)
{
	// if the button should not use Windows XP themes return FALSE.
	if (!pButton || (pButton->GetXButtonStyle() & BS_XT_WINXP_COMPAT) == 0)
		return FALSE;

	// if we got this far then we try to load the theme data for
	// this control if it is not currently open.
	if (!m_themeWrapper.ThemeDataOpen())
		m_themeWrapper.OpenThemeData(pButton->m_hWnd, L"BUTTON");

	// if our application is not "Theme Ready" meaning that we cannot
	// display Windows XP themes, then return FALSE.
	if (!m_themeWrapper.IsAppThemeReady())
		return FALSE;

	// this will return TRUE if we can display visual styles.
	return m_themeWrapper.ThemeDataOpen();
}

BOOL CXTButtonTheme::CanHilite(CXTButton* pButton)
{
	if (UseWinXPThemes(pButton))
		return TRUE;

	return ((pButton->GetXButtonStyle() & BS_XT_FLAT_ANY) != 0);
}

BOOL CXTButtonTheme::DrawWinThemeBackground(LPDRAWITEMSTRUCT lpDIS, CXTButton* pButton)
{
	if (UseWinXPThemes(pButton))
	{
		CDC*  pDC       = CDC::FromHandle(lpDIS->hDC);
		CRect rcItem    = lpDIS->rcItem;
		int   nState    = lpDIS->itemState;

		bool  bSelected = ((nState & ODS_SELECTED) != 0);
		bool  bHasFocus = ((nState & ODS_FOCUS)    != 0);
		bool  bDisabled = ((nState & ODS_DISABLED) != 0);

		int iStateId = PBS_NORMAL;

		if (bHasFocus)
			iStateId = PBS_DEFAULTED;

		if (pButton->GetHilite())
			iStateId = PBS_HOT;

		if (bSelected || pButton->GetChecked())
			iStateId = PBS_PRESSED;

		if (bDisabled)
			iStateId = PBS_DISABLED;


		if (m_themeWrapper.DrawThemeBackground(pDC->GetSafeHdc(),
			BP_PUSHBUTTON, iStateId, &rcItem, NULL) != S_OK)
		{
			TRACE0("Error drawing background using WinTheme API.\n");
			return FALSE;
		}

		return TRUE;
	}

	return FALSE;
}

BOOL CXTButtonTheme::DrawButtonThemeBackground(LPDRAWITEMSTRUCT lpDIS, CXTButton* pButton)
{
	// define some temporary variables.
	CDC*  pDC       = CDC::FromHandle(lpDIS->hDC);
	CRect rcItem    = lpDIS->rcItem;
	int   nState    = lpDIS->itemState;

	BOOL  bSelected = pButton->GetChecked() || (nState & ODS_SELECTED);

	// get the cursor position.
	CXTPClientCursorPos pt(pButton);

	// Paint the background.
	pDC->FillSolidRect(rcItem, m_crBack);

	DWORD dwxStyle = pButton->GetXButtonStyle();
	if (dwxStyle & BS_XT_HILITEPRESSED)
	{
		if ((pButton->GetChecked() && !rcItem.PtInRect(pt)) || bSelected)
		{
			XTFuncDrawShadedRect(pDC, rcItem);
		}
	}

	if (dwxStyle & BS_XT_FLAT)
	{
		if (pButton->GetHilite() ||  bSelected)
		{
			pDC->Draw3dRect(rcItem,
				bSelected ? m_crBorderShadow : m_crBorder3DHilite,
				bSelected ? m_crBorder3DHilite : m_crBorderShadow);
		}
	}
	else if (dwxStyle & BS_XT_SEMIFLAT)
	{
		pDC->Draw3dRect(rcItem,
			bSelected ? m_crBorderShadow : m_crBorder3DHilite,
			bSelected ? m_crBorder3DHilite : m_crBorderShadow);
	}
	else // normal
	{
		pDC->Draw3dRect(rcItem,
			bSelected ? m_crBorder3DShadow : m_crBorder3DHilite,
			bSelected ? m_crBorder3DHilite : m_crBorder3DShadow);

		rcItem.DeflateRect(1,1);

		pDC->Draw3dRect(rcItem,
			bSelected ? m_crBorderShadow : m_crBorderHilite,
			bSelected ? m_crBorderHilite : m_crBorderShadow);
	}

	return TRUE;
}

void CXTButtonTheme::DrawFocusRect(CDC* pDC, UINT nState, CRect& rcItem, CXTButton* pButton)
{
	UNREFERENCED_PARAMETER(nState);

	CXTContextTextColorHandler foreHandler(pDC,
		m_crText);

	if (UseWinXPThemes(pButton))
		rcItem.DeflateRect(4,4);
	else
		rcItem.DeflateRect(2,2);

	pDC->DrawFocusRect(rcItem);
}

void CXTButtonTheme::DrawItem(LPDRAWITEMSTRUCT lpDIS, CXTButton* pButton)
{
	ASSERT(lpDIS != NULL);

	// Draw the button background.
	if (!DrawWinThemeBackground(lpDIS, pButton))
		DrawButtonThemeBackground(lpDIS, pButton);

	// define some temporary variables.
	CDC*  pDC    = CDC::FromHandle(lpDIS->hDC);
	CRect rcItem = lpDIS->rcItem;
	int   nState = lpDIS->itemState;

	// Set the background mode to transparent.
	CXTContextBkModeHandler modeHandler(pDC, TRANSPARENT);

	// draw the button text, icon or bitmap.
	DrawButtonText(pDC, nState, rcItem, pButton);
	DrawButtonIcon(pDC, nState, rcItem, pButton);

	// Draw the focus rect if style is set and we have focus.
	if ((pButton->GetXButtonStyle() & BS_XT_SHOWFOCUS) && (nState & ODS_FOCUS))
	{
		DrawFocusRect(pDC, nState, rcItem, pButton);
	}
}

//===========================================================================
// CXTDisabledButtonTheme class
//===========================================================================

void CXTDisabledButtonTheme::DrawButtonIcon(CDC* pDC, UINT nState, CRect& rcItem, CXTButton* pButton)
{
	if (pButton == NULL || m_bShowIcon == FALSE)
		return;

	CXTPImageManagerIcon* pIconManager = pButton->GetIcon();
	if (pIconManager == NULL)
		return;

	CPoint pt = CalculateImagePosition(
		pDC, nState, rcItem, (pButton->GetSelectedIcon() != NULL), pButton);

	BOOL  bSelected = pButton->GetChecked() || (nState & ODS_SELECTED);

	if( bSelected )
		pt.Offset(1,1);

	if (nState & ODS_DISABLED)
	{
		pDC->DrawState(pt, pIconManager->GetExtent(),
			pIconManager->GetIcon(), DSS_NORMAL|DSS_DISABLED, (HBRUSH)0);
	}
	else if (pButton->GetHilite() || bSelected)
	{
		pDC->DrawState(pt, pIconManager->GetExtent(),
			pIconManager->GetHotIcon(), DSS_NORMAL, (HBRUSH)0);
	}
	else
	{
		pDC->DrawState(pt, 0,
			pIconManager->GetIcon(), DSS_NORMAL, (HBRUSH)0);
	}
}

//===========================================================================
// CXTToolbarButtonTheme class
//===========================================================================

BOOL CXTToolbarButtonTheme::DrawWinThemeBackground(LPDRAWITEMSTRUCT lpDIS, CXTButton* pButton)
{
	lpDIS->itemState &= ~ODS_FOCUS;
	return CXTButtonTheme::DrawWinThemeBackground(lpDIS, pButton);
}

BOOL CXTToolbarButtonTheme::UseWinXPThemes(CXTButton* pButton)
{
	// if the button should not use Windows XP themes return FALSE.
	if (!pButton || (pButton->GetXButtonStyle() & BS_XT_WINXP_COMPAT) == 0)
		return FALSE;

	// if our application is not "Theme Ready" meaning that we cannot
	// display Windows XP themes, then return FALSE.
	if (!m_themeWrapper.IsAppThemeReady())
		return FALSE;

	// if we got this far then we try to load the theme data for
	// this control if it is not currently open.
	if (!m_themeWrapper.ThemeDataOpen())
		m_themeWrapper.OpenThemeData(pButton->m_hWnd, L"TOOLBAR");

	// this will return TRUE if we can display visual styles.
	return m_themeWrapper.ThemeDataOpen();
}
