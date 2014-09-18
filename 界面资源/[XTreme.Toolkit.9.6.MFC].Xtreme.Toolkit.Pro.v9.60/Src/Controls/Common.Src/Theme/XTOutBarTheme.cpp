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
#include "XTDefines.h"
#include "XTPDrawHelpers.h"
#include "XTPImageManager.h"
#include "XTPColorManager.h"
#include "XTOutBarTheme.h"
#include "XTWndHook.h"
#include "XTFlatComboBox.h"
#include "XTOutBarCtrl.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_THEME_FACTORY(CXTOutBarTheme, CXTOutBarThemeOfficeXP, CXTOutBarThemeOffice2003)
//===========================================================================
// CXTOutBarTheme class
//===========================================================================

CXTOutBarTheme::CXTOutBarTheme()
{
	m_bFlatIcon = FALSE;
	m_bPlainText =FALSE;
}

CXTOutBarTheme::~CXTOutBarTheme()
{

}

void CXTOutBarTheme::RefreshMetrics()
{
	XTPColorManager()->RefreshColors();

	m_clrBack.SetStandardValue(GetXtremeColor(COLOR_3DSHADOW));
	m_clrText.SetStandardValue(GetXtremeColor(COLOR_WINDOW));
}


void CXTOutBarTheme::DrawItemIcon( CDC* pDC, CPoint pt, CXTOutBarItem* pBarItem, CImageList* pImageList)
{
	int nIndex = pBarItem->GetIndex();

	if ( pBarItem->IsEnabled() )
	{
		pImageList->Draw(pDC, nIndex, pt, ILD_NORMAL);
	}
	else
	{
		HICON hIcon = pImageList->ExtractIcon(nIndex);

		if (m_bFlatIcon)
		{
			CBrush brush( GetXtremeColor(COLOR_3DSHADOW) );
			pDC->DrawState( pt, CSize( 0,0 ), hIcon,
				DST_ICON | DSS_MONO, ( HBRUSH )brush );
		}
		else
		{
			pDC->DrawState( pt, CSize( 0,0 ), hIcon,
				DSS_DISABLED | DST_ICON, ( HBRUSH )NULL );
		}
		DestroyIcon(hIcon);
	}
}

void CXTOutBarTheme::DrawItemText( CDC* pDC, CRect rc, CXTOutBarItem* pBarItem, UINT nFormat)
{
	CString strText = pBarItem->GetName();

	if (pBarItem->IsEnabled( ))
	{
		pDC->DrawText( strText, rc, nFormat );
	}
	else
	{
		if ( m_bPlainText)
		{
			pDC->SetTextColor( GetXtremeColor(COLOR_3DSHADOW) );
			pDC->DrawText( strText, rc, nFormat );
		}
		else
		{
			rc.OffsetRect(1,1);
			pDC->SetTextColor( GetXtremeColor(COLOR_WINDOW) );
			pDC->DrawText( strText, rc, nFormat );

			rc.OffsetRect(-1,-1);
			pDC->SetTextColor( GetXtremeColor(COLOR_3DSHADOW) );
			pDC->DrawText( strText, rc, nFormat );
		}
	}
}

void CXTOutBarTheme::DrawFolder(CDC* pDC, CRect rect, CXTOutBarFolder* pBarFolder, const XTMouseState eHilight)
{
	CRect rcFolder(rect);

	CXTPFontDC fnt(pDC, pBarFolder->GetControl()->GetFontX());
	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(GetXtremeColor(COLOR_BTNTEXT));

	pDC->FillSolidRect(rect, GetXtremeColor(COLOR_3DFACE));

	switch (eHilight)
	{
	case xtMouseNormal:
		pDC->Draw3dRect(rect, GetXtremeColor(COLOR_3DHILIGHT), GetXtremeColor(COLOR_3DSHADOW));
		break;

	case xtMouseSelect:
		pDC->Draw3dRect(rect, GetXtremeColor(COLOR_3DDKSHADOW), GetXtremeColor(COLOR_3DFACE));
		rect.InflateRect(-1,-1);
		pDC->Draw3dRect(rect, GetXtremeColor(COLOR_3DSHADOW), GetXtremeColor(COLOR_3DHILIGHT));
		break;

	case xtMouseHover:
		pDC->Draw3dRect(rect, GetXtremeColor(COLOR_3DFACE), GetXtremeColor(COLOR_3DDKSHADOW));
		rect.InflateRect(-1,-1);
		pDC->Draw3dRect(rect, GetXtremeColor(COLOR_3DHILIGHT), GetXtremeColor(COLOR_3DSHADOW));
		break;
	}

	rcFolder.DeflateRect(4, 0);

	pDC->DrawText(pBarFolder->GetName(), rcFolder,
		DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS);

}

void CXTOutBarTheme::FillInsideRect(CDC* pDC, CRect rc)
{
	pDC->FillSolidRect(rc, m_clrBack);
}


void CXTOutBarTheme::DrawIconFrame(CDC* pDC, CRect rcIcon, BOOL bSelected, BOOL bPressed)
{
	if (!bSelected)
	{
		if (bPressed)
			pDC->Draw3dRect(rcIcon, GetXtremeColor(COLOR_WINDOWFRAME), GetXtremeColor(COLOR_3DFACE));
		else
			pDC->Draw3dRect(rcIcon, GetXtremeColor(COLOR_3DFACE), GetXtremeColor(COLOR_WINDOWFRAME));
	} else
	{
		pDC->Draw3dRect(rcIcon, GetXtremeColor(COLOR_WINDOWFRAME), GetXtremeColor(COLOR_3DFACE));

	}
}

//////////////////////////////////////////////////////////////////////////
// CXTOutBarThemeOfficeXP

CXTOutBarThemeOfficeXP::CXTOutBarThemeOfficeXP()
{
	m_bFlatIcon = TRUE;
	m_bPlainText = TRUE;
	RefreshMetrics();
}

void CXTOutBarThemeOfficeXP::RefreshMetrics()
{
	CXTOutBarTheme::RefreshMetrics();

	m_clrBack.SetStandardValue(GetXtremeColor(XPCOLOR_TOOLBAR_FACE));
	m_clrText.SetStandardValue(GetXtremeColor(COLOR_BTNTEXT));
}

void CXTOutBarThemeOfficeXP::DrawIconFrame(CDC* pDC, CRect rcIcon, BOOL bSelected, BOOL bPressed)
{
	if (!bSelected)
	{
		pDC->FillSolidRect(rcIcon, GetXtremeColor(bPressed? XPCOLOR_HIGHLIGHT_PUSHED: XPCOLOR_HIGHLIGHT));
		pDC->Draw3dRect(rcIcon, GetXtremeColor(COLOR_HIGHLIGHT), GetXtremeColor(COLOR_HIGHLIGHT));
	} else
	{
		pDC->Draw3dRect(rcIcon, GetXtremeColor(COLOR_HIGHLIGHT), GetXtremeColor(COLOR_HIGHLIGHT));
	}
}

//////////////////////////////////////////////////////////////////////////
//
CXTOutBarThemeOffice2003::CXTOutBarThemeOffice2003()
{
	RefreshMetrics();
}

void CXTOutBarThemeOffice2003::RefreshMetrics()
{
	CXTOutBarThemeOfficeXP::RefreshMetrics();

	m_grcPushed.SetStandardValue(GetXtremeColor(XPCOLOR_HIGHLIGHT_PUSHED));
	m_grcHot.SetStandardValue(GetXtremeColor(XPCOLOR_HIGHLIGHT));
	m_grcSelected.SetStandardValue(GetXtremeColor(XPCOLOR_HIGHLIGHT_CHECKED));

	m_bLunaTheme = FALSE;

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
			m_grcSelected.SetStandardValue(RGB(251, 230, 148), RGB(238, 149, 21));
			m_grcPushed.SetStandardValue(RGB(232, 127, 8), RGB(247, 218, 124));
			m_grcHot.SetStandardValue(RGB(255, 255, 220), RGB(247, 192, 91));
			m_bLunaTheme = TRUE;
		}
	}
}

void CXTOutBarThemeOffice2003::DrawFolder(CDC* pDC, CRect rect, CXTOutBarFolder* pBarFolder, const XTMouseState eHilight)
{
	CRect rcFolder(rect);

	CXTPFontDC fnt(pDC, pBarFolder->GetControl()->GetFontX());
	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(GetXtremeColor(COLOR_BTNTEXT));

	switch (eHilight)
	{
	case xtMouseNormal:
		XTPDrawHelpers()->GradientFill(pDC, rect, XTPColorManager()->grcDockBar.clrDark, XTPColorManager()->grcDockBar.clrLight, FALSE);
		break;

	case xtMouseSelect:
		XTPDrawHelpers()->GradientFill(pDC, rect, m_grcPushed, FALSE);
		break;

	case xtMouseHover:
		XTPDrawHelpers()->GradientFill(pDC, rect, m_grcSelected, FALSE);
		break;
	}
	pDC->Draw3dRect(rect, GetXtremeColor(COLOR_3DHILIGHT), GetXtremeColor(XPCOLOR_FRAME));

	rcFolder.DeflateRect(4, 0);

	pDC->DrawText(pBarFolder->GetName(), rcFolder,
		DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS);

}

void CXTOutBarThemeOffice2003::DrawIconFrame(CDC* pDC, CRect rcIcon, BOOL bSelected, BOOL bPressed)
{
	if (!m_bLunaTheme)
	{
		CXTOutBarThemeOfficeXP::DrawIconFrame(pDC, rcIcon, bSelected, bPressed);
		return;
	}

	if (!bSelected)
	{
		if (bPressed)
			XTPDrawHelpers()->GradientFill(pDC, rcIcon, m_grcPushed, FALSE);
		else
			XTPDrawHelpers()->GradientFill(pDC, rcIcon, m_grcHot, FALSE);
	}
	else
	{
		XTPDrawHelpers()->GradientFill(pDC, rcIcon, m_grcSelected, FALSE);

	}
	pDC->Draw3dRect(rcIcon, GetXtremeColor(XPCOLOR_FRAME), GetXtremeColor(XPCOLOR_FRAME));
}
