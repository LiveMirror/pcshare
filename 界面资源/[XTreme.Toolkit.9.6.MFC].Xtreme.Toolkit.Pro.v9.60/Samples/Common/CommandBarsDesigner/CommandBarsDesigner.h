// CommandBarsDesigner.h : main header file for the COMMANDBARSDESIGNER application
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

#if !defined(AFX_COMMANDBARSDESIGNER_H__3D7EA821_8D9C_4378_944E_9FAFC5ADF727__INCLUDED_)
#define AFX_COMMANDBARSDESIGNER_H__3D7EA821_8D9C_4378_944E_9FAFC5ADF727__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols


const LPCTSTR lpTypes[] = {NULL, _T("xtpControlButton"), _T("xtpControlPopup"), _T("xtpControlButtonPopup"), _T("xtpControlSplitButtonPopup"), _T("xtpControlComboBox"), _T("xtpControlEdit"), NULL, _T("xtpControlLabel") };


/////////////////////////////////////////////////////////////////////////////
// CCommandBarsDesignerApp:
// See CommandBarsDesigner.cpp for the implementation of this class
//

class CCommandBarsDesignerApp : public CWinApp
{
public:
	CCommandBarsDesignerApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCommandBarsDesignerApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

	CMultiDocTemplate* m_pDocTemplate;
	BOOL m_bRunAutomated;

// Implementation
	COleTemplateServer m_server;
		// Server object for document creation
	//{{AFX_MSG(CCommandBarsDesignerApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

extern CString StripChars(CString str);
extern void XTPStringRemove(CString& str, TCHAR chRemove);
extern int XTPStringDelete(CString& str, int nIndex, int nCount = 1);

extern COLORREF GetStaticFrameColor();
extern BOOL IsRunAutomated();

AFX_INLINE BOOL IsXMLEngineAvailable() 
{
	static BOOL bAvailable = (BOOL)-1;
	
	if (bAvailable == (BOOL)-1)
	{	
		CXTPDOMDocumentPtr xmlDocPtr;
		bAvailable = SUCCEEDED(xmlDocPtr.CreateInstance(CLSID_XTPDOMDocument));
	}
	return bAvailable;

}



/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COMMANDBARSDESIGNER_H__3D7EA821_8D9C_4378_944E_9FAFC5ADF727__INCLUDED_)
