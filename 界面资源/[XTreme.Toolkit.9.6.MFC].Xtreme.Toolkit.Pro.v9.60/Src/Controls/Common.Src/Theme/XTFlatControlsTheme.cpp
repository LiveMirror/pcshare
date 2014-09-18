// XTTabCtrlTheme.cpp: implementation of the CXTTabCtrlTheme class.
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

#include "XTFlatControlsTheme.h"

#include "XTDefines.h"
#include "XTUtil.h"
#include "XTGlobal.h"
#include "XTVC50Helpers.h"
#include "XTWndHook.h"
#include "XTFlatComboBox.h"

#include "XTHelpers.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


IMPLEMENT_THEME_FACTORY(CXTFlatControlsTheme, CXTFlatControlsThemeOfficeXP, CXTFlatControlsThemeOffice2003)

//===========================================================================
// ÑXTTabCtrlTheme class
//===========================================================================

CXTFlatControlsTheme::CXTFlatControlsTheme()
{
}

CXTFlatControlsTheme::~CXTFlatControlsTheme()
{

}

void CXTFlatControlsTheme::RefreshMetrics()
{
	XTPColorManager()->RefreshColors();

}

void CXTFlatControlsTheme::DrawFlatEdit(CDC* pDC, CXTFlatEdit* pEdit, int eState )
{
	CRect rcClient;
	pEdit->GetClientRect( &rcClient );

	bool bRaised = ( eState == xtMouseHover || CWnd::GetFocus() == pEdit );

	rcClient.InflateRect( 1,1 );
	pDC->Draw3dRect( rcClient,
		pEdit->IsWindowEnabled( ) ? GetXtremeColor(COLOR_3DFACE) : GetXtremeColor(COLOR_WINDOW),
		pEdit->IsWindowEnabled( ) ? GetXtremeColor(COLOR_3DFACE) : GetXtremeColor(COLOR_WINDOW));

	rcClient.InflateRect( 1,1 );
	pDC->Draw3dRect( rcClient,
		bRaised ? GetXtremeColor(COLOR_3DSHADOW)  : GetXtremeColor(COLOR_3DFACE),
		bRaised ? GetXtremeColor(COLOR_3DHILIGHT) : GetXtremeColor(COLOR_3DFACE) );
}


void CXTFlatControlsTheme::DrawFlatComboBox(CDC* pDC, CXTFlatComboBox* pCombo, int eState )
{
	CRect rcItem;
	pCombo->GetClientRect( &rcItem );

	// Cover up dark 3D shadow.
	if ( eState == xtMouseNormal )
	{
		pDC->Draw3dRect( rcItem,
			GetXtremeColor(COLOR_3DFACE),
			GetXtremeColor(COLOR_3DFACE) );
	}
	else
	{
		pDC->Draw3dRect( rcItem,
			GetXtremeColor(COLOR_3DSHADOW),
			GetXtremeColor(COLOR_3DHILIGHT) );
	}

	rcItem.DeflateRect( 1,1 );

	if ( !pCombo->IsWindowEnabled( ) )
	{
		pDC->Draw3dRect( rcItem,
			GetXtremeColor(COLOR_3DHILIGHT),
			GetXtremeColor(COLOR_3DHILIGHT) );
	}
	else
	{
		pDC->Draw3dRect( rcItem,
			GetXtremeColor(COLOR_3DFACE),
			GetXtremeColor(COLOR_3DFACE) );
	}

	// Cover up dark 3D shadow on drop arrow.
	rcItem.DeflateRect( 1,1 );
	rcItem.left = rcItem.right-XTAuxData().cxHThumb;
	pDC->Draw3dRect( rcItem,
		GetXtremeColor(COLOR_3DFACE),
		GetXtremeColor(COLOR_3DFACE) );

	// Cover up NORMAL 3D shadow on drop arrow.
	rcItem.DeflateRect( 1,1 );
	pDC->Draw3dRect( rcItem,
		GetXtremeColor(COLOR_3DFACE),
		GetXtremeColor(COLOR_3DFACE) );

	if ( pCombo->IsWindowEnabled( ) )
	{
		rcItem.InflateRect( 0,1 );
		switch ( eState )
		{
		case xtMouseNormal:
			pDC->Draw3dRect( rcItem,
				GetXtremeColor(COLOR_3DHILIGHT),
				GetXtremeColor(COLOR_3DHILIGHT) );
			rcItem.left -= 1;
			pDC->Draw3dRect( rcItem,
				GetXtremeColor(COLOR_3DHILIGHT),
				GetXtremeColor(COLOR_3DHILIGHT) );
			break;

		case xtMouseHover:
			pDC->Draw3dRect( rcItem,
				GetXtremeColor(COLOR_3DHILIGHT),
				GetXtremeColor(COLOR_3DSHADOW) );
			break;

		case xtMouseSelect:
			rcItem.OffsetRect( 1,1 );
			pDC->Draw3dRect( rcItem,
				GetXtremeColor(COLOR_3DSHADOW),
				GetXtremeColor(COLOR_3DHILIGHT) );
			break;
		}
	}

}

static void DrawDownArrow(
			// Device context for drawing
			CDC* pDC,

			// a rectangle in which to draw
			const CRect& rcArrow
		)
{
	// 1000 0011  0 83
	// 1100 0111  1 C7
	// 1110 1111  2 EF
	static const WORD bits[3] = {0x83, 0xC7, 0xEF};
	CBitmap downArrow;
	downArrow.CreateBitmap(8, 3, 1, 1, bits);
	CDC memDC;
	memDC.CreateCompatibleDC(pDC);
	memDC.SelectObject(&downArrow);
	pDC->BitBlt((rcArrow.left + rcArrow.right - 3) / 2,
				(rcArrow.top + rcArrow.bottom - 1) / 2,
				5, 3, &memDC, 1, 0, SRCCOPY);
}


CXTFlatControlsThemeOfficeXP::CXTFlatControlsThemeOfficeXP()
{
	RefreshMetrics();
}

void CXTFlatControlsThemeOfficeXP::RefreshMetrics()
{
	CXTFlatControlsTheme::RefreshMetrics();

	m_clrHighlightText.SetStandardValue(GetXtremeColor(XPCOLOR_HIGHLIGHT_TEXT));
	m_clrPushedText.SetStandardValue(GetXtremeColor(XPCOLOR_PUSHED_TEXT));
	m_clrHighlight.SetStandardValue(GetXtremeColor(XPCOLOR_HIGHLIGHT));
	m_clrPushed.SetStandardValue(GetXtremeColor(XPCOLOR_HIGHLIGHT_PUSHED));
	m_clrFrameHighlight.SetStandardValue(GetXtremeColor(XPCOLOR_HIGHLIGHT_BORDER));
	m_clrFrameNormal.SetStandardValue(GetXtremeColor(COLOR_WINDOW));
	m_clrNormal.SetStandardValue(GetXtremeColor(COLOR_3DFACE));
	m_clrFrameEdit.SetStandardValue(GetXtremeColor(COLOR_3DFACE));
}


void CXTFlatControlsThemeOfficeXP::DrawFlatComboBox(CDC* pDC, CXTFlatComboBox* pCombo, int eState )
{

	BOOL bHilight = ( eState != xtMouseNormal );
	CRect rcItem;
	pCombo->GetClientRect(&rcItem);

	COLORREF clrBack, clrFore, clrFrame;
	if (pCombo->GetDroppedState())
	{
		clrFore  = m_clrPushedText;
		clrBack  = m_clrPushed;
		clrFrame = m_clrFrameHighlight;
	}
	else
	{
		clrFore  = m_clrHighlightText;
		clrBack  = (bHilight) ? m_clrHighlight: m_clrNormal;
		clrFrame = (bHilight) ? m_clrFrameHighlight : m_clrFrameNormal;
	}

	pDC->Draw3dRect(rcItem, GetXtremeColor(COLOR_3DFACE), GetXtremeColor(COLOR_3DFACE));
	rcItem.DeflateRect(1,1);

	CRect rectArrow = rcItem;
	rectArrow.left = rcItem.right-XTAuxData().cxHThumb - 1;
	pDC->Draw3dRect(rcItem, clrFrame, clrFrame);
	pDC->Draw3dRect(rectArrow, clrFrame, clrFrame);

	rectArrow.DeflateRect(1, 1);

	pDC->FillSolidRect(rectArrow, clrBack);

	CXTContextTextColorHandler handlerTextColor(pDC, clrFore);
	CXTContextBkColorHandler handlerBkColor(pDC, clrBack);
	DrawDownArrow(pDC, rectArrow);
}

void CXTFlatControlsThemeOfficeXP::DrawFlatEdit(CDC* pDC, CXTFlatEdit* pEdit, int eState )
{
	CRect rcClient;
	pEdit->GetClientRect( &rcClient );

	bool bRaised = ( eState == xtMouseHover || CWnd::GetFocus() == pEdit );

	rcClient.InflateRect( 1,1 );
	pDC->Draw3dRect( rcClient, GetXtremeColor(COLOR_WINDOW), GetXtremeColor(COLOR_WINDOW));

	rcClient.InflateRect( 1,1 );
	pDC->Draw3dRect( rcClient,
		bRaised ? m_clrFrameHighlight: m_clrFrameEdit, bRaised ? m_clrFrameHighlight: m_clrFrameEdit );
}

//////////////////////////////////////////////////////////////////////////
// CXTFlatControlsThemeOffice2003

CXTFlatControlsThemeOffice2003::CXTFlatControlsThemeOffice2003()
{
	RefreshMetrics();

}

void CXTFlatControlsThemeOffice2003::RefreshMetrics()
{
	CXTFlatControlsThemeOfficeXP::RefreshMetrics();

	XTPCurrentSystemTheme systemTheme = XTPDrawHelpers()->GetCurrentSystemTheme();

	switch (systemTheme)
	{
	case xtpSystemThemeBlue:
		m_clrHighlightText.SetStandardValue(0);
		m_clrPushedText.SetStandardValue(0);
		m_clrHighlight.SetStandardValue(RGB(255, 238, 194));
		m_clrPushed.SetStandardValue(RGB(254, 128,62));
		m_clrFrameHighlight.SetStandardValue(RGB(0, 0, 128));
		m_clrFrameNormal.SetStandardValue(RGB(127, 157, 185));
		m_clrNormal.SetStandardValue(RGB(169, 199, 240));
		break;

	case xtpSystemThemeOlive:
		m_clrHighlightText.SetStandardValue(0);
		m_clrPushedText.SetStandardValue(0);
		m_clrHighlight.SetStandardValue(RGB(255, 238, 194));
		m_clrPushed.SetStandardValue(RGB(254, 128,62));
		m_clrFrameHighlight.SetStandardValue(RGB(63, 93, 56));
		m_clrFrameNormal.SetStandardValue(RGB(164, 185, 127));
		m_clrNormal.SetStandardValue(RGB(197, 212, 159));
		break;

	case xtpSystemThemeSilver:
		m_clrHighlightText.SetStandardValue(0);
		m_clrPushedText.SetStandardValue(0);
		m_clrHighlight.SetStandardValue(RGB(255, 238, 194));
		m_clrPushed.SetStandardValue(RGB(254, 128,62));
		m_clrFrameHighlight.SetStandardValue(RGB(75, 75, 111));
		m_clrFrameNormal.SetStandardValue(RGB(165, 172, 178));
		m_clrNormal.SetStandardValue(RGB(192, 192, 211));
		break;
	default:
		m_clrFrameNormal.SetStandardValue(GetXtremeColor(COLOR_3DSHADOW));
	}
	m_clrFrameEdit.SetStandardValue(m_clrFrameNormal);
}
