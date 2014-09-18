// XTPDefaultTheme.cpp : implementation of the CXTPDefaultTheme class.
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
#include "XTPDrawHelpers.h"

#include "XTPPaintManager.h"
#include "XTPControlButton.h"
#include "XTPControlPopup.h"
#include "XTPColorManager.h"
#include "XTPToolBar.h"
#include "XTPPopupBar.h"
#include "XTPCommandBars.h"
#include "XTPControlComboBox.h"
#include "XTPControlEdit.h"
#include "XTPControls.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace XTPPaintThemes;

// CXTPDefaultTheme

CXTPDefaultTheme::CXTPDefaultTheme()
{
	m_iconsInfo.bUseFadedIcons = FALSE;
	m_iconsInfo.bIconsWithShadow = FALSE;

	m_nPopupBarText = COLOR_BTNTEXT;
}

void CXTPDefaultTheme::DrawRectangle(CDC* pDC, CRect rc, BOOL bSelected, BOOL bPressed, BOOL bEnabled, BOOL bChecked, BOOL bPopuped, BOOL bToolBar, XTPBarPosition /*barPosition = xtpBarPopup*/)
{
	if (!bEnabled)
	{
		if (bChecked && !bToolBar) Draw3dRect(pDC, rc, COLOR_3DSHADOW, COLOR_BTNHILIGHT);
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

void CXTPDefaultTheme::DrawImage(CDC* pDC, CPoint pt, CSize sz, CXTPImageManagerIcon* pImage, BOOL bSelected, BOOL bPressed, BOOL bEnabled, BOOL bChecked, BOOL bPopuped, BOOL  )
{
	if (!bEnabled)
	{
		if (!m_iconsInfo.bUseDisabledIcons && !pImage->IsAlpha())
		{
		#ifdef _EMBOSSED_STYLE
			pImage->DrawMono(pDC, pt + CPoint(1,1), pImage->GetIcon(), GetXtremeColor(COLOR_3DHILIGHT));
		#endif
			pImage->DrawMono(pDC, pt, pImage->GetIcon(), GetXtremeColor(COLOR_3DSHADOW));
		}
		else pImage->Draw(pDC, pt, pImage->GetDisabledIcon(), sz);
	}

	else if (bPopuped || bChecked) pImage->Draw(pDC, pt, bChecked && (bSelected || bPressed)? pImage->GetHotIcon(): bChecked? pImage->GetCheckedIcon(): pImage->GetIcon(), sz);
	else if (!bSelected && !bPressed)
	{
		pImage->Draw(pDC, pt, m_iconsInfo.bUseFadedIcons? pImage->GetFadedIcon(): pImage->GetIcon(), sz);
	}
	else if ((bSelected && !bPressed) || (!bSelected && bPressed == TRUE))
	{
		if (m_iconsInfo.bIconsWithShadow)
		{
			pImage->Draw(pDC, CPoint(pt.x + 1, pt.y + 1), pImage->GetShadowIcon(), sz);
			pImage->Draw(pDC, CPoint(pt.x - 1, pt.y - 1), pImage->GetHotIcon(), sz);
		} else
		{
			pImage->Draw(pDC, pt, pImage->GetHotIcon(), sz);

		}
	}
	else if ((bSelected && bPressed) || (bPressed == TRUE_KEYBOARD))
		pImage->Draw(pDC, CPoint(pt.x + 1, pt.y + 1), pImage->GetHotIcon(), sz);
}


CSize CXTPDefaultTheme::DrawControlEdit(CDC* pDC, CXTPControlEdit* pControlEdit, BOOL bDraw)
{

	BOOL bPopupBar = pControlEdit->GetParent()->GetType() == xtpBarTypePopup;
	int nImageWidth = max(pControlEdit->GetParent()->GetIconSize().cx + 4, m_nTextHeight);

	if (!bDraw)
	{
		CString strCaption(pControlEdit->GetCaption());
		StripMnemonics(strCaption);
		
		CXTPFontDC font(pDC, &m_fontIcon);
		int nHeight = pDC->GetTextExtent(_T(" ")).cy + 1;

		if (bPopupBar)
		{
			pControlEdit->SetLabelWidth(pDC->GetTextExtent(strCaption).cx + nImageWidth + 9);
			return CSize(pControlEdit->GetWidth() + pControlEdit->GetLabelWidth(), nHeight + 7);
		}

		pControlEdit->SetLabelWidth(pControlEdit->IsLabeled() ?
			pDC->GetTextExtent(strCaption).cx + 5: 0);

		return CSize(pControlEdit->GetWidth(), nHeight + 7);
	}
	else
	{
		pDC->SetBkMode (TRANSPARENT);
		CRect rc = pControlEdit->GetRect();
		BOOL bEnabled = pControlEdit->GetEnabled();

		if (bPopupBar ||pControlEdit->IsLabeled())
		{
			CXTPFontDC font(pDC, &m_fontIcon);

			CRect rcText(rc); rcText.left += bPopupBar? nImageWidth  + 4: 2;
			pDC->SetTextColor(GetXtremeColor( !bEnabled? COLOR_GRAYTEXT: COLOR_BTNTEXT));
			DrawText(pDC, pControlEdit->GetCaption(), rcText, 0, bDraw, FALSE, bEnabled);
			rc.left += pControlEdit->GetLabelWidth();
		}

		BOOL bSelected = pControlEdit->GetSelected();

		rc.DeflateRect(1, 0, 0, 0);

		if (bEnabled)
			pDC->FillSolidRect(rc, GetXtremeColor(COLOR_WINDOW));

		DrawControlEditFrame(pDC, rc, bEnabled, bSelected);

	}
	return 0;
}

void CXTPDefaultTheme::DrawControlEditFrame(CDC* pDC, CRect rc, BOOL bEnabled, BOOL bSelected)
{
	if (!bEnabled)
	{
	#ifdef _EMBOSSED_STYLE
		Draw3dRect(pDC, rc, bSelected == TRUE_KEYBOARD? COLOR_3DSHADOW: COLOR_3DHIGHLIGHT, bSelected == TRUE_KEYBOARD? COLOR_3DSHADOW: COLOR_3DHIGHLIGHT);
	#else
		Rectangle (pDC, rc, bSelected == TRUE_KEYBOARD? COLOR_3DSHADOW: XPCOLOR_DISABLED, COLOR_3DFACE);
	#endif
	}
	else
	{
		Draw3dRect(pDC, rc, COLOR_3DSHADOW, COLOR_BTNHILIGHT);
	}
}
void CXTPDefaultTheme::DrawControlComboBoxButton(CDC* pDC, CRect rcBtn, BOOL bEnabled, BOOL /*bSelected*/, BOOL bDropped)
{
	if (bEnabled)
	{
		rcBtn.DeflateRect(-1, 1, 1, 1);
		Rectangle (pDC, rcBtn, COLOR_3DFACE, COLOR_3DFACE);

		if (bDropped)
		{
			Draw3dRect(pDC, rcBtn, COLOR_3DSHADOW, COLOR_3DSHADOW);

		} else
		{
			Draw3dRect(pDC, rcBtn, COLOR_3DFACE, 0);
			rcBtn.DeflateRect(1, 1);
			Draw3dRect(pDC, rcBtn, COLOR_BTNHILIGHT, COLOR_3DSHADOW);
		}

	}

	rcBtn.left++; rcBtn.top++;
	CXTPFontDC fontMarlett(pDC, &m_fontMarlettIcon);
#ifdef _EMBOSSED_STYLE
	pDC->SetTextColor(GetXtremeColor(COLOR_BTNTEXT));
#else
	pDC->SetTextColor(GetXtremeColor(!bEnabled? XPCOLOR_DISABLED: COLOR_BTNTEXT));
#endif

	pDC->DrawText(_T("u"), rcBtn, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
}

CSize CXTPDefaultTheme::DrawControlComboBox(CDC* pDC, CXTPControlComboBox* pControlCombo, BOOL bDraw)
{

	BOOL bPopupBar = pControlCombo->GetParent()->GetType() == xtpBarTypePopup;
	int nImageWidth = max(pControlCombo->GetParent()->GetIconSize().cx + 4, m_nTextHeight);


	if (!bDraw)
	{
		CString strCaption(pControlCombo->GetCaption());
		StripMnemonics(strCaption);

		CXTPFontDC font(pDC, &m_fontIcon);
		int nHeight = pDC->GetTextExtent(_T(" ")).cy + 1;

		pControlCombo->SetThumbWidth(pDC->GetTextExtent(_T(" ")).cy + 2);

		if (bPopupBar)
		{
			pControlCombo->SetLabelWidth(pDC->GetTextExtent(strCaption).cx + nImageWidth + 9);
			return CSize(pControlCombo->GetWidth() + pControlCombo->GetLabelWidth(), nHeight + 7);
		}

		pControlCombo->SetLabelWidth(pControlCombo->GetStyle() == xtpComboLabel ?
			pDC->GetTextExtent(strCaption).cx + 5: 0);

		return CSize(pControlCombo->GetWidth(), nHeight + 7);
	}
	else
	{
		pDC->SetBkMode (TRANSPARENT);
		CRect rc = pControlCombo->GetRect();
		BOOL bEnabled = pControlCombo->GetEnabled();

		if (bPopupBar ||pControlCombo->GetStyle() == xtpComboLabel)
		{
			CXTPFontDC font(pDC, &m_fontIcon);

			CRect rcText(rc); rcText.left += bPopupBar? nImageWidth + 4: 2;
			pDC->SetTextColor(GetXtremeColor( !bEnabled? COLOR_GRAYTEXT: COLOR_BTNTEXT));
			DrawText(pDC, pControlCombo->GetCaption(), rcText, 0, bDraw, FALSE, bEnabled);
			rc.left += pControlCombo->GetLabelWidth();
		}

		int nThumb = pControlCombo->GetThumbWidth();
		rc.DeflateRect(1, 0, 0, 0);
		CRect rcBtn (rc.right - nThumb, rc.top, rc.right, rc.bottom);

		BOOL bDropped = pControlCombo->GetDroppedState() != 0;
		BOOL bSelected = pControlCombo->GetSelected();

		if (bEnabled)
		{
			Rectangle (pDC, rc, COLOR_WINDOW, COLOR_WINDOW);

			if (!pControlCombo->GetEditCtrl()) // CBS_DROPDOWNLIST
			{
				CRect rcText(rc); rcText.DeflateRect(3, 1, rcBtn.Width() + 2, 1);
				pDC->SetTextColor(GetXtremeColor(COLOR_WINDOWTEXT));
				pDC->DrawText(pControlCombo->GetText(), rcText, DT_SINGLELINE | DT_VCENTER | DT_NOPREFIX);
			}
		}

		DrawControlComboBoxButton(pDC, rcBtn, bEnabled, bSelected, bDropped);
		DrawControlEditFrame(pDC, rc, bEnabled, bSelected);

	}
	return 0;
}

void CXTPDefaultTheme::DrawText(CDC* pDC, CString strText, CRect& rcText, UINT nFlags, BOOL bDraw, BOOL bSelected, BOOL bEnabled)
{
#ifdef _EMBOSSED_STYLE
	if (bDraw && !bEnabled && !bSelected)
	{
		COLORREF clrText = pDC->GetTextColor();
		pDC->SetTextColor(GetXtremeColor(COLOR_3DHIGHLIGHT));
		pDC->DrawText(strText, rcText + CPoint(1, 1), DT_SINGLELINE | DT_VCENTER | nFlags);
		pDC->SetTextColor(clrText);
	}
#endif

	pDC->DrawText(strText, &rcText, DT_SINGLELINE | DT_VCENTER | (!bDraw ? DT_CALCRECT : nFlags) );
}

CSize CXTPDefaultTheme::DrawSpecialControl(CDC* pDC, XTPSpecialControl controlType, CXTPControl* pButton, CXTPCommandBar* pBar, BOOL bDraw, LPVOID lpParam)
{
	if (controlType == xtpButtonExpandFloating || controlType == xtpButtonHideFloating)
	{
		if (pButton->GetPressed() || pButton->GetSelected() || (controlType == xtpButtonExpandFloating && ((CXTPControlPopup*)pButton)->GetPopup()))
			pDC->FillSolidRect(pButton->GetRect(), GetXtremeColor(COLOR_3DFACE));

	}
	if (controlType == xtpButtonCustomize)
	{
		CSize szIcon = pButton->GetParent()->GetIconSize();
		CSize szImage(max(szIcon.cx + 4, m_nTextHeight), max(szIcon.cy + 4, m_nTextHeight));

		if (!bDraw)
		{
			CSize sz = DrawControlPopupParent(pDC, pButton, FALSE);
			return CSize(sz.cx + szImage.cx, sz.cy);
		}
		CRect rcButton = pButton->GetRect();
		CRect rcText(0, 0, 0, 0), rcShortcut(0, 0, 0, 0);
		BOOL bSelected = pButton->GetSelected(), bEnabled = pButton->GetEnabled(), bChecked = pButton->GetChecked();

		CRect rcSelection(rcButton.left, rcButton.top, rcButton.right, rcButton.bottom);
		CRect rcImageRect(rcButton.left, rcButton.top, rcButton.left + szImage.cx, rcButton.bottom);
		CRect rcCheck = rcImageRect;

		COLORREF clrText = GetXtremeColor(!bEnabled? COLOR_GRAYTEXT: bSelected? COLOR_HIGHLIGHTTEXT: m_nPopupBarText);
		pDC->SetTextColor(clrText);

		rcImageRect.OffsetRect(szImage.cx + 1, 0);

		BOOL bLayoutRTL = pButton->GetParent()->IsLayoutRTL();

		CString strCaption = pButton->GetCaption();
		if (bLayoutRTL) UpdateRTLCaption(strCaption);

		if (bChecked)
		{
			DrawRectangle(pDC, rcCheck, bSelected, FALSE, bEnabled, bChecked, FALSE, FALSE);
			DrawCheckMark(pDC, rcCheck, GetXtremeColor(m_nPopupBarText), bLayoutRTL);
			rcSelection.left = rcCheck.right + 1;
		}


		CXTPImageManagerIcon* pImage = pButton->GetImage(szIcon.cx);
		if (pImage)
		{
			szIcon.cy = pImage->GetHeight() * szIcon.cx / pImage->GetWidth();
			CPoint ptImage = CPoint(rcImageRect.left + rcImageRect.Width()/2 - szIcon.cx/2, rcImageRect.top + rcImageRect.Height()/2 - szIcon.cy/2);

			DrawRectangle(pDC, rcImageRect, bSelected, FALSE, bEnabled, FALSE, FALSE, FALSE);
			DrawImage(pDC, ptImage, szIcon, pImage, bSelected, FALSE, bEnabled, FALSE, FALSE, FALSE);

			rcSelection.left = rcImageRect.right + 1;
		}


		pDC->SetBkMode (TRANSPARENT);

		if (bSelected) Rectangle(pDC, rcSelection, COLOR_HIGHLIGHT, COLOR_HIGHLIGHT);

		rcText.SetRect(rcButton.left + 2 * szImage.cx + 4, rcButton.top, rcButton.right, rcButton.bottom);
		rcShortcut.SetRect(rcButton.left + 2 * szImage.cx + 8, rcButton.top, rcButton.right - 19, rcButton.bottom);

		DrawText(pDC, strCaption, rcText, DT_END_ELLIPSIS, bDraw, bSelected, bEnabled);

		if (pButton->GetShortcutText().GetLength() != 0 )
			DrawText(pDC, pButton->GetShortcutText(), rcShortcut, DT_RIGHT, bDraw, bSelected, bEnabled);

		return 0;
	}

	return CXTPPaintManager::DrawSpecialControl(pDC, controlType, pButton, pBar, bDraw, lpParam);
}


CSize CXTPDefaultTheme::DrawControlPopupParent(CDC* pDC, CXTPControl* pButton, BOOL bDraw)
{
	CRect rcButton = pButton->GetRect();
	XTPControlType controlType = pButton->GetType();
	XTPButtonStyle buttonStyle = pButton->GetStyle();
	CRect rcText(0, 0, 0, 0), rcShortcut(0, 0, 0, 0);
	BOOL bDrawImage = (buttonStyle == xtpButtonAutomatic && controlType != xtpControlPopup) ||
		(buttonStyle == xtpButtonIcon) || (buttonStyle == xtpButtonIconAndCaption);

	BOOL bSelected = pButton->GetSelected(), bEnabled = pButton->GetEnabled(), bChecked = pButton->GetChecked(),
		bPopuped = IsPopupControlType(controlType) && ((CXTPControlPopup*)pButton)->GetPopup();

	BOOL bLayoutRTL = pButton->GetParent()->IsLayoutRTL();

	CString strCaption = pButton->GetCaption();
	if (bLayoutRTL) UpdateRTLCaption(strCaption);

	CSize szIcon = pButton->GetParent()->GetIconSize();
	CSize szImage(max(szIcon.cx + 4, m_nTextHeight), max(szIcon.cy + 4, m_nTextHeight));

	if (bDraw)
	{
		CRect rcSelection(rcButton.left, rcButton.top, rcButton.right, rcButton.bottom);
		CRect rcImageRect(rcButton.left, rcButton.top, rcButton.left + szImage.cx, rcButton.bottom);
		CRect rcCheck (CPoint(rcImageRect.left + rcImageRect.Width()/2 - 10, rcImageRect.top + rcImageRect.Height()/2 - 10), CSize(20, 20));

		if (pButton->GetExpanded())
		{
			pDC->FillSolidRect(rcButton, GetXtremeColor(COLOR_3DLIGHT));
		}


		COLORREF clrText = GetXtremeColor(!bEnabled? COLOR_GRAYTEXT: bSelected? COLOR_HIGHLIGHTTEXT: m_nPopupBarText);
		pDC->SetTextColor(clrText);

		CXTPImageManagerIcon* pImage = pButton->GetIconId() != 0 && bDrawImage? pButton->GetImage(szIcon.cx): NULL;
		if (pImage)
		{
			szIcon.cy = pImage->GetHeight() * szIcon.cx / pImage->GetWidth();
			DrawRectangle(pDC, rcImageRect, bSelected, FALSE, bEnabled, bChecked, FALSE, FALSE);
			CPoint ptImage = CPoint(rcCheck.left + rcCheck.Width()/2 - szIcon.cx/2, rcCheck.top + rcCheck.Height()/2 - szIcon.cy/2);
			DrawImage(pDC, ptImage, szIcon, pImage, bSelected, FALSE, bEnabled, bChecked, FALSE, FALSE);
			rcSelection.left = rcImageRect.right + 1;
		}
		else if (bChecked)
		{
			rcSelection.left = rcImageRect.right + 1;
			DrawRectangle(pDC, rcImageRect, bSelected, FALSE, bEnabled, bChecked, FALSE, FALSE);
			DrawCheckMark(pDC, rcCheck, GetXtremeColor(m_nPopupBarText), bLayoutRTL);
		}


		pDC->SetBkMode (TRANSPARENT);

		if (IsPopupControlType(controlType))
		{
			CPoint pt = CPoint(rcButton.right - 11, rcButton.CenterPoint().y);
			if (pButton->GetType() == xtpControlSplitButtonPopup && bEnabled)
			{
				if (!bPopuped || !bSelected)
				{
					if (bSelected) Rectangle(pDC, rcSelection, COLOR_HIGHLIGHT, COLOR_HIGHLIGHT);
					Line(pDC, rcButton.right - 20, rcButton.top + 1, rcButton.right - 20, rcButton.bottom - 1, bSelected? COLOR_3DSHADOW: m_nPopupBarText );
				}
				else
				{
					rcSelection.right -= 20;
					Draw3dRect(pDC, CRect(rcButton.right - 20, rcButton.top, rcButton.right, rcButton.bottom), COLOR_3DSHADOW, COLOR_BTNHILIGHT);
					if (bSelected) Rectangle(pDC, rcSelection, COLOR_HIGHLIGHT, COLOR_HIGHLIGHT);
				}

				Triangle(pDC, CPoint(pt.x, pt.y - 3), CPoint(pt.x + 3, pt.y), CPoint(pt.x, pt.y + 3), !bPopuped || !bSelected? clrText: GetXtremeColor(m_nPopupBarText));
			}
			else
			{
				if (bSelected) Rectangle(pDC, rcSelection, COLOR_HIGHLIGHT, COLOR_HIGHLIGHT);
				Triangle(pDC, CPoint(pt.x, pt.y - 3), CPoint(pt.x + 3, pt.y), CPoint(pt.x, pt.y + 3), clrText);
			}

		} else
		if (bSelected) Rectangle(pDC, rcSelection, COLOR_HIGHLIGHT, COLOR_HIGHLIGHT);


		rcText.SetRect(rcButton.left + szImage.cx + 4, rcButton.top, rcButton.right, rcButton.bottom);
		rcShortcut.SetRect(rcButton.left + szImage.cx + 8, rcButton.top, rcButton.right - 19, rcButton.bottom);

	}

	DrawText(pDC, strCaption, rcText, DT_END_ELLIPSIS, bDraw, bSelected, bEnabled);

	if (controlType == xtpControlButton && pButton->GetShortcutText().GetLength() != 0 )
		DrawText(pDC, pButton->GetShortcutText(), rcShortcut, DT_RIGHT, bDraw, bSelected, bEnabled);

	return CSize(szImage.cx + 8 + rcText.Width() +  rcShortcut.Width() + 32, szImage.cy);
}

void CXTPDefaultTheme::DrawControlEntry(CDC* pDC, CRect rc, BOOL bEnabled, BOOL bSelected, BOOL bPressed, BOOL bChecked, BOOL bPopuped, XTPBarPosition barPosition)
{
	DrawRectangle(pDC, rc, bSelected, bPressed, bEnabled, bChecked, bPopuped, TRUE, barPosition);

	int nColorFace = barPosition == xtpBarPopup? m_nPopupBarText: COLOR_BTNTEXT;

	pDC->SetTextColor(GetXtremeColor(!bEnabled? COLOR_GRAYTEXT: nColorFace));
	pDC->SetBkMode (TRANSPARENT);

}

void CXTPDefaultTheme::DrawShadedRect(CDC *pDC, CRect& rect)
{
	WORD bits[] = {
		0x0055, 0x00AA, 0x0055, 0x00AA,
		0x0055, 0x00AA, 0x0055, 0x00AA
	};

	CBitmap bitmap;
	bitmap.CreateBitmap( 8, 8, 1, 1, &bits );

	CBrush brush;
	brush.CreatePatternBrush( &bitmap );
	CBrush* pOldBrush = pDC->SelectObject(&brush);

	COLORREF crBack = pDC->SetBkColor(GetXtremeColor(COLOR_3DFACE));
	COLORREF crText = pDC->SetTextColor(GetXtremeColor(COLOR_3DHIGHLIGHT));

	pDC->PatBlt(rect.left, rect.top, rect.Width(), rect.Height(), PATCOPY);

	pDC->SelectObject(pOldBrush);
	pDC->SetBkColor(crBack);
	pDC->SetTextColor(crText);

	brush.DeleteObject();
}


void CXTPDefaultTheme::DrawSplitButtonFrame(CDC* pDC, CXTPControl* pButton, CRect rcButton)
{
	BOOL bControlPopup = DYNAMIC_DOWNCAST(CXTPControlPopup, pButton) != NULL;

	BOOL bPressed = pButton->GetPressed(), bSelected = pButton->GetSelected(), bEnabled = pButton->GetEnabled(), bChecked = pButton->GetChecked(),
		bPopuped = bControlPopup && ((CXTPControlPopup*)pButton)->GetPopup();

	DrawControlEntry(pDC, CRect(rcButton.left, rcButton.top, rcButton.right - 12, rcButton.bottom), bEnabled, bSelected || bPopuped, bPressed, bChecked, FALSE, pButton->GetParent()->GetPosition());

	CPoint pt = CPoint(rcButton.right - 7, rcButton.CenterPoint().y);

	if ((bSelected || bPressed || bPopuped) && bEnabled)
		DrawControlEntry(pDC, CRect(rcButton.right - 12, rcButton.top, rcButton.right, rcButton.bottom), bEnabled, TRUE, FALSE, FALSE, bPopuped, pButton->GetParent()->GetPosition());

	Triangle(pDC, CPoint(pt.x - 2 , pt.y - 1), CPoint(pt.x + 2, pt.y - 1), CPoint (pt.x, pt.y + 1), pDC->GetTextColor());
}


void CXTPDefaultTheme::FillCommandBarEntry(CDC* pDC, CXTPCommandBar* pBar)
{
	CRect rc;
	pBar->GetClientRect(&rc);
	pDC->FillSolidRect(rc, GetXtremeColor(COLOR_3DFACE));

	if (pBar->GetPosition() == xtpBarFloating || pBar->GetPosition() == xtpBarPopup)
	{
		Draw3dRect(pDC, rc, COLOR_3DFACE, 0);
		rc.DeflateRect(1, 1);
	}
	Draw3dRect(pDC, rc, COLOR_BTNHILIGHT, COLOR_3DSHADOW);
}


CSize CXTPDefaultTheme::DrawDialogBarGripper(CDC* pDC, CXTPCommandBar* /*pBar*/, BOOL bDraw)
{
	CSize sz(6, max(22, m_nTextHeight + 4));

	if (pDC && bDraw)
	{
		Draw3dRect(pDC, CRect(3, 3, 6, sz.cy), COLOR_BTNHILIGHT, COLOR_3DSHADOW);
	}

	return sz;
}


CSize CXTPDefaultTheme::DrawCommandBarGripper(CDC* pDC, CXTPCommandBar* pBar, BOOL bDraw)
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
			pDC->FillSolidRect(rc, GetXtremeColor(COLOR_ACTIVECAPTION));
			pDC->SetTextColor(GetXtremeColor(COLOR_3DHIGHLIGHT));
			pDC->DrawText(pBar->GetTitle(), CRect(5, 3, rc.right - 2* rc.Size().cy, rc.bottom), DT_SINGLELINE | DT_VCENTER );
		}
		return rc.Size();

	}
	else if (pBar->GetPosition() == xtpBarRight || pBar->GetPosition() == xtpBarLeft)
	{
		if (pDC && bDraw)
			Draw3dRect(pDC, CRect(3, 3, rc.right - 3, 6), COLOR_BTNHILIGHT, COLOR_3DSHADOW);
		return CSize(0, 8);
	}
	else if (pBar->GetPosition() == xtpBarTop || pBar->GetPosition() == xtpBarBottom)
	{
		if (pDC && bDraw)
			Draw3dRect(pDC, CRect(3, 3, 6, rc.bottom - 3), COLOR_BTNHILIGHT, COLOR_3DSHADOW);
		return CSize(6, 0);
	}
	return 0;
}

CSize CXTPDefaultTheme::DrawCommandBarSeparator(CDC* pDC, CXTPCommandBar* pBar, CXTPControl* pControl, BOOL bDraw)
{
	if (!bDraw)
	{
		ASSERT(pBar->GetType() == xtpBarTypePopup);
		return CSize(0, 8);
	}


	CRect rc;
	pBar->GetClientRect(&rc);
	CRect rcControl = pControl->GetRect();
	CRect rcRaw = pControl->GetRawRect();

	if (pBar->GetType() == xtpBarTypePopup)
	{
		Draw3dRect(pDC, CRect(14, rcControl.top - 2 - 2 -1, rc.right - 14, rcControl.top - 0 - 2 -  1), COLOR_3DSHADOW, COLOR_BTNHILIGHT);
	}
	else if (pBar->GetPosition() != xtpBarPopup && IsVerticalPosition(pBar->GetPosition()))
	{
		if (!pControl->GetWrap())
			Draw3dRect(pDC, CRect(rcRaw.left, rcControl.top - 4, rcRaw.right, rcControl.top - 2), COLOR_3DSHADOW, COLOR_BTNHILIGHT);
		else
			Draw3dRect(pDC, CRect(rcRaw.right + 2, rcRaw.top + 4, rcRaw.right + 4,  rcRaw.bottom), COLOR_3DSHADOW, COLOR_BTNHILIGHT);
	} else
	{
		if (!pControl->GetWrap())
			Draw3dRect(pDC, CRect(rcControl.left - 4, rcRaw.top, rcControl.left - 2, rcRaw.bottom), COLOR_3DSHADOW, COLOR_BTNHILIGHT);
		else
			Draw3dRect(pDC, CRect(rcRaw.left + 4, rcRaw.top - 4, rcRaw.right, rcRaw.top - 2), COLOR_3DSHADOW, COLOR_BTNHILIGHT);
	}
	return 0;
}

void CXTPDefaultTheme::AdjustExcludeRect(CRect& rc, CXTPControl* pControl, BOOL bVertical)
{
	CXTPCommandBar* pParent = pControl->GetParent();
	ASSERT(pParent);

	if (pControl->GetType() == xtpControlComboBox) return;

	if (pParent->GetType() != xtpBarTypePopup)
	{
		if (bVertical)
			rc.InflateRect(0, 1); else rc.InflateRect(1, 0);
	} else rc.DeflateRect(1, 0);
}

CRect CXTPDefaultTheme::GetCommandBarBorders(CXTPCommandBar* pBar)
{
	if (pBar->GetType() == xtpBarTypePopup)
		return CRect(3, 3, 3, 3);
	else return CXTPPaintManager::GetCommandBarBorders(pBar);
}

CSize CXTPDefaultTheme::DrawListBoxControl(CDC* pDC, CXTPControl* pButton, CRect rcButton, BOOL bSelected, BOOL bDraw, CXTPCommandBars* pCommandBars)
{
	CSize szIcon = pCommandBars? pCommandBars->GetCommandBarsOptions()->szPopupIcons: CSize(16, 16);
	CSize szImage(max(szIcon.cx + 4, m_nTextHeight), max(szIcon.cy + 4, m_nTextHeight));

	if (bDraw)
	{
		if (pButton == NULL)
		{
			pDC->FillSolidRect(rcButton, GetXtremeColor(COLOR_3DFACE));
			return 0;
		}
		pDC->FillSolidRect(rcButton, GetXtremeColor(COLOR_3DFACE));

		CRect rcSelection(rcButton.left + 1, rcButton.top, rcButton.right - 1, rcButton.bottom);
		XTPControlType controlType = pButton->GetType();
		XTPButtonStyle buttonStyle = pButton->GetStyle();

		BOOL bDrawImage = (buttonStyle == xtpButtonAutomatic && controlType != xtpControlPopup) ||
			(buttonStyle == xtpButtonIcon) || (buttonStyle == xtpButtonIconAndCaption);

		CXTPImageManagerIcon* pImage = pButton->GetIconId() != 0 && bDrawImage? pButton->GetImage(szIcon.cx): NULL;
		if (pImage)
		{
			szIcon.cy = pImage->GetHeight() * szIcon.cx / pImage->GetWidth();
			CRect rcImageRect(rcButton.left, rcButton.top, rcButton.left + szImage.cx, rcButton.bottom);
			DrawRectangle(pDC, rcImageRect, bSelected, FALSE, TRUE, FALSE, FALSE, FALSE);
			CPoint ptImage = CPoint(rcImageRect.left + rcImageRect.Width()/2 - szIcon.cx/2, rcImageRect.top +  rcImageRect.Height()/2 - szIcon.cy/2);
			DrawImage(pDC, ptImage, szIcon, pImage, bSelected, FALSE, TRUE, FALSE, FALSE, FALSE);
			rcSelection.left = rcImageRect.right + 1;
		}
		if (bSelected)
			Rectangle(pDC, rcSelection, COLOR_HIGHLIGHT, COLOR_HIGHLIGHT);


		COLORREF clrText = GetXtremeColor(bSelected? COLOR_HIGHLIGHTTEXT: COLOR_BTNTEXT);

		pDC->SetTextColor(clrText);
		pDC->SetBkMode (TRANSPARENT);

		CString strText = pButton->GetCaption();
		if (pCommandBars && (pCommandBars->GetSite()->GetExStyle() & WS_EX_LAYOUTRTL))
			UpdateRTLCaption(strText);
		else
			StripMnemonics(strText);

		CRect rcText(rcButton.left + szImage.cx + 4, rcButton.top, rcButton.right, rcButton.bottom);
		CXTPFontDC fnt(pDC, &m_fontRegular);
		pDC->DrawText(strText, &rcText, DT_SINGLELINE | DT_VCENTER | DT_END_ELLIPSIS|DT_NOPREFIX);

		if (controlType == xtpControlPopup || controlType == xtpControlButtonPopup || controlType == xtpControlSplitButtonPopup)
		{
			CPoint pt(rcButton.right - 7, rcButton.CenterPoint().y);
			Triangle(pDC, pt, CPoint(pt.x - 5, pt.y - 5), CPoint(pt.x - 5, pt.y + 5), clrText);
		}
		if (controlType == xtpControlSplitButtonPopup)
		{
			Line(pDC, rcButton.right - 20, rcButton.top + 1, rcButton.right - 20, rcButton.bottom - 1, bSelected? COLOR_HIGHLIGHTTEXT: COLOR_BTNTEXT );
		}

		if (controlType == xtpControlComboBox || controlType == xtpControlEdit)
		{
			CXTPEmptyRect rcText;
			pDC->DrawText(strText, &rcText, DT_SINGLELINE|DT_CALCRECT);
			int nThumb = pDC->GetTextExtent(_T(" ")).cy + 2;

			CRect rcCombo(rcButton.left + szImage.cx + 4 + rcText.Width() + 5, rcButton.top, rcButton.right, rcButton.bottom);
			CRect rcThumb(rcButton.right - nThumb, rcButton.top, rcButton.right, rcButton.bottom);

			Rectangle(pDC, rcCombo, bSelected? COLOR_HIGHLIGHT: COLOR_3DFACE, COLOR_WINDOW);

			if (controlType == xtpControlComboBox)
			{
				if (!bSelected) pDC->FillSolidRect(rcThumb.left + 2, rcThumb.top + 2, rcThumb.Width() - 4, rcThumb.Height() - 4, GetXtremeColor(COLOR_3DFACE));
				else Rectangle(pDC, rcThumb, COLOR_HIGHLIGHT, COLOR_3DFACE);

				rcThumb.left++; rcThumb.top++;
				CXTPFontDC fontMarlett(pDC, &m_fontMarlettIcon);
				pDC->SetTextColor(GetXtremeColor(bSelected? XPCOLOR_HIGHLIGHT_TEXT: XPCOLOR_TOOLBAR_TEXT));
				pDC->DrawText(_T("u"), rcThumb, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
			}
		}
	}
	return szImage;
}

CSize CXTPDefaultTheme::DrawTearOffGripper(CDC* pDC, CRect rcGripper, BOOL bSelected, BOOL bDraw)
{
	if (bDraw)
	{
		CRect rc(rcGripper.left, rcGripper.top, rcGripper.right, rcGripper.bottom - 1);
		pDC->FillSolidRect(rc, GetXtremeColor(bSelected? COLOR_ACTIVECAPTION: COLOR_INACTIVECAPTION));
	}

	return CSize(0, 8);
}
