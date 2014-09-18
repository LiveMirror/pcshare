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

#if !defined(AFX_MAINFRM_H__F09AFCA0_D058_4459_A180_1F62F89ECA12__INCLUDED_)
#define AFX_MAINFRM_H__F09AFCA0_D058_4459_A180_1F62F89ECA12__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DockingPaneManager.h"

#include "ExplorerView.h"
#include "IconsView.h"
#include "ToolsView.h"
#include "ColorsView.h"

#ifndef _XTP_INCLUDE_COMMANDBARS
#define CXTPMDIFrameWnd CMDIFrameWnd
#endif

class CMainFrame : public CXTPMDIFrameWnd
{
	DECLARE_DYNAMIC(CMainFrame)
public:
	CMainFrame();

// Attributes
public:
	CDockingPaneManager m_paneManager;

	CExplorerView m_wndExplorerView;
	CIconsView m_wndIconsView;
	CColorsView m_wndColorsView;
	CToolsView m_wndToolsView;

	LRESULT OnDockingPaneNotify(WPARAM wParam, LPARAM lParam);



// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnSetPreviewMode(BOOL bPreview, CPrintPreviewState* pState);
	//}}AFX_VIRTUAL

// Implementation
public:
	void RefreshPanes();
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
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG

#ifdef _XTP_INCLUDE_COMMANDBARS
	afx_msg void OnCustomize();
	afx_msg void OnClose();
#endif

	DECLARE_MESSAGE_MAP()


};


AFX_INLINE void RefreshPanes()
{
	((CMainFrame*)AfxGetMainWnd())->RefreshPanes();
}

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__F09AFCA0_D058_4459_A180_1F62F89ECA12__INCLUDED_)
