// EclipseHostOutline.h : header file
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

#if !defined(AFX_ECLIPSEHOSTOUTLINE_H__C6B0F280_046B_41DA_809E_D5C28A29C67E__INCLUDED_)
#define AFX_ECLIPSEHOSTOUTLINE_H__C6B0F280_046B_41DA_809E_D5C28A29C67E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "EclipseHost.h"
/////////////////////////////////////////////////////////////////////////////
// CEclipseHostOutline window

class CEclipseHostOutline : public CEclipseHost
{
// Construction
public:
	CEclipseHostOutline();

// Attributes
public:

	CEdit m_wndEdit;
	virtual CWnd* GetClientWnd() { return &m_wndEdit; }

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEclipseHostOutline)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CEclipseHostOutline();

	// Generated message map functions
protected:
	//{{AFX_MSG(CEclipseHostOutline)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ECLIPSEHOSTOUTLINE_H__C6B0F280_046B_41DA_809E_D5C28A29C67E__INCLUDED_)
