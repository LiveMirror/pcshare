#if !defined(AFX_MYBASEWND_H__D446450B_6961_482F_95E1_A0B2B09502CC__INCLUDED_)
#define AFX_MYBASEWND_H__D446450B_6961_482F_95E1_A0B2B09502CC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyBaseWnd.h : header file
//

#include "resource.h" 
#include "MyServerTran.h"
#include "MyWaitTransDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CMyBaseWnd frame

#define WM_GETSOCKET_OK	WM_USER + 5001

class CMyBaseWnd : public CXTPFrameWnd
{
	DECLARE_DYNCREATE(CMyBaseWnd)
protected:
	CMyBaseWnd();           // protected constructor used by dynamic creation

// Attributes
public:
	virtual BOOL StartWork(LPCTSTR sCmdStr);

	BOOL StartWorkWithCmd(LPCTSTR sCmdStr, DWORD nCmd, LPTCPCONNECTINFO pSocketInfo);

	BOOL RecvDataWithCmd(LPBYTE* pRecvData, DWORD* dRecvLen, DWORD* dCmd);
	BOOL SendDataWithCmd(LPVOID pSendData, DWORD dSendLen, DWORD nCmd);
	BOOL TranData(LPVOID InData, DWORD InLen, LPVOID OutData, DWORD OutLen);
	UINT GetAcp();
	BOOL IsInit();
	void CloseSocket();
	
// Operations
public:

	MyServerTran	m_Tran;
	HICON			hIcon;
	BOOL			m_CurSorStatus;
	CXTPStatusBar	m_wndStatusBar;
	TCHAR			m_ExtCmdStr[2048];

private:
	
	BOOL			m_IsInit;
	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyBaseWnd)
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CMyBaseWnd();

	// Generated message map functions
	//{{AFX_MSG(CMyBaseWnd)
	afx_msg void OnClose();
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	
	afx_msg LRESULT OnSetMyCursor(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYBASEWND_H__D446450B_6961_482F_95E1_A0B2B09502CC__INCLUDED_)
