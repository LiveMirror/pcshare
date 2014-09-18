// ToolsView.h: interface for the CToolsView class.
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

#if !defined(AFX_TOOLSVIEW_H__13CC4D7C_B850_4205_9C22_3C58C58CC400__INCLUDED_)
#define AFX_TOOLSVIEW_H__13CC4D7C_B850_4205_9C22_3C58C58CC400__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PaneView.h"




/////////////////////////////////////////////////////////////////////////////
// CToolsNavigateWnd window

class CToolsNavigateWnd : public CStatic
{
// Construction
public:
	CToolsNavigateWnd();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CToolsNavigateWnd)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CToolsNavigateWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CToolsNavigateWnd)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};


class CToolsView : public CPaneView
{
public:
	void Refresh();
	CToolsView();
	virtual ~CToolsView();

	CWnd* OnCreateView();

	UINT m_nCurrentTool;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CToolsView)
	//}}AFX_VIRTUAL

	CToolsNavigateWnd m_wndNavigateWnd;

	//{{AFX_MSG(CToolsView)
	afx_msg void OnToolsCommand(UINT);
	void OnUpdateToolsCommand(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

#endif // !defined(AFX_TOOLSVIEW_H__13CC4D7C_B850_4205_9C22_3C58C58CC400__INCLUDED_)
