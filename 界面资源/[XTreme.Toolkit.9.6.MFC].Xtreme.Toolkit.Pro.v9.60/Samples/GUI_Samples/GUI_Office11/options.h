// options.h : header file
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

class CUnit
{
public:
	int m_nTPU;
	int m_nSmallDiv;    // small divisions - small line displayed
	int m_nMediumDiv;   // medium divisions - large line displayed
	int m_nLargeDiv;    // large divisions - numbers displayed
	int m_nMinMove;     // minimum tracking movements
	UINT m_nAbbrevID;
	BOOL m_bSpaceAbbrev; // put space before abbreviation
	CString m_strAbbrev;// cm, pt, pi, ", in, inch, inches

	CUnit() {}
	CUnit(int nTPU, int nSmallDiv, int nMediumDiv, int nLargeDiv,
		int nMinMove, UINT nAbbrevID, BOOL bSpaceAbbrev);
	const CUnit& operator=(const CUnit& unit);
};

class CDocOptions
{
public:
	CDocOptions(int nDefWrap) {m_nDefWrap = nDefWrap;}
	CDockState m_ds1;
	CDockState m_ds2;
	int m_nWordWrap;
	int m_nDefWrap;
	void SaveOptions(LPCTSTR lpsz);
	void LoadOptions(LPCTSTR lpsz);
	void SaveDockState(CDockState& ds, LPCTSTR lpszProfileName,
		LPCTSTR lpszLayout);
	void LoadDockState(CDockState& ds, LPCTSTR lpszProfileName,
		LPCTSTR lpszLayout);
	CDockState& GetDockState(BOOL bPrimary) {return (bPrimary) ? m_ds1 : m_ds2;}
};

/////////////////////////////////////////////////////////////////////////////
