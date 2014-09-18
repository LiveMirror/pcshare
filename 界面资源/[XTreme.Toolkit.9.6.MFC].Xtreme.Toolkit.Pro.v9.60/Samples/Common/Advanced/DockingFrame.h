// DockingFrame.h : header file
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

#if !defined(AFX_DOCKINGFRAME_H__87AC066C_12F8_47DB_8D4F_54BCF6559D31__INCLUDED_)
#define AFX_DOCKINGFRAME_H__87AC066C_12F8_47DB_8D4F_54BCF6559D31__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "DockingView.h"
#include "HeroesView.h"
/////////////////////////////////////////////////////////////////////////////
// CDockingFrame frame

class CDockingFrame : public CXTPFrameWnd
{
	DECLARE_DYNCREATE(CDockingFrame)
protected:
	CDockingFrame();           // protected constructor used by dynamic creation

// Attributes
public:
	CNoFlickerWnd<CListBox> m_wndView;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDockingFrame)
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CDockingFrame();

	// Generated message map functions
	//{{AFX_MSG(CDockingFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnButtonInsert();
	afx_msg void OnButtonClear();
	afx_msg void OnButtonDelete();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DOCKINGFRAME_H__87AC066C_12F8_47DB_8D4F_54BCF6559D31__INCLUDED_)
