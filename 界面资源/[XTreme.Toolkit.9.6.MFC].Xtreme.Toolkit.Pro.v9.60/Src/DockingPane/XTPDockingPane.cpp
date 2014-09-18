// XTPDockingPane.cpp : implementation of the CXTPDockingPaneclass.
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
#include "XTPTabManager.h"
#include "XTPDrawHelpers.h"

#include "XTPDockingPane.h"
#include "XTPDockingPaneContext.h"
#include "XTPDockingPaneManager.h"
#include "XTPDockingPaneTabbedContainer.h"
#include "XTPDockingPaneAutoHidePanel.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CXTPDockingPane::CXTPDockingPane(CXTPDockingPaneLayout* pLayout)
	: CXTPDockingPaneBase(xtpPaneTypeDockingPane, pLayout)
{
	m_dwOptions = 0;
	m_hwndChild = 0;
	m_nID = 0;
	m_nIconID = -1;
	m_dwData = 0;

	m_ptMinTrackSize = CPoint(0, 0);
	m_ptMaxTrackSize = CPoint(32000, 32000);

	EnableAutomation();


}

void CXTPDockingPane::SetID(long nID)
{
	ASSERT(nID != 0);

	m_nID = nID;
	m_strTitle.LoadString(nID);

}

void CXTPDockingPane::SetWindowRect(CRect rc)
{
	m_rcWindow = rc;
	m_szDocking = m_rcWindow.Size();
}

CXTPDockingPane::~CXTPDockingPane()
{

}
void CXTPDockingPane::OnSizeParent(CFrameWnd* pParent, CRect rect, LPVOID lParam)
{
	AFX_SIZEPARENTPARAMS* lpLayout = (AFX_SIZEPARENTPARAMS*)lParam;
	SetParentFrame(pParent);

	if (lpLayout == 0 || lpLayout->hDWP != NULL)
	{
		m_rcWindow = rect;
		MapWindowPoints(pParent->GetSafeHwnd(), ((CXTPDockingPaneTabbedContainer*)m_pParentContainer)->GetSafeHwnd(), (LPPOINT)&rect, 2);
		if (m_hwndChild) ::MoveWindow(m_hwndChild, rect.left, rect.top, rect.Width(), rect.Height(), TRUE);

	}
}

void CXTPDockingPane::SetFocus()
{
	if (m_hwndChild)
	{
		if (!IsChild(m_hwndChild, ::GetFocus()))
			::SetFocus(m_hwndChild);
	}
}
BOOL CXTPDockingPane::IsFocus()
{
	HWND hwndFocus = ::GetFocus();
	return (m_hwndChild != 0) && (hwndFocus == m_hwndChild || IsChild(m_hwndChild, hwndFocus));

}

void CXTPDockingPane::SetParentContainer(CXTPDockingPaneBase* pContainer)
{
	m_pParentContainer = pContainer;

	if (m_hwndChild == 0)
		return;

	if (pContainer)
	{
		ASSERT(pContainer->GetType() == xtpPaneTypeTabbedContainer);
		::SetParent(m_hwndChild, ((CXTPDockingPaneTabbedContainer*)pContainer)->GetSafeHwnd());

	} else
	{
		::ShowWindow(m_hwndChild, SW_HIDE);
		::SetParent(m_hwndChild, GetDockingPaneManager()->GetSafeHwnd());

		m_pParentFrame = NULL;
	}
}

void CXTPDockingPane::OnIdleUpdate()
{
	if (m_pParentContainer)
	{
		ASSERT(m_pParentContainer->GetType() == xtpPaneTypeTabbedContainer);
		m_pParentContainer->OnIdleUpdate();
	}
}

void CXTPDockingPane::ShowWindow(BOOL bShow)
{
	if (bShow)
	{
		GetDockingPaneManager()->GetSite()->SendMessage(XTPWM_DOCKINGPANE_NOTIFY, XTP_DPN_SHOWWINDOW, (LPARAM)this);
	}
	if (m_hwndChild)
	{
		::ShowWindow(m_hwndChild, bShow? SW_SHOW: SW_HIDE);
	}
}

void CXTPDockingPane::Close()
{
	GetDockingPaneManager()->ClosePane(this);
}

void CXTPDockingPane::Hide()
{
	GetDockingPaneManager()->HidePane(this);
}

void CXTPDockingPane::Select()
{
	GetDockingPaneManager()->ShowPane(this);
}

CFrameWnd* CXTPDockingPane::GetParentFrame()
{
	return m_pParentContainer? m_pParentContainer->m_pParentFrame: 0;
}

void CXTPDockingPane::Copy(CXTPDockingPane* pClone)
{
	m_szDocking = pClone->m_szDocking;
	m_strTitle = pClone->m_strTitle;
	m_nIconID = pClone->m_nIconID;
	m_dwOptions = pClone->m_dwOptions;
	m_dwData = pClone->m_dwData;
	m_nID = pClone->m_nID;

	m_ptMinTrackSize = pClone->m_ptMinTrackSize;
	m_ptMaxTrackSize = pClone->m_ptMaxTrackSize;

	m_hwndChild = 0;
}

CXTPDockingPaneBase* CXTPDockingPane::Clone(CXTPDockingPaneLayout* pLayout, CXTPPaneToPaneMap* pMap, DWORD /*dwIgnoredOptions*/)
{
	ASSERT(pMap);

	CXTPDockingPane* pPane = (CXTPDockingPane*)GetDockingPaneManager()->OnCreatePane(GetType(), pLayout);

	pPane->Copy(this);

	pMap->SetAt(this, pPane);


	return pPane;
}


BOOL CXTPDockingPane::IsValid()
{
	return m_hwndChild != NULL;

}
void CXTPDockingPane::Attach(CWnd* pWnd)
{
	if (pWnd)
	{
		m_hwndChild = pWnd->GetSafeHwnd();
		GetDockingPaneManager()->_Redraw();
		if (m_pParentContainer)
		{
			::SetParent(m_hwndChild, ((CXTPDockingPaneTabbedContainer*)m_pParentContainer)->GetSafeHwnd());
		}
	}
}

CWnd* CXTPDockingPane::AttachView(CWnd* pParentWnd, CRuntimeClass *pViewClass, CDocument *pDocument/*=NULL*/, CCreateContext *pContext/*=NULL*/)
{
#ifdef _DEBUG
	ASSERT(pContext != NULL || pViewClass != NULL);
	ASSERT(pContext != NULL || pViewClass->IsDerivedFrom(RUNTIME_CLASS(CWnd)));
	ASSERT(pContext != NULL || AfxIsValidAddress(pViewClass, sizeof(CRuntimeClass), FALSE));
#endif

	CCreateContext contextT;
	if (pContext == NULL)
	{
		// if no context specified, generate one from the
		// currently selected client if possible.
		contextT.m_pLastView       = NULL;
		contextT.m_pCurrentFrame   = NULL;
		contextT.m_pNewDocTemplate = NULL;
		contextT.m_pCurrentDoc     = pDocument;
		contextT.m_pNewViewClass   = pViewClass;

		if (pDocument != NULL)
			contextT.m_pNewDocTemplate = pDocument->GetDocTemplate();

		pContext = &contextT;
	}

	CFrameWnd* pFrame = new CFrameWnd;

	pFrame->Create(NULL, NULL, WS_CHILD|WS_CLIPCHILDREN|WS_CLIPSIBLINGS,
		CRect(0, 0, 0, 0), pParentWnd, NULL, 0, pContext);

	pFrame->ModifyStyleEx(WS_EX_CLIENTEDGE, 0);

	Attach(pFrame);

	return pFrame->GetDescendantWindow(AFX_IDW_PANE_FIRST, TRUE);
}

void CXTPDockingPane::Detach()
{
	if (m_hwndChild)
	{
		::ShowWindow(m_hwndChild, SW_HIDE);
		::SetParent(m_hwndChild, GetDockingPaneManager()->GetSite()->GetSafeHwnd());
		m_hwndChild = 0;
	}
	GetDockingPaneManager()->_Redraw();
}

CString CXTPDockingPane::GetTitle()
{
	int nIndex = m_strTitle.Find('\n');
	if (nIndex == -1)
		return m_strTitle;
	return m_strTitle.Left(nIndex);
}

CString CXTPDockingPane::GetShortTitle()
{
	int nIndex = m_strTitle.Find('\n');
	if (nIndex == -1)
		return m_strTitle;
	return m_strTitle.Mid(nIndex + 1);
}

void CXTPDockingPane::SetTitle(LPCTSTR lpszTitle)
{
	m_strTitle = lpszTitle;

	CXTPDockingPaneBase* pBase = GetContainer();
	if (pBase )
	{
		if (IsFloating())
		{
			SAFE_CALLPTR(pBase->GetContainer(), OnChildConainerChanged(this));
		}
		else
		{
			DELAYRECALCLAYOUT(GetParentFrame());
		}
	}
}

BOOL CXTPDockingPane::IsClosed()
{
	return m_pParentContainer == NULL;
}

BOOL CXTPDockingPane::IsHidden()
{
	if (m_pParentContainer == NULL) return FALSE;
	CXTPDockingPaneBase* pContainer = m_pParentContainer->GetContainer();
	return (pContainer != NULL) && (pContainer->GetType() == xtpPaneTypeAutoHidePanel);
}

BOOL CXTPDockingPane::IsSelected()
{
	if (m_pParentContainer == NULL) return FALSE;
	ASSERT(m_pParentContainer->GetType() == xtpPaneTypeTabbedContainer);
	return ((CXTPDockingPaneTabbedContainer*)m_pParentContainer)->GetSelected() == this;
}

BOOL CXTPDockingPane::IsFloating()
{
	if (m_pParentContainer == NULL) return FALSE;
	if (IsHidden()) return FALSE;
	ASSERT(m_pParentContainer->GetType() == xtpPaneTypeTabbedContainer);

	CXTPDockingPaneTabbedContainer* pParentContainer = (CXTPDockingPaneTabbedContainer*)m_pParentContainer;

	if (pParentContainer->GetSafeHwnd() == 0) return FALSE;

	CFrameWnd* pFrame = pParentContainer->GetParentFrame();
	return pFrame && pFrame->IsKindOf(RUNTIME_CLASS(CMiniFrameWnd));
}


CXTPImageManagerIcon* CXTPDockingPane::GetIcon(int nWidth)
{
	return GetDockingPaneManager()->GetIcon(GetIconID(), nWidth);
}

COLORREF CXTPDockingPane::GetItemColor()
{
	if (m_hwndChild)
	{
		COLORREF clr = (COLORREF)::SendMessage(m_hwndChild, WM_XTP_GETTABCOLOR, 0, 0);
		if (clr != 0)
			return clr;
	}
	return xtpTabColorBlue + GetID() % 8;
}

void CXTPDockingPane::GetMinMaxInfo(LPMINMAXINFO pMinMaxInfo)
{
	ZeroMemory(pMinMaxInfo, sizeof(MINMAXINFO));
	pMinMaxInfo->ptMinTrackSize = m_ptMinTrackSize;
	pMinMaxInfo->ptMaxTrackSize = m_ptMaxTrackSize;
}



DWORD CXTPDockingPane::GetOptions()
{
	return GetDockingPaneManager()->m_dwDefaultPaneOptions | m_dwOptions;
}


