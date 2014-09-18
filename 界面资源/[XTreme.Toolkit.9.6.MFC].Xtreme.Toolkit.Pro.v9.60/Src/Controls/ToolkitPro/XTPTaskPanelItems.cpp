// XTPTaskPanelItems.cpp : implementation of the CXTPTaskPanelItems class.
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
#include "XTPPropExchange.h"

#include "XTPTaskPanelItems.h"
#include "XTPTaskPanelItem.h"
#include "XTPTaskPanelGroup.h"
#include "XTPTaskPanelGroupItem.h"
#include "XTPTaskPanel.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CXTPTaskPanelItems

IMPLEMENT_DYNCREATE(CXTPTaskPanelItems, CCmdTarget)

CXTPTaskPanelItems::CXTPTaskPanelItems(CXTPTaskPanel* pPanel)
{
	m_pPanel = pPanel;
	m_pOwner = NULL;

	EnableAutomation();


}

CXTPTaskPanelItems::~CXTPTaskPanelItems()
{
	for (int i = 0; i < m_arrItems.GetSize(); i++)
		m_arrItems[i]->InternalRelease();
}

CXTPTaskPanelItem* CXTPTaskPanelItems::Add(CXTPTaskPanelItem* pItem, UINT nID)
{
	pItem->SetID(nID);

	m_arrItems.Add(pItem);

	pItem->m_pItems = this;
	pItem->m_pPanel = m_pPanel;

	m_pPanel->Reposition();

	return pItem;

}

CXTPTaskPanelItem* CXTPTaskPanelItems::InsertAt(int nIndex, CXTPTaskPanelItem* pItem)
{
	m_arrItems.InsertAt(nIndex, pItem);

	pItem->m_pItems = this;
	pItem->m_pPanel = m_pPanel;

	m_pPanel->Reposition();

	return pItem;

}

CXTPTaskPanelItem* CXTPTaskPanelItems::Find(UINT nID)
{
	for (int i = 0; i < m_arrItems.GetSize(); i++)
	{
		if (m_arrItems[i]->GetID() == nID)
			return m_arrItems[i];
	}
	return NULL;
}

int CXTPTaskPanelItems::GetIndex(CXTPTaskPanelItem* pItem)
{
	for (int i = 0; i < m_arrItems.GetSize(); i++)
	{
		if (m_arrItems[i] == pItem)
			return i;
	}
	return -1;
}

void CXTPTaskPanelItems::Move(CXTPTaskPanelItem* pItem, int nIndex)
{
	ASSERT(pItem->m_pItems == this);

	int nItemIndex = GetIndex(pItem);

	if ((nItemIndex == -1) || nItemIndex == nIndex)
		return;

	m_arrItems.RemoveAt(nItemIndex);

	m_arrItems.InsertAt(nIndex, pItem);

	m_pPanel->Reposition(FALSE);


}

void CXTPTaskPanelItems::Clear(BOOL bReposition)
{
	for (int i = 0; i < m_arrItems.GetSize(); i++)
	{
		CXTPTaskPanelItem* pItem = m_arrItems[i];

		m_pPanel->OnItemRemoved(pItem);
		pItem->InternalRelease();
	}

	m_arrItems.RemoveAll();

	if (bReposition)
	{
		m_pPanel->Reposition();
	}
}

void CXTPTaskPanelItems::Remove(CXTPTaskPanelItem* pItem)
{
	int nIndex = GetIndex(pItem);

	if (nIndex != -1)
	{
		RemoveAt(nIndex);
	}
}
void CXTPTaskPanelItems::RemoveAt(int nIndex)
{
	if (nIndex < 0 || nIndex >= m_arrItems.GetSize())
		return;

	CXTPTaskPanelItem* pItem = m_arrItems[nIndex];
	m_pPanel->OnItemRemoved(pItem);

	pItem->InternalRelease();
	m_arrItems.RemoveAt(nIndex);

	m_pPanel->Reposition();
}


void CXTPTaskPanelItems::DoPropExchange(CXTPPropExchange* pPX)
{
	m_pPanel->SetLockRedraw(TRUE);

	try
	{
		pPX->m_dwData = (DWORD_PTR)m_pPanel;

		CXTPPropExchangeEnumeratorPtr pEnumerator(pPX->GetEnumerator( _T("Item")));
		POSITION pos = pEnumerator->GetPosition(GetCount());
		
		if (pPX->IsStoring())
		{
			for (int i = 0; i < GetCount(); i++)
			{
				CXTPTaskPanelItem* pItem = GetAt(i);
				
				CXTPPropExchangeSection sec(pEnumerator->GetNext(pos));
				PX_Object(&sec, pItem, GetDefaultItemClass());
			}
		} else
		{
			Clear(FALSE);

			while (pos)
			{
				CXTPTaskPanelItem* pItem = NULL;
				
				CXTPPropExchangeSection sec(pEnumerator->GetNext(pos));
				PX_Object(&sec, pItem, GetDefaultItemClass());

				if (!pItem)
					AfxThrowArchiveException(CArchiveException::badClass);

				pItem->m_pItems = this;

				m_arrItems.Add(pItem);
			}
		}
	}
	catch (COleException* pEx)
	{
		TRACE(_T("CXTPTaskPanelItems::Serialize. OLE exception: %x\r\n"),
			pEx->m_sc);
		pEx->Delete ();
	}
	catch (CArchiveException* pEx)
	{
		TRACE(_T("CXTPTaskPanelItems::Serialize. Archive exception\r\n"));
		pEx->Delete ();
	}

	m_pPanel->SetLockRedraw(FALSE);

}

CRuntimeClass* CXTPTaskPanelItems::GetDefaultItemClass()
{
	return RUNTIME_CLASS(CXTPTaskPanelItem);
}


//////////////////////////////////////////////////////////////////////////
// CXTPTaskPanelGroups

CXTPTaskPanelGroups::CXTPTaskPanelGroups(CXTPTaskPanel* pPanel)
	: CXTPTaskPanelItems(pPanel)
{
	EnableAutomation();
}

CRuntimeClass* CXTPTaskPanelGroups::GetDefaultItemClass()
{
	return RUNTIME_CLASS(CXTPTaskPanelGroup);
}

//////////////////////////////////////////////////////////////////////////
// CXTPTaskPanelGroupItems

CXTPTaskPanelGroupItems::CXTPTaskPanelGroupItems(CXTPTaskPanel* pPanel)
	: CXTPTaskPanelItems(pPanel)
{
	EnableAutomation();
}


CRuntimeClass* CXTPTaskPanelGroupItems::GetDefaultItemClass()
{
	return RUNTIME_CLASS(CXTPTaskPanelGroupItem);
}



