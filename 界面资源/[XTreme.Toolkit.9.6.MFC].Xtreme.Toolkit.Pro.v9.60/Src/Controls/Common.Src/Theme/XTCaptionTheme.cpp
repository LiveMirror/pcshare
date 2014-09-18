// XTCaptionTheme.cpp: implementation of the CXTCaptionTheme class.
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
#include "XTPWinThemeWrapper.h"

#include "XTThemeManager.h"
#include "XTCaptionTheme.h"
#include "XTDefines.h"
#include "XTButton.h"
#include "XTCaption.h"
#include "XTButtonTheme.h"
#include "XTButtonThemeOffice.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_THEME_FACTORY(CXTCaptionTheme, CXTCaptionTheme, CXTCaptionThemeOffice2003)

CXTCaptionTheme::CXTCaptionTheme()
{
	RefreshMetrics();
}

void CXTCaptionTheme::RefreshMetrics()
{
	m_clrEdgeShadow.SetStandardValue(GetXtremeColor(COLOR_3DSHADOW));
	m_clrEdgeLight.SetStandardValue(GetXtremeColor(COLOR_3DHILIGHT));
}

void CXTCaptionTheme::DrawCaptionBack(CDC* pDC, CXTCaption* pCaption, CRect& rcItem)
{
	CRect rc = rcItem;

	// Set the background mode to transparent.
	pDC->SetBkMode( TRANSPARENT );


	// repaint the background.
	pDC->FillSolidRect( rc, pCaption->m_clrBorder );

	// draw the caption border.
	if (pCaption->m_dwExStyle & CPWS_EX_GROOVE_EDGE)
	{
		pDC->Draw3dRect(rc, m_clrEdgeShadow, m_clrEdgeLight);
		rc.DeflateRect(1,1);
		pDC->Draw3dRect(rc, m_clrEdgeLight, m_clrEdgeShadow);

		if (pCaption->m_nBorder == 0) {
			rc.DeflateRect(1,1);
		}
	}

	else
	if (pCaption->m_dwExStyle & CPWS_EX_RAISED_EDGE)
	{
		pDC->Draw3dRect(rc, m_clrEdgeLight, m_clrEdgeShadow);

		if (pCaption->m_nBorder == 0) {
			rc.DeflateRect(1,1);
		}
	}

	// draw the caption banner border.
	rc.DeflateRect( pCaption->m_nBorder, pCaption->m_nBorder );
	pDC->FillSolidRect(rc, pCaption->m_clrFace);
}

void CXTCaptionTheme::DrawCaptionText(CDC* pDC, CXTCaption* pCaption)
{
		// if the caption text is empty, return.
	if (pCaption->GetCaptionText().IsEmpty()) {
		return;
	}

	// Get the display area for the text.
	CRect rcText(pCaption->GetTextRect());

	// Set the text color and select the caption font.
	CXTPFontDC fontDC(pDC, pCaption->GetFont());
	pDC->SetTextColor(pCaption->m_clrText);

	// draw the text, and select the original font.
	pDC->DrawText(pCaption->GetCaptionText(), rcText,
		DT_SINGLELINE|DT_END_ELLIPSIS|DT_VCENTER|DT_CENTER|DT_NOPREFIX);


}

void CXTCaptionTheme::DrawCaptionIcon(CDC* pDC, CXTCaption* pCaption, CRect& rcItem)
{
	// If no icon was defined, just return.
	if( pCaption->m_hIcon == NULL )
		return;

	CRect rc = rcItem;
	rc.left = ( rc.right - pCaption->m_sizeIcon.cx )-( pCaption->m_nBorder + 2 );

	// stop the icon at the left hand border
	if (rc.left < pCaption->m_nBorder) {
		rc.left = pCaption->m_nBorder;
	}

	rc.top = ( rcItem.Height() - pCaption->m_sizeIcon.cy )/2;
	rc.right  = rc.left + pCaption->m_sizeIcon.cx;
	rc.bottom = rc.top  + pCaption->m_sizeIcon.cy;

	// Only draw the icon if it will fit into the space between the borders
	if (rc.right < rcItem.right - pCaption->m_nBorder) {
		// draw the icon associated with the caption.
		::DrawIconEx( pDC->GetSafeHdc(), rc.left, rc.top, pCaption->m_hIcon,
			rc.Width(), rc.Height(), NULL, (HBRUSH)NULL, DI_NORMAL);
	}
}


/////////////////////////////////////////////////////////////////////////////
// CXTCaptionButtonThemeOfficeXP
/////////////////////////////////////////////////////////////////////////////

class CXTCaptionButtonThemeOffice : public CXTButtonTheme
{
public:
	CXTCaptionButtonThemeOffice(CXTCaption* pCaption);
protected:
	virtual void DrawButtonText(CDC* pDC, UINT nState, CRect& rcItem, CXTButton* pButton);
	CXTCaption* m_pCaption;
};

CXTCaptionButtonThemeOffice::CXTCaptionButtonThemeOffice(CXTCaption* pCaption)
{
	ASSERT_VALID(pCaption);

	m_bOffsetHiliteText = FALSE;
	m_pCaption          = pCaption;
}

void CXTCaptionButtonThemeOffice::DrawButtonText(CDC* pDC, UINT nState, CRect& rcItem, CXTButton* pButton)
{
	if (!m_pCaption->HasCloseButton() && !m_pCaption->GetCaptionText().IsEmpty())
	{
		// select font into device context.
		CXTPFontDC fontDC(pDC, GetThemeFont(pButton));
		pDC->SetTextColor(GetTextColor(nState, pButton));

		if( nState & ODS_SELECTED && m_bOffsetHiliteText)
			rcItem.OffsetRect(1,1);

		// draw the text, and select the original font.
		pDC->DrawText(m_pCaption->GetCaptionText(), rcItem,
			DT_SINGLELINE|DT_END_ELLIPSIS|DT_VCENTER|DT_CENTER|DT_NOPREFIX);
	}
}

/////////////////////////////////////////////////////////////////////////////
// CXTCaptionButtonThemeOfficeXP
/////////////////////////////////////////////////////////////////////////////

class CXTCaptionButtonThemeOfficeXP : public CXTButtonThemeOfficeXP
{
public:
	CXTCaptionButtonThemeOfficeXP(CXTCaption* pCaption);

protected:
	virtual void DrawButtonText(CDC* pDC, UINT nState, CRect& rcItem, CXTButton* pButton);
	virtual BOOL DrawButtonThemeBackground(LPDRAWITEMSTRUCT lpDIS, CXTButton* pButton);
	virtual COLORREF GetTextColor(UINT nState, CXTButton* pButton);
	CXTCaption* m_pCaption;
};

CXTCaptionButtonThemeOfficeXP::CXTCaptionButtonThemeOfficeXP(CXTCaption* pCaption)
{
	ASSERT_VALID(pCaption);

	m_bAnimateIcon      = FALSE;
	m_bFadedIcon        = FALSE;
	m_bOffsetHiliteText = FALSE;
	m_pCaption          = pCaption;
}

COLORREF CXTCaptionButtonThemeOfficeXP::GetTextColor(UINT nState, CXTButton* pButton)
{
	if (nState & ODS_DISABLED)
		return GetXtremeColor(COLOR_GRAYTEXT);

	if (pButton->GetHilite() || (nState & ODS_SELECTED))
		return m_crTextHilite;

	return m_crText;
}

BOOL CXTCaptionButtonThemeOfficeXP::DrawButtonThemeBackground(LPDRAWITEMSTRUCT lpDIS, CXTButton* pButton)
{
	// define some temporary variables.
	CDC*  pDC       = CDC::FromHandle(lpDIS->hDC);
	CRect rcItem    = lpDIS->rcItem;
	int   nState    = lpDIS->itemState;

	// Paint the background.
	if (pButton->GetHilite() || (nState & ODS_SELECTED))
	{
		pDC->FillSolidRect(rcItem, (nState & ODS_SELECTED) ?
			GetXtremeColor(XPCOLOR_HIGHLIGHT_PUSHED) : GetXtremeColor(XPCOLOR_HIGHLIGHT));

		pDC->Draw3dRect(rcItem, GetXtremeColor(XPCOLOR_HIGHLIGHT_BORDER),
			GetXtremeColor(XPCOLOR_HIGHLIGHT_BORDER));
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

void CXTCaptionButtonThemeOfficeXP::DrawButtonText(CDC* pDC, UINT nState, CRect& rcItem, CXTButton* pButton)
{
	if (!m_pCaption->HasCloseButton() && !m_pCaption->GetCaptionText().IsEmpty())
	{
		// select font into device context.
		CXTPFontDC fontDC(pDC, GetThemeFont(pButton));
		pDC->SetTextColor(GetTextColor(nState, pButton));

		if( nState & ODS_SELECTED && m_bOffsetHiliteText)
			rcItem.OffsetRect(1,1);

		// draw the text, and select the original font.
		pDC->DrawText(m_pCaption->GetCaptionText(), rcItem,
			DT_SINGLELINE|DT_END_ELLIPSIS|DT_VCENTER|DT_CENTER|DT_NOPREFIX);
	}
}


CXTButtonTheme* CXTCaptionTheme::GetButtonTheme(CXTCaption* pCaption)
{
	if (GetTheme() != xtThemeDefault)
		return new CXTCaptionButtonThemeOfficeXP(pCaption);
	return new CXTCaptionButtonThemeOffice(pCaption);
}


class CXTButtonCaptionThemeOffice2003 : public CXTButtonThemeOffice2003
{
public:
	CXTButtonCaptionThemeOffice2003(BOOL bPrimaryCaption)
	{
		m_bPrimaryCaption = bPrimaryCaption;

	}
	virtual BOOL DrawButtonThemeBackground(LPDRAWITEMSTRUCT lpDIS, CXTButton* pButton)
	{
		CDC*  pDC       = CDC::FromHandle(lpDIS->hDC);
		CRect rc    = lpDIS->rcItem;
		int   nState    = lpDIS->itemState;

		// Paint the background.
		if (pButton->GetHilite() || (nState & ODS_SELECTED))
		{
			return CXTButtonThemeOffice2003::DrawButtonThemeBackground(lpDIS,  pButton);
		}
		else
		{
			if (m_bPrimaryCaption)
			{
				XTPDrawHelpers()->GradientFill(pDC, rc, XTPColorManager()->grcCaption, FALSE);
				m_crText = GetXtremeColor(COLOR_WINDOW);
			}
			else
			{
				XTPDrawHelpers()->GradientFill(pDC, rc, XTPColorManager()->grcToolBar, FALSE);
			}
		}

		return TRUE;
	}
	BOOL m_bPrimaryCaption;
};

CXTCaptionThemeOffice2003::CXTCaptionThemeOffice2003(BOOL bPrimaryCaption)
{
	m_bPrimaryCaption = bPrimaryCaption;
}

void CXTCaptionThemeOffice2003::FillCaptionGradient(CDC* pDC, CRect& rc)
{
	if (m_bPrimaryCaption)
	{
		XTPDrawHelpers()->GradientFill(pDC, rc, XTPColorManager()->grcCaption, FALSE);
	}
	else
	{
		XTPDrawHelpers()->GradientFill(pDC, rc, XTPColorManager()->grcToolBar, FALSE);
	}
}

void CXTCaptionThemeOffice2003::DrawCaptionBack(CDC* pDC, CXTCaption* /*pCaption*/, CRect& rc)
{
	pDC->SetBkMode( TRANSPARENT );

	FillCaptionGradient(pDC, rc);

	if (m_bPrimaryCaption)
		pDC->Draw3dRect(rc, GetXtremeColor(XPCOLOR_FRAME), GetXtremeColor(XPCOLOR_FRAME));
	else
		pDC->FillSolidRect(rc.left, rc.bottom - 1, rc.Width(), 1, GetXtremeColor(XPCOLOR_FRAME));
}

CXTButtonTheme* CXTCaptionThemeOffice2003::GetButtonTheme(CXTCaption* /*pCaption*/)
{
	return new CXTButtonCaptionThemeOffice2003(m_bPrimaryCaption);

}
