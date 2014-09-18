// PcSocksServer.h : main header file for the PCSOCKSSERVER application
//

#if !defined(AFX_PCSOCKSSERVER_H__87836CC1_9B6A_44D5_A44B_B98AE3E141A1__INCLUDED_)
#define AFX_PCSOCKSSERVER_H__87836CC1_9B6A_44D5_A44B_B98AE3E141A1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CPcSocksServerApp:
// See PcSocksServer.cpp for the implementation of this class
//

class CPcSocksServerApp : public CWinApp
{
public:
	CPcSocksServerApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPcSocksServerApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CPcSocksServerApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PCSOCKSSERVER_H__87836CC1_9B6A_44D5_A44B_B98AE3E141A1__INCLUDED_)
