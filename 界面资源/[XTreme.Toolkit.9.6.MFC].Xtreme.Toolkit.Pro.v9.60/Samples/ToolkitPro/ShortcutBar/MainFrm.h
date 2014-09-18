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

#if !defined(AFX_MAINFRM_H__9D907276_5C73_4B62_A824_6E2EF45A3300__INCLUDED_)
#define AFX_MAINFRM_H__9D907276_5C73_4B62_A824_6E2EF45A3300__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CSplitterWndEx : public CXTSplitterWnd
{
public:
	CSplitterWndEx()
	{
		m_cyBorder = m_cxBorder = 2;
		m_cxSplitter = m_cySplitter = 4;
		m_cySplitterGap = m_cxSplitterGap = 4;

		m_cxBorderShare = m_cyBorderShare = 0;



		m_bFlatSplit = TRUE;
		m_dwxStyle = XT_SPLIT_NOFULLDRAG|XT_SPLIT_NOBORDER;
		m_bFullDrag = FALSE;
	}

};

#include "ShortcutPaneMail.h"
#include "ShortcutPaneCalendar.h"
#include "ShortcutPaneFolders.h"
#include "ShortcutPaneContacts.h"

class CMainFrame : public CXTPFrameWnd
{

protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Attributes
public:
	CSplitterWndEx m_wndSplitter;
	CSplitterWndEx m_wndSplitter2;

	CXTPShortcutBar m_wndShortcutBar;

	CImageList m_ilSmallShortcuts;
	CImageList m_ilLargeShortcuts;

	BOOL m_bClientPane;



	CShortcutPaneMail m_paneMail;
	CShortcutPaneCalendar m_paneCalendar;
	CShortcutPaneFolders m_paneFolders;
	CShortcutPaneContacts m_paneContacts;

// Operations
public:
	void CreateShortcutBar();

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

// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnThemeOffice2000();
	afx_msg void OnUpdateThemeOffice2000(CCmdUI* pCmdUI);
	afx_msg void OnThemeOfficexp();
	afx_msg void OnUpdateThemeOfficexp(CCmdUI* pCmdUI);
	afx_msg void OnThemeOffice2003();
	afx_msg void OnUpdateThemeOffice2003(CCmdUI* pCmdUI);
	afx_msg void OnThemeNativexp();
	afx_msg void OnUpdateThemeNativexp(CCmdUI* pCmdUI);
	afx_msg void OnClose();
	afx_msg void OnThemeClientpane();
	afx_msg void OnUpdateThemeClientpane(CCmdUI* pCmdUI);
	//}}AFX_MSG
	afx_msg void OnCustomize();
	afx_msg LRESULT OnShortcutBarNotify(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__9D907276_5C73_4B62_A824_6E2EF45A3300__INCLUDED_)
