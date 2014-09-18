// Scribble.h : main header file for the SCRIBBLE application
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

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CScribbleApp:
// See Scribble.cpp for the implementation of this class
//

class CScribbleApp : public CWinApp
{
public:
	CScribbleApp();


	void ReloadDefaultMenu();
	void SetRTLLayout(BOOL bRTLLayout);
	BOOL IsUnicodeAndLayoutSupport();
	BOOL SetLocale(LCID Locale, const char* szLocale, BOOL bIsRTL, BOOL bIsTextFile = FALSE);

	UINT m_curLanguage;
	BOOL m_bIsWin2000OrLater;
	BOOL m_bIsWinNT;
	BOOL m_bIsRTL;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CScribbleApp)
	public:
	virtual BOOL InitInstance();
	virtual CDocument* OpenDocumentFile(LPCTSTR lpszFileName);
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
	COleTemplateServer m_server;
		// Server object for document creation

	//{{AFX_MSG(CScribbleApp)
	afx_msg void OnAppAbout();
	afx_msg void OnLanguageEnglish();
	afx_msg void OnUpdateLanguageEnglish(CCmdUI* pCmdUI);
	afx_msg void OnLanguageRussian();
	afx_msg void OnUpdateLanguageRussian(CCmdUI* pCmdUI);
	afx_msg void OnLanguageGerman();
	afx_msg void OnUpdateLanguageGerman(CCmdUI* pCmdUI);
	afx_msg void OnLanguageJapanese();
	afx_msg void OnUpdateLanguageJapanese(CCmdUI* pCmdUI);
	afx_msg void OnLanguageArabic();
	afx_msg void OnUpdateLanguageArabic(CCmdUI* pCmdUI);
	afx_msg void OnFileNew();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()



};


/////////////////////////////////////////////////////////////////////////////
