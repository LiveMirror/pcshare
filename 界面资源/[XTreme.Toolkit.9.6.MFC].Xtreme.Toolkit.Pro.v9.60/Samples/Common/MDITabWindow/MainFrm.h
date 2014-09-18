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

#if !defined(AFX_MAINFRM_H__64AFB86C_B332_4F73_979E_E5C92E0CCA6F__INCLUDED_)
#define AFX_MAINFRM_H__64AFB86C_B332_4F73_979E_E5C92E0CCA6F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMainFrame : public CXTPMDIFrameWnd
{
	DECLARE_DYNAMIC(CMainFrame)
public:
	CMainFrame();

// Attributes
public:
	CXTPTabClientWnd m_MTIClientWnd;

	CXTPDockingPaneManager m_paneManager;
	int m_nCount;

	CMap<UINT,UINT, CWnd* , CWnd*> m_mapPanes;

// Operations
	void ReparentAllChildren(CFrameWnd *pFrameOld, CFrameWnd* pFrameNew);
	BOOL ActivateDockableFrame(CFrameWnd* pFrameNew);
	static CFrameWnd* GetParentDockingFrame(CWnd* pWnd);

	void DockFrame(CFrameWnd* pFrameOld);
	void UnDockFrame(CFrameWnd* pFrameOld, CXTPDockingPane* pPane);

public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

	int OnCreateControl(LPCREATECONTROLSTRUCT lpCreateControl);

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

	afx_msg void OnCustomize();
	afx_msg void OnClose();
// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnViewThemeOffice2000();
	afx_msg void OnViewThemeOfficexp();
	afx_msg void OnViewThemeOffice2003();
	afx_msg void OnViewThemeNativexp();
	afx_msg void OnUpdateViewThemeOffice2000(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewThemeOfficexp(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewThemeOffice2003(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewThemeNativexp(CCmdUI* pCmdUI);
	afx_msg LRESULT OnTabbarMouseMsg(WPARAM wParam,LPARAM lParam);
	afx_msg void OnToggleDock(void);
	afx_msg void OnWindowCloseAll();
	afx_msg void OnUpdateWindowCloseAll(CCmdUI *pCmdUI);
	afx_msg void OnWindowDockable();
	afx_msg LRESULT OnDockingPaneNotify(WPARAM wParam, LPARAM lParam);
	afx_msg void OnUpdateWindowDockable(CCmdUI* pCmdUI);
	afx_msg void OnWindowHide();
	afx_msg void OnWindowHideall();
	afx_msg void OnWindowAutohide();
	afx_msg void OnUpdateWindowAutohide(CCmdUI* pCmdUI);
	afx_msg void OnUpdateWindowHideall(CCmdUI* pCmdUI);
	afx_msg void OnToggleTabs();
	afx_msg void OnUpdateToggleTabs(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__64AFB86C_B332_4F73_979E_E5C92E0CCA6F__INCLUDED_)
