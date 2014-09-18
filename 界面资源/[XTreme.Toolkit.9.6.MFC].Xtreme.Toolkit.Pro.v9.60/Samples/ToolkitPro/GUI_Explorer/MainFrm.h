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

#if !defined(AFX_MAINFRM_H__BAAD8886_B8B5_4A5C_BC69_CE7E6103993B__INCLUDED_)
#define AFX_MAINFRM_H__BAAD8886_B8B5_4A5C_BC69_CE7E6103993B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CSearchView;
class CHistoryView;
class CInternetView;
class CGUI_ExplorerTree;
class CGUI_ExplorerView;


class CCJLogoWnd : public CStatic
{
public:
	virtual LRESULT WindowProc(UINT message,WPARAM wParam,LPARAM lParam)
	{
		switch (message)
		{
		case WM_PAINT:
			{
				CPaintDC dc(this);
				CXTMemDC memDC(&dc, CXTPClientRect(this), RGB(255, 255, 255));

				return CStatic::DefWindowProc(WM_PAINT, (WPARAM)memDC.m_hDC, 0);
			}

		case WM_ERASEBKGND:
			{
				return TRUE;
			}
		}

		return CStatic::WindowProc(message, wParam, lParam);
	}
};

class CMainFrame : public CXTPFrameWnd
{

protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Attributes
protected:

public:

// Operations
public:

	CSearchView*       m_pSearchView;
	CHistoryView*      m_pHistoryView;
	CInternetView*     m_pINetView;
	CGUI_ExplorerTree* m_pTreeView;
	CGUI_ExplorerView* m_pFolderView;
	CCJLogoWnd  m_wndLogo;
	CBitmap m_bmpLogo;

	CXTPReBar m_wndReBar;
	CXTComboBoxEx m_wndAddressBar;


	inline CStatusBar& GetStatusBar() {
		return m_wndStatusBar;
	}
	inline CXTSplitterWnd& GetSplitterWnd() {
		return m_wndSplitter;
	}

	// Overrode CWnd implementation to restore saved window position.
	BOOL ShowWindowEx(int nCmdShow);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	public:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
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

	CStatusBar     m_wndStatusBar;

	CXTSplitterWndEx m_wndSplitter;

	CXTShellTree*    m_pTreeCtrl;
	CXTShellList*    m_pListCtrl;

	CXTWindowPos     m_wndPosition;

	void InitializeAddressBar();

// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnBack();
	afx_msg void OnForward();
	afx_msg void OnUp();
	afx_msg void OnSearch();
	afx_msg void OnFolders();
	afx_msg void OnHistory();
	afx_msg void OnMoveto();
	afx_msg void OnCopyto();
	afx_msg void OnDelete();
	afx_msg void OnUndo();
	afx_msg void OnViews();
	afx_msg void OnUpdateFolders(CCmdUI* pCmdUI);
	afx_msg void OnUpdateSearch(CCmdUI* pCmdUI);
	afx_msg void OnUpdateHistory(CCmdUI* pCmdUI);
	afx_msg void OnViewXpmode();
	afx_msg void OnUpdateViewXpmode(CCmdUI* pCmdUI);
	afx_msg void OnClose();
	afx_msg void OnUpdateBack(CCmdUI* pCmdUI);
	afx_msg void OnUpdateForward(CCmdUI* pCmdUI);
	afx_msg void OnUpdateUp(CCmdUI* pCmdUI);
	afx_msg void OnUpdateMoveto(CCmdUI* pCmdUI);
	afx_msg void OnUpdateCopyto(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDelete(CCmdUI* pCmdUI);
	afx_msg void OnUpdateUndo(CCmdUI* pCmdUI);
	afx_msg void OnGo();
	//}}AFX_MSG
	afx_msg void OnUpdateViewStyles(CCmdUI* pCmdUI);
	afx_msg void OnViewStyle(UINT nCommandID);
	afx_msg LRESULT OnUpdateShell(WPARAM wParam, LPARAM lParam);
	afx_msg void OnAddressBarEnter();
	afx_msg void OnSelEndOkAddressBar();
	afx_msg void OnCustomize();

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__BAAD8886_B8B5_4A5C_BC69_CE7E6103993B__INCLUDED_)
