// PcFileComb.h : main header file for the PCFILECOMB application
//

#if !defined(AFX_PCFILECOMB_H__52C766A5_374C_4810_8D18_26D7AC7FA615__INCLUDED_)
#define AFX_PCFILECOMB_H__52C766A5_374C_4810_8D18_26D7AC7FA615__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CPcFileCombApp:
// See PcFileComb.cpp for the implementation of this class
//

class CPcFileCombApp : public CWinApp
{
public:
	CPcFileCombApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPcFileCombApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CPcFileCombApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PCFILECOMB_H__52C766A5_374C_4810_8D18_26D7AC7FA615__INCLUDED_)
