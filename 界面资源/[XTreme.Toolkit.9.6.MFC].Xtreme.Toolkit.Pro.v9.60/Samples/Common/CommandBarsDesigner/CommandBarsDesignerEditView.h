// CommandBarsDesignerEditView.h : header file
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

#if !defined(AFX_COMMANDBARSDESIGNEREDITVIEW_H__63EBEC96_8A3C_430A_8D39_344A9B166C97__INCLUDED_)
#define AFX_COMMANDBARSDESIGNEREDITVIEW_H__63EBEC96_8A3C_430A_8D39_344A9B166C97__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "afxrich.h"
/////////////////////////////////////////////////////////////////////////////
// CCommandBarsDesignerEditView view

#define CEditViewBase CEditView

class CCommandBarsDesignerEditView : public CEditViewBase
{
protected:
	CCommandBarsDesignerEditView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CCommandBarsDesignerEditView)

// Attributes
public:
	CFont m_fnt;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCommandBarsDesignerEditView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CCommandBarsDesignerEditView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CCommandBarsDesignerEditView)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COMMANDBARSDESIGNEREDITVIEW_H__63EBEC96_8A3C_430A_8D39_344A9B166C97__INCLUDED_)
