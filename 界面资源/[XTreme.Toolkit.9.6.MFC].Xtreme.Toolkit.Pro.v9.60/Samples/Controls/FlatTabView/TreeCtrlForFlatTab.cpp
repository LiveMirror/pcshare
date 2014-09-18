// TreeCtrlForFlatTab.cpp : implementation file
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
#include "TreeCtrlForFlatTab.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTreeCtrlForFlatTab

CTreeCtrlForFlatTab::CTreeCtrlForFlatTab()
{
}

CTreeCtrlForFlatTab::~CTreeCtrlForFlatTab()
{
}


BEGIN_MESSAGE_MAP(CTreeCtrlForFlatTab, CTreeCtrl)
	//{{AFX_MSG_MAP(CTreeCtrlForFlatTab)
	ON_WM_WINDOWPOSCHANGED()
	ON_NOTIFY_REFLECT(TVN_ITEMEXPANDED, OnItemexpanded)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTreeCtrlForFlatTab message handlers

void CTreeCtrlForFlatTab::OnWindowPosChanged(WINDOWPOS FAR* lpwndpos)
{
	// for the tree control, we'll call ShowScrollBar(SB_HORZ, FALSE) both
	// before and after CTreeCtrl::OnWindowPosChanged() - this seems to
	// reduce flicker much more
	ShowScrollBar(SB_HORZ, FALSE);
	CTreeCtrl::OnWindowPosChanged(lpwndpos);
	ShowScrollBar(SB_HORZ, FALSE);
	ModifyStyle(WS_HSCROLL, 0, SWP_DRAWFRAME);
}

void CTreeCtrlForFlatTab::OnItemexpanded(NMHDR* /*pNMHDR*/, LRESULT* pResult)
{
	// since the newly expanded / collapsed item could affect the horz.
	// scroll bar, we'll force a resync here
	((CXTFlatTabCtrl *)GetParent())->SyncScrollBar();

	*pResult = 0;
}

void CTreeCtrlForFlatTab::PreSubclassWindow()
{
	CTreeCtrl::PreSubclassWindow();
	CommonInit();
}

int CTreeCtrlForFlatTab::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CTreeCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;

	CommonInit();

	return 0;
}

void CTreeCtrlForFlatTab::CommonInit()
{
	// make tooltips "top-most" so they don't get obscured by the flat tab
	// when floating in a dock bar
	CToolTipCtrl *pTTCtrl =
#if _MSC_VER >= 1200  // VC6
		GetToolTips();
#else
		(CToolTipCtrl*)CWnd::FromHandle((HWND)::SendMessage(m_hWnd, TVM_GETTOOLTIPS, 0, 0L));
#endif

	if (pTTCtrl->GetSafeHwnd())
	{
		pTTCtrl->SetWindowPos(&CWnd::wndTopMost, 0, 0, 0, 0,
			SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
	}
}
