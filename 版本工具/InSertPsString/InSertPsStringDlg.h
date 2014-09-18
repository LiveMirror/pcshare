// InSertPsStringDlg.h : header file
//

#if !defined(AFX_INSERTPSSTRINGDLG_H__B052BCBB_65C2_4709_9610_438F15A33601__INCLUDED_)
#define AFX_INSERTPSSTRINGDLG_H__B052BCBB_65C2_4709_9610_438F15A33601__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CInSertPsStringDlg dialog

class CInSertPsStringDlg : public CDialog
{
// Construction
public:
	CInSertPsStringDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CInSertPsStringDlg)
	enum { IDD = IDD_INSERTPSSTRING_DIALOG };
	CString	m_FilePath;
	CString	m_Value;
	CString	m_ShowText;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInSertPsStringDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;
	CString m_IniFileName;
	// Generated message map functions
	//{{AFX_MSG(CInSertPsStringDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnButtonInsert();
	afx_msg void OnButtonPath();
	afx_msg void OnButtonSaveas();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INSERTPSSTRINGDLG_H__B052BCBB_65C2_4709_9610_438F15A33601__INCLUDED_)
