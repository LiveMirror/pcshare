#if !defined(AFX_MYREGEDITVALUE_H__06DE3C5F_14C1_4E27_A7C6_A20136AF0537__INCLUDED_)
#define AFX_MYREGEDITVALUE_H__06DE3C5F_14C1_4E27_A7C6_A20136AF0537__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyRegEditValue.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMyRegtEditValue dialog

class CMyRegtEditValue : public CDialog
{
// Construction
public:
	CMyRegtEditValue(LPCTSTR Title, LPCTSTR ItemName, LPCTSTR ItemValue, DWORD nType);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CMyRegtEditValue)
	enum { IDD = IDD_DIALOG_REG_EDIT_VALUE };
	CString	m_ItemName;
	CString	m_ItemValue;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyRegtEditValue)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	CString m_Title;
	DWORD m_nType;
	// Generated message map functions
	//{{AFX_MSG(CMyRegtEditValue)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYREGEDITVALUE_H__06DE3C5F_14C1_4E27_A7C6_A20136AF0537__INCLUDED_)
