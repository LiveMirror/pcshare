// MainFrm.h : interface of the CMainFrame class
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

#if !defined(AFX_MAINFRM_H__EC818EBF_8F75_4D49_95D9_045CF71AA9A8__INCLUDED_)
#define AFX_MAINFRM_H__EC818EBF_8F75_4D49_95D9_045CF71AA9A8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000



#ifndef _XTP_INCLUDE_COMMANDBARS
#define CXTPFrameWnd CFrameWnd
#endif

class CMainFrame : public CXTPFrameWnd
{

protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Attributes
public:
	CXTPDockingPaneManager m_paneManager;


// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CStatusBar  m_wndStatusBar;
	CToolBar    m_wndToolBar;

	CEdit m_wndPanes[4];

	BOOL m_bCancelAction[4][_countof(lpszActions)];

// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

	afx_msg void OnActionsClassviewCancel(UINT);
	afx_msg void OnUpdateActionsClassviewCancel(CCmdUI* pCmdUI);
	afx_msg void OnActionsClassviewFlags(UINT);
	afx_msg void OnUpdateActionsClassviewFlags(CCmdUI* pCmdUI);

	afx_msg void OnActionsResourceviewCancel(UINT);
	afx_msg void OnUpdateActionsResourceviewCancel(CCmdUI* pCmdUI);
	afx_msg void OnActionsResourceviewFlags(UINT);
	afx_msg void OnUpdateActionsResourceviewFlags(CCmdUI* pCmdUI);

	afx_msg void OnActionsFileviewCancel(UINT);
	afx_msg void OnUpdateActionsFileviewCancel(CCmdUI* pCmdUI);
	afx_msg void OnActionsFileviewFlags(UINT);
	afx_msg void OnUpdateActionsFileviewFlags(CCmdUI* pCmdUI);

	afx_msg void OnActionsInfoviewCancel(UINT);
	afx_msg void OnUpdateActionsInfoviewCancel(CCmdUI* pCmdUI);
	afx_msg void OnActionsInfoviewFlags(UINT);
	afx_msg void OnUpdateActionsInfoviewFlags(CCmdUI* pCmdUI);

	afx_msg void OnActionsAllCancel(UINT);
	afx_msg void OnUpdateActionsAllCancel(CCmdUI* pCmdUI);
	afx_msg void OnActionsAllFlags(UINT);
	afx_msg void OnUpdateActionsAllFlags(CCmdUI* pCmdUI);

#ifdef _XTP_INCLUDE_COMMANDBARS
	afx_msg void OnCustomize();
	afx_msg void OnClose();
#endif
	afx_msg LRESULT OnDockingPaneNotify(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__EC818EBF_8F75_4D49_95D9_045CF71AA9A8__INCLUDED_)
