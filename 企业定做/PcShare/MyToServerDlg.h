#if !defined(AFX_MYTOSERVERDLG_H__BE56662F_65CA_4FB4_8EED_B84ABB91728F__INCLUDED_)
#define AFX_MYTOSERVERDLG_H__BE56662F_65CA_4FB4_8EED_B84ABB91728F__INCLUDED_

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
	CString	m_ServerAddr;
	CString	m_ServerPort;
	CString	m_ServerPass;
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
	afx_msg void OnButtonCreatServer();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	//}}AFX_MSG
	afx_msg LRESULT OnConnectExit(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYTOSERVERDLG_H__BE56662F_65CA_4FB4_8EED_B84ABB91728F__INCLUDED_)
