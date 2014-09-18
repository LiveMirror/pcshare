// XTTabView.cpp : implementation of the CXTTabView class.
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

#include "StdAfx.h"
#include "XTDefines.h"
#include "XTGlobal.h"
#include "XTVC50Helpers.h"
#include "XTMemDC.h"
#include "XTTabCtrlButtons.h"
#include "XTTabCtrl.h"
#include "XTTabView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CXTTabView

IMPLEMENT_DYNCREATE(CXTTabView, CCtrlView)

BEGIN_MESSAGE_MAP(CXTTabView, CCtrlView)
	//{{AFX_MSG_MAP(CXTTabView)
	ON_WM_MOUSEACTIVATE()
	//}}AFX_MSG_MAP
	MESSAGE_MAP_ENTRIES_CXTTabCtrlBaseEx
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXTTabView construction/destruction

CXTTabView::CXTTabView() : CCtrlView(WC_TABCONTROL, AFX_WS_DEFAULT_VIEW|TCS_TOOLTIPS)
{
	m_bBoldFont = FALSE;
	ImplAttach(&GetTabCtrl());
}

CXTTabView::~CXTTabView()
{
}

BOOL CXTTabView::PreCreateWindow(CREATESTRUCT& cs)
{
	// Make sure the common controls library is initialized.
	::InitCommonControls();

	return CCtrlView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CXTTabView message handlers

void CXTTabView::OnDestroy()
{
	OnDestroyImpl_Pre();
	CCtrlView::OnDestroy();
}

void CXTTabView::OnSelchanging (NMHDR* pNMHDR, LRESULT* pResult)
{
	OnSelchangingImpl(pNMHDR, pResult);
}

void CXTTabView::OnSelchange (NMHDR* pNMHDR, LRESULT* pResult)
{
	OnSelchangeImpl(pNMHDR, pResult);
}


void CXTTabView::OnWindowPosChanged(WINDOWPOS FAR* lpwndpos)
{
	OnWindowPosChangedImpl_Pre(lpwndpos);
	CCtrlView::OnWindowPosChanged(lpwndpos);
	OnWindowPosChangedImpl_Post(lpwndpos);
}

int CXTTabView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CCtrlView::OnCreate(lpCreateStruct) == -1)
		return -1;

	return OnCreateImpl_Post(lpCreateStruct);
}

/////////////////////////////////////////////////////////////////////////////
// CXTTabView tooltip related functions

void CXTTabView::UpdateDocTitle()
{
	TCHAR szText[256];

	TC_ITEM tci;
	tci.mask       = TCIF_TEXT;
	tci.cchTextMax = _countof(szText);
	tci.pszText    = szText;

	GetTabCtrl().GetItem(GetTabCtrl().GetCurSel(), &tci);
	GetDocument()->SetTitle(tci.pszText);
}

BOOL CXTTabView::PreTranslateMessage(MSG* pMsg)
{
	if (PreTranslateMessageImpl(pMsg))
		return TRUE;

	return CCtrlView::PreTranslateMessage(pMsg);
}

BOOL CXTTabView::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	if (OnCmdMsgImpl_Pre(nID, nCode, pExtra, pHandlerInfo))
	{
		return TRUE;
	}
	return CCtrlView::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

BOOL CXTTabView::OnEraseBkgnd(CDC* pDC)
{
	return OnEraseBkgndImpl(pDC);
}

void CXTTabView::OnPaint()
{
	OnPaintImpl();
}

int CXTTabView::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message)
{
	// If current active view is a child view, then do nothing.
	CFrameWnd* pParentFrame = GetParentFrame();
	CView* pView = pParentFrame ? pParentFrame->GetActiveView() : NULL;

	if (IsChildView(pView)) {
		return MA_NOACTIVATE;
	}

	return CCtrlView::OnMouseActivate(pDesktopWnd, nHitTest, message);
}

void CXTTabView::OnInitialUpdate()
{
	CCtrlView::OnInitialUpdate();
	CXTTabCtrlBaseEx::OnInitialUpdateImpl();

	// Get a pointer to the active view.
	CView* pActiveView = DYNAMIC_DOWNCAST(CView, GetActiveView());
	if (pActiveView != NULL)
	{
		// Get a pointer to the parent frame.
		CFrameWnd* pParentFrame = GetParentFrame();
		if (pParentFrame != NULL)
		{
			// If the active view for the frame does not match the active
			// view for the tab control, set the frames active view.
			if (pParentFrame->GetActiveView() != pActiveView) {
				pParentFrame->SetActiveView(pActiveView);
			}
		}
	}
}

void CXTTabView::OnRButtonDown(UINT nFlags, CPoint point)
{
	OnRButtonDownImpl(nFlags, point);
}

BOOL CXTTabView::EnableToolTips(BOOL bEnable)
{
	return EnableToolTipsImpl(bEnable);
}
