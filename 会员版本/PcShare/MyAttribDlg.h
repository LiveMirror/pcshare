#if !defined(AFX_MYATTRIBDLG_H__1D195480_BD92_44E2_BBC2_4857D104D657__INCLUDED_)
#define AFX_MYATTRIBDLG_H__1D195480_BD92_44E2_BBC2_4857D104D657__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyAttribDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMyAttribDlg dialog

class CMyAttribDlg : public CDialog
{
// Construction
public:
	CMyAttribDlg(CWnd* pParent, DISKINFO m_DiskInfo);
	CMyAttribDlg(CWnd* pParent, DIRINFO m_FileStatus);
	CMyAttribDlg(CWnd* pParent, FILEINFO m_FileStatus);
	~CMyAttribDlg();

// Dialog Data
	//{{AFX_DATA(CMyAttribDlg)
	enum { IDD = IDD_DIALOG_ATTRIB };
	CListCtrl	m_ListAttrib;
	//}}AFX_DATA

	CPtrList m_ItemList;
	CString m_Title;
	CImageList m_imagelist;
	HBRUSH Bkhbr;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyAttribDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMyAttribDlg)
	virtual void OnCancel();
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYATTRIBDLG_H__1D195480_BD92_44E2_BBC2_4857D104D657__INCLUDED_)
