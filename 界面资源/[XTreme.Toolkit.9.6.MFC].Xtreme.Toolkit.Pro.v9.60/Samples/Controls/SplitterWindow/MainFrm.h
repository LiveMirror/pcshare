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

#if !defined(AFX_MAINFRM_H__A105F07A_6E79_4360_9389_0BFC230BAC68__INCLUDED_)
#define AFX_MAINFRM_H__A105F07A_6E79_4360_9389_0BFC230BAC68__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CView1;
class CView2;
class CView3;
class CView4;

class CMainFrame : public CXTPFrameWnd
{

protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Attributes

public:

// Operations
public:

	BOOL m_bRowHidden;
	BOOL m_bColHidden;
	BOOL m_bSwapped;
	BOOL m_bReplaced;
	BOOL m_bFullDrag;
	BOOL m_bDotTrack;
	BOOL m_bNoSizing;
	BOOL m_bFlatSplit;

	CMap<int,int,CWnd*,CWnd*> m_mapRowCol;

	void SynchronizePanes(CXTSplitterWnd& wndSplitter);

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
	CXTPStatusBar    m_wndStatusBar;
	CXTSplitterWnd  m_wndSplitter;

	CView1* m_pView1;
	CView2* m_pView2;
	CView3* m_pView3;
	CView4* m_pView4;

// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSwapViews();
	afx_msg void OnReplaceView();
	afx_msg void OnUpdateReplaceView(CCmdUI* pCmdUI);
	afx_msg void OnUpdateSwapViews(CCmdUI* pCmdUI);
	afx_msg void OnHideFirstRow();
	afx_msg void OnUpdateHideFirstRow(CCmdUI* pCmdUI);
	afx_msg void OnHideFirstCol();
	afx_msg void OnUpdateHideFirstCol(CCmdUI* pCmdUI);
	afx_msg void OnSplitNofulldrag();
	afx_msg void OnUpdateSplitNofulldrag(CCmdUI* pCmdUI);
	afx_msg void OnSplitDottracker();
	afx_msg void OnUpdateSplitDottracker(CCmdUI* pCmdUI);
	afx_msg void OnSplitNosize();
	afx_msg void OnUpdateSplitNosize(CCmdUI* pCmdUI);
	afx_msg void OnSplitFlatsplitter();
	afx_msg void OnUpdateSplitFlatsplitter(CCmdUI* pCmdUI);
	afx_msg void OnViewXpmode();
	afx_msg void OnUpdateViewXpmode(CCmdUI* pCmdUI);
	afx_msg void OnClose();
	//}}AFX_MSG
	afx_msg void OnCustomize();
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__A105F07A_6E79_4360_9389_0BFC230BAC68__INCLUDED_)
