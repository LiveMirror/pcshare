// GUI_ExplorerView.cpp : implementation file
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
#include "GUI_ExplorerView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGUI_ExplorerView

IMPLEMENT_DYNCREATE(CGUI_ExplorerView, CView)

CGUI_ExplorerView::CGUI_ExplorerView()
{
}

CGUI_ExplorerView::~CGUI_ExplorerView()
{
}


BEGIN_MESSAGE_MAP(CGUI_ExplorerView, CView)
	//{{AFX_MSG_MAP(CGUI_ExplorerView)
	ON_WM_CREATE()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
	ON_MESSAGE(XTWM_SHELL_NOTIFY, OnUpdateShell)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGUI_ExplorerView drawing

void CGUI_ExplorerView::OnDraw(CDC* /*pDC*/)
{
}

/////////////////////////////////////////////////////////////////////////////
// CGUI_ExplorerView diagnostics

#ifdef _DEBUG
void CGUI_ExplorerView::AssertValid() const
{
	CView::AssertValid();
}

void CGUI_ExplorerView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CGUI_ExplorerView message handlers

BOOL CGUI_ExplorerView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Add your specialized code here and/or call the base class
	cs.style |= WS_CLIPCHILDREN|WS_CLIPSIBLINGS;
	cs.dwExStyle |= WS_EX_STATICEDGE;

	return CView::PreCreateWindow(cs);
}

int CGUI_ExplorerView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO: Add your specialized creation code here
	if (!m_shellList.Create(WS_VISIBLE | WS_CHILD | WS_CLIPCHILDREN
		| LVS_REPORT | LVS_SHAREIMAGELISTS | LVS_EDITLABELS,
		CRect(0,0,0,0), this, 0))
	{
		TRACE0("Failed to create list control.\n");
		return -1;
	}

	m_shellList.GetFlatHeaderCtrl()->EnableAutoSize();
	m_shellList.SetMinimumColSize(15);

	return 0;
}

void CGUI_ExplorerView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	if(m_shellList.GetSafeHwnd()) {
		m_shellList.MoveWindow(0,0,cx,cy);
	}
}

LRESULT CGUI_ExplorerView::OnUpdateShell(WPARAM wParam, LPARAM /*lParam*/)
{
	// Pass along to parent frame.
	CFrameWnd* pFrameWnd = GetParentFrame();
	ASSERT_VALID(pFrameWnd);
	pFrameWnd->SendMessage(XTWM_SHELL_NOTIFY, wParam);
	return 0;
}
