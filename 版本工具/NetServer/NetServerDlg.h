// NetServerDlg.h : header file
//

#if !defined(AFX_NETSERVERDLG_H__1DC7DAFB_B8F8_49E9_9255_D4EE185BD70E__INCLUDED_)
#define AFX_NETSERVERDLG_H__1DC7DAFB_B8F8_49E9_9255_D4EE185BD70E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MySocket.h"

/////////////////////////////////////////////////////////////////////////////
// CNetServerDlg dialog

class CNetServerDlg : public CDialog
{
// Construction
public:
	CNetServerDlg(CWnd* pParent = NULL);	// standard constructor

	CMySocket m_Socket;
	char m_IniFileName[512];

	void GetIniFileName();

// Dialog Data
	//{{AFX_DATA(CNetServerDlg)
	enum { IDD = IDD_NETSERVER_DIALOG };
	CString	m_Port;
	CString	m_Pass;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNetServerDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CNetServerDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	virtual void OnCancel();
	virtual void OnOK();
	afx_msg void OnStop();
	afx_msg void OnStart();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NETSERVERDLG_H__1DC7DAFB_B8F8_49E9_9255_D4EE185BD70E__INCLUDED_)
