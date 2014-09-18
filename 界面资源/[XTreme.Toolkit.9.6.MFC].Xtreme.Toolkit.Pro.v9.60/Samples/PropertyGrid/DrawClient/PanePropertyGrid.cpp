// PanePropertyGrid.cpp : implementation file
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
#include "drawcli.h"
#include "PanePropertyGrid.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#ifdef _XTP_INCLUDE_DOCKINGPANE

/////////////////////////////////////////////////////////////////////////////
// CPanePropertyGrid

CPanePropertyGrid::CPanePropertyGrid()
{
}

CPanePropertyGrid::~CPanePropertyGrid()
{
}


BEGIN_MESSAGE_MAP(CPanePropertyGrid, CWnd)
	//{{AFX_MSG_MAP(CPanePropertyGrid)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_SYSCOLORCHANGE()
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_PANEPROPERIES_CATEGORIZED, OnPaneproperiesCategorized)
	ON_UPDATE_COMMAND_UI(ID_PANEPROPERIES_CATEGORIZED, OnUpdatePaneproperiesCategorized)
	ON_COMMAND(ID_PANEPROPERIES_ALPHABETIC, OnPaneproperiesAlphabetic)
	ON_UPDATE_COMMAND_UI(ID_PANEPROPERIES_ALPHABETIC, OnUpdatePaneproperiesAlphabetic)


END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CPanePropertyGrid message handlers

int CPanePropertyGrid::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_wndPropertyGrid.Create(CRect(0, 0, 0, 0), this, 1000);

	m_wndPropertyGrid.SetOwner(GetTopLevelFrame());

#ifdef _XTP_INCLUDE_COMMANDBARS
	COLORREF clrMask = XTPImageManager()->SetMaskColor(RGB(0, 255, 0));
	VERIFY(m_wndToolBar.CreateToolBar(WS_TABSTOP|WS_VISIBLE|WS_CHILD|CBRS_TOOLTIPS, this));
	VERIFY(m_wndToolBar.LoadToolBar(IDR_PANE_PROPERTIES));
	XTPImageManager()->SetMaskColor(clrMask);
#else
	m_wndPropertyGrid.ShowToolBar();
#endif

	// Enable Office 2003 Theme
	m_wndPropertyGrid.SetTheme(xtpGridThemeOffice2003);

	return 0;
}

void CPanePropertyGrid::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	int nTop = 0;

#ifdef _XTP_INCLUDE_COMMANDBARS

	if (m_wndToolBar.GetSafeHwnd())
	{
		CSize sz = m_wndToolBar.CalcDockingLayout(cx, /*LM_HIDEWRAP|*/ LM_HORZDOCK|LM_HORZ | LM_COMMIT);

		m_wndToolBar.MoveWindow(0, nTop, cx, sz.cy);
		m_wndToolBar.Invalidate(FALSE);
		nTop += sz.cy;
	}
#endif
	if (m_wndPropertyGrid.GetSafeHwnd())
	{
		m_wndPropertyGrid.MoveWindow(0, nTop, cx, cy - nTop);
		m_wndPropertyGrid.Invalidate(FALSE);
	}
}


void CPanePropertyGrid::OnPaneproperiesCategorized()
{
	m_wndPropertyGrid.SetPropertySort(xtpGridSortCategorized);

}

void CPanePropertyGrid::OnUpdatePaneproperiesCategorized(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_wndPropertyGrid.GetPropertySort() == xtpGridSortCategorized);

}

void CPanePropertyGrid::OnPaneproperiesAlphabetic()
{
	m_wndPropertyGrid.SetPropertySort(xtpGridSortAlphabetical);

}

void CPanePropertyGrid::OnUpdatePaneproperiesAlphabetic(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_wndPropertyGrid.GetPropertySort() == xtpGridSortAlphabetical);

}

void CPanePropertyGrid::OnSysColorChange()
{
	CWnd::OnSysColorChange();

#ifdef _XTP_INCLUDE_COMMANDBARS
	XTPPaintManager()->RefreshMetrics();
#endif
}

#endif
