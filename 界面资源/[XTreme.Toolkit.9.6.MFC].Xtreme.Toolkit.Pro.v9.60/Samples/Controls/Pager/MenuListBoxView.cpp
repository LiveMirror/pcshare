// MenuListBoxView.cpp : implementation file
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
#include "Pager.h"
#include "MenuListBoxView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMenuListBoxView

IMPLEMENT_DYNCREATE(CMenuListBoxView, CView)

CMenuListBoxView::CMenuListBoxView()
{
}

CMenuListBoxView::~CMenuListBoxView()
{
}

#ifndef _countof
#define _countof(array) (sizeof(array)/sizeof(array[0]))
#endif

BEGIN_MESSAGE_MAP(CMenuListBoxView, CView)
	//{{AFX_MSG_MAP(CMenuListBoxView)
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_MESSAGE( XTWM_OUTBAR_NOTIFY,  OnOutbarNotify )
	//}}AFX_MSG_MAP
	ON_NOTIFY(PGN_SCROLL, IDC_PAGER_CTRL, OnPagerScroll)
	ON_NOTIFY(PGN_CALCSIZE, IDC_PAGER_CTRL, OnPagerCalcSize)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMenuListBoxView drawing

void CMenuListBoxView::OnDraw(CDC*)
{
}

/////////////////////////////////////////////////////////////////////////////
// CMenuListBoxView diagnostics

#ifdef _DEBUG
void CMenuListBoxView::AssertValid() const
{
	CView::AssertValid();
}

void CMenuListBoxView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMenuListBoxView message handlers

void CMenuListBoxView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	if(m_wndPager.GetSafeHwnd()) {
		m_wndPager.MoveWindow(0,0,cx,cy);
	}
}

int CMenuListBoxView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// Create the pager control.
	if (!m_wndPager.Create(WS_CHILD|WS_VISIBLE|PGS_VERT,
		CRect(0,0,0,0), this, IDC_PAGER_CTRL ))
	{
		TRACE0("Failed to create CPagerCtrl...\n");
		return -1;
	}

	// Create the OutlookBar control using m_wndPager as the parent.
	if (!m_wndMenuListBox.Create( WS_CHILD | WS_VISIBLE | WS_TABSTOP,
		CRect(0,0,0,0), &m_wndPager, IDC_OUTBAR ))
	{
		TRACE0("Failed to create COutlookBar...\n");
		return -1;
	}

	// Set the CWnd object you want messages sent to.
	m_wndMenuListBox.SetOwner(this);
//  m_wndMenuListBox.SetColors(RGB(0xff,0xff,0xff), RGB(0x3a,0x6e,0xa5));

	// Add items to the Outlook Bar control.
	m_wndMenuListBox.AddMenuItem(IDI_ICON_OUTLOOK,  _T("Outlook Today")),
	m_wndMenuListBox.AddMenuItem(IDI_ICON_CONTACTS, _T("Contacts")     ),
	m_wndMenuListBox.AddMenuItem(IDI_ICON_TASKS,    _T("Tasks")        ),
	m_wndMenuListBox.AddMenuItem(IDI_ICON_JOURNAL,  _T("Journal")      ),
	m_wndMenuListBox.AddMenuItem(IDI_ICON_NOTES,    _T("Notes")        ),
	m_wndMenuListBox.AddMenuItem(IDI_ICON_DELETED,  _T("Deleted Items")),
	m_wndMenuListBox.AddMenuItem(IDI_ICON_PUBLIC,   _T("Public")       ),
	m_wndMenuListBox.AddMenuItem(IDI_ICON_DRAFTS,   _T("Drafts")       ),
	m_wndMenuListBox.AddMenuItem(IDI_ICON_OUTBOX,   _T("Outbox")       ),
	m_wndMenuListBox.AddMenuItem(IDI_ICON_SENT,     _T("Sent")         ),

	// Insert menu items at a specific index.
	m_wndMenuListBox.InsertMenuItem(1, IDI_ICON_INBOX,    _T("Inbox")   ),
	m_wndMenuListBox.InsertMenuItem(2, IDI_ICON_CALENDAR, _T("Calendar")),

	// Set the child HWND to COutlookBar, and button size to 15.
	m_wndPager.SetChild(m_wndMenuListBox.GetSafeHwnd());
	m_wndPager.SetButtonSize(15);

	m_wndMenuListBox.EnableMenuItem(2, FALSE);

	return 0;
}

void CMenuListBoxView::OnPagerCalcSize(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMPGCALCSIZE* pNMPGCalcSize = (NMPGCALCSIZE*)pNMHDR;

	switch(pNMPGCalcSize->dwFlag)
	{
	case PGF_CALCWIDTH:
		break;

	case PGF_CALCHEIGHT:
		pNMPGCalcSize->iHeight = m_wndMenuListBox.GetCount()
			*(::GetSystemMetrics(SM_CYICON)*2);
		break;
	}

	*pResult = 0;
}

void CMenuListBoxView::OnPagerScroll(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMPGSCROLL* pNMPGScroll = (NMPGSCROLL*)pNMHDR;
	UNREFERENCED_PARAMETER(pNMPGScroll);

	*pResult = 0;

	// TODO: Handle pager scroll notification
}

LRESULT CMenuListBoxView::OnOutbarNotify(UINT lParam, LONG wParam)
{
	if ((int)lParam == -1) // -1 means no selection
		return 0;

	switch( wParam ) // control id.
	{
	case IDC_OUTBAR:
		{
			// Get the menu item.
			CXTMenuListBox::CONTENT_ITEM* pContentItems = m_wndMenuListBox.GetMenuItem((int)lParam);
			ASSERT(pContentItems);

			AfxMessageBox(pContentItems->m_strText);
		}
		break;
	}
	return 0;
}

BOOL CMenuListBoxView::PreCreateWindow(CREATESTRUCT& cs)
{
	if (!CView::PreCreateWindow(cs))
		return FALSE;

	cs.style |= WS_CLIPCHILDREN | WS_CLIPSIBLINGS;

	// TODO: Add your specialized code here and/or call the base class

	return TRUE;
}
