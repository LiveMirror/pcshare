// SolutionExplorerPane.cpp : implementation file
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
#include "GUI_Whidbey.h"
#include "SolutionExplorerPane.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSolutionExplorerPane

CSolutionExplorerPane::CSolutionExplorerPane()
{
}

CSolutionExplorerPane::~CSolutionExplorerPane()
{
}


BEGIN_MESSAGE_MAP(CSolutionExplorerPane, CWnd)
	//{{AFX_MSG_MAP(CSolutionExplorerPane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_COMMAND_RANGE(ID_SOLUTION_PROPERTIES, ID_SOLUTION_ADDNEWFOLDER, OnEmptyCommand)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CSolutionExplorerPane message handlers

int CSolutionExplorerPane::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	VERIFY(m_wndToolBar.CreateToolBar(WS_TABSTOP|WS_VISIBLE|WS_CHILD|CBRS_TOOLTIPS, this));
	VERIFY(m_wndToolBar.LoadToolBar(IDR_PANE_SOLUTIONEXPLORER));


	if (m_wndSolutionView.GetSafeHwnd() == 0)
	{
		if (!m_wndSolutionView.Create( WS_VISIBLE | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS | TVS_SHOWSELALWAYS,
			CRect(0,0,0,0), this, 0xFF ))
		{
			TRACE0( "Unable to create tree control.\n" );
			return 0;
		}
		m_wndSolutionView.ModifyStyleEx(0, WS_EX_STATICEDGE);

		// load the tree images bitmap and add it to the image list.
		if (!CreateImageList(m_ilSolutionView, IDB_FILETREE))
			return 0;

		// Set the image list for the tree control.
		m_wndSolutionView.SetImageList( &m_ilSolutionView, TVSIL_NORMAL );

		// add the parent item, make it bold
		HTREEITEM htiParent = m_wndSolutionView.InsertItem(_T("Solution 'XtremeDemo': 1 project(s)"));
		HTREEITEM htiChild; // child item

		htiChild = m_wndSolutionView.InsertItem (_T("XtremeDemo Files"), 1, 1, htiParent);
		m_wndSolutionView.SetItemState (htiChild, TVIS_BOLD, TVIS_BOLD);

		// add the children of the parent item
		HTREEITEM hti = m_wndSolutionView.InsertItem(_T("Source Files"), 2, 3, htiChild);
		m_wndSolutionView.InsertItem(_T("ChildFrm.cpp"), 4, 4, hti);
		m_wndSolutionView.InsertItem(_T("MainFrm.cpp"), 4, 4, hti);
		m_wndSolutionView.InsertItem(_T("StdAfx.cpp"), 4, 4, hti);
		m_wndSolutionView.InsertItem(_T("XtremeDemo.cpp"), 4, 4, hti);
		m_wndSolutionView.InsertItem(_T("XtremeDemo.rc"), 4, 4, hti);
		m_wndSolutionView.InsertItem(_T("XtremeDemoDoc.cpp"), 4, 4, hti);
		m_wndSolutionView.InsertItem(_T("XtremeDemoView.cpp"), 4, 4, hti);
		m_wndSolutionView.Expand(hti, TVE_EXPAND);

		hti = m_wndSolutionView.InsertItem(_T("Header Files"), 2, 3, htiChild);
		m_wndSolutionView.InsertItem(_T("ChildFrm.h"), 5, 5, hti);
		m_wndSolutionView.InsertItem(_T("MainFrm.h"), 5, 5, hti);
		m_wndSolutionView.InsertItem(_T("Resource.rc"), 5, 5, hti);
		m_wndSolutionView.InsertItem(_T("StdAfx.h"), 5, 5, hti);
		m_wndSolutionView.InsertItem(_T("XtremeDemo.h"), 5, 5, hti);
		m_wndSolutionView.InsertItem(_T("XtremeDemoDoc.h"), 5, 5, hti);
		m_wndSolutionView.InsertItem(_T("XtremeDemoView.h"), 5, 5, hti);

		hti = m_wndSolutionView.InsertItem(_T("Resource Files"), 2, 3, htiChild);
		m_wndSolutionView.InsertItem(_T("Toolbar.bmp"), 5, 5, hti);
		m_wndSolutionView.InsertItem(_T("XtremeDemo.ico"), 5, 5, hti);
		m_wndSolutionView.InsertItem(_T("XtremeDemo.rc2"), 5, 5, hti);
		m_wndSolutionView.InsertItem(_T("XtremeDemoDoc.ico"), 5, 5, hti);

		m_wndSolutionView.InsertItem(_T("ReadMe.txt"), 5, 5, htiChild);
		m_wndSolutionView.InsertItem(_T("External Dependencies"), 2, 3, htiChild);

		m_wndSolutionView.Expand(htiParent, TVE_EXPAND);
		m_wndSolutionView.Expand(htiChild, TVE_EXPAND);


	}
	return 0;
}

void CSolutionExplorerPane::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	int nTop = 0;

	if (m_wndToolBar.GetSafeHwnd())
	{
		CSize sz = m_wndToolBar.CalcDockingLayout(cx, /*LM_HIDEWRAP|*/ LM_HORZDOCK|LM_HORZ | LM_COMMIT);

		m_wndToolBar.MoveWindow(0, nTop, cx, sz.cy);
		m_wndToolBar.Invalidate(FALSE);
		nTop += sz.cy;
	}

	if (m_wndSolutionView.GetSafeHwnd())
	{
		m_wndSolutionView.MoveWindow(0, nTop, cx, cy - nTop);
		m_wndSolutionView.Invalidate(FALSE);
	}
}

void CSolutionExplorerPane::OnEmptyCommand(UINT)
{

}
