// TipOfTheDay.h : main header file for the TIPOFTHEDAY application
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

#if !defined(AFX_TIPOFTHEDAY_H__A1085441_6545_40DE_A7D9_659F40B7B74C__INCLUDED_)
#define AFX_TIPOFTHEDAY_H__A1085441_6545_40DE_A7D9_659F40B7B74C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CTipOfTheDayApp:
// See TipOfTheDay.cpp for the implementation of this class
//

class CTipOfTheDayApp : public CWinApp
{
public:
	CTipOfTheDayApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTipOfTheDayApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CTipOfTheDayApp)
	afx_msg void OnAppAbout();
	afx_msg void OnTipoftheday();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	void ShowTipAtStartup();
	void FindTipsFile(LPCTSTR lpszTipsFile);
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TIPOFTHEDAY_H__A1085441_6545_40DE_A7D9_659F40B7B74C__INCLUDED_)
