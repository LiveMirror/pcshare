// PcLKey.h : main header file for the PCLKEY DLL
//

#if !defined(AFX_PCLKEY_H__10613D21_273E_48EC_8234_673B894F95D3__INCLUDED_)
#define AFX_PCLKEY_H__10613D21_273E_48EC_8234_673B894F95D3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CPcLKeyApp
// See PcLKey.cpp for the implementation of this class
//

class CPcLKeyApp : public CWinApp
{
public:
	CPcLKeyApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPcLKeyApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CPcLKeyApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PCLKEY_H__10613D21_273E_48EC_8234_673B894F95D3__INCLUDED_)
