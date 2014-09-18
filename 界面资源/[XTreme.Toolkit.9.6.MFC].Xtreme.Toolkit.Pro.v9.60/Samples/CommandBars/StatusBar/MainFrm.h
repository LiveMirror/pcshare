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

#if !defined(AFX_MAINFRM_H__4FA08078_1069_44D7_8380_47F9D64AA544__INCLUDED_)
#define AFX_MAINFRM_H__4FA08078_1069_44D7_8380_47F9D64AA544__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMainFrame : public CXTPMDIFrameWnd
{
	DECLARE_DYNAMIC(CMainFrame)
public:
	CMainFrame();
	virtual ~CMainFrame();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:

	void AddEdit();
	void RemoveEdit();

	void AddLogo();
	void RemoveLogo();

	void AddAnimation();
	void RemoveAnimation();

	void AddIcon();
	void RemoveIcon();

	void AddProgress();
	void RemoveProgress();
	void TestProgress();

	void ResizeToolbar();

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members

	CXTPStatusBar  m_wndStatusBar;
	CEdit         m_wndEditCtrl;
	CAnimateCtrl  m_wndAnimCtrl;
	CProgressCtrl m_wndProgCtrl;
	CXTPStatusBarLogoPane   m_wndLogoPane;
	CXTPStatusBarIconPane   m_wndIconPane;

	int m_nTheme;
	void SetTheme(int nTheme);

// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnAdditems();
	afx_msg void OnUpdateViewTheme(CCmdUI* pCmdUI);
	afx_msg void OnClose();
	void OnViewTheme(UINT nID);
	//}}AFX_MSG
	afx_msg void OnCustomize();
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__4FA08078_1069_44D7_8380_47F9D64AA544__INCLUDED_)
