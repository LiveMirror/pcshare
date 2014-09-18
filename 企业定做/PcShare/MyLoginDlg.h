#if !defined(AFX_MYLOGINDLG_H__B9E10D6C_504C_426D_8805_11549135DFEB__INCLUDED_)
#define AFX_MYLOGINDLG_H__B9E10D6C_504C_426D_8805_11549135DFEB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyLoginDlg.h : header file
//

#include "MyLinkCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CMyLoginDlg dialog

class CMyLoginDlg : public CDialog
{
// Construction
public:
	CMyLoginDlg(CWnd* pParent, BOOL IsDlFile);   // standard constructor
	~CMyLoginDlg();

// Dialog Data
	//{{AFX_DATA(CMyLoginDlg)
	enum { IDD = IDD_DIALOG_LOGIN_DLG };
	CStatic	m_Title;
	CString	m_User;
	CString	m_Pass;
	//}}AFX_DATA

	TCHAR m_ServerUrl[512];
	static void PsLoginThread(LPVOID lPvoid);
	void PsLogin();
	
	BOOL m_IsDlFile;
	BOOL m_IsExit;
	HANDLE hThread;
	char* pSystemFileData;
	HINTERNET hMainIe;
	HINTERNET hMainFile;
	CRect m_Rect;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyLoginDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMyLoginDlg)
	afx_msg void OnStaticLosePin();
	afx_msg void OnStaticChangePin();
	afx_msg void OnStaticGetPin();
	virtual void OnOK();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	afx_msg LRESULT OnLogonEnd(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYLOGINDLG_H__B9E10D6C_504C_426D_8805_11549135DFEB__INCLUDED_)
