// ShortcutPaneContacts.h: interface for the CShortcutPaneContacts class.
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

#if !defined(AFX_SHORTCUTPANECONTACTS_H__03BA0B52_DDD8_4E32_BFBA_CC2F6B7CDCEB__INCLUDED_)
#define AFX_SHORTCUTPANECONTACTS_H__03BA0B52_DDD8_4E32_BFBA_CC2F6B7CDCEB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CShortcutPaneContacts : public CXTPShortcutBarPane
{
public:
	CShortcutPaneContacts();
	virtual ~CShortcutPaneContacts();

	BOOL Create(LPCTSTR lpszCaption, CXTPShortcutBar* pParent);

	CXTTreeCtrl m_wndTreeContacts;
	CImageList m_ilTreeIcons;
};

#endif // !defined(AFX_SHORTCUTPANECONTACTS_H__03BA0B52_DDD8_4E32_BFBA_CC2F6B7CDCEB__INCLUDED_)
