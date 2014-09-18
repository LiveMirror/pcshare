// ControlSelector.cpp: implementation of the CXTPControlSelector class.
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
#include "ControlSelector.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_XTP_CONTROL(CControlTable, CXTPControlSelector)

CControlTable::CControlTable()
{
	m_szItemsCount = CSize(5, 4);
	m_szItemsMax = CSize(12, 12);
	m_szItem = CSize(28, 28);
}

//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

IMPLEMENT_XTP_CONTROL(CControlColumns, CXTPControlSelector);

CControlColumns::CControlColumns()
{

	m_szItemsCount = CSize(4, 1);
	m_szItemsMax = CSize(8, 1);
	m_szItem = CSize(28, 52);
}



void CControlColumns::OnDrawItem(CDC* pDC, CRect rcItem, COLORREF clrText)
{
	rcItem.DeflateRect(5, 8, 5, 8);

	for (int y = rcItem.top, i = 0; y < rcItem.bottom; y += 3, i++)
	{
		pDC->FillSolidRect(rcItem.left, y, (i + 1) % 5 == 0? rcItem.Width() / 2: rcItem.Width(), 1, clrText);
	}

}
