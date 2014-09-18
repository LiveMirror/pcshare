#if !defined(AFX_MYCWNDWND_H__D1C5CFE1_90D5_4453_9DD8_A26B1F267D1A__INCLUDED_)
#define AFX_MYCWNDWND_H__D1C5CFE1_90D5_4453_9DD8_A26B1F267D1A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyCwndWnd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMyCwndWnd frame
#include "MyCwndListView.h"
class CMyCwndWnd : public CMyBaseWnd
{
	DECLARE_DYNCREATE(CMyCwndWnd)
protected:

	CMyCwndListView* pCwndListView;

// Attributes
public:

	CMyCwndWnd();   
	virtual ~CMyCwndWnd();
	void OnCwndCtrl(UINT nType);
	virtual BOOL StartWork(LPCTSTR sCmdStr);

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyCwndWnd)
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	//}}AFX_VIRTUAL

// Implementation
protected:
	
	
	// Generated message map functions
	//{{AFX_MSG(CMyCwndWnd)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnCwndFlush();
	afx_msg void OnCwndMax();
	afx_msg void OnUpdateCwndMax(CCmdUI* pCmdUI);
	afx_msg void OnCwndMin();
	afx_msg void OnUpdateCwndMin(CCmdUI* pCmdUI);
	afx_msg void OnCwndHide();
	afx_msg void OnUpdateCwndHide(CCmdUI* pCmdUI);
	afx_msg void OnCwndShow();
	afx_msg void OnUpdateCwndShow(CCmdUI* pCmdUI);
	afx_msg void OnCwndClose();
	afx_msg void OnUpdateCwndClose(CCmdUI* pCmdUI);
	afx_msg void OnUpdatePane(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYCWNDWND_H__D1C5CFE1_90D5_4453_9DD8_A26B1F267D1A__INCLUDED_)
