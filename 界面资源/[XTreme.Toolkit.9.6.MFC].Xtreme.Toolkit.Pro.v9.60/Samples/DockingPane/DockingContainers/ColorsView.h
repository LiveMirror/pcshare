// ColorsView.h: interface for the CColorsView class.
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

#if !defined(AFX_COLORSVIEW_H__61D80AFE_737D_4CB2_A464_730390F83629__INCLUDED_)
#define AFX_COLORSVIEW_H__61D80AFE_737D_4CB2_A464_730390F83629__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PaneView.h"

/////////////////////////////////////////////////////////////////////////////
// CColorsSwatchWnd window

class CColorsSwatchWnd : public CStatic
{
// Construction
public:
	CColorsSwatchWnd();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CColorsSwatchWnd)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CColorsSwatchWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CColorsSwatchWnd)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////


class CColorsView : public CPaneView
{
public:
	CColorsView();
	virtual ~CColorsView();

	CWnd* OnCreateView();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CColorsView)
	//}}AFX_VIRTUAL

	CColorsSwatchWnd m_wndColors;

	//{{AFX_MSG(CColorsView)
	afx_msg void OnColorsCommand(UINT);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

#endif // !defined(AFX_COLORSVIEW_H__61D80AFE_737D_4CB2_A464_730390F83629__INCLUDED_)
