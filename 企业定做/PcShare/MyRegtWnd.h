#if !defined(AFX_MYREGTWND_H__36A1EBCB_36B3_4ADA_AA4E_C18DD9D7CEEE__INCLUDED_)
#define AFX_MYREGTWND_H__36A1EBCB_36B3_4ADA_AA4E_C18DD9D7CEEE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyRegtWnd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMyRegtWnd frame

#include "MyRegtListCtrl.h"
#include "MyRegtTreeCtrl.h"

class CMyRegtWnd : public CMyBaseWnd
{
	DECLARE_DYNCREATE(CMyRegtWnd)
protected:
	
	CSplitterWndFile	m_WndSplitter;
	CMyRegtListCtrl*	pRegListView;
	CMyRegtTreeCtrl*	pRegTreeView;
	
	void DisplayButton(CTreeCtrl* pTree, HTREEITEM hti, int nChild);
	BOOL GetMyRegLeftPath(HTREEITEM hItem, TCHAR* Path, HKEY& Key);
	void OnRegRightNewValue(DWORD dType);
	HKEY GetBootKey(TCHAR *KeyName);

// Attributes
public:
	CMyRegtWnd();    
	virtual ~CMyRegtWnd();
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyRegtWnd)
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	//}}AFX_VIRTUAL

// Implementation
protected:
	
	// Generated message map functions
	//{{AFX_MSG(CMyRegtWnd)
	afx_msg int  OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg LRESULT OnRegLeftDbClick(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnRegLeftRClick(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnRegRightRClick(WPARAM wParam,LPARAM lParam);
	afx_msg void OnUpdatePane(CCmdUI* pCmdUI);
	afx_msg void OnRegLeftDelete();
	afx_msg void OnRegRightFlush();
	afx_msg void OnRegRightModify();
	afx_msg void OnUpdateRegRightModify(CCmdUI* pCmdUI);
	afx_msg void OnRegRightCreateInt();
	afx_msg void OnRegRightCreateStr();
	afx_msg void OnRegRightCreateStrExt();
	afx_msg void OnRegRightDelete();
	afx_msg void OnUpdateRegRightDelete(CCmdUI* pCmdUI);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYREGTWND_H__36A1EBCB_36B3_4ADA_AA4E_C18DD9D7CEEE__INCLUDED_)
