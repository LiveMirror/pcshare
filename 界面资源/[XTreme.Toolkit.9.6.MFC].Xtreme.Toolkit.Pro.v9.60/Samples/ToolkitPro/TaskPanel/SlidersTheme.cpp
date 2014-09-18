// SlidersTheme.cpp: implementation of the CSlidersTheme class.
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
#include "taskpanel.h"
#include "SlidersTheme.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSlidersTheme::CSlidersTheme()
{
	m_rcControlMargins = CRect(0, 0, 0, 0);
	m_nGroupSpacing = 0;

	RefreshMetrics();
}

CSlidersTheme::~CSlidersTheme()
{

}

void CSlidersTheme::RefreshMetrics()
{
	CXTPTaskPanelPaintManagerPlain::RefreshMetrics();

	m_eGripper = xtpTaskPanelGripperTriangle;

	m_clrBackground.SetStandardValue(GetXtremeColor(COLOR_WINDOW));
	m_groupNormal.clrClient = GetXtremeColor(COLOR_WINDOW);
	m_groupNormal.clrClientBorder =GetXtremeColor(COLOR_BTNTEXT);
	m_groupNormal.clrClientLink = GetXtremeColor(COLOR_BTNTEXT);
	m_groupNormal.clrClientLinkHot = GetXtremeColor(COLOR_HIGHLIGHT);

	m_bRoundedFrame = FALSE;

	m_fntCaption.DeleteObject();

	LOGFONT lfIcon;
	::ZeroMemory(&lfIcon, sizeof(LOGFONT));

	VERIFY(::SystemParametersInfo(SPI_GETICONTITLELOGFONT,sizeof(LOGFONT), &lfIcon, 0));

	lfIcon.lfWeight = FW_NORMAL;
	m_fntCaption.CreateFontIndirect(&lfIcon);


}

CRect CSlidersTheme::DrawCaptionGripper(CDC* pDC, CXTPTaskPanelGroup* pGroup, CRect rc)
{

	XTPDrawHelpers()->GradientFill(pDC, rc, RGB(177, 195, 217), RGB(204, 216, 230), FALSE);
	pDC->FillSolidRect(rc.left, rc.top, rc.Width(), 1, RGB(107, 118, 131));
	pDC->FillSolidRect(rc.left, rc.top + 1, rc.Width(), 1, RGB(235, 240, 245));

	pDC->FillSolidRect(rc.left, rc.bottom - 2, rc.Width(), 1, RGB(227, 233, 241));
	pDC->FillSolidRect(rc.left, rc.bottom - 1, rc.Width(), 1, RGB(203, 209, 217));


	for (int i = 0; i < 5; i++)
	{
		pDC->SetPixel(rc.left + 4, rc.CenterPoint().y - 5 + i * 2, RGB(89, 98, 109));
		pDC->SetPixel(rc.left + 5, rc.CenterPoint().y - 5 + i * 2 + 1, RGB(222, 237, 255));

		pDC->SetPixel(rc.left + 4 + 3, rc.CenterPoint().y - 5 + i * 2, RGB(89, 98, 109));
		pDC->SetPixel(rc.left + 5 + 3, rc.CenterPoint().y - 5 + i * 2 + 1, RGB(222, 237, 255));
	}

	if (pGroup->IsExpandable())
	{

		COLORREF clr =  m_groupNormal.clrClientText;
		CPoint pt(rc.left + 14, rc.CenterPoint().y);

		if (pGroup->IsExpanded())
			CXTPDrawHelpers::Triangle(pDC, CPoint(pt.x, pt.y - 2), CPoint(pt.x + 8, pt.y - 2), CPoint(pt.x + 4, pt.y + 4 - 2), clr);
		else
			CXTPDrawHelpers::Triangle(pDC, CPoint(pt.x + 3, pt.y - 4), CPoint(pt.x + 3 + 4, pt.y), CPoint(pt.x + 3, pt.y + 4), clr);

	}
	rc.left += 14;

	return rc;
}


int CSlidersTheme::DrawGroupCaption(CDC* pDC, CXTPTaskPanelGroup* pGroup, BOOL bDraw)
{
	CXTPFontDC fnt(pDC, &m_fntCaption);

	if (bDraw)
	{
		CRect rc = pGroup->GetCaptionRect();

		if (rc.IsRectEmpty())
			return 0;

		XTP_TASKPANEL_GROUPCOLORS& groupColors =  m_groupNormal;

		if (pGroup->IsCaptionVisible())
		{

			//CRect rcText(rc);
			CRect rcText = DrawCaptionGripper(pDC, pGroup, rc);

			rcText.DeflateRect(13, 5, 13, 0);


			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(!pGroup->GetEnabled()? GetXtremeColor(COLOR_GRAYTEXT): groupColors.clrClientText);
			pDC->DrawText(pGroup->GetCaption(), rcText, DT_WORDBREAK|DT_LEFT);

			if (pGroup->IsItemFocused())
			{
				pDC->SetTextColor(groupColors.clrClientText);
				pDC->DrawText(pGroup->GetCaption(), rcText, DT_WORDBREAK|DT_LEFT|DT_CALCRECT);
				pDC->DrawFocusRect(rcText);
			}


		}
		return 0;
	}
	else
	{
		CRect rcText(pGroup->GetTargetClientRect());

		rcText.DeflateRect(13 + 14, 0, 13, 0);
		rcText.top = rcText.bottom = 0;

		pDC->DrawText(pGroup->GetCaption(), rcText, DT_WORDBREAK|DT_LEFT|DT_CALCRECT);
		return rcText.Height() + 5 + 5;
	}
}
