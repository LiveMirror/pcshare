// ListBoxSampleView.h : interface of the CListBoxSampleView class
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

#if !defined(AFX_LISTBOXSAMPLEVIEW_H__2E66D584_07E4_4F62_BA0B_62EA6AD029FF__INCLUDED_)
#define AFX_LISTBOXSAMPLEVIEW_H__2E66D584_07E4_4F62_BA0B_62EA6AD029FF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CXTPControlStatic;
class CXTPControlListBox;

class CListBoxSampleView : public CEditView
{
protected: // create from serialization only
	CListBoxSampleView();
	DECLARE_DYNCREATE(CListBoxSampleView)

// Attributes
public:
	CListBoxSampleDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CListBoxSampleView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL


	CXTPControlStatic* FindInfoControl(CXTPControlListBox* pControlListBox);

	afx_msg void OnEditUndo(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnListBoxControlSelChange(NMHDR* pNMHDR, LRESULT* pRes);
	afx_msg void OnListBoxControlPoup(NMHDR* pNMHDR, LRESULT* pRes);

// Implementation
public:
	virtual ~CListBoxSampleView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CListBoxSampleView)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in ListBoxSampleView.cpp
inline CListBoxSampleDoc* CListBoxSampleView::GetDocument()
	{ return (CListBoxSampleDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LISTBOXSAMPLEVIEW_H__2E66D584_07E4_4F62_BA0B_62EA6AD029FF__INCLUDED_)
