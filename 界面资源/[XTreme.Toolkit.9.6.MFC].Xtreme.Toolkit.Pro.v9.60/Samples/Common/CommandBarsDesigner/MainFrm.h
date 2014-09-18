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

#if !defined(AFX_MAINFRM_H__7AAA87E0_7A85_4ECD_83FF_E7325A68E317__INCLUDED_)
#define AFX_MAINFRM_H__7AAA87E0_7A85_4ECD_83FF_E7325A68E317__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PaneToolbars.h"
#include "PaneProperties.h"
#include "PaneControls.h"
#include "PaneAccelerators.h"
#include "PaneIcons.h"

#include "EmbeddedFrame.h"

class CMainFrame : public CXTPMDIFrameWnd
{
	DECLARE_DYNAMIC(CMainFrame)
public:
	CMainFrame();

// Attributes
public:
	CXTPTabClientWnd m_MTIClientWnd;
	CXTPDockingPaneManager m_paneManager;

	CPaneToolbars m_paneToolbars;
	CPaneProperties m_paneProperties;
	CPaneControls m_paneControls;
	CPaneAccelerators m_paneAccelerators;
	CPaneIcons m_paneIcons;


	CXTPCommandBar* m_pActiveCommandBar;
	CXTPCommandBars* m_pActiveCommandBars;

// Operations
public:
	void RefreshPanes(BOOL bForceAll = FALSE);
	CXTPCommandBars* GetActiveCommandBars();
	CEmbeddedFrame* GetActiveEmbeddedFrame();

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

// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnClose();
	afx_msg void OnViewProperties();
	afx_msg void OnViewAccelerators();
	afx_msg void OnViewControlspane();
	afx_msg void OnViewIconspane();
	afx_msg void OnViewToolbarspane();
	afx_msg void OnFileImportXmldocument();
	afx_msg void OnUpdateFileImportXmldocument(CCmdUI* pCmdUI);
	//}}AFX_MSG
	afx_msg LRESULT OnDockingPaneNotify(WPARAM wParam, LPARAM lParam);
	afx_msg void OnCustomize();

	void OnShowEmbeddedCommands(CCmdUI* pCmdUI);

	DECLARE_MESSAGE_MAP()
};

inline CMainFrame* GetMainFrame() {
	return (CMainFrame*)AfxGetMainWnd();
}


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__7AAA87E0_7A85_4ECD_83FF_E7325A68E317__INCLUDED_)
