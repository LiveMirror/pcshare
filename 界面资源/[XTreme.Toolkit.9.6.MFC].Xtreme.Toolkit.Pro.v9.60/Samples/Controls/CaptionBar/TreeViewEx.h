// TreeViewEx.h : interface of the CTreeViewEx class
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

#if !defined(AFX_TREEVIEWEX_H__8BD95F4F_4B40_4C99_9763_C42EA2DE8889__INCLUDED_)
#define AFX_TREEVIEWEX_H__8BD95F4F_4B40_4C99_9763_C42EA2DE8889__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CListViewExDoc;
class CTreeViewEx : public CView
{
protected: // create from serialization only
	CTreeViewEx();
	DECLARE_DYNCREATE(CTreeViewEx)

// Attributes
public:
	CListViewExDoc* GetDocument();

// Operations
public:

	CXTCaption  m_wndCaption;
	CImageList  m_ilTreeIcons;
	CXTTreeCtrl m_wndTreeCtrl;

public:

	BOOL InitializeImageLists();
	BOOL InitilalizeTreeControl();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTreeViewEx)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnInitialUpdate(); // called first time after construct
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CTreeViewEx();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CTreeViewEx)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnWindowPosChanged(WINDOWPOS FAR* lpwndpos);
	//}}AFX_MSG
	afx_msg void OnCaptButton();
	DECLARE_MESSAGE_MAP()

	friend class CMainFrame;
};

#ifndef _DEBUG  // debug version in TreeViewEx.cpp
inline CListViewExDoc* CTreeViewEx::GetDocument()
	{ return (CListViewExDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TREEVIEWEX_H__8BD95F4F_4B40_4C99_9763_C42EA2DE8889__INCLUDED_)
