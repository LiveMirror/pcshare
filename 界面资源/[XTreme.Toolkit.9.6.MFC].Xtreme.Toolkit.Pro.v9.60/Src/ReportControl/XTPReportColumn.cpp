// XTPReportColumn.cpp : implementation of the CXTPReportColumn class.
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
#include "XTPPropExchange.h"

#include "XTPReportColumn.h"
#include "XTPReportColumns.h"
#include "XTPReportSubListControl.h"
#include "XTPReportRecordItem.h"
#include "XTPReportControl.h"
#include "XTPReportInplaceControls.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



//////////////////////////////////////////////////////////////////////////
// CXTPReportColumn

CXTPReportColumn::CXTPReportColumn()
{
}

CXTPReportColumn::CXTPReportColumn(int nItemIndex, CString strName, int nWidth,
									BOOL bAutoSize, int nIconID, BOOL bSortable, BOOL bVisible) :
	m_nItemIndex(nItemIndex), m_strName(strName), m_nIconID(nIconID),
	m_nMinWidth(10), m_bSortable(bSortable), m_bVisible(bVisible)
{
	m_bGroupable = TRUE;
	m_bFiltrable = TRUE;
	m_bIsResizable = bAutoSize;
	m_rcColumn.SetRectEmpty();

	m_pColumns = NULL;
	m_bSortIncreasing = TRUE;

	m_bAllowRemove = TRUE;
	m_bAllowDrag = TRUE;

	m_nMaxItemWidth = 0;
	m_nAlignment = DT_LEFT;

	m_bAutoSize = bAutoSize;

	m_nColumnStaticWidth = m_nColumnAutoWidth = nWidth;

	m_bShowInFieldChooser = TRUE;
	m_bEditable = TRUE;

	m_pEditOptions = new CXTPReportRecordItemEditOptions();
}

CXTPReportColumn::~CXTPReportColumn(void)
{
	if (m_pEditOptions)
	{
		m_pEditOptions->InternalRelease();
	}
}

void CXTPReportColumn::SetCaption(LPCTSTR strCaption) 
{
	m_strName = strCaption;
	m_pColumns->GetReportHeader()->OnColumnsChanged();
}


int CXTPReportColumn::GetWidth()
{
	return m_nColumnStaticWidth + (!m_bIsResizable? GetIndent(): 0);
}

CRect CXTPReportColumn::GetRect()
{
	return m_rcColumn;
}


int CXTPReportColumn::SetWidth(int nNewWidth)
{
	int nOldWidth = m_nColumnStaticWidth;
	m_nColumnStaticWidth = m_nColumnAutoWidth = nNewWidth;

	m_pColumns->GetReportHeader()->OnColumnsChanged();
	return nOldWidth;
}

BOOL CXTPReportColumn::IsSortedIncreasing()
{
	return m_bSortIncreasing;
}
BOOL CXTPReportColumn::IsSortedDecreasing()
{
	return !m_bSortIncreasing;
}

BOOL CXTPReportColumn::IsSorted()
{
	return (m_pColumns->GetSortOrder()->IndexOf(this) != -1);
}

void CXTPReportColumn::SetTreeColumn(BOOL bIsTreeColumn)
{
	if (bIsTreeColumn)
	{
		m_pColumns->m_pTreeColumn = this;
	}
	else if (IsTreeColumn())
	{
		m_pColumns->m_pTreeColumn = NULL;
	}
}

BOOL CXTPReportColumn::IsTreeColumn()
{
	return m_pColumns->m_pTreeColumn == this;
}

BOOL CXTPReportColumn::IsVisible()
{
	return m_bVisible;
}


void CXTPReportColumn::DoPropExchange(CXTPPropExchange* pPX)
{
	PX_Bool(pPX, _T("SortIncreasing"), m_bSortIncreasing, TRUE);
	PX_Bool(pPX, _T("Visible"), m_bVisible, TRUE);
	PX_Int(pPX, _T("Alignment"), m_nAlignment, DT_LEFT);
	PX_Int(pPX, _T("StaticWidth"), m_nColumnStaticWidth, 0);
	PX_Int(pPX, _T("AutoWidth"), m_nColumnAutoWidth, 0);
}

BOOL CXTPReportColumn::HasSortTriangle()
{
	if (m_pColumns->GetSortOrder()->IndexOf(this) != -1)
		return TRUE;

	if (m_pColumns->GetGroupsOrder()->IndexOf(this) != -1)
		return TRUE;

	return FALSE;
}

int CXTPReportColumn::GetCaptionWidth(CDC* pDC)
{
	return pDC->GetTextExtent(m_strName).cx;
}

BOOL CXTPReportColumn::IsDragging()
{
	return m_pColumns && m_pColumns->GetReportHeader() && m_pColumns->GetReportHeader()->m_pDragColumn == this;
}

int CXTPReportColumn::GetIndent()
{
	if (!m_pColumns->GetReportHeader())
		return 0;

	if (m_pColumns->GetVisibleAt(0) == this)
		return GetControl()->GetHeaderIndent();

	return 0;
}

int CXTPReportColumn::GetMinWidth()
{
	return (m_bIsResizable || m_bAutoSize? m_nMinWidth : m_nColumnStaticWidth) + GetIndent();
}

void CXTPReportColumn::SetVisible(BOOL bVisible)
{
	if (bVisible != m_bVisible)
	{
		m_bVisible = bVisible;
		m_pColumns->GetReportHeader()->OnColumnsChanged();
	}
}

int CXTPReportColumn::GetItemIndex()
{
	return m_nItemIndex;
}

int CXTPReportColumn::GetIndex()
{
	return m_pColumns->IndexOf(this);
}

CXTPReportControl* CXTPReportColumn::GetControl()
{
	return m_pColumns->GetReportHeader()->GetControl();
}

void CXTPReportColumn::SetShowInFieldChooser(BOOL bShow)
{
	if (bShow != m_bShowInFieldChooser)
	{
		m_bShowInFieldChooser = bShow;

		CXTPReportSubListControl* pSubList = m_pColumns->GetReportHeader()->GetSubListCtrl();
		if (pSubList)
		{
			pSubList->UpdateList();
		}
	}
}

BOOL CXTPReportColumn::IsShowInFieldChooser()
{
	return m_bShowInFieldChooser;
}



