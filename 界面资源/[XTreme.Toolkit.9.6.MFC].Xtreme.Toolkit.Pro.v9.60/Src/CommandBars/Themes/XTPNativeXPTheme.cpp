// XTPNativeXPTheme.cpp : implementation of the CXTPNativeXPTheme class.
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
#include "Tmschema.h"
#include "XTPImageManager.h"
#include "XTPDrawHelpers.h"

#include "XTPPaintManager.h"
#include "XTPControlButton.h"
#include "XTPControlPopup.h"
#include "XTPColorManager.h"
#include "XTPToolBar.h"
#include "XTPPopupBar.h"
#include "XTPControlComboBox.h"
#include "XTPDockBar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace XTPPaintThemes;


CXTPNativeXPTheme::CXTPNativeXPTheme()
{
	m_iconsInfo.bUseFadedIcons = TRUE;
	m_iconsInfo.bIconsWithShadow = FALSE;

	m_nPopupBarText = COLOR_MENUTEXT;

	RefreshMetrics();
}
CXTPNativeXPTheme::~CXTPNativeXPTheme()
{
}

void CXTPNativeXPTheme::RefreshMetrics()
{
	CXTPDefaultTheme::RefreshMetrics();

	HWND hWnd = AfxGetMainWnd()? AfxGetMainWnd()->GetSafeHwnd(): 0;
	m_themeToolbar.OpenThemeData(hWnd, L"TOOLBAR");
	m_themeRebar.OpenThemeData(hWnd, L"REBAR");
	m_themeCombo.OpenThemeData(hWnd, L"COMBOBOX");
	m_themeStatusBar.OpenThemeData(hWnd, L"STATUS");

	if (!m_themeRebar.IsAppThemed() || FAILED(m_themeRebar.GetThemeColor(RP_BAND, 0, TMT_EDGESHADOWCOLOR, &m_clrEdgeShadowColor)))
		m_clrEdgeShadowColor = GetXtremeColor(COLOR_3DSHADOW);

	COLORREF clr;
	if (m_themeToolbar.IsAppThemed() && SUCCEEDED(m_themeToolbar.GetThemeColor(0, 0, TMT_TEXTCOLOR, &clr)))
		m_arrColor[COLOR_BTNTEXT].SetStandardValue(clr);

	if (m_themeRebar.IsAppThemed() && SUCCEEDED(m_themeRebar.GetThemeColor(0, 0, TMT_FILLCOLORHINT, &clr)))
		m_arrColor[COLOR_3DFACE].SetStandardValue(clr);

	if (m_themeStatusBar.IsAppThemed())
		m_themeStatusBar.GetThemeColor(SP_PANE, 0, TMT_TEXTCOLOR, &m_clrStatusTextColor);

}

void CXTPNativeXPTheme::DrawRectangle(CDC* pDC, CRect rc, BOOL bSelected, BOOL bPressed, BOOL bEnabled , BOOL bChecked , BOOL bPopuped, BOOL bToolBar, XTPBarPosition barPosition /*= xtpBarPopup*/)
{
	if (!m_themeToolbar.IsAppThemed())
	{
		CXTPDefaultTheme::DrawRectangle(pDC, rc, bSelected, bPressed, bEnabled , bChecked , bPopuped, bToolBar, barPosition);
		return;
	}

	int nState = !bEnabled? (bChecked && !bToolBar? TS_PRESSED: TS_DISABLED)    :
		bPopuped? TS_PRESSED:
		bChecked && !bPressed? (bSelected? TS_HOTCHECKED: TS_CHECKED):
		bSelected && bPressed || bPressed == TRUE_KEYBOARD? TS_PRESSED:
		bSelected ? TS_HOT: TS_NORMAL;

	m_themeToolbar.DrawThemeBackground(pDC->GetSafeHdc(), TP_BUTTON, nState, &rc, 0);

}
void CXTPNativeXPTheme::FillCommandBarEntry(CDC* pDC, CXTPCommandBar* pBar)
{
	if (!m_themeRebar.IsAppThemed())
	{
		CXTPDefaultTheme::FillCommandBarEntry(pDC, pBar);
		return;
	}
	CRect rc;
	pBar->GetClientRect(&rc);

	if (pBar->GetPosition() == xtpBarPopup)
	{
		Rectangle(pDC, rc, COLOR_3DSHADOW, COLOR_MENU);
	}
	else if (pBar->GetPosition() == xtpBarFloating)
	{
		pDC->FillSolidRect(rc, GetXtremeColor(COLOR_3DFACE));
		Draw3dRect(pDC, rc, COLOR_3DFACE, 0);
		rc.DeflateRect(1, 1);
		Draw3dRect(pDC, rc, COLOR_BTNHILIGHT, COLOR_3DSHADOW);
	}
	else
	{
		if (pBar->GetPosition() == xtpBarTop)
			FillDockBarRect(pDC, pBar, pBar->GetParent());
		else pDC->FillSolidRect(rc, GetXtremeColor(COLOR_3DFACE));

		pDC->Draw3dRect(rc, GetXtremeColor(COLOR_BTNHILIGHT), m_clrEdgeShadowColor);

	}


}
void CXTPNativeXPTheme::FillDockBarRect(CDC* pDC, CWnd* pWnd, CWnd* pParent)
{
	if(m_themeRebar.IsAppThemed())
	{
		CRect rc, rcParent, rcClient, rcClipRect;
		pWnd->GetClientRect(&rcClient);
		rcClipRect = rcClient;

		if (pParent)
		{
			pParent->GetWindowRect(rcParent);
			pWnd->GetWindowRect(rc);

			rcClient.TopLeft() += rcParent.TopLeft() - rc.TopLeft();
			rcClient.BottomRight() += rcParent.BottomRight() - rc.BottomRight();
		}

		m_themeRebar.DrawThemeBackground(pDC->GetSafeHdc(), 0, 0, &rcClient, &rcClipRect);
	}
	else
		pDC->FillSolidRect(CXTPClientRect(pWnd), GetXtremeColor(COLOR_3DFACE));
}

void CXTPNativeXPTheme::FillDockBar(CDC* pDC, CXTPDockBar* pBar)
{
	if (pBar->GetPosition() == xtpBarTop)
		FillDockBarRect(pDC, pBar, pBar);
	else pDC->FillSolidRect(CXTPClientRect(pBar), GetXtremeColor(COLOR_3DFACE));

}
void CXTPNativeXPTheme::FillStatusBar(CDC* pDC, CStatusBar* pBar)
{
	if (!m_themeStatusBar.IsAppThemed())
	{
		CXTPDefaultTheme::FillStatusBar(pDC, pBar);
		return;
	}
	m_themeStatusBar.DrawThemeBackground(pDC->GetSafeHdc(), 0, 0, CXTPClientRect(pBar), 0);
}

void CXTPNativeXPTheme::DrawStatusBarPaneBorder(CDC* pDC, CRect rc, BOOL bGripperPane)
{
	if (!m_themeStatusBar.IsAppThemed())
	{
		CXTPDefaultTheme::DrawStatusBarPaneBorder(pDC, rc, bGripperPane);
		return;
	}
	m_themeStatusBar.DrawThemeBackground(pDC->GetSafeHdc(), bGripperPane? SP_GRIPPERPANE: SP_PANE, 0, rc, 0);
}

CSize CXTPNativeXPTheme::DrawDialogBarGripper(CDC* pDC, CXTPCommandBar* /*pBar*/, BOOL bDraw)
{
	CSize sz(8, max(22, m_nTextHeight + 4));

	if (pDC && bDraw)
	{
		CRect rc(2, 3, 8, sz.cy);
		m_themeRebar.DrawThemeBackground(pDC->GetSafeHdc(), RP_GRIPPER , 1, &rc, 0);
	}

	return sz;
}

CSize CXTPNativeXPTheme::DrawCommandBarGripper(CDC* pDC, CXTPCommandBar* pBar, BOOL bDraw )
{
	if (!m_themeRebar.IsAppThemed())
	{
		return CXTPDefaultTheme::DrawCommandBarGripper(pDC, pBar, bDraw);
	}
	if (pBar->IsDialogBar())
		return DrawDialogBarGripper(pDC, pBar, bDraw);

	CRect rc;
	pBar->GetClientRect(&rc);

	if (pBar->GetPosition() == xtpBarFloating)
	{
		return CXTPDefaultTheme::DrawCommandBarGripper(pDC, pBar, bDraw);
	}
	else if (pBar->GetPosition() == xtpBarRight || pBar->GetPosition() == xtpBarLeft)
	{
		if (pDC && bDraw)
		{
			rc.top += 2;
			rc.bottom = rc.top + 6;
			m_themeRebar.DrawThemeBackground(pDC->GetSafeHdc(), RP_GRIPPERVERT , 1, &rc, 0);
		}
		return CSize(0, 8);
	}
	else if (pBar->GetPosition() == xtpBarTop || pBar->GetPosition() == xtpBarBottom)
	{
		if (pDC && bDraw)
		{
			rc.left += 2;
			rc.right = rc.left + 6;
			m_themeRebar.DrawThemeBackground(pDC->GetSafeHdc(), RP_GRIPPER , 1, &rc, 0);
		}
		return CSize(8, 0);
	}
	return 0;

}

void CXTPNativeXPTheme::AdjustExcludeRect(CRect& rc, CXTPControl* pControl, BOOL bVertical)
{
	if (!m_themeRebar.IsAppThemed())
	{
		CXTPDefaultTheme::AdjustExcludeRect(rc, pControl, bVertical);
		return;
	}
	CXTPCommandBar* pParent = pControl->GetParent();
	ASSERT(pParent);

	if (pControl->GetType() == xtpControlComboBox) return;

	if (pParent->GetType() != xtpBarTypePopup)
	{
		if (bVertical)
			rc.DeflateRect(0, 0); else rc.DeflateRect(0, 0);
	} else rc.InflateRect(-3, 3);

}

void CXTPNativeXPTheme::DrawSplitButtonFrame(CDC* pDC, CXTPControl* pButton, CRect rcButton)
{
	if (!m_themeToolbar.IsAppThemed())
	{
		CXTPDefaultTheme::DrawSplitButtonFrame(pDC, pButton, rcButton);
		return;
	}

	BOOL bControlPopup = DYNAMIC_DOWNCAST(CXTPControlPopup, pButton) != NULL;

	BOOL bPressed = pButton->GetPressed(), bSelected = pButton->GetSelected(), bEnabled = pButton->GetEnabled(), bChecked = pButton->GetChecked(),
		bPopuped = bControlPopup && ((CXTPControlPopup*)pButton)->GetPopup();

	UINT nState = !bEnabled? TS_DISABLED:
		bPressed && bSelected? TS_PRESSED:
		bChecked? TS_CHECKED:
		bPopuped || bSelected? TS_HOT: TS_NORMAL;

	CRect rc(rcButton.left, rcButton.top, rcButton.right - 12, rcButton.bottom);
	m_themeToolbar.DrawThemeBackground(pDC->GetSafeHdc(), TP_SPLITBUTTON, nState, &rc, 0);


	nState = bPopuped? TS_PRESSED: nState;

	CRect rcArrow(rcButton.right - 12, rcButton.top, rcButton.right, rcButton.bottom);
	m_themeToolbar.DrawThemeBackground(pDC->GetSafeHdc(), TP_SPLITBUTTONDROPDOWN, nState, &rcArrow, 0);

	pDC->SetTextColor(GetXtremeColor(!bEnabled? COLOR_GRAYTEXT: COLOR_BTNTEXT));
	pDC->SetBkMode (TRANSPARENT);

}

void CXTPNativeXPTheme::DrawControlEditFrame(CDC* pDC, CRect rc, BOOL bEnabled, BOOL bSelected)
{
	if (!m_themeCombo.IsAppThemed())
	{
		CXTPDefaultTheme::DrawControlEditFrame(pDC, rc, bEnabled, bSelected);
		return;
	}

	int nStateID = !bEnabled? CBXS_DISABLED: CBXS_NORMAL;

	COLORREF clrFrame = GetXtremeColor(COLOR_3DSHADOW);
	m_themeCombo.GetThemeColor(CP_DROPDOWNBUTTON, nStateID, TMT_BORDERCOLOR, &clrFrame);

	Draw3dRect(pDC, rc, clrFrame, clrFrame);
}

void CXTPNativeXPTheme::DrawControlComboBoxButton(CDC* pDC, CRect rcBtn, BOOL bEnabled, BOOL bSelected, BOOL bDropped)
{
	if (!m_themeCombo.IsAppThemed())
	{
		CXTPDefaultTheme::DrawControlComboBoxButton(pDC, rcBtn, bEnabled, bSelected, bDropped);
		return;
	}

	int nStateID = !bEnabled? CBXS_DISABLED:
		bDropped? CBXS_PRESSED: bSelected? CBXS_HOT: CBXS_NORMAL;

	rcBtn.DeflateRect(-2, 1, 1, 1);
	m_themeCombo.DrawThemeBackground(pDC->GetSafeHdc(), CP_DROPDOWNBUTTON, nStateID, &rcBtn, 0);

}

void CXTPNativeXPTheme::DrawStatusBarGripper(CDC* pDC, CRect rcClient)
{
	if (m_themeStatusBar.IsAppThemed())
	{
		CRect rcGripper(rcClient.right - 18, rcClient.top, rcClient.right, rcClient.bottom);
		m_themeStatusBar.DrawThemeBackground(pDC->GetSafeHdc(), SP_GRIPPER, 0, &rcGripper, 0);
	}
	else
	{
		CXTPDefaultTheme::DrawStatusBarGripper(pDC, rcClient);

	}
}
