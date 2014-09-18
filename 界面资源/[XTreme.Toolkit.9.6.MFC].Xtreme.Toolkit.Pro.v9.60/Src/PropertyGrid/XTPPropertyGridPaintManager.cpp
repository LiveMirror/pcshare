// XTPPropertyGridPaintManager.cpp : implementation of the CXTPPropertyGridPaintManager class.
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
#include "XTPVC80Helpers.h"
#include "XTPDrawHelpers.h"

#include "XTPPropertyGridPaintManager.h"
#include "XTPPropertyGridInplaceEdit.h"
#include "XTPPropertyGridInplaceButton.h"
#include "XTPPropertyGridInplaceList.h"
#include "XTPPropertyGridItem.h"
#include "XTPPropertyGrid.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CXTPPropertyGridPaintManager::CXTPPropertyGridPaintManager(CXTPPropertyGrid* pGrid)
	: m_pGrid(pGrid)
{
	m_bThemed = FALSE;

	LOGFONT lfIcon;
	VERIFY( ::SystemParametersInfo( SPI_GETICONTITLELOGFONT, sizeof( lfIcon ), &lfIcon, 0 ) );
	lfIcon.lfWeight = FW_NORMAL;
	lfIcon.lfItalic = FALSE;

	VERIFY( m_fntVerbNormal.CreateFontIndirect(&lfIcon  ) );
	lfIcon.lfUnderline = TRUE;
	VERIFY( m_fntVerbUnderline.CreateFontIndirect(&lfIcon  ) );


	RefreshMetrics();

}

CXTPPropertyGridPaintManager::~CXTPPropertyGridPaintManager()
{

}

void CXTPPropertyGridPaintManager::RefreshMetrics()
{
	XTPColorManager()->RefreshColors();

	m_clrFace = GetXtremeColor(COLOR_3DFACE);
	m_clrShadow = GetXtremeColor(COLOR_3DSHADOW);
	m_clrVerbFace = GetXtremeColor(COLOR_ACTIVECAPTION);

	CXTPPropertyGridView* pView = &m_pGrid->GetGridView();

	m_pGrid->m_clrHelpBack.SetStandardValue(m_clrFace);
	m_pGrid->m_clrHelpFore.SetStandardValue(GetXtremeColor(COLOR_BTNTEXT));
	pView->m_clrLine.SetStandardValue(GetSysColor(COLOR_SCROLLBAR));
	pView->m_clrCategoryFore.SetStandardValue(GetXtremeColor(COLOR_GRAYTEXT));
	pView->m_clrBack.SetStandardValue(GetXtremeColor(COLOR_WINDOW));
	pView->m_clrFore.SetStandardValue(GetXtremeColor(COLOR_WINDOWTEXT));
	pView->m_clrReadOnlyFore.SetStandardValue(GetXtremeColor(COLOR_GRAYTEXT));

	HWND hWnd = AfxGetMainWnd()? AfxGetMainWnd()->GetSafeHwnd(): 0;
	m_themeTree.OpenThemeData(hWnd, L"TREEVIEW");
	m_themeButton.OpenThemeData(hWnd, L"BUTTON");
	m_themeCombo.OpenThemeData(hWnd, L"COMBOBOX");


}

void CXTPPropertyGridPaintManager::FillPropertyGrid(CDC* pDC)
{
	CXTPClientRect rc(m_pGrid);
	CXTPPropertyGridView* pView = &m_pGrid->GetGridView();
	pDC->FillSolidRect(rc, m_clrFace);
	pDC->SetBkMode(TRANSPARENT);

	if (m_pGrid->IsHelpVisible())
	{
		CXTPPenDC pen(*pDC, m_clrShadow);
		CXTPBrushDC brush(*pDC, m_pGrid->m_clrHelpBack);

		CRect rcHelp(rc);
		rcHelp.top = rc.bottom - m_pGrid->m_nHelpHeight;
		pDC->Rectangle(rcHelp);


		CXTPFontDC font(pDC, &pView->m_fontBold);
		CXTPPropertyGridItem* pItem = m_pGrid->GetSelectedItem();
		if (pItem)
		{
			pDC->SetTextColor(m_pGrid->m_clrHelpFore);
			pDC->SetBkColor(m_pGrid->m_clrHelpBack);
			CRect rcCaption(rcHelp);
			rcCaption.DeflateRect(6, 3);
			pDC->DrawText(pItem->GetCaption(), rcCaption, DT_SINGLELINE|DT_NOPREFIX);

			CString strDesc = pItem->GetDescription();
			if (!strDesc.IsEmpty())
			{
				CXTPFontDC font(pDC, &pView->m_fontNormal);
				CXTPEmptyRect rcCaption;
				pDC->DrawText(pItem->GetCaption(), rcCaption, DT_SINGLELINE|DT_NOPREFIX|DT_CALCRECT);
				CRect rcDesc(rcHelp);
				rcDesc.DeflateRect(6, 3 + rcCaption.Height() + 3, 6, 3);
				pDC->DrawText(strDesc, rcDesc, DT_WORDBREAK|DT_NOPREFIX);
			}
		}

		rc.bottom -= m_pGrid->m_nHelpHeight + 3;
	}

	if (m_pGrid->IsVerbsVisible())
	{
		CXTPPenDC pen(*pDC, m_clrShadow);
		CXTPBrushDC brush(*pDC, m_pGrid->m_clrHelpBack);

		CRect rcVerbs(rc);
		rcVerbs.top = rc.bottom - m_pGrid->m_nVerbsHeight;

		HitTestVerbs(pDC, rcVerbs, CPoint(0));
	}
}

int CXTPPropertyGridPaintManager::HitTestVerbs(CDC* pDC, CRect rcVerbs, CPoint pt)
{
	#define DRAWTEXTPART(strCaption)\
		CRect rcText(CRect(x, y, min(x + nTextWidth, rcVerbs.right), min(y + nTextHeight, rcVerbs.bottom)));\
		if (bDraw) {\
			pVerb->SetPart(rcText);\
			pDC->SetTextColor(m_clrVerbFace);\
			pDC->DrawText(strCaption, rcText, DT_SINGLELINE|DT_NOPREFIX); \
			if (pVerb->IsFocused()) {\
				pDC->SetTextColor(m_pGrid->m_clrHelpBack); pDC->SetBkColor(0); rcText.bottom ++;\
				pDC->DrawFocusRect(rcText);\
			}\
		}\
		else if (rcText.PtInRect(pt)) return i;

	BOOL bDraw = (pt == CPoint(0));

	if (bDraw) pDC->Rectangle(rcVerbs);

	CXTPFontDC font(pDC, &m_fntVerbUnderline);

	rcVerbs.DeflateRect(6, 4);

	int nCount = m_pGrid->GetVerbs()->GetCount();

	int x = rcVerbs.left, y = rcVerbs.top, nWidth = rcVerbs.Width();
	int nTextHeight = pDC->GetTextExtent(_T("XXX")).cy;

	for (int i = 0; i < nCount; i++)
	{
		CXTPPropertyGridVerb* pVerb = m_pGrid->GetVerbs()->GetAt(i);
		CString strCaption = pVerb->GetCaption();

		while(TRUE)
		{
			if (strCaption.IsEmpty())
				break;

			int nTextWidth = pDC->GetTextExtent(strCaption).cx;

			if (x + nTextWidth < nWidth)
			{
				DRAWTEXTPART(strCaption);
				x += nTextWidth;
				break;
			}
			else
			{
				int nStartIndex = (strCaption.GetLength() - 1) * (nWidth - x) / nTextWidth;
				int nIndex = nStartIndex;

				if ((nStartIndex < 1) && (x == rcVerbs.left))
				{
					i = nCount - 1;
					break;
				}

				for (;nIndex >= 0; nIndex--)
				{
					if (strCaption[nIndex] == _T(' ') || strCaption[nIndex] == _T(',') || strCaption[nIndex] == _T('-'))
						break;
				}

				if (nIndex > 0 || (x == rcVerbs.left))
				{
					if (nIndex <= 0) nIndex = nStartIndex;

					CString strLeft = strCaption.Left(nIndex + 1);
					CString strRight = strCaption.Mid(nIndex + 1);

					int nTextWidth = pDC->GetTextExtent(strLeft).cx;
					DRAWTEXTPART(strLeft);

					x = rcVerbs.left;
					y += nTextHeight;
					strCaption = strRight;
				}
				else
				{
					y += nTextHeight;
					x = rcVerbs.left;
				}
			}
			if (y >= rcVerbs.bottom)
				break;
		}

		if ((y >= rcVerbs.bottom) || (i == nCount - 1))
			break;

		if (bDraw)
		{
			font.SetFont(&m_fntVerbNormal);
			pDC->SetTextColor(m_pGrid->m_clrHelpFore);
			pDC->DrawText(_T(","), 1, CRect(x, y, x + 8, min(y + nTextHeight, rcVerbs.bottom)), DT_SINGLELINE|DT_NOPREFIX);
			font.SetFont(&m_fntVerbUnderline);
		}
		x += 8;
	}
	return -1;
}

void CXTPPropertyGridPaintManager::FillPropertyGridView(CDC* pDC, BOOL bClient)
{

	CXTPPropertyGridView* pView = &m_pGrid->GetGridView();


	if (bClient)
	{
		CXTPClientRect rc(pView);
		pDC->FillSolidRect(rc, pView->m_clrBack);
	} else
	{
		CXTPWindowRect rc(pView);
		rc.OffsetRect(-rc.TopLeft());
		pDC->Draw3dRect(rc, m_clrShadow, m_clrShadow);
	}

}

void CXTPPropertyGridPaintManager::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	CXTPPropertyGridView* pView = &m_pGrid->GetGridView();
	if (pView->GetStyle() & LBS_OWNERDRAWVARIABLE)
	{
		CXTPPropertyGridItem* pItem = (CXTPPropertyGridItem*)lpMeasureItemStruct->itemData;
		pItem->MeasureItem(lpMeasureItemStruct);
	}
}

void CXTPPropertyGridPaintManager::DrawCategoryCaptionBackground(CDC* pDC, CRect rc)
{
	CXTPPropertyGridView* pView = &m_pGrid->GetGridView();
	pDC->FillSolidRect(rc, pView->m_clrLine);
}

void CXTPPropertyGridPaintManager::DrawItem(PDRAWITEMSTRUCT lpDrawItemStruct)
{
	CXTPPropertyGridView* pView = &m_pGrid->GetGridView();
	CXTPPropertyGridItem* pItem = (CXTPPropertyGridItem*)lpDrawItemStruct->itemData;

	CXTPWindowRect rcWindow(pView);
	CRect rc = lpDrawItemStruct->rcItem;

	CXTPBufferDC dc(lpDrawItemStruct->hDC, rc);
	CXTPFontDC font(&dc, &pView->m_fontNormal);

	COLORREF clrWindow = pView->m_clrBack;

	dc.FillSolidRect(rc, clrWindow);
	dc.SetBkMode(TRANSPARENT);


	CRect rcCaption(rc.left, rc.top, rc.left + pView->GetDividerPos(), rc.bottom);

	if (pItem->IsCategory())
	{
		DrawCategoryCaptionBackground(&dc, rc);

		CXTPFontDC font(&dc, &pView->m_fontBold);

		dc.SetTextColor(pView->m_clrCategoryFore);
		CXTPEmptyRect rcDrawText;
		dc.DrawText( pItem->GetCaption(), rcDrawText,  DT_SINGLELINE|DT_NOPREFIX|DT_VCENTER|DT_CALCRECT);

		CRect rcText(CPoint(((pItem->GetIndent() + 1) * XTP_PGI_EXPAND_BORDER) - 1, rc.top + 1) , CSize(rcDrawText.Width() + 4, rc.Height() - 3));

		dc.DrawText( pItem->GetCaption(), rcText,  DT_SINGLELINE|DT_NOPREFIX|DT_VCENTER|DT_RIGHT);

		if ((lpDrawItemStruct->itemAction | ODA_FOCUS) && (lpDrawItemStruct->itemState & ODS_FOCUS))
		{
			dc.SetTextColor(0);
			dc.DrawFocusRect(rcText);
		}
	}
	else
	{
		dc.SetTextColor(pItem->GetReadOnly()? pView->m_clrReadOnlyFore: pView->m_clrFore);

		CRect rcValue(rc);
		rcValue.left = rcCaption.right + 4;
		rcValue.bottom -= 1;

		if (!pItem->OnDrawItemValue(dc, rcValue))
		{
			dc.DrawText( pItem->GetViewValue(), rcValue,  DT_SINGLELINE|DT_NOPREFIX|DT_VCENTER);
		}

		BOOL bSelected = ((lpDrawItemStruct->itemAction | ODA_SELECT) && (lpDrawItemStruct->itemState & ODS_SELECTED));
		BOOL bFocused = bSelected && CWnd::GetFocus() && ((CWnd::GetFocus() == pView) || (CWnd::GetFocus()->GetParent() == pView) || (CWnd::GetFocus()->GetOwner() == pView));
		if (bFocused)
		{
			dc.SetTextColor(::GetXtremeColor(COLOR_HIGHLIGHTTEXT));
			dc.FillSolidRect(rcCaption, ::GetXtremeColor(COLOR_HIGHLIGHT));
		}
		else if (bSelected)
		{
			dc.FillSolidRect(rcCaption, pView->m_clrLine);
		}

		CXTPPropertyGridItem* pCategory = pItem->GetParentItem();
		while (pCategory != NULL)
		{
			if (pCategory->IsCategory())
				dc.FillSolidRect(rc.left + pCategory->GetIndent() * XTP_PGI_EXPAND_BORDER, rc.top, XTP_PGI_EXPAND_BORDER, rc.Height(), pView->m_clrLine);

			pCategory = pCategory->GetParentItem();
		}

		CXTPPenDC pen(dc, pView->m_clrLine);
		dc.MoveTo(0, rc.bottom - 1); dc.LineTo(rc.right, rc.bottom - 1);
		dc.MoveTo(rcCaption.right, rc.top); dc.LineTo(rcCaption.right, rc.bottom - 1);

		CRect rcText(rc);
		rcText.left = pItem->GetIndent() * XTP_PGI_EXPAND_BORDER + 3;
		rcText.right = rcCaption.right - 1;
		rcText.bottom -= 1;
		dc.DrawText( pItem->GetCaption(),   rcText,  DT_SINGLELINE|DT_NOPREFIX|DT_VCENTER);
	}

	CXTPPropertyGridItem* pNext = pView->GetItem(lpDrawItemStruct->itemID + 1);
	if (pNext && pNext->IsCategory() && pNext->IsVisible())
	{
		dc.FillSolidRect(pNext->GetIndent() * XTP_PGI_EXPAND_BORDER, rc.bottom - 1, rc.Width(), 1, m_clrFace);
	}



	if (pItem->HasChilds())
	{
		CRgn rgn;
		if (!pItem->IsCategory())
		{
			rgn.CreateRectRgnIndirect(&rcCaption);
			dc.SelectClipRgn(&rgn);
		}

		DrawExpandButton(dc, pItem, rcCaption);

		dc.SelectClipRgn(NULL);
	}
}

void CXTPPropertyGridPaintManager::DrawExpandButton(CDC& dc, CXTPPropertyGridItem* pItem, CRect rcCaption)
{
	CXTPPropertyGridView* pView = &m_pGrid->GetGridView();


	CRect rcSign(CPoint(XTP_PGI_EXPAND_BORDER / 2 - 5, rcCaption.CenterPoint().y - 4), CSize(9, 9));

	if (pItem->GetIndent() > 0)
		rcSign.OffsetRect((pItem->GetIndent() - (pItem->IsCategory()? 0: 1)) * XTP_PGI_EXPAND_BORDER, 0);


	if (m_bThemed && m_themeTree.IsAppThemed())
	{
		m_themeTree.DrawThemeBackground(dc, TVP_GLYPH, pItem->IsExpanded()? GLPS_OPENED: GLPS_CLOSED, &rcSign, 0);

	} else
	{

		CPoint pt = rcSign.CenterPoint();

		if (!pItem->IsCategory()) dc.FillSolidRect(rcSign, pView->m_clrBack);
		dc.Draw3dRect(rcSign, pView->m_clrFore, pView->m_clrFore);
		CXTPPenDC pen (dc,  pView->m_clrFore);

		dc.MoveTo(pt.x - 2, pt.y);
		dc.LineTo(pt.x + 3, pt.y);

		if(!pItem->IsExpanded())
		{
			dc.MoveTo(pt.x, pt.y - 2);
			dc.LineTo(pt.x, pt.y + 3);
		}
	}


}

void CXTPPropertyGridPaintManager::FillInplaceButton(CDC* pDC, CXTPPropertyGridInplaceButton* pButton)
{
	CXTPClientRect rect(pButton);
	CXTPPropertyGridView* pView = &m_pGrid->GetGridView();
	CPoint ptDots(rect.CenterPoint().x , rect.CenterPoint().y + 3);

	if (m_bThemed && m_themeButton.IsAppThemed() && m_themeCombo.IsAppThemed())
	{
		pDC->FillSolidRect(rect, pView->m_clrBack);
		pDC->FillSolidRect(rect.left, rect.top, rect.Width(), 1, pView->m_clrLine);
		pDC->FillSolidRect(rect.left, rect.bottom - 1, rect.Width(), 1, pView->m_clrLine);
		
		if (pButton->GetItem()->GetFlags() & xtpGridItemHasExpandButton)
		{
			m_themeButton.DrawThemeBackground(*pDC, BP_PUSHBUTTON, pButton->m_bPressed && pButton->m_bOver? PBS_PRESSED: 
				pButton->m_bOver? PBS_HOT: CWnd::GetFocus() == pButton? PBS_DEFAULTED: PBS_NORMAL, rect, 0);		
		}
		
		if (pButton->GetItem()->GetFlags() & xtpGridItemHasComboButton)
		{
			m_themeCombo.DrawThemeBackground(*pDC, CP_DROPDOWNBUTTON, 
				pButton->m_bPressed && pButton->m_bOver? CBXS_PRESSED: pButton->m_bOver? CBXS_HOT: CBXS_NORMAL, rect, 0);		
		} 
	}
	else
	{
		pDC->FillSolidRect(rect, GetXtremeColor(COLOR_3DFACE));

		if (pButton->m_bPressed && pButton->m_bOver) 
			pDC->Draw3dRect(rect, GetXtremeColor(COLOR_3DSHADOW), GetXtremeColor(COLOR_3DSHADOW));
		else
		{
			pDC->Draw3dRect(rect, GetXtremeColor(COLOR_3DFACE), GetXtremeColor(COLOR_3DDKSHADOW));
			rect.DeflateRect(1, 1);
			pDC->Draw3dRect(rect, GetXtremeColor(COLOR_WINDOW), GetXtremeColor(COLOR_3DSHADOW));
		}
		
		if (pButton->GetItem()->GetFlags() & xtpGridItemHasComboButton)
		{
			CPoint pt = rect.CenterPoint();
			CXTPDrawHelpers::Triangle(pDC, CPoint(pt.x -3, pt.y -1), CPoint(pt.x + 3, pt.y -1), CPoint(pt.x, pt.y +2), GetXtremeColor(COLOR_BTNTEXT));
		} 
	}

	if (pButton->GetItem()->GetFlags() & xtpGridItemHasExpandButton)
	{		
		pDC->FillSolidRect(ptDots.x - 4, ptDots.y, 2, 2, GetXtremeColor(COLOR_BTNTEXT));
		pDC->FillSolidRect(ptDots.x,  ptDots.y, 2, 2, GetXtremeColor(COLOR_BTNTEXT));
		pDC->FillSolidRect(ptDots.x + 4, ptDots.y, 2, 2, GetXtremeColor(COLOR_BTNTEXT));
	}

	if (CWnd::GetFocus() == pButton)
	{
		pDC->SetTextColor(GetXtremeColor(COLOR_BTNTEXT));
		pDC->SetBkColor(GetXtremeColor(COLOR_3DFACE));
		rect.DeflateRect(2, 2);
		pDC->DrawFocusRect(rect);

	}
}

//////////////////////////////////////////////////////////////////////
//

CXTPPropertyGridNativeXPTheme::CXTPPropertyGridNativeXPTheme(CXTPPropertyGrid* pGrid)
	: CXTPPropertyGridPaintManager(pGrid)
{
	m_bThemed = TRUE;

	RefreshMetrics();
}

void CXTPPropertyGridNativeXPTheme::RefreshMetrics()
{
	CXTPPropertyGridPaintManager::RefreshMetrics();

	XTPCurrentSystemTheme systemTheme = XTPDrawHelpers()->GetCurrentSystemTheme();

	switch (systemTheme)
	{
		case xtpSystemThemeSilver:
		case xtpSystemThemeBlue: m_clrShadow = RGB(127, 157, 185); break;
		case xtpSystemThemeOlive: m_clrShadow = RGB(164, 185, 127); break;
	}
}

//////////////////////////////////////////////////////////////////////
//

CXTPPropertyGridOffice2003Theme::CXTPPropertyGridOffice2003Theme(CXTPPropertyGrid* pGrid)
	: CXTPPropertyGridPaintManager(pGrid)
{
	RefreshMetrics();

}



void CXTPPropertyGridOffice2003Theme::RefreshMetrics()
{
	CXTPPropertyGridPaintManager::RefreshMetrics();

	m_bThemed = TRUE;
	m_bLunaTheme = FALSE;

	if (!XTPColorManager()->IsLunaColorsDisabled())
	{

		CXTPPropertyGridView* pView = &m_pGrid->GetGridView();

		XTPCurrentSystemTheme systemTheme = XTPDrawHelpers()->GetCurrentSystemTheme();

		switch (systemTheme)
		{
		case xtpSystemThemeBlue:
			m_clrFace = RGB(221, 236, 254);
			m_clrShadow = RGB(123, 164, 224);
			m_pGrid->m_clrHelpBack.SetStandardValue(m_clrFace);
			pView->m_clrLine.SetStandardValue(RGB(169, 199, 240));
			pView->m_clrCategoryFore.SetStandardValue(0);

			m_bLunaTheme = TRUE;
			break;

		case xtpSystemThemeOlive:
			m_clrFace = RGB(243, 242, 231);
			m_clrShadow = RGB(188, 187, 177);
			m_pGrid->m_clrHelpBack.SetStandardValue(m_clrFace);
			pView->m_clrLine.SetStandardValue(RGB(197, 212, 159));
			pView->m_clrCategoryFore.SetStandardValue(0);

			m_bLunaTheme = TRUE;
			break;

		case xtpSystemThemeSilver:
			m_clrFace = RGB(238, 238, 244);
			m_clrShadow = RGB(161, 160, 187);
			m_pGrid->m_clrHelpBack.SetStandardValue(m_clrFace);
			pView->m_clrLine.SetStandardValue(RGB(192, 192, 211));
			pView->m_clrCategoryFore.SetStandardValue(0);

			m_bLunaTheme = TRUE;
			break;
		}
	}
}


//////////////////////////////////////////////////////////////////////
// CXTPPropertyGridCoolTheme

CXTPPropertyGridCoolTheme::CXTPPropertyGridCoolTheme(CXTPPropertyGrid* pGrid)
	: CXTPPropertyGridPaintManager(pGrid)
{
	RefreshMetrics();
}

void CXTPPropertyGridCoolTheme::RefreshMetrics()
{
	CXTPPropertyGridPaintManager::RefreshMetrics();

	CXTPPropertyGridView* pView = &m_pGrid->GetGridView();

	pView->m_clrLine.SetStandardValue(GetXtremeColor(COLOR_3DFACE));

}


void CXTPPropertyGridCoolTheme::DrawItem(PDRAWITEMSTRUCT lpDrawItemStruct)
{
	CXTPPropertyGridView* pView = &m_pGrid->GetGridView();
	CXTPPropertyGridItem* pItem = (CXTPPropertyGridItem*)lpDrawItemStruct->itemData;

	COLORREF clrWindow = pView->m_clrBack;

	ASSERT(pItem != NULL);

	CXTPWindowRect rcWindow(pView);
	CRect rc = lpDrawItemStruct->rcItem;

	CXTPBufferDC dc(lpDrawItemStruct->hDC, rc);
	CXTPFontDC font(&dc, &pView->m_fontNormal);

	dc.FillSolidRect(rc, clrWindow);
	dc.SetBkMode(TRANSPARENT);


	CRect rcCaption(rc.left, rc.top, rc.left + pView->GetDividerPos(), rc.bottom);

	COLORREF clrLine = pView->m_clrLine;
	COLORREF clrFore = pView->m_clrFore;
	COLORREF clrShadow = GetXtremeColor(COLOR_3DSHADOW);

	if (pItem->IsCategory())
	{
		dc.FillSolidRect(rc, clrLine);
		CXTPFontDC font(&dc, &pView->m_fontBold);
		dc.SetTextColor(pView->m_clrCategoryFore);

		int nIndent = XTP_PGI_EXPAND_BORDER * (pItem->GetIndent() + 1);

		if ((lpDrawItemStruct->itemAction | ODA_FOCUS) && (lpDrawItemStruct->itemState & ODS_FOCUS))
		{
			dc.SetTextColor(::GetXtremeColor(COLOR_HIGHLIGHTTEXT));
			dc.FillSolidRect(CRect(nIndent, rc.top , rc.right, rc.bottom), ::GetXtremeColor(COLOR_HIGHLIGHT));
		}

		CRect rcText(nIndent + 3, rc.top , rc.right, rc.bottom - 1);
		dc.DrawText( pItem->GetCaption(),   rcText,  DT_SINGLELINE|DT_NOPREFIX|DT_VCENTER);

		dc.FillSolidRect((pItem->GetIndent() + (pItem->IsExpanded()? 1: 0)) * XTP_PGI_EXPAND_BORDER - 1, rc.bottom - 1, rc.Width(), 1, clrShadow);

		CXTPPropertyGridItem* pCategory = pItem->GetParentItem();
		while (pCategory != NULL)
		{
			if (pCategory->IsCategory())
			{
				dc.FillSolidRect(rc.left + pCategory->GetIndent() * XTP_PGI_EXPAND_BORDER, rc.top, XTP_PGI_EXPAND_BORDER, rc.Height(), clrLine);
				dc.FillSolidRect(rc.left + (pCategory->GetIndent() + 1) * XTP_PGI_EXPAND_BORDER - 1, rc.top, 1, rc.Height() + 2, clrShadow);
			}
			pCategory = pCategory->GetParentItem();
		}
	}
	else
	{
		dc.SetTextColor(pItem->GetReadOnly()? pView->m_clrReadOnlyFore: clrFore);

		CRect rcValue(rc);
		rcValue.left = rcCaption.right + 4;
		rcValue.bottom -= 1;

		if (!pItem->OnDrawItemValue(dc, rcValue))
		{
			dc.DrawText( pItem->GetViewValue(), rcValue,  DT_SINGLELINE|DT_VCENTER|DT_NOPREFIX);
		}

		BOOL bSelected = ((lpDrawItemStruct->itemAction | ODA_SELECT) && (lpDrawItemStruct->itemState & ODS_SELECTED));
		BOOL bFocused = bSelected && CWnd::GetFocus() && ((CWnd::GetFocus() == pView) || (CWnd::GetFocus()->GetParent() == pView) || (CWnd::GetFocus()->GetOwner() == pView));
		if (bFocused)
		{
			dc.SetTextColor(::GetXtremeColor(COLOR_HIGHLIGHTTEXT));
			dc.FillSolidRect(rcCaption, ::GetXtremeColor(COLOR_HIGHLIGHT));
		}
		else if (bSelected)
		{
			dc.FillSolidRect(rcCaption, clrLine);
		}

		CXTPPenDC pen(dc, clrLine);
		dc.MoveTo(0, rc.bottom - 1); dc.LineTo(rc.right, rc.bottom - 1);
		dc.MoveTo(rcCaption.right, rc.top); dc.LineTo(rcCaption.right, rc.bottom - 1);

		CXTPPropertyGridItem* pCategory = pItem->GetParentItem();
		while (pCategory != NULL)
		{
			if (pCategory->IsCategory())
			{
				dc.FillSolidRect(rc.left + pCategory->GetIndent() * XTP_PGI_EXPAND_BORDER, rc.top, XTP_PGI_EXPAND_BORDER, rc.Height(), clrLine);
				dc.FillSolidRect(rc.left + (pCategory->GetIndent() + 1) * XTP_PGI_EXPAND_BORDER - 1, rc.top, 1, rc.Height() + 2, clrShadow);
			}
			pCategory = pCategory->GetParentItem();
		}

		CRect rcText(rc);
		rcText.left = pItem->GetIndent() * XTP_PGI_EXPAND_BORDER + 3;
		rcText.right = rcCaption.right - 1;
		rcText.bottom -= 1;
		dc.DrawText( pItem->GetCaption(),   rcText,  DT_SINGLELINE|DT_NOPREFIX|DT_VCENTER);
	}

	CXTPPropertyGridItem* pNext = (CXTPPropertyGridItem*)pView->GetItemDataPtr(lpDrawItemStruct->itemID + 1);
	if (((ULONG_PTR)pNext == (ULONG_PTR)(-1)))
		dc.FillSolidRect(0, rc.bottom - 1, rc.Width(), 1, clrShadow);
	else if (pNext && pNext->IsCategory() && pNext->IsVisible())
	{
		dc.FillSolidRect(pNext->GetIndent() * XTP_PGI_EXPAND_BORDER, rc.bottom - 1, rc.Width(), 1, clrShadow);
	}

	CXTPPropertyGridItem* pPrev = pItem;
	CXTPPropertyGridItem* pCategory = pPrev->GetParentItem();
	while (pCategory != NULL)
	{
		if (pItem->HasChilds() && pItem->IsExpanded())
			break;

		if (pCategory->IsCategory() && pCategory->IsVisible() && pCategory->GetChilds()->GetAt(pCategory->GetChilds()->GetCount() - 1) == pPrev)
		{
			dc.FillSolidRect(pCategory->GetIndent() * XTP_PGI_EXPAND_BORDER, rc.bottom - 1, rc.Width(), 1, clrShadow);
		} else
			break;
		pPrev = pCategory;
		pCategory = pPrev->GetParentItem();
	}


	if (pItem->HasChilds())
	{
		CRgn rgn;
		if (!pItem->IsCategory())
		{
			rgn.CreateRectRgnIndirect(&rcCaption);
			dc.SelectClipRgn(&rgn);
		}

		DrawExpandButton(dc, pItem, rcCaption);

		dc.SelectClipRgn(NULL);
	}
}



//////////////////////////////////////////////////////////////////////
// CXTPPropertyGridSimpleTheme

CXTPPropertyGridSimpleTheme::CXTPPropertyGridSimpleTheme(CXTPPropertyGrid* pGrid)
	: CXTPPropertyGridPaintManager(pGrid)
{
	RefreshMetrics();
}

void CXTPPropertyGridSimpleTheme::RefreshMetrics()
{
	CXTPPropertyGridPaintManager::RefreshMetrics();

	CXTPPropertyGridView* pView = &m_pGrid->GetGridView();

	pView->m_clrLine.SetStandardValue(GetXtremeColor(COLOR_3DFACE));

}



void CXTPPropertyGridSimpleTheme::DrawItem(PDRAWITEMSTRUCT lpDrawItemStruct)
{
	CXTPPropertyGridView* pView = &m_pGrid->GetGridView();
	CXTPPropertyGridItem* pItem = (CXTPPropertyGridItem*)lpDrawItemStruct->itemData;

	COLORREF clrWindow = pView->m_clrBack;

	ASSERT(pItem != NULL);


	CXTPWindowRect rcWindow(pView);
	CRect rc = lpDrawItemStruct->rcItem;

	CXTPBufferDC dc(lpDrawItemStruct->hDC, rc);
	CXTPFontDC font(&dc, pItem->IsCategory()? &pView->m_fontBold: &pView->m_fontNormal);

	dc.FillSolidRect(rc, clrWindow);
	dc.SetBkMode(TRANSPARENT);


	CRect rcCaption(rc.left, rc.top, rc.left + pView->GetDividerPos(), rc.bottom);

	COLORREF clrLine = pView->m_clrLine;
	COLORREF clrFore = pView->m_clrFore;

	{
		if (!pItem->IsCategory())
		{

			dc.SetTextColor(pItem->GetReadOnly()? pView->m_clrReadOnlyFore: clrFore);

			CRect rcValue(rc);
			rcValue.left = rcCaption.right + 4;
			rcValue.bottom -= 1;

			if (!pItem->OnDrawItemValue(dc, rcValue))
			{
				dc.DrawText( pItem->GetViewValue(), rcValue,  DT_SINGLELINE|DT_VCENTER|DT_NOPREFIX);
			}
		}

		dc.SetTextColor(clrFore);

		BOOL bSelected = ((lpDrawItemStruct->itemAction | ODA_SELECT) && (lpDrawItemStruct->itemState & ODS_SELECTED));
		BOOL bFocused = bSelected && CWnd::GetFocus() && ((CWnd::GetFocus() == pView) || (CWnd::GetFocus()->GetParent() == pView) || (CWnd::GetFocus()->GetOwner() == pView));
		if (bFocused)
		{
			dc.SetTextColor(::GetXtremeColor(COLOR_HIGHLIGHTTEXT));
			dc.FillSolidRect(CRect(XTP_PGI_EXPAND_BORDER, rcCaption.top, rcCaption.right, rcCaption.bottom), ::GetXtremeColor(COLOR_HIGHLIGHT));
		}
		else if (bSelected)
		{
			dc.FillSolidRect(CRect(XTP_PGI_EXPAND_BORDER, rcCaption.top, rcCaption.right, rcCaption.bottom), clrLine);
		}
		dc.FillSolidRect(XTP_PGI_EXPAND_BORDER - 1, rc.top, 1, rc.Height(), clrLine);

		CXTPPenDC pen(dc, clrLine);
		dc.MoveTo(XTP_PGI_EXPAND_BORDER, rc.bottom - 1); dc.LineTo(rc.right, rc.bottom - 1);
		dc.MoveTo(rcCaption.right, rc.top); dc.LineTo(rcCaption.right, rc.bottom - 1);


		CRect rcText(rc);
		rcText.left = (pItem->GetIndent() + (pItem->IsCategory()? 1: 0)) * XTP_PGI_EXPAND_BORDER + 3;
		rcText.right = rcCaption.right - 1;
		rcText.bottom -= 1;
		dc.DrawText( pItem->GetCaption(),   rcText,  DT_SINGLELINE|DT_NOPREFIX|DT_VCENTER);
	}

	if (pView->GetCount() == (int)lpDrawItemStruct->itemID + 1)
		dc.FillSolidRect(0, rc.bottom - 1, rc.Width(), 1, clrLine);

	if (pItem->HasChilds())
	{
		CRgn rgn;
		rgn.CreateRectRgnIndirect(&rcCaption);
		dc.SelectClipRgn(&rgn);

		DrawExpandButton(dc, pItem, rcCaption);

		dc.SelectClipRgn(NULL);
	}
}



//////////////////////////////////////////////////////////////////////
// CXTPPropertyGridDelphiTheme

CXTPPropertyGridDelphiTheme::CXTPPropertyGridDelphiTheme(CXTPPropertyGrid* pGrid)
	: CXTPPropertyGridPaintManager(pGrid)
{
	RefreshMetrics();
}


void CXTPPropertyGridDelphiTheme::DrawItem(PDRAWITEMSTRUCT lpDrawItemStruct)
{
	CXTPPropertyGridView* pView = &m_pGrid->GetGridView();
	CXTPPropertyGridItem* pItem = (CXTPPropertyGridItem*)lpDrawItemStruct->itemData;

	COLORREF clrWindow = pView->m_clrBack;

	ASSERT(pItem != NULL);


	CXTPWindowRect rcWindow(pView);
	CRect rc = lpDrawItemStruct->rcItem;

	CXTPBufferDC dc(lpDrawItemStruct->hDC, rc);
	CXTPFontDC font(&dc, pItem->IsCategory()? &pView->m_fontBold: &pView->m_fontNormal);

	dc.FillSolidRect(rc, clrWindow);
	dc.SetBkMode(TRANSPARENT);


	CRect rcCaption(rc.left, rc.top, rc.left + pView->GetDividerPos(), rc.bottom);

	COLORREF clrFore = pView->m_clrFore;
	COLORREF clrShadow = GetXtremeColor(COLOR_3DSHADOW);
	COLORREF clrLight = GetXtremeColor(COLOR_BTNHIGHLIGHT);

	{
		if (!pItem->IsCategory())
		{

			dc.SetTextColor(pItem->GetReadOnly()? pView->m_clrReadOnlyFore: clrFore);

			CRect rcValue(rc);
			rcValue.left = rcCaption.right + 4;
			rcValue.bottom -= 1;

			if (!pItem->OnDrawItemValue(dc, rcValue))
			{
				dc.DrawText( pItem->GetViewValue(), rcValue,  DT_SINGLELINE|DT_VCENTER|DT_NOPREFIX);
			}
		}

		dc.SetTextColor(clrFore);

		dc.Draw3dRect(rcCaption.right - 1, rc.top, 1, rc.Height(), clrShadow, clrShadow);
		dc.Draw3dRect(rcCaption.right, rc.top, 1, rc.Height(), clrLight, clrLight);

		BOOL bSelected = ((lpDrawItemStruct->itemAction | ODA_SELECT) && (lpDrawItemStruct->itemState & ODS_SELECTED));
		BOOL bFocused = bSelected && CWnd::GetFocus() && ((CWnd::GetFocus() == pView) || (CWnd::GetFocus()->GetParent() == pView) || (CWnd::GetFocus()->GetOwner() == pView));
		if (bFocused || bSelected)
		{
			dc.Draw3dRect(rc, clrFore, clrLight);
		}
		else
		{
			for (int i = 0; i < rc.right; i += 2)
				dc.SetPixel(i, rc.bottom - 1, clrShadow);
		}


		CRect rcText(rc);
		rcText.left = (pItem->GetIndent() + (pItem->IsCategory()? 1: 0)) * XTP_PGI_EXPAND_BORDER + 3;
		rcText.right = rcCaption.right - 1;
		rcText.bottom -= 1;
		dc.DrawText( pItem->GetCaption(),   rcText,  DT_SINGLELINE|DT_NOPREFIX|DT_VCENTER);
	}

	if (pItem->HasChilds())
	{
		CRgn rgn;
		rgn.CreateRectRgnIndirect(&rcCaption);
		dc.SelectClipRgn(&rgn);

		DrawExpandButton(dc, pItem, rcCaption);

		dc.SelectClipRgn(NULL);
	}
}

void CXTPPropertyGridDelphiTheme::RefreshMetrics()
{
	CXTPPropertyGridPaintManager::RefreshMetrics();

	CXTPPropertyGridView* pView = &m_pGrid->GetGridView();

	pView->m_clrBack.SetStandardValue(GetXtremeColor(COLOR_3DFACE));
	pView->m_clrFore.SetStandardValue(RGB(0, 0, 128));

}

//////////////////////////////////////////////////////////////////////////
// CXTPPropertyGridWhidbeyTheme

CXTPPropertyGridWhidbeyTheme::CXTPPropertyGridWhidbeyTheme(CXTPPropertyGrid* pGrid)
	: CXTPPropertyGridPaintManager(pGrid)
{
	m_bThemed = TRUE;
	RefreshMetrics();
}

void CXTPPropertyGridWhidbeyTheme::DrawCategoryCaptionBackground(CDC* pDC, CRect rc)
{
	CXTPPropertyGridView* pView = &m_pGrid->GetGridView();

	pDC->FillSolidRect(rc, pView->m_clrLine);

	XTPDrawHelpers()->GradientFill(pDC,
		CRect(rc.left, rc.bottom - 2, rc.right, rc.bottom - 1), pView->m_clrLine, GetXtremeColor(COLOR_3DHIGHLIGHT), TRUE);

}

void CXTPPropertyGridWhidbeyTheme::RefreshMetrics()
{
	CXTPPropertyGridPaintManager::RefreshMetrics();

	CXTPPropertyGridView* pView = &m_pGrid->GetGridView();

	pView->m_clrLine.SetStandardValue(GetXtremeColor(COLOR_3DFACE));
	pView->m_clrCategoryFore.SetStandardValue(GetXtremeColor(COLOR_BTNTEXT));
	m_clrShadow = GetXtremeColor(XPCOLOR_STATICFRAME);
}

void CXTPPropertyGridWhidbeyTheme::DrawExpandButton(CDC& dc, CXTPPropertyGridItem* pItem, CRect rcCaption)
{

	CRect rcButton(CPoint(XTP_PGI_EXPAND_BORDER / 2 - 5, rcCaption.CenterPoint().y - 4), CSize(9, 9));

	if (pItem->GetIndent() > 0)
		rcButton.OffsetRect((pItem->GetIndent() - (pItem->IsCategory()? 0: 1)) * XTP_PGI_EXPAND_BORDER, 0);

	CDC* pDC = &dc;

	COLORREF clrButton = GetXtremeColor(COLOR_3DFACE);
	COLORREF clrFrame = GetXtremeColor(COLOR_3DSHADOW);
	pDC->FillSolidRect(rcButton.left + 1, rcButton.top + 1, 7, 7, clrButton);
	pDC->FillSolidRect(rcButton.left + 1, rcButton.top, 9 - 2, 1, clrFrame);
	pDC->FillSolidRect(rcButton.left + 1, rcButton.bottom - 1, 9 - 2, 1, clrFrame);
	pDC->FillSolidRect(rcButton.left, rcButton.top + 1, 1, 9 - 2, clrFrame);
	pDC->FillSolidRect(rcButton.right - 1, rcButton.top + 1, 1, 9 - 2, clrFrame);
	pDC->FillSolidRect(rcButton.left + 1, rcButton.top + 1, 9 - 2, 3, 0xFFFFFF);
	pDC->FillSolidRect(rcButton.left + 1, rcButton.top + 4, 9 - 4, 2, 0xFFFFFF);

	pDC->FillSolidRect(rcButton.left + 2, rcButton.top + 4, 9 - 4, 1, 0);
	if (!pItem->IsExpanded())
		pDC->FillSolidRect(rcButton.left + 4, rcButton.top + 2, 1, 9 - 4, 0);
}
