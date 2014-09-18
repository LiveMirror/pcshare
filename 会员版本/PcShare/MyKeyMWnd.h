#if !defined(AFX_MYKEYMWND_H__5FCF10B9_0141_4D5B_B63D_1625CDE8ADE3__INCLUDED_)
#define AFX_MYKEYMWND_H__5FCF10B9_0141_4D5B_B63D_1625CDE8ADE3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyKeyMWnd.h : header file
//

#include "MyKeyMEditView.h"

/////////////////////////////////////////////////////////////////////////////
// CMyKeyMWnd frame

class CMyKeyMWnd : public CMyBaseWnd
{
	DECLARE_DYNCREATE(CMyKeyMWnd)
protected:
	         // protected constructor used by dynamic creation

// Attributes
public:

	virtual ~CMyKeyMWnd();
	CMyKeyMWnd();  

// Operations
public:

	virtual BOOL StartWork(LPCTSTR sCmdStr);
	
	CMyKeyMEditView* pChildView;

	void KeyMRecv();
	static void KeyMRecvThread(LPVOID lPvoid);
	HANDLE hThreadRecv;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyKeyMWnd)
	protected:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMyKeyMWnd)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnClose();
	afx_msg void OnSave();
	afx_msg void OnUpdateSave(CCmdUI* pCmdUI);
	//}}AFX_MSG
	afx_msg LRESULT OnConnBreak(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYKEYMWND_H__5FCF10B9_0141_4D5B_B63D_1625CDE8ADE3__INCLUDED_)
