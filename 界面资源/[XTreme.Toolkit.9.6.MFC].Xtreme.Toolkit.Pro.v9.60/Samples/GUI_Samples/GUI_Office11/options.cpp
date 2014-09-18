// options.cpp : implementation file
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
#include "wordpad.h"
#include "strings.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDocOptions

void CDocOptions::SaveDockState(CDockState& ds, LPCTSTR lpszProfileName, LPCTSTR lpszLayout)
{
	CMemFile file;
	CArchive ar(&file, CArchive::store);
	ds.Serialize(ar);
	ar.Close();
	int nSize = (int)file.GetLength();
	ASSERT(nSize < 4096);
	BYTE* p = new BYTE[nSize];
	file.SeekToBegin();
	file.Read(p, nSize);
	theApp.WriteProfileBinary(lpszProfileName, lpszLayout, p, nSize);
	delete [] p;
}

void CDocOptions::SaveOptions(LPCTSTR lpszProfileName)
{
	SaveDockState(m_ds1, lpszProfileName, szLayout1);
	SaveDockState(m_ds2, lpszProfileName, szLayout2);
	theApp.WriteProfileInt(lpszProfileName, szWrap, m_nWordWrap);
}

void CDocOptions::LoadDockState(CDockState& ds, LPCTSTR lpszProfileName, LPCTSTR lpszLayout)
{
	BYTE* p;
	UINT nLen = 0;
	if (theApp.GetProfileBinary(lpszProfileName, lpszLayout, &p, &nLen))
	{
		ASSERT(nLen < 4096);
		CMemFile file;
		file.Write(p, nLen);
		file.SeekToBegin();
		CArchive ar(&file, CArchive::load);
		ds.Serialize(ar);
		ar.Close();
		delete p;
	}
}

void CDocOptions::LoadOptions(LPCTSTR lpszProfileName)
{
	LoadDockState(m_ds1, lpszProfileName, szLayout1);
	LoadDockState(m_ds2, lpszProfileName, szLayout2);
	m_nWordWrap = theApp.GetProfileInt(lpszProfileName, szWrap, m_nDefWrap);
}

/////////////////////////////////////////////////////////////////////////////
// CUnit

const CUnit& CUnit::operator=(const CUnit& unit)
{
	m_nTPU = unit.m_nTPU;
	m_nSmallDiv = unit.m_nSmallDiv;
	m_nMediumDiv = unit.m_nMediumDiv;
	m_nLargeDiv = unit.m_nLargeDiv;
	m_nMinMove = unit.m_nMinMove;
	m_nAbbrevID = unit.m_nAbbrevID;
	m_bSpaceAbbrev = unit.m_bSpaceAbbrev;
	m_strAbbrev = unit.m_strAbbrev;
	return *this;
}

CUnit::CUnit(int nTPU, int nSmallDiv, int nMediumDiv, int nLargeDiv,
		int nMinMove, UINT nAbbrevID, BOOL bSpaceAbbrev)
{
	m_nTPU = nTPU;
	m_nSmallDiv = nSmallDiv;
	m_nMediumDiv = nMediumDiv;
	m_nLargeDiv = nLargeDiv;
	m_nMinMove = nMinMove;
	m_nAbbrevID = nAbbrevID;
	m_bSpaceAbbrev = bSpaceAbbrev;
}
