#if !defined(AFX_MYSTARTSOCKSDLG_H__9A2C5E2B_A5F6_4E17_B333_05348C0A0F61__INCLUDED_)
#define AFX_MYSTARTSOCKSDLG_H__9A2C5E2B_A5F6_4E17_B333_05348C0A0F61__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyStartSocksDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMyStartSocksDlg dialog

class CMyStartSocksDlg : public CDialog
{
// Construction
public:
	CMyStartSocksDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CMyStartSocksDlg)
	enum { IDD = IDD_DIALOG_SOCKS };
	CString	m_Pass;
	CString	m_Port;
	CString	m_User;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyStartSocksDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMyStartSocksDlg)
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYSTARTSOCKSDLG_H__9A2C5E2B_A5F6_4E17_B333_05348C0A0F61__INCLUDED_)
