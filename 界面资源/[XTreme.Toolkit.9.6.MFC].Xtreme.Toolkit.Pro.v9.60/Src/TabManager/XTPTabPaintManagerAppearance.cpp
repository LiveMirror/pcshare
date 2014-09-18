// XTPTabPaintManager.cpp: implementation of the CXTPTabPaintManager class.
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
#include "XTPDrawHelpers.h"
#include "XTPColorManager.h"

#include "XTPTabPaintManager.h"
#include "XTPTabManager.h"
#include "XTPTabPaintManager.inl"

#ifndef _countof
#define _countof(array) (sizeof(array)/sizeof(array[0]))
#endif

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif



//////////////////////////////////////////////////////////////////////////
// CAppearanceSet

CXTPTabPaintManager::CAppearanceSet::CAppearanceSet()
{
	m_pPaintManager = NULL;
	m_rcHeaderMargin.SetRect(0, 0, 0, 0);
}
CXTPTabPaintManager::CAppearanceSet::~CAppearanceSet()
{
}


void CXTPTabPaintManager::CAppearanceSet::RefreshMetrics()
{
	CWindowDC dc (NULL);
	CXTPFontDC fnt(&dc, m_pPaintManager->GetFont());
	CSize szRect = dc.GetTextExtent(_T(" "));

	m_nButtonHeight = max(18, szRect.cy + 3);
}

int CXTPTabPaintManager::CAppearanceSet::GetButtonHeight(CXTPTabManager* pTabManager)
{
	CSize szButtonMin(0);

	int nMargin = m_pPaintManager->m_rcButtonMargin.top + m_pPaintManager->m_rcButtonMargin.bottom;

	if (pTabManager) pTabManager->GetItemMetrics(NULL, &szButtonMin, NULL);
	int nButtonHeight = max(m_nButtonHeight + nMargin, szButtonMin.cy);

	if (!m_pPaintManager->m_bShowIcons)
		return nButtonHeight;

	CSize szIcon(m_pPaintManager->m_szIcon);
	if (pTabManager) pTabManager->DrawIcon(NULL, 0, NULL, FALSE, szIcon);

	return max(max(szIcon.cx, szIcon.cy) + 4 + nMargin, nButtonHeight);

}


void CXTPTabPaintManager::CAppearanceSet::DrawPolyLine(CDC* pDC, COLORREF clr, LPPOINT pts, int nCount)
{
	CXTPPenDC pen(*pDC, clr);
	pDC->MoveTo(pts[0].x, pts[0].y);
	CPoint pt(pts[0]);

	for (int i = 1; i < nCount; i++)
	{
		pt.Offset(pts[i]);
		pDC->LineTo(pt);
	}
}

void CXTPTabPaintManager::CAppearanceSet::InflateRectEx(CRect& rc, CRect rcInflate, XTPTabPosition position)
{
	switch (position)
	{
		case xtpTabPositionTop: rc.top -= rcInflate.top; rc.left -= rcInflate.left; rc.bottom += rcInflate.bottom; rc.right += rcInflate.right; break;
		case xtpTabPositionLeft: rc.left -= rcInflate.top; rc.bottom += rcInflate.right; rc.right += rcInflate.bottom;  rc.top -= rcInflate.left; break;
		case xtpTabPositionBottom: rc.bottom += rcInflate.top; rc.right += rcInflate.right; rc.top -= rcInflate.bottom; rc.left -= rcInflate.left; break;
		case xtpTabPositionRight: rc.right += rcInflate.top; rc.top -= rcInflate.left; rc.left -= rcInflate.bottom; rc.bottom += rcInflate.right; break;

	}
}

void CXTPTabPaintManager::CAppearanceSet::DeflateRectEx(CRect& rc, CRect rcDeflate, XTPTabPosition position)
{
	InflateRectEx(rc, CRect(-rcDeflate.left, -rcDeflate.top, -rcDeflate.right, -rcDeflate.bottom), position);
}

void CXTPTabPaintManager::CAppearanceSet::DrawSingleLineBorder(CDC* pDC, CRect rc, XTPTabPosition position, COLORREF clrTopLeft, COLORREF clrBottomRight)
{
	switch(position)
	{
		case xtpTabPositionTop: HorizontalLine(pDC, rc.left, rc.top, rc.Width(), clrTopLeft); break;
		case xtpTabPositionLeft: VerticalLine(pDC, rc.left, rc.top, rc.Height(), clrTopLeft); break;
		case xtpTabPositionBottom: HorizontalLine(pDC, rc.left, rc.bottom - 1, rc.Width(), clrBottomRight); break;
		case xtpTabPositionRight: VerticalLine(pDC, rc.right - 1, rc.top, rc.Height(), clrBottomRight); break;
	}
}


COLORREF CXTPTabPaintManager::CAppearanceSet::FillButton(CDC* pDC, CXTPTabManagerItem* pItem, LPPOINT pts, int nCount, XTPTabPosition /*tabPosition*/, BOOL bAbsoulute)
{
	LPPOINT lpPointRgn = new POINT[nCount];
	CPoint pt(0);
	for (int i = 0; i < nCount; i++)
	{
		if (!bAbsoulute)
		{
			pt.Offset(pts[i]);
			lpPointRgn[i] = pt;
		}
		else
		{
			lpPointRgn[i] = pts[i];
		}
	}

	CRect rcClip;
	pDC->GetClipBox(rcClip);
	CRgn rgnClip;
	rgnClip.CreateRectRgnIndirect(rcClip);


	CRgn rgn;
	rgn.CreatePolygonRgn(lpPointRgn, nCount, ALTERNATE);

	pDC->SelectClipRgn(&rgn, RGN_AND);

	CRect rcFill;
	rgn.GetRgnBox(rcFill);

	CBitmap bmp;
	bmp.CreateCompatibleBitmap(pDC, rcFill.Width(), rcFill.Height());
	CXTPCompatibleDC dcButton(pDC, &bmp);

	COLORREF clrFill = GetColorSet()->FillPropertyButton(&dcButton, CRect(0, 0, rcFill.Width(), rcFill.Height()), pItem);

	pDC->BitBlt(rcFill.left, rcFill.top, rcFill.Width(), rcFill.Height(), &dcButton, 0, 0, SRCCOPY);

	pDC->SelectClipRgn(&rgnClip);
	delete[] lpPointRgn;

	return clrFill;
}

int CXTPTabPaintManager::CAppearanceSet::GetButtonLength(CDC* pDC, CXTPTabManagerItem* pItem)
{
	return m_pPaintManager->DrawSingleButtonIconAndText(pDC, pItem, CXTPEmptyRect(), FALSE);
}

CRect CXTPTabPaintManager::CAppearanceSet::GetHeaderRect(CRect rcControl, CXTPTabManager* pTabManager)
{
	CRect rcHeader(rcControl);
	int nMargin = m_rcHeaderMargin.top + m_rcHeaderMargin.bottom;
	switch (pTabManager->GetPosition())
	{
		case xtpTabPositionTop: rcHeader.bottom = rcControl.top + GetButtonHeight(pTabManager) + nMargin; break;
		case xtpTabPositionLeft: rcHeader.right = rcControl.left + GetButtonHeight(pTabManager) + nMargin; break;
		case xtpTabPositionBottom: rcHeader.top = rcControl.bottom  - GetButtonHeight(pTabManager) - nMargin; break;
		case xtpTabPositionRight: rcHeader.left = rcControl.right - GetButtonHeight(pTabManager) - nMargin; break;
	}
	return rcHeader;
}
CRect CXTPTabPaintManager::CAppearanceSet::GetClientRect(CRect rcControl, CXTPTabManager* pTabManager)
{
	CRect rcClient(rcControl);
	CRect rcHeader = GetHeaderRect(rcControl, pTabManager);

	switch (pTabManager->GetPosition())
	{
		case xtpTabPositionTop: rcClient.top = rcHeader.bottom - 1; break;
		case xtpTabPositionLeft: rcClient.left = rcHeader.right - 1; break;
		case xtpTabPositionBottom: rcClient.bottom = rcHeader.top + 1; break;
		case xtpTabPositionRight: rcClient.right = rcHeader.left + 1; break;
	}
	return rcClient;
}

CRect CXTPTabPaintManager::CAppearanceSet::GetClientMargin()
{
	return m_pPaintManager->m_rcClientMargin;
}


void CXTPTabPaintManager::CAppearanceSet::AdjustClientRect(CXTPTabManager* pTabManager, CRect& rcClient)
{
	DeflateRectEx(rcClient, m_pPaintManager->m_rcControlMargin, pTabManager->GetPosition());
	rcClient = GetClientRect(rcClient, pTabManager);

	DeflateRectEx(rcClient, GetClientMargin(), pTabManager->GetPosition());

	if (m_pPaintManager->m_clientFrame == xtpTabFrameBorder)
	{
		DeflateRectEx(rcClient, CRect(2, 2, 2, 2), pTabManager->GetPosition());
	}
	if (m_pPaintManager->m_clientFrame == xtpTabFrameSingleLine)
	{
		DeflateRectEx(rcClient, CRect(0, 2, 0, 0), pTabManager->GetPosition());
	}
}

void CXTPTabPaintManager::CAppearanceSet::DrawWorkspacePart(CDC* pDC, LPRECT lpRect, XTPTabWorkspacePart workspacePart)
{
	if (!m_pPaintManager->m_bStaticFrame)
	{

		switch (workspacePart)
		{
		case xtpTabWorkspacePartBorder:
			break;

		case xtpTabWorkspacePartWidth:
			lpRect->top -= 2;
			lpRect->left -= 2;
			lpRect->right += 2;
			lpRect->bottom += 2;
			break;

		case xtpTabWorkspacePartVSplitter:
			pDC->FillSolidRect(lpRect, GetColorSet()->m_clrHeaderFace.clrLight);
			break;

		case xtpTabWorkspacePartHSplitter:
			pDC->FillSolidRect(lpRect, GetColorSet()->m_clrHeaderFace.clrLight);
			break;
		}
	}
	else
	{

		switch (workspacePart)
		{
		case xtpTabWorkspacePartBorder:
			pDC->Draw3dRect(1, 0, lpRect->right - lpRect->left - 2, lpRect->bottom - lpRect->top - 1, GetXtremeColor(COLOR_3DFACE), GetXtremeColor(COLOR_3DFACE));
			pDC->Draw3dRect(0, 0, lpRect->right - lpRect->left, lpRect->bottom - lpRect->top, GetColorSet()->m_clrFrameBorder, GetColorSet()->m_clrFrameBorder);
			break;

		case xtpTabWorkspacePartWidth:
			lpRect->top -= 1;
			break;

		case xtpTabWorkspacePartVSplitter:
			pDC->FillSolidRect(lpRect, GetColorSet()->m_clrControlFace);
			pDC->FillSolidRect(lpRect->right - 1, lpRect->top, 1, lpRect->bottom - lpRect->top, GetColorSet()->m_clrFrameBorder);
			break;

		case xtpTabWorkspacePartHSplitter:
			pDC->FillSolidRect(lpRect, GetColorSet()->m_clrControlFace);
			pDC->FillSolidRect(lpRect->left, lpRect->bottom - 1, lpRect->right - lpRect->left, 1, GetColorSet()->m_clrFrameBorder);
			break;
		}
	}
}


//////////////////////////////////////////////////////////////////////////
// CAppearanceSetVisualStudio

void CXTPTabPaintManager::CAppearanceSetVisualStudio::DrawSingleButton(CDC* pDC, CXTPTabManagerItem* pItem)
{
	CRect rcItem = pItem->GetRect();

	if (!pItem->IsSelected())
	{
		if (!IsHorizontalTabPosition(pItem->GetTabManager()->GetPosition()))
		{
			HorizontalLine(pDC, rcItem.left + 2, rcItem.bottom, rcItem.Width() - 4, GetColorSet()->m_clrFrameBorder);
		}
		else
		{
			VerticalLine(pDC, rcItem.right, rcItem.top + 2, rcItem.Height() - 4, GetColorSet()->m_clrFrameBorder);
		}
	}
	else
	{
		GetColorSet()->FillPropertyButton(pDC, rcItem, pItem);

		InflateRectEx(rcItem, CRect(0, 0, 0, 1), pItem->GetTabManager()->GetPosition());
		pDC->Draw3dRect(rcItem, GetColorSet()->m_clrHighlight, GetColorSet()->m_clrSelectedText);
	}

	m_pPaintManager->DrawSingleButtonIconAndText(pDC, pItem, pItem->GetRect(), TRUE);
}




CRect CXTPTabPaintManager::CAppearanceSetVisualStudio::FillTabControl(CXTPTabManager* pTabManager, CDC* pDC, CRect rcControl)
{
	CRect rcHeader = GetHeaderRect(rcControl, pTabManager);
	GetColorSet()->FillHeader(pDC, rcHeader, pTabManager);

	CRect rcClient = GetClientRect(rcControl, pTabManager);

	if (m_pPaintManager->m_clientFrame == xtpTabFrameBorder)
	{
		GetColorSet()->FillClient(pDC, rcClient, pTabManager);
		pDC->Draw3dRect(rcClient, GetColorSet()->m_clrHighlight, GetColorSet()->m_clrSelectedText);
	}
	if (m_pPaintManager->m_clientFrame == xtpTabFrameSingleLine)
	{
		DrawSingleLineBorder(pDC, rcClient, pTabManager->GetPosition(), GetColorSet()->m_clrHighlight, GetColorSet()->m_clrSelectedText);
	}

	return rcHeader;
}




//////////////////////////////////////////////////////////////////////////
// CAppearanceSetPropertyPage

void CXTPTabPaintManager::CAppearanceSetPropertyPage::RefreshMetrics()
{
	CAppearanceSet::RefreshMetrics();

}


CRect CXTPTabPaintManager::CAppearanceSetPropertyPage::FillTabControl(CXTPTabManager* pTabManager, CDC* pDC, CRect rcControl)
{
	CRect rcHeader = GetHeaderRect(rcControl, pTabManager);
	InflateRectEx(rcHeader, CRect(0, 0, 0, 1), pTabManager->GetPosition());

	GetColorSet()->FillHeader(pDC, rcHeader, pTabManager);


	CRect rcClient = GetClientRect(rcControl, pTabManager);
	if (m_pPaintManager->m_clientFrame == xtpTabFrameBorder)
	{
		GetColorSet()->FillClient(pDC, rcClient, pTabManager);
		Draw3dRect(pDC, rcClient, GetColorSet()->m_csPropertyPage.clrHighlight, GetColorSet()->m_csPropertyPage.clrDarkShadow);

		HorizontalLine(pDC, rcClient.left + 1, rcClient.bottom - 2, rcClient.Width() - 2, GetColorSet()->m_csPropertyPage.clrShadow);
		VerticalLine(pDC, rcClient.right - 2, rcClient.top + 1, rcClient.Height() - 2, GetColorSet()->m_csPropertyPage.clrShadow);
	}
	if (m_pPaintManager->m_clientFrame == xtpTabFrameSingleLine)
	{
		switch(pTabManager->GetPosition())
		{
			case xtpTabPositionTop: HorizontalLine(pDC, rcClient.left, rcClient.top, rcClient.Width(), GetColorSet()->m_csPropertyPage.clrHighlight); break;
			case xtpTabPositionLeft: VerticalLine(pDC, rcClient.left, rcClient.top, rcClient.Height(), GetColorSet()->m_csPropertyPage.clrHighlight); break;
			case xtpTabPositionBottom:
				HorizontalLine(pDC, rcClient.left, rcClient.bottom - 1, rcClient.Width(), GetColorSet()->m_csPropertyPage.clrDarkShadow);
				HorizontalLine(pDC, rcClient.left, rcClient.bottom - 2, rcClient.Width(), GetColorSet()->m_csPropertyPage.clrShadow);
				break;
			case xtpTabPositionRight:
				VerticalLine(pDC, rcClient.right - 1, rcClient.top, rcClient.Height(), GetColorSet()->m_csPropertyPage.clrDarkShadow);
				VerticalLine(pDC, rcClient.right - 2, rcClient.top, rcClient.Height(), GetColorSet()->m_csPropertyPage.clrShadow);
				break;
		}

	}


	return rcHeader;
}

CRect CXTPTabPaintManager::CAppearanceSetPropertyPage::GetClientMargin()
{
	CRect rc = m_pPaintManager->m_rcClientMargin;

	if (m_pPaintManager->GetColorSet()->IsAppThemed() && m_pPaintManager->m_clientFrame == xtpTabFrameBorder)
	{
		if (m_pPaintManager->GetPosition() == xtpTabPositionTop ||
			m_pPaintManager->GetPosition() == xtpTabPositionLeft)
		{
			rc.bottom += 2;
		}
		rc.right += 2;
	}

	return rc;
}

void CXTPTabPaintManager::CAppearanceSetPropertyPage::DrawSingleButton(CDC* pDC, CXTPTabManagerItem* pItem)
{
	CRect rc = pItem->GetRect();

	COLORREF clrShadow = GetColorSet()->m_csPropertyPage.clrShadow, clrHighlight = GetColorSet()->m_csPropertyPage.clrHighlight, clrDarkShadow = GetColorSet()->m_csPropertyPage.clrDarkShadow;

	switch(pItem->GetTabManager()->GetPosition())
	{
	case xtpTabPositionTop:
		{
			if (pItem->IsSelected())
				rc.InflateRect(2, 2, 2, 0);

			COLORREF clr = GetColorSet()->FillPropertyButton(pDC, CRect(rc.left + 1, rc.top + 1, rc.right - 1, rc.bottom - 1), pItem);

			VerticalLine(pDC, rc.left, rc.top + 2, rc.Height() - 2, clrHighlight);
			Pixel(pDC, rc.left + 1, rc.top + 1, clrHighlight);
			HorizontalLine(pDC, rc.left + 2, rc.top, rc.Width() - 4,  clrHighlight);

			Pixel(pDC, rc.right - 2 , rc.top + 1, clrDarkShadow);

			VerticalLine(pDC, rc.right - 1, rc.top + 2, rc.Height() - 3, clrDarkShadow);
			VerticalLine(pDC, rc.right - 2, rc.top + 2, rc.Height() - 3, clrShadow);

			if (pItem->IsSelected())
				HorizontalLine(pDC, rc.left + 1, rc.bottom - 1, rc.Width() - 3, clr);
		}
		break;

	case xtpTabPositionLeft:
		{
			if (pItem->IsSelected())
				rc.InflateRect(2, 2, 0, 2);

			COLORREF clr = GetColorSet()->FillPropertyButton(pDC, CRect(rc.left + 1, rc.top + 1, rc.right - 1, rc.bottom - 1), pItem);

			HorizontalLine(pDC, rc.left + 2, rc.top, rc.Width() - 2, clrHighlight);
			Pixel(pDC, rc.left + 1, rc.top + 1, clrHighlight);

			VerticalLine(pDC, rc.left, rc.top + 2, rc.Height() - 4,  clrHighlight);

			HorizontalLine(pDC, rc.left + 1, rc.bottom - 2, rc.Width() - 2, clrShadow);
			HorizontalLine(pDC, rc.left + 2, rc.bottom - 1, rc.Width() - 3, clrDarkShadow);

			Pixel(pDC, rc.left + 2, rc.bottom - 2, clrDarkShadow);
			Pixel(pDC, rc.left + 2, rc.bottom - 1, clrShadow);

			if (pItem->IsSelected())
				VerticalLine(pDC, rc.right - 1, rc.top + 1, rc.Height() - 3, clr);
		}
		break;

	case xtpTabPositionBottom:
		{
			if (pItem->IsSelected())
				rc.InflateRect(2, 2);

			COLORREF clr = GetColorSet()->FillPropertyButton(pDC, CRect(rc.left + 1, rc.top + 1, rc.right - 1, rc.bottom - 1), pItem);

			VerticalLine(pDC, rc.left, rc.top + 1, rc.Height() - 2, clrHighlight);
			Pixel(pDC, rc.left + 1, rc.bottom - 1, clrHighlight);

			HorizontalLine(pDC, rc.left + 2, rc.bottom -1, rc.Width() - 4,  clrShadow);
			HorizontalLine(pDC, rc.left + 2, rc.bottom, rc.Width() - 4,  clrDarkShadow);

			Pixel(pDC, rc.right - 2, rc.bottom - 1, clrDarkShadow);

			VerticalLine(pDC, rc.right - 1, rc.top, rc.Height() - 1, clrDarkShadow);
			VerticalLine(pDC, rc.right - 2, rc.top, rc.Height() - 1, clrShadow);

			if (pItem->IsSelected())
			{
				HorizontalLine(pDC, rc.left + 1, rc.top + 1, rc.Width() - 1,  clr);
				rc.bottom += 2;
			}
		}
		break;

	case xtpTabPositionRight:
		{
			if (pItem->IsSelected())
				rc.InflateRect(2, 2);

			COLORREF clr = GetColorSet()->FillPropertyButton(pDC, CRect(rc.left + 1, rc.top + 1, rc.right - 1, rc.bottom - 1), pItem);

			HorizontalLine(pDC, rc.left + 1, rc.top, rc.Width() - 2, clrHighlight);
			Pixel(pDC, rc.right - 1, rc.top + 1, clrHighlight);

			VerticalLine(pDC, rc.right - 1, rc.top + 2, rc.Height() - 4,  clrShadow);
			VerticalLine(pDC, rc.right - 0, rc.top + 2, rc.Height() - 4,  clrDarkShadow);

			Pixel(pDC, rc.right - 1, rc.bottom - 2, clrDarkShadow);

			HorizontalLine(pDC, rc.left + 1, rc.bottom - 2, rc.Width() - 2, clrShadow);
			HorizontalLine(pDC, rc.left + 1, rc.bottom - 1, rc.Width() - 2, clrDarkShadow);

			if (pItem->IsSelected())
			{
				VerticalLine(pDC, rc.left + 1, rc.top + 1, rc.Height() - 1,  clr);
				rc.right += 2;
			}
		}
		break;


	}


	m_pPaintManager->DrawSingleButtonIconAndText(pDC, pItem, rc, TRUE);
}

//////////////////////////////////////////////////////////////////////////
//
void CXTPTabPaintManager::CAppearanceSetPropertyPageSelected::DrawSingleButton(CDC* pDC, CXTPTabManagerItem* pItem)
{
	if (pItem->IsSelected() || pItem->IsHighlighted())
	{
		CAppearanceSetPropertyPage::DrawSingleButton(pDC, pItem);
	} else
	{
		m_pPaintManager->DrawSingleButtonIconAndText(pDC, pItem, pItem->GetRect(), TRUE);
	}
}



//////////////////////////////////////////////////////////////////////////
// CAppearanceSetStateButtons

CRect CXTPTabPaintManager::CAppearanceSetStateButtons::FillTabControl(CXTPTabManager* pTabManager, CDC* /*pDC*/, CRect rcControl)
{
	return GetHeaderRect(rcControl, pTabManager);
}


void CXTPTabPaintManager::CAppearanceSetStateButtons::DrawSingleButton(CDC* pDC, CXTPTabManagerItem* pItem)
{
	CRect rcItem = pItem->GetRect();

	GetColorSet()->FillStateButton(pDC, rcItem, pItem);

	m_pPaintManager->DrawSingleButtonIconAndText(pDC, pItem, pItem->GetRect(), TRUE);
}


//////////////////////////////////////////////////////////////////////////
// CAppearanceSetPropertyPage2003

void CXTPTabPaintManager::CAppearanceSetPropertyPage2003::RefreshMetrics()
{
	CAppearanceSet::RefreshMetrics();


}


CRect CXTPTabPaintManager::CAppearanceSetPropertyPage2003::FillTabControl(CXTPTabManager* pTabManager, CDC* pDC, CRect rcControl)
{

	CRect rcHeader = GetHeaderRect(rcControl, pTabManager);

	GetColorSet()->FillHeader(pDC, rcHeader, pTabManager);

	CRect rcClient = GetClientRect(rcControl, pTabManager);
	if (m_pPaintManager->m_clientFrame == xtpTabFrameBorder)
	{
		GetColorSet()->FillClient(pDC, rcClient, pTabManager);
		pDC->Draw3dRect(rcClient, GetColorSet()->m_csPropertyPage2003.clrFrameBorder, GetColorSet()->m_csPropertyPage2003.clrFrameBorder);
	}
	if (m_pPaintManager->m_clientFrame == xtpTabFrameSingleLine)
	{
		DrawSingleLineBorder(pDC, rcClient, pTabManager->GetPosition(), GetColorSet()->m_csPropertyPage2003.clrFrameBorder, GetColorSet()->m_csPropertyPage2003.clrFrameBorder);
	}

	return rcHeader;
}



void CXTPTabPaintManager::CAppearanceSetPropertyPage2003::DrawSingleButton(CDC* pDC, CXTPTabManagerItem* pItem)
{
	CRect rc = pItem->GetRect();

	COLORREF clrBorder = GetColorSet()->m_csPropertyPage2003.clrDarkShadow, clrLeftHighlight = GetColorSet()->m_csPropertyPage2003.clrBorderLeftHighlight, clrTopHighlight = GetColorSet()->m_csPropertyPage2003.clrBorderTopHighlight;
	if (pItem->IsSelected())
	{
		clrBorder = GetColorSet()->m_csPropertyPage2003.clrFrameBorder, clrLeftHighlight = RGB(255, 255, 255), clrTopHighlight = RGB(255, 255, 255);
	}

	if (pItem->IsHighlighted() && m_bDoubleHighlightedBorder)
	{
		clrLeftHighlight = clrTopHighlight = clrBorder = GetColorSet()->m_csStateButton.clrBorder.clrDark;
	}


	switch(pItem->GetTabManager()->GetPosition())
	{
	case xtpTabPositionTop:
		{
			rc.bottom --;

			POINT ptsFill[] = {{rc.right - 1, rc.bottom}, {0, 2 - rc.Height()}, {5 - rc.Width(), 0}, {3 - rc.Height(), rc.Height() - 3}};
			COLORREF clrDark = FillButton(pDC, pItem, ptsFill, sizeof(ptsFill) / sizeof(ptsFill[0]), pItem->GetTabManager()->GetPosition());

			POINT ptsBorder[] = {{rc.right, rc.bottom - 1}, {+0, 3 - rc.Height()}, {-2, -2}, {9 - rc.Width(), 0}, {-5, 2}, {3 - rc.Height(), rc.Height() - 3}, {-2, 0}};
			DrawPolyLine(pDC, clrBorder, ptsBorder, sizeof(ptsBorder) / sizeof(ptsBorder[0]));

			POINT ptsLeftHighlight[] = {{rc.right - 1, rc.bottom - 1}, {+0, 3 - rc.Height()}, {-2, -2}};
			DrawPolyLine(pDC, clrLeftHighlight, ptsLeftHighlight, sizeof(ptsLeftHighlight) / sizeof(ptsLeftHighlight[0]));

			POINT ptsTopHighlight[] = {{rc.right - 3, rc.top + 1}, {9 - rc.Width(), 0}, {-5, +3}, {4 - rc.Height(), rc.Height() - 4}};
			DrawPolyLine(pDC, clrTopHighlight, ptsTopHighlight, sizeof(ptsTopHighlight) / sizeof(ptsTopHighlight[0]));

			if (pItem->IsSelected())
				HorizontalLine(pDC, rc.left - rc.Height() + 4, rc.bottom, rc.Width() + rc.Height() - 3, clrDark);
		}
		break;

	case xtpTabPositionLeft:
		{
			rc.right --;

			POINT ptsFill[] = {{rc.right, rc.bottom - 1}, {2 - rc.Width(), 0}, {0, 5 - rc.Height()}, {rc.Width() - 2, 2 - rc.Width()}};
			COLORREF clrDark = FillButton(pDC, pItem, ptsFill, sizeof(ptsFill) / sizeof(ptsFill[0]), pItem->GetTabManager()->GetPosition());

			POINT ptsBorder[] = {{rc.right - 1, rc.bottom}, {3 - rc.Width(), 0}, {-2, -2}, {0, 9 - rc.Height()}, {2, -5}, {rc.Width() - 3, 3 - rc.Width()}, {0, -2}};
			DrawPolyLine(pDC, clrBorder, ptsBorder, sizeof(ptsBorder) / sizeof(ptsBorder[0]));

			POINT ptsLeftHighlight[] = {{rc.right - 1, rc.bottom - 1}, {3 - rc.Width(), 0}, {-2, -2}};
			DrawPolyLine(pDC, clrLeftHighlight, ptsLeftHighlight, sizeof(ptsLeftHighlight) / sizeof(ptsLeftHighlight[0]));

			POINT ptsTopHighlight[] = {{rc.left + 1, rc.bottom - 3}, {0, 9 - rc.Height()}, {+3, -5}, {rc.Width() - 4, 4 - rc.Width()}};
			DrawPolyLine(pDC, clrTopHighlight, ptsTopHighlight, sizeof(ptsTopHighlight) / sizeof(ptsTopHighlight[0]));

			if (pItem->IsSelected())
				VerticalLine(pDC, rc.right, rc.top - rc.Width() + 4, rc.Width() + rc.Height() - 3, clrDark);
		}

		break;

	case xtpTabPositionBottom:
		{
			POINT ptsFill[] = {{rc.right - 1, rc.top}, {0, rc.Height() - 1}, {5 - rc.Width(), 0}, {3 - rc.Height(), 2 - rc.Height()}};
			COLORREF clrDark = FillButton(pDC, pItem, ptsFill, sizeof(ptsFill) / sizeof(ptsFill[0]), pItem->GetTabManager()->GetPosition());

			POINT ptsBorder[] = {{rc.right, rc.top + 1}, {+0, rc.Height() - 3}, {-2, 2}, {9 - rc.Width(), 0}, {-5, -2}, {3 - rc.Height(), 3 - rc.Height()}, {-2, 0}};
			DrawPolyLine(pDC, clrBorder, ptsBorder, sizeof(ptsBorder) / sizeof(ptsBorder[0]));

			POINT ptsLeftHighlight[] = {{rc.right - 1, rc.top + 1}, {+0, rc.Height() - 3}, {-2, 2}};
			DrawPolyLine(pDC, clrLeftHighlight, ptsLeftHighlight, sizeof(ptsLeftHighlight) / sizeof(ptsLeftHighlight[0]));

			POINT ptsTopHighlight[] = {{rc.right - 3, rc.bottom - 1}, {9 - rc.Width(), 0}, {-5, -3}, {4 - rc.Height(), 4 - rc.Height()}};
			DrawPolyLine(pDC, clrTopHighlight, ptsTopHighlight, sizeof(ptsTopHighlight) / sizeof(ptsTopHighlight[0]));

			if (pItem->IsSelected())
				HorizontalLine(pDC, rc.left - rc.Height() + 4, rc.top, rc.Width() + rc.Height() - 3, clrDark);

		}
		break;

	case xtpTabPositionRight:
		{
			POINT ptsFill[] = {{rc.left + 1, rc.bottom - 1}, {rc.Width() - 2, 0}, {0, 5 - rc.Height()}, {2 - rc.Width(), 2 - rc.Width()}};
			//POINT ptsFill[] = {{rc.left, rc.bottom - 1}, {rc.Width() - 1, 0}, {0, 5 - rc.Height()}, {2 - rc.Width(), 2 - rc.Width()}};
			COLORREF clrDark = FillButton(pDC, pItem, ptsFill, sizeof(ptsFill) / sizeof(ptsFill[0]), pItem->GetTabManager()->GetPosition());

			POINT ptsBorder[] = {{rc.left + 1, rc.bottom}, {rc.Width() -3,  0}, {2, -2}, {0, 9 - rc.Height()}, {-2, -5}, {3 - rc.Width(), 3 - rc.Width()}, {0, -2}};
			DrawPolyLine(pDC, clrBorder, ptsBorder, sizeof(ptsBorder) / sizeof(ptsBorder[0]));

			POINT ptsLeftHighlight[] = {{rc.left + 1, rc.bottom - 1}, {rc.Width() - 3, 0}, {2, -2}};
			DrawPolyLine(pDC, clrLeftHighlight, ptsLeftHighlight, sizeof(ptsLeftHighlight) / sizeof(ptsLeftHighlight[0]));

			POINT ptsTopHighlight[] = {{rc.right - 1, rc.bottom - 3}, {0, 9 - rc.Height()}, {-3, -5}, {4 - rc.Width(), 4 - rc.Width()}};
			DrawPolyLine(pDC, clrTopHighlight, ptsTopHighlight, sizeof(ptsTopHighlight) / sizeof(ptsTopHighlight[0]));

			if (pItem->IsSelected())
				VerticalLine(pDC, rc.left, rc.top - rc.Width() + 4, rc.Width() + rc.Height() - 3, clrDark);


		}
		break;


	}


	m_pPaintManager->DrawSingleButtonIconAndText(pDC, pItem, pItem->GetRect(), TRUE);
}



//////////////////////////////////////////////////////////////////////////
// CAppearanceSetFlat

void CXTPTabPaintManager::CAppearanceSetFlat::DrawSingleButton(CDC* pDC, CXTPTabManagerItem* pItem)
{
	CRect rcItem(pItem->GetRect());

	InflateRectEx(rcItem, CRect(0, 0, 1, 0), pItem->GetTabManager()->GetPosition());

	GetColorSet()->FillPropertyButton(pDC, rcItem, pItem);

	if (pItem->IsSelected())
		InflateRectEx(rcItem, CRect(0, 0, 0, 1), pItem->GetTabManager()->GetPosition());

	pDC->Draw3dRect(rcItem, GetColorSet()->m_clrFrameBorder, GetColorSet()->m_clrFrameBorder);


	m_pPaintManager->DrawSingleButtonIconAndText(pDC, pItem, pItem->GetRect(), TRUE);
}


CRect CXTPTabPaintManager::CAppearanceSetFlat::FillTabControl(CXTPTabManager* pTabManager, CDC* pDC, CRect rcControl)
{

	CRect rcHeader = GetHeaderRect(rcControl, pTabManager);
	GetColorSet()->FillHeader(pDC, rcHeader, pTabManager);

	CRect rcClient = GetClientRect(rcControl, pTabManager);

	if (m_pPaintManager->m_clientFrame == xtpTabFrameBorder)
	{
		GetColorSet()->FillClient(pDC, rcClient, pTabManager);
		pDC->Draw3dRect(rcClient, GetColorSet()->m_clrFrameBorder, GetColorSet()->m_clrFrameBorder);
	}
	if (m_pPaintManager->m_clientFrame == xtpTabFrameSingleLine)
	{
		DrawSingleLineBorder(pDC, rcClient, pTabManager->GetPosition(), GetColorSet()->m_clrFrameBorder, GetColorSet()->m_clrFrameBorder);
	}


	return rcHeader;
}

//////////////////////////////////////////////////////////////////////////
// CAppearanceSetPropertyPageFlat

void CXTPTabPaintManager::CAppearanceSetPropertyPageFlat::DrawSingleButton(CDC* pDC, CXTPTabManagerItem* pItem)
{
	CRect rcItem(pItem->GetRect());
	XTPTabPosition tabPosition = pItem->GetTabManager()->GetPosition();

	InflateRectEx(rcItem, CRect(0, 0, 1, 0), tabPosition);

	CRect rcTab(rcItem);

	CRect rcClip;
	pDC->GetClipBox(rcClip);
	CRgn rgn;
	rgn.CreateRectRgnIndirect(rcClip);
	InflateRectEx(rcTab, CRect(0, 0, 0, 1), tabPosition);
	XTPDrawHelpers()->ExcludeCorners(pDC, rcTab);

	CRect rcEntry(rcItem);
	InflateRectEx(rcEntry, CRect(-1, -1, -1, 0), tabPosition);
	GetColorSet()->FillPropertyButton(pDC, rcEntry, pItem);

	if (m_bBlurPoints)
	{
		if (pItem->IsSelected())
			InflateRectEx(rcItem, CRect(0, 0, 0, 1), tabPosition);
		pDC->Draw3dRect(rcItem, GetColorSet()->m_clrFrameBorder, GetColorSet()->m_clrFrameBorder);
	} else
	{
		if (!pItem->IsSelected())
			DrawSingleLineBorder(pDC, rcItem, XTPTabPosition((tabPosition + 2) % 4), GetColorSet()->m_clrFrameBorder, GetColorSet()->m_clrFrameBorder);
		InflateRectEx(rcItem, CRect(0, 0, 0, 2), tabPosition);

		CXTPPenDC pen(*pDC, GetColorSet()->m_clrFrameBorder);
		CGdiObject* pOldBrush = pDC->SelectStockObject(NULL_BRUSH);
		pDC->RoundRect(rcItem, CPoint(6, 6));
		pDC->SelectObject(pOldBrush);
	}

	pDC->SelectClipRgn(&rgn);

	if (m_bBlurPoints)
	switch(tabPosition)
	{
	case xtpTabPositionTop:
		{
			POINT pts[] = {
				{rcTab.left, rcTab.top + 1}, {0, -1}, {rcTab.left + 1, rcTab.top}, {-1, 0},
				{rcTab.right - 2, rcTab.top}, {+1, 0}, {rcTab.right - 1, rcTab.top + 1}, {0, -1},
				{rcTab.left + 1, rcTab.top + 1}, {-1, 0}, {rcTab.right - 2, rcTab.top + 1}, {+1, 0},
				{rcTab.left + 2, rcTab.top + 1}, {-1, 0}, {rcTab.left + 1, rcTab.top +2}, {0, -1},
				{rcTab.right - 3, rcTab.top + 1}, {+1, 0}, {rcTab.right - 2, rcTab.top + 2}, {0, -1},
			};
			XTPDrawHelpers()->BlurPoints(pDC, pts, _countof(pts));
		}
		break;

	case xtpTabPositionLeft:
		{
			POINT pts[] = {
				{rcTab.left, rcTab.top + 1}, {0, -1}, {rcTab.left + 1, rcTab.top}, {-1, 0},
				{rcTab.left, rcTab.bottom - 2}, {0, +1}, {rcTab.left + 1, rcTab.bottom}, {-1, 0},
				{rcTab.left + 1, rcTab.top + 1}, {-1, 0}, {rcTab.left + 1, rcTab.bottom - 2}, {-1, 0},
				{rcTab.left + 2, rcTab.top + 1}, {-1, 0}, {rcTab.left + 1, rcTab.top +2}, {0, -1},
				{rcTab.left + 2, rcTab.bottom - 2}, {-1, 0}, {rcTab.left + 1, rcTab.bottom - 3}, {0, +1},
			};
			XTPDrawHelpers()->BlurPoints(pDC, pts, _countof(pts));
		}
		break;

	case xtpTabPositionBottom:
		{
			POINT pts[] = {
				{rcTab.left, rcTab.bottom - 2}, {0, +1}, {rcTab.left + 1, rcTab.bottom -1}, {-1, 0},
				{rcTab.right - 1, rcTab.bottom - 2}, {0, +1}, {rcTab.right - 2, rcTab.bottom - 1}, {+1, 0},
				{rcTab.left + 1, rcTab.bottom - 2}, {0, +1}, {rcTab.right - 2, rcTab.bottom - 2}, {+1, 0},
				{rcTab.left + 2, rcTab.bottom - 2}, {-1, 0}, {rcTab.left + 1, rcTab.bottom - 3}, {0, +1},
				{rcTab.right - 3, rcTab.bottom - 2}, {+1, 0}, {rcTab.right - 2, rcTab.bottom - 3}, {0, +1},
			};
			XTPDrawHelpers()->BlurPoints(pDC, pts, _countof(pts));
		}
		break;

	case xtpTabPositionRight:
		{
			POINT pts[] = {
				{rcTab.right - 1, rcTab.top + 1}, {0, -1}, {rcTab.right - 2, rcTab.top}, {+1, 0},
				{rcTab.right - 1, rcTab.bottom - 2}, {0, +1}, {rcTab.right - 2, rcTab.bottom}, {+1, 0},
				{rcTab.right - 2, rcTab.top + 1}, {+1, 0}, {rcTab.right - 2, rcTab.bottom - 2}, {+1, 0},
				{rcTab.right - 3, rcTab.top + 1}, {+1, 0}, {rcTab.right - 2, rcTab.top +2}, {0, -1},
				{rcTab.right - 3, rcTab.bottom - 2}, {+1, 0}, {rcTab.right - 2, rcTab.bottom - 3}, {0, +1},
			};
			XTPDrawHelpers()->BlurPoints(pDC, pts, _countof(pts));
		}
		break;
	}


	m_pPaintManager->DrawSingleButtonIconAndText(pDC, pItem, pItem->GetRect(), TRUE);
}


//////////////////////////////////////////////////////////////////////////
// CAppearanceExcel

void CXTPTabPaintManager::CAppearanceSetExcel::DrawSingleButton(CDC* pDC, CXTPTabManagerItem* pItem)
{
	CRect rc(pItem->GetRect());

	CXTPTabPaintManager::CColorSet::COLORSET_TAB_EXCEL& cs = pItem->IsSelected()? GetColorSet()->m_csExcelSelected: GetColorSet()->m_csExcelNormal;

	switch(pItem->GetTabManager()->GetPosition())
	{
	case xtpTabPositionTop:
		{
			rc.bottom -= 1;
			int nMargin = rc.Height() / 2;
			rc.OffsetRect(- nMargin / 2 , 0);

			POINT ptsFill[] = {{rc.left,  rc.bottom}, {nMargin, - nMargin * 2}, {rc.Width() - nMargin, 0}, {nMargin, nMargin * 2}};
			COLORREF clrDark = FillButton(pDC, pItem, ptsFill, _countof(ptsFill), pItem->GetTabManager()->GetPosition(), FALSE);

			LineEx(pDC, cs.clrLeftOuterBorder, rc.left + 1, rc.bottom - 1, rc.left + nMargin + 1, rc.top);
			LineEx(pDC, cs.clrLeftOuterBorder, rc.left + nMargin, rc.top + 1, rc.right, rc.top + 1);
			LineEx(pDC, cs.clrRightOuterBorder, rc.right - 1, rc.top, rc.right + nMargin - 1, rc.bottom);

			LineEx(pDC, cs.clrLeftInnerBorder, rc.left, rc.bottom - 1, rc.left + nMargin, rc.top);
			LineEx(pDC, cs.clrLeftInnerBorder, rc.left + nMargin, rc.top, rc.right, rc.top);
			LineEx(pDC, cs.clrRightInnerBorder, rc.right, rc.top, rc.right + nMargin, rc.bottom);

			if (pItem->IsSelected())
				HorizontalLine(pDC, rc.left, rc.bottom, rc.Width() + nMargin, clrDark);

		}
		break;
	case xtpTabPositionLeft:
		{
			rc.right -= 1;
			int nMargin = rc.Width() / 2;
			rc.OffsetRect(0, - nMargin / 2);

			POINT ptsFill[] = {{rc.right,  rc.top}, {-nMargin * 2, nMargin}, {0, rc.Height() - nMargin}, {nMargin * 2, nMargin}};
			COLORREF clrDark = FillButton(pDC, pItem, ptsFill, _countof(ptsFill), pItem->GetTabManager()->GetPosition(), FALSE);

			LineEx(pDC, cs.clrLeftOuterBorder, rc.right - 1, rc.top + 1, rc.left, rc.top + nMargin + 1);
			LineEx(pDC, cs.clrLeftOuterBorder, rc.left + 1, rc.top + nMargin, rc.left + 1, rc.bottom);
			LineEx(pDC, cs.clrRightOuterBorder, rc.left , rc.bottom - 1, rc.right, rc.bottom + nMargin - 1);

			LineEx(pDC, cs.clrLeftInnerBorder, rc.right - 1, rc.top, rc.left, rc.top + nMargin);
			LineEx(pDC, cs.clrLeftInnerBorder, rc.left, rc.top + nMargin, rc.left, rc.bottom);
			LineEx(pDC, cs.clrRightInnerBorder, rc.left, rc.bottom, rc.right, rc.bottom + nMargin);

			if (pItem->IsSelected())
				VerticalLine(pDC, rc.right, rc.top, rc.Height() + nMargin, clrDark);

		}
		break;

	case xtpTabPositionBottom:
		{
			rc.bottom -= 1;
			int nMargin = rc.Height() / 2;
			rc.OffsetRect(- nMargin /2 , 0);

			POINT ptsFill[] = {{rc.left,  rc.top + 1}, {nMargin, nMargin * 2}, {rc.Width() - nMargin, 0}, {nMargin, - nMargin * 2}};
			COLORREF clrDark = FillButton(pDC, pItem, ptsFill, _countof(ptsFill), pItem->GetTabManager()->GetPosition(), FALSE);

			LineEx(pDC, cs.clrLeftOuterBorder, rc.left + 1, rc.top + 1, rc.left + nMargin + 1, rc.bottom);
			LineEx(pDC, cs.clrTopOuterBorder, rc.left + nMargin, rc.bottom - 1, rc.right, rc.bottom - 1);
			LineEx(pDC, cs.clrRightOuterBorder, rc.right - 1, rc.bottom, rc.right + nMargin - 1, rc.top);

			LineEx(pDC, cs.clrLeftInnerBorder, rc.left, rc.top + 1, rc.left + nMargin, rc.bottom);
			LineEx(pDC, cs.clrTopInnerBorder, rc.left + nMargin, rc.bottom, rc.right, rc.bottom);
			LineEx(pDC, cs.clrRightInnerBorder, rc.right, rc.bottom, rc.right + nMargin, rc.top);

			if (pItem->IsSelected())
				HorizontalLine(pDC, rc.left, rc.top, rc.Width() + nMargin, clrDark);
		}

		break;

	case xtpTabPositionRight:
		{
			rc.right -= 1;
			int nMargin = rc.Width() / 2;
			rc.OffsetRect(0, - nMargin / 2);

			POINT ptsFill[] = {{rc.left + 1,  rc.top}, {nMargin * 2, nMargin}, {0, rc.Height() - nMargin}, {- nMargin * 2, nMargin}};
			COLORREF clrDark = FillButton(pDC, pItem, ptsFill, _countof(ptsFill), pItem->GetTabManager()->GetPosition(), FALSE);

			LineEx(pDC, cs.clrLeftOuterBorder, rc.left + 1, rc.top + 1, rc.right, rc.top + nMargin + 1);
			LineEx(pDC, cs.clrLeftOuterBorder, rc.right - 1, rc.top + nMargin, rc.right - 1, rc.bottom);
			LineEx(pDC, cs.clrRightOuterBorder, rc.right , rc.bottom - 1, rc.left, rc.bottom + nMargin - 1);

			LineEx(pDC, cs.clrLeftInnerBorder, rc.left + 1, rc.top, rc.right, rc.top + nMargin);
			LineEx(pDC, cs.clrLeftInnerBorder, rc.right, rc.top + nMargin, rc.right, rc.bottom);
			LineEx(pDC, cs.clrRightInnerBorder, rc.right, rc.bottom, rc.left, rc.bottom + nMargin);

			if (pItem->IsSelected())
				VerticalLine(pDC, rc.left, rc.top, rc.Height() + nMargin, clrDark);
		}
		break;
	}

	m_pPaintManager->DrawSingleButtonIconAndText(pDC, pItem, pItem->GetRect(), TRUE);
}

//////////////////////////////////////////////////////////////////////////
// CAppearanceSetVisio


void CXTPTabPaintManager::CAppearanceSetVisio::AdjustClientRect(CXTPTabManager* pTabManager, CRect& rcClient)
{
	DeflateRectEx(rcClient, m_pPaintManager->m_rcControlMargin, pTabManager->GetPosition());

	rcClient = GetClientRect(rcClient, pTabManager);

	DeflateRectEx(rcClient, GetClientMargin(), pTabManager->GetPosition());
}

void CXTPTabPaintManager::CAppearanceSetVisio::RepositionTabControl(CXTPTabManager* pTabManager, CDC* /*pDC*/, CRect rcClient)
{

	CAppearanceSet::DeflateRectEx(rcClient, m_pPaintManager->m_rcControlMargin, pTabManager->GetPosition());

	if (pTabManager->GetItemCount() == 0)
		return;

	int nButtonHeight = GetButtonHeight(pTabManager);

	CRect rc(rcClient);

	if (m_pPaintManager->m_clientFrame != xtpTabFrameNone)
	{
		rc.left++;
	}

	rc.top++;

	CRect rcItem(rc.left, rc.top, rc.right, rc.top);

	for (int i = 0; i < pTabManager->GetItemCount(); i++)
	{
		CXTPTabManagerItem* pItem = pTabManager->GetItem(i);

		rcItem.bottom =  rcItem.top + nButtonHeight;

		pItem->SetRect(rcItem);

		rcItem.top += nButtonHeight;

		if (pItem->IsSelected())
		{
			int nBottom = max(rcItem.top + nButtonHeight, rc.bottom - (pTabManager->GetItemCount() - i - 1) * nButtonHeight);
			nBottom = max(rcItem.top, min(nBottom, rc.bottom - 1));
			rcItem.top = nBottom;
		}
	}
}

CRect CXTPTabPaintManager::CAppearanceSetVisio::GetHeaderRect(CRect /*rcControl*/, CXTPTabManager* /*pTabManager*/)
{
	return CXTPEmptyRect();
}


CRect CXTPTabPaintManager::CAppearanceSetVisio::GetClientRect(CRect rcControl, CXTPTabManager* pTabManager)
{
	CXTPTabManagerItem* pItem = pTabManager->GetSelectedItem();

	if (pItem)
	{
		int nButtonHeight = GetButtonHeight(pTabManager);

		rcControl.top += (pItem->GetIndex() + 1) * nButtonHeight + 1;

		rcControl.bottom -= (pTabManager->GetItemCount() - pItem->GetIndex() - 1) * nButtonHeight;

		rcControl.bottom = max(rcControl.top + nButtonHeight, rcControl.bottom);

		if (pItem->GetIndex() == pTabManager->GetItemCount() - 1 &&
			m_pPaintManager->m_clientFrame != xtpTabFrameNone) rcControl.bottom --;
	}

	if (m_pPaintManager->m_clientFrame != xtpTabFrameNone)
	{
		rcControl.DeflateRect(1, 0, 1, 0);
	}


	return rcControl;
}


void CXTPTabPaintManager::CAppearanceSetVisio::DrawSingleButton(CDC* pDC, CXTPTabManagerItem* pItem)
{
	CRect rcItem = pItem->GetRect();

	GetColorSet()->FillPropertyButton(pDC, rcItem, pItem);

	pDC->Draw3dRect(rcItem, GetColorSet()->m_clrHighlight, GetColorSet()->m_clrFrameBorder);

	for (int y = rcItem.top + 4; y < rcItem.bottom - 3; y += 2)
	{
		HorizontalLine(pDC, rcItem.right - 7, y, 3, GetXtremeColor(XPCOLOR_TOOLBAR_GRIPPER));
	}
	rcItem.right -= 7;

	m_pPaintManager->DrawSingleButtonIconAndText(pDC, pItem, rcItem, TRUE);
}

void CXTPTabPaintManager::CAppearanceSetVisio::DrawTabControl(CXTPTabManager* pTabManager, CDC* pDC, CRect rcClient)
{
	pDC->FillSolidRect(rcClient, GetColorSet()->m_clrControlFace);

	CAppearanceSet::DeflateRectEx(rcClient, m_pPaintManager->m_rcControlMargin, pTabManager->GetPosition());

	CRgn rgn;
	rgn.CreateRectRgnIndirect(rcClient);
	pDC->SelectClipRgn(&rgn);

	if (m_pPaintManager->m_clientFrame != xtpTabFrameNone)
	{
		pDC->Draw3dRect(rcClient, GetColorSet()->m_clrFrameBorder, GetColorSet()->m_clrFrameBorder);
	}
	else if (pTabManager->GetItemCount() > 0)
	{
		HorizontalLine(pDC, rcClient.left, rcClient.top, rcClient.Width(), GetColorSet()->m_clrFrameBorder);
	}

	pDC->SetBkMode(TRANSPARENT);

	for (int i = pTabManager->GetItemCount() - 1; i >= 0; i--)
	{
		CXTPTabManagerItem* pItem = pTabManager->GetItem(i);

		CRect rcItem = pItem->GetRect();

		if (CRect().IntersectRect(rcClient, rcItem) && !rcItem.IsRectEmpty())
		{
			DrawSingleButton(pDC, pItem);
		}
	}

	pDC->SelectClipRgn(NULL);
}
