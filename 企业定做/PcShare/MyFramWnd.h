#if !defined(AFX_MYFRAMEWND_H__FE4E9B37_C1C1_489A_8779_0745C311B462__INCLUDED_)
#define AFX_MYFRAMEWND_H__FE4E9B37_C1C1_489A_8779_0745C311B462__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyFrameWnd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMyFramWnd frame
#include "MyMultView.h"

class CMyFramWnd : public CMyBaseWnd
{
	DECLARE_DYNCREATE(CMyFramWnd)
protected:

public:
	CMyFramWnd();           // protected constructor used by dynamic creation
	virtual ~CMyFramWnd();

// Attributes
public:

// Operations
public:
	
	virtual BOOL StartWork(LPCTSTR sCmdStr);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyFramWnd)
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	//}}AFX_VIRTUAL

// Implementation
protected:
	
	CMyMultView*		pFrameView;
	BOOL				m_IsEnableInput;
	CMyImageList		m_ToolBarList;
	CToolBar			m_wndToolBar;

	// Generated message map functions
	//{{AFX_MSG(CMyFramWnd)
	afx_msg void OnSendCtrl();
	afx_msg void OnUpdateSendCtrl(CCmdUI* pCmdUI);
	afx_msg void OnSendMouse();
	afx_msg void OnUpdateSendMouse(CCmdUI* pCmdUI);
	afx_msg void OnRecordStart();
	afx_msg void OnUpdateRecordStart(CCmdUI* pCmdUI);
	afx_msg void OnRecordStop();
	afx_msg void OnUpdateRecordStop(CCmdUI* pCmdUI);
	afx_msg void OnSaveImage();
	afx_msg void OnUpdateSaveImage(CCmdUI* pCmdUI);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSysCommand( UINT nID, LPARAM lParam);
	//}}AFX_MSG
	afx_msg void OnUpdatePane(CCmdUI* pCmdUI);
	DECLARE_MESSAGE_MAP()
public:
	
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYFRAMEWND_H__FE4E9B37_C1C1_489A_8779_0745C311B462__INCLUDED_)
