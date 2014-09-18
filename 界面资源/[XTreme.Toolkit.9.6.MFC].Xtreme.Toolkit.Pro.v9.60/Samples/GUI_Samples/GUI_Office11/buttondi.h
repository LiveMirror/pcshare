// buttondi.h : header file
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
// CButtonDialog dialog

class CButtonDialog : public CCSDialog
{
// Construction
public:
	CButtonDialog(LPCTSTR lpszText, LPCTSTR lpszCaption, LPCTSTR lpszButtons,
		WORD wStyle, DWORD* pHelpIDs = NULL, CWnd* pParentWnd = NULL);
	~CButtonDialog();

// Attributes
	CFont m_font;
// Operations
	static int DisplayMessageBox(LPCTSTR lpszText, LPCTSTR lpszCaption,
		LPCTSTR lpszButtons, WORD wStyle, int nDef = 0, int nCancel = -1,
		DWORD* pHelpIDs = NULL, CWnd* pParentWnd = NULL);

	void AddButton(CString& strButton) { m_strArray.Add(strButton);}
	void AddButtons(LPCTSTR lpszButton);
	void SetCancel(int nCancel)
		{ ASSERT(nCancel < m_strArray.GetSize()); m_nCancel = nCancel;}
	void SetDefault(int nDef)
		{ ASSERT(nDef < m_strArray.GetSize()); m_nDefButton = nDef;}
	void FillInHeader(LPDLGTEMPLATE lpDlgTmp);

// Overridables
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual int DoModal();

// Implementation
protected:
	virtual const DWORD* GetHelpIDs() {return m_pHelpIDs;}
	DWORD* m_pHelpIDs;
	int m_nDefButton;
	int m_nCancel;
	HGLOBAL m_hDlgTmp;
	UINT m_nBaseID;
	WORD m_wStyle;
	CButton* m_pButtons;
	CStatic m_staticIcon;
	CStatic m_staticText;
	CString m_strCaption;
	CString m_strText;
	CStringArray m_strArray;
	CSize GetBaseUnits();
	LPCTSTR GetIconID(WORD wFlags);
	void PositionControls();

	// Generated message map functions
	//{{AFX_MSG(CButtonDialog)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
