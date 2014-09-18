// XTPOfficeTheme.cpp : implementation of the CXTPOfficeTheme class.
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
#include "XTPControlComboBox.h"
#include "XTPControlEdit.h"
#include "XTPControls.h"
#include "XTPCommandBars.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace XTPPaintThemes;

CXTPOfficeTheme::CXTPOfficeTheme()
{
	m_iconsInfo.bUseFadedIcons = TRUE;
	m_iconsInfo.bIconsWithShadow = TRUE;
}

void CXTPOfficeTheme::DrawRectangle(CDC* pDC, CRect rc, BOOL bSelected, BOOL bPressed, BOOL bEnabled, BOOL bChecked, BOOL bPopuped, BOOL bToolBar, XTPBarPosition /*barPosition = xtpBarPopup*/)
{
	if (!bEnabled)
	{
		if (bChecked && !bToolBar)
		{
			Rectangle(pDC, rc, XPCOLOR_DISABLED, XPCOLOR_TOOLBAR_FACE);
		}
		if ( bSelected == TRUE_KEYBOARD)
			Rectangle(pDC, rc, XPCOLOR_HIGHLIGHT_BORDER, bToolBar? XPCOLOR_HIGHLIGHT: XPCOLOR_MENUBAR_FACE);
		return;
	}
	if (bPopuped) Rectangle(pDC, rc, XPCOLOR_MENUBAR_BORDER, XPCOLOR_TOOLBAR_FACE);
	else if (bChecked && !bSelected && !bPressed) Rectangle(pDC, rc, XPCOLOR_HIGHLIGHT_PUSHED_BORDER, XPCOLOR_HIGHLIGHT_CHECKED);
	else if (bChecked && bSelected && !bPressed) Rectangle(pDC, rc, XPCOLOR_HIGHLIGHT_CHECKED_BORDER, XPCOLOR_HIGHLIGHT_PUSHED);
	else if (bPressed == TRUE_KEYBOARD || (bSelected && bPressed)) Rectangle(pDC, rc, XPCOLOR_HIGHLIGHT_PUSHED_BORDER, XPCOLOR_HIGHLIGHT_PUSHED);
	else if (bSelected || bPressed)  Rectangle(pDC, rc, XPCOLOR_HIGHLIGHT_BORDER, XPCOLOR_HIGHLIGHT);
}


void CXTPOfficeTheme::DrawImage(CDC* pDC, CPoint pt, CSize sz, CXTPImageManagerIcon* pImage, BOOL bSelected, BOOL bPressed, BOOL bEnabled, BOOL bChecked, BOOL bPopuped, BOOL)
{
	if (!bEnabled)
	{
		if (!m_iconsInfo.bUseDisabledIcons && !pImage->IsAlpha())
			pImage->DrawMono(pDC, pt, pImage->GetIcon(), GetXtremeColor(XPCOLOR_DISABLED));
		else pImage->Draw(pDC, pt, pImage->GetDisabledIcon(), sz);
	}

	else if (bPopuped || bChecked)
		pImage->Draw(pDC, pt, bChecked && (bSelected || bPressed)? pImage->GetHotIcon(): bChecked? pImage->GetCheckedIcon(): pImage->GetIcon(), sz);
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
		pImage->Draw(pDC, pt, pImage->GetHotIcon(), sz);
}




CSize CXTPOfficeTheme::DrawControlEdit(CDC* pDC, CXTPControlEdit* pControlEdit, BOOL bDraw)
{

	BOOL bPopupBar = pControlEdit->GetParent()->GetType() == xtpBarTypePopup;

	if (!bDraw)
	{
		CString strCaption(pControlEdit->GetCaption());
		StripMnemonics(strCaption);

		CXTPFontDC font(pDC, &m_fontIcon);
		int nHeight = pDC->GetTextExtent(_T(" ")).cy + 1;

		if (bPopupBar)
		{
			pControlEdit->SetLabelWidth(pDC->GetTextExtent(strCaption).cx + GetGripperWidth(pControlEdit->GetParent()) + 12);
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
		BOOL bSelected = pControlEdit->GetSelected();

		if (bPopupBar ||pControlEdit->IsLabeled())
		{
			CXTPFontDC font(pDC, &m_fontIcon);


			CRect rcText(rc); rcText.left += bPopupBar? GetGripperWidth(pControlEdit->GetParent()) + 7: 2;

			if (bPopupBar) pDC->SetTextColor( GetXtremeColor(!pControlEdit->GetEnabled()? XPCOLOR_GRAYTEXT: bSelected? XPCOLOR_HIGHLIGHT_TEXT: XPCOLOR_MENUBAR_TEXT));
			else pDC->SetTextColor( GetXtremeColor( !pControlEdit->GetEnabled()? XPCOLOR_DISABLED: XPCOLOR_TOOLBAR_TEXT));

			if (bPopupBar && bSelected)
				DrawRectangle(pDC, rc, bSelected, FALSE, pControlEdit->GetEnabled(), FALSE, FALSE, FALSE);

			pDC->DrawText(pControlEdit->GetCaption(), rcText, DT_SINGLELINE | DT_VCENTER);
			rc.left += pControlEdit->GetLabelWidth();
		}

		if (!pControlEdit->GetEnabled())
		{
			rc.DeflateRect(1, 0, 0, 0);
			Rectangle (pDC, rc, bSelected == TRUE_KEYBOARD? XPCOLOR_HIGHLIGHT_BORDER: XPCOLOR_DISABLED, COLOR_3DFACE);
		}
		else
		{
			pDC->FillSolidRect(rc.left + 1, rc.top + 1, rc.Width() - 2, rc.Height() - 2, GetXtremeColor(COLOR_WINDOW));

			if ( bSelected)
			{
				Rectangle(pDC, rc, XPCOLOR_HIGHLIGHT_BORDER, COLOR_WINDOW);
			} else if (bPopupBar)
			{
				Draw3dRect(pDC, rc, XPCOLOR_EDITCTRLBORDER, XPCOLOR_EDITCTRLBORDER);
			}

		}
	}

	return 0;
}


CSize CXTPOfficeTheme::DrawControlComboBox(CDC* pDC, CXTPControlComboBox* pControlCombo, BOOL bDraw)
{
	BOOL bPopupBar = pControlCombo->GetParent()->GetType() == xtpBarTypePopup;

	if (!bDraw)
	{
		CString strCaption(pControlCombo->GetCaption());
		StripMnemonics(strCaption);

		CXTPFontDC font(pDC, &m_fontIcon);
		int nHeight = pDC->GetTextExtent(_T(" ")).cy + 1;

		pControlCombo->SetThumbWidth(pDC->GetTextExtent(_T(" ")).cy + 2);

		if (bPopupBar)
		{
			pControlCombo->SetLabelWidth(pDC->GetTextExtent(strCaption).cx + GetGripperWidth(pControlCombo->GetParent()) + 12);
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
		BOOL bDropped = pControlCombo->GetDroppedState() != 0;
		BOOL bSelected = pControlCombo->GetSelected();

		if (bPopupBar ||pControlCombo->GetStyle() == xtpComboLabel)
		{
			CXTPFontDC font(pDC, &m_fontIcon);


			CRect rcText(rc); rcText.left += bPopupBar? GetGripperWidth(pControlCombo->GetParent()) + 7: 2;

			if (bPopupBar) pDC->SetTextColor( GetXtremeColor(!pControlCombo->GetEnabled()? XPCOLOR_GRAYTEXT: bSelected? XPCOLOR_HIGHLIGHT_TEXT: XPCOLOR_MENUBAR_TEXT));
			else pDC->SetTextColor( GetXtremeColor( !pControlCombo->GetEnabled()? XPCOLOR_DISABLED: XPCOLOR_TOOLBAR_TEXT));

			if (bPopupBar && bSelected)
			{
				DrawRectangle(pDC, rc, bSelected, FALSE, pControlCombo->GetEnabled(), FALSE, FALSE, FALSE);
			}


			pDC->DrawText(pControlCombo->GetCaption(), rcText, DT_SINGLELINE | DT_VCENTER);
			rc.left += pControlCombo->GetLabelWidth();
		}
		int nThumb = pControlCombo->GetThumbWidth();
		CRect rcBtn (rc.right - nThumb, rc.top, rc.right, rc.bottom);

		if (!pControlCombo->GetEnabled())
		{
			rc.DeflateRect(1, 0, 0, 0);
			Rectangle (pDC, rc, bSelected == TRUE_KEYBOARD? XPCOLOR_HIGHLIGHT_BORDER: XPCOLOR_DISABLED, COLOR_3DFACE);
		}
		else
		{

			CRect rcEntry(rc.left + 1, rc.top + 1, rc.right - nThumb + 1, rc.bottom -1);
			Rectangle (pDC, rcEntry, COLOR_WINDOW, COLOR_WINDOW);

			if ( bSelected)
			{

				Rectangle(pDC, rc, XPCOLOR_HIGHLIGHT_BORDER, COLOR_WINDOW);
				Rectangle(pDC, rcBtn, XPCOLOR_HIGHLIGHT_BORDER, bDropped ? XPCOLOR_HIGHLIGHT_PUSHED: XPCOLOR_HIGHLIGHT);
			} else
			{
				if (bPopupBar)
				{
					Draw3dRect(pDC, rc, XPCOLOR_EDITCTRLBORDER, XPCOLOR_EDITCTRLBORDER);
					rcBtn.DeflateRect(1, 1);
					Rectangle(pDC, rcBtn, COLOR_WINDOW, XPCOLOR_EDITCTRLBORDER);
				}
				else
				{
					rcBtn.DeflateRect(1, 1);
					Draw3dRect(pDC, rcBtn, COLOR_WINDOW, COLOR_WINDOW);
				}
			}

			if (!pControlCombo->GetEditCtrl()) // CBS_DROPDOWNLIST
			{
				CRect rcText(rc); rcText.DeflateRect(3, 1, rcBtn.Width(), 1);
				pDC->SetTextColor(GetXtremeColor(COLOR_WINDOWTEXT));
				pDC->DrawText(pControlCombo->GetText(), rcText, DT_SINGLELINE | DT_VCENTER | DT_NOPREFIX);

			}
		}

		rcBtn.left++; rcBtn.top++;
		CXTPFontDC fontMarlett(pDC, &m_fontMarlettIcon);
		pDC->SetTextColor(GetXtremeColor(!pControlCombo->GetEnabled()? XPCOLOR_DISABLED: bDropped? XPCOLOR_PUSHED_TEXT: bSelected? XPCOLOR_HIGHLIGHT_TEXT: XPCOLOR_TOOLBAR_TEXT));
		pDC->DrawText(_T("u"), rcBtn, DT_SINGLELINE | DT_VCENTER | DT_CENTER);

	}

	return 0;
}


void CXTPOfficeTheme::DrawPopupBarGripper(CDC* pDC, int x, int y, int cx, int cy, BOOL bExpanded)
{
	pDC->FillSolidRect(x, y, cx, cy, GetXtremeColor(bExpanded? XPCOLOR_MENUBAR_EXPANDED: XPCOLOR_TOOLBAR_FACE));
}

CSize CXTPOfficeTheme::DrawControlPopupParent(CDC* pDC, CXTPControl* pButton, BOOL bDraw)
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
	CSize szCheck(szIcon.cx + 4, szIcon.cy + 4);
	CSize szImage(max(szIcon.cx + 4, m_nTextHeight + 4), max(szIcon.cy + 6, m_nTextHeight));

	if (bDraw)
	{
		if (pButton->GetExpanded())
		{
			DrawPopupBarGripper(pDC, 1, rcButton.top, szImage.cx + 1, rcButton.Height(), TRUE);
		}

		if (pButton->IsLabel())
		{
			pDC->FillSolidRect(szImage.cx + 3, rcButton.top, rcButton.right - (szImage.cx + 3), rcButton.Height(),
				GetXtremeColor(XPCOLOR_LABEL));
		}
		else
			DrawRectangle(pDC, rcButton, bSelected, FALSE, bEnabled, FALSE, FALSE, FALSE);

		CRect rcImageRect(rcButton.left, rcButton.top, rcButton.left + szImage.cx, rcButton.bottom);
		CRect rcCheck (CPoint(rcImageRect.left + rcImageRect.Width()/2 - szCheck.cx/2, rcImageRect.top + rcImageRect.Height()/2 - szCheck.cy/2), szCheck);

		if (bChecked && bEnabled)
			DrawRectangle(pDC, rcCheck, bSelected, FALSE, bEnabled, TRUE, FALSE, FALSE);

		CXTPImageManagerIcon* pImage = pButton->GetIconId() != 0 && bDrawImage? pButton->GetImage(szIcon.cx): NULL;
		if (pImage)
		{
			szIcon.cy = pImage->GetHeight() * szIcon.cx / pImage->GetWidth();
			CPoint ptImage = CPoint(rcImageRect.left + rcImageRect.Width()/2 - szIcon.cx/2, rcImageRect.top + rcImageRect.Height()/2 - szIcon.cy/2);
			DrawImage(pDC, ptImage, szIcon, pImage, bSelected, FALSE, bEnabled, bChecked, FALSE, FALSE);
		}
		else if (bChecked) DrawCheckMark(pDC, rcCheck, 0, bLayoutRTL);


		COLORREF clrText = !bEnabled? GetXtremeColor(XPCOLOR_GRAYTEXT): bSelected? GetXtremeColor(XPCOLOR_HIGHLIGHT_TEXT): GetXtremeColor(XPCOLOR_MENUBAR_TEXT);

		pDC->SetTextColor(clrText);
		pDC->SetBkMode (TRANSPARENT);

		if (IsPopupControlType(controlType))
		{
			CPoint pt = CPoint(rcButton.right - 11, rcButton.CenterPoint().y);
			if (pButton->GetType() == xtpControlSplitButtonPopup && bEnabled)
			{
				if (!bPopuped || !bSelected)
					Line(pDC, rcButton.right - 20, rcButton.top + 1, rcButton.right - 20, rcButton.bottom - 1, bSelected? XPCOLOR_HIGHLIGHT_BORDER: COLOR_3DSHADOW);
				else
					Rectangle(pDC, CRect(rcButton.right - 20, rcButton.top, rcButton.right, rcButton.bottom), XPCOLOR_HIGHLIGHT_BORDER, XPCOLOR_HIGHLIGHT_PUSHED);

				Triangle(pDC, CPoint(pt.x, pt.y - 3), CPoint(pt.x + 3, pt.y), CPoint(pt.x, pt.y + 3), !bPopuped || !bSelected? clrText: GetXtremeColor(XPCOLOR_PUSHED_TEXT));
			}
			else
				Triangle(pDC, CPoint(pt.x, pt.y - 3), CPoint(pt.x + 3, pt.y), CPoint(pt.x, pt.y + 3), clrText);
		}

		rcText.SetRect(rcButton.left + szImage.cx + 8, rcButton.top, rcButton.right, rcButton.bottom);
		rcShortcut.SetRect(rcButton.left + szImage.cx + 8, rcButton.top, rcButton.right - 19, rcButton.bottom);
	}

	pDC->DrawText(strCaption, &rcText, DT_SINGLELINE | DT_VCENTER | (!bDraw ? DT_CALCRECT : DT_END_ELLIPSIS) );

	if (controlType == xtpControlButton && pButton->GetShortcutText().GetLength() != 0 )
		pDC->DrawText(pButton->GetShortcutText(), &rcShortcut, DT_SINGLELINE | DT_VCENTER | (!bDraw ? DT_CALCRECT : DT_RIGHT) );

	return CSize(szImage.cx + 8 + rcText.Width() +  rcShortcut.Width() + 32, szImage.cy);
}

void CXTPOfficeTheme::DrawControlEntry(CDC* pDC, CRect rc, BOOL bEnabled, BOOL bSelected, BOOL bPressed, BOOL bChecked, BOOL bPopuped, XTPBarPosition barPosition)
{
	DrawRectangle(pDC, rc, bSelected, bPressed, bEnabled, bChecked, bPopuped, TRUE, barPosition);

	int nColorFace = barPosition == xtpBarPopup? XPCOLOR_MENUBAR_TEXT: XPCOLOR_TOOLBAR_TEXT;

	COLORREF clrText = GetXtremeColor( !bEnabled? XPCOLOR_DISABLED:
		bPopuped? XPCOLOR_TOOLBAR_TEXT:
		bChecked && !bSelected && !bPressed? XPCOLOR_CHECKED_TEXT:
		bPressed == TRUE_KEYBOARD || (bSelected && (bPressed || bChecked))? XPCOLOR_PUSHED_TEXT:
		bSelected || bPressed? XPCOLOR_HIGHLIGHT_TEXT: nColorFace);

	pDC->SetTextColor(clrText);
	pDC->SetBkMode (TRANSPARENT);
}

void CXTPOfficeTheme::FillIntersectRect(CDC* pDC, CXTPPopupBar* pPopupBar, COLORREF clr)
{
	if (pPopupBar && pPopupBar->GetControlPopup())
	{
		CRect rcParent;
		pPopupBar->GetControlPopup()->GetParent()->GetWindowRect(rcParent);
		CRect rcExclude = pPopupBar->GetControlPopup()->GetRect();
		pPopupBar->GetControlPopup()->GetParent()->ClientToScreen(&rcExclude);

		CRect rcWindow;
		pPopupBar->GetWindowRect(&rcWindow);

		CRect rcIntersect;

		if (rcIntersect.IntersectRect(rcWindow, rcExclude))
		{
			pPopupBar->ScreenToClient(rcIntersect);

			if (rcIntersect.Width() > 1) rcIntersect.DeflateRect(1, 0);
			if (rcIntersect.Height() > 1) rcIntersect.DeflateRect(0, 1);

			pDC->FillSolidRect(rcIntersect, clr);
		}
	}
}

int CXTPOfficeTheme::GetGripperWidth(CXTPCommandBar* pBar)
{
	CSize szIcon = pBar->GetIconSize();
	return max(szIcon.cx + 4, m_nTextHeight + 4) + 1;
}


void CXTPOfficeTheme::FillCommandBarEntry(CDC* pDC, CXTPCommandBar* pBar)
{
	CRect rc;
	pBar->GetClientRect(&rc);

	if (pBar->GetPosition() == xtpBarFloating)
	{
		CXTPPenDC pen(*pDC, GetXtremeColor(XPCOLOR_MENUBAR_BORDER));
		CXTPBrushDC brush(*pDC, GetXtremeColor(XPCOLOR_TOOLBAR_FACE));
		if (pBar->GetType() == xtpBarTypeMenuBar)
			brush.Color(GetXtremeColor(COLOR_3DFACE));


		pDC->Rectangle(rc);
		rc.DeflateRect(1, 1);
		pDC->Rectangle(rc);

		pDC->SetPixel(2, 2, pen.Color());
		pDC->SetPixel(rc.right - 2, 2, pen.Color());
		pDC->SetPixel(2, rc.bottom - 2, pen.Color());
		pDC->SetPixel(rc.right - 2, rc.bottom - 2, pen.Color());


	} else if (pBar->GetType() == xtpBarTypeMenuBar)
	{
		pDC->FillSolidRect(rc, GetXtremeColor(COLOR_3DFACE));
	}
	else if (pBar->GetPosition() == xtpBarPopup)
	{
		Rectangle(pDC, rc, XPCOLOR_MENUBAR_BORDER, XPCOLOR_MENUBAR_FACE);
		if (pBar->GetType() == xtpBarTypePopup)
			DrawPopupBarGripper(pDC, 1, 2, GetGripperWidth(pBar), rc.bottom - 4);

		CXTPPopupBar* pPopupBar = DYNAMIC_DOWNCAST(CXTPPopupBar, pBar);
		FillIntersectRect(pDC, pPopupBar, GetXtremeColor(XPCOLOR_TOOLBAR_FACE));


	}
	else
	{
		COLORREF clr3DFace = GetXtremeColor(COLOR_3DFACE);

		pDC->FillSolidRect(rc, GetXtremeColor(XPCOLOR_TOOLBAR_FACE));
		pDC->Draw3dRect(rc, clr3DFace, clr3DFace);

		pDC->SetPixel(1, 1, clr3DFace);
		pDC->SetPixel(rc.right - 2, 1, clr3DFace);
		pDC->SetPixel(1, rc.bottom - 2, clr3DFace);
		pDC->SetPixel(rc.right - 2, rc.bottom - 2, clr3DFace);
	}
}

CSize CXTPOfficeTheme::DrawDialogBarGripper(CDC* pDC, CXTPCommandBar* /*pBar*/, BOOL bDraw)
{
	CSize sz(6, max(22, m_nTextHeight + 4));

	if (pDC && bDraw)
	{
		CXTPPenDC pen (*pDC, GetXtremeColor(XPCOLOR_TOOLBAR_GRIPPER));
		for (int y = 6; y < sz.cy - 2; y += 2)
			Line(pDC, CPoint(3, y), CPoint(6, y));
	}

	return sz;
}

CSize CXTPOfficeTheme::DrawCommandBarGripper(CDC* pDC, CXTPCommandBar* pBar, BOOL bDraw)
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
			pDC->FillSolidRect(rc, GetXtremeColor(COLOR_3DSHADOW));
			pDC->SetTextColor(GetXtremeColor(COLOR_3DHIGHLIGHT));
			pDC->DrawText(pBar->GetTitle(), CRect(5, 3, rc.right - 2* rc.Size().cy, rc.bottom), DT_SINGLELINE | DT_VCENTER );
		}
		return rc.Size();

	} else if (pBar->GetPosition() == xtpBarRight || pBar->GetPosition() == xtpBarLeft)
	{
		if (pDC && bDraw)
		{
			CXTPPenDC pen (*pDC, GetXtremeColor(XPCOLOR_TOOLBAR_GRIPPER));
			for (int x = 5; x < rc.Width() - 5; x += 2)
				Line(pDC, CPoint(x, 3), CPoint(x, 6));
		}
		return CSize(0, 8);
	} else if (pBar->GetPosition() == xtpBarTop || pBar->GetPosition() == xtpBarBottom)
	{
		if (pDC && bDraw)
		{
			CXTPPenDC pen (*pDC, GetXtremeColor(XPCOLOR_TOOLBAR_GRIPPER));
			for (int y = 5; y < rc.Height() - 5; y += 2)
				Line(pDC, CPoint(3, y), CPoint(6, y));
		}
		return CSize(6, 0);
	}
	return 0;
}
CSize CXTPOfficeTheme::DrawCommandBarSeparator(CDC* pDC, CXTPCommandBar* pBar, CXTPControl* pControl, BOOL bDraw)
{
	if (!bDraw)
	{
		ASSERT(pBar->GetType() == xtpBarTypePopup);
		return CSize(0, 3);
	}

	CRect rc;
	pBar->GetClientRect(&rc);
	CRect rcControl = pControl->GetRect();
	CXTPPenDC pen (*pDC, GetXtremeColor(XPCOLOR_SEPARATOR));
	CRect rcRaw = pControl->GetRawRect();

	if (pBar->GetType() == xtpBarTypePopup)
	{
		pDC->MoveTo (GetGripperWidth(pBar) + 9 + (pControl->m_bDoubleGripper? GetGripperWidth(pBar) - 1: 0), rcControl.top - 2 );
		pDC->LineTo (rc.right - 1, rcControl.top - 2);
	}
	else if (pBar->GetPosition() != xtpBarPopup && IsVerticalPosition(pBar->GetPosition()))
	{
		if (!pControl->GetWrap())
			Line(pDC, CPoint(rcRaw.left, rcControl.top - 4), CPoint(rcRaw.right, rcControl.top - 4));
		else
			Line(pDC, CPoint(rcRaw.right + 2, rcRaw.top + 4), CPoint(rcRaw.right + 2, rcRaw.bottom));

	} else
	{
		if (!pControl->GetWrap())
			Line(pDC, CPoint(rcControl.left - 4, rcRaw.top), CPoint(rcControl.left - 4, rcRaw.bottom));
		else
			Line(pDC, CPoint(rcRaw.left + 4, rcRaw.top - 4), CPoint(rcRaw.right, rcRaw.top - 4));
	}
	return 0;
}
void CXTPOfficeTheme::AdjustExcludeRect(CRect& rc, CXTPControl* pControl, BOOL bVertical)
{
	CXTPCommandBar* pParent = pControl->GetParent();
	ASSERT(pParent);

	if (pParent->GetType() != xtpBarTypePopup)
	{
		if (pControl->GetType() == xtpControlComboBox) return;

		if (bVertical)
			rc.DeflateRect(1, 0); else rc.DeflateRect(0, 1);
	} else rc.InflateRect(1, 0);
}

void CXTPOfficeTheme::DrawStatusBarPaneBorder(CDC* pDC, CRect rc, BOOL /*bGripperPane*/)
{
	Draw3dRect(pDC, rc, COLOR_3DSHADOW, COLOR_3DSHADOW);
}

CSize CXTPOfficeTheme::DrawListBoxControl(CDC* pDC, CXTPControl* pButton, CRect rcButton, BOOL bSelected, BOOL bDraw, CXTPCommandBars* pCommandBars)
{
	CSize szIcon = pCommandBars? pCommandBars->GetCommandBarsOptions()->szPopupIcons: CSize(16, 16);
	CSize szImage(max(szIcon.cx + 4, m_nTextHeight + 4), max(szIcon.cy + 6, m_nTextHeight));

	if (bDraw)
	{
		pDC->FillSolidRect(rcButton, GetXtremeColor(XPCOLOR_MENUBAR_FACE));
		DrawPopupBarGripper(pDC, 0, rcButton.top, szImage.cx + 1, rcButton.Height());

		if (pButton == NULL)
		{
			return 0;
		}


		XTPControlType controlType = pButton->GetType();
		XTPButtonStyle buttonStyle = pButton->GetStyle();

		if (bSelected)
			DrawRectangle(pDC, rcButton, bSelected, FALSE, TRUE, FALSE, FALSE, FALSE);

		BOOL bDrawImage = (buttonStyle == xtpButtonAutomatic && controlType != xtpControlPopup) ||
			(buttonStyle == xtpButtonIcon) || (buttonStyle == xtpButtonIconAndCaption);

		CXTPImageManagerIcon* pImage = pButton->GetIconId() != 0 && bDrawImage? pButton->GetImage(szIcon.cx): NULL;
		if (pImage)
		{
			szIcon.cy = pImage->GetHeight() * szIcon.cx / pImage->GetWidth();
			CPoint ptImage = CPoint(rcButton.left + szImage.cx/2  - szIcon.cx/2, rcButton.top + rcButton.Height()/2  - szIcon.cy/2);
			DrawImage(pDC, ptImage, szIcon, pImage, bSelected, FALSE, TRUE, FALSE, FALSE, FALSE);
		}


		COLORREF clrText = bSelected? GetXtremeColor(XPCOLOR_HIGHLIGHT_TEXT): GetXtremeColor(XPCOLOR_MENUBAR_TEXT);

		pDC->SetTextColor(clrText);
		pDC->SetBkMode (TRANSPARENT);

		CString strText = pButton->GetCaption();
		if (pCommandBars && (pCommandBars->GetSite()->GetExStyle() & WS_EX_LAYOUTRTL))
			UpdateRTLCaption(strText);
		else
		StripMnemonics(strText);



		CRect rcText(rcButton.left + szImage.cx + 8, rcButton.top, rcButton.right, rcButton.bottom);
		CXTPFontDC fnt(pDC, &m_fontRegular);
		pDC->DrawText(strText, &rcText, DT_SINGLELINE | DT_VCENTER | DT_END_ELLIPSIS|DT_NOPREFIX);

		if (controlType == xtpControlPopup || controlType == xtpControlButtonPopup || controlType == xtpControlSplitButtonPopup)
		{
			CPoint pt(rcButton.right - 7, rcButton.CenterPoint().y);
			Triangle(pDC, pt, CPoint(pt.x - 5, pt.y - 5), CPoint(pt.x - 5, pt.y + 5), clrText);
		}
		if (controlType == xtpControlSplitButtonPopup)
		{
			Line(pDC, rcButton.right - 20, rcButton.top + 1, rcButton.right - 20, rcButton.bottom - 1, bSelected? XPCOLOR_HIGHLIGHT_BORDER: COLOR_3DSHADOW );
		}
		if (controlType == xtpControlComboBox || controlType == xtpControlEdit)
		{
			CXTPEmptyRect rcText;
			pDC->DrawText(strText, &rcText, DT_SINGLELINE|DT_CALCRECT);
			int nThumb = pDC->GetTextExtent(_T(" ")).cy + 2;

			CRect rcCombo(rcButton.left + szImage.cx + 8 + rcText.Width() + 5, rcButton.top, rcButton.right, rcButton.bottom);
			CRect rcThumb(rcButton.right - nThumb, rcButton.top, rcButton.right, rcButton.bottom);

			Rectangle(pDC, rcCombo, bSelected? XPCOLOR_HIGHLIGHT_BORDER: COLOR_3DFACE, COLOR_WINDOW);
			if (controlType == xtpControlComboBox)
			{
				if (!bSelected) pDC->FillSolidRect(rcThumb.left + 2, rcThumb.top + 2, rcThumb.Width() - 4, rcThumb.Height() - 4, GetXtremeColor(COLOR_3DFACE));
				else DrawRectangle(pDC, rcThumb, bSelected, FALSE, TRUE, FALSE, FALSE, FALSE);

				rcThumb.left++; rcThumb.top++;
				CXTPFontDC fontMarlett(pDC, &m_fontMarlettIcon);
				pDC->SetTextColor(GetXtremeColor(bSelected? XPCOLOR_HIGHLIGHT_TEXT: XPCOLOR_TOOLBAR_TEXT));
				pDC->DrawText(_T("u"), rcThumb, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
			}
		}

	}
	return szImage;
}


CSize CXTPOfficeTheme::DrawTearOffGripper(CDC* pDC, CRect rcGripper, BOOL bSelected, BOOL bDraw)
{
	if (bDraw)
	{
		CRect rc(1, 1, rcGripper.right + 1, rcGripper.Height() + 1);
		pDC->FillSolidRect(rc, GetXtremeColor(XPCOLOR_MENUBAR_FACE));
		rc.DeflateRect(1, 1);

		int nLeft = rc.CenterPoint().x - min(16, rcGripper.Width() /2 - 10);
		int nRight = rc.CenterPoint().x + min(16, rcGripper.Width() /2 - 10);

		if (bSelected)
		{
			Rectangle(pDC, rc, XPCOLOR_HIGHLIGHT_BORDER, XPCOLOR_HIGHLIGHT);
			if (nLeft < nRight)
			{
				Line(pDC, nLeft, 4, nRight, 4, XPCOLOR_HIGHLIGHT_TEXT);
				Line(pDC, nLeft, 6, nRight, 6, XPCOLOR_HIGHLIGHT_TEXT);
			}

		} else
		{
			pDC->FillSolidRect(rc, GetXtremeColor(XPCOLOR_TOOLBAR_FACE));

			if (nLeft < nRight)
			{
				Line(pDC, nLeft, 3, nRight, 3, XPCOLOR_TOOLBAR_GRIPPER);
				Line(pDC, nLeft, 5, nRight, 5, XPCOLOR_TOOLBAR_GRIPPER);
				Line(pDC, nLeft, 7, nRight, 7, XPCOLOR_TOOLBAR_GRIPPER);
			}
		}
	}

	return CSize(0, 9);
}


CSize CXTPOfficeTheme::DrawSpecialControl(CDC* pDC, XTPSpecialControl controlType, CXTPControl* pButton, CXTPCommandBar* pBar, BOOL bDraw, LPVOID lpParam)
{
	if (controlType == xtpButtonCustomize)
	{
		CSize szIcon = pButton->GetParent()->GetIconSize();
		CSize szCheck(szIcon.cx + 4, szIcon.cy + 4);
		CSize szImage(max(szIcon.cx + 4, m_nTextHeight + 4), max(szIcon.cy + 6, m_nTextHeight));

		if (!bDraw)
		{
			CSize sz = DrawControlPopupParent(pDC, pButton, FALSE);
			return CSize(sz.cx + szImage.cx, sz.cy);
		}

		CRect rcButton = pButton->GetRect();
		CRect rcText(0, 0, 0, 0), rcShortcut(0, 0, 0, 0);
		BOOL bSelected = pButton->GetSelected(), bEnabled = pButton->GetEnabled(), bChecked = pButton->GetChecked();

		BOOL bLayoutRTL = pButton->GetParent()->IsLayoutRTL();

		CString strCaption = pButton->GetCaption();
		if (bLayoutRTL) UpdateRTLCaption(strCaption);

		pDC->FillSolidRect(szImage.cx + 1, (pButton->m_bDoubleGripper? -3: 0) + rcButton.top, szImage.cx, (pButton->m_bDoubleGripper? 3: 0) + rcButton.Height(), GetXtremeColor(XPCOLOR_TOOLBAR_FACE));

		DrawRectangle(pDC, rcButton, bSelected, FALSE, bEnabled, FALSE, FALSE, FALSE);

		CRect rcImageRect(rcButton.left, rcButton.top, rcButton.left + szImage.cx, rcButton.bottom);
		CRect rcCheck (CPoint(rcImageRect.left + rcImageRect.Width()/2 - szCheck.cx/2, rcImageRect.top + rcImageRect.Height()/2 - szCheck.cy/2), szCheck);

		rcImageRect.OffsetRect(szImage.cx, 0);

		CXTPImageManagerIcon* pImage = pButton->GetImage(szIcon.cx);
		if (pImage)
		{
			szIcon.cy = pImage->GetHeight() * szIcon.cx / pImage->GetWidth();
			CPoint ptImage = CPoint(rcImageRect.left + rcImageRect.Width()/2 - szIcon.cx/2, rcImageRect.top + rcImageRect.Height()/2 - szIcon.cy/2);
			DrawImage(pDC, ptImage, szIcon, pImage, bSelected, FALSE, bEnabled, FALSE, FALSE, FALSE);
		}

		if (bChecked)
		{
			DrawRectangle(pDC, rcCheck, bSelected, FALSE, bEnabled, TRUE, FALSE, FALSE);
			DrawCheckMark(pDC, rcCheck, !bEnabled? GetXtremeColor(XPCOLOR_DISABLED): 0, bLayoutRTL);
		}


		COLORREF clrText = !bEnabled? GetXtremeColor(XPCOLOR_GRAYTEXT): bSelected? GetXtremeColor(XPCOLOR_HIGHLIGHT_TEXT): GetXtremeColor(XPCOLOR_MENUBAR_TEXT);

		pDC->SetTextColor(clrText);
		pDC->SetBkMode (TRANSPARENT);

		rcText.SetRect(rcButton.left + szImage.cx * 2 + 8, rcButton.top, rcButton.right, rcButton.bottom);
		rcShortcut.SetRect(rcButton.left + szImage.cx * 2 + 8, rcButton.top, rcButton.right - 19, rcButton.bottom);

		pDC->DrawText(strCaption, &rcText, DT_SINGLELINE | DT_VCENTER | (!bDraw ? DT_CALCRECT : DT_END_ELLIPSIS) );

		if (pButton->GetShortcutText().GetLength() != 0 )
			pDC->DrawText(pButton->GetShortcutText(), &rcShortcut, DT_SINGLELINE | DT_VCENTER | (!bDraw ? DT_CALCRECT : DT_RIGHT) );

		return 0;
	}
	return CXTPPaintManager::DrawSpecialControl(pDC, controlType, pButton, pBar, bDraw, lpParam);
}
