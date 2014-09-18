#if !defined(AFX_MYFILEDOWNWND_H__74608738_3113_4C40_B48D_87494DD79FFB__INCLUDED_)
#define AFX_MYFILEDOWNWND_H__74608738_3113_4C40_B48D_87494DD79FFB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyFileDownWnd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMyFileDownWnd frame

#include "MyFileDownView.h"
#include "MyFileLogsView.h"

class CMyFileDownWnd : public CXTPFrameWnd
{
	DECLARE_DYNCREATE(CMyFileDownWnd)
public:
	CMyFileDownWnd();           // protected constructor used by dynamic creation
	virtual ~CMyFileDownWnd();
// Attributes
public:

	CMyFileDownView* pFileDownView;
// Operations
public:

	virtual BOOL StartWork(LPCTSTR sCmdStr);
	
	BOOL m_IsShow;
	HICON				hIcon;
	CSplitterWndEx		m_WndSplitter;
	CMyFileLogsView		*pLogView;
	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyFileDownWnd)
	public:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
protected:
	

	// Generated message map functions
	//{{AFX_MSG(CMyFileDownWnd)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct);
	afx_msg void OnClose();
	afx_msg void OnExit();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	afx_msg LRESULT OnInsertLogs(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYFILEDOWNWND_H__74608738_3113_4C40_B48D_87494DD79FFB__INCLUDED_)
