// ShortcutPaneMail.cpp: implementation of the CShortcutPaneMail class.
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
#include "ShortcutPaneMail.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

static UINT treeIcons[] =
{
	IDI_INBOX           ,
	IDI_SEARCH          ,
	IDI_SENT_ITEMS      ,
	IDI_OUTBOX          ,
	IDI_OUTLOOK_TODAY   ,
	IDI_DELETED_ITEMS   ,
	IDI_DRAFTS          ,
	IDI_ARCHIVE
};

CShortcutPaneMail::CShortcutPaneMail()
{

	m_ilTreeIcons.Create(16, 16, ILC_MASK|ILC_COLOR32, 1, 1);

	for (int i  = 0; i < _countof(treeIcons); i++)
	{


		HICON hIcon = AfxGetApp()->LoadIcon(treeIcons[i]);
		ASSERT(hIcon);

		m_ilTreeIcons.Add (hIcon);
	}


}

CShortcutPaneMail::~CShortcutPaneMail()
{

}


BOOL CShortcutPaneMail::Create(LPCTSTR lpszCaption, CXTPShortcutBar* pParent)
{
	if (!CXTPShortcutBarPane::Create(lpszCaption, pParent))
		return FALSE;

	VERIFY(m_wndTreeFavorites.Create(WS_VISIBLE|TVS_HASBUTTONS|TVS_LINESATROOT, CXTPEmptyRect(), this, 0));

	m_wndTreeFavorites.SetImageList(&m_ilTreeIcons, TVSIL_NORMAL);

	m_wndTreeFavorites.InsertItem (_T("Inbox"), 0, 0);
	m_wndTreeFavorites.InsertItem (_T("For Follow Up"), 1, 1);
	m_wndTreeFavorites.InsertItem (_T("Sent Items"), 2, 2);
	m_wndTreeFavorites.InsertItem (_T("Outbox"), 3, 3);
	m_wndTreeFavorites.InsertItem (_T("Unread Mail"), 1, 1);
	m_wndTreeFavorites.InsertItem (_T("Deleted Items"), 5, 5);
	m_wndTreeFavorites.InsertItem (_T("Drafts"), 6, 6);



	AddItem(_T("Favorite Folders"), &m_wndTreeFavorites, 115);


	VERIFY(m_wndTreeMail.Create(WS_VISIBLE|TVS_HASBUTTONS|TVS_LINESATROOT, CXTPEmptyRect(), this, 0));

	m_wndTreeMail.SetImageList(&m_ilTreeIcons, TVSIL_NORMAL);

	m_wndTreeMail.InsertItem (_T("Personal"), 4, 4);
	HTREEITEM hItem = m_wndTreeMail.InsertItem (_T("Archive Folders"), 7, 7);
	m_wndTreeMail.InsertItem (_T("Deleted Items"), 5, 5, hItem);
	m_wndTreeMail.InsertItem (_T("Search Folders"), 1, 1, hItem);

	AddItem(_T("Favorite Folders"), &m_wndTreeMail, 125);



	return TRUE;
}
