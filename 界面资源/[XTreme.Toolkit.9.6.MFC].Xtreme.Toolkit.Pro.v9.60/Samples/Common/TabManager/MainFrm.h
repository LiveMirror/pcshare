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

#if !defined(AFX_MAINFRM_H__EFA85F2A_8033_4021_81D2_F18E86D9F901__INCLUDED_)
#define AFX_MAINFRM_H__EFA85F2A_8033_4021_81D2_F18E86D9F901__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PropertiesDialog.h"

class CMainFrame : public CXTPMDIFrameWnd
{
	DECLARE_DYNAMIC(CMainFrame)
public:
	CMainFrame();

// Attributes
public:
	CXTPTabClientWnd m_MTIClientWnd;
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
	CXTPStatusBar  m_wndStatusBar;
	CPropertiesDialog m_wndProperties;
	CEdit m_wndPanes[7];
	void OnThemeChanged();

// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnViewThemeOffice2000();
	afx_msg void OnViewThemeOfficexp();
	afx_msg void OnViewThemeOffice2003();
	afx_msg void OnViewThemeNativexp();
	afx_msg void OnViewThemeWhidbey();
	afx_msg void OnUpdateViewThemeOffice2000(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewThemeOfficexp(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewThemeOffice2003(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewThemeNativexp(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewThemeWhidbey(CCmdUI* pCmdUI);
	afx_msg void OnClose();
	afx_msg void OnSwitchTheme();
	//}}AFX_MSG

	void OnSetPreviewMode(BOOL bPreview, CPrintPreviewState* pState);
	afx_msg void OnCustomize();
	afx_msg LRESULT OnDockingPaneNotify(WPARAM wParam, LPARAM lParam);
	afx_msg int OnCreateControl(LPCREATECONTROLSTRUCT lpCreateControl);

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__EFA85F2A_8033_4021_81D2_F18E86D9F901__INCLUDED_)
