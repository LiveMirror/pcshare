// XTPReportGroupRow.cpp : implementation of the CXTPReportGroupRow class.
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

#include "XTPReportControl.h"
#include "XTPReportPaintManager.h"
#include "XTPReportRecordItemText.h"
#include "XTPReportInplaceControls.h"
#include "XTPReportGroupRow.h"
#include "XTPReportControlResource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//////////////////////////////////////////////////////////////////////////
// CXTPReportGroupItem


CXTPReportGroupRow::CXTPReportGroupRow()
{
	m_bExpanded = TRUE;
}

void CXTPReportGroupRow::SetCaption(LPCTSTR lpszCaption)
{
	m_strGroupText = lpszCaption;
}


CString CXTPReportGroupRow::GetCaption()
{
	return m_strGroupText;
}

void CXTPReportGroupRow::Draw(CDC* pDC, CRect rcRow)
{
	CXTPReportPaintManager* pPaintManager = m_pControl->GetPaintManager();
	pDC->SetBkMode(TRANSPARENT);

	m_rcRow = rcRow;

	pPaintManager->DrawGroupRow(pDC, this, rcRow);
}

void CXTPReportGroupRow::OnClick(CPoint ptClicked)
{
	// expand/collapse on single click at the collapse bitmap
	if (m_rcCollapse.PtInRect(ptClicked))
	{
		SetExpanded(!IsExpanded());
	}
}

void CXTPReportGroupRow::OnDblClick(CPoint ptClicked)
{
	// do not expand if double clicked on the collapse bitmap
	if (!m_rcCollapse.PtInRect(ptClicked))
	{
		// otherwise expand on double click
		SetExpanded(!IsExpanded());
	}
}
