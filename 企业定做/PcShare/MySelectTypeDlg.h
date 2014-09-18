#if !defined(AFX_MYSELECTTYPEDLG_H__87EAD387_E582_42C0_A905_B631CD53FC6C__INCLUDED_)
#define AFX_MYSELECTTYPEDLG_H__87EAD387_E582_42C0_A905_B631CD53FC6C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MySelectTypeDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMySelectTypeDlg dialog

class CMySelectTypeDlg : public CDialog
{
// Construction
public:
	CMySelectTypeDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CMySelectTypeDlg)
	enum { IDD = IDD_DIALOG_SELECT_TYPE };
	int		m_LoginType;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMySelectTypeDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMySelectTypeDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYSELECTTYPEDLG_H__87EAD387_E582_42C0_A905_B631CD53FC6C__INCLUDED_)
