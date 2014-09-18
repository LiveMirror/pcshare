// XTPDockingPaneManager.cpp : implementation of the CXTPDockingPaneManager class.
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
#include "XTPColorManager.h"
#include "XTPDrawHelpers.h"
#include "XTPImageManager.h"
#include "XTPTabManager.h"

#include "XTPDockingPaneManager.h"
#include "XTPDockingPaneLayout.h"
#include "XTPDockingPaneBase.h"
#include "XTPDockingPane.h"
#include "XTPDockingPaneSplitterContainer.h"
#include "XTPDockingPaneTabbedContainer.h"
#include "XTPDockingPaneMiniWnd.h"
#include "XTPDockingPanePaintManager.h"
#include "XTPDockingPaneLayout.h"
#include "XTPDockingPaneAutoHidePanel.h"
#include "XTPDockingPaneContext.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



CXTPDockingPaneManager::CXTPDockingPaneManager()
{
	m_pSite = 0;
	m_pLayout = 0;

	m_nSplitterGap = 22;

	m_dwDefaultPaneOptions = 0;

	m_bAttachingPane = FALSE;

	m_bHideClient = FALSE;
	m_bUseSplitterTracker = TRUE;

	m_pPaintManager = NULL;
	SetTheme(xtpPaneThemeDefault);

	m_pImageManager = new CXTPImageManager();

	m_bLockSplitters = FALSE;

	m_bThemedFloatingFrames = FALSE;

	m_pDockingContext = new CXTPDockingPaneContext();
	m_pDockingContext->m_pManager = this;

	m_bAlphaDockingContext = FALSE;
	m_bShowDockingContextStickers = FALSE;

	m_bCloseGroupOnButtonClick = FALSE;
	m_bHideGroupOnButtonClick = TRUE;
	m_pPreviewLayout = 0;

	OleInitialize(NULL);
}

CXTPDockingPaneManager::~CXTPDockingPaneManager()
{
	if (m_pLayout) delete m_pLayout;
	m_pLayout = 0;

	if (m_pPaintManager)
	{
		m_pPaintManager->InternalRelease();
		m_pPaintManager = 0;
	}

	delete m_pDockingContext;

	ClearIconMap();

	m_pImageManager->InternalRelease();

	OleUninitialize();
}

void CXTPDockingPaneManager::HideClient(BOOL bHide)
{
	m_pSite->RecalcLayout();

	m_bHideClient = bHide;
	m_pLayout->m_pTopContainer->OnParentConainerChanged(m_pLayout->m_pTopContainer);

	RedrawPanes();
}

BEGIN_MESSAGE_MAP(CXTPDockingPaneManager, CWnd)
	ON_MESSAGE(WM_SIZEPARENT, OnSizeParent)
	ON_MESSAGE(WM_IDLEUPDATECMDUI, OnIdleUpdateCmdUI)
	ON_WM_SYSCOLORCHANGE()
	ON_WM_SETTINGCHANGE()
END_MESSAGE_MAP()

void CXTPDockingPaneManager::OnSysColorChange()
{
	XTPColorManager()->RefreshColors();


	_Redraw();
}

void CXTPDockingPaneManager::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
{
	UNREFERENCED_PARAMETER(uFlags);
	UNREFERENCED_PARAMETER(lpszSection);
	m_pPaintManager->RefreshMetrics();
	_Redraw();
}


void CXTPDockingPaneManager::UpdatePanes()
{
	POSITION pos = GetPaneList().GetHeadPosition();
	while (pos)
	{
		CXTPDockingPane* pPane = GetPaneList().GetNext(pos);
		pPane->OnIdleUpdate();
	}
}

LRESULT CXTPDockingPaneManager::OnIdleUpdateCmdUI(WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);
	UpdatePanes();
	return 0;
}

void CXTPDockingPaneManager::SyncPanesState()
{
	POSITION pos = GetPaneList().GetHeadPosition();
	while (pos)
	{
		XTP_DOCKINGPANE_INFO& info = GetPaneList().GetNext(pos);
		CXTPDockingPaneBase* pContainer = info.pPane->m_pParentContainer;

		if (info.pPane->GetParentFrame() && pContainer)
		{
			if (pContainer->m_pParentContainer == 0 || pContainer->m_pParentContainer->GetType() != xtpPaneTypeAutoHidePanel)
			{

				if (DYNAMIC_DOWNCAST(CMiniFrameWnd, info.pPane->GetParentFrame()))
				{
					info.pLastHolder = info.pFloatingHolder = pContainer;
				}
				else
				{
					info.pLastHolder = info.pDockingHolder= pContainer;
				}
			}
		}
	}
}

LRESULT CXTPDockingPaneManager::OnSizeParent(WPARAM, LPARAM lParam)
{
	if (GetTopPane() != NULL)
	{
		AFX_SIZEPARENTPARAMS* lpLayout = (AFX_SIZEPARENTPARAMS*)lParam;

		m_pLayout->OnSizeParent(m_pSite, lpLayout->rect, lpLayout);

		SyncPanesState();
	}
	return 0;
}

CXTPDockingPaneBase* CXTPDockingPaneManager::_Clone(CXTPDockingPaneBase* pPane)
{
	return (CXTPDockingPaneBase*)pPane->Clone(m_pLayout, 0);
}

BOOL CXTPDockingPaneManager::_OnAction(XTPDockingPaneAction action, CXTPDockingPane* pPane, CXTPDockingPaneBase* pDockContainer /*= NULL*/, XTPDockingPaneDirection dockDirection /*= xtpPaneDockLeft*/)
{
	XTP_DOCKINGPANE_ACTION dpAction(action);
	dpAction.pPane = pPane;
	dpAction.pDockContainer = pDockContainer;
	dpAction.dockDirection = dockDirection;

	GetSite()->SendMessage(XTPWM_DOCKINGPANE_NOTIFY, XTP_DPN_ACTION, (LPARAM)&dpAction);

	return dpAction.bCancel;
}

void CXTPDockingPaneManager::LockSplitters(BOOL bLock /*= TRUE*/)
{
	m_bLockSplitters = bLock;

	POSITION pos = GetPaneList().GetHeadPosition();
	while (pos)
	{
		CXTPDockingPane* pPane = GetPaneList().GetNext(pos);
		if (pPane->GetContainer())
			SAFE_CALLPTR(pPane->GetContainer()->GetContainer(), OnChildConainerChanged(pPane));
	}
}


void CXTPDockingPaneManager::FloatPane(CXTPDockingPaneBase* pPane, CRect rc)
{
	ASSERT(pPane->GetType() == xtpPaneTypeDockingPane || pPane->GetType() == xtpPaneTypeTabbedContainer);

	if (pPane->GetType() == xtpPaneTypeDockingPane)
	{
		_RemovePane(pPane);
	}
	else
	{
		pPane = pPane->Clone(m_pLayout, 0);
	}

	CXTPDockingPaneMiniWnd* pMiniFrame = (CXTPDockingPaneMiniWnd*)OnCreatePane(xtpPaneTypeMiniWnd, m_pLayout);
	pMiniFrame->Init(pPane, rc);
}

CXTPDockingPane* CXTPDockingPaneManager::CreatePane(UINT nID, CRect rc, XTPDockingPaneDirection direction, CXTPDockingPaneBase* pNeighbour)
{
	ASSERT(GetTopPane());
	CXTPDockingPane* pPane = (CXTPDockingPane*)OnCreatePane(xtpPaneTypeDockingPane, m_pLayout);
	ASSERT(pPane);

	pPane->SetID(nID);
	pPane->SetWindowRect(rc);


	XTP_DOCKINGPANE_INFO pane(pPane);
	GetPaneList().AddTail(pane);

	_InsertPane(pPane, direction, pNeighbour);

	SyncPanesState();

	return pPane;
}


CXTPDockingPaneBase* CXTPDockingPaneManager::OnCreatePane(XTPDockingPaneType type, CXTPDockingPaneLayout* pLayout)
{
	CXTPDockingPaneBase* pPane = NULL;
	switch (type)
	{
		case xtpPaneTypeDockingPane: pPane = new CXTPDockingPane(pLayout); break;
		case xtpPaneTypeSplitterContainer:  pPane = new CXTPDockingPaneSplitterContainer(pLayout); break;
		case xtpPaneTypeTabbedContainer: pPane = new CXTPDockingPaneTabbedContainer(pLayout); break;
		case xtpPaneTypeClient: pPane = new CXTPDockingPaneBase(xtpPaneTypeClient, pLayout); break;
		case xtpPaneTypeMiniWnd: pPane = new CXTPDockingPaneMiniWnd(pLayout); break;
		case xtpPaneTypeAutoHidePanel: pPane = new CXTPDockingPaneAutoHidePanel(pLayout); break;
	}
	ASSERT(pPane);
	return pPane;
}


void CXTPDockingPaneManager::DockPane(CXTPDockingPaneBase* pPane, XTPDockingPaneDirection direction, CXTPDockingPaneBase* pNeighbour)
{
	if (pPane->GetType() == xtpPaneTypeDockingPane)
	{
		_RemovePane(pPane);
	}
	else
	{
		pPane = _Clone((CXTPDockingPaneBase*)pPane);
	}
	_InsertPane(pPane, direction, pNeighbour);
}

void CXTPDockingPaneManager::_RemovePane(CXTPDockingPaneBase* pPane)
{
	ASSERT(pPane->m_pParentContainer);
	pPane->m_pParentContainer->RemovePane(pPane);
}

void CXTPDockingPaneManager::AttachPane(CXTPDockingPaneBase* pPane, CXTPDockingPaneBase* pNeighbour)
{
	_AttachPane(pPane, pNeighbour);
	SyncPanesState();
}

void CXTPDockingPaneManager::_AttachPane(CXTPDockingPaneBase* pPane, CXTPDockingPaneBase* pNeighbour )
{
	ASSERT(pNeighbour);

	if (pNeighbour->GetType() == xtpPaneTypeDockingPane)
	{
		pNeighbour = pNeighbour->m_pParentContainer;
		ASSERT(pNeighbour);
	}
	ASSERT(pNeighbour->GetType() == xtpPaneTypeTabbedContainer);

	if (pPane->GetType() == xtpPaneTypeDockingPane)
	{
		_RemovePane(pPane);
		((CXTPDockingPaneTabbedContainer*)pNeighbour)->_InsertPane((CXTPDockingPane*)pPane);
	} else
	{
		ASSERT(pPane->GetType() == xtpPaneTypeSplitterContainer || pPane->GetType() == xtpPaneTypeTabbedContainer);

		CXTPDockingPaneBaseList lst;
		((CXTPDockingPaneBase*)pPane)->FindPane(xtpPaneTypeDockingPane, &lst);

		ASSERT(lst.GetCount() > 0);
		POSITION pos = lst.GetHeadPosition();
		while (pos)
		{
			CXTPDockingPaneBase* pPane = lst.GetNext(pos);
			ASSERT(pPane->GetType() == xtpPaneTypeDockingPane);

			_RemovePane(pPane);
			((CXTPDockingPaneTabbedContainer*)pNeighbour)->_InsertPane((CXTPDockingPane*)pPane);
		}

	}
}

CRect CXTPDockingPaneManager::_CalculateResultDockingRect(CXTPDockingPaneBase* pPane, XTPDockingPaneDirection direction, CXTPDockingPaneBase* pNeighbour)
{
	if (pNeighbour == NULL)
	{
		pNeighbour = GetTopPane();
	}

	ASSERT(pNeighbour);

	BOOL bHoriz = (direction == xtpPaneDockLeft || direction == xtpPaneDockRight);

	if (pNeighbour == GetTopPane())
	{
		CXTPDockingPaneSplitterContainer* pSplitter = (CXTPDockingPaneSplitterContainer*)pNeighbour;

		return CXTPDockingPaneSplitterContainer::_CalculateResultDockingRectChild(pSplitter, pPane, direction);
	}
	else
	{
		if (pNeighbour->m_pParentContainer->GetType() == xtpPaneTypeTabbedContainer)
		{
			pNeighbour = pNeighbour->m_pParentContainer;
			ASSERT(pNeighbour->m_pParentContainer != NULL);
		}

		ASSERT(pNeighbour->m_pParentContainer);
		ASSERT(pNeighbour->m_pParentContainer->GetType() == xtpPaneTypeSplitterContainer);

		CXTPDockingPaneSplitterContainer* pSplitter = (CXTPDockingPaneSplitterContainer*)pNeighbour->m_pParentContainer;

		if (pSplitter->m_bHoriz == bHoriz)
			return pSplitter->_CalculateResultDockingRectSelf(pPane, direction, pNeighbour);

		return CXTPDockingPaneSplitterContainer::_CalculateResultDockingRectChild(pNeighbour, pPane, direction);
	}
}

void CXTPDockingPaneManager::_InsertPane(CXTPDockingPaneBase* pPane, XTPDockingPaneDirection direction, CXTPDockingPaneBase* pNeighbour)
{
	ASSERT(pPane->m_pParentContainer == NULL);

	if (pNeighbour == NULL)
	{
		pNeighbour = GetTopPane();
	}

	ASSERT(pNeighbour);

	CFrameWnd* pParentFrame = pNeighbour->m_pParentFrame;
	ASSERT(pParentFrame != NULL);

	BOOL bHoriz = (direction == xtpPaneDockLeft || direction == xtpPaneDockRight);
	BOOL bAfter = (direction == xtpPaneDockRight || direction == xtpPaneDockBottom);

	if (pPane->GetType() == xtpPaneTypeDockingPane)
	{
		CXTPDockingPaneTabbedContainer* pContainer = (CXTPDockingPaneTabbedContainer*)OnCreatePane(xtpPaneTypeTabbedContainer, m_pLayout);
		pContainer->Init((CXTPDockingPane*)pPane, pParentFrame);

		pPane = pContainer;
	}

	if (pNeighbour == GetTopPane())
	{
		CXTPDockingPaneSplitterContainer* pSplitter = (CXTPDockingPaneSplitterContainer*)pNeighbour;

		if (pSplitter->m_bHoriz == bHoriz)
			pSplitter->_InsertPane(pPane, NULL, bAfter);
		else
		{
			CXTPDockingPaneSplitterContainer* pChildSplitter = (CXTPDockingPaneSplitterContainer*)OnCreatePane(xtpPaneTypeSplitterContainer, m_pLayout);
			pChildSplitter->Init(pSplitter, bHoriz, pParentFrame);

			pChildSplitter->_InsertPane(pPane, NULL, bAfter);
			m_pLayout->m_pTopContainer = pChildSplitter;
		}
	}
	else
	{
		if (pNeighbour->m_pParentContainer->GetType() == xtpPaneTypeTabbedContainer)
		{
			pNeighbour = pNeighbour->m_pParentContainer;
			ASSERT(pNeighbour->m_pParentContainer != NULL);
		}

		ASSERT(pNeighbour->m_pParentContainer);
		ASSERT(pNeighbour->m_pParentContainer->GetType() == xtpPaneTypeSplitterContainer);

		CXTPDockingPaneSplitterContainer* pSplitter = (CXTPDockingPaneSplitterContainer*)pNeighbour->m_pParentContainer;

		if (pSplitter->m_bHoriz == bHoriz)
		{
			pSplitter->_InsertPane(pPane, pNeighbour, bAfter);
		}
		else
		{
			CXTPDockingPaneSplitterContainer* pChildSplitter = (CXTPDockingPaneSplitterContainer*)OnCreatePane(xtpPaneTypeSplitterContainer, m_pLayout);
			pChildSplitter->Init(pNeighbour, bHoriz, pParentFrame);

			pChildSplitter->_InsertPane(pPane, pNeighbour, bAfter);
			pSplitter->_Replace(pNeighbour, pChildSplitter);
		}


	}
	DELAYRECALCLAYOUT(pParentFrame);
}

CXTPDockingPaneBase* CXTPDockingPaneManager::_GetHolder(CXTPDockingPaneBase* pPane, BOOL bFloating)
{
	POSITION pos = GetPaneList().GetHeadPosition();
	while (pos)
	{
		XTP_DOCKINGPANE_INFO& info = GetPaneList().GetNext(pos);
		if (info.pPane == pPane)
			return bFloating? info.pFloatingHolder: info.pDockingHolder;
	}
	return NULL;
}

void CXTPDockingPaneManager::ToggleDocking(CXTPDockingPaneBase* pPane)
{
	ASSERT(pPane);
	BOOL bFloating = DYNAMIC_DOWNCAST(CMiniFrameWnd, pPane->m_pParentFrame) != NULL;

	if (pPane->GetType() == xtpPaneTypeSplitterContainer || pPane->GetType() == xtpPaneTypeTabbedContainer)
	{

		CXTPDockingPaneBaseList lst;
		((CXTPDockingPaneBase*)pPane)->FindPane(xtpPaneTypeDockingPane, &lst);

		int nIDSelected = 0, nIDFocused = 0;

		POSITION pos = lst.GetHeadPosition();
		while (pos)
		{
			CXTPDockingPane* pPane = (CXTPDockingPane*)lst.GetNext(pos);
			if (pPane->IsSelected()) nIDSelected = pPane->GetID();
			if (pPane->IsFocus()) nIDFocused = pPane->GetID();
		}

		pos = lst.GetHeadPosition();
		CXTPDockingPaneBase* pLastHolder = NULL;
		while (pos)
		{
			CXTPDockingPane* pPane = (CXTPDockingPane*)lst.GetNext(pos);

			CXTPDockingPaneTabbedContainer* pHolder = (CXTPDockingPaneTabbedContainer*)_GetHolder(pPane, !bFloating);
			if (!_ToggleDocking(pPane, pHolder? pHolder: pLastHolder))
			{
				if (pPane->GetID() == nIDSelected)  nIDSelected = 0;
				if (pPane->GetID() == nIDFocused)  nIDFocused = 0;
			}

			pLastHolder = pHolder;
		}

		if (nIDFocused > 0) ShowPane(nIDFocused);else if (nIDSelected > 0) ShowPane(nIDSelected);
	}
	else if (pPane->GetType() == xtpPaneTypeDockingPane)
	{
		CXTPDockingPaneTabbedContainer* pHolder = (CXTPDockingPaneTabbedContainer*)_GetHolder(pPane, !bFloating);
		_ToggleDocking((CXTPDockingPane*)pPane, pHolder);
	}
	else
	{
		ASSERT(FALSE);
	}
}



BOOL CXTPDockingPaneManager::_ToggleDocking(CXTPDockingPane* pPane, CXTPDockingPaneBase* pHolder)
{
	BOOL bFloating = DYNAMIC_DOWNCAST(CMiniFrameWnd, pPane->m_pParentFrame) != NULL;

	if (!bFloating && (pPane->GetOptions() & xtpPaneNoFloatable))
		return FALSE;

	if (_OnAction(bFloating? xtpPaneActionDocking: xtpPaneActionFloating, pPane, pHolder))
		return FALSE;

	if (pHolder)
	{
		m_bAttachingPane = TRUE;
		AttachPane(pPane, pHolder);
		m_bAttachingPane = FALSE;
	}
	else
	{
		FloatPane(pPane, pPane->GetContainer()->GetWindowRect());
	}

	_OnAction(bFloating? xtpPaneActionDocked: xtpPaneActionFloated, pPane);

	return TRUE;
}


void CXTPDockingPaneManager::InstallDockingPanes(CFrameWnd* pSite)
{
	if (m_pSite == 0)
	{

#ifdef _DEBUG
		ASSERT(m_pLayout == 0);
		ASSERT(m_pSite == 0);
		ASSERT(pSite != 0);

		if (DYNAMIC_DOWNCAST(CMDIFrameWnd, pSite) == 0 && ((pSite->GetStyle() & WS_CLIPCHILDREN) == 0) )
		{
			TRACE(_T("Warning: Add WS_CLIPCHILDREN and WS_CLIPSIBLINGS to frame\n"));
		}

#endif

		m_pSite = pSite;
		VERIFY(Create(_T("STATIC"), _T(""), WS_CHILD, CRect(0, 0, 0, 0), pSite, 0));

		m_pLayout = CreateLayout();
		m_pLayout->m_bUserLayout = FALSE;
	}
}

CXTPDockingPane* CXTPDockingPaneManager::FindPane(int nID)
{
	if (!m_pLayout)
		return NULL;

	POSITION pos = GetPaneList().GetHeadPosition();
	while (pos)
	{
		CXTPDockingPane* pPane = GetPaneList().GetNext(pos);
		if (pPane->GetID() == nID)
			return pPane;
	}

	return NULL;
}

BOOL CXTPDockingPaneManager::SetIcons(UINT nIDResource, const int* nIDs, int nCount, COLORREF clrMask)
{
	CImageList il;
	il.Create(16, 16, ILC_COLOR24|ILC_MASK, nCount, 1);

	CBitmap bmp;
	bmp.LoadBitmap(nIDResource);
	il.Add(&bmp, clrMask);

	ASSERT(il.GetImageCount() == nCount); // Icons must be 16x16.
	if (il.GetImageCount() != nCount)
		return FALSE;

	for (int i = 0; i < nCount; i++)
	{
		HICON hIcon = il.ExtractIcon(i);
		if (nIDs[i] > 0)
		{
			SetIcon(nIDs[i], hIcon);
		}
		DestroyIcon(hIcon);
	}

	return TRUE;
}

void CXTPDockingPaneManager::SetImageManager(CXTPImageManager* pImageManager)
{
	if (pImageManager)
	{
		m_pImageManager->InternalRelease();

		m_pImageManager = pImageManager;
	}
}

void CXTPDockingPaneManager::ShowPane(CXTPDockingPane* pPane)
{
	if (pPane == NULL || m_pLayout ==  NULL) return;

	if (pPane->m_pParentContainer == NULL)
	{
		POSITION pos = GetPaneList().GetHeadPosition();
		while (pos)
		{
			XTP_DOCKINGPANE_INFO& info = GetPaneList().GetNext(pos);
			if (info.pPane == pPane && info.pLastHolder != NULL)
			{
				((CXTPDockingPaneTabbedContainer*)info.pLastHolder)->_InsertPane(pPane);
			}
		}
	} else
	{
		((CXTPDockingPaneTabbedContainer*)pPane->m_pParentContainer)->SelectPane(pPane);
		((CXTPDockingPaneTabbedContainer*)pPane->m_pParentContainer)->Show();
	}
	pPane->SetFocus();
}

void CXTPDockingPaneManager::ClosePane(CXTPDockingPane* pPane)
{
	if (pPane == NULL) return;

	if (m_pSite->SendMessage(XTPWM_DOCKINGPANE_NOTIFY, XTP_DPN_CLOSEPANE, (LPARAM)pPane) == XTP_ACTION_NOCLOSE)
		return;

	SAFE_CALLPTR(pPane->m_pParentContainer, RemovePane(pPane));
}


void CXTPDockingPaneManager::HidePane(CXTPDockingPaneBase* pPane)
{
	if (pPane == NULL) return;

	if  (pPane->GetType() == xtpPaneTypeDockingPane && m_bHideGroupOnButtonClick)
	{
		pPane = pPane->m_pParentContainer;
	}

	if (pPane == NULL || pPane->GetContainer() == NULL)
		return;

	if (pPane->GetContainer()->GetType() == xtpPaneTypeAutoHidePanel)
	{
		((CXTPDockingPaneAutoHidePanel*)pPane->GetContainer())->CloseActiveWindow();
		return;
	}

	m_pLayout->HidePane(pPane);

	DELAYRECALCLAYOUT(m_pSite);
}

void CXTPDockingPaneManager::DestroyPane(CXTPDockingPane* pPane)
{
	if (m_pLayout && pPane)
	{
		m_pLayout->DestroyPane(pPane);
		m_pSite->RecalcLayout();
	}
}

BOOL CXTPDockingPaneManager::IsPaneClosed(CXTPDockingPane* pPane)
{
	return pPane == NULL || pPane->IsClosed();
}

BOOL CXTPDockingPaneManager::IsPaneHidden(CXTPDockingPane* pPane)
{
	return pPane != NULL && pPane->IsHidden();
}

BOOL CXTPDockingPaneManager::IsPaneSelected(CXTPDockingPane* pPane)
{
	return pPane != NULL && pPane->IsSelected();
}

void CXTPDockingPaneManager::_Redraw()
{
	if (!m_hWnd)
		return;

	POSITION pos = GetPaneList().GetHeadPosition();
	while (pos)
	{
		CXTPDockingPane* pPane = GetPaneList().GetNext(pos);
		CFrameWnd* pFrame = pPane->GetParentFrame();
		DELAYRECALCLAYOUT(pFrame);

		if (DYNAMIC_DOWNCAST(CXTPDockingPaneMiniWnd, pFrame))
		{
			((CXTPDockingPaneMiniWnd*)pFrame)->OnThemedChanged();
		}
	}
}

void CXTPDockingPaneManager::SetTheme(XTPDockingPanePaintTheme paintTheme)
{
	if (m_pPaintManager != NULL)
		m_pPaintManager->InternalRelease();

	if (paintTheme == xtpPaneThemeOffice)   m_pPaintManager = new CXTPDockingPaneOfficeTheme();
	else if (paintTheme == xtpPaneThemeGrippered) m_pPaintManager = new CXTPDockingPaneGripperedTheme();
	else if (paintTheme == xtpPaneThemeVisio) m_pPaintManager = new CXTPDockingPaneVisioTheme();
	else if (paintTheme == xtpPaneThemeOffice2003) m_pPaintManager = new CXTPDockingPaneOffice2003Theme();
	else if (paintTheme == xtpPaneThemeNativeWinXP)  m_pPaintManager = new CXTPDockingPaneNativeXPTheme();
	else if (paintTheme == xtpPaneThemeWhidbey)  m_pPaintManager = new CXTPDockingPaneWhidbeyTheme();
	else if (paintTheme == xtpPaneThemeShortcutBar2003)  m_pPaintManager = new CXTPDockingPaneShortcutBar2003Theme();	
	else if (paintTheme == xtpPaneThemeExplorer)  m_pPaintManager = new CXTPDockingPaneExplorerTheme();	
	else m_pPaintManager = new CXTPDockingPaneDefaultTheme();

	m_pPaintManager->m_themeCurrent = paintTheme;

	_Redraw();

}

void CXTPDockingPaneManager::SetCustomTheme(CXTPDockingPanePaintManager* pTheme)
{
	ASSERT(pTheme);

	if (m_pPaintManager != NULL)
		m_pPaintManager->InternalRelease();

	m_pPaintManager = pTheme;
	m_pPaintManager->m_themeCurrent = xtpPaneThemeCustom;

	_Redraw();
}


XTPDockingPanePaintTheme CXTPDockingPaneManager::GetCurrentTheme()
{
	ASSERT(m_pPaintManager);
	return m_pPaintManager->m_themeCurrent;
}

CXTPDockingPaneLayout* CXTPDockingPaneManager::CreateLayout()
{
	return new CXTPDockingPaneLayout(this);
}


void CXTPDockingPaneManager::GetLayout(CXTPDockingPaneLayout* pLayout)
{
	ASSERT(pLayout);
	pLayout->Copy(m_pLayout);
}

void CXTPDockingPaneManager::_DetachAll()
{
	POSITION pos = GetPaneList().GetHeadPosition();
	while (pos)
	{
		CXTPDockingPane* pPane = GetPaneList().GetNext(pos);
		pPane->Detach();
	}
}

void CXTPDockingPaneManager::SetLayout(CXTPDockingPaneLayout* pLayout)
{
	ASSERT(pLayout);
	_DetachAll();

	m_pLayout->Copy(pLayout);
	_Redraw();
}

void CXTPDockingPaneManager::DestroyAll()
{
	if (m_pLayout)
	{
		delete m_pLayout;
		m_pLayout = CreateLayout();
		m_pLayout->m_bUserLayout = FALSE;
	}
}
void CXTPDockingPaneManager::CloseAll()
{
	CXTPDockingPaneInfoList& paneList = GetPaneList();
	POSITION pos = paneList.GetHeadPosition();
	while (pos)
	{
		CXTPDockingPane* pPane = paneList.GetNext(pos);
		pPane->Close();
	}
}


CXTPDockingPaneBaseList& CXTPDockingPaneManager::GetPaneStack()
{
	return m_pLayout->m_lstStack;
}

CXTPDockingPaneInfoList& CXTPDockingPaneManager::GetPaneList()
{
	return m_pLayout->m_lstPanes;
}

CXTPDockingPaneBase* CXTPDockingPaneManager::GetClientPane()
{
	return m_pLayout? m_pLayout->m_pClient: NULL;
}

CXTPDockingPaneBase* CXTPDockingPaneManager::GetTopPane()
{
	return m_pLayout? (CXTPDockingPaneBase*)m_pLayout->m_pTopContainer: NULL;
}

void CXTPDockingPaneManager::SetIcon(UINT nID, CXTPImageManagerIconHandle hIcon)
{
	m_pImageManager->SetIcon(hIcon, nID, 0, xtpImageNormal);
}

CXTPImageManagerIcon* CXTPDockingPaneManager::GetIcon(UINT nID, int nWidth)
{
	return m_pImageManager->GetImage(nID, nWidth);
}

void CXTPDockingPaneManager::ClearIconMap()
{
	m_pImageManager->RemoveAll();
}

void CXTPDockingPaneManager::SetThemedFloatingFrames(BOOL bThemedFloatingFrames)
{
	m_bThemedFloatingFrames = bThemedFloatingFrames;

	if (!m_pLayout)
		return;

	POSITION pos = GetPaneList().GetHeadPosition();
	while (pos)
	{
		CXTPDockingPane* pPane = GetPaneList().GetNext(pos);
		CFrameWnd* pFrame = pPane->GetParentFrame();

		if (DYNAMIC_DOWNCAST(CXTPDockingPaneMiniWnd, pFrame))
		{
			((CXTPDockingPaneMiniWnd*)pFrame)->OnThemedChanged();
		}
	}

}



void CXTPDockingPaneManager::SetAnimationDelay(double dAnimationDelay /*= -1*/, int nAnimationDuration /*= 128*/, int nAnimationInterval /*= 16*/)
{
	CXTPDockingPaneAutoHideWnd::m_dAnimationDelay = dAnimationDelay;
	CXTPDockingPaneAutoHideWnd::m_nAnimationDuration = nAnimationDuration;
	CXTPDockingPaneAutoHideWnd::m_nAnimationInterval = nAnimationInterval;
}
double CXTPDockingPaneManager::GetAnimationDelay(int* pAnimationDuration /*= NULL*/, int* pAnimationInterval /*= NULL*/)
{
	if (pAnimationDuration) *pAnimationDuration = CXTPDockingPaneAutoHideWnd::m_nAnimationDuration;
	if (pAnimationInterval) *pAnimationInterval = CXTPDockingPaneAutoHideWnd::m_nAnimationInterval;

	return CXTPDockingPaneAutoHideWnd::m_dAnimationDelay;
}

BOOL CXTPDockingPaneManager::IsLayoutRTL()
{
	return (GetSite()->GetExStyle() & WS_EX_LAYOUTRTL) != 0;
}

XTPDockingPaneDirection CXTPDockingPaneManager::GetRTLDirection(XTPDockingPaneDirection direction)
{
	if (IsLayoutRTL())
	{
		if (direction == xtpPaneDockLeft) direction = xtpPaneDockRight;
		else if (direction == xtpPaneDockRight) direction = xtpPaneDockLeft;
	}
	return direction;
}

XTPDockingPaneDirection CXTPDockingPaneManager::GetPaneDirection(CXTPDockingPaneBase* pPane)
{
	ASSERT(m_pLayout);
	return m_pLayout->_GetPaneDirection(pPane);
}

void CXTPDockingPaneManager::SetDockingContext(CXTPDockingPaneContext* pDockingContext)
{
	delete m_pDockingContext;
	m_pDockingContext = pDockingContext;
	m_pDockingContext->m_pManager = this;
}



void CXTPDockingPaneManager::NormalizeSplitters()
{
	CXTPDockingPaneBaseList& lst = GetPaneStack();
	POSITION pos = lst.GetHeadPosition();

	while(pos)
	{
		CXTPDockingPaneBase* pPane = lst.GetNext(pos);

		if (pPane->GetType() == xtpPaneTypeSplitterContainer)
		{
			((CXTPDockingPaneSplitterContainer*)pPane)->NormalizeDockingSize();
		}
	}
}

void CXTPDockingPaneManager::RecalcFramesLayout()
{
	CXTPDockingPaneBaseList& lst = GetPaneStack();
	POSITION pos = lst.GetHeadPosition();

	while(pos)
	{
		CXTPDockingPaneBase* pPane = lst.GetNext(pos);

		if (pPane->GetType() == xtpPaneTypeMiniWnd)
		{
			CXTPDockingPaneMiniWnd* pMiniWnd = (CXTPDockingPaneMiniWnd*)pPane;

			if (pMiniWnd->m_hWnd && pMiniWnd->GetStyle() & WS_VISIBLE)
			{
				pMiniWnd->RecalcLayout(TRUE);
				pMiniWnd->MoveWindow(CXTPWindowRect(pMiniWnd));
			}
		}
	}
	GetSite()->RecalcLayout();
}

void CXTPDockingPaneManager::OnSetPreviewMode(BOOL bPreview)
{

// Toggle Docking Panes.
	if (bPreview)
	{
		ASSERT(m_pPreviewLayout == NULL);
		m_pPreviewLayout   = CreateLayout();
		GetLayout(m_pPreviewLayout);
		CloseAll();
	} else
	{
		ASSERT(m_pPreviewLayout != NULL);
		SetLayout(m_pPreviewLayout);
		delete m_pPreviewLayout;
		m_pPreviewLayout = 0;
	}
}