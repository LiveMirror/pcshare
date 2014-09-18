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

#if !defined(AFX_MAINFRM_H__7E0A3FCA_D127_49DC_B5A6_E3CEE804794A__INCLUDED_)
#define AFX_MAINFRM_H__7E0A3FCA_D127_49DC_B5A6_E3CEE804794A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

enum Borders {borderTop = 1, borderLeft = 2, borderRight = 4, borderBottom = 8, borderInsideVert = 16, borderInsideHorz = 32,
	borderOutside = borderBottom | borderTop | borderLeft | borderRight,
	borderInside = borderInsideVert | borderInsideHorz,
	borderAll = borderInside | borderOutside};


class CMainFrame : public CXTPMDIFrameWnd
{
	DECLARE_DYNAMIC(CMainFrame)
public:
	CMainFrame();

// Attributes
public:
	DWORD m_dwBorders;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

	void SwitchBorders(DWORD dwState);
// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CXTPStatusBar  m_wndStatusBar;
	CToolBar    m_wndToolBar;

// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnThemesOffice2000();
	afx_msg void OnUpdateThemesOffice2000(CCmdUI* pCmdUI);
	afx_msg void OnThemesOfficexp();
	afx_msg void OnUpdateThemesOfficexp(CCmdUI* pCmdUI);
	afx_msg void OnThemesOffice2003();
	afx_msg void OnUpdateThemesOffice2003(CCmdUI* pCmdUI);
	afx_msg void OnThemesNativexptheme();
	afx_msg void OnUpdateThemesNativexptheme(CCmdUI* pCmdUI);
	afx_msg void OnClose();
	//}}AFX_MSG
	afx_msg int OnCreateCommandBar(LPCREATEBARSTRUCT lpCreatePopup);
	afx_msg int OnCreateControl(LPCREATECONTROLSTRUCT lpCreateControl);
	afx_msg void OnBorders(UINT nID);
	afx_msg void OnUpdateBorders(CCmdUI* pCmdUI);
	afx_msg void OnCustomize();

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__7E0A3FCA_D127_49DC_B5A6_E3CEE804794A__INCLUDED_)
