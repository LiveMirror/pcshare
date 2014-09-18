// XTPDockingPaneBase.cpp : implementation of the CXTPDockingPaneBase class.
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

#include "XTPDockingPaneBase.h"
#include "XTPDockingPaneManager.h"
#include "XTPDockingPaneLayout.h"
#include "XTPDockingPanePaintManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//////////////////////////////////////////////////////////////////////////
// CXTPDockingPaneCaptionButtons

BOOL CXTPDockingPaneCaptionButtons::CheckForMouseOver(CPoint pt)
{
	BOOL bResult = FALSE;
	for (int i = 0; i < GetSize(); i++)
	{
		if (GetAt(i)->CheckForMouseOver(pt))
			bResult = TRUE;
	}
	return bResult;
}


//////////////////////////////////////////////////////////////////////////
// CXTPDockingPaneBase


CXTPDockingPaneBase::CXTPDockingPaneBase(XTPDockingPaneType type, CXTPDockingPaneLayout* pLayout)
	: m_type(type), m_pLayout(pLayout)
{
	m_pParentContainer = 0;
	m_pParentFrame = GetDockingPaneManager()->GetSite();
	m_szDocking = 0;
	m_rcWindow.SetRectEmpty();

	m_nIndex = 0;
	m_nLength = 0;

	pLayout->Push(this);

}

CXTPDockingPaneBase::~CXTPDockingPaneBase()
{
	for (int i = 0; i < m_arrCaptionButtons.GetSize(); i++)
		delete m_arrCaptionButtons[i];
}

CXTPDockingPaneCaptionButton* CXTPDockingPaneBase::FindCaptionButton(int nID)
{
	for (int i = 0; i < m_arrCaptionButtons.GetSize(); i++)
	{
		if (m_arrCaptionButtons[i]->GetID() == nID)
			return m_arrCaptionButtons[i];
	}

	ASSERT(FALSE);
	return NULL;
}

void CXTPDockingPaneBase::OnSizeParent(CFrameWnd* pParent, CRect rect, LPVOID lParam)
{
	UNREFERENCED_PARAMETER(pParent);
	AFX_SIZEPARENTPARAMS* lpLayout = (AFX_SIZEPARENTPARAMS*)lParam;

	lpLayout->rect = m_rcWindow = rect;
}

void CXTPDockingPaneBase::RedrawPane()
{
	if (GetSafeHwnd())
		::InvalidateRect(GetSafeHwnd(), 0, FALSE);
}

void CXTPDockingPaneBase::OnIdleUpdate()
{

}

void CXTPDockingPaneBase::GetMinMaxInfo(LPMINMAXINFO pMinMaxInfo)
{
	ZeroMemory(pMinMaxInfo, sizeof(MINMAXINFO));
	pMinMaxInfo->ptMaxTrackSize = CPoint(32000, 32000);
}

POSITION CXTPDockingPaneBase::ContainPane(CXTPDockingPaneBase* pPane)
{
	return pPane == this? (POSITION)TRUE: NULL;
}

CRect CXTPDockingPaneBase::GetWindowRect()
{
	ASSERT(m_pParentFrame);

	CRect rc(m_rcWindow);
	m_pParentFrame->ClientToScreen(&rc);
	return rc;
}

void CXTPDockingPaneBase::OnChildConainerChanged(CXTPDockingPaneBase* pContainer)
{
	UNREFERENCED_PARAMETER(pContainer);
}

void CXTPDockingPaneBase::OnParentConainerChanged(CXTPDockingPaneBase* pContainer)
{
	UNREFERENCED_PARAMETER(pContainer);
}


void CXTPDockingPaneBase::RemovePane(CXTPDockingPaneBase* pPane)
{
	UNREFERENCED_PARAMETER(pPane);
}

void CXTPDockingPaneBase::FindPane(XTPDockingPaneType type, CXTPDockingPaneBaseList* pList)
{
	if (GetType() == type)
	{
		pList->AddTail(this);
	}
}

BOOL CXTPDockingPaneBase::IsEmpty()
{
	return GetDockingPaneManager()->m_bHideClient && GetType() == xtpPaneTypeClient;
}

CXTPDockingPaneBase* CXTPDockingPaneBase::Clone(CXTPDockingPaneLayout* pLayout, CXTPPaneToPaneMap* pMap, DWORD /*dwIgnoredOptions*/)
{
	ASSERT(pMap);
	CXTPDockingPaneBase* pClone = new CXTPDockingPaneBase(m_type, pLayout);
	pMap->SetAt(this, pClone);
	return pClone;
}


CXTPDockingPaneManager* CXTPDockingPaneBase::GetDockingPaneManager()
{
	ASSERT(m_pLayout);
	ASSERT(m_pLayout->m_pManager);
	return m_pLayout->m_pManager;
}

CXTPDockingPanePaintManager* CXTPDockingPaneBase::GetPaintManager()
{
	ASSERT(m_pLayout);
	ASSERT(m_pLayout->m_pManager);
	ASSERT(m_pLayout->m_pManager->m_pPaintManager);
	return m_pLayout->m_pManager->m_pPaintManager;
}
