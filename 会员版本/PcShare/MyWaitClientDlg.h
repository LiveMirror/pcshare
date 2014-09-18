#if !defined(AFX_MYWAITCLIENTDLG_H__8C3AD2EB_F2FF_43D6_844E_CA4B82C41572__INCLUDED_)
#define AFX_MYWAITCLIENTDLG_H__8C3AD2EB_F2FF_43D6_844E_CA4B82C41572__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyWaitClientDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMyWaitClientDlg dialog

class CMyWaitClientDlg : public CDialog
{
// Construction
public:
	CMyWaitClientDlg(CWnd* pParent, LPCTSTR sWndTitle, DWORD nCmd);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CMyWaitClientDlg)
	enum { IDD = IDD_DIALOG_WAITCLIENT };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	TCHAR			m_ExtCmdStr[2048];
	TCPCONNECTINFO	m_SocketInfo;
	DWORD			nSaveCmd;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyWaitClientDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMyWaitClientDlg)
	virtual void OnCancel();
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg BOOL OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct);
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYWAITCLIENTDLG_H__8C3AD2EB_F2FF_43D6_844E_CA4B82C41572__INCLUDED_)
