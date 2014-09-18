#if !defined(AFX_MYPROCWND_H__5FCE5BA8_B5B3_4F6A_8A8A_8DD9C5AE2125__INCLUDED_)
#define AFX_MYPROCWND_H__5FCE5BA8_B5B3_4F6A_8A8A_8DD9C5AE2125__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyProcWnd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMyProcWnd frame

#include "MyProcListView.h"
class CMyProcWnd : public CMyBaseWnd
{
	DECLARE_DYNCREATE(CMyProcWnd)
protected:
	          // protected constructor used by dynamic creation

// Attributes
public:

	virtual BOOL StartWork(LPCTSTR sCmdStr);

// Operations
public:

	CMyProcWnd(); 
	virtual ~CMyProcWnd();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyProcWnd)
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	//}}AFX_VIRTUAL

// Implementation
protected:

	CMyProcListView* pProcessListView;
	
	// Generated message map functions
	//{{AFX_MSG(CMyProcWnd)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnProcessFlush();
	afx_msg void OnProcessKill();
	afx_msg void OnUpdateProcessKill(CCmdUI* pCmdUI);
	afx_msg void OnUpdatePane(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYPROCWND_H__5FCE5BA8_B5B3_4F6A_8A8A_8DD9C5AE2125__INCLUDED_)
