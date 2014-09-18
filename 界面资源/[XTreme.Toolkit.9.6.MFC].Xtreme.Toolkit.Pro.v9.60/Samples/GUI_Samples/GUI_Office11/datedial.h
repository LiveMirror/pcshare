// datedial.h : header file
//
// This file is a part of the XTREME TOOLKIT PRO MFC class library.
// ©1998-2005 Codejock Software, All Rights Reserved.
//
// THIS SOURCE FILE IS THE PROPERTY OF CODEJOCK SOFTWARE AND IS NOT TO BE
// RE-DISTRIBUTED BY ANY MEANS WHATSOEVER WITHOUT THE EXPRESSED WRITTEN
// CONSENT OF CODEJOCK SOFTWARE.
//
// THIS SOURCE CODE CAN ONLY BE USED UNDER THE TERMS AND CONDITIONS OUTLINED
// IN THE XTREME TOOLKIT PRO LICENSE AGREEMENT. CODEJOCK SOFTWARE GRANTS TO
// YOU (ONE SOFTWARE DEVELOPER) THE LIMITED RIGHT TO USE THIS SOFTWARE ON A
// SINGLE COMPUTER.
//
// CONTACT INFORMATION:
// support@codejock.com
// http://www.codejock.com
//
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// CDateDialog dialog

class CDateDialog : public CCSDialog
{
// Construction
public:
	CDateDialog(CWnd* pParent = NULL);   // standard constructor

// Attributes
	static SYSTEMTIME m_time;
	static LCID m_id;
	static CListBox* m_pListBox;
	static BOOL CALLBACK DateFmtEnumProc(LPTSTR lpszFormatString);
	static BOOL CALLBACK TimeFmtEnumProc(LPTSTR lpszFormatString);

// Dialog Data
	//{{AFX_DATA(CDateDialog)
	enum { IDD = IDD_DATEDIALOG };
	CListBox    m_listBox;
	CString m_strSel;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDateDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	static const DWORD m_nHelpIDs[];
	virtual const DWORD* GetHelpIDs() {return m_nHelpIDs;}

	// Generated message map functions
	//{{AFX_MSG(CDateDialog)
	virtual BOOL OnInitDialog();
	afx_msg void OnDblclkDatedialogList();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
