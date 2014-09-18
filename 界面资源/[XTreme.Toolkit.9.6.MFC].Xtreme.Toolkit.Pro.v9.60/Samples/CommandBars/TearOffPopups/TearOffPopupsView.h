// TearOffPopupsView.h : interface of the CTearOffPopupsView class
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

#if !defined(AFX_TEAROFFPOPUPSVIEW_H__6BE28777_0699_4B72_BE08_5B52749DC664__INCLUDED_)
#define AFX_TEAROFFPOPUPSVIEW_H__6BE28777_0699_4B72_BE08_5B52749DC664__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CTearOffPopupsCntrItem;

class CTearOffPopupsView : public CRichEditView
{
protected: // create from serialization only
	CTearOffPopupsView();
	DECLARE_DYNCREATE(CTearOffPopupsView)

// Attributes
public:
	CTearOffPopupsDoc* GetDocument();

	COLORREF m_clrText;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTearOffPopupsView)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void OnInitialUpdate(); // called first time after construct
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CTearOffPopupsView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CTearOffPopupsView)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	afx_msg void OnDestroy();
	//}}AFX_MSG

	afx_msg void OnTextAuto();
	afx_msg void OnUpdateTextAuto(CCmdUI* pCmd);
	afx_msg void OnTextMore();

	afx_msg void OnButtonText();
	afx_msg void OnUpdateText(CCmdUI* pCmd);

	afx_msg void OnSelectorText(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnUpdateSelectorText(CCmdUI* pCmd);

	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in TearOffPopupsView.cpp
inline CTearOffPopupsDoc* CTearOffPopupsView::GetDocument()
	{ return (CTearOffPopupsDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TEAROFFPOPUPSVIEW_H__6BE28777_0699_4B72_BE08_5B52749DC664__INCLUDED_)
