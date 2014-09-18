#if !defined(AFX_MYSENDMESSDLG_H__FB7826A4_6E7F_4DB3_9D07_B2BEE17EAB93__INCLUDED_)
#define AFX_MYSENDMESSDLG_H__FB7826A4_6E7F_4DB3_9D07_B2BEE17EAB93__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MySendMessDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMySendMessDlg dialog

class CMySendMessDlg : public CDialog
{
// Construction
public:
	CMySendMessDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CMySendMessDlg)
	enum { IDD = IDD_DIALOG_MESS };
	int		m_Type;
	CString	m_Title;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMySendMessDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMySendMessDlg)
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYSENDMESSDLG_H__FB7826A4_6E7F_4DB3_9D07_B2BEE17EAB93__INCLUDED_)
