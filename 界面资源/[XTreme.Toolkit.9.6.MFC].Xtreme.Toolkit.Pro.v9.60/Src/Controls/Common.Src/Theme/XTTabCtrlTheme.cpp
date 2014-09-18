// XTTabCtrlTheme.cpp: implementation of the CXTTabCtrlTheme class.
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

#include "XTTabCtrlTheme.h"
#include "XTDefines.h"
#include "XTUtil.h"
#include "XTGlobal.h"
#include "XTVC50Helpers.h"
#include "XTTabCtrl.h"
#include "XTTabCtrlButtons.h"

#define DB_LEFT        0x0002
#define DB_RIGHT       0x0004
#define DB_CROSS       0x0008

#define DB_ENABLED     0x0000
#define DB_DISABLED    0x0100
#define DB_PRESSED     0x1000
#define DB_OVER        0x4000

#define DB_ISENABLED(Style)     (!((Style)&DB_DISABLED))
#define DB_ISPRESSED(Style)     (((Style)&(DB_PRESSED))!=0)
#define DB_ISOVER(Style)        (((Style)&DB_OVER)==DB_OVER)


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


IMPLEMENT_THEME_FACTORY(CXTTabCtrlTheme, CXTTabCtrlThemeOfficeXP, CXTTabCtrlThemeOfficeXP)
//===========================================================================
// ÑXTTabCtrlTheme class
//===========================================================================

CXTTabCtrlTheme::CXTTabCtrlTheme()
{
}

CXTTabCtrlTheme::~CXTTabCtrlTheme()
{

}

void CXTTabCtrlTheme::RefreshMetrics()
{
	XTPColorManager()->RefreshColors();

}

void CXTTabCtrlTheme::PaintButtons(CXTTabCtrlBase* pTabCtrlBase)
{
	CXTTabCtrlButtons* pNavBtns = pTabCtrlBase->GetButtons();
	CTabCtrl* pTabCtrl = pTabCtrlBase->GetTabCtrlImpl();

	if ( pNavBtns )
	{
		const bool bVertical = (pTabCtrl->GetStyle() & TCS_VERTICAL) == TCS_VERTICAL;
		const bool bMultiLine = bVertical || ((pTabCtrl->GetStyle() & TCS_MULTILINE) == TCS_MULTILINE);

		if ((m_enumTheme != xtThemeDefault) && !bVertical && !bMultiLine && !pTabCtrlBase->m_bAutoCondensing)
		{
			pNavBtns->SubclassTabButtons(pTabCtrl->GetDlgItem(1));
			pNavBtns->ShowButtons(TRUE);
		}
		else
		{
			pNavBtns->ShowButtons(FALSE);
		}
	}
}

void CXTTabCtrlTheme::DrawButton(CDC* /*pDC*/, CXTTabCtrlButton* /*pButton*/, COLORREF /*clrButton*/)
{
	ASSERT(FALSE);
}

void CXTTabCtrlTheme::DrawTabCtrl(CDC* pDC, CXTTabCtrlBase* pTabCtrlBase)
{
	PaintButtons(pTabCtrlBase);

	PROTECTED_DEFWINDOWPROC_CALL(CXTTabCtrlTheme, pTabCtrlBase->GetTabCtrlImpl(), WM_PAINT, (WPARAM)pDC->m_hDC, 0);
}

//////////////////////////////////////////////////////////////////////////
// CXTTabCtrlThemeOfficeXP

CXTTabCtrlThemeOfficeXP::CXTTabCtrlThemeOfficeXP()
{
	m_enumTheme = xtThemeOfficeXP;
	RefreshMetrics();

}

void CXTTabCtrlThemeOfficeXP::RefreshMetrics()
{
	CXTTabCtrlTheme::RefreshMetrics();

	// Update pen colors
	m_penBlack.DeleteObject();
	m_penBlack.CreatePen(PS_SOLID, 1, GetXtremeColor(COLOR_BTNTEXT));

	m_penWhite.DeleteObject();
	m_penWhite.CreatePen(PS_SOLID, 1, GetXtremeColor(COLOR_3DHILIGHT));

	m_penFace.DeleteObject();
	m_penFace.CreatePen(PS_SOLID, 1, GetXtremeColor(COLOR_3DFACE));

	m_penText.DeleteObject();
	m_penText.CreatePen(PS_SOLID, 1, GetXtremeColor(COLOR_3DSHADOW));

	m_iEdge     = ::GetSystemMetrics(SM_CYEDGE);

}

void CXTTabCtrlThemeOfficeXP::DrawTabCtrl(CDC* pDC, CXTTabCtrlBase* pTabCtrlBase)
{
	CTabCtrl* pTabCtrl = pTabCtrlBase->GetTabCtrlImpl();
	CRect rcClient;
	pTabCtrl->GetClientRect(&rcClient);

	if (pTabCtrl->GetItemCount() != 0)
	{
		// Save the device context state.
		int nSavedDC = pDC->SaveDC();

		// Get the current tab selection.
		int iCurSel = pTabCtrl->GetCurSel();

		// get the selected tab item rect.
		CRect rcItem;
		pTabCtrl->GetItemRect(iCurSel, &rcItem);

		DWORD dwStyle = pTabCtrl->GetStyle();

		// Vertical tabs
		if (dwStyle & TCS_VERTICAL)
		{
			// Right
			if (dwStyle & TCS_RIGHT) {
				OnDrawRight(pDC, pTabCtrlBase, rcClient, rcItem);
			}
			// Left
			else {
				OnDrawLeft(pDC, pTabCtrlBase, rcClient, rcItem);
			}
		}

		// Horizontal tabs
		else
		{
			// Bottom
			if (dwStyle & TCS_BOTTOM) {
				OnDrawBottom(pDC, pTabCtrlBase, rcClient, rcItem);
			}
			// Top
			else {
				OnDrawTop(pDC, pTabCtrlBase, rcClient, rcItem);
			}
		}

		PaintButtons(pTabCtrlBase);

		if (pTabCtrlBase->m_bXPBorder)
		{
			CRect rcChild;
			pTabCtrlBase->GetChildRect(rcChild);
			rcChild.InflateRect(1,1);

			pDC->Draw3dRect(&rcChild,
				GetXtremeColor(COLOR_3DSHADOW), GetXtremeColor(COLOR_3DSHADOW));
		}

		pDC->RestoreDC(nSavedDC);
	}
	else
	{
		PaintButtons(pTabCtrlBase);
	}
}

void CXTTabCtrlThemeOfficeXP::OnDrawBottom(CDC *pDC, CXTTabCtrlBase* pTabCtrlBase, CRect rcClient, CRect rcItem)
{
	CTabCtrl* pTabCtrl = pTabCtrlBase->GetTabCtrlImpl();

	// Make sure the correct font is selected.
	pDC->SelectObject(&XTAuxData().font);

	// repaint the tab area.
	CRect rcHeight;
	pTabCtrl->GetItemRect(-1, &rcHeight);
	rcClient.top = rcItem.top;
	pDC->FillSolidRect(rcClient, GetXtremeColor(XPCOLOR_TAB_INACTIVE_BACK));

	// draw a line to separate tab area.
	pDC->SelectObject(&m_penBlack);

	pDC->MoveTo(rcClient.left, rcClient.top);
	pDC->LineTo(rcClient.right, rcClient.top);

	rcItem.top -=1;
	// paint the selected tab background.
	pDC->FillSolidRect(&rcItem, GetXtremeColor(COLOR_3DFACE));

	// draw the selected tab.
	pDC->MoveTo(rcItem.right, rcItem.top+1);
	pDC->LineTo(rcItem.right, rcItem.bottom-1);
	pDC->MoveTo(rcItem.right, rcItem.bottom-1);
	pDC->LineTo(rcItem.left,  rcItem.bottom-1);

	pDC->SelectObject(&m_penWhite);
	pDC->MoveTo(rcItem.left, rcItem.top);
	pDC->LineTo(rcItem.left, rcItem.bottom);

	pDC->SelectObject(&m_penFace);
	pDC->MoveTo(rcItem.right-1, rcItem.bottom-2);
	pDC->LineTo(rcItem.left,    rcItem.bottom-2);

	// Get the current tab selection.
	int iCurSel = pTabCtrl->GetCurSel();

	// draw remaining tabs
	int iItem;
	for (iItem = 0; iItem < pTabCtrl->GetItemCount(); ++iItem)
	{
		pTabCtrl->GetItemRect(iItem, &rcItem);

		TCHAR szLabel[256];
		TC_ITEM tci;
		tci.mask       = TCIF_TEXT|TCIF_IMAGE;
		tci.pszText    = szLabel;
		tci.cchTextMax = _countof(szLabel);

		if (!pTabCtrl->GetItem(iItem, &tci )) {
			continue;
		}

		pDC->SetBkMode(TRANSPARENT);

		CRect rcText(rcItem);

		// Draw any associated icons.
		CImageList* pImageList = pTabCtrl->GetImageList();
		if (pImageList && tci.iImage >= 0)
		{
			CPoint point = rcItem.TopLeft();
			point.x += 2;

			point.Offset(m_iEdge, m_iEdge);
			pImageList->Draw(pDC, tci.iImage, point, ILD_TRANSPARENT);

			IMAGEINFO info;
			pImageList->GetImageInfo(tci.iImage, &info);
			CRect rcImage(info.rcImage);
			rcText.left += rcImage.Width() + m_iEdge;
		}

		// Set the font for the tab label.
		if (iItem == iCurSel)
		{
			pDC->SetTextColor(GetXtremeColor(COLOR_BTNTEXT));
			pDC->SelectObject(pTabCtrlBase->m_bBoldFont ? &XTAuxData().fontBold : &XTAuxData().font);
		}
		else
		{
			pDC->SetTextColor(GetXtremeColor(XPCOLOR_TAB_INACTIVE_TEXT));
			pDC->SelectObject(&XTAuxData().font);
		}

		// Draw the tab label.
		pDC->DrawText(szLabel, rcText, DT_SINGLELINE|DT_VCENTER|DT_CENTER|DT_END_ELLIPSIS);

		if (iItem == iCurSel || iItem == iCurSel-1) {
			continue;
		}

		// Draw the tab separator.
		pDC->SelectObject(&m_penText);
		pDC->MoveTo(rcItem.right, rcItem.top+4);
		pDC->LineTo(rcItem.right, rcItem.bottom-2);
	}
}

void CXTTabCtrlThemeOfficeXP::OnDrawTop(CDC *pDC, CXTTabCtrlBase* pTabCtrlBase, CRect rcClient, CRect rcItem)
{
	CTabCtrl* pTabCtrl = pTabCtrlBase->GetTabCtrlImpl();

	// Make sure the correct font is selected.
	pDC->SelectObject(&XTAuxData().font);

	int iBottom = (rcClient.top+rcItem.Height())*pTabCtrl->GetRowCount();

	// repaint the tab area.
	rcClient.bottom = iBottom;
	pDC->FillSolidRect(rcClient, GetXtremeColor(XPCOLOR_TAB_INACTIVE_BACK));

	// draw a line to separate tab area.
	pDC->SelectObject(&m_penWhite);
	pDC->MoveTo(rcClient.left,  iBottom);
	pDC->LineTo(rcClient.right, iBottom);

	// paint the selected tab background.
	pDC->FillSolidRect(&rcItem, GetXtremeColor(COLOR_3DFACE));

	// draw the selected tab.
	pDC->MoveTo(rcItem.left, rcItem.top);
	pDC->LineTo(rcItem.left, iBottom + 1);
	pDC->MoveTo(rcItem.right, rcItem.top);
	pDC->LineTo(rcItem.left,  rcItem.top);

	pDC->SelectObject(&m_penBlack);
	pDC->MoveTo(rcItem.right, iBottom-1);
	pDC->LineTo(rcItem.right, rcItem.top-1);

	pDC->SelectObject(&m_penFace);
	pDC->MoveTo(rcItem.left+1, iBottom);
	pDC->LineTo(rcItem.right, iBottom);

	// Get the current tab selection.
	int iCurSel = pTabCtrl->GetCurSel();

	// draw remaining tabs
	int iItem;
	for (iItem = 0; iItem < pTabCtrl->GetItemCount(); ++iItem)
	{
		pTabCtrl->GetItemRect(iItem, &rcItem);

		TCHAR szLabel[256];
		TC_ITEM tci;
		tci.mask       = TCIF_TEXT|TCIF_IMAGE;
		tci.pszText    = szLabel;
		tci.cchTextMax = _countof(szLabel);

		if (!pTabCtrl->GetItem(iItem, &tci )) {
			continue;
		}

		pDC->SetBkMode(TRANSPARENT);

		CRect rcText(rcItem);

		// Draw any associated icons.
		CImageList* pImageList = pTabCtrl->GetImageList();
		if (pImageList && tci.iImage >= 0)
		{
			CPoint point = rcItem.TopLeft();
			point.x += 2;
			point.Offset(m_iEdge, m_iEdge);
			pImageList->Draw(pDC, tci.iImage, point, ILD_TRANSPARENT);

			IMAGEINFO info;
			pImageList->GetImageInfo(tci.iImage, &info);
			CRect rcImage(info.rcImage);
			rcText.left += rcImage.Width() + m_iEdge;
		}

		// Set the font for the tab label.
		if (iItem == iCurSel)
		{
			pDC->SetTextColor(GetXtremeColor(COLOR_BTNTEXT));
			pDC->SelectObject(pTabCtrlBase->m_bBoldFont ? &XTAuxData().fontBold : &XTAuxData().font);
		}
		else
		{
			pDC->SetTextColor(GetXtremeColor(XPCOLOR_TAB_INACTIVE_TEXT));
			pDC->SelectObject(&XTAuxData().font);
		}

		// Draw the tab label.
		pDC->DrawText(szLabel, rcText, DT_SINGLELINE|DT_VCENTER|DT_CENTER|DT_END_ELLIPSIS);

		if (iItem == iCurSel || iItem == iCurSel-1) {
			continue;
		}

		// Draw the tab separator.
		pDC->SelectObject(&m_penText);
		pDC->MoveTo(rcItem.right, rcItem.top+3);
		pDC->LineTo(rcItem.right, rcItem.bottom-3);
	}
}

void CXTTabCtrlThemeOfficeXP::OnDrawLeft(CDC *pDC, CXTTabCtrlBase* pTabCtrlBase, CRect rcClient, CRect rcItem)
{
	CTabCtrl* pTabCtrl = pTabCtrlBase->GetTabCtrlImpl();

	// repaint the tab area.
	rcClient.right = rcItem.right;
	pDC->FillSolidRect(rcClient, GetXtremeColor(XPCOLOR_TAB_INACTIVE_BACK));

	// draw a line to separate tab area.
	pDC->SelectObject(&m_penWhite);
	pDC->MoveTo(rcClient.right, rcClient.top);
	pDC->LineTo(rcClient.right, rcClient.bottom);

	// paint the selected tab background.
	pDC->FillSolidRect(&rcItem, GetXtremeColor(COLOR_3DFACE));

	// draw the selected tab.
	pDC->MoveTo(rcItem.left, rcItem.top);
	pDC->LineTo(rcItem.left, rcItem.bottom);
	pDC->MoveTo(rcItem.right, rcItem.top);
	pDC->LineTo(rcItem.left,  rcItem.top);

	pDC->SelectObject(&m_penBlack);
	pDC->MoveTo(rcItem.left, rcItem.bottom);
	pDC->LineTo(rcItem.right, rcItem.bottom);

	pDC->SelectObject(&m_penFace);
	pDC->MoveTo(rcItem.right, rcItem.top);
	pDC->LineTo(rcItem.right, rcItem.bottom);

	// Get the current tab selection.
	int iCurSel = pTabCtrl->GetCurSel();

	// draw remaining tabs
	int iItem;
	for (iItem = 0; iItem < pTabCtrl->GetItemCount(); ++iItem)
	{
		pTabCtrl->GetItemRect(iItem, &rcItem);

		TCHAR szLabel[256];
		TC_ITEM tci;
		tci.mask       = TCIF_TEXT|TCIF_IMAGE;
		tci.pszText    = szLabel;
		tci.cchTextMax = _countof(szLabel);

		if (!pTabCtrl->GetItem(iItem, &tci )) {
			continue;
		}

		pDC->SetBkMode(TRANSPARENT);

		CRect rcText(rcItem);

		CPoint point(rcItem.TopLeft());
		point.Offset(m_iEdge, m_iEdge);

		// Draw any associated icons.
		CImageList* pImageList = pTabCtrl->GetImageList();
		if (pImageList && tci.iImage >= 0)
		{
			pImageList->Draw(pDC, tci.iImage, point, ILD_TRANSPARENT);

			IMAGEINFO info;
			pImageList->GetImageInfo(tci.iImage, &info);
			CRect rcImage(info.rcImage);
			rcText.top += rcImage.Height() + m_iEdge;
		}

		// Set the font for the tab label.
		if (iItem == iCurSel)
		{
			pDC->SetTextColor(GetXtremeColor(COLOR_BTNTEXT));
			pDC->SelectObject(pTabCtrlBase->m_bBoldFont ? &XTAuxData().fontVertBold : &XTAuxData().fontVert);
		}
		else
		{
			pDC->SetTextColor(GetXtremeColor(XPCOLOR_TAB_INACTIVE_TEXT));
			pDC->SelectObject(&XTAuxData().fontVert);
		}

		CSize sz = pDC->GetTextExtent(szLabel);

		rcText.left = rcText.right-(rcText.Width()-sz.cy+1)/ 2;
		rcText.top = rcText.top+(rcText.Height()-sz.cx+1)/ 2;

		pDC->DrawText(szLabel, &rcText, DT_SINGLELINE|DT_NOCLIP);

		if (iItem == iCurSel || iItem == iCurSel-1) {
			continue;
		}

		// Draw the tab separator.
		pDC->SelectObject(&m_penText);
		pDC->MoveTo(rcItem.left +2, rcItem.bottom);
		pDC->LineTo(rcItem.right-2, rcItem.bottom);
	}
}

void CXTTabCtrlThemeOfficeXP::OnDrawRight(CDC *pDC, CXTTabCtrlBase* pTabCtrlBase, CRect rcClient, CRect rcItem)
{
	CTabCtrl* pTabCtrl = pTabCtrlBase->GetTabCtrlImpl();

	// repaint the tab area.
	rcClient.left = rcItem.left;
	pDC->FillSolidRect(rcClient, GetXtremeColor(XPCOLOR_TAB_INACTIVE_BACK));

	// draw a line to separate tab area.
	pDC->SelectObject(&m_penBlack);
	pDC->MoveTo(rcClient.left, rcClient.top);
	pDC->LineTo(rcClient.left, rcClient.bottom);

	// paint the selected tab background.
	pDC->FillSolidRect(&rcItem, GetXtremeColor(COLOR_3DFACE));

	// draw the selected tab.
	pDC->MoveTo(rcItem.right, rcItem.top);
	pDC->LineTo(rcItem.right, rcItem.bottom+1);
	pDC->MoveTo(rcItem.left, rcItem.bottom);
	pDC->LineTo(rcItem.right, rcItem.bottom);

	pDC->SelectObject(&m_penWhite);
	pDC->MoveTo(rcItem.right, rcItem.top);
	pDC->LineTo(rcItem.left,  rcItem.top);

	pDC->SelectObject(&m_penFace);
	pDC->MoveTo(rcItem.left, rcItem.top);
	pDC->LineTo(rcItem.left, rcItem.bottom);

	// Get the current tab selection.
	int iCurSel = pTabCtrl->GetCurSel();

	// draw remaining tabs
	int iItem;
	for (iItem = 0; iItem < pTabCtrl->GetItemCount(); ++iItem)
	{
		pTabCtrl->GetItemRect(iItem, &rcItem);

		TCHAR szLabel[256];
		TC_ITEM tci;
		tci.mask       = TCIF_TEXT|TCIF_IMAGE;
		tci.pszText    = szLabel;
		tci.cchTextMax = _countof(szLabel);

		if (!pTabCtrl->GetItem(iItem, &tci )) {
			continue;
		}

		pDC->SetBkMode(TRANSPARENT);

		CRect rcText(rcItem);

		CPoint point(rcItem.TopLeft());
		point.Offset(m_iEdge, m_iEdge);

		// Draw any associated icons.
		CImageList* pImageList = pTabCtrl->GetImageList();
		if (pImageList && tci.iImage >= 0)
		{
			pImageList->Draw(pDC, tci.iImage, point, ILD_TRANSPARENT);

			IMAGEINFO info;
			pImageList->GetImageInfo(tci.iImage, &info);
			CRect rcImage(info.rcImage);
			rcText.top += rcImage.Height() + m_iEdge;
		}

		// Set the font for the tab label.
		if (iItem == iCurSel)
		{
			pDC->SetTextColor(GetXtremeColor(COLOR_BTNTEXT));
			pDC->SelectObject(pTabCtrlBase->m_bBoldFont ? &XTAuxData().fontVertBold : &XTAuxData().fontVert);
		}
		else
		{
			pDC->SetTextColor(GetXtremeColor(XPCOLOR_TAB_INACTIVE_TEXT));
			pDC->SelectObject(&XTAuxData().fontVert);
		}

		CSize sz = pDC->GetTextExtent(szLabel);

		rcText.left = rcText.right-(rcText.Width()-sz.cy+1)/ 2;
		rcText.top = rcText.top+(rcText.Height()-sz.cx+1)/ 2;

		pDC->DrawText(szLabel, &rcText, DT_SINGLELINE|DT_NOCLIP);

		if (iItem == iCurSel || iItem == iCurSel-1) {
			continue;
		}

		// Draw the tab separator.
		pDC->SelectObject(&m_penText);
		pDC->MoveTo(rcItem.left +2, rcItem.bottom);
		pDC->LineTo(rcItem.right-2, rcItem.bottom);
	}
}


void CXTTabCtrlThemeOfficeXP::DrawButton(CDC* pDC, CXTTabCtrlButton* pButton, COLORREF clrButton)
{
	ASSERT (pDC != NULL);

	CRect rc (pButton->GetRect());
	DWORD wStyle = pButton->m_wStyle;

	if (DB_ISENABLED(wStyle))
	{
		if (DB_ISPRESSED(wStyle))
			pDC->Draw3dRect(rc, RGB(0, 0, 0), GetSysColor(0x16));
		else if (DB_ISOVER(wStyle))
			pDC->Draw3dRect(rc, GetSysColor(0x16), RGB(0, 0, 0));

		if (DB_ISPRESSED(wStyle) )
		{
			rc.OffsetRect(1, 1);
		}
	}

	CXTPPenDC pen(*pDC, clrButton);

	CBrush brush;
	CBrush* pOldBrush;

	if (DB_ISENABLED(wStyle))
	{
		brush.CreateSolidBrush(clrButton);
		pOldBrush = pDC->SelectObject(&brush);
	} else
	{
		pOldBrush = (CBrush*)pDC->SelectStockObject(NULL_BRUSH);
	}

	if (wStyle & DB_CROSS)
	{
		pDC->MoveTo(rc.left + 3, rc.top + 3);
		pDC->LineTo(rc.right - 4, rc.bottom - 5);
		pDC->MoveTo(rc.left + 4, rc.top + 3);
		pDC->LineTo(rc.right - 3, rc.bottom - 5);

		pDC->MoveTo(rc.left + 3, rc.bottom  - 6);
		pDC->LineTo(rc.right - 4, rc.top + 2);
		pDC->MoveTo(rc.left + 4, rc.bottom  - 6);
		pDC->LineTo(rc.right - 3, rc.top + 2);
	}
	else if (wStyle & DB_LEFT)
	{
		CPoint pts[] =
		{
			CPoint(rc.right - 5, rc.top + 2),
			CPoint(rc.right - 9, rc.top + 6),
			CPoint(rc.right - 5, rc.top + 10)
		};
		pDC->Polygon( pts, 3 );
	}
	else if (wStyle & DB_RIGHT)
	{
		CPoint pts[] =
		{
			CPoint(rc.left + 4, rc.top + 2),
			CPoint(rc.left + 8, rc.top + 6),
			CPoint(rc.left + 4, rc.top + 10)
		};
		pDC->Polygon( pts, 3 );
	}

	pDC->SelectObject(pOldBrush);
}
