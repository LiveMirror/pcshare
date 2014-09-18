// PsProxyDlg.h : header file
//

#if !defined(AFX_PSPROXYDLG_H__E2C36E23_09C9_4F2F_A0EE_35DFB973D0D0__INCLUDED_)
#define AFX_PSPROXYDLG_H__E2C36E23_09C9_4F2F_A0EE_35DFB973D0D0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CPsProxyDlg dialog

#include "MySocket.h"

class CPsProxyDlg : public CDialog
{
// Construction
public:
	CPsProxyDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CPsProxyDlg)
	enum { IDD = IDD_PSPROXY_DIALOG };
	CString	m_LocalPort;
	CString	m_RemoteAddr;
	CString	m_RemotePort;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPsProxyDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

	
	CMySocket m_Socket;
	char m_IniFileName[512];

	void GetIniFileName();

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CPsProxyDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnStop();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PSPROXYDLG_H__E2C36E23_09C9_4F2F_A0EE_35DFB973D0D0__INCLUDED_)
