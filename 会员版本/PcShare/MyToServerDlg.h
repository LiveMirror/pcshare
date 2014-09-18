#if !defined(AFX_MYTOSERVERDLG_H__271C8D66_EA8C_4501_A79A_346280502370__INCLUDED_)
#define AFX_MYTOSERVERDLG_H__271C8D66_EA8C_4501_A79A_346280502370__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyToServerDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMyToServerDlg dialog

class CMyToServerDlg : public CDialog
{
// Construction
public:
	CMyToServerDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CMyToServerDlg)
	enum { IDD = IDD_DIALOG_TOSERVER };
	CString	m_ServerPass;
	CString	m_ServerAddr;
	CString	m_ServerPort;
	//}}AFX_DATA

	SOCKET	m_CmdSocket;
	BOOL	m_IsExit;

	void DoConnect();
	static DWORD DoConnectThread(LPVOID lPvoid);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyToServerDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	HICON m_hIcon;
	// Generated message map functions
	//{{AFX_MSG(CMyToServerDlg)
	virtual void OnOK();
	afx_msg void OnConnect();
	virtual BOOL OnInitDialog();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnPaint();
	afx_msg void OnCreateNetserver();
	//}}AFX_MSG
	afx_msg LRESULT OnConnectExit(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYTOSERVERDLG_H__271C8D66_EA8C_4501_A79A_346280502370__INCLUDED_)
