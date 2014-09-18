// PaneView.cpp : implementation file
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
#include "DockingContainers.h"
#include "PaneView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPaneView

CPaneView::CPaneView()
{
	m_pView = 0;

	m_nToolBarID = 0;
}

CPaneView::~CPaneView()
{
}


BEGIN_MESSAGE_MAP(CPaneView, CWnd)
	//{{AFX_MSG_MAP(CPaneView)
	ON_WM_CREATE()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CPaneView message handlers

int CPaneView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_pView = OnCreateView();

#ifdef _XTP_INCLUDE_COMMANDBARS
	if (m_nToolBarID != 0)
	{
		VERIFY(m_wndToolBar.CreateToolBar(WS_VISIBLE|WS_CHILD|CBRS_TOOLTIPS, this));
		VERIFY(m_wndToolBar.LoadToolBar(m_nToolBarID));
		m_wndToolBar.SetButtonSize(CSize(23, 23));
	}
#endif

	return 0;
}


CWnd* CPaneView::OnCreateView()
{
	if (!m_wndView.Create(WS_CHILD|WS_VISIBLE|LVS_ICON, CXTPEmptyRect(), this, 0))
		return NULL;

	return &m_wndView;
}

void CPaneView::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	CSize sz(0);

#ifdef _XTP_INCLUDE_COMMANDBARS
	if (m_wndToolBar.GetSafeHwnd())
	{

		sz = m_wndToolBar.CalcDockingLayout(cx, /*LM_HIDEWRAP|*/ LM_HORZDOCK|LM_HORZ | LM_COMMIT);

		m_wndToolBar.MoveWindow(0, 0, cx, sz.cy);
		m_wndToolBar.Invalidate(FALSE);
	}
#endif

	if (m_pView && m_pView->GetSafeHwnd())
	{
		m_pView->MoveWindow(0, sz.cy, cx, cy - sz.cy);
	}

}


CWnd* CPaneView::CreatePane(CWnd* pParentWnd)
{
	if (GetSafeHwnd() == 0)
	{
		VERIFY(Create(_T("STATIC"), NULL, WS_CHILD|WS_CLIPCHILDREN|WS_CLIPSIBLINGS, CXTPEmptyRect(), pParentWnd, 0));
	}
	return this;
}
