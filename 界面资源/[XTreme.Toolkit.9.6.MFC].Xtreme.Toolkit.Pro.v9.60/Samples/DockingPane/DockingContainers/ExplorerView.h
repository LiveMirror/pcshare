// ExplorerView.h : header file
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

#if !defined(AFX_EXPLORERVIEW_H__38375D16_655F_4C83_BDEC_60CC1986A706__INCLUDED_)
#define AFX_EXPLORERVIEW_H__38375D16_655F_4C83_BDEC_60CC1986A706__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PaneView.h"

/////////////////////////////////////////////////////////////////////////////
// CExplorerView window

class CExplorerView : public CPaneView
{
// Construction
public:
	CExplorerView();

// Attributes
public:
	CTreeCtrl   m_wndExplorer;

// Operations
public:
	CWnd* OnCreateView();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExplorerView)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CExplorerView();

	// Generated message map functions
protected:
	//{{AFX_MSG(CExplorerView)
	afx_msg void OnExplorerCommand(UINT);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EXPLORERVIEW_H__38375D16_655F_4C83_BDEC_60CC1986A706__INCLUDED_)
