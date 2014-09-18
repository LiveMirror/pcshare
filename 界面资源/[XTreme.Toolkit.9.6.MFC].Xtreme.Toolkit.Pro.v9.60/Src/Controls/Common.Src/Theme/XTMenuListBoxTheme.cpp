// XTMenuListBoxTheme.cpp: implementation of the CXTMenuListBoxTheme class.
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

#include "XTDefines.h"
#include "XTUtil.h"
#include "XTGlobal.h"

#include "XTMenuListBoxTheme.h"
#include "XTMenuListBox.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_THEME_FACTORY(CXTMenuListBoxTheme, CXTMenuListBoxThemeOfficeXP, CXTMenuListBoxThemeOffice2003)
//===========================================================================
// CXTMenuListBoxTheme class
//===========================================================================

CXTMenuListBoxTheme::CXTMenuListBoxTheme()
{
	RefreshMetrics();
}

CXTMenuListBoxTheme::~CXTMenuListBoxTheme()
{

}

void CXTMenuListBoxTheme::RefreshMetrics()
{
	XTPColorManager()->RefreshColors();

	m_cxIcon = ::GetSystemMetrics(SM_CXICON);
	m_cyIcon = ::GetSystemMetrics(SM_CYICON);
	m_cyEdge = ::GetSystemMetrics(SM_CYEDGE)*2;


	m_clrBack.SetStandardValue(GetXtremeColor(COLOR_3DSHADOW));
	m_clrText.SetStandardValue(GetXtremeColor(COLOR_WINDOW));

	m_clrHighlighPushed.SetStandardValue((COLORREF)-1);
	m_clrHighligh.SetStandardValue((COLORREF)-1);
	m_clrHighlighBorderTopLeft.SetStandardValue(GetXtremeColor(COLOR_3DDKSHADOW));
	m_clrHighlighBorderBottomRight.SetStandardValue(GetXtremeColor(COLOR_3DHILIGHT));

}


void CXTMenuListBoxTheme::DrawItem(CXTMenuListBox* pMenuListBox, LPDRAWITEMSTRUCT lpDIS)
{
	// Define temporary variables.
	CDC*  pDC     = CDC::FromHandle (lpDIS->hDC);
	CRect rcIcon  = lpDIS->rcItem;
	CRect rcText  = lpDIS->rcItem;
	int   iItemID = lpDIS->itemID;

	// if empty, draw focus rect.
	if (iItemID < 0)
	{
		rcIcon.bottom = rcIcon.top + 15;
		pDC->DrawFocusRect(rcIcon);
		return;
	}

	// Get the menu item.
	CXTMenuListBox::CONTENT_ITEM* pCI = pMenuListBox->GetMenuItem(iItemID);
	ASSERT(pCI);

	// set the text foreground and background colors.
	pDC->SetTextColor( GetTextColor() );

	// repaint the background.
	pDC->FillSolidRect( rcIcon, GetBackColor() );
	pDC->SetBkMode( TRANSPARENT );

	long iMargin = (rcIcon.Width()-m_cxIcon)/2;

	rcIcon.top    += m_cyEdge;
	rcIcon.left   += iMargin;
	rcIcon.right   = rcIcon.left + m_cxIcon;
	rcIcon.bottom  = rcIcon.top  + m_cyIcon;

	int iOffset = 0;

	BOOL bSelected = FALSE;

	if ((lpDIS->itemState & ODS_SELECTED) && pCI->m_bEnabled)
	{
		bSelected = TRUE;

		if (pMenuListBox->m_bNoBorder)
		{
			if (::GetKeyState(VK_LBUTTON) < 0)
				iOffset = 1;
			else
				iOffset = -1;
		}
		else
		{
			CRect rcBorder(rcIcon);
			rcBorder.InflateRect(2,2);

			if (::GetKeyState(VK_LBUTTON) < 0)
			{
				if (m_clrHighlighPushed != (COLORREF)-1) pDC->FillSolidRect(&rcBorder, m_clrHighlighPushed);
				pDC->Draw3dRect(&rcBorder, m_clrHighlighBorderTopLeft, m_clrHighlighBorderBottomRight);
			}
			else
			{
				if (m_clrHighligh != (COLORREF)-1) pDC->FillSolidRect(&rcBorder, m_clrHighligh);
				pDC->Draw3dRect(&rcBorder, m_clrHighlighBorderBottomRight, m_clrHighlighBorderTopLeft);
			}
		}
	}

	LOGFONT lf;
	pMenuListBox->GetFont()->GetLogFont(&lf);

	if (bSelected && !pMenuListBox->m_bNoUnderline)
		lf.lfUnderline = 1;

	// select the font used by the button text.
	CFont font;
	font.CreateFontIndirect(&lf);
	CXTPFontDC fnt(pDC, &font);

	// get the text height.
	int iTextHeight = pMenuListBox->GetTextHeight(pCI->m_strText);

	// define the text rect.
	rcText.top    = rcText.bottom - (iTextHeight + m_cyEdge);
	rcText.left  += m_cyEdge;
	rcText.right -= m_cyEdge;

	if (iOffset != 0)
	{
		rcIcon.OffsetRect(iOffset,iOffset);
		rcText.OffsetRect(iOffset,iOffset);
	}

	DWORD dwDrawStyle = DST_ICON;

	if (!pCI->m_bEnabled)
		dwDrawStyle |= DSS_DISABLED;

	// draw the icon and the text associated with the button.
	pDC->DrawState( rcIcon.TopLeft(), rcIcon.Size(), pCI->m_hIcon,
		dwDrawStyle, ( HBRUSH )NULL );

	if (!pCI->m_bEnabled)
	{
		CRect rc = rcText;
		rc.OffsetRect(1,1);

		pDC->SetTextColor(GetXtremeColor(COLOR_WINDOW));
		pDC->DrawText(pCI->m_strText, rc,
			DT_CENTER | DT_TOP | DT_END_ELLIPSIS);

		pDC->SetTextColor(GetXtremeColor(COLOR_3DSHADOW));
	}

	pDC->DrawText(pCI->m_strText, rcText,
		DT_CENTER | DT_TOP | DT_END_ELLIPSIS);
}

COLORREF CXTMenuListBoxTheme::GetBackColor()
{
	return m_clrBack;
}

COLORREF CXTMenuListBoxTheme::GetTextColor()
{
	return m_clrText;
}



//////////////////////////////////////////////////////////////////////////
// CXTMenuListBoxThemeOfficeXP

CXTMenuListBoxThemeOfficeXP::CXTMenuListBoxThemeOfficeXP()
{
	RefreshMetrics();

}

void CXTMenuListBoxThemeOfficeXP::RefreshMetrics()
{
	CXTMenuListBoxTheme::RefreshMetrics();

	m_clrBack.SetStandardValue(GetXtremeColor(XPCOLOR_TOOLBAR_FACE));
	m_clrText.SetStandardValue(GetXtremeColor(COLOR_BTNTEXT));

	m_clrHighlighPushed.SetStandardValue(GetXtremeColor(XPCOLOR_HIGHLIGHT_PUSHED));
	m_clrHighligh.SetStandardValue(GetXtremeColor(XPCOLOR_HIGHLIGHT));
	m_clrHighlighBorderTopLeft.SetStandardValue(GetXtremeColor(XPCOLOR_HIGHLIGHT_BORDER));
	m_clrHighlighBorderBottomRight.SetStandardValue(GetXtremeColor(XPCOLOR_HIGHLIGHT_BORDER));

}

//////////////////////////////////////////////////////////////////////////
//  CXTOutBarThemeOffice2003

CXTMenuListBoxThemeOffice2003::CXTMenuListBoxThemeOffice2003()
{
	RefreshMetrics();
}

void CXTMenuListBoxThemeOffice2003::RefreshMetrics()
{
	CXTMenuListBoxThemeOfficeXP::RefreshMetrics();

	if (!XTPColorManager()->IsLunaColorsDisabled())
	{
		XTPCurrentSystemTheme systemTheme = XTPColorManager()->GetCurrentSystemTheme();

		switch (systemTheme)
		{
		case xtpSystemThemeBlue:
			m_clrBack.SetStandardValue(RGB(216, 231, 252));
			break;
		case xtpSystemThemeOlive:
			m_clrBack.SetStandardValue(RGB(226, 231, 191));
			break;
		case xtpSystemThemeSilver:
			m_clrBack.SetStandardValue(RGB(223, 223, 234));
			break;
		}

		if (systemTheme != xtpSystemThemeUnknown)
		{
			m_clrHighlighPushed.SetStandardValue(RGB(254, 128, 62));
			m_clrHighligh.SetStandardValue(RGB(255, 238, 194));
			m_clrHighlighBorderTopLeft.SetStandardValue(GetXtremeColor(COLOR_HIGHLIGHT));
			m_clrHighlighBorderBottomRight.SetStandardValue(GetXtremeColor(COLOR_HIGHLIGHT));
		}
	}

}
