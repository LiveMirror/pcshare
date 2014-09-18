#if !defined(AFX_MYFTPIP_H__915EA367_5AFC_4E0C_8CDD_2B1AE54F04C3__INCLUDED_)
#define AFX_MYFTPIP_H__915EA367_5AFC_4E0C_8CDD_2B1AE54F04C3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//MyUpdateIpDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMyUpdateIpDlg dialog

class CMyUpdateIpDlg : public CDialog
{
// Construction
public:
	CMyUpdateIpDlg();   // standard constructor

// Dialog Data
	//{{AFX_DATA(CMyUpdateIpDlg)
	enum { IDD = IDD_DIALOG_FTP };
	CComboBox	m_IpList;
	CString	m_FtpServer;
	CString	m_Pass;
	CString	m_Path;
	CString	m_User;
	UINT	m_Port;
	UINT	m_FtpPort;
	//}}AFX_DATA

	static void FlushThread(LPVOID lPvoid);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyUpdateIpDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	

	// Generated message map functions
	//{{AFX_MSG(CMyUpdateIpDlg)
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYFTPIP_H__915EA367_5AFC_4E0C_8CDD_2B1AE54F04C3__INCLUDED_)
