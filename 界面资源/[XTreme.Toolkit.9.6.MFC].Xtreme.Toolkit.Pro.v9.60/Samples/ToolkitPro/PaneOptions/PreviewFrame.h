// PreviewFrame.h : header file
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

#if !defined(AFX_PREVIEWFRAME_H__A61FB103_CC13_4E95_BEAB_6E1C19ADC196__INCLUDED_)
#define AFX_PREVIEWFRAME_H__A61FB103_CC13_4E95_BEAB_6E1C19ADC196__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CPreviewFrame frame

class CPreviewFrame : public CFrameWnd
{
	DECLARE_DYNCREATE(CPreviewFrame)
protected:
	CPreviewFrame();           // protected constructor used by dynamic creation

// Attributes
public:
	CXTPDockingPaneManager m_paneManager;

	CXTSplitterWnd m_wndSplitter;

	CEdit m_wndPane;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPreviewFrame)
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	//}}AFX_VIRTUAL

	LRESULT OnDockingPaneNotify(WPARAM wParam, LPARAM lParam);

// Implementation
protected:
	virtual ~CPreviewFrame();

	// Generated message map functions
	//{{AFX_MSG(CPreviewFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PREVIEWFRAME_H__A61FB103_CC13_4E95_BEAB_6E1C19ADC196__INCLUDED_)
