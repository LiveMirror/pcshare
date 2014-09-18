#if !defined(AFX_MYMAINSHOWWND_H__6A4D3C7A_D3A5_4E02_8A0E_CEF106E2CA48__INCLUDED_)
#define AFX_MYMAINSHOWWND_H__6A4D3C7A_D3A5_4E02_8A0E_CEF106E2CA48__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyMainShowWnd.h : header file
//

#include "MyClientManaView.h"
#include "MyLogsView.h"
#include "MyTopView.h"
#include "MyBottomView.h"

/////////////////////////////////////////////////////////////////////////////
// CMyMainShowWnd frame

class CMyMainShowWnd : public CXTPFrameWnd
{
	DECLARE_DYNCREATE(CMyMainShowWnd)

public:
	CMyMainShowWnd();           // protected constructor used by dynamic creation

// Attributes
public:

	CSplitterWndEx		m_WndSplitter;
	CMyImageList		m_ToolBarList;
	NOTIFYICONDATA		m_IconData;
	CMyTopView			*pMainWnd;
	CMyLogsView			*pLogView;
	CMyBottomView		*pBottomView;
	HICON				hIcon;
	CXTPStatusBar		m_wndStatusBar;
	CToolBar			m_wndToolBar;

// Operations
public:

	void ResizeWnd();
	virtual BOOL StartWork();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyMainShowWnd)
	protected:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CMyMainShowWnd();

	// Generated message map functions
	//{{AFX_MSG(CMyMainShowWnd)
	afx_msg void OnUpdatePane(CCmdUI *pCmdUI);
	afx_msg BOOL OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnIconExit();
	afx_msg void OnIconShow();
	afx_msg void OnAbout();
	afx_msg void OnClientCreate();
	afx_msg void OnFlushIp();
	afx_msg void OnHelp();
	afx_msg void OnGlobalSetup();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnUserAddPin();
	afx_msg void OnUserGetPin();
	afx_msg void OnUserModifyPin();
	afx_msg void OnClose();
	afx_msg void OnClientManaDl();
	afx_msg void OnUpdateClientManaDl(CCmdUI* pCmdUI);
	//}}AFX_MSG
	afx_msg LRESULT OnInsertLogs(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnIconInfo(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYMAINSHOWWND_H__6A4D3C7A_D3A5_4E02_8A0E_CEF106E2CA48__INCLUDED_)
