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

#if !defined(AFX_MAINFRM_H__9DAEF6A8_0CAE_449C_BB70_0610A17BA268__INCLUDED_)
#define AFX_MAINFRM_H__9DAEF6A8_0CAE_449C_BB70_0610A17BA268__INCLUDED_

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

	CMap<UINT,UINT, CWnd* , CWnd*> m_mapPanes;

	int m_nCount;

	CXTPDockingPane* m_pPopupPane;

	BOOL m_bContextMenu;

	BOOL m_bForbidEditClosing;
	BOOL m_bForbidFormClosing;
	BOOL m_bForbidListClosing;

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

// Generated message map functions

	CXTPDockingPane* CreatePane(CRuntimeClass* pNewViewClass, CString strFormat, XTPDockingPaneDirection direction, CXTPDockingPane* pNeighbour = NULL) ;

public:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnManagerCreateform();
	afx_msg LRESULT OnDockingPaneNotify(WPARAM wParam, LPARAM lParam);
	afx_msg void OnManagerCreateedit();
	afx_msg void OnManagerCreatelistcontrol();
	afx_msg void OnManagerCreate3inrow();
	afx_msg void OnManagerCreate3incolumn();
	afx_msg void OnManagerCreate3inwindow();
	afx_msg void OnManagerHideall();
	afx_msg void OnManagerShowall();
	afx_msg void OnManagerDestoyall();
	afx_msg void OnUpdateManagerAll(CCmdUI* pCmdUI);
	afx_msg void OnPopupClose();
	afx_msg void OnPopupDestroy();
	afx_msg void OnPopupHide();
	afx_msg void OnUpdatePopupHide(CCmdUI* pCmdUI);
	afx_msg void OnToolsAllowcontextmenu();
	afx_msg void OnUpdateToolsAllowcontextmenu(CCmdUI* pCmdUI);
	afx_msg void OnToolsForbideditclosing();
	afx_msg void OnUpdateToolsForbideditclosing(CCmdUI* pCmdUI);
	afx_msg void OnToolsForbidformclosing();
	afx_msg void OnUpdateToolsForbidformclosing(CCmdUI* pCmdUI);
	afx_msg void OnToolsForbidlistclosing();
	afx_msg void OnUpdateToolsForbidlistclosing(CCmdUI* pCmdUI);
	afx_msg void OnToolsThemeOffice2000();
	afx_msg void OnToolsThemeOffice2003();
	afx_msg void OnToolsThemeOfficexp();
	afx_msg void OnUpdateToolsThemeOffice2000(CCmdUI* pCmdUI);
	afx_msg void OnUpdateToolsThemeOffice2003(CCmdUI* pCmdUI);
	afx_msg void OnUpdateToolsThemeOfficexp(CCmdUI* pCmdUI);
	//}}AFX_MSG
#ifdef _XTP_INCLUDE_COMMANDBARS
	afx_msg void OnClose();
	afx_msg void OnCustomize();
#endif
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__9DAEF6A8_0CAE_449C_BB70_0610A17BA268__INCLUDED_)
