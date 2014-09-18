// CXTPPropertyGridItemBool.cpp : implementation of the XTPPropertyGridItemBool class.
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
#include "XTPPropertyGriditembool.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CXTPPropertyGridItemBool

IMPLEMENT_DYNAMIC(CXTPPropertyGridItemBool, CXTPPropertyGridItem)

CXTPPropertyGridItemBool::CXTPPropertyGridItemBool(CString strCaption, BOOL bValue, BOOL* pBindBool)
: CXTPPropertyGridItem(strCaption)
, m_strTrueText(_T("True"))
, m_strFalseText(_T("False"))
{
	m_pBindBool = pBindBool;
	_Init(bValue);

}

CXTPPropertyGridItemBool::CXTPPropertyGridItemBool(UINT nID, BOOL bValue, BOOL* pBindBool)
: CXTPPropertyGridItem(nID)
, m_strTrueText(_T("True"))
, m_strFalseText(_T("False"))
{
	m_pBindBool = pBindBool;
	_Init(bValue);
}

CXTPPropertyGridItemBool::~CXTPPropertyGridItemBool()
{
}

/////////////////////////////////////////////////////////////////////////////
//

void CXTPPropertyGridItemBool::_Init(BOOL bValue)
{
	SetBool(bValue);
	m_nFlags = xtpGridItemHasComboButton | xtpGridItemHasEdit;

	m_pConstraints->AddConstraint(m_strTrueText);
	m_pConstraints->AddConstraint(m_strFalseText);

	SetConstraintEdit(TRUE);
}

void CXTPPropertyGridItemBool::SetValue(CString strValue)
{
	SetBool(strValue.CompareNoCase(m_strTrueText) == 0);
}

void CXTPPropertyGridItemBool::SetBool(BOOL bValue)
{
	m_bValue = bValue;

	if (m_pBindBool)
	{
		*m_pBindBool = bValue;
	}

	CXTPPropertyGridItem::SetValue(bValue ? m_strTrueText: m_strFalseText);
}

void CXTPPropertyGridItemBool::BindToBool(BOOL* pBindBool)
{
	m_pBindBool = pBindBool;
	if (m_pBindBool)
	{
		*m_pBindBool = m_bValue;
	}
}

void CXTPPropertyGridItemBool::OnBeforeInsert()
{
	if( m_pBindBool && *m_pBindBool != m_bValue)
	{
		SetBool(*m_pBindBool);
	}
}


BOOL CXTPPropertyGridItemBool::SetValueText(CString& strValueText, CString strNewText)
{
	// see if the value exists.
	int iIndex = m_pConstraints->FindConstraint(strValueText);
	if (iIndex != -1)
	{
		// if this is the current value change it as well.
		if (GetValue() == strValueText) {
			CXTPPropertyGridItem::SetValue(strNewText);
		}

		// update the value.
		strValueText  = strNewText;
		m_pConstraints->GetConstraintAt(iIndex)->m_strConstraint = strValueText;
		return TRUE;
	}
	return FALSE;
}

BOOL CXTPPropertyGridItemBool::SetTrueFalseText(CString strTrueText, CString strFalseText)
{
	// update the "True" value text
	if (!SetValueText(m_strTrueText, strTrueText))
		return FALSE;

	// update the "False" value text
	if (!SetValueText(m_strFalseText, strFalseText))
		return FALSE;

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CXTPPropertyGridItemEnum

IMPLEMENT_DYNAMIC(CXTPPropertyGridItemEnum, CXTPPropertyGridItem)

CXTPPropertyGridItemEnum::CXTPPropertyGridItemEnum(CString strCaption, int nValue, int* pBindEnum)
: CXTPPropertyGridItem(strCaption)
{
	m_pBindEnum = pBindEnum;
	_Init(nValue);

}

CXTPPropertyGridItemEnum::CXTPPropertyGridItemEnum(UINT nID, int nValue, int* pBindEnum)
: CXTPPropertyGridItem(nID)
{
	m_pBindEnum = pBindEnum;
	_Init(nValue);
}

CXTPPropertyGridItemEnum::~CXTPPropertyGridItemEnum()
{
}

void CXTPPropertyGridItemEnum::_Init(int nValue)
{
	SetEnum(nValue);
	m_nFlags = xtpGridItemHasComboButton | xtpGridItemHasEdit;

	SetConstraintEdit(TRUE);
}

void CXTPPropertyGridItemEnum::SetValue(CString strValue)
{
	int nIndex = m_pConstraints->FindConstraint(strValue);
	ASSERT(nIndex >= 0);
	if (nIndex >= 0)
	{
		SetEnum(m_pConstraints->GetConstraintAt(nIndex));
	}
}

void CXTPPropertyGridItemEnum::SetEnum(int nValue)
{
	m_nValue = nValue;

	if (m_pBindEnum)
	{
		*m_pBindEnum = nValue;
	}

	int nIndex = m_pConstraints->FindConstraint(nValue);
	CXTPPropertyGridItem::SetValue(m_pConstraints->GetAt(nIndex));
}

void CXTPPropertyGridItemEnum::SetEnum(CXTPPropertyGridItemConstraint* pContraint)
{
	m_nValue = (int)pContraint->m_dwData;

	if (m_pBindEnum)
	{
		*m_pBindEnum = m_nValue;
	}

	CXTPPropertyGridItem::SetValue(pContraint->m_strConstraint);
}

void CXTPPropertyGridItemEnum::BindToEnum(int* pBindEnum)
{
	m_pBindEnum = pBindEnum;
	if (m_pBindEnum)
	{
		*m_pBindEnum = m_nValue;
	}
}

void CXTPPropertyGridItemEnum::OnBeforeInsert()
{
	if( m_pBindEnum && *m_pBindEnum != m_nValue)
	{
		SetEnum(*m_pBindEnum);
	}
}

void CXTPPropertyGridItemEnum::OnConstraintsChanged()
{
	if (m_strValue.IsEmpty())
	{
		int nIndex = m_pConstraints->FindConstraint(m_nValue);
		if (nIndex != -1)
			m_strValue = m_pConstraints->GetAt(nIndex);
	}

}



/////////////////////////////////////////////////////////////////////////////
// CXTPPropertyGridItemFlags

class CXTPPropertyGridItemFlags::CXTPPropertyGridItemFlag : public CXTPPropertyGridItemBool
{
public:

	CXTPPropertyGridItemFlag(CString strCaption, DWORD dwFlag)
		:  CXTPPropertyGridItemBool(strCaption), m_dwFlag(dwFlag)
	{
	}

	void OnValueChanged(CString strValue);

	DWORD m_dwFlag;
};

void CXTPPropertyGridItemFlags::CXTPPropertyGridItemFlag::OnValueChanged(CString strValue)
{
	SetValue(strValue);

	CXTPPropertyGridItemFlags* pParent = DYNAMIC_DOWNCAST(CXTPPropertyGridItemFlags, m_pParent);
	ASSERT(pParent);

	if (GetBool())
		pParent->m_nValue |= m_dwFlag; else pParent->m_nValue &= ~m_dwFlag;

	pParent->OnValueChanged(pParent->GetFlagsString());

}

IMPLEMENT_DYNAMIC(CXTPPropertyGridItemFlags, CXTPPropertyGridItem)

CXTPPropertyGridItemFlags::CXTPPropertyGridItemFlags(CString strCaption, int nValue, int* pBindFlags)
: CXTPPropertyGridItem(strCaption)
{
	m_pBindFlags = pBindFlags;
	_Init(nValue);

}

CXTPPropertyGridItemFlags::CXTPPropertyGridItemFlags(UINT nID, int nValue, int* pBindFlags)
: CXTPPropertyGridItem(nID)
{
	m_pBindFlags = pBindFlags;
	_Init(nValue);
}

CXTPPropertyGridItemFlags::~CXTPPropertyGridItemFlags()
{
}

void CXTPPropertyGridItemFlags::_Init(int nValue)
{
	SetFlags(nValue);
	m_nFlags = xtpGridItemHasEdit;
}

AFX_INLINE BOOL HasFlag(CString strValue, CString strFlag)
{
	strFlag.MakeLower();
	int nIndex = strValue.Find(strFlag);
	if (nIndex == -1)
		return FALSE;

	TCHAR chLast = nIndex + strFlag.GetLength() == strValue.GetLength()? _T(' '): strValue[strFlag.GetLength() + nIndex];
	TCHAR chFirst = nIndex == 0? _T(' '): strValue[nIndex - 1];

	return (chLast == ' ' || chLast == ',' || chLast == ';' || chLast == ']')
		&& (chFirst == ' ' || chFirst == ',' || chFirst == ';' || chFirst == '[');
}

void CXTPPropertyGridItemFlags::SetValue(CString strValue)
{
	int nValue = 0;
	strValue.MakeLower();

	CXTPPropertyGridItemConstraints* pConstraints = GetConstraints();
	for (int i = 0; i < pConstraints->GetCount(); i++)
	{
		if (HasFlag(strValue, pConstraints->GetAt(i)))
			nValue |= pConstraints->GetConstraintAt(i)->m_dwData;
	}

	SetFlags(nValue);
}

void CXTPPropertyGridItemFlags::SetFlags(int nValue)
{
	m_nValue = nValue;

	if (m_pBindFlags)
	{
		*m_pBindFlags = nValue;
	}

	UpdateChilds();

	CXTPPropertyGridItem::SetValue(GetFlagsString());
}

void CXTPPropertyGridItemFlags::BindToFlags(int* pBindFlags)
{
	m_pBindFlags = pBindFlags;
	if (m_pBindFlags)
	{
		*m_pBindFlags = m_nValue;
	}
}

CString CXTPPropertyGridItemFlags::GetFlagsString()
{
	CString str;

	CXTPPropertyGridItemConstraints* pConstraints = GetConstraints();
	int nValue = 0;

	for (int i = 0; i < pConstraints->GetCount(); i++)
	{
		CXTPPropertyGridItemConstraint* pConstraint =  pConstraints->GetConstraintAt(i);

		if ((nValue & pConstraint->m_dwData) == pConstraint->m_dwData)
			continue;

		if ((m_nValue & pConstraint->m_dwData) == pConstraint->m_dwData)
		{
			str += (str.IsEmpty()? _T(""): _T(";")) + pConstraint->m_strConstraint;
			nValue |= pConstraint->m_dwData;
		}
	}

	return  _T("[") + str + _T("]");
}

void CXTPPropertyGridItemFlags::UpdateChilds()
{
	CXTPPropertyGridItems* pItems = GetChilds();
	for (int i = 0; i < pItems->GetCount(); i++)
	{
		CXTPPropertyGridItemFlag* pItem = (CXTPPropertyGridItemFlag*)pItems->GetAt(i);
		pItem->SetBool((m_nValue & pItem->m_dwFlag) == pItem->m_dwFlag);
	}
}


void CXTPPropertyGridItemFlags::OnBeforeInsert()
{
	if( m_pBindFlags && *m_pBindFlags != m_nValue)
	{
		SetFlags(*m_pBindFlags);
	}
}

void CXTPPropertyGridItemFlags::OnConstraintsChanged()
{
	GetChilds()->Clear();

	CXTPPropertyGridItemConstraints* pConstraints = GetConstraints();

	for (int i = 0; i < pConstraints->GetCount(); i++)
	{
		AddChildItem(new CXTPPropertyGridItemFlag(pConstraints->GetAt(i), (int)pConstraints->GetConstraintAt(i)->m_dwData));
	}
	UpdateChilds();
	m_strValue = GetFlagsString();
}

