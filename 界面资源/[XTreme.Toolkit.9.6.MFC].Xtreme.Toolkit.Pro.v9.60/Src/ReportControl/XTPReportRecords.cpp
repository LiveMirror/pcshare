// XTPReportRecords.cpp : implementation of the CXTPReportRecords class.
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
#include "XTPReportRecord.h"
#include "XTPReportRecords.h"
#include "XTPReportControlResource.h"
#include "XTPReportInplaceControls.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CXTPReportRecords::CXTPReportRecords()
{
	m_pVirtualRecord = NULL;
	m_nVirtualRecordsCount = 0;


}

void CXTPReportRecords::SetVirtualMode(CXTPReportRecord* pVirtualRecord, int nCount)
{
	if (m_pVirtualRecord)
	{
		m_pVirtualRecord->InternalRelease();
	}
	m_pVirtualRecord = pVirtualRecord;
	m_nVirtualRecordsCount = nCount;
}

CXTPReportRecords::~CXTPReportRecords(void)
{
	RemoveAll();

	if (m_pVirtualRecord)
	{
		m_pVirtualRecord->InternalRelease();
	}
}

void CXTPReportRecords::RemoveAll()
{
	// array cleanup
	for (int nRecord = (int)m_arrRecords.GetSize() - 1; nRecord >= 0; nRecord--)
	{
		CXTPReportRecord* pRecord = m_arrRecords.GetAt(nRecord);
		pRecord->InternalRelease();
	}
	m_arrRecords.RemoveAll();
}

void CXTPReportRecords::UpdateIndexes(int nStart /*= 0*/)
{
	for (int i = nStart; i < GetCount(); i++)
		GetAt(i)->m_nIndex = i;
}
CXTPReportRecord* CXTPReportRecords::Add(CXTPReportRecord* pRecord)
{
	int nIndex = (int)m_arrRecords.Add(pRecord);
	pRecord->m_nIndex = nIndex;

	return pRecord;
}

void CXTPReportRecords::RemoveAt(int nIndex)
{
	if (nIndex < (int)m_arrRecords.GetSize())
	{
		m_arrRecords[nIndex]->InternalRelease();
		m_arrRecords.RemoveAt(nIndex);

		UpdateIndexes(nIndex);
	}
}

void CXTPReportRecords::InsertAt(int nIndex, CXTPReportRecord* pRecord)
{
	m_arrRecords.InsertAt(nIndex, pRecord);
	UpdateIndexes(nIndex);
}

int CXTPReportRecords::GetCount()
{
	if (m_pVirtualRecord != NULL)
		return m_nVirtualRecordsCount;

	return (int)m_arrRecords.GetSize();
}
CXTPReportRecord* CXTPReportRecords::GetAt(int nIndex)
{
	if (m_pVirtualRecord)
	{
		m_pVirtualRecord->m_nIndex = nIndex;
		return m_pVirtualRecord;
	}

	return m_arrRecords.GetAt(nIndex);
}


