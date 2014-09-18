// VirtualListView.h : interface of the CVirtualListView class
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

#if !defined(AFX_VIRTUALLISTVIEW_H__86612257_FD2F_4D33_826F_CF3546BF74CE__INCLUDED_)
#define AFX_VIRTUALLISTVIEW_H__86612257_FD2F_4D33_826F_CF3546BF74CE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000



class CVirtualListView : public CXTPReportView
{
protected: // create from serialization only
	CVirtualListView();
	DECLARE_DYNCREATE(CVirtualListView)

// Attributes
public:
	CVirtualListDoc* GetDocument();

// Operations
public:
	BOOL m_bColorize;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CVirtualListView)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CVirtualListView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CVirtualListView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnEditCopy();
	afx_msg void OnReportSetrowscount();
	afx_msg void OnReportColorize();
	afx_msg void OnUpdateReportColorize(CCmdUI* pCmdUI);
	//}}AFX_MSG
	afx_msg void OnReportItemClick(NMHDR * pNotifyStruct, LRESULT * /*result*/);
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in VirtualListView.cpp
inline CVirtualListDoc* CVirtualListView::GetDocument()
	{ return (CVirtualListDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VIRTUALLISTVIEW_H__86612257_FD2F_4D33_826F_CF3546BF74CE__INCLUDED_)
