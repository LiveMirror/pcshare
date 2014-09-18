// PropertiesTabCtrl.cpp : implementation file
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
#include "CommandBarIcons.h"
#include "PropertiesTabCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPropertiesTabCtrl

CPropertiesTabCtrl::CPropertiesTabCtrl()
{
	m_iSelectedTab = 0;
}

CPropertiesTabCtrl::~CPropertiesTabCtrl()
{
}

void CPropertiesTabCtrl::AddTab(CWnd* pWnd, LPTSTR lpszCaption)
{
	ASSERT_VALID(pWnd);

	TCITEM item;
	item.mask = TCIF_TEXT|TCIF_PARAM;
	item.lParam = (LPARAM) pWnd;
	item.pszText = lpszCaption;

	InsertItem(GetItemCount(), &item);

	CRect rc;
	GetClientRect(&rc);
	AdjustRect(FALSE, &rc);

	pWnd->SetWindowPos(NULL, rc.left, rc.top, rc.Width(), rc.Height(),
						SWP_FRAMECHANGED|SWP_NOZORDER);

	pWnd->ShowWindow(GetItemCount() != 1 ? SW_HIDE : SW_SHOW);
}


BEGIN_MESSAGE_MAP(CPropertiesTabCtrl, CTabCtrl)
	//{{AFX_MSG_MAP(CPropertiesTabCtrl)
	ON_NOTIFY_REFLECT(TCN_SELCHANGE, OnSelectionChanged)
	ON_NOTIFY_REFLECT(TCN_SELCHANGING, OnSelectionChanging)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropertiesTabCtrl message handlers

void CPropertiesTabCtrl::OnSelectionChanged(NMHDR* /*pNMHDR*/, LRESULT* pResult)
{
	int iNewTab = GetCurSel();

	TCITEM item;
	CPropertiesTabPage* pWnd;

	item.mask = TCIF_PARAM;

	//** hide the current tab ---------
	GetItem(m_iSelectedTab, &item);
	pWnd = reinterpret_cast<CPropertiesTabPage*> (item.lParam);
	ASSERT_VALID(pWnd);
	pWnd->ShowWindow(SW_HIDE);

	//** show the selected tab --------
	GetItem(iNewTab, &item);
	pWnd = reinterpret_cast<CPropertiesTabPage*> (item.lParam);
	ASSERT_VALID(pWnd);
	pWnd->ShowWindow(SW_SHOW);
	pWnd->OnActivate();

	*pResult = 0;
}

void CPropertiesTabCtrl::OnSelectionChanging(NMHDR* /*pNMHDR*/, LRESULT* pResult)
{
	m_iSelectedTab = GetCurSel();

	*pResult = 0;
}
