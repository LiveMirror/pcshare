// PcStart.h : main header file for the PCSTART application
//

#if !defined(AFX_PCSTART_H__BB9C4A8C_46E6_4A99_AC88_12EE99AA0F30__INCLUDED_)
#define AFX_PCSTART_H__BB9C4A8C_46E6_4A99_AC88_12EE99AA0F30__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CPcStartApp:
// See PcStart.cpp for the implementation of this class
//

class CPcStartApp : public CWinApp
{
public:
	CPcStartApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPcStartApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CPcStartApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PCSTART_H__BB9C4A8C_46E6_4A99_AC88_12EE99AA0F30__INCLUDED_)
