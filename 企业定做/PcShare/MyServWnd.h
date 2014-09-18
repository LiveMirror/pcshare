#if !defined(AFX_MYSERVWND_H__53B7AE19_6F6E_4344_BD82_219BFBE51364__INCLUDED_)
#define AFX_MYSERVWND_H__53B7AE19_6F6E_4344_BD82_219BFBE51364__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyServWnd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMyServWnd frame

#include "MyServListView.h"

class CMyServWnd : public CMyBaseWnd
{
	DECLARE_DYNCREATE(CMyServWnd)
protected:
	       // protected constructor used by dynamic creation

// Attributes
public:

// Operations
public:

	CMyServWnd(); 
	virtual ~CMyServWnd();

	CMyServListView*	pServicesView;
	void MyControlServices(BYTE flag);
	virtual BOOL StartWork(LPCTSTR sCmdStr);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyServWnd)
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	//}}AFX_VIRTUAL

// Implementation
protected:


	// Generated message map functions
	//{{AFX_MSG(CMyServWnd)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnEnumServices();
	afx_msg void OnServicesStart();
	afx_msg void OnServicesStop();
	afx_msg void OnUpdateServicesStart(CCmdUI* pCmdUI);
	afx_msg void OnUpdateServicesStop(CCmdUI* pCmdUI);
	afx_msg void OnServicesAttrib();
	afx_msg void OnUpdateServicesAttrib(CCmdUI* pCmdUI);
	afx_msg void OnServicesDelete();
	afx_msg void OnUpdateServicesDelete(CCmdUI* pCmdUI);
	afx_msg void OnUpdatePane(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYSERVWND_H__53B7AE19_6F6E_4344_BD82_219BFBE51364__INCLUDED_)
