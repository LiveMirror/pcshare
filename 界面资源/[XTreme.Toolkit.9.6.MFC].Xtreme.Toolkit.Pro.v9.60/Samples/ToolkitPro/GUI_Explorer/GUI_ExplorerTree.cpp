// GUI_ExplorerTree.cpp : implementation file
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
#include "GUI_Explorer.h"
#include "GUI_ExplorerTree.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CGUI_ExplorerTree

IMPLEMENT_DYNCREATE(CGUI_ExplorerTree, CView)

CGUI_ExplorerTree::CGUI_ExplorerTree()
{
}

CGUI_ExplorerTree::~CGUI_ExplorerTree()
{
}


BEGIN_MESSAGE_MAP(CGUI_ExplorerTree, CView)
	//{{AFX_MSG_MAP(CGUI_ExplorerTree)
	ON_WM_CREATE()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_CAPT_BUTTON, OnCaptionButton)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGUI_ExplorerTree drawing

void CGUI_ExplorerTree::OnDraw(CDC* /*pDC*/)
{
}

/////////////////////////////////////////////////////////////////////////////
// CGUI_ExplorerTree diagnostics

#ifdef _DEBUG
void CGUI_ExplorerTree::AssertValid() const
{
	CView::AssertValid();
}

void CGUI_ExplorerTree::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CGUI_ExplorerTree message handlers

BOOL CGUI_ExplorerTree::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Add your specialized code here and/or call the base class
	cs.dwExStyle |= WS_EX_STATICEDGE;
	cs.style |= WS_CLIPCHILDREN|WS_CLIPSIBLINGS;

	return CView::PreCreateWindow(cs);
}

int CGUI_ExplorerTree::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// Create the caption.
	if (!m_wndCaption.Create( this, _T("Folder List"), CPWS_EX_RAISED_EDGE | CPWS_EX_CLOSEBUTTON,
		WS_VISIBLE | SS_CENTER | SS_CENTERIMAGE, CRect(0,0,0,0), IDC_CAPT_BUTTON ))
	{
		TRACE0( "Unable to create caption.\n" );
		return -1;
	}

	if( !m_shellTree.Create(WS_VISIBLE, CRect(0,0,0,0), this, IDC_TREE_VIEW))
	{
		TRACE0( "Unable to create tree control.\n" );
		return -1;
	}

	return 0;
}

void CGUI_ExplorerTree::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	if( m_wndCaption.GetSafeHwnd()) {
		m_wndCaption.MoveWindow( 0, 0, cx, 19 );
	}
	if( m_shellTree.GetSafeHwnd()) {
		m_shellTree.MoveWindow( 0, 19, cx, cy-19 );
	}
}

void CGUI_ExplorerTree::OnCaptionButton()
{
	CMainFrame* pMainFrame = (CMainFrame*)GetParentFrame();
	ASSERT_VALID(pMainFrame);
	pMainFrame->GetSplitterWnd().HideColumn(0);
}

void CGUI_ExplorerTree::SelectParentItem()
{
	if (::IsWindow(m_shellTree))
	{
		HTREEITEM htItem = m_shellTree.GetSelectedItem();
		if (htItem != m_shellTree.GetRootItem())
		{
			m_shellTree.SelectItem(m_shellTree.GetParentItem(htItem));
			m_shellTree.SetFocus();
		}
	}
}
