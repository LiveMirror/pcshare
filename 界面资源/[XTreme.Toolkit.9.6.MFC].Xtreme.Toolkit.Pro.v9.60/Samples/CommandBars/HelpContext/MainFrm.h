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

#if !defined(AFX_MAINFRM_H__9AAFF9EE_0152_4252_A6DD_BB9B7C4205BB__INCLUDED_)
#define AFX_MAINFRM_H__9AAFF9EE_0152_4252_A6DD_BB9B7C4205BB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class COrangeTheme : public CXTPOfficeTheme
{
public:
	COrangeTheme()
	{
		RefreshMetrics();
	}

	void RefreshMetrics()
	{
		CXTPOfficeTheme::RefreshMetrics();

		INT nElements[] = {
			XPCOLOR_HIGHLIGHT, XPCOLOR_HIGHLIGHT_PUSHED,  XPCOLOR_HIGHLIGHT_CHECKED,
			XPCOLOR_HIGHLIGHT_TEXT, XPCOLOR_CHECKED_TEXT, XPCOLOR_PUSHED_TEXT,
			XPCOLOR_HIGHLIGHT_BORDER, XPCOLOR_HIGHLIGHT_PUSHED_BORDER, XPCOLOR_HIGHLIGHT_CHECKED_BORDER
		};

		COLORREF clrValues[] = {
			RGB(255, 204, 0),RGB(255, 183, 0), RGB(255, 224, 0),
			0, 0, 0,
			RGB(255, 153, 0), RGB(255, 153, 0), RGB(255, 153, 0)
		};

		SetColors(sizeof(nElements)/sizeof(INT), nElements, clrValues);
	}
};



class CMainFrame : public CXTPMDIFrameWnd
{
	DECLARE_DYNAMIC(CMainFrame)
public:
	CMainFrame();

// Attributes
public:
	int m_nIDControlTracking;

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
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnClose();
	//}}AFX_MSG
	afx_msg void OnCustomize();
	LRESULT OnControlSelected(WPARAM wParam, LPARAM);
	LRESULT OnCommandHelp(WPARAM wParam, LPARAM);

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__9AAFF9EE_0152_4252_A6DD_BB9B7C4205BB__INCLUDED_)
