// PcFileCombDlg.h : header file
//

#if !defined(AFX_PCFILECOMBDLG_H__6835D2E2_D04E_4A82_BA3F_BCC2526FE965__INCLUDED_)
#define AFX_PCFILECOMBDLG_H__6835D2E2_D04E_4A82_BA3F_BCC2526FE965__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CPcFileCombDlg dialog

class CPcFileCombDlg : public CDialog
{
// Construction
public:
	CPcFileCombDlg(CWnd* pParent = NULL);	// standard constructor

	BOOL GetMySource(TCHAR* pFileName, BYTE* pFileData, UINT* pFileLen);

// Dialog Data
	//{{AFX_DATA(CPcFileCombDlg)
	enum { IDD = IDD_PCFILECOMB_DIALOG };
	CString	m_Value;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPcFileCombDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CPcFileCombDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnButtonPath();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PCFILECOMBDLG_H__6835D2E2_D04E_4A82_BA3F_BCC2526FE965__INCLUDED_)
