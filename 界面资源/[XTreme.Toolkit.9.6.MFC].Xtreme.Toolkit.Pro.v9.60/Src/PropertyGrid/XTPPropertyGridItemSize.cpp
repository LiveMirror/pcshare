// XTPPropertyGridItemSize.cpp : implementation of the CXTPPropertyGridItemSize class.
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
#include "XTPPropertyGridItemSize.h"
#include "XTPPropertyGridItemNumber.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CXTPPropertyGridItemSize::CXTPPropertyGridItemSizeWidth

class CXTPPropertyGridItemSize::CXTPPropertyGridItemSizeWidth : public CXTPPropertyGridItemNumber
{
public:
	CXTPPropertyGridItemSizeWidth(CString strCaption)
		: CXTPPropertyGridItemNumber(strCaption) {}

	virtual void OnValueChanged(CString strValue)
	{
		((CXTPPropertyGridItemSize*)m_pParent)->SetWidth(strValue);
	}
};

/////////////////////////////////////////////////////////////////////////////
// CXTPPropertyGridItemSize::CXTPPropertyGridItemSizeHeight

class CXTPPropertyGridItemSize::CXTPPropertyGridItemSizeHeight : public CXTPPropertyGridItemNumber
{
public:
	CXTPPropertyGridItemSizeHeight(CString strCaption)
		: CXTPPropertyGridItemNumber(strCaption) {}

	virtual void OnValueChanged(CString strValue)
	{
		((CXTPPropertyGridItemSize*)m_pParent)->SetHeight(strValue);
	}
};

/////////////////////////////////////////////////////////////////////////////
// CXTPPropertyGridItemSize
IMPLEMENT_DYNAMIC(CXTPPropertyGridItemSize, CXTPPropertyGridItem)

CXTPPropertyGridItemSize::CXTPPropertyGridItemSize(CString strCaption, CSize size, CSize* pBindSize)
	: CXTPPropertyGridItem(strCaption)
{
	m_szValue = size;
	BindToSize(pBindSize);
	m_strValue = SizeToString(size);
}

CXTPPropertyGridItemSize::CXTPPropertyGridItemSize(UINT nID,  CSize size, CSize* pBindSize)
	: CXTPPropertyGridItem(nID)
{
	m_szValue = size;
	BindToSize(pBindSize);
	m_strValue = SizeToString(size);

}

CXTPPropertyGridItemSize::~CXTPPropertyGridItemSize()
{

}

/////////////////////////////////////////////////////////////////////////////
//

void CXTPPropertyGridItemSize::OnAddChildItem()
{
	m_itemWidth = (CXTPPropertyGridItemSizeWidth*)AddChildItem(new CXTPPropertyGridItemSizeWidth(_T("Width")));
	m_itemHeight = (CXTPPropertyGridItemSizeHeight*)AddChildItem(new CXTPPropertyGridItemSizeHeight(_T("Height")));
	UpdateChilds();
}

CString CXTPPropertyGridItemSize::SizeToString(CSize size)
{
	CString str;
	str.Format(_T("%i; %i"), size.cx, size.cy);
	return str;
}

CSize CXTPPropertyGridItemSize::StringToSize(CString str)
{
	CString strWidth, strHeight;

	AfxExtractSubString(strWidth, str, 0, ';');
	AfxExtractSubString(strHeight, str, 1, ';');

	return CSize(_ttoi(strWidth), _ttoi(strHeight));
}

void CXTPPropertyGridItemSize::SetValue(CString strValue)
{
	SetSize(StringToSize(strValue));
}

void CXTPPropertyGridItemSize::SetSize(CSize size)
{
	m_szValue = size;

	if (m_pBindSize)
	{
		*m_pBindSize = m_szValue;
	}

	CXTPPropertyGridItem::SetValue(SizeToString(m_szValue));
	UpdateChilds();
}

void CXTPPropertyGridItemSize::BindToSize(CSize* pBindSize)
{
	m_pBindSize = pBindSize;
	if (m_pBindSize)
	{
		*m_pBindSize = m_szValue;
	}
}

void CXTPPropertyGridItemSize::OnBeforeInsert()
{
	if( m_pBindSize && *m_pBindSize != m_szValue)
	{
		SetSize(*m_pBindSize);
	}
}

void CXTPPropertyGridItemSize::UpdateChilds()
{
	m_itemWidth->SetNumber(m_szValue.cx);
	m_itemHeight->SetNumber(m_szValue.cy);
}

void CXTPPropertyGridItemSize::SetWidth(CString strWidth)
{
	OnValueChanged(SizeToString(CSize(_ttoi(strWidth), m_szValue.cy)));

}

void CXTPPropertyGridItemSize::SetHeight(CString strHeight)
{
	OnValueChanged(SizeToString(CSize(m_szValue.cx, _ttoi(strHeight))));
}
