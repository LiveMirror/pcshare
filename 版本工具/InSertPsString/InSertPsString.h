// InSertPsString.h : main header file for the INSERTPSSTRING application
//

#if !defined(AFX_INSERTPSSTRING_H__376E4E67_F05F_4090_9020_42ADE64D49B2__INCLUDED_)
#define AFX_INSERTPSSTRING_H__376E4E67_F05F_4090_9020_42ADE64D49B2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CInSertPsStringApp:
// See InSertPsString.cpp for the implementation of this class
//

class CInSertPsStringApp : public CWinApp
{
public:
	CInSertPsStringApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInSertPsStringApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CInSertPsStringApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INSERTPSSTRING_H__376E4E67_F05F_4090_9020_42ADE64D49B2__INCLUDED_)
