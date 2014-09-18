// ShortcutPaneFolders.cpp: implementation of the CShortcutPaneFolders class.
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
#include "shortcutbar.h"
#include "ShortcutPaneFolders.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


static UINT treeIcons[] =
{
	IDI_INBOX           ,
	IDI_SEARCH          ,
	IDI_SENT_ITEMS      ,
	IDI_OUTBOX          ,
	IDI_OUTLOOK_TODAY   ,
	IDI_DELETED_ITEMS   ,
	IDI_DRAFTS          ,
	IDI_ARCHIVE         ,
	IDI_CALENDAR        ,
	IDI_CONTACTS
};
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CShortcutPaneFolders::CShortcutPaneFolders()
{


	m_ilTreeIcons.Create(16, 16, ILC_MASK|ILC_COLOR32, 1, 1);

	for (int i  = 0; i < _countof(treeIcons); i++)
	{


		HICON hIcon = AfxGetApp()->LoadIcon(treeIcons[i]);
		ASSERT(hIcon);

		m_ilTreeIcons.Add (hIcon);
	}

}

CShortcutPaneFolders::~CShortcutPaneFolders()
{

}

BOOL CShortcutPaneFolders::Create(LPCTSTR lpszCaption, CXTPShortcutBar* pParent)
{
	if (!CXTPShortcutBarPane::Create(lpszCaption, pParent))
		return FALSE;

	VERIFY(m_wndTreeFolders.Create(WS_VISIBLE|TVS_HASBUTTONS|TVS_LINESATROOT, CXTPEmptyRect(), this, 0));

	m_wndTreeFolders.SetImageList(&m_ilTreeIcons, TVSIL_NORMAL);

	HTREEITEM hItem = m_wndTreeFolders.InsertItem (_T("Personal"), 4, 4);

	m_wndTreeFolders.InsertItem (_T("Deleted Items"), 5, 5, hItem);
	m_wndTreeFolders.InsertItem (_T("Inbox"), 0, 0, hItem);
	m_wndTreeFolders.InsertItem (_T("Sent Items"), 2, 2, hItem);
	m_wndTreeFolders.InsertItem (_T("Outbox"), 3, 3, hItem);
	m_wndTreeFolders.InsertItem (_T("Calendar"), 8, 8, hItem);
	m_wndTreeFolders.InsertItem (_T("Unread Mail"), 1, 1, hItem);
	m_wndTreeFolders.InsertItem (_T("Drafts"), 6, 6, hItem);
	m_wndTreeFolders.InsertItem(_T("Contacts"), 9, 9, hItem);

	HTREEITEM hItemSearch = m_wndTreeFolders.InsertItem (_T("Search Folders"), 1, 1, hItem);
	m_wndTreeFolders.InsertItem (_T("For Follow Up"), 1, 1, hItemSearch);
	m_wndTreeFolders.InsertItem (_T("Large Mail"), 1, 1, hItemSearch);
	m_wndTreeFolders.InsertItem (_T("Unread Mail"), 1, 1, hItemSearch);

	m_wndTreeFolders.Expand(hItem, TVE_EXPAND);

	hItem = m_wndTreeFolders.InsertItem (_T("Archive Folders"), 7, 7);
	m_wndTreeFolders.InsertItem (_T("Deleted Items"), 5, 5, hItem);
	m_wndTreeFolders.InsertItem (_T("Search Folders"), 1, 1, hItem);



	AddItem(_T("All Folders"), &m_wndTreeFolders, 115);



	return TRUE;
}
