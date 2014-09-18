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

#if !defined(AFX_MAINFRM_H__22CB24AE_BB90_499A_86AF_05F3A606A16C__INCLUDED_)
#define AFX_MAINFRM_H__22CB24AE_BB90_499A_86AF_05F3A606A16C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CSplitterWndEx : public CXTSplitterWnd
{
public:
	CSplitterWndEx()
	{
		m_cyBorder = m_cxBorder = 0;
		m_cxSplitter = m_cySplitter = 4;
		m_cySplitterGap = m_cxSplitterGap = 4;
	}

};

class CPreviewFrame;


class CMainFrame : public CXTPFrameWnd
{

protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Attributes
public:
	CSplitterWndEx m_wndSplitter;
	CSplitterWndEx m_wndSplitterCaption;
	CSplitterWndEx m_wndSplitterWorkspace;
	CSplitterWndEx m_wndSplitterPreview;

	CXTCaption       m_wndCaption;
	CFont            m_fontCaption;

	CXTPDockingPaneManager m_paneManager;

	CXTTreeCtrl m_wndTreeCtrl;

	CImageList m_ilTreeIcons;

	CPreviewFrame* m_pPreviewFrame;


// Operations
public:
	BOOL InitializeCaptionBar();

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
	CToolBar    m_wndToolBar;

	int m_nState;

// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnViewOfficexp();
	afx_msg void OnUpdateViewOfficexp(CCmdUI* pCmdUI);
	afx_msg void OnViewOffice2003();
	afx_msg void OnUpdateViewOffice2003(CCmdUI* pCmdUI);
	//}}AFX_MSG
	afx_msg int OnCreateControl(LPCREATECONTROLSTRUCT lpCreateControl);
	afx_msg void OnEmptyCommand();
	afx_msg void OnBuildConfiguration(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnUpdateBuildConfiguration(CCmdUI* pCmdUI);
	afx_msg void OnCustomize();
	afx_msg LRESULT OnDockingPaneNotify(WPARAM wParam, LPARAM lParam);

public:
	static CArray<CXTPDockingPaneManager*, CXTPDockingPaneManager*> m_arrManagers;


	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__22CB24AE_BB90_499A_86AF_05F3A606A16C__INCLUDED_)
