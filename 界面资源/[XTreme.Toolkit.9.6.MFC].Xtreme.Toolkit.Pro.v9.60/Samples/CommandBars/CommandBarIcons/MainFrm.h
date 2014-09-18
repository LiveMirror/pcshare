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

#if !defined(AFX_MAINFRM_H__874D37D9_4899_404C_95AD_8463196C173C__INCLUDED_)
#define AFX_MAINFRM_H__874D37D9_4899_404C_95AD_8463196C173C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "PropertiesDialog.h"

class CMainFrame : public CXTPFrameWnd
{

protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Attributes
public:

	CPropertiesDialog m_wndProperties ;

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
	CToolBar    m_wndToolBar;

// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnClose();
	//}}AFX_MSG
	afx_msg void OnCustomize();

	afx_msg void OnEmptyCommand();
	afx_msg void OnUpdateProperties(CCmdUI* pCmdUI);
	afx_msg void OnProperties();

	void SwitchTheme(XTPPaintTheme paintTheme);

	afx_msg void OnSwitchOffice2000();
	afx_msg void OnSwitchOfficeXP();
	afx_msg void OnSwitchOffice2003();
	afx_msg void OnSwitchNativeXP();

	afx_msg void OnUpdateSwitchOffice2000(CCmdUI* pCmdUI);
	afx_msg void OnUpdateSwitchOfficeXP(CCmdUI* pCmdUI);
	afx_msg void OnUpdateSwitchOffice2003(CCmdUI* pCmdUI);
	afx_msg void OnUpdateSwitchNativeXP(CCmdUI* pCmdUI);

	afx_msg int OnCreateControl(LPCREATECONTROLSTRUCT lpCreateControl);


	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__874D37D9_4899_404C_95AD_8463196C173C__INCLUDED_)
