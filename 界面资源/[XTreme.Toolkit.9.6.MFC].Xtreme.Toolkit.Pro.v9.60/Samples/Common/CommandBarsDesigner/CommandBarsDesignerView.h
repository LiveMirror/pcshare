// CommandBarsDesignerView.h : interface of the CCommandBarsDesignerView class
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

#if !defined(AFX_COMMANDBARSDESIGNERVIEW_H__CF86FF0B_B006_4C6A_B760_0DEF6028053F__INCLUDED_)
#define AFX_COMMANDBARSDESIGNERVIEW_H__CF86FF0B_B006_4C6A_B760_0DEF6028053F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CEmbeddedFrame;

#include "CommandBarsDesignerDesignerView.h"
#include "CommandBarsDesignerEditView.h"



class CCommandBarsDesignerDoc;

class CCommandBarsDesignerView : public CView
{
protected: // create from serialization only
	CCommandBarsDesignerView();
	DECLARE_DYNCREATE(CCommandBarsDesignerView)

	void OnUpdateFrameTitle();
	void Serialize(CArchive& ar);

// Attributes
public:
	CCommandBarsDesignerDoc* GetDocument();

	CEmbeddedFrame* GetEmbeddedFrame();

	CXTPTabControl m_wndTabControl;

	BOOL AddView(CRuntimeClass* pViewClass, LPCTSTR lpszTitle, int nIcon);
	void UpdateDocTitle();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCommandBarsDesignerView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CCommandBarsDesignerView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	void OnInitialUpdate();
protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CCommandBarsDesignerView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	afx_msg void OnSelectedChanged(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in CommandBarsDesignerView.cpp
inline CCommandBarsDesignerDoc* CCommandBarsDesignerView::GetDocument()
	{ return (CCommandBarsDesignerDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COMMANDBARSDESIGNERVIEW_H__CF86FF0B_B006_4C6A_B760_0DEF6028053F__INCLUDED_)
