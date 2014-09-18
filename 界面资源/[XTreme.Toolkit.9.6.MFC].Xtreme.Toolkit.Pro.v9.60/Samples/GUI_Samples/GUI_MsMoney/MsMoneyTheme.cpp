// MsMoneyTheme.cpp: implementation of the CMsMoneyTheme class.
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
#include "GUI_MsMoney.h"
#include "MsMoneyTheme.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMsMoneyTheme::CMsMoneyTheme(COLORREF clrGradientLight, COLORREF clrGradientDark, COLORREF clrLineLight, COLORREF clrLineDark)
{
	m_clrGradientLight = clrGradientLight;
	m_clrGradientDark = clrGradientDark;
	m_clrLineLight = clrLineLight;
	m_clrLineDark = clrLineDark;

}

CMsMoneyTheme::~CMsMoneyTheme()
{

}


void CMsMoneyTheme::FillCommandBarEntry(CDC* pDC, CXTPCommandBar* pBar)
{
	CXTPClientRect rc(pBar);

	if (pBar->GetType() == xtpBarTypeMenuBar)
	{
		pDC->FillSolidRect(rc, RGB(91, 91, 91));
	}
	else if (pBar->GetType() == xtpBarTypeNormal)
	{
		GradientFill(pDC, rc, m_clrGradientLight, m_clrGradientDark, FALSE);

		CXTPPenDC pen(*pDC, 0);

		for (int i = rc.top + 1; i < rc.bottom; i+= 3)
		{
			pDC->FillSolidRect(rc.left, i, rc.Width(), 1, XTPDrawHelpers()->BlendColors(m_clrLineLight, m_clrLineDark, (float)(1.0 - ((i - rc.top)) / (float)rc.Height())));

		}

	}
	else if (pBar->GetType() == xtpBarTypePopup)
	{
		pDC->FillSolidRect(rc, RGB(91, 91, 91));
		pDC->Draw3dRect(rc, RGB(68, 68, 68), RGB(68, 68, 68));

	}

}

CSize CMsMoneyTheme::DrawControlPopupParent(CDC* pDC, CXTPControl* pButton, BOOL bDraw)
{
	CRect rcButton = pButton->GetRect();
	XTPControlType controlType = pButton->GetType();
	CRect rcText(0, 0, 0, 0), rcShortcut(0, 0, 0, 0);
	BOOL bSelected = pButton->GetSelected(), bEnabled = pButton->GetEnabled(), bChecked = pButton->GetChecked();

	ASSERT(controlType == xtpControlButton || controlType == xtpControlPopup); // implemented only.

	if (bDraw)
	{
		CRect rcSelection(rcButton.left, rcButton.top, rcButton.right, rcButton.bottom);


		COLORREF clrText = !bEnabled? RGB(145, 145, 145): bSelected? RGB(255, 223, 127): RGB(255, 254, 249);
		pDC->SetTextColor(clrText);
		pDC->SetBkMode (TRANSPARENT);
		if (bSelected) pDC->FillSolidRect(rcSelection, RGB(80, 80, 80));

		if (bChecked)
		{
			CRect rcCheck (CPoint(rcButton.left - 2, rcButton.top + rcButton.Height()/2 - 10), CSize(20, 20));
			DrawCheckMark(pDC, rcCheck, clrText);
		}

		if (controlType == xtpControlPopup)
		{
			CPoint pt = CPoint(rcButton.right - 11, rcButton.CenterPoint().y);
			Triangle(pDC, CPoint(pt.x, pt.y - 3), CPoint(pt.x + 3, pt.y), CPoint(pt.x, pt.y + 3), clrText);
		}

		rcText.SetRect(rcButton.left + 16, rcButton.top, rcButton.right, rcButton.bottom);
		rcShortcut.SetRect(rcButton.left + 16 + 4, rcButton.top, rcButton.right - 19, rcButton.bottom);

	}

	pDC->DrawText(pButton->GetCaption(), &rcText, DT_SINGLELINE | DT_VCENTER | (!bDraw ? DT_CALCRECT : DT_END_ELLIPSIS) );

	if (controlType == xtpControlButton && pButton->GetShortcutText().GetLength() != 0 )
		pDC->DrawText(pButton->GetShortcutText(), &rcShortcut, DT_SINGLELINE | DT_VCENTER | (!bDraw ? DT_CALCRECT : DT_RIGHT) );

	return CSize(16 + 12 + rcText.Width() +  rcShortcut.Width() + 32, max(17, m_nTextHeight - 4));
}


CSize CMsMoneyTheme::DrawCommandBarSeparator(CDC* pDC, CXTPCommandBar* pBar, CXTPControl* pControl, BOOL bDraw)
{
	if (!bDraw)
	{
		ASSERT(pBar->GetType() == xtpBarTypePopup);
		return CSize(0, 3);
	}

	CRect rc;
	pBar->GetClientRect(&rc);
	CRect rcControl = pControl->GetRect();

	if (pBar->GetType() == xtpBarTypePopup)
	{
		CXTPPenDC pen(*pDC, RGB(112, 112, 112));
		Line(pDC, CPoint(16, rcControl.top - 1), CPoint(rc.right - 14, rcControl.top - 1));
	}
	return 0;
}


CSize CMsMoneyTheme::DrawControlToolBarParent(CDC* pDC, CXTPControl* pButton, BOOL bDraw)
{
	CRect rcButton = bDraw? pButton->GetRect(): CXTPEmptyRect();
	XTPControlType controlType = pButton->GetType();

	CSize szIcon = pButton->GetParent()->GetIconSize();
	CSize szButton = CSize(szIcon.cx  + 7, szIcon.cy  + 7);

	BOOL bVert = pButton->GetParent()->GetPosition() == xtpBarRight || pButton->GetParent()->GetPosition() == xtpBarLeft;

	BOOL bPressed = pButton->GetPressed(), bSelected = pButton->GetSelected(), bEnabled = pButton->GetEnabled(), bChecked = pButton->GetChecked(),
		bPopuped = IsPopupControlType(controlType) && ((CXTPControlPopup*)pButton)->GetPopup();

	if (bDraw)
	{
		pDC->SetTextColor(!bEnabled? RGB(177, 177, 177): bSelected||bPressed? RGB(255, 223, 127): RGB(255, 254, 249));
		pDC->SetBkMode (TRANSPARENT);

	}

	switch(controlType)
	{
	case xtpControlPopup:
		{
			if (bDraw && (bSelected || bPopuped) && pButton->GetParent()->GetPosition() != xtpBarPopup)
			{
				pDC->FillSolidRect(rcButton, RGB(80, 80, 80));
			}


			CSize sz = DrawTextEx(pDC, pButton->GetCaption(), rcButton, bDraw, bVert, TRUE, FALSE, FALSE);
			return !bVert? CSize(sz.cx + 16, sz.cy + 8): CSize(sz.cx + 8, sz.cy + 16);
		}
	case xtpControlSplitButtonPopup:
		{
			ASSERT(FALSE); // not implemented.
			return 0;
		}
	case xtpControlButtonPopup:
		return szButton;

	case xtpControlButton:
		{
			CXTPFontDC font (pDC, &m_fontRegular);

			if (bDraw)
			{

				CRect rcText = rcButton;
				rcText.top = rcButton.top + szIcon.cy;

				DrawTextEx(pDC, pButton->GetCaption(), rcText, TRUE, FALSE, TRUE, FALSE, FALSE);

				if (pButton->GetIconId() != 0)
				{
					CPoint pt = CPoint(rcButton.CenterPoint().x - szIcon.cx / 2, rcButton.top + 3);
					CXTPImageManagerIcon* pImage = XTPImageManager()->GetImage(pButton->GetIconId(), szIcon.cx);
					DrawImage(pDC, pt, CSize(szIcon.cx, 0), pImage, bSelected, bPressed, bEnabled, bChecked, FALSE);
				}
			}
			else
			{
				CString strCaption = pButton->GetCaption();
				CSize szText = DrawTextEx(pDC, strCaption.GetLength() > 0? strCaption: _T(" ") , CXTPEmptyRect(), FALSE, FALSE, TRUE, FALSE, FALSE);
				szButton.cy = 10 + szIcon.cy + szText.cy;
				szButton.cx = max(szButton.cx, szText.cx + 10);
			}

			return szButton;
		}
	}
	return 0;
}


void CMsMoneyTheme::AdjustExcludeRect(CRect& rc, CXTPControl* pControl)
{
	CXTPCommandBar* pParent = pControl->GetParent();
	ASSERT(pParent);

	if (pControl->GetType() == xtpControlComboBox) return;

	if (pParent->GetType() != xtpBarTypePopup)
	{
		if (IsVerticalPosition(pParent->GetPosition()))
			rc.InflateRect(0, 0); else rc.InflateRect(0, 0);
	}
	else rc.DeflateRect(0, 0);
}

CRect CMsMoneyTheme::GetCommandBarBorders(CXTPCommandBar* pBar)
{
	if (pBar->GetType() == xtpBarTypePopup) return CRect(1, 1, 1, 1);
	if (pBar->GetType() == xtpBarTypeNormal) return CRect(0, 5, 0, 5);
	return CXTPEmptyRect();
}

CSize CMsMoneyTheme::DrawCommandBarGripper(CDC* pDC, CXTPCommandBar* pBar, BOOL bDraw)
{
	if (pBar->GetPosition() == xtpBarFloating)
	{
		CXTPClientRect rc(pBar);
		ASSERT(pDC);
		CXTPFontDC font(pDC, &m_fontSmCaption);
		CSize sz = pDC->GetTextExtent(_T(" "));

		rc.SetRect(3, 3, rc.right - 3, 3 + max(15, sz.cy));
		if (pDC && bDraw)
		{
			pDC->FillSolidRect(rc, pBar->GetType() == xtpBarTypeMenuBar? RGB(80, 80, 80): m_clrGradientDark);
			pDC->SetTextColor(GetXtremeColor(COLOR_3DHIGHLIGHT));
			pDC->DrawText(pBar->GetTitle(), CRect(5, 3, rc.right - 2* rc.Size().cy, rc.bottom), DT_SINGLELINE | DT_VCENTER );
		}
		return rc.Size();
		//return CXTPDefaultTheme::DrawCommandBarGripper(pDC, pBar, bDraw);
	}

	return 0;
}

void CMsMoneyTheme::DrawImage(CDC* pDC, CPoint pt, CSize /*sz*/, CXTPImageManagerIcon* pImage, BOOL bSelected, BOOL bPressed, BOOL bEnabled, BOOL bChecked, BOOL bPopuped, BOOL  )
{
	if (!bEnabled)
	{
		pDC->DrawState (pt, 0, pImage->GetDisabledIcon(), (UINT)DSS_NORMAL, HBRUSH(0));
	}
	else if (bPopuped || bChecked) pDC->DrawState(pt, 0,  bChecked && (bSelected || bPressed)? pImage->GetHotIcon(): pImage->GetIcon(), (UINT)DSS_NORMAL, HBRUSH(0));
	else
	{
		pDC->DrawState(pt, 0,  !bSelected && !bPressed? pImage->GetIcon(): pImage->GetHotIcon(), DSS_NORMAL, HBRUSH(0));
	}
}



CSize CMsMoneyTheme::DrawSpecialControl(CDC* pDC, XTPSpecialControl controlType, CXTPControl* pButton, CXTPCommandBar* pBar, BOOL bDraw, LPVOID lpParam)
{
	if (controlType == xtpButtonExpandToolbar)
	{
		if (!bDraw)
		{
			CSize szBar = *(CSize*)lpParam;
			if (!IsVerticalPosition(pBar->GetPosition()))
				pButton->SetRect(CRect(szBar.cx - (11 + 2), 2, szBar.cx - 2, szBar.cy - 2));
			else
				pButton->SetRect(CRect(2, szBar.cy - (11 + 2), szBar.cx - 2, szBar.cy - 2));

		}
		else
		{
			COLORREF clrText = pButton->GetSelected()||pButton->GetPressed()? RGB(255, 223, 127): RGB(255, 254, 249);
			CXTPPenDC pen(*pDC, clrText);
			BOOL bHiddenExists = *(BOOL*)lpParam;
			CRect rcButton = pButton->GetRect();
			if (!IsVerticalPosition(pBar->GetPosition()))
			{
				CPoint pt = CPoint(rcButton.left + rcButton.Width()/2, rcButton.bottom - 6);
				Triangle(pDC, CPoint(pt.x -2 , pt.y - 1), CPoint(pt.x + 2, pt.y - 1), CPoint (pt.x, pt.y + 1), clrText);
				if (bHiddenExists)
				{
					pDC->MoveTo(rcButton.left + 2, rcButton.top + 4); pDC->LineTo(rcButton.left + 4, rcButton.top + 6);     pDC->LineTo(rcButton.left + 1, rcButton.top + 9);
					pDC->MoveTo(rcButton.left + 3, rcButton.top + 4); pDC->LineTo(rcButton.left + 5, rcButton.top + 6);     pDC->LineTo(rcButton.left + 2, rcButton.top + 9);

					pDC->MoveTo(rcButton.left + 6, rcButton.top + 4); pDC->LineTo(rcButton.left + 8, rcButton.top + 6);     pDC->LineTo(rcButton.left + 5, rcButton.top + 9);
					pDC->MoveTo(rcButton.left + 7, rcButton.top + 4); pDC->LineTo(rcButton.left + 9, rcButton.top + 6);     pDC->LineTo(rcButton.left + 6, rcButton.top + 9);
				}
			} else
			{
				CPoint pt = CPoint(rcButton.left + 4, rcButton.top + 5);
				Triangle(pDC, CPoint(pt.x +2 , pt.y - 2), CPoint(pt.x + 2, pt.y + 2), CPoint (pt.x, pt.y), clrText);

				if (bHiddenExists)
				{
					pDC->MoveTo(rcButton.right - 8, rcButton.top + 2); pDC->LineTo(rcButton.right - 6, rcButton.top + 4);   pDC->LineTo(rcButton.right - 3, rcButton.top + 1);
					pDC->MoveTo(rcButton.right - 8, rcButton.top + 3); pDC->LineTo(rcButton.right - 6, rcButton.top + 5);   pDC->LineTo(rcButton.right - 3, rcButton.top + 2);

					pDC->MoveTo(rcButton.right - 8, rcButton.top + 6); pDC->LineTo(rcButton.right - 6, rcButton.top + 8);   pDC->LineTo(rcButton.right - 3, rcButton.top + 5);
					pDC->MoveTo(rcButton.right - 8, rcButton.top + 7); pDC->LineTo(rcButton.right - 6, rcButton.top + 9);   pDC->LineTo(rcButton.right - 3, rcButton.top + 6);
				}

			}
		}
	}
	else return CXTPDefaultTheme::DrawSpecialControl(pDC, controlType, pButton, pBar, bDraw, lpParam);

	return 0;
}
