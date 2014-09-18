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

#if !defined(AFX_MAINFRM_H__6E2E3717_7233_437A_BFD5_69F290ADB4E5__INCLUDED_)
#define AFX_MAINFRM_H__6E2E3717_7233_437A_BFD5_69F290ADB4E5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "EclipseHost.h"
#include "EclipseHostTasks.h"
#include "EclipseHostNavigator.h"
#include "EclipseHostOutline.h"
#include "EclipseHostBookMarks.h"

class CMainFrame : public CXTPMDIFrameWnd
{
	DECLARE_DYNAMIC(CMainFrame)
public:
	CMainFrame();

// Attributes
public:
	static void AddIcons(UINT nID);
	void OnSetPreviewMode(BOOL bPreview, CPrintPreviewState* pState);


// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	void OnThemeChanged();

protected:  // control bar embedded members
	CXTPStatusBar  m_wndStatusBar;
	CXTPDockingPaneManager m_paneManager;

	CXTPTabClientWnd m_MTIClientWnd;
	CImageList m_ilIcons;

	CEclipseHost m_wndPanes[7];
	CEclipseHostTasks m_wndTasks;
	CEclipseHostNavigator m_wndNavigator;
	CEclipseHostOutline m_wndOutline;
	CEclipseHostBookmarks m_wndBookmarks;

// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnEmptyCommand();
	afx_msg void OnViewBasethemeOffice2000();
	afx_msg void OnViewBasethemeOfficexp();
	afx_msg void OnViewBasethemeOffice2003();
	afx_msg void OnClose();
	//}}AFX_MSG
	afx_msg void OnCustomize();
	afx_msg int OnCreateControl(LPCREATECONTROLSTRUCT lpCreateControl);
	afx_msg LRESULT OnDockingPaneNotify(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__6E2E3717_7233_437A_BFD5_69F290ADB4E5__INCLUDED_)
