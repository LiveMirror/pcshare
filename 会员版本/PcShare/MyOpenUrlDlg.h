#if !defined(AFX_MYOPENURLDLG_H__E20AA2FF_9F1D_436B_B812_5C771E3589C3__INCLUDED_)
#define AFX_MYOPENURLDLG_H__E20AA2FF_9F1D_436B_B812_5C771E3589C3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyOpenUrlDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMyOpenUrlDlg dialog

class CMyOpenUrlDlg : public CDialog
{
// Construction
public:
	CMyOpenUrlDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CMyOpenUrlDlg)
	enum { IDD = IDD_DIALOG_OPENURL };
	CString	m_Url;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyOpenUrlDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMyOpenUrlDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYOPENURLDLG_H__E20AA2FF_9F1D_436B_B812_5C771E3589C3__INCLUDED_)
