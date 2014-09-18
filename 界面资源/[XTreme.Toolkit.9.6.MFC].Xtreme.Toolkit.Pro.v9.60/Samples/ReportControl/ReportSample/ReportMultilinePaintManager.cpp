// ReportMultilinePaintManager.cpp: implementation of the CReportMultilinePaintManager class.
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
#include "ReportMultilinePaintManager.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CReportMultilinePaintManager::CReportMultilinePaintManager()
{
	m_bFixedRowHeight = FALSE;
}

CReportMultilinePaintManager::~CReportMultilinePaintManager()
{

}

int CReportMultilinePaintManager::GetRowHeight(CDC* pDC, CXTPReportRow* pRow)
{
	if (pRow->IsGroupRow() || !pRow->IsItemsVisible())
		return CXTPReportPaintManager::GetRowHeight(pDC, pRow);

	CXTPReportColumns* pColumns = pRow->GetControl()->GetColumns();
	int nColumnCount = pColumns->GetCount();

	XTP_REPORTRECORDITEM_DRAWARGS drawArgs;
	drawArgs.pControl = pRow->GetControl();
	drawArgs.pDC = pDC;
	drawArgs.pRow = pRow;

	int nHeight = 0;

	for (int nColumn = 0; nColumn < nColumnCount; nColumn++)
	{
		CXTPReportColumn* pColumn = pColumns->GetAt(nColumn);
		if (pColumn && pColumn->IsVisible())
		{
			CXTPReportRecordItem* pItem = pRow->GetRecord()->GetItem(pColumn);
			drawArgs.pItem = pItem;

			XTP_REPORTRECORDITEM_METRICS itemMetrics;
			pRow->GetItemMetrics(&drawArgs, &itemMetrics);

			CXTPFontDC fnt(pDC, itemMetrics.pFont);

			CRect rcItem(0, 0, pColumn->GetWidth() - 4, 0);
			pRow->ShiftTreeIndent(rcItem, pColumn);

			pItem->GetCaptionRect(&drawArgs, rcItem);
			pDC->DrawText(pItem->GetCaption(pColumn), rcItem, DT_WORDBREAK|DT_CALCRECT);

			nHeight = max(nHeight, rcItem.Height());
		}
	}


	return max(nHeight + 5, m_nRowHeight) + (IsGridVisible(FALSE)? 1: 0);
}

void CReportMultilinePaintManager::DrawItemCaption(XTP_REPORTRECORDITEM_DRAWARGS* pDrawArgs, XTP_REPORTRECORDITEM_METRICS* pMetrics)
{
	CRect& rcItem = pDrawArgs->rcItem;
	CDC* pDC = pDrawArgs->pDC;
	CString strText = pMetrics->strText;

	// draw item text
	if(!strText.IsEmpty())
	{
		rcItem.DeflateRect(2, 1, 2, 0);
		pDC->DrawText(strText, rcItem, pDrawArgs->nTextAlign|DT_WORDBREAK);
	}
}
