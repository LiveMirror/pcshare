// ResourceViewPane.h : header file
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

#if !defined(AFX_RESOURCEVIEWPANE_H__C3883562_44A0_48FF_95E0_0DD0645B896A__INCLUDED_)
#define AFX_RESOURCEVIEWPANE_H__C3883562_44A0_48FF_95E0_0DD0645B896A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DockPaneWnd.h"
/////////////////////////////////////////////////////////////////////////////
// CResourceViewPane window

class CResourceViewPane : public CWnd
{
// Construction
public:
	CResourceViewPane();

// Attributes
public:
	CDockPaneWnd<CTreeCtrl> m_wndResourceView;
	CImageList m_ilResourceView;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CResourceViewPane)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CResourceViewPane();

	// Generated message map functions
protected:
	//{{AFX_MSG(CResourceViewPane)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RESOURCEVIEWPANE_H__C3883562_44A0_48FF_95E0_0DD0645B896A__INCLUDED_)
