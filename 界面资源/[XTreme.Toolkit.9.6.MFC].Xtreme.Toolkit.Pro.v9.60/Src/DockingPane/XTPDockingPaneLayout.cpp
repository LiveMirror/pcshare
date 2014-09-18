// XTPDockingPaneLayout.cpp : implementation of the ÑXTPDockingPaneLayout class.
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
#include "XTPVC80Helpers.h"
#include "XTPTabManager.h"
#include "XTPPropExchange.h"

#include "XTPDockingPaneLayout.h"
#include "XTPDockingPaneBase.h"
#include "XTPDockingPaneSplitterContainer.h"
#include "XTPDockingPaneTabbedContainer.h"
#include "XTPDockingPane.h"
#include "XTPDockingPaneMiniWnd.h"
#include "XTPDockingPaneManager.h"
#include "XTPDockingPaneAutoHidePanel.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

AFX_STATIC_DATA const TCHAR _xtRootSection[] =  _T("DockingPaneLayouts");
AFX_STATIC_DATA const TCHAR _xtPaneSection[] = _T("Pane-%d");
AFX_STATIC_DATA const TCHAR _xtPanes[] = _T("Panes");
AFX_STATIC_DATA const TCHAR _xtSummary[] = _T("Summary");

CXTPDockingPaneLayout::CXTPDockingPaneLayout(CXTPDockingPaneManager* pManager)
	: m_pManager(pManager)
{

	m_pClient = m_pManager->OnCreatePane(xtpPaneTypeClient, this);
	m_pTopContainer = (CXTPDockingPaneSplitterContainer*)m_pManager->OnCreatePane(xtpPaneTypeSplitterContainer, this);
	m_pTopContainer->Init(m_pClient, TRUE, m_pManager->GetSite());

	m_bUserLayout = TRUE;

	memset(m_wndPanels, 0, sizeof(m_wndPanels));
}

CXTPDockingPaneLayout::~CXTPDockingPaneLayout()
{
	Free();
}

BOOL CXTPDockingPaneLayout::IsValid()
{
	return (m_pTopContainer != NULL) && (m_pClient != NULL)
		&& (m_pClient->GetType() == xtpPaneTypeClient) && (m_pManager != NULL);
}

void CXTPDockingPaneLayout::Free()
{
	while (!m_lstStack.IsEmpty())
	{
		CXTPDockingPaneBase* pPane = m_lstStack.RemoveTail();
		if (pPane->GetType() == xtpPaneTypeDockingPane)
		{
			((CXTPDockingPane*)pPane)->InternalRelease();

		} else
			delete pPane;
	}

	m_lstPanes.RemoveAll();

	memset(m_wndPanels, 0, sizeof(m_wndPanels));

	m_pTopContainer = 0;
	m_pClient = 0;
}

BOOL CXTPDockingPaneLayout::_Load(CXTPPropExchange* pPX)
{
	ASSERT(pPX->IsLoading());

	Free();

	CXTPPropExchangeSection secSummary(pPX->GetSection(_xtSummary));

	int nCount = 0;
	PX_Int(&secSummary, _xtPanes, nCount);

	if (nCount < 2)
		return FALSE;

	secSummary->ExchangeSchema();


	TCHAR szSection[256];
	CMapWordToPtr map;

	for (int i = 1; i <= nCount; i++)
	{
		wsprintf(szSection, _xtPaneSection, i);

		CXTPPropExchangeSection secPane(pPX->GetSection(szSection));

		int nType = -1;
		PX_Int(&secPane, _T("Type"), nType, -1);

		CXTPDockingPaneBase* pPane = m_pManager->OnCreatePane((XTPDockingPaneType)nType, this);

		if (!pPane)
		{
			return FALSE;
		}
		pPane->m_nIndex = i;
		map.SetAt((WORD)i, pPane);
	}

	ASSERT(map.GetCount() == m_lstStack.GetCount());

	POSITION pos = m_lstStack.GetHeadPosition();
	while(pos)
	{
		CXTPDockingPaneBase* pPane = m_lstStack.GetNext(pos);

		wsprintf(szSection, _xtPaneSection, pPane->m_nIndex);

		CXTPPropExchangeSection secPane(pPX->GetSection(szSection));
		secPane->m_dwData = (DWORD_PTR)&map;

		if (!pPane->DoPropExchange(&secPane))
		{
			return FALSE;
		}
	}

	int nIndex = 0;
	if (!PX_Int(&secSummary, _T("TopContainer"), nIndex))
		return FALSE;

	m_pTopContainer = (CXTPDockingPaneSplitterContainer*)map[(WORD)nIndex];
	if (!m_pTopContainer)
		return FALSE;

	if (!PX_Int(&secSummary, _T("Client"), nIndex))
		return FALSE;;

	m_pClient = (CXTPDockingPaneBase*)map[(WORD)nIndex];
	if (!m_pClient || m_pClient->GetType() != xtpPaneTypeClient)
		return FALSE;

	return TRUE;

}

// The main difference between the .Net versions and the normal versions of _Save is that in the .Net
// versions all the save is done is done in the exact same order of the load.

void CXTPDockingPaneLayout::_Save(CXTPPropExchange* pPX)
{
	ASSERT(!pPX->IsLoading());

	pPX->EmptySection();

	_FreeEmptyPanes();

	CXTPPropExchangeSection secSummary(pPX->GetSection(_xtSummary));

	int nCount = (int)m_lstStack.GetCount();
	PX_Int(&secSummary, _xtPanes, nCount);

	secSummary->ExchangeSchema();


	TCHAR szSection[256];

	// Assign an unique value to each pane
	POSITION pos = m_lstStack.GetHeadPosition();
	int nIndex = 1;  // important
	while(pos)
	{
		CXTPDockingPaneBase* pPane = m_lstStack.GetNext(pos);
		pPane->m_nIndex = nIndex++;

		wsprintf(szSection, _xtPaneSection, pPane->m_nIndex);

		CXTPPropExchangeSection secPane(pPX->GetSection(szSection));
		int nType = pPane->m_type;
		PX_Int(&secPane, _T("Type"), nType);
	}

	// Save all panes
	pos = m_lstStack.GetHeadPosition();
	while(pos)
	{
		CXTPDockingPaneBase* pPane = m_lstStack.GetNext(pos);
		wsprintf(szSection, _xtPaneSection, pPane->m_nIndex);

		CXTPPropExchangeSection secPane(pPX->GetSection(szSection));
		pPane->DoPropExchange(&secPane);
	}

	PX_Int(&secSummary, _T("TopContainer"), m_pTopContainer->m_nIndex);
	PX_Int(&secSummary, _T("Client"), m_pClient->m_nIndex);

}


BOOL CXTPDockingPaneLayout::DoPropExchange(CXTPPropExchange* pPX)
{
	if (pPX->IsLoading())
		return _Load(pPX);

	_Save(pPX);
	return TRUE;
}


BOOL CXTPDockingPaneLayout::Load(CString strSection)
{
	HKEY hRoot = AfxGetApp()->GetSectionKey(_xtRootSection);
	ASSERT(hRoot);

	CXTPPropExchangeRegistry px(TRUE, hRoot, strSection);
	BOOL bResult = DoPropExchange(&px);

	RegCloseKey(hRoot);
	return bResult;
}

void CXTPDockingPaneLayout::Save(CString strSection)
{
	HKEY hRoot = AfxGetApp()->GetSectionKey(_xtRootSection);
	ASSERT(hRoot);

	CXTPPropExchangeRegistry px(FALSE, hRoot, strSection);
	DoPropExchange(&px);

	RegCloseKey(hRoot);
}


void CXTPDockingPaneLayout::Serialize(CArchive& ar)
{
	CXTPPropExchangeArchive px(ar);
	DoPropExchange(&px);
}


#ifndef _XTP_EXCLUDE_XML


BOOL CXTPDockingPaneLayout::LoadFromNode(CXTPDOMNodePtr xmlNode, CString strSection)
{
	if (xmlNode == 0)
		return FALSE;

	CXTPPropExchangeXMLNode px(TRUE, xmlNode, strSection);
	return DoPropExchange(&px);
}


BOOL CXTPDockingPaneLayout::LoadFromFile(CString strFileName, CString strSection)
{
	CXTPPropExchangeXMLNode px(TRUE, 0, _xtRootSection);
	if (!px.LoadFromFile(strFileName))
		return FALSE;

	CXTPPropExchangeSection pxSection(px.GetSection(strSection));
	if (!pxSection->OnBeforeExchange())
		return FALSE;

	return DoPropExchange(&pxSection);



}

void CXTPDockingPaneLayout::SaveToNode (CXTPDOMNodePtr xmlNode, CString strSection)
{
	if (xmlNode == 0)
		return;

	CXTPPropExchangeXMLNode px(FALSE, xmlNode, strSection);
	DoPropExchange(&px);
}

void CXTPDockingPaneLayout::SaveToFile (CString strFileName, CString strSection)
{
	CXTPPropExchangeXMLNode px(FALSE, 0, _xtRootSection);

	CXTPPropExchangeSection pxSection(px.GetSection(strSection));
	DoPropExchange(&pxSection);

	px.SaveToFile(strFileName);
}

#endif



BOOL CXTPDockingPaneBase::DoPropExchange(CXTPPropExchange* pPX)
{
	PX_Long(pPX, _T("DockingCX"), m_szDocking.cx, 0);
	PX_Long(pPX, _T("DockingCY"), m_szDocking.cy, 0);

	return TRUE;
}

BOOL CXTPDockingPane::DoPropExchange(CXTPPropExchange* pPX)
{
	CXTPDockingPaneBase::DoPropExchange(pPX);

	PX_String(pPX, _T("Title"), m_strTitle);
	PX_Long(pPX, _T("ID"), m_nID, 0);

	PX_Int(pPX, _T("IconID"), m_nIconID, -1);
	PX_ULong(pPX, _T("Options"), m_dwOptions, 0);

	int nIndex = 0;
	CMapWordToPtr* pMap = (CMapWordToPtr*)pPX->m_dwData;

	if (!pPX->IsLoading())
	{
		XTP_DOCKINGPANE_INFO* pInfo = m_pLayout->FindPane(this);
		ASSERT(pInfo);
		if (pInfo)
		{

			nIndex = pInfo->pDockingHolder? pInfo->pDockingHolder->m_nIndex: 0;
			PX_Int(pPX,  _T("DockingHolder"), nIndex, 0);
			nIndex = pInfo->pFloatingHolder? pInfo->pFloatingHolder->m_nIndex: 0;
			PX_Int(pPX,  _T("FloatingHolder"), nIndex, 0);
			nIndex = pInfo->pLastHolder? pInfo->pLastHolder->m_nIndex: 0;
			PX_Int(pPX,  _T("LastHolder"), nIndex, 0);
		}
	}
	else
	{
		XTP_DOCKINGPANE_INFO info (this);

		PX_Int(pPX, _T("DockingHolder"), nIndex, 0);
		if (nIndex) info.pDockingHolder = (CXTPDockingPaneBase*)(*pMap)[(WORD)nIndex];
		PX_Int(pPX, _T("FloatingHolder"), nIndex, 0);
		if (nIndex) info.pFloatingHolder = (CXTPDockingPaneBase*)(*pMap)[(WORD)nIndex];
		PX_Int(pPX, _T("LastHolder"), nIndex, 0);
		if (nIndex) info.pLastHolder = (CXTPDockingPaneBase*)(*pMap)[(WORD)nIndex];
		m_pLayout->m_lstPanes.AddTail(info);

	}

	PX_Long(pPX, _T("MinTrackX"), m_ptMinTrackSize.x, 0);
	PX_Long(pPX, _T("MinTrackY"), m_ptMinTrackSize.y, 0);
	PX_Long(pPX, _T("MaxTrackX"), m_ptMaxTrackSize.x, 32000);
	PX_Long(pPX, _T("MaxTrackY"), m_ptMaxTrackSize.y, 32000);

	return m_nID > 0;
}

BOOL CXTPDockingPaneSplitterContainer::DoPropExchange(CXTPPropExchange* pPX)
{
	CXTPDockingPaneBase::DoPropExchange(pPX);

	PX_Bool(pPX, _T("Horiz"), m_bHoriz, FALSE);

	int nCount = 0;

	if (!pPX->IsLoading())
	{
		nCount = (int)m_lstPanes.GetCount();
		PX_Int(pPX, _xtPanes, nCount);

		int nIndex = 1;
		TCHAR szPane[256];
		POSITION pos = m_lstPanes.GetHeadPosition();
		while (pos)
		{
			CXTPDockingPaneBase* pPane = m_lstPanes.GetNext(pos);
			wsprintf(szPane, _xtPaneSection, nIndex++);

			PX_Int(pPX, szPane, pPane->m_nIndex);
		}
	}
	else
	{
		PX_Int(pPX, _xtPanes, nCount, 0);

		int nIndex = 0;
		TCHAR szPane[256];
		CMapWordToPtr* pMap = (CMapWordToPtr*)pPX->m_dwData;

		for (int i = 1; i <= nCount; i++)
		{
			wsprintf(szPane, _xtPaneSection, i);
			PX_Int(pPX, szPane, nIndex, 0);
			ASSERT(nIndex > 0);

			CXTPDockingPaneBase* pPane = NULL;
			ASSERT(pMap->Lookup((WORD)nIndex, (void*&)pPane));

			pPane = (CXTPDockingPaneBase*)((*pMap)[(WORD)nIndex]);
			if (!pPane) return FALSE;

			_InsertPane(pPane);
		}

		ASSERT(nCount > 0);

	}
	return TRUE;
}

BOOL CXTPDockingPaneAutoHidePanel::DoPropExchange(CXTPPropExchange* pPX)
{
	CXTPDockingPaneBase::DoPropExchange(pPX);

	int direction = m_direction;
	PX_Int(pPX, _T("Direction"), direction);

	int nCount = 0;

	if (!pPX->IsLoading())
	{
		nCount = (int)m_lstPanes.GetCount();
		PX_Int(pPX, _xtPanes, nCount);

		int nIndex = 1;
		TCHAR szPane[256];
		POSITION pos = m_lstPanes.GetHeadPosition();
		while (pos)
		{
			CXTPDockingPaneBase* pPane = m_lstPanes.GetNext(pos);
			wsprintf(szPane, _xtPaneSection, nIndex++);
			PX_Int(pPX, szPane, pPane->m_nIndex);
		}
	}
	else
	{
		m_direction = (XTPDockingPaneDirection)direction;

		PX_Int(pPX, _xtPanes, nCount, 0);

		int nIndex = 0;
		TCHAR szPane[256];
		CMapWordToPtr* pMap = (CMapWordToPtr*)pPX->m_dwData;

		for (int i = 1; i <= nCount; i++)
		{
			wsprintf(szPane, _xtPaneSection, i);
			PX_Int(pPX, szPane, nIndex, 0);
			ASSERT(nIndex > 0);

			CXTPDockingPaneBase* pPane = NULL;
			ASSERT(pMap->Lookup((WORD)nIndex, (void*&)pPane));

			pPane = (CXTPDockingPaneBase*)((*pMap)[(WORD)nIndex]);
			if (!pPane) return FALSE;

			_InsertPane(pPane);
		}

		m_pLayout->m_wndPanels[m_direction] = this;
	}

	return TRUE;
}

BOOL CXTPDockingPaneTabbedContainer::DoPropExchange(CXTPPropExchange* pPX)
{
	CXTPDockingPaneBase::DoPropExchange(pPX);

	int nCount, nSelected;

	if (!pPX->IsLoading())
	{
		nCount = (int)m_lstPanes.GetCount();
		PX_Int(pPX, _xtPanes, nCount);

		nSelected = m_pSelectedPane? m_pSelectedPane->m_nIndex: 0;
		PX_Int(pPX, _T("Selected"), nSelected, 0);

		POSITION pos = m_lstPanes.GetHeadPosition();
		int nIndex = 1;
		while (pos)
		{
			CXTPDockingPaneBase* pPane = m_lstPanes.GetNext(pos);

			TCHAR szPane[256];
			wsprintf(szPane, _xtPaneSection, nIndex++);

			PX_Int(pPX, szPane, pPane->m_nIndex);
		}

	}
	else
	{
		PX_Int(pPX, _xtPanes, nCount);
		PX_Int(pPX, _T("Selected"), nSelected, 0);

		TCHAR szPane[256];
		CXTPDockingPane* pSelected = NULL;
		int nIndex = 0;
		CMapWordToPtr* pMap = (CMapWordToPtr*)pPX->m_dwData;

		for (int i = 1; i <= nCount; i++)
		{
			wsprintf(szPane, _xtPaneSection, i);
			PX_Int(pPX, szPane, nIndex, 0);
			ASSERT(nIndex > 0);

			CXTPDockingPaneBase* pPane = NULL;
			ASSERT(pMap->Lookup((WORD)nIndex, (void*&)pPane));

			pPane = (CXTPDockingPaneBase*)((*pMap)[(WORD)nIndex]);

			if (!pPane || pPane->GetType() != xtpPaneTypeDockingPane) return FALSE;

			_InsertPane((CXTPDockingPane*)pPane);

			if (nIndex == nSelected) pSelected = (CXTPDockingPane*)pPane;
		}

		if (pSelected)
		{
			SelectPane(pSelected);
		}
	}

	return TRUE;
}

BOOL CXTPDockingPaneMiniWnd::DoPropExchange(CXTPPropExchange* pPX)
{
	CXTPDockingPaneBase::DoPropExchange(pPX);

	PX_Long(pPX, _T("WindowRectTopPos"), m_rcWindow.top);
	PX_Long(pPX, _T("WindowRectBottomPos"), m_rcWindow.bottom);
	PX_Long(pPX, _T("WindowRectLeftPos"), m_rcWindow.left);
	PX_Long(pPX, _T("WindowRectRightPos"), m_rcWindow.right);
	PX_Bool(pPX, _T("Collapsed"), m_bCollapsed, FALSE);
	PX_Int(pPX, _T("ExpandedHeight"), m_nExpandedHeight, 0);

	if (!pPX->IsLoading())
	{
		int nTopContainer = m_pTopContainer? m_pTopContainer->m_nIndex: 0;
		PX_Int(pPX, _T("TopContaner"), nTopContainer);
	}
	else
	{
		int nTopContainer = 0;
		PX_Int(pPX, _T("TopContaner"), nTopContainer);

		if (nTopContainer == 0)
			return FALSE;

		CMapWordToPtr* pMap = (CMapWordToPtr*)pPX->m_dwData;

		CXTPDockingPaneBase* pTop = (CXTPDockingPaneBase*)(*pMap)[(WORD)nTopContainer];
		if (!pTop || pTop->GetType() != xtpPaneTypeSplitterContainer)
			return FALSE;

		m_pTopContainer = (CXTPDockingPaneSplitterContainer*)pTop;
		m_pTopContainer->m_pParentContainer = this;

	}

	return TRUE;
}

void CXTPDockingPaneLayout::Copy(CXTPDockingPaneLayout* pLayout)
{
	Free();

	CXTPPaneToPaneMap map;

	// Clone them
	POSITION pos = pLayout->m_lstStack.GetHeadPosition();
	while(pos)
	{
		CXTPDockingPaneBase* pPane = pLayout->m_lstStack.GetNext(pos);
		if (pPane->m_pParentContainer == NULL) // hidden pane, top pane, miniwnd.
		{
			pPane->Clone(this, &map);
		}
	}

	ASSERT(map.GetCount() == pLayout->m_lstStack.GetCount());
	ASSERT(map.Lookup(pLayout->m_pClient, m_pClient));
	ASSERT(map.Lookup(pLayout->m_pTopContainer, (CXTPDockingPaneBase*&)m_pTopContainer));

	m_pClient = map[pLayout->m_pClient];
	m_pTopContainer = (CXTPDockingPaneSplitterContainer*)map[pLayout->m_pTopContainer];

	for (int i = 0; i < 4; i++)
		if (pLayout->m_wndPanels[i]) m_wndPanels[i] = (CXTPDockingPaneAutoHidePanel*)map[pLayout->m_wndPanels[i]];

	// Map Holders
	pos = pLayout->m_lstPanes.GetHeadPosition();
	while(pos)
	{
		XTP_DOCKINGPANE_INFO& info = pLayout->m_lstPanes.GetNext(pos);
		XTP_DOCKINGPANE_INFO infoCopy;
		ASSERT(map.Lookup((CXTPDockingPaneBase*)info.pPane, (CXTPDockingPaneBase*&)infoCopy.pPane));

		infoCopy.pPane = (CXTPDockingPane*)map[(CXTPDockingPaneBase*)info.pPane];

		if (info.pDockingHolder) infoCopy.pDockingHolder = (CXTPDockingPaneBase*)map[info.pDockingHolder];
		if (info.pLastHolder) infoCopy.pLastHolder = (CXTPDockingPaneBase*)map[info.pLastHolder];
		if (info.pFloatingHolder) infoCopy.pFloatingHolder = (CXTPDockingPaneBase*)map[info.pFloatingHolder];

		m_lstPanes.AddTail(infoCopy);
	}
}


void CXTPDockingPaneLayout::_FreeEmptyPanes()
{

	WORD w;
	CMapPtrToWord map;

	POSITION pos = m_lstPanes.GetHeadPosition();
	while (pos)
	{
		XTP_DOCKINGPANE_INFO& info = m_lstPanes.GetNext(pos);
		map.SetAt(info.pDockingHolder, TRUE);
		map.SetAt(info.pFloatingHolder, TRUE);
		map.SetAt(info.pPane->m_pParentContainer, TRUE);
	}

	pos = m_lstStack.GetHeadPosition();
	while (pos)
	{
		CXTPDockingPaneBase* pPane = m_lstStack.GetNext(pos);
		if (pPane->GetType() == xtpPaneTypeTabbedContainer && !map.Lookup(pPane, w))
		{
			pPane->m_pParentContainer->RemovePane(pPane);
		}
	}
	POSITION posRemove = pos = m_lstStack.GetHeadPosition();
	while (pos)
	{
		CXTPDockingPaneBase* pPane = m_lstStack.GetNext(pos);
		if (pPane->GetType() == xtpPaneTypeTabbedContainer && !map.Lookup(pPane, w))
		{
			m_lstStack.RemoveAt(posRemove);
			delete pPane;
		}
		else if (pPane->GetType() == xtpPaneTypeSplitterContainer
			&& ((CXTPDockingPaneSplitterContainer*)pPane)->m_lstPanes.GetCount() == 0)
		{
			m_lstStack.RemoveAt(posRemove);
			delete pPane;
		}
		else if (pPane->GetType() == xtpPaneTypeMiniWnd
			&& ((CXTPDockingPaneMiniWnd*)pPane)->m_pTopContainer == NULL)
		{
			m_lstStack.RemoveAt(posRemove); //CHECKME
			//((CXTPDockingPaneMiniWnd*)pPane)->DestroyWindow();
			delete pPane;
		}
		posRemove = pos;
	}
}


XTP_DOCKINGPANE_INFO* CXTPDockingPaneLayout::FindPane(CXTPDockingPane* pPane)
{
	POSITION pos = m_lstPanes.GetHeadPosition();
	while (pos)
	{
		XTP_DOCKINGPANE_INFO& info = m_lstPanes.GetNext(pos);
		if (info.pPane == pPane)
			return &info;
	}
	return 0;
}

XTPDockingPaneDirection CXTPDockingPaneLayout::_GetPaneDirection(CXTPDockingPaneBase* pPane)
{
	if (pPane->m_pParentContainer)
	{
		if (pPane->m_pParentContainer->GetType() == xtpPaneTypeAutoHidePanel)
		{
			return ((CXTPDockingPaneAutoHidePanel*)pPane->m_pParentContainer)->m_direction;
		}

		if (pPane->m_pParentContainer->GetType() == xtpPaneTypeSplitterContainer)
		{
			POSITION pos = pPane->m_pParentContainer->ContainPane(m_pClient);
			if (pos)
			{
				CXTPDockingPaneSplitterContainer* pSplitter = (CXTPDockingPaneSplitterContainer*)pPane->m_pParentContainer;

				if (pSplitter->_Before(pPane, pos))
				{
					return  pSplitter->IsHoriz()? xtpPaneDockLeft: xtpPaneDockTop;

				} else
				{
					return  pSplitter->IsHoriz()? xtpPaneDockRight: xtpPaneDockBottom;
				}
			}
		}
		return _GetPaneDirection(pPane->m_pParentContainer);
	}
	return xtpPaneDockLeft;
}

void CXTPDockingPaneLayout::HidePane(CXTPDockingPaneBase* pPane)
{
	ASSERT(!m_bUserLayout);

	XTPDockingPaneDirection direction = _GetPaneDirection(pPane);

	if (m_wndPanels[direction] == 0)
	{
		m_wndPanels[direction] = (CXTPDockingPaneAutoHidePanel*)m_pManager->
			OnCreatePane(xtpPaneTypeAutoHidePanel, this);
		m_wndPanels[direction]->m_direction = direction;
	}

	if (pPane->GetType() == xtpPaneTypeDockingPane)
	{
		pPane->m_pParentContainer->RemovePane(pPane);

		CXTPDockingPaneTabbedContainer* pContainer = (CXTPDockingPaneTabbedContainer*)m_pManager->
			OnCreatePane(xtpPaneTypeTabbedContainer, this);
		pContainer->Init((CXTPDockingPane*)pPane, m_pManager->GetSite());

		pPane = pContainer;

	}
	else if (pPane->GetType() == xtpPaneTypeTabbedContainer)
	{
		pPane = ((CXTPDockingPaneBase*)pPane)->Clone(this, 0, xtpPaneNoHideable);
	}

	m_wndPanels[direction]->_InsertPane(pPane);
}

void CXTPDockingPaneLayout::OnSizeParent(CFrameWnd* pParent, CRect rect, LPVOID lParam)
{
	for (int i = 0; i < 4; i++)
	{
		SAFE_CALLPTR(m_wndPanels[i], OnSizeParentEx(pParent, rect, lParam));
	}

	m_pClient->OnSizeParent(pParent, rect, lParam);
	m_pTopContainer->OnSizeParent(pParent, rect, lParam);
}


void CXTPDockingPaneLayout::DestroyPane(CXTPDockingPane* pPane)
{
	if (pPane == NULL) return;

	SAFE_CALLPTR(pPane->m_pParentContainer, RemovePane(pPane));

	POSITION pos = m_lstPanes.GetHeadPosition();
	while (pos)
	{
		if (m_lstPanes.GetAt(pos) == pPane)
		{
			m_lstPanes.RemoveAt(pos);
			break;
		}
		m_lstPanes.GetNext(pos);
	}

	pos = m_lstStack.GetHeadPosition();
	while (pos)
	{
		if (m_lstStack.GetAt(pos) == pPane)
		{
			m_lstStack.RemoveAt(pos);
			break;
		}
		m_lstStack.GetNext(pos);
	}
	((CXTPDockingPane*)pPane)->InternalRelease();
	_FreeEmptyPanes();

}
