// XTPPopupPaintManager.cpp: implementation of the CXTPPopupPaintManager class.
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
#include "XTPImageManager.h"

#include "XTPPopupPaintManager.h"
#include "XTPPopupItem.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CXTPPopupPaintManager::CXTPPopupPaintManager()
{
	RefreshMetrics();
}

CXTPPopupPaintManager::~CXTPPopupPaintManager()
{
}

void CXTPPopupPaintManager::RefreshMetrics()
{
	XTPColorManager()->RefreshColors();


	m_fntText.DeleteObject();

	LOGFONT lfIcon;
	VERIFY( ::SystemParametersInfo( SPI_GETICONTITLELOGFONT, sizeof( lfIcon ), &lfIcon, 0 ) );

	VERIFY( m_fntText.CreateFontIndirect(&lfIcon) );

	m_clrText = GetXtremeColor(COLOR_BTNTEXT);
	m_clrBackground.SetStandardValue(GetXtremeColor(COLOR_3DFACE));
	m_clrFrame.SetStandardValue(GetXtremeColor(COLOR_3DSHADOW));

	m_clrButtonSelected = m_clrButtonPressed = (COLORREF)-1;
	m_clrButtonPressedBorder.SetStandardValue(GetXtremeColor(COLOR_3DSHADOW), GetXtremeColor(COLOR_3DHIGHLIGHT));
	m_clrButtonSelectedBorder.SetStandardValue(GetXtremeColor(COLOR_3DHIGHLIGHT), GetXtremeColor(COLOR_3DSHADOW));

}

void CXTPPopupPaintManager::DrawBackground(CDC* pDC, CRect rcClient)
{
	XTPDrawHelpers()->GradientFill(pDC, rcClient, m_clrBackground, TRUE);

	if (m_clrFrame.clrLight != (COLORREF)-1)
		pDC->Draw3dRect(rcClient, m_clrFrame.clrLight, m_clrFrame.clrDark);
}

void CXTPPopupPaintManager::DrawButton(CDC* pDC, CXTPPopupItem* pItem)
{
	if (pItem->IsPressed() && pItem->IsSelected())
	{
		if (m_clrButtonPressed != (COLORREF)-1)
			pDC->FillSolidRect(pItem->GetRect(), m_clrButtonPressed);
		pDC->Draw3dRect(pItem->GetRect(), m_clrButtonPressedBorder.clrLight, m_clrButtonPressedBorder.clrDark);
	}
	else if (pItem->IsSelected() || pItem->IsPressed())
	{
		if (m_clrButtonSelected != (COLORREF)-1)
			pDC->FillSolidRect(pItem->GetRect(), m_clrButtonSelected);
		pDC->Draw3dRect(pItem->GetRect(), m_clrButtonSelectedBorder.clrLight, m_clrButtonSelectedBorder.clrDark);
	}
}

void CXTPPopupPaintManager::SetFont(HFONT hFont)
{
	LOGFONT lf;
	CFont::FromHandle(hFont)->GetLogFont(&lf);

	m_fntText.DeleteObject();
	m_fntText.CreateFontIndirect(&lf);
}

void CXTPPopupPaintManager::DrawItem(CDC* pDC, CXTPPopupItem* pItem)
{
	CRect rc = pItem->GetRect();

	if (pItem->GetBackgroundColor() != (COLORREF)-1)
		pDC->FillSolidRect(rc, pItem->GetBackgroundColor());

	if (pItem->GetBorderColor() != (COLORREF)-1)
		pDC->Draw3dRect(rc, pItem->GetBorderColor(), pItem->GetBorderColor());

	CXTPImageManagerIcon* pIcon = pItem->GetImage();

	if (pIcon)
	{
		rc.OffsetRect(pItem->GetIconOffset());

		if (pItem->IsPressed() && pItem->IsSelected())
		{
			if (pItem->IsButton())
				DrawButton(pDC, pItem);

			pIcon->Draw(pDC, rc.TopLeft(), pIcon->GetCheckedIcon());
		}
		else if (pItem->IsSelected() || pItem->IsPressed())
		{
			if (pItem->IsButton())
				DrawButton(pDC, (CXTPPopupItem*)pItem);

			pIcon->Draw(pDC, rc.TopLeft(), pIcon->GetHotIcon());
		}
		else
		{
			pIcon->Draw(pDC, rc.TopLeft(), pIcon->GetIcon());
		}
		return;
	}

	if (pItem->IsButton())
		DrawButton(pDC, (CXTPPopupItem*)pItem);

	if (!pItem->GetCaption().IsEmpty())
	{

		CFont fntUnderline;
		CFont* pFont = pItem->GetTextFont();

		if ((pItem->IsSelected() && pItem->IsHyperLink()) || pItem->IsBold())
		{
			LOGFONT lpLogFont;
			pFont->GetLogFont(&lpLogFont);
			lpLogFont.lfUnderline = (pItem->IsSelected() && pItem->IsHyperLink());
			lpLogFont.lfWeight = pItem->IsBold() ? FW_BOLD : FW_NORMAL;

			VERIFY(fntUnderline.CreateFontIndirect(&lpLogFont));
			pFont = &fntUnderline;
		}
		CRect rcText(pItem->GetRect());

		CXTPFontDC font(pDC, pFont);
		
		if ((pItem->GetTextAlignment() & (DT_WORDBREAK|DT_VCENTER)) == (DT_WORDBREAK|DT_VCENTER))
		{
			CRect rc(rcText.left, 0, rcText.right, 0);
			pDC->DrawText(pItem->GetCaption(), rc, pItem->GetTextAlignment()|DT_CALCRECT);
			rcText.top = (rcText.top + rcText.bottom - rc.bottom) / 2;
		}

		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(pItem->GetTextColor() == (COLORREF)-1? m_clrText: pItem->GetTextColor());

		pDC->DrawText(pItem->GetCaption(), rcText, pItem->GetTextAlignment());
	}
}

//////////////////////////////////////////////////////////////////////////
// CXTPPopupThemeOffice2000

CXTPPopupThemeOffice2000::CXTPPopupThemeOffice2000()
{
	RefreshMetrics();
}

CXTPPopupThemeOffice2000::~CXTPPopupThemeOffice2000()
{
}

void CXTPPopupThemeOffice2000::RefreshMetrics()
{
	CXTPPopupPaintManager::RefreshMetrics();

	m_clrFrame.SetStandardValue(GetXtremeColor(COLOR_3DHIGHLIGHT), GetXtremeColor(COLOR_3DSHADOW));
}

void CXTPPopupThemeOffice2000::DrawBackground(CDC* pDC, CRect rcClient)
{
	XTPDrawHelpers()->GradientFill(pDC, rcClient, m_clrBackground, FALSE);
	pDC->Draw3dRect(rcClient, m_clrFrame.clrLight, m_clrFrame.clrDark);
	rcClient.DeflateRect(1, 1);

	//rcClient.DeflateRect(2, 2);
	CRect rcBackground(rcClient.left, rcClient.top, rcClient.right, rcClient.top + 22);
	XTPDrawHelpers()->GradientFill(pDC, rcBackground, RGB(0,0,128), RGB(24,180,192), TRUE);

	rcClient.DeflateRect(1, 1);
	rcBackground = CRect(rcClient.left, rcBackground.top + 23 , rcClient.right, rcClient.bottom);
	pDC->Draw3dRect(rcBackground, m_clrFrame.clrDark, m_clrFrame.clrLight);

	rcBackground.DeflateRect(1, 1);
	pDC->Draw3dRect(rcBackground, m_clrFrame.clrLight, m_clrFrame.clrDark);

	rcBackground.DeflateRect(1, 1);
	rcBackground.right = rcBackground.left + 30;
	XTPDrawHelpers()->GradientFill(pDC, rcBackground, RGB(0,0,128), RGB(0,0,128), FALSE);
}

//////////////////////////////////////////////////////////////////////////
// CXTPPopupThemeOfficeXP

CXTPPopupThemeOfficeXP::CXTPPopupThemeOfficeXP()
{
	RefreshMetrics();
}

CXTPPopupThemeOfficeXP::~CXTPPopupThemeOfficeXP()
{
}

void CXTPPopupThemeOfficeXP::RefreshMetrics()
{
	CXTPPopupThemeOffice2000::RefreshMetrics();

	m_clrBackground.SetStandardValue(GetXtremeColor(XPCOLOR_TOOLBAR_FACE));
	m_clrButtonSelected =  GetXtremeColor(XPCOLOR_HIGHLIGHT);
	m_clrButtonPressed = GetXtremeColor(XPCOLOR_HIGHLIGHT_PUSHED);
	m_clrButtonSelectedBorder.SetStandardValue(GetXtremeColor(XPCOLOR_HIGHLIGHT_BORDER));
	m_clrButtonPressedBorder.SetStandardValue(GetXtremeColor(XPCOLOR_HIGHLIGHT_BORDER));
	m_clrFrame.SetStandardValue(GetXtremeColor(COLOR_3DSHADOW));
}

void CXTPPopupThemeOfficeXP::DrawBackground(CDC* pDC, CRect rcClient)
{
	XTPDrawHelpers()->GradientFill(pDC, rcClient, m_clrBackground, FALSE);
	pDC->Draw3dRect(rcClient, m_clrFrame.clrLight, m_clrFrame.clrDark);
	rcClient.DeflateRect(1, 1);

	CRect rcBackground(rcClient.left, rcClient.top, rcClient.right, rcClient.top + 22);
	XTPDrawHelpers()->GradientFill(pDC, rcBackground, RGB(0,0,128), RGB(24,180,192), TRUE);

	rcBackground = CRect(rcClient.left, rcBackground.top + 22 , rcClient.right, rcClient.bottom);
	rcBackground.DeflateRect(1, 1);
	rcBackground.right = rcBackground.left + 30;
	XTPDrawHelpers()->GradientFill(pDC, rcBackground, RGB(0,0,128), RGB(0,0,128), FALSE);
}

//////////////////////////////////////////////////////////////////////////
// CXTPPopupThemeOffice2003

CXTPPopupThemeOffice2003::CXTPPopupThemeOffice2003()
{
	RefreshMetrics();
}

CXTPPopupThemeOffice2003::~CXTPPopupThemeOffice2003()
{
}

void CXTPPopupThemeOffice2003::RefreshMetrics()
{
	CXTPPopupThemeOfficeXP::RefreshMetrics();

	m_clrBackground.SetStandardValue(XTPColorManager()->grcDockBar.clrDark, XTPColorManager()->grcDockBar.clrLight);//.SetStandardValue(RGB(214, 231, 252), RGB(168, 198, 238));
	m_clrGripper = XTPColorManager()->grcShortcutBarGripper;
	m_clrFrame.SetStandardValue(GetXtremeColor(XPCOLOR_FRAME));

	if (!XTPColorManager()->IsLunaColorsDisabled())
	{

		XTPCurrentSystemTheme systemTheme = XTPDrawHelpers()->GetCurrentSystemTheme();

		switch (systemTheme)
		{
		case xtpSystemThemeBlue:
			m_clrButtonSelected = RGB(255, 238, 194);
			m_clrButtonPressed = RGB(254, 128, 62);
			m_clrButtonPressedBorder.SetStandardValue(RGB(0, 0, 128));
			m_clrButtonSelectedBorder.SetStandardValue(RGB(0, 0, 128));
			break;

		case xtpSystemThemeOlive:
			m_clrButtonSelected = RGB(255, 238, 194);
			m_clrButtonPressed = RGB(254, 128, 62);
			m_clrButtonPressedBorder.SetStandardValue(RGB(63, 93, 56));
			m_clrButtonSelectedBorder.SetStandardValue(RGB(63, 93, 56));
			break;

		case xtpSystemThemeSilver:
			m_clrButtonSelected = RGB(255, 238, 194);
			m_clrButtonPressed = RGB(254, 128, 62);
			m_clrButtonPressedBorder.SetStandardValue(RGB(75, 75, 111));
			m_clrButtonSelectedBorder.SetStandardValue(RGB(75, 75, 111));
			break;
		}
	}
}

void CXTPPopupThemeOffice2003::DrawBackground(CDC* pDC, CRect rcClient)
{
	XTPDrawHelpers()->GradientFill(pDC, rcClient, m_clrBackground, FALSE);
	pDC->Draw3dRect(rcClient, m_clrFrame.clrLight, m_clrFrame.clrDark);
	rcClient.DeflateRect(1, 1);

	CRect rcGripper(rcClient);
	rcGripper.bottom = rcGripper.top + 7;
	XTPDrawHelpers()->GradientFill(pDC, rcGripper, m_clrGripper, FALSE);

	int nRight = max (2, (rcGripper.Width() - 4 * 9) / 2);
	for (int i = 0; i <9; i++)
	{
		pDC->FillSolidRect(nRight, rcGripper.top + 1, 2, 2, RGB(40, 50, 71));
		pDC->FillSolidRect(nRight + 1, rcGripper.top + 2, 2, 2, RGB(249, 249, 251));
		pDC->FillSolidRect(nRight + 1, rcGripper.top + 2, 1, 1, RGB(97, 116, 152));

		nRight += 4;
	}
}

//////////////////////////////////////////////////////////////////////////
// CXTPPopupThemeMSN

CXTPPopupThemeMSN::CXTPPopupThemeMSN()
{
	RefreshMetrics();
}

CXTPPopupThemeMSN::~CXTPPopupThemeMSN()
{
}

void CXTPPopupThemeMSN::RefreshMetrics()
{
	CXTPPopupThemeOffice2000::RefreshMetrics();


	m_clrButtonSelected = RGB(194, 238, 255);
	m_clrButtonSelectedBorder.SetStandardValue(RGB(114, 142, 184));

	m_clrButtonPressed = RGB(134, 162, 224);
	m_clrButtonPressedBorder.SetStandardValue(RGB(114, 142, 184));

}

void CXTPPopupThemeMSN::DrawBackground(CDC* pDC, CRect rcClient)
{
	pDC->Draw3dRect(rcClient, RGB(166, 180, 207), RGB(69, 86, 144));
	rcClient.DeflateRect(1, 1);
	pDC->Draw3dRect(rcClient, RGB(255, 255, 255), RGB(207, 222, 244));
	rcClient.DeflateRect(1, 1);

	CRect rcBackground(rcClient.left, rcClient.top, rcClient.right, rcClient.top + 12);
	XTPDrawHelpers()->GradientFill(pDC, rcBackground, RGB(207, 215, 236), RGB(255, 255, 255) , FALSE);

	rcBackground = CRect(rcClient.left, rcBackground.bottom, rcClient.right, rcClient.top + 23);
	XTPDrawHelpers()->GradientFill(pDC, rcBackground, RGB(255, 255, 255), RGB(207, 221, 244), FALSE);

	rcBackground = CRect(rcClient.left, rcBackground.bottom, rcClient.right, rcClient.top + 40);
	XTPDrawHelpers()->GradientFill(pDC, rcBackground, RGB(207, 221, 244), RGB(255, 255, 255), FALSE);

	rcBackground = CRect(rcClient.left, rcBackground.bottom, rcClient.right, rcClient.bottom);
	XTPDrawHelpers()->GradientFill(pDC, rcBackground, RGB(255, 255, 255), RGB(207, 221, 244), FALSE);

	CRect rcFrame(rcClient.left, rcClient.top + 22, rcClient.right, rcClient.bottom);
	pDC->Draw3dRect(rcFrame, RGB(114, 142, 184), RGB(185, 201, 239));
}
