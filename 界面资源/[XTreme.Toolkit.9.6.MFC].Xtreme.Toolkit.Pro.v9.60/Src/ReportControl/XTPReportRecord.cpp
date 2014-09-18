// XTPReportRecord.cpp : implementation of the CXTPReportRecord class.
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

#include "XTPReportRecordItem.h"
#include "XTPReportRecordItemText.h"
#include "XTPReportColumn.h"
#include "XTPReportInplaceControls.h"
#include "XTPReportRecord.h"
#include "XTPReportRecords.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE(CXTPReportRecord, CCmdTarget)


CXTPReportRecord::CXTPReportRecord(void)
	: m_bVisible(TRUE), m_bLocked(FALSE), m_pPreviewItem(NULL)
{
	m_pChildren = NULL;

	m_bExpanded = FALSE;

	m_bEditable = TRUE;

	m_nIndex = 0;

}

CXTPReportRecord::~CXTPReportRecord(void)
{
	RemoveAll();

	if (m_pChildren)
	{
		m_pChildren->InternalRelease();
	}
}

void CXTPReportRecord::RemoveAll()
{
	for (int nItem = GetItemCount() - 1; nItem >= 0; nItem--)
	{
		CXTPReportRecordItem* pItem = m_arrItems.GetAt(nItem);
		pItem->InternalRelease();
	}
	m_arrItems.RemoveAll();

	if (m_pPreviewItem != NULL)
	{
		m_pPreviewItem->InternalRelease();
		m_pPreviewItem = NULL;
	}

}

BOOL CXTPReportRecord::HasChildren()
{
	return m_pChildren && (m_pChildren->GetCount() > 0);
}

CXTPReportRecords* CXTPReportRecord::GetChilds()
{
	if (m_pChildren == 0)
	{
		m_pChildren = new CXTPReportRecords;
	}

	return m_pChildren;
}

CXTPReportRecordItem* CXTPReportRecord::GetItem(CXTPReportColumn* pColumn)
{
	if (this == NULL)
		return NULL;

	int nItemIndex = pColumn->GetItemIndex();
	if (nItemIndex >= 0 && nItemIndex < GetItemCount())
	{
		return m_arrItems.GetAt(nItemIndex);
	}
	return NULL;
}

int CXTPReportRecord::IndexOf(CXTPReportRecordItem* pItem)
{
	for (int nItem = 0; nItem < GetItemCount(); nItem++)
	{
		if (GetItem(nItem) == pItem)
			return nItem;
	}
	return -1;
}

CXTPReportRecordItem* CXTPReportRecord::AddItem(CXTPReportRecordItem* pItem)
{
	m_arrItems.Add(pItem);
	pItem->m_pRecord = this;
	return pItem;
}

CXTPReportRecordItemPreview* CXTPReportRecord::GetItemPreview()
{

	return m_pPreviewItem;
}

void CXTPReportRecord::SetEditable(BOOL bEditable)
{
	m_bEditable = bEditable;
}

void CXTPReportRecord::SetPreviewItem(CXTPReportRecordItemPreview* pItemPreview)
{
	if (m_pPreviewItem != NULL)
		m_pPreviewItem->InternalRelease();
	m_pPreviewItem = pItemPreview;

	m_pPreviewItem->m_pRecord = this;
}

BOOL CXTPReportRecord::IsFiltered()
{
	return FALSE;
}

int CXTPReportRecord::GetIndex()
{
	return m_nIndex;
}

