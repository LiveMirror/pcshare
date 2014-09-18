// GnomeTheme.cpp: implementation of the CGnomeTheme class.
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
#include "GUI_Gnome.h"
#include "GnomeTheme.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGnomeTheme::CGnomeTheme()
{

}

CGnomeTheme::~CGnomeTheme()
{

}

void CGnomeTheme::FillStatusBar(CDC* pDC, CXTPStatusBar* pBar)
{
	pDC->FillSolidRect(CXTPClientRect(pBar), GNOMECOLOR_3DFACE);
}

void CGnomeTheme::FillDockBar(CDC* pDC, CXTPDockBar* pBar)
{
	pDC->FillSolidRect(CXTPClientRect(pBar), GNOMECOLOR_3DFACE);
}

void CGnomeTheme::DrawStatusBarGripper(CDC* pDC, CRect rcClient)
{
	int i;

	CXTPPenDC penBorder(*pDC, GNOMECOLOR_3DSHADOW);
	for (i = 0; i < 4; i++ ) Line(pDC, CPoint(rcClient.Width() - 4 - i * 4, rcClient.Height() - 2),
		CPoint(rcClient.Width() - 1, rcClient.Height() - 5 - i * 4));

	CXTPPenDC penBorderWhite(*pDC, GNOMECOLOR_BTNHIGHLIGHT);
	for (i = 0; i < 4; i++ )
		Line(pDC, CPoint(rcClient.Width() - 5 -i * 4, rcClient.Height() - 2), CPoint(rcClient.Width() - 1 , rcClient.Height() - 4 - i * 4 - 2));
}

void CGnomeTheme::DrawStatusBarPane(CDC* pDC, CRect& rc, DWORD dwStyle, CString str)
{
	if (!(dwStyle & SBPS_DISABLED))
	{
		rc.DeflateRect(2, 1);
		pDC->SetBkMode(TRANSPARENT);
		pDC->DrawText(str, rc, DT_SINGLELINE | DT_VCENTER) ;
	}
}

void CGnomeTheme::FillCommandBarEntry(CDC* pDC, CXTPCommandBar* pBar)
{
	CRect rc;
	pBar->GetClientRect(&rc);
	pDC->FillSolidRect(rc, GNOMECOLOR_3DFACE);

	if (pBar->GetPosition() == xtpBarFloating || pBar->GetPosition() == xtpBarPopup)
	{
		Draw3dRect(pDC, rc, GNOMECOLOR_3DSHADOW, GNOMECOLOR_3DSHADOW);
		rc.DeflateRect(1, 1);
		Draw3dRect(pDC, rc, GNOMECOLOR_BTNHIGHLIGHT, RGB(206, 206, 206));
	}
	else
	{
		Draw3dRect(pDC, rc, RGB(245, 245, 245), RGB(196, 196, 196));
	}

}

CSize CGnomeTheme::DrawCommandBarGripper(CDC* pDC, CXTPCommandBar* pBar, BOOL bDraw)
{
	CRect rc;
	pBar->GetClientRect(&rc);

	CXTPPenDC pen(*pDC, 0);

	if (pBar->GetPosition() == xtpBarFloating)
	{
		ASSERT(pDC);
		CXTPFontDC font(pDC, &m_fontSmCaption);
		CSize sz = pDC->GetTextExtent(_T(" "));

		rc.SetRect(3, 3, rc.right - 3, 3 + max(15, sz.cy));
		if (pDC && bDraw)
		{
			pDC->FillSolidRect(rc, RGB(68, 100, 172));
			pDC->SetTextColor(GetXtremeColor(COLOR_3DHIGHLIGHT));
			pDC->DrawText(pBar->GetTitle(), CRect(5, 3, rc.right - 2* rc.Size().cy, rc.bottom), DT_SINGLELINE | DT_VCENTER );
		}
		return rc.Size();

	}
	else if (pBar->GetPosition() == xtpBarRight || pBar->GetPosition() == xtpBarLeft)
	{
		if (pDC && bDraw)
		{
			for (int i = 4; i < rc.right - 4; i += 5)
			{
				//pen.Color(clrGripper[i % 2]); pDC->MoveTo(i, 3); pDC->LineTo(i, 8);

				pen.Color(GNOMECOLOR_3DSHADOW); pDC->MoveTo(i, 7); pDC->LineTo(i + 4, 3);
				pen.Color(GNOMECOLOR_BTNHIGHLIGHT); pDC->MoveTo(i, 8); pDC->LineTo(i + 4, 4);

			}
		}
		return CSize(0, 8);
	}
	else if (pBar->GetPosition() == xtpBarTop || pBar->GetPosition() == xtpBarBottom)
	{
		if (pDC && bDraw)
		{
			for (int i = 8; i < rc.bottom - 4; i += 5)
			{
				pen.Color(GNOMECOLOR_BTNHIGHLIGHT); pDC->MoveTo(3, i); pDC->LineTo(7, i - 4);
				pen.Color(GNOMECOLOR_3DSHADOW); pDC->MoveTo(3, i-1); pDC->LineTo(7, i - 5);
			}
		}
		return CSize(8, 0);
	}
	return 0;

}


CSize CGnomeTheme::DrawCommandBarSeparator(CDC* pDC, CXTPCommandBar* pBar, CXTPControl* pControl, BOOL bDraw)
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
		Draw3dRect(pDC, CRect(14, rcControl.top - 2 - 2 -1, rc.right - 14, rcControl.top - 0 - 2 -  1), RGB(206, 206, 206), GNOMECOLOR_BTNHIGHLIGHT);
	}
	else if (pBar->GetPosition() != xtpBarPopup && IsVerticalPosition(pBar->GetPosition()))
	{
		if (!pControl->GetWrap())
			pDC->FillSolidRect(rcRaw.left + 4, rcControl.top - 4, rcRaw.Width() - 8, 1, RGB(161, 161, 161));
		else
			pDC->FillSolidRect(rcRaw.right + 2, rcRaw.top + 4, 1,  rcRaw.Height() - 8, RGB(161, 161, 161));
	} else
	{
		if (!pControl->GetWrap())
			pDC->FillSolidRect(rcControl.left - 4, rcRaw.top + 4, 1, rcRaw.Height() - 8, RGB(161, 161, 161));
		else
			pDC->FillSolidRect(rcRaw.left + 4, rcRaw.top - 4, rcRaw.Width() - 8, 1, RGB(161, 161, 161));
	}
	return 0;
}

void CGnomeTheme::DrawRectangle(CDC* pDC, CRect rc, BOOL bSelected, BOOL bPressed, BOOL bEnabled, BOOL bChecked, BOOL bPopuped, BOOL /*bToolBar*/, XTPBarPosition /*barPosition = xtpBarPopup*/)
{
	if ((bSelected || bPressed || bPopuped) && bEnabled)
		pDC->FillSolidRect(rc, RGB(221, 221, 221));

	if (!bEnabled)
	{
		if ( bSelected == TRUE_KEYBOARD) Draw3dRect(pDC, rc, COLOR_3DSHADOW, COLOR_BTNHILIGHT);
	}
	else if (bChecked && !bSelected && !bPressed)
	{
		Draw3dRect(pDC, rc, GNOMECOLOR_3DSHADOW, GNOMECOLOR_BTNHIGHLIGHT);
	}
	else if (bChecked ||bPopuped || bPressed == TRUE_KEYBOARD || (bSelected && bPressed))
	{
		//Draw3dRect(pDC, rc, COLOR_3DSHADOW, COLOR_BTNHILIGHT);
		pDC->FillSolidRect(rc, RGB(204, 204, 204));
		pDC->Draw3dRect(rc, RGB(92, 92, 92), RGB(92, 92, 92));

		rc.DeflateRect(1, 1);
		pDC->Draw3dRect(rc, RGB(204, 204, 204), GNOMECOLOR_BTNHIGHLIGHT);
	}
	else if (bSelected || bPressed)
	{
		pDC->FillSolidRect(rc, RGB(245, 245, 245));
		pDC->Draw3dRect(rc, RGB(92, 92, 92), RGB(92, 92, 92));
		rc.DeflateRect(1, 1);
		pDC->Draw3dRect(rc, GNOMECOLOR_BTNHIGHLIGHT, RGB(206, 206, 206));
	}
}

void CGnomeTheme::AdjustExcludeRect(CRect& /*rc*/, CXTPControl* /*pControl*/)
{
}
