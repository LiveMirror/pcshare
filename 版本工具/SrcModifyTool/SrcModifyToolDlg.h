// SrcModifyToolDlg.h : header file
//

#if !defined(AFX_SRCMODIFYTOOLDLG_H__B23FEF5E_9BB9_4049_AB3D_5945EAC1002C__INCLUDED_)
#define AFX_SRCMODIFYTOOLDLG_H__B23FEF5E_9BB9_4049_AB3D_5945EAC1002C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CSrcModifyToolDlg dialog

class CSrcModifyToolDlg : public CDialog
{
// Construction
public:
	CString GetStrcpyReplaceCode(CString strSrcCode);
	CString GetMemcpyReplaceCode(CString strSrcCode);
	CString GetReplaceCode(CString strSrcCode);
	void GetModifyCode(char* szFlagCode);
	BOOL ModifySingleSrcFileStrRes(char* szSrcFilename, char* szModifyFlag);
	void EnCryptString(char* szSrcStr);
	void BatchModifySrcFiles(CString strSrcFilePath, char* szAsmOpCode);
	BOOL ModifySingleSrcFile(char* szSrcFilename, char* szAsmOpCodes);


    int GetDir(CString& path ) ;


	CSrcModifyToolDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CSrcModifyToolDlg)
	enum { IDD = IDD_SRCMODIFYTOOL_DIALOG };
	CEdit	m_ctRunState;
	CString	m_strRunState;
	CString	m_strSrcPath;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSrcModifyToolDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CSrcModifyToolDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBtnFolderbrowse();
	afx_msg void OnBtnModifycode();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SRCMODIFYTOOLDLG_H__B23FEF5E_9BB9_4049_AB3D_5945EAC1002C__INCLUDED_)
