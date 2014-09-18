// NetServer.h : main header file for the NETSERVER application
//

#if !defined(AFX_NETSERVER_H__AE82581F_072E_400B_A3B2_8E30AAAFC79C__INCLUDED_)
#define AFX_NETSERVER_H__AE82581F_072E_400B_A3B2_8E30AAAFC79C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CNetServerApp:
// See NetServer.cpp for the implementation of this class
//

class CNetServerApp : public CWinApp
{
public:
	CNetServerApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNetServerApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CNetServerApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NETSERVER_H__AE82581F_072E_400B_A3B2_8E30AAAFC79C__INCLUDED_)
