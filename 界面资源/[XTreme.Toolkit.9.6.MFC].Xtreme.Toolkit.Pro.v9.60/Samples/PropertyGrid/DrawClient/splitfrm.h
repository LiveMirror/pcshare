// splitfrm.h : header file
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

/////////////////////////////////////////////////////////////////////////////
// CSplitFrame frame with splitter

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif



class CSplitFrame : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CSplitFrame)
protected:
	CSplitFrame();          // protected constructor used by dynamic creation

// Attributes
protected:

#ifdef _XTP_INCLUDE_CONTROLS
	CXTSplitterWnd    m_wndSplitter;
#else
	CSplitterWnd    m_wndSplitter;
#endif

	int m_nColor;
public:

// Operations
public:
	afx_msg LRESULT OnGetTabColor(WPARAM, LPARAM);

	afx_msg void OnUpdateSelectColor(CCmdUI* pCmdUI);
	afx_msg void OnSelectColor(UINT nColor);

	afx_msg void OnUpdateSelectColorNone(CCmdUI* pCmdUI);
	afx_msg void OnSelectColorNone();

// Implementation
public:
	virtual ~CSplitFrame();
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void ActivateFrame(int nCmdShow = -1);

	// Generated message map functions
	//{{AFX_MSG(CSplitFrame)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
