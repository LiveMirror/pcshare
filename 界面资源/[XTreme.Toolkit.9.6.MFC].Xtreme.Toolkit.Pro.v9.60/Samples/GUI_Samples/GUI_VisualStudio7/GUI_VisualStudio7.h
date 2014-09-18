// GUI_VisualStudio7.h : main header file for the GUI_VISUALSTUDIO7 application
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

#if !defined(AFX_GUI_VISUALSTUDIO7_H__7C25AEDA_700F_4186_B072_BC912D5186EC__INCLUDED_)
#define AFX_GUI_VISUALSTUDIO7_H__7C25AEDA_700F_4186_B072_BC912D5186EC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CGUI_VisualStudio7App:
// See GUI_VisualStudio7.cpp for the implementation of this class
//

class CGUI_VisualStudio7App : public CWinApp
{
public:
	CGUI_VisualStudio7App();

	BOOL m_bFirstTime;

	CMultiDocTemplate* m_pDocTemplateStartPage;
	CMultiDocTemplate* m_pDocTemplate;
	CMultiDocTemplate* m_pDocTemplateBrowserPage;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGUI_VisualStudio7App)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CGUI_VisualStudio7App)
	afx_msg void OnAppAbout();
	afx_msg void OnFileNew();
	afx_msg void OnWindowCloseAll();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	afx_msg void OnGotoURL(NMHDR* pNMHDR, LRESULT* pResult);
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GUI_VISUALSTUDIO7_H__7C25AEDA_700F_4186_B072_BC912D5186EC__INCLUDED_)
