// mainfrm.h : interface of the CMainFrame class
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

#if !defined(AFX_MAINFRM_H__4CCF8A15_BE48_4C19_9F2B_8C1AA1EB4A2C__INCLUDED_)
#define AFX_MAINFRM_H__4CCF8A15_BE48_4C19_9F2B_8C1AA1EB4A2C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "drawobj.h"
#include "PanePropertyGrid.h"


#ifndef _XTP_INCLUDE_COMMANDBARS
#define CXTPMDIFrameWnd CMDIFrameWnd
#endif

CXTPPropertyGrid& GetPropertyGrid(CWnd* pFrame = 0);

#ifndef _XTP_INCLUDE_DOCKINGPANE
class CPropertyBar : public CControlBar
{
public:
	virtual CSize CalcFixedLayout(BOOL bStretch, BOOL bHorz)
	{
		int nHeight = 600;
		if (GetParent())
		{
			CRect rc;
			GetParent()->GetWindowRect(rc);
			nHeight = rc.Height();
		}

		if (bStretch) // if not docked stretch to fit
			return CSize(bHorz ? 32767 : 200, bHorz ? 120 : 32767);
		else return CSize(250, nHeight);
	}
	void OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler)
	{

	}

	BOOL Create(CWnd* pParentWnd)
	{

		if (!CWnd::CreateEx(0,
			AfxRegisterWndClass(0, LoadCursor(0, IDC_ARROW)), 0, WS_CLIPCHILDREN|WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS,
				CRect(0, 0, 200, 120), pParentWnd, 120))
				return FALSE;

		VERIFY(GetPropertyGrid(pParentWnd).Create(CRect(0, 0, 200, 120), this, 1000));
		GetPropertyGrid(pParentWnd).SetOwner(pParentWnd);
		SetOwner(pParentWnd);

		return TRUE;
	}

	virtual LRESULT WindowProc(UINT message, WPARAM wParam,LPARAM lParam)
	{
		switch (message)
		{
			case WM_SIZE:
				CWnd* pParent = AfxGetMainWnd()? AfxGetMainWnd(): GetParentFrame();
				if (GetPropertyGrid(pParent).GetSafeHwnd())
				{
					GetPropertyGrid(pParent).MoveWindow(0, 0, LOWORD(lParam), HIWORD(lParam));
				}
				break;

		}
		return CControlBar::WindowProc(message, wParam, lParam);
	}

};

#endif


class CMainFrame : public CXTPMDIFrameWnd
{
	DECLARE_DYNAMIC(CMainFrame)
public:
	CMainFrame();

#ifdef _XTP_INCLUDE_DOCKINGPANE

// Attributes
public:
	CXTPDockingPaneManager m_paneManager;

// Operations
public:
	CXTPDockingPaneManager* GetDockingPaneManager() {
		return &m_paneManager;
	}

	CPanePropertyGrid m_panePropertyGrid;
#else

	CXTPPropertyGrid m_wndPropertyGrid;
#endif




// Operations
public:
	void UpdatePropertyGridContent(CDrawView* pView);
	CDrawView* m_pView;
	BOOL m_bInUpdate;

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

public:
	// control bar embedded members
	CToolBar    m_wndToolBar;

#ifndef _XTP_INCLUDE_DOCKINGPANE
	CPropertyBar m_wndPropertyBar;
#endif

#ifdef _XTP_INCLUDE_COMMANDBARS
	CXTPStatusBar  m_wndStatusBar;
	CXTPTabClientWnd m_MTIClientWnd;
#else
	CStatusBar  m_wndStatusBar;
#endif


	CXTPPropertyGridUpdateContext m_stateExpanding;


	BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);

// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg LRESULT OnDockingPaneNotify(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnGridNotify(WPARAM wParam, LPARAM lParam);
	afx_msg void OnProperties();
	//}}AFX_MSG
#ifdef _XTP_INCLUDE_COMMANDBARS
	afx_msg void OnClose();
	afx_msg void OnCustomize();
	void OnSetPreviewMode(BOOL bPreview, CPrintPreviewState* pState);
#endif
	afx_msg LRESULT OnTabbarMouseMsg(WPARAM wParam,LPARAM lParam);

	DECLARE_MESSAGE_MAP()
};

inline CXTPPropertyGrid& GetPropertyGrid(CWnd* pFrame) {
	if (pFrame == 0) pFrame = AfxGetMainWnd();
#ifdef _XTP_INCLUDE_DOCKINGPANE
	return ((CMainFrame*)pFrame)->m_panePropertyGrid.m_wndPropertyGrid;
#else
	return ((CMainFrame*)pFrame)->m_wndPropertyGrid;
#endif

}


/////////////////////////////////////////////////////////////////////////////
#endif // !defined(AFX_MAINFRM_H__4CCF8A15_BE48_4C19_9F2B_8C1AA1EB4A2C__INCLUDED_)
