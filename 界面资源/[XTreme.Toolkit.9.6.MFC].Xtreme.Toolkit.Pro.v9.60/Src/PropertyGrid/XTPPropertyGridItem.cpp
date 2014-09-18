// XTPPropertyGridItem.cpp : implementation of the CXTPPropertyGridItem class.
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
#include "XTPPropertyGrid.h"
#include "XTPPropertyGridDefines.h"


#define SAFE_CALLPTR(p, f) if (p) p->f

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CXTPPropertyGridItem

IMPLEMENT_DYNAMIC(CXTPPropertyGridItem, CCmdTarget)

CXTPPropertyGridItem::CXTPPropertyGridItem(CString strCaption, LPCTSTR strValue, CString* pBindString)
{

	Init();
	SetPrompt(strCaption);
	m_strValue = strValue;

	BindToString(pBindString);
}


CXTPPropertyGridItem::CXTPPropertyGridItem(UINT nID, LPCTSTR strValue, CString* pBindString)
{
	Init();
	m_nID = nID;

	CString strPrompt;
	if (strPrompt.LoadString(nID))
	{
		SetPrompt(strPrompt);
	}

	m_strValue = strValue;
	BindToString(pBindString);
}

CXTPPropertyGridItem::~CXTPPropertyGridItem()
{
	if (m_pGrid && m_pGrid->m_pSelected == this)
		m_pGrid->m_pSelected = NULL;

	m_pChilds->Clear();

	if ( GetInplaceButton().m_pItem == this )
	{
		GetInplaceButton().DestroyItem( );
	}
	if ( GetInplaceList().m_pItem == this )
	{
		GetInplaceList().DestroyItem( );
	}
	if ( GetInplaceEdit().m_pItem == this )
	{
		GetInplaceEdit().DestroyItem( );
	}

	m_pChilds->InternalRelease();

	m_pConstraints->m_pItem = NULL;
	m_pConstraints->InternalRelease();
}

void CXTPPropertyGridItem::SetPrompt(CString strText)
{
	int nIndex = strText.Find('\n');
	if (nIndex == -1)
	{
		m_strCaption = strText;
	} else
	{
		m_strCaption = strText.Left(nIndex);
		m_strDescription = strText.Mid(nIndex + 1);
	}
}


void CXTPPropertyGridItem::Init()
{
	m_bExpanded = FALSE;
	m_nIndex = -1;
	m_bVisible = FALSE;
	m_pParent = 0;
	m_pGrid = 0;
	m_nIndent = 0;
	m_strValue = "";
	m_bReadOnly = FALSE;
	m_bCategory = FALSE;
	m_nID = 0;
	m_nFlags = xtpGridItemHasEdit;
	m_pConstraints = new CXTPPropertyGridItemConstraints(this);
	m_pConstraints->SetCurrent(-1);
	m_pBindString = 0;
	m_bConstraintEdit = FALSE;


	m_bUseMask = FALSE;
	m_bPassword = FALSE;
	m_chPrompt = _T('*');

	m_dwData = 0;

	EnableAutomation();



	m_pChilds = new CXTPPropertyGridItems();
}

CXTPPropertyGridItem* CXTPPropertyGridItem::AddChildItem(CXTPPropertyGridItem* pItem)
{
	return InsertChildItem(m_pChilds->GetCount(), pItem);
}

CXTPPropertyGridItem* CXTPPropertyGridItem::InsertChildItem(int nIndex, CXTPPropertyGridItem* pItem)
{
	ASSERT(!pItem->IsCategory() || IsCategory());

	if (nIndex < 0 || nIndex > m_pChilds->GetCount())
		nIndex = m_pChilds->GetCount();

	m_pChilds->InsertAt(nIndex, pItem);
	pItem->m_pGrid = pItem->m_pChilds->m_pGrid = m_pGrid;

	ASSERT(pItem->m_pParent == NULL);
	pItem->m_pParent = this;
	pItem->m_nIndent = m_nIndent + 1;

	pItem->OnAddChildItem();

	if (m_bVisible)
	{
		if (m_bExpanded) m_pGrid->SetPropertySort(m_pGrid->m_properetySort, TRUE);
		else if (m_pGrid->GetSafeHwnd() && m_pChilds->GetCount() == 1) m_pGrid->Invalidate(FALSE);
	}

	return pItem;
}

void CXTPPropertyGridItem::Collapse()
{
	if (!m_bExpanded) return;

	if (m_pGrid->m_nLockUpdate == 0)
	{
		if (m_pGrid->GetSafeHwnd() && m_bVisible)
		{
			m_pGrid->_DoCollapse(this);
			m_pGrid->OnSelectionChanged();
		}
	}

	m_bExpanded = FALSE;

	if (m_pGrid->GetSafeHwnd())
		m_pGrid->GetParent()->GetOwner()->SendMessage(XTPWM_PROPERTYGRID_NOTIFY, XTP_PGN_ITEMEXPANDCHANGED, (LPARAM)this);
}

void CXTPPropertyGridItem::Expand()
{
	if (m_bExpanded) return;

	if (m_pGrid->m_nLockUpdate == 0)
	{
		if (m_pGrid->GetSafeHwnd() && m_bVisible)
		{
			m_pGrid->_DoExpand(this, m_nIndex);
			m_pGrid->_RefreshIndexes();
			m_pGrid->OnSelectionChanged();
		}
	}

	m_bExpanded = TRUE;

	if (m_pGrid->GetSafeHwnd())
		m_pGrid->GetParent()->GetOwner()->SendMessage(XTPWM_PROPERTYGRID_NOTIFY, XTP_PGN_ITEMEXPANDCHANGED, (LPARAM)this);

}

BOOL CXTPPropertyGridItem::HasParent(CXTPPropertyGridItem* pParent)
{
	if (m_pParent == NULL) return FALSE;
	if (m_pParent == pParent) return TRUE;
	return m_pParent->HasParent(pParent);
}

void CXTPPropertyGridItem::Select()
{
	ASSERT(m_pGrid);

	if (m_bVisible)
	{
		m_pGrid->SetCurSel(m_nIndex);
		m_pGrid->OnSelectionChanged();
	}
}

void CXTPPropertyGridItem::OnDeselect()
{
	GetInplaceButton().HideWindow();
	OnValidateEdit();

}

void CXTPPropertyGridItem::OnValidateEdit()
{
	GetInplaceEdit().HideWindow();

	if (GetInplaceEdit().GetSafeHwnd() && GetInplaceEdit().m_pItem == this)
	{
		CString strValue;
		GetInplaceEdit().GetWindowText(strValue);

		if (m_strValue != strValue)
		{
			OnValueChanged(strValue);

		}
	}
}

void CXTPPropertyGridItem::SetEditText(CString str)
{
	if (!m_pGrid) return;

	if (GetInplaceEdit().GetSafeHwnd() && GetInplaceEdit().m_pItem == this)
	{
		GetInplaceEdit().SetWindowText(str);
	}
}

void CXTPPropertyGridItem::SetValue(CString strValue)
{
	m_strValue = strValue;

	if (m_pBindString)
	{
		*m_pBindString = strValue;
	}

	SetEditText(m_strValue);

	if (m_pGrid && m_pGrid->m_hWnd) m_pGrid->Invalidate(FALSE);
}

void CXTPPropertyGridItem::OnValueChanged(CString strValue)
{
	SetValue(strValue);
	m_pGrid->Invalidate(FALSE);
	m_pGrid->GetParent()->GetOwner()->SendMessage(XTPWM_PROPERTYGRID_NOTIFY, XTP_PGN_ITEMVALUE_CHANGED, (LPARAM)this);
}

void CXTPPropertyGridItem::OnSelect()
{
	ASSERT(m_bVisible);

	if (!m_bReadOnly && (m_nFlags & (xtpGridItemHasComboButton | xtpGridItemHasExpandButton)))
	{
		GetInplaceButton().Create(this, GetItemRect());
	} else
	{
		GetInplaceButton().HideWindow();
	}


	if (m_nFlags & xtpGridItemHasEdit)
	{

		GetInplaceEdit().SetValue(m_strValue);
		GetInplaceEdit().Create(this, GetValueRect());
		GetInplaceEdit().SetReadOnly(m_bReadOnly);
	}
	else
	{
		GetInplaceEdit().HideWindow();
	}
}

void CXTPPropertyGridItem::OnInplaceButtonDown()
{
	if (GetReadOnly())
		return;

	if (m_pGrid->GetParent()->GetOwner()->SendMessage(XTPWM_PROPERTYGRID_NOTIFY, XTP_PGN_INPLACEBUTTONDOWN, (LPARAM)this) == TRUE)
		return;

	if ((m_nFlags & xtpGridItemHasComboButton) && !m_pConstraints->IsEmpty())
	{
		GetInplaceList().Create(this, GetItemRect());
		m_pGrid->Invalidate(FALSE);
	}
}

CRect CXTPPropertyGridItem::GetValueRect()
{
	ASSERT(m_bVisible);
	CRect rc = GetItemRect();
	CRect rcValue(m_pGrid->GetDividerPos() + 1, rc.top + 1, rc.right, rc.bottom - 1);
	if (m_nFlags & (xtpGridItemHasComboButton | xtpGridItemHasExpandButton))
	{
		if (GetInplaceButton().m_pItem == this && GetInplaceButton().GetStyle() & WS_VISIBLE)
		{
			rcValue.right -= GetSystemMetrics(SM_CXHTHUMB);
		}
	}

	return rcValue;
}

CRect CXTPPropertyGridItem::GetItemRect()
{
	CRect rc;
	m_pGrid->GetItemRect(m_nIndex, rc);
	return rc;
}

BOOL CXTPPropertyGridItem::OnChar(UINT nChar)
{
	ASSERT(nChar != VK_TAB);

	if (m_nFlags & xtpGridItemHasEdit)
	{
		OnSelect();
		GetInplaceEdit().SetFocus();
		GetInplaceEdit().SetSel(0, -1);
		if (nChar != VK_TAB) GetInplaceEdit().SendMessage(WM_CHAR, nChar);
		return TRUE;
	} else if (nChar == VK_TAB)
	{
		CXTPPropertyGridInplaceButton& wndButton = GetInplaceButton();
		if (wndButton.GetSafeHwnd() && wndButton.IsWindowVisible() && wndButton.GetItem() == this)
		{
			wndButton.SetFocus();
		}
	}
	return FALSE;
}

void CXTPPropertyGridItem::OnLButtonDblClk()
{
	if (HasChilds())
	{
		if(m_bExpanded) Collapse(); else Expand();
	}
	else
	{
		OnSelect();
		if (m_nFlags & xtpGridItemHasEdit)
		{
			if (!GetReadOnly() && GetInplaceEdit().SelectConstraint(+1, TRUE))
			{
				OnValidateEdit();

			} else
			{
				GetInplaceEdit().SetFocus();
				GetInplaceEdit().SetSel(0, -1);
			}
		} else if (!GetReadOnly())
		{
			CXTPPropertyGridItemConstraints* pList = GetConstraints();
			if (pList->IsEmpty())
				return ;

			int nIndex = pList->FindConstraint(m_strValue);

			nIndex += +1;
			if (nIndex >= pList->GetCount()) nIndex = 0;
			if (nIndex < 0) nIndex = (ULONG)pList->GetCount() - 1;

			pList->SetCurrent(nIndex);
			OnValueChanged(pList->GetAt(nIndex));
		}
	}
}

BOOL CXTPPropertyGridItem::OnLButtonDown(UINT, CPoint point)
{
	Select();

	int nOffset = (m_nIndent - (IsCategory()? 0: 1)) * XTP_PGI_EXPAND_BORDER;

	if(point.x >= nOffset && point.x <= XTP_PGI_EXPAND_BORDER + nOffset)
	{
		if(m_bExpanded) Collapse(); else Expand();
	}

	CRect rc = GetValueRect();
	if ((m_nFlags & xtpGridItemHasEdit) && rc.PtInRect(point))
	{
		mouse_event(GetSystemMetrics(SM_SWAPBUTTON)? MOUSEEVENTF_RIGHTDOWN: MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
	}

	return TRUE;
}

void CXTPPropertyGridItem::BindToString(CString* pBindString)
{
	m_pBindString = pBindString;
	if (m_pBindString)
	{
		*m_pBindString = m_strValue;
	}
}

void CXTPPropertyGridItem::OnBeforeInsert()
{
	if( m_pBindString && *m_pBindString != m_strValue)
	{
		m_strValue = *m_pBindString;
	}
}

CXTPPropertyGridInplaceEdit& CXTPPropertyGridItem::GetInplaceEdit()
{
	if (m_pGrid) return m_pGrid->m_wndEdit;

	static CXTPPropertyGridInplaceEdit  wndEdit;
	return wndEdit;
}
CXTPPropertyGridInplaceButton& CXTPPropertyGridItem::GetInplaceButton()
{
	if (m_pGrid) return m_pGrid->m_wndButton;

	static CXTPPropertyGridInplaceButton    wndButton;
	return wndButton;
}
CXTPPropertyGridInplaceList& CXTPPropertyGridItem::GetInplaceList()
{
	if (m_pGrid) return m_pGrid->m_wndListBox;

	static CXTPPropertyGridInplaceList  wndListBox;
	return wndListBox;
}



// Deletes the item.
void CXTPPropertyGridItem::Remove()
{
	CXTPPropertyGridItems* pItems = GetParentItem() == 0? m_pGrid->m_pCategories: GetParentItem()->m_pChilds;
	ASSERT(pItems);

	pItems->m_arrItems.RemoveAt(pItems->Find(this));
	m_pGrid->Refresh();

	InternalRelease();
}

BOOL CXTPPropertyGridItem::IsSelected()
{
	return m_pGrid && m_pGrid->GetSelectedItem() == this;
}

void CXTPPropertyGridItem::Refresh(BOOL bInvalidate /*= TRUE*/)
{
	if (m_pGrid && m_pGrid->GetSafeHwnd())
	{
		if (IsSelected())
		{
			OnDeselect();
			OnSelect();

		}
		if (bInvalidate)
		{
			m_pGrid->Invalidate(FALSE);
		}
	}
}

void CXTPPropertyGridItem::SetReadOnly(BOOL bReadOnly)
{
	if (m_bReadOnly != bReadOnly)
	{	
		m_bReadOnly = bReadOnly;
		Refresh(TRUE);
	}
}

void CXTPPropertyGridItem::SetMask(LPCTSTR strMask, LPCTSTR strLiteral, TCHAR chPrompt /*= _T('_')*/)
{
	m_bUseMask = TRUE;
	m_strMask = strMask;
	m_strLiteral = strLiteral;
	m_chPrompt = chPrompt;
	Refresh(FALSE);
}

CString CXTPPropertyGridItem::GetViewValue()
{
	CString str = GetValue();
	if (m_bPassword)
	{
		return CString(m_chPrompt, str.GetLength());
	}
	return str;
}

CString CXTPPropertyGridItem::GetMaskedText()
{
	if (m_bUseMask)
	{
		int nLength = m_strValue.GetLength();

		CString strBuffer;
		for (int i = 0; i < nLength; ++i )
		{
			if ( m_strLiteral[i] == m_chPrompt )
			{
				strBuffer += m_strValue.GetAt( i );
			}
		}

		return strBuffer;
	}
	else
	{
		return m_strValue;
	}
}

void CXTPPropertyGridItem::SetMaskedText(LPCTSTR lpszMaskedText)
{
	CString strMaskedText = lpszMaskedText;

	if (m_bUseMask)
	{
		int nLen = strMaskedText.GetLength();
		int x = 0;

		m_strValue = m_strLiteral;

		for (int i = 0; (i < m_strValue.GetLength()) && (x < nLen); i++)
		{
			if (strMaskedText[x] == m_strValue[i])
			{
				x ++;
			} else if (m_strValue[i] == m_chPrompt)
			{
				m_strValue.SetAt(i, strMaskedText[x]);
				x ++;
			}
		}
	}
	else
	{
		m_strValue = strMaskedText;
	}

	SetValue(m_strValue);
}

void CXTPPropertyGridItem::SetDescription(LPCTSTR lpszDescription) 
{
	m_strDescription = lpszDescription;
	if (IsSelected()) GetGrid()->GetParent()->Invalidate(FALSE);
}



/////////////////////////////////////////////////////////////////////////////
// CXTPPropertyGridItems

CXTPPropertyGridItems::CXTPPropertyGridItems()
{
	m_pGrid = 0;

}

void CXTPPropertyGridItems::Clear()
{

	if (m_pGrid)
	{
		if (m_pGrid->m_nLockUpdate == 0)
			m_pGrid->ResetContent();
		m_pGrid->m_nLockUpdate++;
	}

	for (int i = 0; i < GetCount(); i++)
	{
		m_arrItems[i]->InternalRelease();
	}
	m_arrItems.RemoveAll();

	if (m_pGrid)
	{
		m_pGrid->m_nLockUpdate--;
		if (m_pGrid->m_nLockUpdate == 0)
			m_pGrid->Refresh();

	}

}

CXTPPropertyGridItem* CXTPPropertyGridItems::GetAt(int nIndex)
{
	if (nIndex >= 0 && nIndex < GetCount())
	{
		return m_arrItems.GetAt(nIndex);
	}
	return 0;
}

void CXTPPropertyGridItems::RemoveAt (int nIndex)
{
	ASSERT(m_pGrid);

	CXTPPropertyGridItem* pItem = GetAt(nIndex);

	if (pItem)
	{
		m_arrItems.RemoveAt(nIndex);
		m_pGrid->Refresh();

		pItem->InternalRelease();
	}
}

int CXTPPropertyGridItems::Find(CXTPPropertyGridItem* pItem)
{
	for (int i = 0; i < GetCount(); i++)
	{
		if (m_arrItems[i] == pItem)
			return i;
	}
	return -1;
}


void CXTPPropertyGridItems::AddTail(CXTPPropertyGridItems* pItems)
{
	ASSERT(this != pItems);

	for (int i = 0; i < pItems->GetCount(); i++)
	{
		AddTail(pItems->GetAt(i));
	}
}

int CXTPPropertyGridItems::CompareFunc(const CXTPPropertyGridItem** ppItem1, const CXTPPropertyGridItem** ppItem2)
{
	return (*ppItem1)->GetCaption().Compare((*ppItem2)->GetCaption());
}

void CXTPPropertyGridItems::Sort()
{
	typedef int (_cdecl *GENERICCOMPAREFUNC)(const void *,const void*);

	qsort(m_arrItems.GetData(), m_arrItems.GetSize(), sizeof(CXTPPropertyGridItem*), (GENERICCOMPAREFUNC)CompareFunc);
}

CXTPPropertyGridItem* CXTPPropertyGridItems::FindItem(CString strCaption)
{
	for (int i = 0; i < GetCount(); i++)
	{
		CXTPPropertyGridItem* pItem = GetAt(i);
		if (pItem->GetCaption() == strCaption)
			return pItem;

		CXTPPropertyGridItem* pChild = pItem->m_pChilds->FindItem(strCaption);
		if (pChild) return pChild;
	}
	return NULL;
}

CXTPPropertyGridItem* CXTPPropertyGridItems::FindItem(UINT nID)
{
	for (int i = 0; i < GetCount(); i++)
	{
		CXTPPropertyGridItem* pItem = GetAt(i);
		if (pItem->GetID() == nID)
			return pItem;

		CXTPPropertyGridItem* pChild = pItem->m_pChilds->FindItem(nID);
		if (pChild) return pChild;
	}
	return NULL;
}



//////////////////////////////////////////////////////////////////////////
// CXTPPropertyGridItemConstraint
CXTPPropertyGridItemConstraint::CXTPPropertyGridItemConstraint()
{
	m_nIndex = 0;
	m_dwData = 0;
}
/////////////////////////////////////////////////////////////////////////////
// CXTPPropertyGridItemConstraints

// Summary: Constructs a CXTPPropertyGridItemConstraints object
CXTPPropertyGridItemConstraints::CXTPPropertyGridItemConstraints(CXTPPropertyGridItem* pItem)
{
	m_pItem = pItem;

}

CXTPPropertyGridItemConstraints::~CXTPPropertyGridItemConstraints()
{
	RemoveAll();
}

CXTPPropertyGridItemConstraint* CXTPPropertyGridItemConstraints::AddConstraint(CString str, DWORD dwData)
{
	CXTPPropertyGridItemConstraint* pConstaint = new CXTPPropertyGridItemConstraint();

	pConstaint->m_strConstraint = str;
	pConstaint->m_dwData = dwData;
	pConstaint->m_nIndex = (int)m_arrConstraints.Add(pConstaint);

	SAFE_CALLPTR(m_pItem, OnConstraintsChanged());

	return pConstaint;
}

CString CXTPPropertyGridItemConstraints::GetAt(int nIndex)
{
	return nIndex >= 0 && nIndex < GetCount()? m_arrConstraints.GetAt(nIndex)->m_strConstraint: _T("");
}

void CXTPPropertyGridItemConstraints::RemoveAll()
{
	for (int i = 0; i < GetCount(); i++)
		m_arrConstraints[i]->InternalRelease();
	m_arrConstraints.RemoveAll();

	SAFE_CALLPTR(m_pItem, OnConstraintsChanged());
}

void CXTPPropertyGridItemConstraints::RemoveAt(int nIndex)
{
	m_arrConstraints[nIndex]->InternalRelease();
	m_arrConstraints.RemoveAt(nIndex);

	for (int i = nIndex; i < GetCount(); i++)
		m_arrConstraints[i]->m_nIndex = i;

	SAFE_CALLPTR(m_pItem, OnConstraintsChanged());
}

int CXTPPropertyGridItemConstraints::FindConstraint(CString str)
{
	for (int i = 0; i < GetCount(); i++)
	{
		if (str == GetAt(i))
			return i;
	}
	return -1;
}

int CXTPPropertyGridItemConstraints::FindConstraint(DWORD dwData)
{
	for (int i = 0; i < GetCount(); i++)
	{
		if (dwData == GetConstraintAt(i)->m_dwData)
			return i;
	}
	return -1;
}

CXTPPropertyGridItemConstraint* CXTPPropertyGridItemConstraints::GetConstraintAt(int nIndex)
{
	return m_arrConstraints[nIndex];
}

int CXTPPropertyGridItemConstraints::CompareFunc(const CXTPPropertyGridItemConstraint** ppConstraint1, const CXTPPropertyGridItemConstraint** ppConstraint2)
{
	return (*ppConstraint1)->m_strConstraint.Compare((*ppConstraint2)->m_strConstraint);

}

void CXTPPropertyGridItemConstraints::Sort()
{
	typedef int (_cdecl *GENERICCOMPAREFUNC)(const void *,const void*);

	qsort(m_arrConstraints.GetData(), (size_t)m_arrConstraints.GetSize(), sizeof(CXTPPropertyGridItemConstraint*), (GENERICCOMPAREFUNC)CompareFunc);
}


//////////////////////////////////////////////////////////////////////////
// CXTPPropertyGridItemCategory

CXTPPropertyGridItemCategory::CXTPPropertyGridItemCategory(CString strCaption)
	:CXTPPropertyGridItem(strCaption)
{
	m_bCategory = TRUE;
	m_nFlags = 0;
}

CXTPPropertyGridItemCategory::CXTPPropertyGridItemCategory(UINT nID)
	:CXTPPropertyGridItem(nID)
{
	m_bCategory = TRUE;
	m_nFlags = 0;
}





///////////////////////////////////////////////////////////////////////////////////
//  Acteve X Methods
//
