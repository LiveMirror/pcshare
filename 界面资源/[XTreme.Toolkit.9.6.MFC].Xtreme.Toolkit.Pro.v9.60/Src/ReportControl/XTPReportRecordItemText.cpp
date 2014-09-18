// XTPReportRecordItemText.cpp : implementation of the CXTPReportRecordItemText class.
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
#include <math.h>
#include "XTPDrawHelpers.h"

#include "XTPReportRecordItem.h"
#include "XTPReportControl.h"
#include "XTPReportPaintManager.h"
#include "XTPReportRecordItemText.h"
#include "XTPReportInplaceControls.h"
#include "XTPReportRow.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
// CXTPReportRecordItemText

IMPLEMENT_DYNAMIC(CXTPReportRecordItemText, CXTPReportRecordItem)

CXTPReportRecordItemText::CXTPReportRecordItemText(LPCTSTR szText)
	: CXTPReportRecordItem(), m_strText(szText)
{
}


CString CXTPReportRecordItemText::GetCaption(CXTPReportColumn* /*pColumn*/)
{
	if (!m_strCaption.IsEmpty())
		return m_strCaption;

	if (m_strFormatString == _T("%s"))
		return m_strText;

	CString strCaption;
	strCaption.Format(m_strFormatString, (LPCTSTR)m_strText);
	return strCaption;
}

void CXTPReportRecordItemText::OnEditChanged(XTP_REPORTRECORDITEM_ARGS* /*pItemArgs*/, LPCTSTR szText)
{
	SetValue(szText);
}

//////////////////////////////////////////////////////////////////////////
// CXTPReportRecordItemNumber

IMPLEMENT_DYNAMIC(CXTPReportRecordItemNumber, CXTPReportRecordItem)

CXTPReportRecordItemNumber::CXTPReportRecordItemNumber(double dValue)
	: CXTPReportRecordItem(), m_dValue(dValue)
{
	m_strFormatString = _T("%.0f");
}

CXTPReportRecordItemNumber::CXTPReportRecordItemNumber(double dValue, CString strFormat)
	: CXTPReportRecordItem(), m_dValue(dValue)
{
	m_strFormatString = strFormat;
}

CString CXTPReportRecordItemNumber::GetCaption(CXTPReportColumn* /*pColumn*/)
{
	if (!m_strCaption.IsEmpty())
		return m_strCaption;

	CString strCaption;
	strCaption.Format(m_strFormatString, m_dValue);
	return strCaption;
}

int CXTPReportRecordItemNumber::Compare(CXTPReportColumn*, CXTPReportRecordItem* pItem)
{
	CXTPReportRecordItemNumber* pItemNumber = DYNAMIC_DOWNCAST(CXTPReportRecordItemNumber, pItem);
	if (!pItemNumber)
		return 0;

	if (m_dValue == pItemNumber->m_dValue)
		return 0;
	else if (m_dValue > pItemNumber->m_dValue)
		return 1;
	else
		return -1;
}

void CXTPReportRecordItemNumber::OnEditChanged(XTP_REPORTRECORDITEM_ARGS* /*pItemArgs*/, LPCTSTR szText)
{
#ifdef _UNICODE
	char astring[20];
	WideCharToMultiByte (CP_ACP, 0, szText, -1, astring, 20, NULL, NULL);
	SetValue((double)atof(astring));
#else
	SetValue((double)atof(szText));
#endif
}


//////////////////////////////////////////////////////////////////////////
// CXTPReportRecordItemDateTime

IMPLEMENT_DYNAMIC(CXTPReportRecordItemDateTime, CXTPReportRecordItem)

CXTPReportRecordItemDateTime::CXTPReportRecordItemDateTime(COleDateTime odtValue)
	: CXTPReportRecordItem(), m_odtValue(odtValue)
{
	m_strFormatString = _T("%a %b/%d/%Y %I:%M %p");
}

CString CXTPReportRecordItemDateTime::GetCaption(CXTPReportColumn* /*pColumn*/)
{
	if (!m_strCaption.IsEmpty())
		return m_strCaption;

	return m_odtValue.Format(m_strFormatString);
}

int CXTPReportRecordItemDateTime::Compare(CXTPReportColumn*, CXTPReportRecordItem* pItem)
{
	CXTPReportRecordItemDateTime* pItemDateTime = DYNAMIC_DOWNCAST(CXTPReportRecordItemDateTime, pItem);
	if (!pItemDateTime)
		return 0;

	if (m_odtValue == pItemDateTime->m_odtValue)
		return 0;

	if (m_odtValue.GetStatus() != COleDateTime::valid || pItemDateTime->m_odtValue.GetStatus() != COleDateTime::valid)
		return int((DATE)m_odtValue - (DATE)pItemDateTime->m_odtValue);

	if (m_odtValue > pItemDateTime->m_odtValue)
		return 1;

	return -1;
}

void CXTPReportRecordItemDateTime::OnEditChanged(XTP_REPORTRECORDITEM_ARGS* /*pItemArgs*/, LPCTSTR szText)
{
	m_odtValue.ParseDateTime(szText);
}


//////////////////////////////////////////////////////////////////////////
// CXTPReportRecordItemPreview

CXTPReportRecordItemPreview::CXTPReportRecordItemPreview(LPCTSTR szPreviewText)
	: CXTPReportRecordItem(), m_strPreviewText(szPreviewText)
{
}

CString CXTPReportRecordItemPreview::GetCaption(CXTPReportColumn* /*pColumn*/)
{
	return GetPreviewText();
}

void CXTPReportRecordItemPreview::GetItemMetrics(XTP_REPORTRECORDITEM_DRAWARGS* pDrawArgs, XTP_REPORTRECORDITEM_METRICS* pItemMetrics)
{
	pItemMetrics->clrForeground = pDrawArgs->pControl->GetPaintManager()->m_clrPreviewText;
	pItemMetrics->pFont = &pDrawArgs->pControl->GetPaintManager()->m_fontPreview;
}

int CXTPReportRecordItemPreview::GetPreviewHeight(CDC* pDC, CXTPReportRow* pRow, int nWidth)
{
	XTP_REPORTRECORDITEM_METRICS metrics;
	metrics.strText = GetPreviewText();
	pRow->FillMetrics(NULL, this, &metrics);

	CString strPreviewText = metrics.strText;

	if (strPreviewText.IsEmpty())
		return 0;

	CXTPReportControl* pControl = pRow->GetControl();

	int nIndentWidth = pControl->GetHeaderIndent();


	CRect& rcIndent = pControl->GetPaintManager()->m_rcPreviewIndent;

	CRect rcPreviewItem(nIndentWidth + rcIndent.left, 0, nWidth - rcIndent.right, 0);

	CXTPFontDC font(pDC, metrics.pFont);

	int nMaxPreviewLines = pControl->GetPaintManager()->GetPreviewLinesCount(pDC, rcPreviewItem, strPreviewText);
	int nFontHeight = pDC->GetTextExtent(_T(" ")).cy;

	return nFontHeight * nMaxPreviewLines + rcIndent.top + rcIndent.bottom;
}

void CXTPReportRecordItemPreview::OnDrawCaption(XTP_REPORTRECORDITEM_DRAWARGS* pDrawArgs, XTP_REPORTRECORDITEM_METRICS* pMetrics)
{
	CString strText = pMetrics->strText;

	// draw item text
	if(!strText.IsEmpty())
	{
		CRect rcItem(pDrawArgs->rcItem);

		CRect& rcIndent = pDrawArgs->pControl->GetPaintManager()->m_rcPreviewIndent;

		rcItem.DeflateRect(rcIndent.left, rcIndent.top, rcIndent.right, rcIndent.bottom);

		pDrawArgs->pDC->DrawText(strText, rcItem, DT_WORDBREAK|DT_LEFT|DT_NOPREFIX);
	}
}



