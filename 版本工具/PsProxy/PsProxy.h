// PsProxy.h : main header file for the PSPROXY application
//

#if !defined(AFX_PSPROXY_H__07EF599B_7F16_44B4_9EBB_DA54CF75893C__INCLUDED_)
#define AFX_PSPROXY_H__07EF599B_7F16_44B4_9EBB_DA54CF75893C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CPsProxyApp:
// See PsProxy.cpp for the implementation of this class
//

class CPsProxyApp : public CWinApp
{
public:
	CPsProxyApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPsProxyApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CPsProxyApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PSPROXY_H__07EF599B_7F16_44B4_9EBB_DA54CF75893C__INCLUDED_)
