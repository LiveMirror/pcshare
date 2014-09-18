// PaneView.h : header file
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

#if !defined(AFX_PANEVIEW_H__31D830D9_23DE_4FA5_9213_3FC8E271E0CD__INCLUDED_)
#define AFX_PANEVIEW_H__31D830D9_23DE_4FA5_9213_3FC8E271E0CD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CPaneView window

#ifndef _XTP_INCLUDE_COMMANDBARS
#define CXTPToolBar CToolBar
#endif

class CPaneView : public CWnd
{
// Construction
public:
	CPaneView();

// Attributes
public:
	CWnd* m_pView;

	CListCtrl m_wndView;
	int m_nToolBarID;


	CXTPToolBar m_wndToolBar;

// Operations
public:

	virtual CWnd* CPaneView::OnCreateView();


	CWnd* CreatePane(CWnd* pParentWnd);


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPaneView)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CPaneView();

	// Generated message map functions
protected:
	//{{AFX_MSG(CPaneView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PANEVIEW_H__31D830D9_23DE_4FA5_9213_3FC8E271E0CD__INCLUDED_)
