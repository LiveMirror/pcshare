// ModernTheme.cpp: implementation of the CModernTheme class.
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
#include "IntelligentMenus.h"
#include "ModernTheme.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CModernTheme::CModernTheme()
{

}

CModernTheme::~CModernTheme()
{

}

void CModernTheme::_Fill(CDC* pDC, CWnd* pBar)
{
	CXTPDrawHelpers* pDrawHelper = XTPDrawHelpers();
	if (!pDrawHelper->IsFastFillAvailable())
	{
		pDC->FillSolidRect(CXTPClientRect(pBar), GetXtremeColor(COLOR_3DFACE));
		return;
	}

	CWnd* pFrame = pBar->GetParentFrame();
	CRect rc, rcParent, rcClient;
	pFrame->GetWindowRect(rcParent);
	pBar->GetWindowRect(rc);

	pBar->GetClientRect(rcClient);
	rcClient.TopLeft() += rcParent.TopLeft() -rc.TopLeft();
	rcClient.BottomRight() += rcParent.BottomRight() - rc.BottomRight();

	COLORREF clr3DFace = GetXtremeColor(COLOR_3DFACE);

	TRIVERTEX vert[2];
	vert[0].x      = rcClient.left;
	vert[0].y      = rcClient.top;
	vert[0].Red    = (COLOR16)(255 << 8);
	vert[0].Green  = (COLOR16)(255 << 8);
	vert[0].Blue   = (COLOR16)(255 << 8);
	vert[0].Alpha  = 0x0000;

	vert[1].x      = rcClient.right;
	vert[1].y      = rcClient.bottom;
	vert[1].Red    = (COLOR16)(int(0.67 * GetRValue(clr3DFace) + 0.33 *255) << 8);
	vert[1].Green  = (COLOR16)(int(0.67 * GetGValue(clr3DFace) + 0.33 *255) << 8);
	vert[1].Blue   = (COLOR16)(int(0.67 * GetBValue(clr3DFace) + 0.33 *255) << 8);
	vert[1].Alpha  = 0x0000;

	GRADIENT_RECT gRect = { 0, 1 };

	pDrawHelper->GradientFill(*pDC, vert, 2, &gRect, 1, GRADIENT_FILL_RECT_H );


}

void CModernTheme::FillDockBar(CDC* pDC, CXTPDockBar* pBar)
{
	_Fill(pDC, pBar);
}

void CModernTheme::FillCommandBarEntry(CDC* pDC, CXTPCommandBar* pBar)
{
	if (pBar->GetPosition() == xtpBarPopup || pBar->GetPosition() == xtpBarFloating || pBar->GetType() != xtpBarTypeNormal)
	{
		CXTPNativeXPTheme::FillCommandBarEntry(pDC, pBar);
	}
	else
	{
		_Fill(pDC, pBar);
	}

}
