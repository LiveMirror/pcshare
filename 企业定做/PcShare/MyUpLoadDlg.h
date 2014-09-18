#if !defined(AFX_MYUPLOADDLG_H__8C252D8F_A56A_4DC7_8169_E19E2F827259__INCLUDED_)
#define AFX_MYUPLOADDLG_H__8C252D8F_A56A_4DC7_8169_E19E2F827259__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyUpLoadDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMyUpLoadDlg dialog

class CMyUpLoadDlg : public CDialog
{
// Construction
public:
	CMyUpLoadDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CMyUpLoadDlg)
	enum { IDD = IDD_DIALOG_UPLOAD };
	CString	m_Value;
	int		m_Show;
	CString	m_Param;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyUpLoadDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMyUpLoadDlg)
	virtual void OnOK();
	afx_msg void OnButtonFile();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYUPLOADDLG_H__8C252D8F_A56A_4DC7_8169_E19E2F827259__INCLUDED_)
