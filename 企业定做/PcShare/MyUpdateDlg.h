#if !defined(AFX_MYUPDATEDLG_H__2AAF6A61_F2D9_43BA_BC6F_177BD20EFA1E__INCLUDED_)
#define AFX_MYUPDATEDLG_H__2AAF6A61_F2D9_43BA_BC6F_177BD20EFA1E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyUpdateDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMyUpdateDlg dialog

class CMyUpdateDlg : public CDialog
{
// Construction
public:
	CMyUpdateDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CMyUpdateDlg)
	enum { IDD = IDD_DIALOG_UPDATE };
	CString	m_Value;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyUpdateDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMyUpdateDlg)
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnButtonFile();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYUPDATEDLG_H__2AAF6A61_F2D9_43BA_BC6F_177BD20EFA1E__INCLUDED_)
