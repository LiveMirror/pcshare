// CustomThemesView.h : interface of the CCustomThemesView class
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

#if !defined(AFX_CUSTOMTHEMESVIEW_H__C2E22812_ABF7_47C9_86ED_E3020F897A2E__INCLUDED_)
#define AFX_CUSTOMTHEMESVIEW_H__C2E22812_ABF7_47C9_86ED_E3020F897A2E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CCustomThemesView : public CRichEditView
{
protected: // create from serialization only
	CCustomThemesView();
	DECLARE_DYNCREATE(CCustomThemesView)

// Attributes
public:
	CCustomThemesDoc* GetDocument();

	BOOL m_bCheck;
	COLORREF m_clr, m_clrBack;


// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCustomThemesView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);

	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CCustomThemesView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CCustomThemesView)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	afx_msg void OnToolsHex();
	afx_msg void OnUpdateToolsHex(CCmdUI *pCmdUI);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);

	afx_msg void OnUpdateSelectorText(CCmdUI* pCmd);
	afx_msg void OnSelectorText(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnUpdateText(CCmdUI* pCmd);
	afx_msg void OnButtonText();

	afx_msg void OnUpdateSelectorBack(CCmdUI* pCmd);
	afx_msg void OnSelectorBack(NMHDR* pNMHDR, LRESULT* pResult);

	afx_msg void OnUpdateBack(CCmdUI* pCmd);
	afx_msg void OnButtonBack();

	afx_msg void OnTextAuto();
	afx_msg void OnUpdateTextAuto(CCmdUI* pCmd);

	afx_msg void OnBackNone();
	afx_msg void OnUpdateBackNone(CCmdUI* pCmd);

	afx_msg void OnTextMore();
	afx_msg void OnBackMore();

	afx_msg void OnEditFind(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnUpdateEditFind(CCmdUI* pCmd);


	afx_msg void OnSetCharFormat(NMHDR* pNMHDR, LRESULT* pRes);
	afx_msg void OnUpdateComboFont(CCmdUI* pCmd);


	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in CustomThemesView.cpp
inline CCustomThemesDoc* CCustomThemesView::GetDocument()
	{ return (CCustomThemesDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CUSTOMTHEMESVIEW_H__C2E22812_ABF7_47C9_86ED_E3020F897A2E__INCLUDED_)
