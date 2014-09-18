// FileInsertDlg.h : header file
//

#if !defined(AFX_FILEINSERTDLG_H__A6243310_A834_48ED_BE49_9474E49EA139__INCLUDED_)
#define AFX_FILEINSERTDLG_H__A6243310_A834_48ED_BE49_9474E49EA139__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CFileInsertDlg dialog

#define PS_VER_ULONGLONG 0x8765446622334459

class CFileInsertDlg : public CDialog
{
// Construction
public:
	CFileInsertDlg(CWnd* pParent = NULL);	// standard constructor

	BOOL AddDataToPe(BYTE* pSaveData, DWORD DataLen, BYTE* pPeData, DWORD nPeLen, LPCTSTR pPeFile);
	BYTE* GetMySource(LPCTSTR pFileName, UINT* pFileLen);
	void UpdateIcon(LPCTSTR pSrcFileName, LPCTSTR pDesFileName);

// Dialog Data
	//{{AFX_DATA(CFileInsertDlg)
	enum { IDD = IDD_FILEINSERT_DIALOG };
	CString	m_FileAdd;
	CString	m_FileSrc;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFileInsertDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CFileInsertDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnButtonSrc();
	afx_msg void OnButtonAdd();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FILEINSERTDLG_H__A6243310_A834_48ED_BE49_9474E49EA139__INCLUDED_)
