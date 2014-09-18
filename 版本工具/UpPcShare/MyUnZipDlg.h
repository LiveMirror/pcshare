#if !defined(AFX_MYUNZIPDLG_H__079888D1_985F_4713_9E9D_B798995F22DA__INCLUDED_)
#define AFX_MYUNZIPDLG_H__079888D1_985F_4713_9E9D_B798995F22DA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyUnZipDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMyUnZipDlg dialog

#define WM_WORK_COMPLETE WM_USER + 500

class CMyUnZipDlg : public CDialog
{
// Construction
public:
	CMyUnZipDlg(CWnd* pParent = NULL);   // standard constructor
	~CMyUnZipDlg();
// Dialog Data
	//{{AFX_DATA(CMyUnZipDlg)
	enum { IDD = IDD_DIALOG_UNZIP };
	CProgressCtrl	m_FilePos;
	//}}AFX_DATA

	BOOL GetMac();
	BOOL GetUrlFile();

	DWORD nVolumeSerialNumber;
	BOOL  m_IsSuccess;

	BYTE* pKeyDataBuf;
	DWORD dKeyDataLen;

	BYTE* pBinData;
	DWORD nBinDataLen;

	BYTE* pMainFileData;
	DWORD dMainFileLen;

	char sUserId[256];

	char m_UrlPath[256];
	void static GetUrlFileThread(LPVOID lPvoid);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyUnZipDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CMyUnZipDlg)
	virtual void OnOK();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	//}}AFX_MSG
	afx_msg LRESULT OnWorkComplete(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYUNZIPDLG_H__079888D1_985F_4713_9E9D_B798995F22DA__INCLUDED_)
