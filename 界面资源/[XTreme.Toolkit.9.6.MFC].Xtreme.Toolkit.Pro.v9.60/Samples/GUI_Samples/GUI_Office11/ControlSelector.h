// ControlSelector.h: interface for the CXTPControlSelector class.
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

#if !defined(AFX_CONTROLSELECTOR_H__1790E8B0_8D50_4786_8C69_1A106AA634A5__INCLUDED_)
#define AFX_CONTROLSELECTOR_H__1790E8B0_8D50_4786_8C69_1A106AA634A5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


//////////////////////////////////////////////////////////////////////////

class CControlColumns : public CXTPControlSelector
{
	DECLARE_XTP_CONTROL(CControlColumns);
public:
	CControlColumns();

	void OnDrawItem(CDC* pDC, CRect rcItem, COLORREF clrText);
};

//////////////////////////////////////////////////////////////////////////

class CControlTable : public CXTPControlSelector
{
	DECLARE_XTP_CONTROL(CControlTable);

public:
	CControlTable();

};

#endif // !defined(AFX_CONTROLSELECTOR_H__1790E8B0_8D50_4786_8C69_1A106AA634A5__INCLUDED_)
