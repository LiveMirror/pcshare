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

#if !defined(AFX_MAINFRM_H__23C539D4_D35C_4487_A4DE_CF7DEACA0909__INCLUDED_)
#define AFX_MAINFRM_H__23C539D4_D35C_4487_A4DE_CF7DEACA0909__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CMainFrame : public CXTPFrameWnd
{

protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Attributes
public:
	CXTSplitterWndEx m_wndSplitter;

	CXTPTaskPanel m_wndTaskPanel;

// Operations
public:
	BOOL CreateTaskPanel();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
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

	void SetTheme(XTPPaintTheme commandBarsTheme, XTPTaskPanelPaintTheme taskPanelTheme);

// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnThemeOffice2000();
	afx_msg void OnUpdateThemeOffice2000(CCmdUI* pCmdUI);
	afx_msg void OnThemeOffice2003();
	afx_msg void OnUpdateThemeOffice2003(CCmdUI* pCmdUI);
	afx_msg void OnThemeNativewinxp();
	afx_msg void OnUpdateThemeNativewinxp(CCmdUI* pCmdUI);
	afx_msg void OnThemeOffice2000plain();
	afx_msg void OnUpdateThemeOffice2000plain(CCmdUI* pCmdUI);
	afx_msg void OnThemeOfficexpplain();
	afx_msg void OnUpdateThemeOfficexpplain(CCmdUI* pCmdUI);
	afx_msg void OnThemeOffice2003plain();
	afx_msg void OnUpdateThemeOffice2003plain(CCmdUI* pCmdUI);
	afx_msg void OnThemeNativewinxpplain();
	afx_msg void OnUpdateThemeNativewinxpplain(CCmdUI* pCmdUI);
	afx_msg void OnTaskpanelAnimation();
	afx_msg void OnUpdateTaskpanelAnimation(CCmdUI* pCmdUI);
	afx_msg void OnViewSearch();
	afx_msg void OnThemeCustom();
	afx_msg void OnClose();
	//}}AFX_MSG
	afx_msg void OnCustomize();

	afx_msg LRESULT OnTaskPanelNotify(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__23C539D4_D35C_4487_A4DE_CF7DEACA0909__INCLUDED_)
