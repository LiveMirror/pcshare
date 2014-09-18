// ToolsView.cpp: implementation of the CToolsView class.
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
#include "ToolsView.h"
#include "DockingContainersView.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CToolsView::CToolsView()
{
	m_nToolBarID = IDR_TOOLS_BAR;
	m_nCurrentTool = ID_TOOLS_SELECTION;
}

CToolsView::~CToolsView()
{

}


BEGIN_MESSAGE_MAP(CToolsView, CPaneView)
	//{{AFX_MSG_MAP(CToolsView)
	ON_COMMAND_RANGE(ID_TOOLS_SELECTION, ID_TOOLS_ELLIPSE2, OnToolsCommand)
	ON_UPDATE_COMMAND_UI_RANGE(ID_TOOLS_SELECTION, ID_TOOLS_ELLIPSE2, OnUpdateToolsCommand)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CToolsView::Refresh()
{
	if (m_wndNavigateWnd.GetSafeHwnd())
		m_wndNavigateWnd.Invalidate(FALSE);
}

CWnd* CToolsView::OnCreateView()
{
	if (!m_wndNavigateWnd.Create(NULL, WS_VISIBLE | WS_CHILD,
		CRect(0,0,0,0), this ))
	{
		TRACE0( "Unable to create tree control.\n" );
		return NULL;
	}

	return &m_wndNavigateWnd;
}

void CToolsView::OnToolsCommand(UINT nID)
{
	m_nCurrentTool = nID;

}

void CToolsView::OnUpdateToolsCommand(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_nCurrentTool == pCmdUI->m_nID? TRUE: FALSE);

}


/////////////////////////////////////////////////////////////////////////////
// CToolsNavigateWnd

CToolsNavigateWnd::CToolsNavigateWnd()
{
}

CToolsNavigateWnd::~CToolsNavigateWnd()
{
}


BEGIN_MESSAGE_MAP(CToolsNavigateWnd, CStatic)
	//{{AFX_MSG_MAP(CToolsNavigateWnd)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CToolsNavigateWnd message handlers

BOOL CToolsNavigateWnd::OnEraseBkgnd(CDC*)
{
	return TRUE;
}

void CToolsNavigateWnd::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	CXTPClientRect rc(this);

	dc.FillSolidRect(rc, GetSysColor(COLOR_WINDOW));


	CDockingContainersView* pView =
		((CMDIFrameWnd*)AfxGetMainWnd())->MDIGetActive()?
		(CDockingContainersView*)(((CMDIFrameWnd*)AfxGetMainWnd())->MDIGetActive()->GetActiveView())
		: NULL;

	if (pView)
	{
		pView->DrawLines(&dc, rc);
	}
}

void CToolsNavigateWnd::OnSize(UINT nType, int cx, int cy)
{
	CStatic::OnSize(nType, cx, cy);

	Invalidate(FALSE);
}
