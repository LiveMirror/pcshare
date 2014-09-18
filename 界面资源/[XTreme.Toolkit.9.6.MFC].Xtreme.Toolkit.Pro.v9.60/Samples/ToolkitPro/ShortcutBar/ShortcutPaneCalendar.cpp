// ShortcutPaneCalendar.cpp: implementation of the CShortcutPaneCalendar class.
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
#include "resource.h"
#include "ShortcutPaneCalendar.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CShortcutPaneCalendar::CShortcutPaneCalendar()
{
	m_ilTreeIcons.Create(16, 16, ILC_MASK|ILC_COLOR32, 1, 1);

	HICON hIcon = AfxGetApp()->LoadIcon(IDI_CALENDAR);
	ASSERT(hIcon);

	m_ilTreeIcons.Add (hIcon);
}

CShortcutPaneCalendar::~CShortcutPaneCalendar()
{

}


BOOL CShortcutPaneCalendar::Create(LPCTSTR lpszCaption, CXTPShortcutBar* pParent)
{
	if (!CXTPShortcutBarPane::Create(lpszCaption, pParent))
		return FALSE;

	m_wndCalendar.Create(WS_CHILD|WS_VISIBLE|MCS_NOTODAY , CPoint(0, 0), this, 0);

	AddItem(_T("Calendar"), &m_wndCalendar, 300);


	VERIFY(m_wndTreeCalendar.Create(WS_VISIBLE|TVS_HASBUTTONS|TVS_LINESATROOT, CXTPEmptyRect(), this, 0));

	m_wndTreeCalendar.SetImageList(&m_ilTreeIcons, TVSIL_NORMAL);

	m_wndTreeCalendar.InsertItem (_T("Calendar"), 0, 0);


	AddItem(_T("My Calendars"), &m_wndTreeCalendar, 115);



	return TRUE;
}

BEGIN_MESSAGE_MAP(CShortcutPaneCalendar, CXTPShortcutBarPane)
	//{{AFX_MSG_MAP(CShortcutPaneCalendar)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


void CShortcutPaneCalendar::OnSize(UINT nType, int cx, int cy)
{
	CXTPShortcutBarPaneItem* pItem = GetItem(0);
	if (pItem)
	{
		pItem->SetHeight(max(0, cy - 150));
	}

	CXTPShortcutBarPane::OnSize(nType, cx, cy);
}
