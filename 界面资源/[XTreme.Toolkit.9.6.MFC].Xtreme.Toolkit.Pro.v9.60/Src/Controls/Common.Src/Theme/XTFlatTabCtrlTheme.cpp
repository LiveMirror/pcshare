// XTFlatTabCtrlTheme.cpp: implementation of the CXTFlatTabCtrlTheme class.
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
#include "XTPIntel80Helpers.h"

#include "XTFlatTabCtrlTheme.h"
#include "XTDefines.h"
#include "XTUtil.h"
#include "XTGlobal.h"
#include "XTMemDC.h"
#include "XTVC50Helpers.h"
#include "XTFlatTabCtrl.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


IMPLEMENT_THEME_FACTORY(CXTFlatTabCtrlTheme, CXTFlatTabCtrlTheme, CXTFlatTabCtrlTheme)
//===========================================================================
// ÑXTFlatTabCtrlTheme class
//===========================================================================

CXTFlatTabCtrlTheme::CXTFlatTabCtrlTheme()
{
}

CXTFlatTabCtrlTheme::~CXTFlatTabCtrlTheme()
{

}

void CXTFlatTabCtrlTheme::RefreshMetrics()
{
	XTPColorManager()->RefreshColors();

	m_cy = ::GetSystemMetrics(SM_CYHSCROLL);
	m_cx = ::GetSystemMetrics(SM_CXHSCROLL)-1;

	m_clr3DShadow.SetStandardValue(GetSysColor(COLOR_3DSHADOW));
	m_clr3DHilight.SetStandardValue(GetSysColor(COLOR_3DHILIGHT));
	m_clr3DFace.SetStandardValue(GetSysColor(COLOR_3DFACE));
	m_clrBtnText.SetStandardValue(GetSysColor(COLOR_BTNTEXT));
	m_clrWindow.SetStandardValue(GetSysColor(COLOR_WINDOW));
	m_clrWindowText.SetStandardValue(GetSysColor(COLOR_WINDOWTEXT));
}


int CXTFlatTabCtrlTheme::DrawTab(CDC* pDC, CXTFlatTabCtrl* pTabCtrl, const CPoint& pt, bool bSelected,
	LPCTSTR lpszTabLabel)
{
	// Saves the current state of the device context, we will
	// restore the state when the method looses scope. This will
	// simplify selecting objects.
	const int nSavedDC = pDC->SaveDC();

	const int iHeight = m_cy;
	const int iBase   = iHeight / 2;
	const int iWidth  = iHeight + iHeight / 2; // 2 bases + 2 margins

	const bool bBottom = (pTabCtrl->GetTabStyle() & FTS_XT_BOTTOM) == FTS_XT_BOTTOM;

	pDC->SelectObject(pTabCtrl->GetTabFont(bSelected));
	const CSize szText = pDC->GetTextExtent(lpszTabLabel);

	CRect rcText;
	rcText.left   = pt.x + iBase + iBase / 2;
	rcText.top    = pt.y + ((iHeight - szText.cy)/2)-1;
	rcText.right  = rcText.left + szText.cx;
	rcText.bottom = rcText.top  + szText.cy;

	COLORREF crBack;
	COLORREF crFore;

	if (bSelected)
	{
		crBack = m_clrWindow;
		crFore = m_clrWindowText;
	}
	else
	{
		crBack = m_clr3DFace;
		crFore = m_clrBtnText;
	}

	CPen penBack(PS_SOLID, 1, crBack);
	CPen penWindow(PS_SOLID, 1, m_clr3DHilight);
	CPen penShadow(PS_SOLID, 1, m_clr3DShadow);
	CPen penOutline(PS_SOLID, 1, m_clrBtnText);

	CBrush brush;
	brush.CreateSolidBrush(crBack);

	POINT points[] =
	{
		{ pt.x,                                  pt.y + iHeight - 1 },
		{ pt.x + iBase - 1,                      pt.y               },
		{ pt.x + szText.cx + iWidth - iBase - 1, pt.y               },
		{ pt.x + szText.cx + iWidth - 1,         pt.y + iHeight - 1 }
	};

	// swap vertical coordinates
	if (bBottom)
	{
		points[0].y = points[1].y;
		points[2].y = points[3].y;
		points[1].y = points[2].y;
		points[3].y = points[0].y;
	}

	pDC->SelectObject(&penOutline);
	pDC->SetBkColor(crBack);
	pDC->SelectObject(&brush);
	pDC->Polygon(points, 4);
	pDC->SetTextColor(crFore);
	pDC->DrawText(lpszTabLabel, rcText, DT_CENTER);
	pDC->SelectObject(&penShadow);

	if (bSelected)
	{
		pDC->MoveTo(pt.x + iBase, points[1].y);
		pDC->LineTo(pt.x + iBase * 2 + szText.cx - 1, points[1].y);
		pDC->SelectObject(&penBack);
		pDC->MoveTo(pt.x + 1, points[0].y);
		pDC->LineTo(pt.x + szText.cx + iWidth - 1, points[0].y);
	}
	else
	{
		// highlight line on left
		pDC->SelectObject(&penWindow);
		pDC->MoveTo(pt.x + 1, points[0].y);
		pDC->LineTo(pt.x + 1 + iBase, points[0].y + iHeight);

		// shadow line on top
		pDC->SelectObject(&penShadow);
		pDC->MoveTo(pt.x, points[0].y);
		pDC->LineTo(pt.x + szText.cx + iWidth, points[0].y);

		// shadow line on bottom
		pDC->MoveTo(pt.x + iBase, points[1].y);
		pDC->LineTo(pt.x + szText.cx + iHeight - 1, points[1].y);
	}

	// Cleanup.
	pDC->RestoreDC(nSavedDC);
	brush.DeleteObject();

	return szText.cx + iWidth;
}


void CXTFlatTabCtrlTheme::DrawButton(CDC* pDC, CXTFlatTabCtrl* pTabCtrl, CXTFlatTabCtrlButtonState& button_state)
{
	CRect r(button_state.m_rect);
	if (r.IsRectEmpty()) return;
	r.OffsetRect(0, -pTabCtrl->GetTabsRect().top);
	if (button_state.m_bPressed)
	{
		pDC->Draw3dRect(r, GetXtremeColor(COLOR_3DSHADOW), GetXtremeColor(COLOR_3DHILIGHT));
	}
	else
	{
		pDC->Draw3dRect(r, GetXtremeColor(COLOR_3DHILIGHT), GetXtremeColor(COLOR_3DSHADOW));
	}
	r.DeflateRect(1, 1);
	pDC->FillSolidRect(r, GetXtremeColor(COLOR_3DFACE));

	CPen pen(PS_SOLID, 1, button_state.m_bEnabled? m_clrBtnText: m_clr3DShadow);

	CPen *pOldPen = pDC->SelectObject(&pen);

	int direction = 1 - ((button_state.m_IconType & 0xFF00) >> 8);
	int x = (direction < 0)? r.right : r.left, y;

	const int arrow_cy = r.Width() / 2 + 2;
	const int arrow_cx = (arrow_cy + 1) / 2 + 1;
	const int top      = r.top + (r.Height() - arrow_cy) / 2;

	x += (((r.Width() - arrow_cx) / 2) * direction);
	y = top;

	// draw arrow
	int height = arrow_cy;
	while (height > 0)
	{
		pDC->MoveTo(x, y);
		pDC->LineTo(x, y + height);
		y += 1;
		x += direction;
		height -= 2;
	}

	if (button_state.m_IconType & 0xFF)
	{
		x += (1 * direction);
		pDC->MoveTo(x, top);
		pDC->LineTo(x, top + arrow_cy);
	}

	pDC->SelectObject(pOldPen);
}
