// IconsView.h : header file
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

#if !defined(AFX_ICONSVIEW_H__2F11BA39_DA1B_407C_B4A5_09487EE6249F__INCLUDED_)
#define AFX_ICONSVIEW_H__2F11BA39_DA1B_407C_B4A5_09487EE6249F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PaneView.h"
/////////////////////////////////////////////////////////////////////////////
// CIconsView window

class CIconsView : public CPaneView
{
// Construction
public:
	CIconsView();

// Attributes
public:
	CListCtrl m_wndIcons;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIconsView)
	//}}AFX_VIRTUAL
	CWnd* OnCreateView();

// Implementation
public:
	virtual ~CIconsView();

	// Generated message map functions
protected:
	//{{AFX_MSG(CIconsView)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ICONSVIEW_H__2F11BA39_DA1B_407C_B4A5_09487EE6249F__INCLUDED_)
