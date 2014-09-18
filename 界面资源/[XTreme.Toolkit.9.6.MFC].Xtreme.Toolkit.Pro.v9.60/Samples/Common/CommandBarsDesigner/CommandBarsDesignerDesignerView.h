// CommandBarsDesignerDesignerView.h : header file
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

#if !defined(AFX_COMMANDBARSDESIGNERDESIGNERVIEW_H__BC5E3607_EDDC_4AA8_B796_D69BFEBE79EF__INCLUDED_)
#define AFX_COMMANDBARSDESIGNERDESIGNERVIEW_H__BC5E3607_EDDC_4AA8_B796_D69BFEBE79EF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CRectTrackerEx : public CRectTracker
{
public:
	CRectTrackerEx()
		: CRectTracker(CRect(10, 10, 310, 289) , CRectTracker::resizeOutside|CRectTracker::hatchedBorder   )
	{
		m_nHandleSize = 7;
		m_sizeMin = CSize (150, 120);
	}
	UINT GetHandleMask() const
	{
		return 32 + 4 + 64;
	}

};

class CCommandBarsDesignerDoc;
class CEmbeddedFrame;
/////////////////////////////////////////////////////////////////////////////
// CCommandBarsDesignerDesignerView view

class CCommandBarsDesignerDesignerView : public CView
{
protected: // create from serialization only
	CCommandBarsDesignerDesignerView();
	DECLARE_DYNCREATE(CCommandBarsDesignerDesignerView)

// Attributes
public:
	CCommandBarsDesignerDoc* GetDocument();
	void OnUpdateFrameTitle();

	CEmbeddedFrame* m_pFrame;
	CRectTrackerEx m_rectTracker;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCommandBarsDesignerDesignerView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInitialUpdate();
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CCommandBarsDesignerDesignerView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CCommandBarsDesignerDesignerView)
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnCommandbarsSettings();
	afx_msg void OnCommandbarsResourcesymbols();
	afx_msg void OnFileExportXmldocument();
	afx_msg void OnDesignMode();
	afx_msg void OnUpdateDesignMode(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFileExportXmldocument(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
#ifndef _DEBUG  // debug version in CommandBarsDesignerView.cpp
inline CCommandBarsDesignerDoc* CCommandBarsDesignerDesignerView::GetDocument()
	{ return (CCommandBarsDesignerDoc*)m_pDocument; }
#endif

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COMMANDBARSDESIGNERDESIGNERVIEW_H__BC5E3607_EDDC_4AA8_B796_D69BFEBE79EF__INCLUDED_)
