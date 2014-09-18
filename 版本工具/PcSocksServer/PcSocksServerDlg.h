// PcSocksServerDlg.h : header file
//

#if !defined(AFX_PCSOCKSSERVERDLG_H__25597F51_02B3_4916_A3F4_1E7E6B40F30E__INCLUDED_)
#define AFX_PCSOCKSSERVERDLG_H__25597F51_02B3_4916_A3F4_1E7E6B40F30E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MySocksModule.h"

/////////////////////////////////////////////////////////////////////////////
// CPcSocksServerDlg dialog

class CPcSocksServerDlg : public CDialog
{
// Construction
public:
	CPcSocksServerDlg(CWnd* pParent = NULL);	// standard constructor

	CMySocksModule m_Socks;

	CString m_IniFileName;

// Dialog Data
	//{{AFX_DATA(CPcSocksServerDlg)
	enum { IDD = IDD_PCSOCKSSERVER_DIALOG };
	CString	m_Pass;
	CString	m_Port;
	CString	m_User;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPcSocksServerDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CPcSocksServerDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnStart();
	afx_msg void OnStop();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PCSOCKSSERVERDLG_H__25597F51_02B3_4916_A3F4_1E7E6B40F30E__INCLUDED_)
