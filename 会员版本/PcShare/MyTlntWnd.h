#if !defined(AFX_MAINFRM_H__53DD2FCE_07F0_4992_B625_965B46946F67__INCLUDED_)
#define AFX_MAINFRM_H__53DD2FCE_07F0_4992_B625_965B46946F67__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MyTlntEditView.h"

class CMyTlntWnd : public CMyBaseWnd 
{
protected:
	
	DECLARE_DYNCREATE(CMyTlntWnd) 
	
public:
	CMyTlntWnd();

	virtual BOOL StartWork(LPCTSTR sCmdStr);

	void TlntRecv();
	static void TlntRecvThread(LPVOID lPvoid);
	void TlntSend();
	static void TlntSendThread(LPVOID lPvoid);

// Attributes
public:

// Operations
public:

	HANDLE	hThreadSend, hThreadRecv, m_InputEvent;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyTlntWnd)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMyTlntWnd();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
		virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
public:

	CMyTlntEditView* pChildView;

// Generated message map functions
protected:
	//{{AFX_MSG(CMyTlntWnd)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSave();
	afx_msg void OnClose();
	afx_msg void OnUpdateSave(CCmdUI* pCmdUI);
	//}}AFX_MSG
	afx_msg LRESULT OnConnBreak(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__53DD2FCE_07F0_4992_B625_965B46946F67__INCLUDED_)
