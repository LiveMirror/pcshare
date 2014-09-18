// XTButtonThemeOffice.cpp: implementation of the CXTButtonThemeOfficeXP class.
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

//===========================================================================
// CXTButtonThemeOfficeXP class
//===========================================================================

CXTButtonThemeOfficeXP::CXTButtonThemeOfficeXP(BOOL bWordTheme/*=FALSE*/)
{
	m_bWordTheme        = bWordTheme;
	m_bOffsetHiliteText = FALSE;
	m_bFadedIcon        = TRUE;
	m_bAnimateIcon      = TRUE;

	RefreshMetrics();
}

CXTButtonThemeOfficeXP::~CXTButtonThemeOfficeXP()
{

}

void CXTButtonThemeOfficeXP::RefreshMetrics()
{
	CXTButtonTheme::RefreshMetrics();

	m_crBack.SetStandardValue(GetXtremeColor(m_bWordTheme ? XPCOLOR_TOOLBAR_FACE : COLOR_BTNFACE));
	m_crBackPushed.SetStandardValue(GetXtremeColor(XPCOLOR_HIGHLIGHT_PUSHED));
	m_crBackHilite.SetStandardValue(GetXtremeColor(XPCOLOR_HIGHLIGHT));
	m_crBorderShadow.SetStandardValue(GetXtremeColor(COLOR_3DSHADOW));
	m_crBorderHilite.SetStandardValue(GetXtremeColor(XPCOLOR_HIGHLIGHT_BORDER));
	m_crText.SetStandardValue(GetXtremeColor(XPCOLOR_TOOLBAR_TEXT));
	m_crTextHilite.SetStandardValue(GetXtremeColor(XPCOLOR_HIGHLIGHT_TEXT));
	m_crTextPushed.SetStandardValue(GetXtremeColor(XPCOLOR_PUSHED_TEXT));
	m_crBackChecked.SetStandardValue(GetXtremeColor(XPCOLOR_HIGHLIGHT_CHECKED));
}

COLORREF CXTButtonThemeOfficeXP::GetTextColor(UINT nState, CXTButton* pButton)
{
	if (nState & ODS_DISABLED)
		return GetXtremeColor(COLOR_GRAYTEXT);

	if (pButton->GetHilite())
		return m_crTextHilite;

	if (nState & ODS_SELECTED)
		return m_crTextPushed;

	return m_crText;
}

void CXTButtonThemeOfficeXP::DrawButtonIcon(CDC* pDC, UINT nState, CRect& rcItem, CXTButton* pButton)
{
	if (pButton == NULL || m_bShowIcon == FALSE)
		return;

	CXTPImageManagerIcon* pIcon = pButton->GetIcon();
	if (pIcon == NULL)
		return;

	CPoint pt = CalculateImagePosition(
		pDC, nState, rcItem, FALSE, pButton);

	if (nState & ODS_DISABLED)
	{
		pIcon->Draw(pDC, pt, pIcon->GetDisabledIcon());
	}
	else if (pButton->GetHilite())
	{
		if (m_bAnimateIcon)
		{
			pIcon->Draw(pDC, CPoint(pt.x + 1, pt.y + 1), pIcon->GetShadowIcon());
			pIcon->Draw(pDC, CPoint(pt.x - 1, pt.y - 1), pIcon->GetHotIcon());
		}
		else
		{
			pIcon->Draw(pDC, pt, pIcon->GetHotIcon());
		}
	}
	else if (nState & ODS_SELECTED)
	{
		pIcon->Draw(pDC, pt, pIcon->GetHotIcon());
	}
	else
	{
		pIcon->Draw(pDC, pt, m_bFadedIcon ? pIcon->GetFadedIcon(): pIcon->GetIcon());
	}
}

BOOL CXTButtonThemeOfficeXP::DrawButtonThemeBackground(LPDRAWITEMSTRUCT lpDIS, CXTButton* pButton)
{
	// define some temporary variables.
	CDC*  pDC       = CDC::FromHandle(lpDIS->hDC);
	CRect rcItem    = lpDIS->rcItem;
	int   nState    = lpDIS->itemState;
	BOOL bPressed = (nState & ODS_SELECTED);

	// Paint the background.
	if (pButton->GetHilite() || bPressed || pButton->GetChecked())
	{
		pDC->FillSolidRect(rcItem, bPressed? m_crBackPushed :
			pButton->GetChecked()? (pButton->GetHilite()? m_crBackPushed: m_crBackChecked): m_crBackHilite);

		pDC->Draw3dRect(rcItem, m_crBorderHilite,
			m_crBorderHilite);
	}
	else
	{
		pDC->FillSolidRect(rcItem, m_crBack);

		if (m_bWordTheme)
		{
			pDC->Draw3dRect(rcItem,
				m_crBorderShadow, m_crBorderShadow);

		}
	}

	return TRUE;
}

BOOL CXTButtonThemeOfficeXP::CanHilite(CXTButton* pButton)
{
	UNREFERENCED_PARAMETER(pButton);
	return TRUE;
}

void CXTButtonThemeOfficeXP::DrawFocusRect(CDC* pDC, UINT nState, CRect& rcItem, CXTButton* pButton)
{
	if (m_bWordTheme && (!(pButton->GetHilite() || (nState & ODS_SELECTED))) && (pButton->GetXButtonStyle() & BS_XT_WINXP_COMPAT) == 0)
	{
		pDC->Draw3dRect(rcItem, GetXtremeColor(COLOR_HIGHLIGHT),
			GetXtremeColor(COLOR_HIGHLIGHT));

		rcItem.DeflateRect(1,1);

		pDC->Draw3dRect(rcItem, GetXtremeColor(COLOR_HIGHLIGHT),
			GetXtremeColor(COLOR_HIGHLIGHT));
	}
	else
	{
		CXTButtonTheme::DrawFocusRect(pDC, nState, rcItem, pButton);
	}
}

void CXTButtonThemeOfficeXP::SetBackPushedColor(COLORREF color)
{
	m_crBackPushed.SetCustomValue(color);
}

void CXTButtonThemeOfficeXP::SetBackHiliteColor(COLORREF color)
{
	m_crBackHilite.SetCustomValue(color);
}

void CXTButtonThemeOfficeXP::SetTextPushedColor(COLORREF color)
{
	m_crTextPushed.SetCustomValue(color);
}

void CXTButtonThemeOfficeXP::SetTextHiliteColor(COLORREF color)
{
	m_crTextHilite.SetCustomValue(color);
}

//===========================================================================
// CXTButtonThemeOffice2003 class
//===========================================================================

CXTButtonThemeOffice2003::CXTButtonThemeOffice2003(BOOL bWordTheme/*=FALSE*/)
{
	m_bWordTheme        = bWordTheme;
	m_bOffsetHiliteText = FALSE;
	m_bFadedIcon        = FALSE;
	m_bAnimateIcon      = FALSE;

	RefreshMetrics();
}

void CXTButtonThemeOffice2003::DrawButtonIcon(CDC* pDC, UINT nState, CRect& rcItem, CXTButton* pButton)
{
	if (pButton == NULL || m_bShowIcon == FALSE)
		return;

	CXTPImageManagerIcon* pIcon = pButton->GetIcon();
	if (pIcon == NULL)
		return;

	CPoint pt = CalculateImagePosition(
		pDC, nState, rcItem, FALSE, pButton);

	if (nState & ODS_DISABLED)
	{
		pIcon->Draw(pDC, pt, pIcon->GetDisabledIcon());
	}
	else if (nState & ODS_SELECTED)
	{
		pIcon->Draw(pDC, pt, pIcon->GetHotIcon());
	}
	else
	{
		pIcon->Draw(pDC, pt, pIcon->GetIcon());
	}
}

void CXTButtonThemeOffice2003::RefreshMetrics()
{
	CXTButtonThemeOfficeXP::RefreshMetrics();

	XTPCurrentSystemTheme systemTheme = XTPDrawHelpers()->GetCurrentSystemTheme();

	m_crBack.SetStandardValue(GetXtremeColor(COLOR_3DFACE));

	if (systemTheme != xtpSystemThemeUnknown)
	{
		m_crBackHilite.SetStandardValue(RGB(255, 238, 194));
		m_crBackPushed.SetStandardValue(RGB(254, 128, 62));
		m_crBackChecked.SetStandardValue(RGB(255, 192, 111));
		m_crText.SetStandardValue(0);
		m_crTextHilite.SetStandardValue(0);
		m_crTextPushed.SetStandardValue(0);
	}

	switch (systemTheme)
	{
	case xtpSystemThemeBlue:
		m_crBack.SetStandardValue(RGB(169, 199, 240));
		m_crBorderShadow.SetStandardValue(RGB(127, 157, 185));
		m_crBorderHilite.SetStandardValue(RGB(0, 0, 128));

		break;

	case xtpSystemThemeOlive:
		m_crBack.SetStandardValue(RGB(197, 212, 159));
		m_crBorderShadow.SetStandardValue(RGB(164, 185, 127));
		m_crBorderHilite.SetStandardValue(RGB(63, 93, 56));
		break;

	case xtpSystemThemeSilver:
		m_crBack.SetStandardValue(RGB(192, 192, 211));
		m_crBorderShadow.SetStandardValue(RGB(165, 172, 178));
		m_crBorderHilite.SetStandardValue(RGB(75, 75, 11));
	}

	if (!m_bWordTheme) m_crBack.SetStandardValue(GetXtremeColor(COLOR_BTNFACE));
}
