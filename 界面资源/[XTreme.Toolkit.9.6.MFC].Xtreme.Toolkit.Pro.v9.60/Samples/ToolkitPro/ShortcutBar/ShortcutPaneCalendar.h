// ShortcutPaneCalendar.h: interface for the CShortcutPaneCalendar class.
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

#if !defined(AFX_SHORTCUTPANECALENDAR_H__9520EE41_3FEF_4F98_A12C_02E592D1FA48__INCLUDED_)
#define AFX_SHORTCUTPANECALENDAR_H__9520EE41_3FEF_4F98_A12C_02E592D1FA48__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CShortcutPaneCalendar : public CXTPShortcutBarPane
{
public:
	CShortcutPaneCalendar();
	virtual ~CShortcutPaneCalendar();

	BOOL Create(LPCTSTR lpszCaption, CXTPShortcutBar* pParent);

	CXTMonthCalCtrl m_wndCalendar;

	CXTTreeCtrl m_wndTreeCalendar;

	CImageList m_ilTreeIcons;

	//{{AFX_MSG(CShortcutPaneCalendar)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

#endif // !defined(AFX_SHORTCUTPANECALENDAR_H__9520EE41_3FEF_4F98_A12C_02E592D1FA48__INCLUDED_)
