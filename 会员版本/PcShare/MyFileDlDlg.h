#if !defined(AFX_MYFILEDLDLG_H__FFFD7ED7_D2C7_4EF0_AC07_967AEF71918F__INCLUDED_)
#define AFX_MYFILEDLDLG_H__FFFD7ED7_D2C7_4EF0_AC07_967AEF71918F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyFileDlDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMyFileDlDlg dialog

class CMyFileDlDlg : public CDialog
{
// Construction
public:
	CMyFileDlDlg();   // standard constructor
	~CMyFileDlDlg(); 
// Dialog Data
	//{{AFX_DATA(CMyFileDlDlg)
	enum { IDD = IDD_DIALOG_FILE_DL };
	CAnimateCtrl	m_Animal;
	CProgressCtrl	m_FileStep;
	CString	m_LocalFile;
	CString	m_RemoteFile;
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	void ReNameFileName(TCHAR* pFileName, BOOL bIsBakFileName);

	MyServerTran m_Tran;
	TCHAR m_Id[256];

	INTERFILEINFO m_FileInfo;

	BOOL bIsUserCancel;
	HANDLE hDlFilethread;

	void DownLoadFile();
	void UpLoadFile();
	static void DownLoadFileThread(LPVOID lPvoid);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyFileDlDlg)
	public:
	virtual BOOL Create(CWnd* pParentWnd, LPCTSTR pId, LPINTERFILEINFO pInfo);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMyFileDlDlg)
	afx_msg BOOL OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct);
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYFILEDLDLG_H__FFFD7ED7_D2C7_4EF0_AC07_967AEF71918F__INCLUDED_)
