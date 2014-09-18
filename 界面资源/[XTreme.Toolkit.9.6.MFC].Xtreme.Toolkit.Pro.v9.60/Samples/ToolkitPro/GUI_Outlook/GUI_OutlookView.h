// GUI_OutlookView.h : interface of the CGUI_OutlookView class
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

#if !defined(AFX_GUI_OUTLOOKVIEW_H__484D02ED_732C_445C_88DC_555072C3680A__INCLUDED_)
#define AFX_GUI_OUTLOOKVIEW_H__484D02ED_732C_445C_88DC_555072C3680A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// class forwards
class CGUI_OutlookDoc;

class CGUI_OutlookView : public CStaticFrameWnd<CXTListView>
{
protected: // create from serialization only
	CGUI_OutlookView();
	DECLARE_DYNCREATE(CGUI_OutlookView)

// Attributes
public:
	CGUI_OutlookDoc* GetDocument();

// Operations
public:
	CImageList  m_imageList;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGUI_OutlookView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

	virtual bool SortList(int nCol, bool bAscending);
	void InitializeHeaderControl();
	void InitializeListControl();
	void SetRedrawEx(BOOL bRedraw);

// Implementation
public:
	virtual ~CGUI_OutlookView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CGUI_OutlookView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
	afx_msg void OnFilePrintPreview();
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in GUI_OutlookView.cpp
inline CGUI_OutlookDoc* CGUI_OutlookView::GetDocument()
	{ return (CGUI_OutlookDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GUI_OUTLOOKVIEW_H__484D02ED_732C_445C_88DC_555072C3680A__INCLUDED_)
