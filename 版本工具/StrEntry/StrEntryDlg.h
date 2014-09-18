// StrEntryDlg.h : header file
//

#if !defined(AFX_STRENTRYDLG_H__FB2D8844_2324_404A_B748_4204530ECFCF__INCLUDED_)
#define AFX_STRENTRYDLG_H__FB2D8844_2324_404A_B748_4204530ECFCF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CStrEntryDlg dialog

class CStrEntryDlg : public CDialog
{
// Construction
public:
	CStrEntryDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CStrEntryDlg)
	enum { IDD = IDD_STRENTRY_DIALOG };
	CString	m_DesStr;
	CString	m_SrcStr;
	CString	m_ValueName;
	BOOL	m_IsExt;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStrEntryDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CStrEntryDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STRENTRYDLG_H__FB2D8844_2324_404A_B748_4204530ECFCF__INCLUDED_)
