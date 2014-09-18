// XTPPropertyGridItemColor.cpp : implementation of the CXTPPropertyGridItemColor class.
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
#include "XTPDrawHelpers.h"

#include "XTPPropertyGridInplaceEdit.h"
#include "XTPPropertyGridInplaceButton.h"
#include "XTPPropertyGridInplaceList.h"
#include "XTPPropertyGridItem.h"
#include "XTPPropertyGridItemColor.h"
#include "XTPPropertyGrid.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CXTPPropertyGridItemColor

IMPLEMENT_DYNAMIC(CXTPPropertyGridItemColor, CXTPPropertyGridItem)

CXTPPropertyGridItemColor::CXTPPropertyGridItemColor(CString strCaption, COLORREF clr, COLORREF* pBindColor)
	: CXTPPropertyGridItem(strCaption)
{
	m_pBindColor = pBindColor;
	SetColor(clr);
	m_nFlags = xtpGridItemHasEdit | xtpGridItemHasExpandButton;
}

CXTPPropertyGridItemColor::CXTPPropertyGridItemColor(UINT nID, COLORREF clr, COLORREF* pBindColor)
	: CXTPPropertyGridItem(nID)
{
	m_pBindColor = pBindColor;
	SetColor(clr);
	m_nFlags = xtpGridItemHasEdit | xtpGridItemHasExpandButton;
}

CXTPPropertyGridItemColor::~CXTPPropertyGridItemColor()
{
}

/////////////////////////////////////////////////////////////////////////////
//

BOOL CXTPPropertyGridItemColor::OnDrawItemValue(CDC& dc, CRect rcValue)
{
	COLORREF clr = dc.GetTextColor();
	CRect rcSample(rcValue.left - 2, rcValue.top + 1, rcValue.left + 18, rcValue.bottom - 1);
	CXTPPenDC pen(dc, clr);
	CXTPBrushDC brush(dc, m_clrValue);
	dc.Rectangle(rcSample);

	CRect rcText(rcValue);
	rcText.left += 25;

	dc.DrawText( m_strValue, rcText,  DT_SINGLELINE|DT_VCENTER);

	return TRUE;
}

CRect CXTPPropertyGridItemColor::GetValueRect()
{
	CRect rcValue(CXTPPropertyGridItem::GetValueRect());
	rcValue.left += 25;
	return rcValue;
}

COLORREF AFX_CDECL CXTPPropertyGridItemColor::StringToRGB(CString str)
{
	CString strRed, strGreen, strBlue;

	AfxExtractSubString(strRed, str, 0, ';');
	AfxExtractSubString(strGreen, str, 1, ';');
	AfxExtractSubString(strBlue, str, 2, ';');

	return RGB(__min(_ttoi(strRed), 255), __min(_ttoi(strGreen), 255), __min(_ttoi(strBlue), 255));
}

CString AFX_CDECL CXTPPropertyGridItemColor::RGBToString(COLORREF clr)
{
	CString str;
	str.Format(_T("%i; %i; %i"), GetRValue(clr), GetGValue(clr), GetBValue(clr));
	return str;
}

void CXTPPropertyGridItemColor::SetValue(CString strValue)
{
	SetColor(StringToRGB(strValue));
}

void CXTPPropertyGridItemColor::SetColor(COLORREF clr)
{
	m_clrValue = clr;

	if (m_pBindColor)
	{
		*m_pBindColor = clr;
	}

	CXTPPropertyGridItem::SetValue(RGBToString(clr));
}

void CXTPPropertyGridItemColor::BindToColor(COLORREF* pBindColor)
{
	m_pBindColor = pBindColor;
	if (m_pBindColor)
	{
		*m_pBindColor = m_clrValue;
	}
}


void CXTPPropertyGridItemColor::OnBeforeInsert()
{
	if( m_pBindColor && *m_pBindColor != m_clrValue)
	{
		SetColor(*m_pBindColor);
	}
}


void CXTPPropertyGridItemColor::OnInplaceButtonDown()
{
#ifdef _INCLUDE_CONTROLS
	CColorDialog dlg( m_clrValue, m_clrValue,
		CPS_XTP_SHOW3DSELECTION|CPS_XTP_SHOWHEXVALUE, m_pGrid );
#else
	CColorDialog dlg( m_clrValue, 0, m_pGrid );
#endif

	if ( dlg.DoModal( ) == IDOK )
	{
		OnValueChanged( RGBToString( dlg.GetColor( ) ) );
		m_pGrid->Invalidate( FALSE );
	}
}



