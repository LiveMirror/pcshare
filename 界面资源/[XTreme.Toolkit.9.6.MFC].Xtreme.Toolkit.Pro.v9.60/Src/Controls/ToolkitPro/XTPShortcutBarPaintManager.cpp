// XTPShortcutBarPaintManager.cpp : implementation of the CXTPShortcutBarPaintManager class.
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
#include "XTResource.h"
#include "XTPVC80Helpers.h"
#include "XTPResource.h"
#include "XTPDrawHelpers.h"
#include "XTPImageManager.h"

#include "XTPPaintManager.h"
#include "XTPShortcutBarPaintManager.h"
#include "XTPShortcutBar.h"
#include "XTPShortcutBarPane.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CXTPShortcutBarPaintManager::CXTPShortcutBarPaintManager()
{
	RefreshMetrics();

	if (XTPImageManager()->IsAlphaIconsImageListSupported())
	{
		m_hExpandIcon =  (HICON)LoadImage(AfxFindResourceHandle(MAKEINTRESOURCE(XTP_IDI_SHORTCUTS_EXPAND_ALPHA),
			RT_GROUP_ICON), MAKEINTRESOURCE(XTP_IDI_SHORTCUTS_EXPAND_ALPHA), IMAGE_ICON, 16, 16, 0);
	} else
	{
		m_hExpandIcon =  (HICON)LoadImage(AfxFindResourceHandle(MAKEINTRESOURCE(XTP_IDI_SHORTCUTS_EXPAND_NORMAL),
			RT_GROUP_ICON), MAKEINTRESOURCE(XTP_IDI_SHORTCUTS_EXPAND_NORMAL), IMAGE_ICON, 16, 16, 0);
	}

	m_nNormalIconWidth = 16;
	m_nExpandedIconWidth = 24;
}

CXTPShortcutBarPaintManager::~CXTPShortcutBarPaintManager()
{
	if (m_hExpandIcon)
		DestroyIcon(m_hExpandIcon);
}

void CXTPShortcutBarPaintManager::RefreshMetrics()
{
	XTPColorManager()->RefreshColors();

	m_fntText.DeleteObject();
	m_fntPaneCaption.DeleteObject();
	m_fntPaneItemCaption.DeleteObject();

	LOGFONT lf = { sizeof(LOGFONT)};

	VERIFY(::SystemParametersInfo(SPI_GETICONTITLELOGFONT,
		sizeof(LOGFONT), &lf, 0));

	m_fntPaneItemCaption.CreateFontIndirect(&lf);

	lf.lfWeight = FW_BOLD;

	m_fntText.CreateFontIndirect(&lf);

	CString strFaceName(_T("Arial"));
	STRCPY_S(lf.lfFaceName, LF_FACESIZE, strFaceName);
	lf.lfWeight = FW_BOLD;
	lf.lfHeight = 20;
	m_fntPaneCaption.CreateFontIndirect(&lf);

	CWindowDC dc(CWnd::GetDesktopWindow());
	CXTPFontDC font(&dc, &m_fntPaneItemCaption);
	m_nPaneItemHeight = max(19, dc.GetTextExtent(_T(" ")).cy + 4);

	m_clrPushedText = GetXtremeColor(XPCOLOR_PUSHED_TEXT);


}

void CXTPShortcutBarPaintManager::DrawExpandButton(CDC* pDC, CRect rc)
{


	CPoint ptImage (rc.CenterPoint().x - 8, rc.CenterPoint().y - 8);
		pDC->DrawState(ptImage, CSize(16, 16), m_hExpandIcon, DST_ICON, (CBrush*)NULL );
}


void CXTPShortcutBarPaintManager::DrawShortcutItem(CDC* pDC, CXTPShortcutBarItem* pItem)
{

	CRect rc = pItem->GetItemRect();


	CXTPShortcutBar* pBar = pItem->GetShortcutBar();

	pDC->SetTextColor(GetXtremeColor(COLOR_BTNTEXT));
	FillShortcutItemEntry(pDC, pItem);

	CSize szIcon(pItem->IsExpanded()? m_nExpandedIconWidth: m_nNormalIconWidth, 0);
	CXTPImageManagerIcon* pImage =  pItem->GetImage(szIcon.cx);

	BOOL bSelected = pBar->GetHotItem() == pItem && (pBar->GetPressedItem() == NULL || pBar->GetPressedItem() == pItem),
		bPressed = pBar->GetPressedItem() == pItem, bChecked = pBar->GetSelectedItem() == pItem;


	if (pItem->IsExpanded())
	{
		CRect rcText(rc);
		rcText.left = 5;

		if (pImage)
		{
			CPoint ptImage (5, rc.CenterPoint().y - m_nExpandedIconWidth / 2);
			DrawImage(pDC, ptImage, szIcon, pImage, bSelected, bPressed, bChecked);

			rcText.left = 5 + m_nExpandedIconWidth + 5;

		}

		pDC->SetBkMode(TRANSPARENT);

		CXTPFontDC font(pDC, &m_fntText);
		pDC->DrawText(pItem->GetCaption(), rcText, DT_VCENTER|DT_LEFT|DT_SINGLELINE|DT_END_ELLIPSIS);

	}
	else
	{
		if (pItem->IsItemExpandButton())
		{
			DrawExpandButton(pDC, rc);

		}

		else if (pImage)
		{
			CPoint ptImage (rc.CenterPoint().x - m_nNormalIconWidth / 2, rc.CenterPoint().y - m_nNormalIconWidth / 2);
			DrawImage(pDC, ptImage, szIcon, pImage, bSelected, bPressed, bChecked);
		}
	}
}

int CXTPShortcutBarPaintManager::DrawPaneCaption(CDC* pDC, CXTPShortcutBarPane* pPane, BOOL bDraw)
{
	if (bDraw)
	{

		CXTPFontDC font(pDC, &m_fntPaneCaption);

		CXTPClientRect rc(pPane);
		rc.bottom = rc.top + 24;
		pDC->FillSolidRect(rc, GetXtremeColor(COLOR_3DSHADOW));

		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(GetXtremeColor(COLOR_WINDOW));

		CRect rcText(rc);
		rcText.left = 4;
		pDC->DrawText(pPane->GetCaption(), rcText, DT_LEFT|DT_SINGLELINE|DT_VCENTER|DT_END_ELLIPSIS);
	}
	return 24;
}

int CXTPShortcutBarPaintManager::DrawPaneItemCaption(CDC* pDC, CXTPShortcutBarPaneItem* pItem, BOOL bDraw)
{
	if (bDraw)
	{
		CXTPFontDC font(pDC, &m_fntPaneItemCaption);

		CRect rc = pItem->GetCaptionRect();
		pDC->FillSolidRect(rc, GetXtremeColor(COLOR_3DFACE));

		pDC->FillSolidRect(0, rc.bottom - 1, rc.Width(), 1, GetXtremeColor(COLOR_3DSHADOW));

		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(GetXtremeColor(COLOR_BTNTEXT));

		CRect rcText(rc);
		rcText.left = 7;
		pDC->DrawText(pItem->GetCaption(), rcText, DT_LEFT|DT_SINGLELINE|DT_VCENTER|DT_END_ELLIPSIS);

	}

	return m_nPaneItemHeight;
}

void CXTPShortcutBarPaintManager::Rectangle(CDC* pDC, CRect rc, int nPen, int nBrush)
{
	pDC->FillSolidRect(rc, GetXtremeColor(nBrush));
	pDC->Draw3dRect(rc, GetXtremeColor(nPen), GetXtremeColor(nPen));
}
void CXTPShortcutBarPaintManager::Line(CDC* pDC, int x0, int y0, int x1, int y1, int nPen)
{
	CXTPPenDC pen(*pDC, GetXtremeColor(nPen));
	pDC->MoveTo(x0, y0);
	pDC->LineTo(x1, y1);
}


//////////////////////////////////////////////////////////////////////////
// CXTPShortcutBarOffice2003Theme

CXTPShortcutBarOffice2003Theme::CXTPShortcutBarOffice2003Theme()
{
	RefreshMetrics();
}

void CXTPShortcutBarOffice2003Theme::RefreshMetrics()
{
	CXTPShortcutBarPaintManager::RefreshMetrics();



	m_grcSelected.SetStandardValue(GetXtremeColor(XPCOLOR_HIGHLIGHT_CHECKED));
	m_grcHot.SetStandardValue(GetXtremeColor(XPCOLOR_HIGHLIGHT));
	m_grcPushed.SetStandardValue(GetXtremeColor(XPCOLOR_HIGHLIGHT_PUSHED));
	m_grcShortcutItem.SetStandardValue(GetXtremeColor(COLOR_WINDOW), GetXtremeColor(COLOR_3DFACE));

	if (!XTPColorManager()->IsLunaColorsDisabled())
	{

		XTPCurrentSystemTheme systemTheme = XTPDrawHelpers()->GetCurrentSystemTheme();

		switch (systemTheme)
		{
		case xtpSystemThemeBlue:
			m_grcShortcutItem.SetStandardValue(RGB(203, 225, 252), RGB(125, 165, 224));
			break;

		case xtpSystemThemeOlive:
			m_grcShortcutItem.SetStandardValue(RGB(234, 240, 207), RGB(177, 192, 140));
			break;

		case xtpSystemThemeSilver:
			m_grcShortcutItem.SetStandardValue(RGB(225, 226, 236), RGB(149, 147, 177));
			break;
		}

		if (systemTheme != xtpSystemThemeUnknown)
		{
			m_grcSelected.SetStandardValue(RGB(251, 230, 148), RGB(238, 149, 21));
			m_grcPushed.SetStandardValue(RGB(232, 127, 8), RGB(247, 218, 124));
			m_grcHot.SetStandardValue(RGB(255, 255, 220), RGB(247, 192, 91));

			m_clrPushedText = 0;
		}
	}
}


int CXTPShortcutBarOffice2003Theme::DrawPaneCaption(CDC* pDC, CXTPShortcutBarPane* pPane, BOOL bDraw)
{
	if (bDraw)
	{
		CXTPFontDC font(pDC, &m_fntPaneCaption);

		CXTPClientRect rc(pPane);
		rc.bottom = rc.top + 24;
		XTPDrawHelpers()->GradientFill(pDC, rc, XTPColorManager()->grcCaption, FALSE);

		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(GetXtremeColor(COLOR_WINDOW));

		CRect rcText(rc);
		rcText.left = 4;
		pDC->DrawText(pPane->GetCaption(), rcText, DT_LEFT|DT_SINGLELINE|DT_VCENTER|DT_END_ELLIPSIS);
	}
	return 24;
}

int CXTPShortcutBarOffice2003Theme::DrawPaneItemCaption(CDC* pDC, CXTPShortcutBarPaneItem* pItem, BOOL bDraw)
{
	if (bDraw)
	{
		CXTPFontDC font(pDC, &m_fntPaneItemCaption);

		CRect rc = pItem->GetCaptionRect();
		XTPDrawHelpers()->GradientFill(pDC, rc, XTPColorManager()->grcDockBar.clrDark, XTPColorManager()->grcDockBar.clrLight, FALSE);

		pDC->FillSolidRect(0, rc.bottom - 1, rc.Width(), 1, GetXtremeColor(XPCOLOR_FRAME));
		pDC->FillSolidRect(0, rc.top, rc.Width(), 1, XTPColorManager()->grcDockBar.clrLight);

		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(GetXtremeColor(COLOR_BTNTEXT));

		CRect rcText(rc);
		rcText.left = 7;
		pDC->DrawText(pItem->GetCaption(), rcText, DT_LEFT|DT_SINGLELINE|DT_VCENTER|DT_END_ELLIPSIS);

	}

	return m_nPaneItemHeight;
}


void CXTPShortcutBarOffice2003Theme::FillShortcutBar(CDC* pDC, CXTPShortcutBar* pShortcutBar)
{
	CXTPClientRect rc(pShortcutBar);

	int nItemHeight = pShortcutBar->GetItemSize().cy;

	if (pShortcutBar->IsClientPaneVisible())
	{
		pDC->FillSolidRect(rc, GetXtremeColor(COLOR_WINDOW));


		for (int i = 0; i < pShortcutBar->GetExpandedLinesCount(); i++)
		{
			CRect rcLine(0, rc.Height() - (i + 1) * nItemHeight, rc.Width(), rc.Height() - i * nItemHeight);

			XTPDrawHelpers()->GradientFill(pDC, rcLine, m_grcShortcutItem, FALSE);
			pDC->FillSolidRect(0, rcLine.top, rc.Width(), 1, GetXtremeColor(XPCOLOR_FRAME));
		}
	}
	else
	{
		XTPDrawHelpers()->GradientFill(pDC, rc, m_grcShortcutItem, FALSE);

		for (int i = pShortcutBar->GetExpandedLinesCount() - 2; i >= 0; i--)
		{
			pDC->FillSolidRect(0, (i + 1)* nItemHeight - 1, rc.Width(), 1, GetXtremeColor(XPCOLOR_FRAME));
		}
	}

}

void CXTPShortcutBarOffice2003Theme::DrawShortcutBarFrame(CDC* pDC, CXTPShortcutBar* pShortcutBar)
{
	CXTPWindowRect rc(pShortcutBar);
	rc.OffsetRect(-rc.TopLeft());
	pDC->Draw3dRect(rc, GetXtremeColor(XPCOLOR_FRAME), GetXtremeColor(XPCOLOR_FRAME));
}

void CXTPShortcutBarOffice2003Theme::FillShortcutItemEntry(CDC* pDC, CXTPShortcutBarItem* pItem)
{
	CXTPShortcutBar* pShortcutBar = pItem->GetShortcutBar();
	CRect rc = pItem->GetItemRect();
	rc.top++;

	if (pShortcutBar->GetSelectedItem() == pItem &&
		(pShortcutBar->GetHotItem() != pItem || (pShortcutBar->GetPressedItem() != NULL && pShortcutBar->GetPressedItem() != pItem)))
	{
		XTPDrawHelpers()->GradientFill(pDC, rc, m_grcSelected, FALSE);
		pDC->SetTextColor(GetXtremeColor(XPCOLOR_CHECKED_TEXT));

	}
	else if (pShortcutBar->GetSelectedItem() == pItem ||
		(pShortcutBar->GetHotItem() == pItem && pShortcutBar->GetPressedItem() == pItem))
	{
		XTPDrawHelpers()->GradientFill(pDC, rc, m_grcPushed, FALSE);
		pDC->SetTextColor(m_clrPushedText);
	}
	else if ((pShortcutBar->GetHotItem() == pItem && pShortcutBar->GetPressedItem() == NULL) ||
		pShortcutBar->GetPressedItem() == pItem)
	{
		XTPDrawHelpers()->GradientFill(pDC, rc, m_grcHot, FALSE);
		pDC->SetTextColor(GetXtremeColor(XPCOLOR_HIGHLIGHT_TEXT));
	}


}


int CXTPShortcutBarOffice2003Theme::DrawGripper(CDC* pDC, CXTPShortcutBar* pShortcutBar, BOOL bDraw)
{
	if (bDraw)
	{
		CRect rc = pShortcutBar->GetGripperRect();
		XTPDrawHelpers()->GradientFill(pDC, rc, XTPColorManager()->grcShortcutBarGripper, FALSE);

		int nRight = max (2, (rc.Width() - 4 * 9) / 2);
		for (int i = 0; i <9; i++)
		{
			pDC->FillSolidRect(nRight, rc.top + 1, 2, 2, RGB(40, 50, 71));
			pDC->FillSolidRect(nRight + 1, rc.top + 2, 2, 2, RGB(249, 249, 251));
			pDC->FillSolidRect(nRight + 1, rc.top + 2, 1, 1, RGB(97, 116, 152));

			nRight += 4;
		}
	}
	return 6;
}



//////////////////////////////////////////////////////////////////////////
// CXTPShortcutBarOfficeXPTheme

void CXTPShortcutBarOfficeXPTheme::FillShortcutBar(CDC* pDC, CXTPShortcutBar* pShortcutBar)
{
	CXTPClientRect rc(pShortcutBar);

	pDC->FillSolidRect(rc, GetXtremeColor(XPCOLOR_TOOLBAR_FACE));

}

void CXTPShortcutBarOfficeXPTheme::DrawShortcutBarFrame(CDC* pDC, CXTPShortcutBar* pShortcutBar)
{
	CXTPWindowRect rc(pShortcutBar);
	rc.OffsetRect(-rc.TopLeft());
	pDC->Draw3dRect(rc, GetXtremeColor(COLOR_3DSHADOW), GetXtremeColor(COLOR_3DHIGHLIGHT));
}

void CXTPShortcutBarOfficeXPTheme::FillShortcutItemEntry(CDC* pDC, CXTPShortcutBarItem* pItem)
{
	CXTPShortcutBar* pShortcutBar = pItem->GetShortcutBar();
	CRect rc = pItem->GetItemRect();
	rc.top++;

	if (pShortcutBar->GetSelectedItem() == pItem &&
		(pShortcutBar->GetHotItem() != pItem || (pShortcutBar->GetPressedItem() != NULL && pShortcutBar->GetPressedItem() != pItem)))
	{
		Rectangle(pDC, rc, XPCOLOR_HIGHLIGHT_PUSHED_BORDER, XPCOLOR_HIGHLIGHT_CHECKED);
		pDC->SetTextColor(GetXtremeColor(XPCOLOR_CHECKED_TEXT));
	}
	else if (pShortcutBar->GetSelectedItem() == pItem ||
		(pShortcutBar->GetHotItem() == pItem && pShortcutBar->GetPressedItem() == pItem))
	{
		Rectangle(pDC, rc, XPCOLOR_HIGHLIGHT_PUSHED_BORDER, XPCOLOR_HIGHLIGHT_PUSHED);
		pDC->SetTextColor(m_clrPushedText);
	}
	else if ((pShortcutBar->GetHotItem() == pItem && pShortcutBar->GetPressedItem() == NULL) ||
		pShortcutBar->GetPressedItem() == pItem)
	{
		Rectangle(pDC, rc, XPCOLOR_HIGHLIGHT_BORDER, XPCOLOR_HIGHLIGHT);
		pDC->SetTextColor(GetXtremeColor(XPCOLOR_HIGHLIGHT_TEXT));
	}
}


int CXTPShortcutBarOfficeXPTheme::DrawGripper(CDC* pDC, CXTPShortcutBar* pShortcutBar, BOOL bDraw)
{
	if (bDraw)
	{
		CRect rc = pShortcutBar->GetGripperRect();

		pDC->FillSolidRect(rc, GetXtremeColor(COLOR_3DFACE));


		pDC->FillSolidRect(rc.left, rc.top, rc.Width(), 1, GetXtremeColor(COLOR_3DHIGHLIGHT));
		pDC->FillSolidRect(rc.left, rc.bottom, rc.Width(), 1, GetXtremeColor(COLOR_3DSHADOW));


		Line(pDC, rc.CenterPoint().x - 16, rc.top + 3, rc.CenterPoint().x + 16, rc.top + 3, XPCOLOR_TOOLBAR_GRIPPER);
		Line(pDC, rc.CenterPoint().x - 16, rc.top + 5, rc.CenterPoint().x + 16, rc.top + 5, XPCOLOR_TOOLBAR_GRIPPER);


	}
	return 7;
}


//////////////////////////////////////////////////////////////////////////
// CXTPShortcutBarOffice2000Theme

void CXTPShortcutBarOffice2000Theme::FillShortcutBar(CDC* pDC, CXTPShortcutBar* pShortcutBar)
{
	CXTPClientRect rc(pShortcutBar);

	pDC->FillSolidRect(rc, GetXtremeColor(COLOR_3DSHADOW));

}

void CXTPShortcutBarOffice2000Theme::DrawShortcutBarFrame(CDC* pDC, CXTPShortcutBar* pShortcutBar)
{
	CXTPWindowRect rc(pShortcutBar);
	rc.OffsetRect(-rc.TopLeft());
	pDC->Draw3dRect(rc, GetXtremeColor(COLOR_3DSHADOW), GetXtremeColor(COLOR_3DHIGHLIGHT));
}


void CXTPShortcutBarOffice2000Theme::FillShortcutItemEntry(CDC* pDC, CXTPShortcutBarItem* pItem)
{
	CXTPShortcutBar* pShortcutBar = pItem->GetShortcutBar();
	CRect rc = pItem->GetItemRect();
	rc.top++;

	if (pShortcutBar->GetSelectedItem() == pItem &&
		(pShortcutBar->GetHotItem() != pItem || (pShortcutBar->GetPressedItem() != NULL && pShortcutBar->GetPressedItem() != pItem)))
	{
		pDC->Draw3dRect(rc, GetXtremeColor(COLOR_BTNTEXT), GetXtremeColor(COLOR_3DHIGHLIGHT));
	}
	else if (pShortcutBar->GetSelectedItem() == pItem ||
		(pShortcutBar->GetHotItem() == pItem && pShortcutBar->GetPressedItem() == pItem))
	{
		pDC->Draw3dRect(rc, GetXtremeColor(COLOR_BTNTEXT), GetXtremeColor(COLOR_3DHIGHLIGHT));
	}
	else if ((pShortcutBar->GetHotItem() == pItem && pShortcutBar->GetPressedItem() == NULL) ||
		pShortcutBar->GetPressedItem() == pItem)
	{
		pDC->Draw3dRect(rc, GetXtremeColor(COLOR_3DHIGHLIGHT), GetXtremeColor(COLOR_BTNTEXT));
	}

	pDC->SetTextColor(GetXtremeColor(COLOR_3DHIGHLIGHT));
}


int CXTPShortcutBarOffice2000Theme::DrawGripper(CDC* pDC, CXTPShortcutBar* pShortcutBar, BOOL bDraw)
{
	if (bDraw)
	{
		CRect rc = pShortcutBar->GetGripperRect();
		pDC->FillSolidRect(rc, GetXtremeColor(COLOR_3DFACE));
	}
	return 4;
}



void CXTPShortcutBarOffice2000Theme::DrawImage(CDC* pDC, CPoint pt, CSize sz, CXTPImageManagerIcon* pImage, BOOL bSelected, BOOL bPressed, BOOL bChecked)
{

	XTPPaintManager()->DrawImage(pDC, pt, sz, pImage, bSelected, bPressed, TRUE, bChecked);
}


void CXTPShortcutBarOfficeXPTheme::DrawImage(CDC* pDC, CPoint pt, CSize sz, CXTPImageManagerIcon* pImage, BOOL bSelected, BOOL bPressed, BOOL bChecked)
{

	XTPPaintManager()->DrawImage(pDC, pt, sz, pImage, bSelected, bPressed, TRUE, bChecked);
}

void CXTPShortcutBarOffice2003Theme::DrawImage(CDC* pDC, CPoint pt, CSize sz, CXTPImageManagerIcon* pImage, BOOL bSelected, BOOL bPressed, BOOL bChecked)
{

	XTPPaintManager()->DrawImage(pDC, pt, sz, pImage, bSelected, bPressed, TRUE, bChecked);
}
