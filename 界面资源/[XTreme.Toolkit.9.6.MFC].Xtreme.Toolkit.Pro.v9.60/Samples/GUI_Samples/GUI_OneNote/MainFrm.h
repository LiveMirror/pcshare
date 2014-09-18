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

#if !defined(AFX_MAINFRM_H__9E34D89A_10B1_4395_BF74_0FDB0C4F45BF__INCLUDED_)
#define AFX_MAINFRM_H__9E34D89A_10B1_4395_BF74_0FDB0C4F45BF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PaneNew.h"
#include "PaneSearch.h"

#ifdef _XTP_INCLUDE_CONTROLS

class CButtonEx : public CButton
{
public:
	CButtonEx()
	{
		m_pItem = NULL;
	}

	inline void SetItem(CXTPTaskPanelGroupItem* pItem) {m_pItem = pItem;}

protected:

	CXTPTaskPanelGroupItem* m_pItem;
	COLORREF m_clrBack;
	CBrush m_brushBack;

	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	afx_msg void OnAutoRun();

	DECLARE_MESSAGE_MAP()
};

#endif

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

protected:  // control bar embedded members
	CXTPStatusBar  m_wndStatusBar;

#ifdef _XTP_INCLUDE_DOCKINGPANE
	CXTPDockingPaneManager m_paneManager;
#endif
	CXTPTabClientWnd m_MTIClientWnd;

	CPaneNew m_paneNew;
	CPaneSearch m_paneSeaarch;

#ifdef _XTP_INCLUDE_CONTROLS
	CXTPTaskPanel m_wndTaskPanel;
	void CreateTaskPanel();
	CImageList m_ilTaskIcons;
	CButtonEx m_wndButtonAutoRun;
#endif;

	afx_msg int OnCreateCommandBar(LPCREATEBARSTRUCT lpCreatePopup);
	afx_msg int OnCreateControl(LPCREATECONTROLSTRUCT lpCreateControl);
	afx_msg void OnCustomize();
	afx_msg LRESULT OnDockingPaneNotify(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnTabbarMouseMsg(WPARAM wParam,LPARAM lParam);

	void OnSetPreviewMode(BOOL bPreview, CPrintPreviewState* pState);

// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnClose();
	afx_msg void OnViewTaskPane();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__9E34D89A_10B1_4395_BF74_0FDB0C4F45BF__INCLUDED_)
