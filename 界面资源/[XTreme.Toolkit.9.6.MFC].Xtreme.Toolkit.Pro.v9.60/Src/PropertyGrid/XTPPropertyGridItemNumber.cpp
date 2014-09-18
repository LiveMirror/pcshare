// XTPPropertyGridItemNumber.cpp : implementation of the CXTPPropertyGridItemNumber class.
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

#include "StdAfx.h"
#include "XTPVC80Helpers.h"

#include "XTPPropertyGridInplaceEdit.h"
#include "XTPPropertyGridInplaceButton.h"
#include "XTPPropertyGridInplaceList.h"
#include "XTPPropertyGridItem.h"
#include "XTPPropertyGridItemNumber.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CXTPPropertyGridItemNumber
IMPLEMENT_DYNAMIC(CXTPPropertyGridItemNumber, CXTPPropertyGridItem)


CXTPPropertyGridItemNumber::CXTPPropertyGridItemNumber(CString strCaption, long nValue, long* pBindNumber)
	: CXTPPropertyGridItem(strCaption)
{
	m_pBindNumber = pBindNumber;
	SetNumber(nValue);
}
CXTPPropertyGridItemNumber::CXTPPropertyGridItemNumber(UINT nID, long nValue, long* pBindNumber)
	: CXTPPropertyGridItem(nID)
{
	m_pBindNumber = pBindNumber;
	SetNumber(nValue);
}

CXTPPropertyGridItemNumber::~CXTPPropertyGridItemNumber()
{
}

/////////////////////////////////////////////////////////////////////////////
//

void CXTPPropertyGridItemNumber::SetValue(CString strValue)
{
	SetNumber(_ttol(strValue));
}

void CXTPPropertyGridItemNumber::SetNumber(long nValue)
{
	m_nValue = nValue;

	if (m_pBindNumber)
	{
		*m_pBindNumber = m_nValue;
	}

	CString strValue;
	strValue.Format(_T("%i"), nValue);
	CXTPPropertyGridItem::SetValue(strValue);
}

void CXTPPropertyGridItemNumber::BindToNumber(long* pBindNumber)
{
	m_pBindNumber = pBindNumber;
	if (m_pBindNumber)
	{
		*m_pBindNumber = m_nValue;
	}
}

void CXTPPropertyGridItemNumber::OnBeforeInsert()
{
	if( m_pBindNumber && *m_pBindNumber != m_nValue)
	{
		SetNumber(*m_pBindNumber);
	}
}





/////////////////////////////////////////////////////////////////////////////
// CXTPPropertyGridItemDouble
IMPLEMENT_DYNAMIC(CXTPPropertyGridItemDouble, CXTPPropertyGridItem)


CXTPPropertyGridItemDouble::CXTPPropertyGridItemDouble(CString strCaption, double fValue, LPCTSTR strFormat, double* pBindDouble)
	: CXTPPropertyGridItem(strCaption)
{
	m_pBindDouble = pBindDouble;
	m_strFormat = strFormat;
	SetDouble(fValue);
}
CXTPPropertyGridItemDouble::CXTPPropertyGridItemDouble(UINT nID, double fValue, LPCTSTR strFormat, double* pBindDouble)
	: CXTPPropertyGridItem(nID)
{
	m_pBindDouble = pBindDouble;
	m_strFormat = strFormat;
	SetDouble(fValue);
}

CXTPPropertyGridItemDouble::~CXTPPropertyGridItemDouble()
{
}

/////////////////////////////////////////////////////////////////////////////
//

void CXTPPropertyGridItemDouble::SetValue(CString strValue)
{
#ifdef _UNICODE
	char astring[20];
	WideCharToMultiByte (CP_ACP, 0, strValue, -1, astring, 20, NULL, NULL);
	SetDouble((double)atof(astring));
#else
	SetDouble((double)atof(strValue));
#endif
}

void CXTPPropertyGridItemDouble::SetDouble(double fValue)
{
	m_fValue = fValue;

	if (m_pBindDouble)
	{
		*m_pBindDouble = m_fValue;
	}

	CString strValue;
	strValue.Format(m_strFormat, fValue);
	CXTPPropertyGridItem::SetValue(strValue);
}

void CXTPPropertyGridItemDouble::BindToDouble(double* pBindNumber)
{
	m_pBindDouble = pBindNumber;
	if (m_pBindDouble)
	{
		*m_pBindDouble = m_fValue;
	}
}

void CXTPPropertyGridItemDouble::OnBeforeInsert()
{
	if( m_pBindDouble && *m_pBindDouble != m_fValue)
	{
		SetDouble(*m_pBindDouble);
	}
}

