// PopoutFrame.h : header file
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

#if !defined(AFX_PopoutFrame_H__C1A1EB05_D687_481E_AB88_E7CF6F0F4051__INCLUDED_)
#define AFX_PopoutFrame_H__C1A1EB05_D687_481E_AB88_E7CF6F0F4051__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ChildView.h"

/////////////////////////////////////////////////////////////////////////////
// CPopoutFrame frame

class CPopoutFrame : public CXTPFrameWnd
{
	DECLARE_DYNCREATE(CPopoutFrame)
public:
	CPopoutFrame();           // protected constructor used by dynamic creation

public:
	CXTPDockingPaneManager m_paneManager;

// Operations
public:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPopoutFrame)
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
protected:
	CChildView    m_wndView;
	CXTPStatusBar  m_wndStatusBar;

	virtual ~CPopoutFrame();

	// Generated message map functions
	//{{AFX_MSG(CPopoutFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPopframeExit();
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PopoutFrame_H__C1A1EB05_D687_481E_AB88_E7CF6F0F4051__INCLUDED_)
