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

#if !defined(AFX_MAINFRM_H__65CF0C32_0558_487A_A456_44C2205DB67A__INCLUDED_)
#define AFX_MAINFRM_H__65CF0C32_0558_487A_A456_44C2205DB67A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// class CControlStatic is used to create a custom static
// control that displays the number of open child windows
// in the application.
class CControlStatic : public CXTPControl
{
	DECLARE_XTP_CONTROL(CControlStatic)
protected:
	virtual CSize GetSize(CDC* pDC);
	virtual void Draw(CDC* pDC);
};

// class CButtonOptions is used to create the
// radio buttons on the Options toolbar.
class CButtonOptions : public CButton
{
protected:
	void OnOptionShow();

protected:
	DECLARE_MESSAGE_MAP()
};

class CMainFrame : public CXTPMDIFrameWnd
{
	DECLARE_DYNAMIC(CMainFrame)
public:
	CMainFrame();

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
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	void OnChangeFileMenu(CMenu* pPopupMenu);
	int FindMenuItem(CMenu* pMenu, UINT nID);

protected:  // control bar embedded members
	CXTPStatusBar  m_wndStatusBar; // Standard status bar
	CToolBar       m_wndToolBar;   // Standard toolbar
	CMenu          m_menu;         // Standard menu

#ifdef _XTP_INCLUDE_CONTROLS
	CXTDateTimeCtrl m_monthCal;   // Calendar control
	CXTDateTimeCtrl m_date;       // Date picker control
	CXTDateTimeCtrl m_time;       // Time picker control
#else

#if _MSC_VER >= 1200 // MFC 5.0
	CDateTimeCtrl m_monthCal;    // Calendar control
	CDateTimeCtrl m_date;        // Date picker control
	CDateTimeCtrl m_time;        // Time picker control
#endif

#endif

	CAnimateCtrl  m_wndAnimCtrl; // Animation control
	CSliderCtrl   m_wndSlider;   // Slider control

	CButtonOptions m_wndOptions[3]; // Array of CButtonOptions

// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnClose();
	//}}AFX_MSG
	afx_msg void OnCustomize();  // This function is called each time a button on the Options toolbar is clicked
	afx_msg int OnCreateControl(LPCREATECONTROLSTRUCT lpCreateControl); // Called by the XTK to create the controls on the CommandBars
	afx_msg void OnUpdateButtonDoc(CCmdUI* pCmdUI); // Called by the framework to update the buttons on the Options toolbar

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__65CF0C32_0558_487A_A456_44C2205DB67A__INCLUDED_)
