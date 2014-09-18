// SrcModifyTool.h : main header file for the SRCMODIFYTOOL application
//

#if !defined(AFX_SRCMODIFYTOOL_H__09DBE969_B852_4D5F_994D_A967B17BAC53__INCLUDED_)
#define AFX_SRCMODIFYTOOL_H__09DBE969_B852_4D5F_994D_A967B17BAC53__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CSrcModifyToolApp:
// See SrcModifyTool.cpp for the implementation of this class
//

class CSrcModifyToolApp : public CWinApp
{
public:
	CSrcModifyToolApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSrcModifyToolApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CSrcModifyToolApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SRCMODIFYTOOL_H__09DBE969_B852_4D5F_994D_A967B17BAC53__INCLUDED_)
