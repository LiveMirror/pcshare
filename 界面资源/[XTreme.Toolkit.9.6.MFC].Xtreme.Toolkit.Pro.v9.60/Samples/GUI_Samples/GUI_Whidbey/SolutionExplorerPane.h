// SolutionExplorerPane.h : header file
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

#if !defined(AFX_SOLUTIONEXPLORERPANE_H__5FFD8D7B_C89F_48B8_A003_5425148766C2__INCLUDED_)
#define AFX_SOLUTIONEXPLORERPANE_H__5FFD8D7B_C89F_48B8_A003_5425148766C2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DockPaneWnd.h"
/////////////////////////////////////////////////////////////////////////////
// CSolutionExplorerPane window

class CSolutionExplorerPane : public CWnd
{
// Construction
public:
	CSolutionExplorerPane();

// Attributes
public:
	CDockPaneWnd<CTreeCtrl> m_wndSolutionView;
	CImageList m_ilSolutionView;
	CXTPToolBar m_wndToolBar;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSolutionExplorerPane)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CSolutionExplorerPane();

	// Generated message map functions
protected:
	//{{AFX_MSG(CSolutionExplorerPane)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	void OnEmptyCommand(UINT);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SOLUTIONEXPLORERPANE_H__5FFD8D7B_C89F_48B8_A003_5425148766C2__INCLUDED_)
