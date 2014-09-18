#if !defined(AFX_MYFILEUPDLG_H__95D4F7A1_2DC0_4BC0_96CF_0852D60B05D4__INCLUDED_)
#define AFX_MYFILEUPDLG_H__95D4F7A1_2DC0_4BC0_96CF_0852D60B05D4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyFileUpDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMyFileUpDlg dialog

class CMyFileUpDlg : public CDialog
{
// Construction
public:
	CMyFileUpDlg();   // standard constructor
	~CMyFileUpDlg(); 

// Dialog Data
	//{{AFX_DATA(CMyFileUpDlg)
	enum { IDD = IDD_DIALOG_FILE_UP };
	CAnimateCtrl	m_Animal;
	CProgressCtrl	m_FileStep;
	CString	m_LocalFile;
	CString	m_RemoteFile;
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	MyServerTran m_Tran;
	TCHAR m_Id[256];

	INTERFILEINFO m_FileInfo;

	BOOL bIsUserCancel;
	HANDLE hDlFilethread;

	void UpLoadFile();
	static void UpLoadFileThread(LPVOID lPvoid);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyFileUpDlg)
	public:
	virtual BOOL Create(CWnd* pParentWnd, LPCTSTR pId, LPINTERFILEINFO pInfo);	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMyFileUpDlg)
	virtual BOOL OnInitDialog();
	afx_msg BOOL OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct);
	virtual void OnCancel();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYFILEUPDLG_H__95D4F7A1_2DC0_4BC0_96CF_0852D60B05D4__INCLUDED_)
