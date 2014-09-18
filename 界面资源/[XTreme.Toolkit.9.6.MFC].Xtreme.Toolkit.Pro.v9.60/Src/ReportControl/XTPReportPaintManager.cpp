// XTPReportPaintManager.cpp : implementation of the CXTPReportPaintManager class.
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
#include "XTPResourceManager.h"
#include "XTPDrawHelpers.h"
#include "XTPImageManager.h"
#include "XTPVC80Helpers.h"


#include "XTPReportPaintManager.h"
#include "XTPReportRow.h"
#include "XTPReportGroupRow.h"
#include "XTPReportColumn.h"
#include "XTPReportHeader.h"
#include "XTPReportControl.h"
#include "XTPReportControlResource.h"
#include "XTPReportInplaceControls.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////
// CXTPReportPaintManager

CXTPReportPaintManager::CXTPReportPaintManager()
{
	m_clrPreviewText.SetStandardValue(RGB(0,0,255));
	m_nMaxPreviewLines = 3;
	m_szGlyph = CSize(12, 12);

	// defaults
	m_nGroupGridLineHeight = 2;
	m_nResizingWidth = 1;
	m_bIsPreviewMode = FALSE;
	m_bShadeGroupHeadings = FALSE;
	m_bShadeSortColumn = TRUE;
	m_columnStyle = xtpColumnShaded;
	m_treeStructureStyle = xtpTreeStructureSolid;

	m_verticalGridStyle = xtpGridNoLines;
	m_horizontalGridStyle = xtpGridSolid;

	XTPResourceManager()->LoadString(&m_strNoItems, XTP_IDS_REPORT_NOITEMS);
	XTPResourceManager()->LoadString(&m_strNoGroupBy, XTP_IDS_REPORT_NOGROUPBY);
	XTPResourceManager()->LoadString(&m_strNoFieldsAvailable, XTP_IDS_REPORT_NOFIELDSAVAILABLE);


	m_bHideSelection = FALSE;

	m_rcPreviewIndent.SetRect(10, 0, 10, 2);

	m_nTreeIndent = 20;
	m_bDrawSortTriangleAlways = TRUE;


	// get system params
	LOGFONT lfIcon;
	VERIFY( ::SystemParametersInfo( SPI_GETICONTITLELOGFONT, sizeof( lfIcon ), &lfIcon, 0 ) );

	VERIFY(m_fontPreview.CreateFontIndirect(&lfIcon));

	SetCaptionFont(lfIcon);
	SetTextFont(lfIcon);

	VERIFY(m_ilGlyphs.Create(12,12, ILC_COLOR24|ILC_MASK, 0, 1));
	CBitmap bmp;
	VERIFY(XTPResourceManager()->LoadBitmap(&bmp, XTP_IDB_REPORT_GLYPHS));
	m_ilGlyphs.Add(&bmp, RGB(255, 0, 255));

	m_bFixedRowHeight = TRUE;



	RefreshMetrics();

}

CXTPReportPaintManager::~CXTPReportPaintManager()
{
}

COLORREF CXTPReportPaintManager::GetControlBackColor(CXTPReportControl* pControl)
{
	UNREFERENCED_PARAMETER(pControl);
	return m_clrControlBack;
}


void CXTPReportPaintManager::RefreshMetrics()
{
	XTPColorManager()->RefreshColors();

	m_clrBtnFace.SetStandardValue(GetXtremeColor(COLOR_BTNFACE));
	m_clrBtnText.SetStandardValue(GetXtremeColor(COLOR_BTNTEXT));
	m_clrControlDark.SetStandardValue(GetXtremeColor(COLOR_3DSHADOW));
	m_clrHighlight.SetStandardValue(GetXtremeColor(COLOR_HIGHLIGHT));
	m_clrHighlightText.SetStandardValue(GetXtremeColor(COLOR_HIGHLIGHTTEXT));
	m_clrWindowText.SetStandardValue(GetXtremeColor(COLOR_WINDOWTEXT));
	m_clrControlLightLight.SetStandardValue(GetXtremeColor(COLOR_3DHIGHLIGHT));
	m_clrControlBack.SetStandardValue(GetXtremeColor(COLOR_WINDOW));

	m_clrIndentControl.SetStandardValue(m_clrBtnFace);
	m_clrCaptionText.SetStandardValue(m_clrBtnText);
	m_clrGridLine.SetStandardValue(m_clrControlDark);
	m_clrHeaderControl.SetStandardValue(m_clrBtnFace);
	m_clrGroupShadeBorder.SetStandardValue(m_clrControlDark);
	m_clrGroupShadeBack.SetStandardValue(m_clrBtnFace);
	m_clrGroupRowText.SetStandardValue(m_clrControlDark);
	m_clrGroupBoxBack.SetStandardValue(m_clrControlDark);

	m_clrHyper.SetStandardValue(RGB(0, 0, 0xFF));
	m_clrItemShade.SetStandardValue(RGB(245, 245, 245));
	m_clrHotDivider.SetStandardValue(RGB(0xFF, 0, 0));

	if (!XTPColorManager()->IsLunaColorsDisabled())
	{

		XTPCurrentSystemTheme systemTheme = XTPDrawHelpers()->GetCurrentSystemTheme();

		switch (systemTheme)
		{
			case xtpSystemThemeBlue:
				m_clrGroupShadeBorder.SetStandardValue(RGB(123, 164, 224));
				m_clrIndentControl.SetStandardValue(RGB(253, 238, 201));
				m_clrGroupShadeBack.SetStandardValue(RGB(190, 218, 251));
				m_clrGroupRowText.SetStandardValue(RGB(55, 104, 185));
				m_clrGridLine.SetStandardValue(RGB(234, 233, 225));
				break;

			case xtpSystemThemeOlive:
				m_clrGroupShadeBorder.SetStandardValue(RGB(181, 196, 143));
				m_clrIndentControl.SetStandardValue(RGB(253, 238, 201));
				m_clrGroupShadeBack.SetStandardValue(RGB(175, 186, 145));
				m_clrGroupRowText.SetStandardValue(RGB(115, 137, 84));
				m_clrGridLine.SetStandardValue(RGB(234, 233, 225));
				break;

			case xtpSystemThemeSilver:
				m_clrGroupShadeBorder.SetStandardValue(RGB(165, 164, 189));
				m_clrIndentControl.SetStandardValue(RGB(253, 238, 201));
				m_clrGroupShadeBack.SetStandardValue(RGB(229, 229, 235));
				m_clrGroupRowText.SetStandardValue(RGB(112, 111, 145));
				m_clrGridLine.SetStandardValue(RGB(229, 229, 235));
				break;
		}
	}

	m_brushVeriticalGrid.DeleteObject();
	m_brushHorizontalGrid.DeleteObject();
}


void CXTPReportPaintManager::FillRow(CDC* /*pDC*/, CXTPReportRow* /*pRow*/, CRect /*rcRow*/)
{
}

void CXTPReportPaintManager::DrawGridPat(CDC* pDC, CBrush* pBrush, CRect rc, CONST VOID*pPattern)
{
	if (pBrush->GetSafeHandle() == 0)
	{
		CBitmap bmp;
		bmp.CreateBitmap(8, 8, 1, 1, pPattern);
		pBrush->CreatePatternBrush(&bmp);
	}
	CBrush* pOldBrush = (CBrush*)pDC->SelectObject(pBrush);

	pDC->SetTextColor(m_clrControlBack);
	pDC->SetBkColor(m_clrGridLine);
	PatBlt(pDC->GetSafeHdc(), rc.left, rc.top, rc.Width(), rc.Height(), PATCOPY);

	pDC->SelectObject(pOldBrush);
}

void CXTPReportPaintManager::SetGridStyle(BOOL bVertical, XTPReportGridStyle gridStyle)
{
	if (bVertical)
	{
		m_verticalGridStyle = gridStyle;
		m_brushVeriticalGrid.DeleteObject();
	}
	else
	{
		m_horizontalGridStyle = gridStyle;
		m_brushHorizontalGrid.DeleteObject();
	}
}

void CXTPReportPaintManager::DrawGrid(CDC* pDC, BOOL bVertical, CRect rc)
{
	if (!bVertical)
	{
		switch (m_horizontalGridStyle)
		{
			case xtpGridSolid:
				DrawHorizontalLine(pDC, rc.left, rc.bottom, rc.Width(), m_clrGridLine);
				break;
			case xtpGridSmallDots:
				{
					const unsigned short _cb[] = {0xaa,0x55,0xaa,0x55,0xaa,0x55,0xaa,0x55};
					DrawGridPat(pDC, &m_brushHorizontalGrid, CRect(rc.left, rc.bottom, rc.right, rc.bottom + 1), _cb);
					break;
				}
			case xtpGridLargeDots:
				{
					const unsigned short _cb[] = {0xcc,0x33,0xcc,0x33,0xcc,0x33,0xcc,0x33};
					DrawGridPat(pDC, &m_brushHorizontalGrid, CRect(rc.left, rc.bottom, rc.right, rc.bottom + 1), _cb);
					break;
				}
			case xtpGridDashes:
				{
					const unsigned short _cb[] = {0xe0,0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0};
					DrawGridPat(pDC, &m_brushHorizontalGrid, CRect(rc.left, rc.bottom, rc.right, rc.bottom + 1), _cb);
					break;

				}

		}
	} else
	{
		int nPos = rc.right - 1;
		switch (m_verticalGridStyle)
		{
			case xtpGridSolid:
				DrawVerticalLine(pDC, nPos, rc.top, rc.Height(), m_clrGridLine);
				break;
			case xtpGridSmallDots:
				{
					const unsigned short _cb[] = {0xaa,0x55,0xaa,0x55,0xaa,0x55,0xaa,0x55};
					DrawGridPat(pDC, &m_brushVeriticalGrid, CRect(nPos, rc.top, nPos + 1, rc.bottom), _cb);
					break;
				}
			case xtpGridLargeDots:
				{
					const unsigned short _cb[] = {0xcc,0xcc,0x33, 0x33,0xcc,0xcc,0x33,0x33};
					DrawGridPat(pDC, &m_brushVeriticalGrid, CRect(nPos, rc.top, nPos + 1, rc.bottom), _cb);
					break;
				}
			case xtpGridDashes:
				{
					const unsigned short _cb[] = {0xf0,0xf0, 0xf0, 0xf0, 0xf, 0xf, 0xf, 0xf};
					DrawGridPat(pDC, &m_brushVeriticalGrid, CRect(nPos, rc.top, nPos + 1, rc.bottom), _cb);
					break;

				}

		}

	}
}


void CXTPReportPaintManager::DrawGroupRow(CDC* pDC, CXTPReportGroupRow* pRow, CRect rcRow)
{
	BOOL bControlFocused = pRow->GetControl()->HasFocus();

	pDC->SetTextColor(m_clrGroupRowText);

	if (pRow->IsSelected() && bControlFocused && !pDC->IsPrinting())
	{
		pDC->FillSolidRect(rcRow,  m_clrHighlight);
		pDC->SetTextColor(m_clrHighlightText);
	}
	else if (m_bShadeGroupHeadings)
	{
		pDC->FillSolidRect(rcRow, m_clrGroupShadeBack);
		pDC->SetTextColor(m_clrWindowText);
	}

	CXTPFontDC font(pDC, &m_fontText);
	int nBitmapWidth = 5;

	int nRowLevel = pRow->GetTreeDepth();
	int nIndent = pRow->GetControl()->GetIndent(nRowLevel + 1);

	pDC->FillSolidRect(rcRow.left, rcRow.bottom - m_nGroupGridLineHeight, rcRow.Width(), m_nGroupGridLineHeight, m_clrGroupShadeBorder);

	if (pRow->IsFocused() && bControlFocused && !pDC->IsPrinting())
	{
		DrawFocusedRow(pDC, CRect(rcRow.left + nIndent, rcRow.top, rcRow.right, rcRow.bottom - 1));
	}

	if (nIndent > 0)
	{
		FillIndent(pDC, CRect(rcRow.left, rcRow.top, rcRow.left + nIndent, rcRow.bottom));
	}
	rcRow.left += nIndent;

	CRect rcBitmap(rcRow);
	rcBitmap.top = rcBitmap.bottom - m_nRowHeight - 12;
	rcBitmap.top = rcBitmap.CenterPoint().y - m_szGlyph.cy / 2;

	nBitmapWidth += DrawCollapsedBitmap(pDC, pRow, rcBitmap);

	CRect rcText(nBitmapWidth, rcRow.top, rcRow.right, rcRow.bottom);
	rcText.top = rcText.bottom - m_nRowHeight - 6;
	pDC->DrawText(pRow->GetCaption(), rcText, DT_SINGLELINE|DT_END_ELLIPSIS|DT_VCENTER|DT_NOPREFIX);


}

int CXTPReportPaintManager::DrawCollapsedBitmap(CDC* pDC, CXTPReportRow* pRow, CRect rcBitmap)
{
	rcBitmap.left += 2;

	if (pDC && pDC->IsPrinting())
		pDC = NULL;

	int nNewIndent = DrawGlyph(pDC, rcBitmap, pRow->IsExpanded()? 0: 1);

	if (nNewIndent != 0 && pDC)
	{
		rcBitmap.right = nNewIndent;
		pRow->SetCollapseRect(rcBitmap);
	}
	return nNewIndent;
}

void CXTPReportPaintManager::FillIndent(CDC* pDC, CRect rcRow)
{
	pDC->FillSolidRect(&rcRow, m_clrIndentControl);
}

void CXTPReportPaintManager::FillItemShade(CDC* pDC, CRect rcItem)
{
	// fill item background shade (for example when sorted by this column)
	if (!m_bShadeSortColumn)
		return;

	if (pDC)
	{
		pDC->FillSolidRect(rcItem, m_clrItemShade);
	}
}

void CXTPReportPaintManager::FillHeaderControl(CDC* pDC, CRect rcHeader)
{
	pDC->FillSolidRect(rcHeader, m_clrHeaderControl);
}

void CXTPReportPaintManager::DrawHorizontalLine(CDC* pDC, int x, int y, int cx, COLORREF clr)
{
	pDC->FillSolidRect(x, y, cx, 1, clr);
}

void CXTPReportPaintManager::DrawVerticalLine(CDC* pDC, int x, int y, int cy, COLORREF clr)
{
	pDC->FillSolidRect(x, y, 1, cy, clr);
}

void CXTPReportPaintManager::DrawTriangle(CDC* pDC, CRect rcTriangle, BOOL bToDown)
{
	CRect rcTRect;

	if (rcTriangle.Width()  > 15)
	{
		CPoint pt(rcTriangle.left + 10, rcTriangle.CenterPoint().y);

		// Set up pens to use for drawing the triangle
		CPen penLight(PS_SOLID, 1, m_clrControlLightLight);
		CPen penShadow(PS_SOLID, 1, m_clrControlDark);

		if (bToDown)
		{
			Line(pDC, pt.x, pt.y - 3 + 6, 3, -6, &penLight);
			Line(pDC, pt.x, pt.y - 2 + 6, 3, -6, &penLight);
			Line(pDC, pt.x - 4, pt.y - 2, 3, +6, &penShadow);
			Line(pDC, pt.x - 1, pt.y - 2 + 6, -4, -7, &penShadow);
			Line(pDC, pt.x - 4, pt.y - 2, 8, 0, &penShadow);
		}
		else
		{
			Line(pDC, pt.x, pt.y - 3, 3, 6, &penLight);
			Line(pDC, pt.x, pt.y - 2, 3, 6, &penLight);
			Line(pDC, pt.x - 3, pt.y + 3, 6, 0, &penLight);
			Line(pDC, pt.x - 1, pt.y - 3, -3, 6, &penShadow);
			Line(pDC, pt.x - 2, pt.y - 2, -3, 6, &penShadow);
		}
	}
}

int CXTPReportPaintManager::DrawBitmap(CDC* pDC, CXTPReportControl* pControl, CRect rcColumn, int iIcon)
{
	int nImageBound = rcColumn.left;

	ASSERT(pControl);

	POINT ptIcon;

	CXTPImageManagerIcon* pIcon = pControl->GetImageManager()->GetImage(iIcon, 0);

	if (!pIcon)
		return nImageBound;

	CSize szImage(pIcon->GetWidth(), pIcon->GetHeight());
	CSize szColumn(rcColumn.Size());

	if (szImage.cx < szColumn.cx && szImage.cy < szColumn.cy && pDC)
	{
		ptIcon.x = rcColumn.left + 1;
		ptIcon.y = rcColumn.top + (szColumn.cy - szImage.cy) /2;


		pIcon->Draw(pDC, ptIcon, pIcon->GetIcon());
	}

	return nImageBound + 1 + szImage.cx;
}

int CXTPReportPaintManager::DrawGlyph(CDC* pDC, CRect rcColumn, int iIcon)
{
	int nImageBound = rcColumn.left;

	CImageList* pImageList = &m_ilGlyphs;

	IMAGEINFO imgInf;

	if (!pImageList->GetImageInfo( iIcon, &imgInf ))
		return nImageBound;

	CSize szImage(imgInf.rcImage.right - imgInf.rcImage.left, imgInf.rcImage.bottom - imgInf.rcImage.top);
	CSize szColumn(rcColumn.Size());

	if (szImage.cy < szColumn.cy && pDC)
	{
		POINT ptIcon;

		ptIcon.x = rcColumn.left + 1;
		ptIcon.y = rcColumn.top + (szColumn.cy - szImage.cy) /2;

		pImageList->Draw(pDC, iIcon, ptIcon, ILD_TRANSPARENT);
	}

	return nImageBound + 1 + szImage.cx;
}

int CXTPReportPaintManager::GetRowHeight(CDC* /*pDC*/, CXTPReportRow* pRow)
{
	if (!pRow->IsGroupRow())
	{
		return m_nRowHeight + (IsGridVisible(FALSE)? 1: 0);
	}

	if (m_bShadeGroupHeadings)
		return m_nRowHeight + 6;

	return m_nRowHeight + 16;
}

void CXTPReportPaintManager::DrawColumn(CDC* pDC, CXTPReportColumn* pColumn, CXTPReportHeader* pHeader, CRect rcColumn, BOOL bDrawExternal)
{
	// save column parameter for future use in drawing

	BOOL bDraggingFromHeader = bDrawExternal && pColumn->IsDragging() &&
						rcColumn.left == 0 && rcColumn.top == 0 && pHeader->IsDragHeader();

	BOOL bPlainColumn = bDrawExternal && !bDraggingFromHeader;


	// draw
	int nIconID = pColumn->GetIconID();
	CString strCaption = pColumn->GetCaption();
	int nShadowWidth = 0;

	if (bPlainColumn)
	{
		pDC->Draw3dRect(rcColumn, m_clrControlLightLight, m_clrControlDark);
		pDC->MoveTo(rcColumn.left,  rcColumn.bottom);
		pDC->LineTo(rcColumn.right, rcColumn.bottom);
		pDC->LineTo(rcColumn.right, rcColumn.top-1);
	}
	else
	{
		if (m_columnStyle == xtpColumnShaded)
		{
			nShadowWidth = 4;

			DrawVerticalLine(pDC, rcColumn.right - 1, rcColumn.top + 2, rcColumn.Height() - 2 - nShadowWidth, m_clrControlDark);

			DrawHorizontalLine(pDC, rcColumn.left, rcColumn.bottom - 3, rcColumn.Width(), MixColor(m_clrHeaderControl, m_clrControlDark, 0.25));
			DrawHorizontalLine(pDC, rcColumn.left, rcColumn.bottom - 2, rcColumn.Width(), MixColor(m_clrHeaderControl, m_clrControlDark, 0.4));
			DrawHorizontalLine(pDC, rcColumn.left, rcColumn.bottom - 1, rcColumn.Width(), MixColor(m_clrHeaderControl, m_clrControlDark, 0.6));

			DrawVerticalLine(pDC, rcColumn.left, rcColumn.top + 2, rcColumn.Height() - 2 - nShadowWidth, m_clrControlLightLight);
		}
		else
		{
			pDC->Draw3dRect(rcColumn, m_clrControlLightLight, m_clrControlDark);
		}
	}

	rcColumn.bottom = rcColumn.bottom - 1 - nShadowWidth;

	CRect rcText(rcColumn);

	if (!pColumn->IsAutoSize() && !bPlainColumn)
	{
		rcText.left += pColumn->GetIndent();
	}
	if (nIconID != XTP_REPORT_NOICON && !bPlainColumn)
	{
		CRect rcIcon(rcText);
		rcIcon.bottom += nShadowWidth;
		// shift column left by bitmap drawing place
		rcText.left = DrawBitmap(pDC, pColumn->GetControl(), rcIcon, nIconID);
		// shift column left to the column right to disallow drawing text on Bitmap column headers
		rcText.left += 2;
	}

	pDC->SetBkMode(TRANSPARENT);

	BOOL bHasSortTriangle = pColumn->HasSortTriangle() && !pDC->IsPrinting();
	rcText.DeflateRect(3, 0, 3, 0);
	CRect rcTriangle(rcText);

	if (strCaption.GetLength() > 0 && rcText.right > rcText.left)
	{
		CXTPFontDC font(pDC, &m_fontCaption);
		CSize sizeText = pDC->GetTextExtent(pColumn->GetCaption());
		pDC->SetTextColor(m_clrCaptionText);

		if (m_bDrawSortTriangleAlways && bHasSortTriangle)
		{
			sizeText.cx = max(-5, min(sizeText.cx, rcText.Width() - 27));
			rcText.right = min(rcText.right, rcText.left + sizeText.cx + 8);
		}

		pDC->DrawText(pColumn->GetCaption(), rcText, DT_END_ELLIPSIS|DT_VCENTER|DT_SINGLELINE|DT_NOPREFIX);
		rcTriangle.left += sizeText.cx;
	}


	if (bHasSortTriangle)
	{
		rcTriangle.left += (bPlainColumn? 17: 10);
		DrawTriangle(pDC, rcTriangle, pColumn->IsSortedDecreasing());
	}

	if (pColumn->IsDragging() && !bDrawExternal && pHeader->IsDragHeader())
	{
		rcColumn.bottom += nShadowWidth;
		pDC->InvertRect(&rcColumn);
	}
}

void CXTPReportPaintManager::FillGroupByControl(CDC* pDC, CRect& rcGroupBy)
{
	pDC->FillSolidRect(rcGroupBy, m_clrGroupBoxBack);
}

void CXTPReportPaintManager::DrawConnector(CDC* pDC, CPoint ptFrom, CPoint ptTo)
{
	COLORREF crConnector = RGB(0, 0, 0);
	DrawVerticalLine(pDC, ptFrom.x, ptFrom.y, ptTo.y - ptFrom.y, crConnector);
	DrawHorizontalLine(pDC, ptFrom.x, ptTo.y, ptTo.x - ptFrom.x, crConnector);
}

void CXTPReportPaintManager::DrawFocusedRow(CDC* pDC, CRect rcRow)
{
	COLORREF clrTextColor = pDC->SetTextColor(m_clrControlBack);
	COLORREF clrBkColor = pDC->SetBkColor(m_clrWindowText);
	pDC->DrawFocusRect(rcRow);
	pDC->SetTextColor(clrTextColor);
	pDC->SetBkColor(clrBkColor);
}


int CXTPReportPaintManager::GetPreviewLinesCount(CDC* pDC, CRect& rcText, CString strText)
{
	if (rcText.Width() < 1)
		return 0;

	if (pDC->GetTextExtent(strText).cx / rcText.Width() > m_nMaxPreviewLines)
		return m_nMaxPreviewLines;

	CRect rcCalc(rcText.left, 0, rcText.right, 0);
	pDC->DrawText(strText, rcCalc, DT_WORDBREAK|DT_CALCRECT|DT_NOPREFIX);

	return min(rcCalc.Height() / pDC->GetTextExtent(_T(" ")).cy, m_nMaxPreviewLines);

}


void CXTPReportPaintManager::SetTextFont(LOGFONT& lf)
{
	m_fontText.DeleteObject();
	m_fontBoldText.DeleteObject();

	m_fontText.CreateFontIndirect(&lf);

	lf.lfWeight = FW_BOLD;
	m_fontBoldText.CreateFontIndirect(&lf);

	CWindowDC dc (NULL);
	CXTPFontDC font(&dc, &m_fontText);

	int nFontHeight = dc.GetTextExtent(_T(" ")).cy;
	m_nRowHeight = max(18, nFontHeight + 4);

}

void CXTPReportPaintManager::SetCaptionFont(LOGFONT& lf)
{
	m_fontCaption.DeleteObject();
	VERIFY(m_fontCaption.CreateFontIndirect(&lf));

	CWindowDC dc (NULL);
	CXTPFontDC font(&dc, &m_fontCaption);

	font.SetFont(&m_fontCaption);
	int nFontHeight = dc.GetTextExtent(_T(" ")).cy;
	m_nHeaderHeight = max(22, nFontHeight + 8);

}

void CXTPReportPaintManager::SetPreviewIndent(int nLeft, int nTop, int nRight, int nBottom)
{
	m_rcPreviewIndent.SetRect(nLeft, nTop, nRight, nBottom);
}

void CXTPReportPaintManager::DrawInplaceButton(CDC* pDC, CXTPReportInplaceButton* pButton)
{
	DrawInplaceButtonFrame(pDC, pButton);

	CXTPClientRect rect((CWnd*)pButton);

	CXTPPenDC pen (*pDC, GetXtremeColor(COLOR_BTNTEXT));
	CXTPBrushDC brush (*pDC, GetXtremeColor(COLOR_BTNTEXT));

	if (pButton->GetID() == XTP_ID_REPORT_COMBOBUTTON)
	{
		CPoint pt = rect.CenterPoint();

		CPoint pts[3];
		pts[0] = CPoint(pt.x -3, pt.y -1);
		pts[1] = CPoint(pt.x + 3, pt.y -1);
		pts[2] = CPoint(pt.x, pt.y +2);
		pDC->Polygon(pts, 3);
	}

	if (pButton->GetID() == XTP_ID_REPORT_EXPANDBUTTON)
	{
		pDC->Rectangle(rect.left + 3, rect.bottom -5, rect.left + 5, rect.bottom -3);
		pDC->Rectangle(rect.left + 7, rect.bottom -5, rect.left + 9, rect.bottom -3);
		pDC->Rectangle(rect.left + 11, rect.bottom -5, rect.left + 13, rect.bottom -3);
	}

}

void CXTPReportPaintManager::DrawInplaceButtonFrame(CDC* pDC, CXTPReportInplaceButton* pButton)
{
	CXTPClientRect rect((CWnd*)pButton);

	pDC->FillSolidRect(rect, GetXtremeColor(COLOR_3DFACE));

	if (pButton->IsPressed())
	{
		pDC->Draw3dRect(rect, GetXtremeColor(COLOR_3DDKSHADOW), GetXtremeColor(COLOR_WINDOW));
	}
	else
	{
		pDC->Draw3dRect(rect, GetXtremeColor(COLOR_3DFACE), GetXtremeColor(COLOR_3DDKSHADOW));
		rect.DeflateRect(1, 1);
		pDC->Draw3dRect(rect, GetXtremeColor(COLOR_WINDOW), GetXtremeColor(COLOR_3DSHADOW));
	}
}

void CXTPReportPaintManager::DrawTreeStructure(XTP_REPORTRECORDITEM_DRAWARGS* pDrawArgs, XTP_REPORTRECORDITEM_METRICS* pMetrics, CRect rcItem)
{
	if (m_treeStructureStyle != xtpTreeStructureSolid)
		return;

	CDC* pDC = pDrawArgs->pDC;
	CXTPReportRow* pRow = pDrawArgs->pRow;
	COLORREF clrTreeStructure = pMetrics->clrForeground;

	if (!pRow->GetParentRow() || pRow->GetParentRow()->IsGroupRow())
		return;

	int nTreeIndent = pDrawArgs->pControl->GetPaintManager()->m_nTreeIndent;
	int nHeight = rcItem.Height();

	if (pRow->IsLastTreeRow())
	{
		pDC->FillSolidRect(rcItem.left, rcItem.top - 1, 1, nHeight / 2 + 1, clrTreeStructure);
	}
	else
	{
		pDC->FillSolidRect(rcItem.left, rcItem.top - 1, 1, nHeight + 1, clrTreeStructure);
	}

	pDC->FillSolidRect(rcItem.left, rcItem.top + (nHeight / 2), 12, 1, clrTreeStructure);

	while (TRUE)
	{
		pRow = pRow->GetParentRow();

		if (pRow == NULL || pRow->GetParentRow() == 0 || pRow->IsGroupRow())
			break;

		rcItem.OffsetRect(-nTreeIndent, 0);

		if (!pRow->IsLastTreeRow())
		{
			pDC->FillSolidRect(rcItem.left, rcItem.top - 1, 1, rcItem.Height() + 1, clrTreeStructure);
		}
	}

}

int CXTPReportPaintManager::DrawString(int* pnCurrDrawPos, CDC* pDC, CString strDraw, CRect rcDraw, UINT nFormat)
{
	int nCharCount = 0;


	switch (nFormat)
	{
		case DT_END_ELLIPSIS:
		{
			// in this case char count only for identify if string printed partially
			*pnCurrDrawPos += pDC->GetTextExtent(strDraw).cx;
			if(*pnCurrDrawPos >= rcDraw.right)
				nCharCount = - 1;
			else
				nCharCount = strDraw.GetLength();

			pDC->DrawText(strDraw, rcDraw, DT_LEFT|DT_END_ELLIPSIS|DT_NOPREFIX); //
			break;
		}
		case DT_WORDBREAK:
		{
			const TCHAR seps[]   = _T(" \t\n");
			TCHAR* lpszContext = 0;
			CString strOut = _T("");
			CString strSrc = strDraw;
			TCHAR* szWord = STRTOK_S(strSrc.GetBuffer(strSrc.GetLength()), seps, &lpszContext);
			int nRightStrMargin = pDC->GetTextExtent(szWord).cx;

			if (nRightStrMargin > rcDraw.Width())
			{
				strOut = szWord;

			} else
			{
				while( szWord != NULL && nRightStrMargin < rcDraw.Width())
				{
					strOut = strOut + szWord + _T(" ");
						szWord = STRTOK_S( NULL, seps, &lpszContext );
						nRightStrMargin = pDC->GetTextExtent(strOut + szWord).cx;
				}
			}

			nCharCount = strOut.GetLength();
			*pnCurrDrawPos += pDC->GetTextExtent(strOut).cx;
			pDC->DrawText(strOut, rcDraw, DT_LEFT|DT_NOPREFIX);
			break;
		}
	} // switch

	return nCharCount;
}

int CXTPReportPaintManager::DrawLink(int* pnCurrDrawPos, XTP_REPORTRECORDITEM_DRAWARGS* pDrawArgs, CXTPReportHyperlink* pHyperlink,
	CString strText, CRect rcLink, int nFlag)
{
	CDC* pDC = pDrawArgs->pDC;
	CString strOut = strText.Mid(pHyperlink->m_nHyperTextBegin, pHyperlink->m_nHyperTextLen);

	COLORREF clrPrev = pDC->SetTextColor(pDrawArgs->pControl->GetPaintManager()->m_clrHyper);

	TEXTMETRIC textMetrics;
	pDC->GetTextMetrics(&textMetrics);
	int nCharCount = 0;
	//!pHyperlink->m_rcHyperSpot.left = pDC->GetCurrentPosition().x; // update hyperlink left bound before drawing
	pHyperlink->m_rcHyperSpot.left = *pnCurrDrawPos;
	nCharCount = DrawString(pnCurrDrawPos, pDC, strOut, rcLink, nFlag);

	// update hyperlink's spot
	pHyperlink->m_rcHyperSpot.top = rcLink.top;
	//!pHyperlink->m_rcHyperSpot.right = pDC->GetCurrentPosition().x;
	pHyperlink->m_rcHyperSpot.right = *pnCurrDrawPos;
	pHyperlink->m_rcHyperSpot.bottom = rcLink.top + textMetrics.tmHeight;

	pDC->SetTextColor(clrPrev);

	return nCharCount;
}

void CXTPReportPaintManager::DrawTextLine(XTP_REPORTRECORDITEM_DRAWARGS* pDrawArgs, CString strText,
	CRect rcItem, int nFlag, int& nCharCounter, int& nHyperlikCounter)
{
	CDC* pDC = pDrawArgs->pDC;
	CXTPReportRecordItem* pItem = pDrawArgs->pItem;
	UINT nAlingFlag = 0;
	int nHyperlink = nHyperlikCounter; // Hyperlink index
	int nCharCount = nCharCounter; // Number of actually printed chars
	CXTPReportHyperlink* pHl = NULL;
	int nHyperlinks = pItem->GetHyperlinksCount();

	int nCurrStrPos = nCharCount;
	CRect rcText;
	rcText.CopyRect(&rcItem);

	if (pDrawArgs->pRow->IsSelected())
		nHyperlinks = 0;

	if (nHyperlinks == 0 && nFlag == DT_END_ELLIPSIS)
	{
		pDC->DrawText(strText, rcText, pDrawArgs->nTextAlign|DT_END_ELLIPSIS|DT_NOPREFIX);
		return;
	}


	// calculate rect for draving text for text alignment feature
	if (nFlag == DT_END_ELLIPSIS)
	{
		pDC->DrawText(strText, rcText, DT_CALCRECT|DT_NOPREFIX);
		if (rcText.Width() < rcItem.Width())
			switch (pDrawArgs->nTextAlign)
			{
				case DT_RIGHT :
					rcText.left = rcItem.right - rcText.Width();
					break;
				case DT_CENTER :
					rcText.left = rcItem.left + (rcItem.Width() - rcText.Width())/2;
					break;
			} // swith

		rcText.right = rcItem.right;
	}

	int nCurrDrawPos = rcText.left;

	while (strText.GetLength() > nCurrStrPos && nCurrDrawPos < rcText.right)
	{
		CString strOut;
		if (nHyperlinks > nHyperlink)
		{
			pHl = pItem->GetHyperlinkAt(nHyperlink);
			strOut = strText.Mid(nCurrStrPos, pHl->m_nHyperTextBegin - nCurrStrPos);
			if (strOut.GetLength() > 0) // print str
			{
				nCharCount = DrawString(&nCurrDrawPos, pDC, strOut, rcText, nFlag);
				rcText.left = nCurrDrawPos > rcText.right ? rcText.right : nCurrDrawPos;

				// if previous string was cut - stop draw
				if (nCharCount < strOut.GetLength() )
				{
					nCharCount += nCurrStrPos;
					break;
				}
			}

			// print link
			nCharCount = DrawLink(&nCurrDrawPos, pDrawArgs, pHl, strText, rcText, nFlag);
			rcText.left = nCurrDrawPos > rcText.right ? rcText.right : nCurrDrawPos;

			// update current position in string
			nCurrStrPos = pHl->m_nHyperTextBegin + pHl->m_nHyperTextLen;
			nHyperlink++;

			// if previous string was cut - stop draw
			if (nCharCount < (pHl->m_nHyperTextLen))
			{
				nCharCount += nCurrStrPos;
				break;
			}
		}
		else
		{

			// print whole string without links or tail of string
			strOut = strText.Mid(nCurrStrPos, strText.GetLength() - nCurrStrPos);
			nCharCount = DrawString(&nCurrDrawPos, pDC, strOut, rcText, nFlag);
			nCharCount += nCurrStrPos;
			break; // all was drawn!
		}
	}

	pDC->SetTextAlign(nAlingFlag);
	nCharCounter = nCharCount;
}

void CXTPReportPaintManager::DrawItemCaption(XTP_REPORTRECORDITEM_DRAWARGS* pDrawArgs, XTP_REPORTRECORDITEM_METRICS* pMetrics)
{
	CString strText = pMetrics->strText;

	CRect& rcItem = pDrawArgs->rcItem;

	rcItem.DeflateRect(2, 1, 2, 0);

	// draw item text
	if(!strText.IsEmpty())
	{
		int nCharCount = 0;
		int nHyperlink = 0;
		DrawTextLine(pDrawArgs, strText, rcItem, DT_END_ELLIPSIS, nCharCount, nHyperlink);
	}
}

