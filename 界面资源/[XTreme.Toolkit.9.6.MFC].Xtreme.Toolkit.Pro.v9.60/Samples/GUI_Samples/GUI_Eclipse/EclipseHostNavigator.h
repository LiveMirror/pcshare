// EclipseHostNavigator.h : header file
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

#if !defined(AFX_ECLIPSEHOSTNAVIGATOR_H__1217333A_2CFC_449D_AF9A_93985BB326A2__INCLUDED_)
#define AFX_ECLIPSEHOSTNAVIGATOR_H__1217333A_2CFC_449D_AF9A_93985BB326A2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "EclipseHost.h"

/////////////////////////////////////////////////////////////////////////////
// CEclipseHostNavigator window

class CEclipseHostNavigator : public CEclipseHost
{
// Construction
public:
	CEclipseHostNavigator();

// Attributes
public:
	CTreeCtrl m_wndTree;
	CImageList m_ilTreeIcons;

// Operations
public:
	virtual CWnd* GetClientWnd() { return &m_wndTree; }

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEclipseHostNavigator)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CEclipseHostNavigator();

	// Generated message map functions
protected:
	//{{AFX_MSG(CEclipseHostNavigator)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ECLIPSEHOSTNAVIGATOR_H__1217333A_2CFC_449D_AF9A_93985BB326A2__INCLUDED_)
