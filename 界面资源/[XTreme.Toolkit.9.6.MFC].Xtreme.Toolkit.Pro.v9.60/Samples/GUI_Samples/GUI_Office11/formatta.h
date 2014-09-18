// formatta.h : header file
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
// CFormatTabDlg dialog

class CFormatTabDlg : public CCSDialog
{
// Construction
public:
	CFormatTabDlg(PARAFORMAT& pf, CWnd* pParent = NULL);    // standard constructor
	~CFormatTabDlg();
	PARAFORMAT m_pf;
	LONG* m_tabarray;
	int m_nCount;

// Dialog Data
	//{{AFX_DATA(CFormatTabDlg)
	enum { IDD = IDD_FORMAT_TAB };
	CButton m_buttonClearAll;
	CButton m_buttonSet;
	CButton m_buttonClear;
	CComboBox   m_comboBox;
	//}}AFX_DATA

// Implementation
protected:
	static const DWORD m_nHelpIDs[];
	virtual const DWORD* GetHelpIDs() {return m_nHelpIDs;}
	void UpdateButton(CButton& button, BOOL b);
	void UpdateButtons();
	BOOL Set();
	BOOL AddTabToArray(LONG lTab);
	BOOL RemoveTabFromArray(LONG lTab);
	void RemoveTabFromArrayByIndex(int nIndex);
	void UpdateListBox();
	void SetEditFocus();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnOK();

	// Generated message map functions
	//{{AFX_MSG(CFormatTabDlg)
	afx_msg void OnClickedClear();
	afx_msg void OnClickedClearAll();
	afx_msg void OnClickedSet();
	afx_msg void OnEditChange();
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchange();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
