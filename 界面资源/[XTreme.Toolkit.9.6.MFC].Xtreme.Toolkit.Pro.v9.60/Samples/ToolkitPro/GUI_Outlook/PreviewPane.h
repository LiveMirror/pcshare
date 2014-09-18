// PreviewPane.h : header file
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

#if !defined(AFX_PREVIEWPANE_H__1D0FB113_4A25_11D4_82DA_D868EA325637__INCLUDED_)
#define AFX_PREVIEWPANE_H__1D0FB113_4A25_11D4_82DA_D868EA325637__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

/////////////////////////////////////////////////////////////////////////////
// CPreviewEdit

class CPreviewEdit : public CRichEditCtrl
{
public:
	CPreviewEdit() {}
	virtual ~CPreviewEdit() {}

	//{{AFX_VIRTUAL(CPreviewEdit)
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CPreviewEdit)
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CPreviewPane view

class CPreviewPane : public CStaticFrameWnd<CView>
{
protected:
	CPreviewPane();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CPreviewPane)

// Attributes
public:

	CString       m_strTitle1;
	CString       m_strTitle2;
	CString       m_strDescp1;
	CString       m_strDescp2;
	CString       m_strViewMessage;
	CRect         m_rcCaption;
	CRect         m_rcView;
	BOOL          m_bMax;
	COLORREF      m_clrViewMessage;
	CPreviewEdit  m_richEditCtrl;

// Operations
public:
	void ResizeEditCtrl();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPreviewPane)
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CPreviewPane();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CPreviewPane)
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PREVIEWPANE_H__1D0FB113_4A25_11D4_82DA_D868EA325637__INCLUDED_)
