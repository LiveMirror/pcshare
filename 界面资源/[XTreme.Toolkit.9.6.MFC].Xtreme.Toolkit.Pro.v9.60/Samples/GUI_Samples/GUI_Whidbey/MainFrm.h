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

#if !defined(AFX_MAINFRM_H__57DF813D_6BDB_4AA1_A29E_0AAA116BC4D5__INCLUDED_)
#define AFX_MAINFRM_H__57DF813D_6BDB_4AA1_A29E_0AAA116BC4D5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ClassViewPane.h"
#include "ResourceViewPane.h"
#include "SolutionExplorerPane.h"
#include "PropertiesPane.h"
#include "OutputPane.h"

class CMainFrame : public CXTPMDIFrameWnd
{
	DECLARE_DYNAMIC(CMainFrame)
public:
	CMainFrame();

// Attributes
public:
	CXTPDockingPaneManager m_paneManager;
	CXTPTabClientWnd m_MTIClientWnd;

	CClassViewPane m_wndClassView;
	CResourceViewPane m_wndResourceView;
	CSolutionExplorerPane m_wndSolutionExplorer;
	CPropertiesPane m_wndProperties;
	COutputPane m_wndOutput;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	void OnSetPreviewMode(BOOL bPreview, CPrintPreviewState* pState);
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
	afx_msg void OnWindowAutohideall();
	afx_msg void OnWindowMore();
	afx_msg void OnClose();
	//}}AFX_MSG

	LRESULT OnDockingPaneNotify(WPARAM wParam, LPARAM lParam);
	void OnShowView(UINT nID);
	void OnEmptyCommandRange(UINT);
	int OnCreateControl(LPCREATECONTROLSTRUCT lpCreateControl);
	void OnCustomize();

	void OnEditState(NMHDR* pNMHDR, LRESULT* pResult);
	void OnUpdateEditState(CCmdUI* pCmdUI);

	void OnEditPlatform(NMHDR* pNMHDR, LRESULT* pResult);
	void OnUpdateEditPlatform(CCmdUI* pCmdUI);
	
	CWnd* CreateToolBox();
	void ResetToolboxItems();

	int m_nState;
	int m_nPlatform;
	CString m_strIniFileName;

#ifdef _XTP_INCLUDE_CONTROLS
	CXTPTaskPanelGroup* CreateToolboxGroup(UINT nID);
	CXTPTaskPanel m_wndTaskPanel;
#endif


	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__57DF813D_6BDB_4AA1_A29E_0AAA116BC4D5__INCLUDED_)
