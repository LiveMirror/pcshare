// EclipseHostBookMarks.h : header file
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

#if !defined(AFX_ECLIPSEHOSTBOOKMARKS_H__0BDCDA6B_6AB4_409E_8059_2C1411C45AF6__INCLUDED_)
#define AFX_ECLIPSEHOSTBOOKMARKS_H__0BDCDA6B_6AB4_409E_8059_2C1411C45AF6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "EclipseHost.h"
/////////////////////////////////////////////////////////////////////////////
// CEclipseHostBookmarks window

class CEclipseHostBookmarks : public CEclipseHost
{
// Construction
public:
	CEclipseHostBookmarks();

// Attributes
public:
	CXTListCtrlEx m_listCtrl;
	void InitListControl();

	virtual CWnd* GetClientWnd() { return &m_listCtrl; }


// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEclipseHostBookmarks)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CEclipseHostBookmarks();

	// Generated message map functions
protected:
	//{{AFX_MSG(CEclipseHostBookmarks)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ECLIPSEHOSTBOOKMARKS_H__0BDCDA6B_6AB4_409E_8059_2C1411C45AF6__INCLUDED_)
