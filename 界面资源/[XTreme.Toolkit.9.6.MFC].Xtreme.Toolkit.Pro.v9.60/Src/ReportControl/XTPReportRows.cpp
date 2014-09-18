// XTPReportRows.cpp : implementation of the CXTPReportRows class.
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

#include "XTPReportRow.h"
#include "XTPReportRecord.h"
#include "XTPReportRecordItem.h"
#include "XTPReportControl.h"
#include "XTPReportColumns.h"
#include "XTPReportColumn.h"
#include "XTPReportInplaceControls.h"
#include "XTPReportRows.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
// CXTPReportRows

CXTPReportRows::CXTPReportRows()
{
	m_pVirtualRow = 0;
}

CXTPReportRows::~CXTPReportRows()
{
	Clear();
}


//////////////////////////////////////////////////////////////////////////
// CXTPReportRows member functions

void CXTPReportRows::Clear(BOOL bResetRow)
{
	// array cleanup
	for (int nRow = (int)m_arrRows.GetSize() - 1; nRow >= 0; nRow--)
	{
		CXTPReportRow* pRow = m_arrRows.GetAt(nRow);
		if (bResetRow)
		{
			pRow->m_bVisible = FALSE;
			pRow->m_nIndex = -1;
		}
		pRow->InternalRelease();
	}

	m_arrRows.RemoveAll();

	if (m_pVirtualRow)
	{
		m_pVirtualRow->InternalRelease();
		m_pVirtualRow = 0;
	}

}

CXTPReportRow* CXTPReportRows::GetNext(CXTPReportRow* pRow, BOOL bSkipGroupFocus)
{
	if (pRow)
	{
		int index = pRow->GetIndex();

		while (index < GetCount() - 1)
		{
			index++;
			pRow = GetAt(index);

			if (!bSkipGroupFocus || !pRow->IsGroupRow() || !pRow->IsExpanded())
				return pRow;
		}
	}
	else if (GetCount() > 0)
	{
		return GetAt(0);
	}
	return pRow;
}

CXTPReportRow* CXTPReportRows::GetPrev(CXTPReportRow* pRow, BOOL bSkipGroupFocus)
{
	if (pRow)
	{
		int index = pRow->GetIndex();

		while (index > 0)
		{
			index--;
			pRow = GetAt(index);

			if (!bSkipGroupFocus || !pRow->IsGroupRow() || !pRow->IsExpanded())
				return pRow;

		}
	}
	else if (GetCount() > 0)
	{
		return GetAt(0);
	}
	return pRow;
}

void CXTPReportRows::InsertAt(int nIndex, CXTPReportRow* pRow)
{
	m_arrRows.InsertAt(nIndex, pRow);
}

int CXTPReportRows::Add(CXTPReportRow* pRow)
{
	int nIndex = (int)m_arrRows.Add(pRow);

	pRow->m_nChildIndex = nIndex;
	pRow->m_pParentRows = this;

	return nIndex;
}

void CXTPReportRows::RemoveAt(int nIndex)
{
	m_arrRows.GetAt(nIndex)->InternalRelease();
	m_arrRows.RemoveAt(nIndex);
}

void CXTPReportRows::SetVirtualMode(CXTPReportRow* pRow, int nCount)
{
	ASSERT(m_pVirtualRow == NULL);

	m_pVirtualRow = pRow;
	m_nVirtualRowsCount = nCount;
}


int _cdecl CXTPReportRows::CompareRows(const CXTPReportRow** ppRow1, const CXTPReportRow** ppRow2)
{
	CXTPReportColumns* pColumns = (**ppRow1).GetControl()->GetColumns();

	ASSERT(pColumns);

	for (int nGroupOrder = 0; nGroupOrder < pColumns->GetGroupsOrder()->GetCount(); nGroupOrder++)
	{
		CXTPReportColumn* pColumn = pColumns->GetGroupsOrder()->GetAt(nGroupOrder);
		BOOL bIncreasing = pColumn->m_bSortIncreasing;

		ASSERT((**ppRow1).GetRecord() && (**ppRow2).GetRecord());

		CXTPReportRecordItem* pItem1 = (**ppRow1).GetRecord()->GetItem(pColumn);
		CXTPReportRecordItem* pItem2 = (**ppRow2).GetRecord()->GetItem(pColumn);

		if (!pItem1 || !pItem2)
			continue;

		int nCompareResult = pItem1->CompareGroupCaption(pColumn, pItem2);

		if ( nCompareResult != 0)
			return nCompareResult * (bIncreasing? 1: -1);
	}


	for (int nSortOrder = 0; nSortOrder < pColumns->GetSortOrder()->GetCount(); nSortOrder++)
	{
		CXTPReportColumn* pColumn = pColumns->GetSortOrder()->GetAt(nSortOrder);
		BOOL bIncreasing = pColumn->IsSortedIncreasing();

		ASSERT((**ppRow1).GetRecord() && (**ppRow2).GetRecord());

		CXTPReportRecordItem* pItem1 = (**ppRow1).GetRecord()->GetItem(pColumn);
		CXTPReportRecordItem* pItem2 = (**ppRow2).GetRecord()->GetItem(pColumn);

		if (!pItem1 || !pItem2)
			continue;

		int nCompareResult = pItem1->Compare(pColumn, pItem2);

		if ( nCompareResult != 0)
			return nCompareResult * (bIncreasing? 1: -1);
	}

	return 0;
}

void CXTPReportRows::Sort()
{
	typedef int (_cdecl *GENERICCOMPAREFUNC)(const void *,const void*);

	qsort(m_arrRows.GetData(), (size_t)m_arrRows.GetSize(), sizeof(CXTPReportRow*), (GENERICCOMPAREFUNC)CompareRows);
}

int CXTPReportRows::GetCount()
{
	if (m_pVirtualRow)
		return m_nVirtualRowsCount;

	return (int)m_arrRows.GetSize();
}

CXTPReportRow* CXTPReportRows::GetAt(int nIndex)
{
	if (m_pVirtualRow)
	{
		m_pVirtualRow->m_nIndex = nIndex;
		return m_pVirtualRow;
	}

	return (nIndex >= 0) && (nIndex < GetCount())? m_arrRows.GetAt(nIndex): NULL;
}

//////////////////////////////////////////////////////////////////////////
// CXTPReportSelectedRows

CXTPReportSelectedRows::CXTPReportSelectedRows(CXTPReportControl* pControl)
	: m_pControl(pControl)
{


	m_nRowBlockBegin = -1;
	m_nPosSelected = 0;
	m_bChanged = FALSE;
}

void CXTPReportSelectedRows::Clear()
{
	if (m_arrSelectedBlocks.GetSize() > 0)
		m_bChanged = TRUE;
	
	m_arrSelectedBlocks.RemoveAll();
	m_nRowBlockBegin = -1;

	m_pControl->RedrawControl();
}

void CXTPReportSelectedRows::_InsertBlock(int nIndexInsert, int nIndexBegin, int nIndexEnd)
{
	SELECTED_BLOCK block;
	block.nIndexBegin = nIndexBegin;
	block.nIndexEnd = nIndexEnd;
	m_arrSelectedBlocks.InsertAt(nIndexInsert, block);
}


void CXTPReportSelectedRows::_OnCollapsed(int nIndex, int nCount)
{
	ASSERT(nCount > 0);

	for (int i = (int)m_arrSelectedBlocks.GetSize() - 1; i >= 0 ; i--)
	{
		int& nIndexBegin = m_arrSelectedBlocks[i].nIndexBegin;
		int& nIndexEnd = m_arrSelectedBlocks[i].nIndexEnd;

		if (nIndexBegin <= nIndex && nIndexEnd > nIndex + 1)
		{
			nIndexEnd = max(nIndexBegin + 1, nIndexEnd - nCount);

		}
		else if (nIndexBegin > nIndex + nCount)
		{
			nIndexBegin -= nCount;
			nIndexEnd -= nCount;
		}
		else if (nIndexEnd < nIndex)
			break;
		else if (nIndexBegin > nIndex)
		{
			nIndexBegin = nIndex;
			nIndexEnd = max(nIndexBegin + 1, nIndexEnd - nCount);
		}
	}
}

void CXTPReportSelectedRows::_OnExpanded(int nIndex, int nCount)
{
	ASSERT(nCount > 0);

	for (int i = (int)m_arrSelectedBlocks.GetSize() - 1; i >= 0 ; i--)
	{
		int& nIndexBegin = m_arrSelectedBlocks[i].nIndexBegin;
		int& nIndexEnd = m_arrSelectedBlocks[i].nIndexEnd;

		if (nIndexBegin <= nIndex && nIndexEnd > nIndex + 1)
		{
			_InsertBlock(i + 1, nIndex + 1 + nCount, nIndexEnd + nCount);
			m_arrSelectedBlocks[i].nIndexEnd = nIndex + 1;

		}
		else if (nIndexBegin > nIndex)
		{
			nIndexBegin += nCount;
			nIndexEnd += nCount;
		}
		else if (nIndexEnd < nIndex)
			break;

	}
}

void CXTPReportSelectedRows::AddBlock(int ib, int ie)
{
	int i = 0;
	int nCount = (int)m_arrSelectedBlocks.GetSize();

	if (nCount > 0)
	{
		int& nIndexEnd = m_arrSelectedBlocks[nCount - 1].nIndexEnd;

		if (nIndexEnd == ib)
		{
			nIndexEnd = ie + 1;
			m_pControl->RedrawControl();
			return;
		}

		if (nIndexEnd < ib)
		{
			i = nCount;
		}
	}

	for (; i < nCount; i++)
	{
		int& nIndexBegin = m_arrSelectedBlocks[i].nIndexBegin;
		int& nIndexEnd = m_arrSelectedBlocks[i].nIndexEnd;

		if ((nIndexBegin <= ib) && (nIndexEnd > ie))
		{
			return;
		}

		if (nIndexEnd == ib)
		{
			nIndexEnd = ie + 1;
			if (i + 1 < nCount && m_arrSelectedBlocks[i + 1].nIndexBegin == nIndexEnd)
			{
				nIndexEnd = m_arrSelectedBlocks[i + 1].nIndexEnd;
				m_arrSelectedBlocks.RemoveAt(i + 1);
			}

			m_pControl->RedrawControl();
			return;
		}
		if (nIndexBegin == ie + 1)
		{
			nIndexBegin = ib;
			m_pControl->RedrawControl();
			return;
		}

		if (nIndexBegin > ie)
			break;
	}

	_InsertBlock(i, ib, ie + 1);
	m_pControl->RedrawControl();
}

void CXTPReportSelectedRows::Add(CXTPReportRow* pRow)
{
	if (!pRow)
		return;

	int nIndex = pRow->GetIndex();

	if (nIndex != -1)
	{
		AddBlock(nIndex, nIndex);
	}
}

void CXTPReportSelectedRows::Remove(CXTPReportRow* pRow)
{
	if (!pRow)
		return;

	int nIndex = pRow->GetIndex();

	for (int i = 0; i < (int)m_arrSelectedBlocks.GetSize(); i++)
	{
		int nIndexBegin = m_arrSelectedBlocks[i].nIndexBegin;
		int nIndexEnd = m_arrSelectedBlocks[i].nIndexEnd;

		if ((nIndexBegin <= nIndex) && (nIndexEnd > nIndex))
		{
			if (nIndexBegin != nIndex)
			{
				_InsertBlock(i, nIndexBegin, nIndex);
				i++;
			}

			if (nIndexEnd - 1 != nIndex)
			{
				_InsertBlock(i + 1, nIndex + 1,nIndexEnd);
			}

			m_arrSelectedBlocks.RemoveAt(i);
			return;
		}
	}

}

void CXTPReportSelectedRows::Select(CXTPReportRow* pRow)
{
	if (!pRow)
		return;

	if (m_arrSelectedBlocks.GetSize() == 1 && m_arrSelectedBlocks[0].nIndexBegin == 
		m_arrSelectedBlocks[0].nIndexEnd - 1 && m_arrSelectedBlocks[0].nIndexBegin == pRow->GetIndex())
	{

	}
	else
	{	
		Clear();
		Add(pRow);
		m_bChanged = TRUE;
	}
}

void CXTPReportSelectedRows::SelectBlock(int nBlockBegin, int nEnd)
{
	nBlockBegin = m_nRowBlockBegin != -1? m_nRowBlockBegin: nBlockBegin;
	
	int nBegin = nBlockBegin;

	if (nBegin == -1 || nEnd == -1)
		return;

	if (nBegin > nEnd)
	{
		nBegin = nEnd;
		nEnd = nBlockBegin;
	}

	if (m_arrSelectedBlocks.GetSize() == 1 && m_arrSelectedBlocks[0].nIndexBegin == nBegin && 
		m_arrSelectedBlocks[0].nIndexEnd == nEnd + 1)
	{
		return;
	}

	Clear();
	if (m_nRowBlockBegin == -1) m_nRowBlockBegin = nBlockBegin;


	BOOL bSkipGroupFocus = m_pControl->IsSkipGroupsFocusEnabled();
	BOOL bHasGroups = m_pControl->GetColumns()->GetGroupsOrder()->GetCount() != 0;

	if (!bHasGroups || !bSkipGroupFocus)
	{
		_InsertBlock(0, nBegin, nEnd + 1);
	}
	else
	{
		for (int i = nBegin; i <= nEnd; i++)
		{
			CXTPReportRow* pRow = m_pControl->GetRows()->GetAt(i);
			if (!pRow)
				continue;

			if (!bSkipGroupFocus || !pRow->IsGroupRow() || !pRow->IsExpanded() || (i == nBegin) || (i == nEnd))
			{
				Add(pRow);
			}
		}
	}
}

BOOL CXTPReportSelectedRows::Contains(CXTPReportRow* pRow)
{
	if (!pRow)
		return FALSE;

	int nIndex = pRow->GetIndex();

	int nCount = (int)m_arrSelectedBlocks.GetSize();
	if (nCount == 0)
		return FALSE;

	if (m_arrSelectedBlocks[0].nIndexBegin > nIndex)
		return FALSE;

	if (m_arrSelectedBlocks[nCount - 1].nIndexEnd <= nIndex)
		return FALSE;

	for (int i = 0; i < nCount; i++)
	{
		if ((m_arrSelectedBlocks[i].nIndexBegin <= nIndex) &&
			(m_arrSelectedBlocks[i].nIndexEnd > nIndex))
		{
			return TRUE;
		}
	}
	return FALSE;
}

void CXTPReportSelectedRows::Invert(CXTPReportRow* pRow)
{
	if (Contains(pRow))
		Remove (pRow);
	else Add(pRow);

	m_nRowBlockBegin = -1;
	m_bChanged = TRUE;
}


int CXTPReportSelectedRows::GetCount()
{
	int nCount = 0;
	for (int i = 0; i < (int)m_arrSelectedBlocks.GetSize(); i++)
	{
		nCount += m_arrSelectedBlocks[i].nIndexEnd - m_arrSelectedBlocks[i].nIndexBegin;
	}
	return nCount;
}


POSITION CXTPReportSelectedRows::GetFirstSelectedRowPosition()
{
	if (m_arrSelectedBlocks.GetSize() == 0)
		return 0;

	m_nPosSelected = 0;

	POSITION pos = 0;
	pos = (POSITION)(DWORD_PTR)(m_arrSelectedBlocks[0].nIndexBegin + 1);
	return pos;
}

CXTPReportRow* CXTPReportSelectedRows::GetNextSelectedRow( POSITION& pos )
{
	ASSERT(m_nPosSelected < m_arrSelectedBlocks.GetSize());

	int nIndex = (int)(DWORD_PTR)pos - 1;
	CXTPReportRow* pRow = m_pControl->GetRows()->GetAt(nIndex);

	if (nIndex < m_arrSelectedBlocks[m_nPosSelected].nIndexEnd - 1)
	{
		(int)(DWORD_PTR)pos++;
	}
	else
	{
		if (m_nPosSelected >= m_arrSelectedBlocks.GetSize() - 1)
		{
			pos = NULL;
		}
		else
		{
			m_nPosSelected ++;
			pos = (POSITION)(DWORD_PTR)(m_arrSelectedBlocks[m_nPosSelected].nIndexBegin + 1);
		}
	}

	return pRow;
}



CXTPReportRow* CXTPReportSelectedRows::GetAt (int nIndex)
{
	for (int i = 0; i < (int)m_arrSelectedBlocks.GetSize(); i++)
	{
		int nCount = m_arrSelectedBlocks[i].nIndexEnd - m_arrSelectedBlocks[i].nIndexBegin;
		if (nCount <= nIndex)
		{
			nIndex -= nCount;
			continue;
		}

		CXTPReportRow* pRow = m_pControl->GetRows()->GetAt(m_arrSelectedBlocks[i].nIndexBegin + nIndex);
		return pRow;
	}

	return 0;
}



