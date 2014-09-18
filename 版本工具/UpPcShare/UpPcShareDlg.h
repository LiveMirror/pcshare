// UpPcShareDlg.h : header file
//

#if !defined(AFX_UPPCSHAREDLG_H__873E37DC_F902_42E1_AAA0_56E11C6D0FC0__INCLUDED_)
#define AFX_UPPCSHAREDLG_H__873E37DC_F902_42E1_AAA0_56E11C6D0FC0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CUpPcShareDlg dialog


class CUpPcShareDlg : public CDialog
{
// Construction
public:
	CUpPcShareDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CUpPcShareDlg)
	enum { IDD = IDD_UPPCSHARE_DIALOG };
	CProgressCtrl	m_FilePos;
	CString	m_FileName;
	CString	m_UserId;
	BOOL	m_NoKey;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUpPcShareDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

	BOOL AddDataToPe(BYTE* pSaveData, DWORD DataLen, BYTE* pPeData, DWORD nPeLen, LPCTSTR pPeFile);
	BOOL FindCopyFile(LPCTSTR m_ParentPath, LPSAVEFILEINFO pInfo, DWORD& nFileLen, DWORD& nFileCount, LPCTSTR pMainFile);
	BOOL GetMySource(TCHAR* pFileName, BYTE** pFileData, DWORD& nFileLen);

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CUpPcShareDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnButtonGetfile();
	afx_msg void OnCheckIskey();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_UPPCSHAREDLG_H__873E37DC_F902_42E1_AAA0_56E11C6D0FC0__INCLUDED_)
