// PropertiesDialog.h : header file
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

#if !defined(__PROPERTIESDIALOG_H__)
#define __PROPERTIESDIALOG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CPropertiesDialog dialog

class CPropertiesDialog : public CXTResizeDialog
{
// Construction
public:
	void EnableControls();
	CPropertiesDialog(CWnd* pParentWnd = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPropertiesDialog)
	enum { IDD = IDD_PROPERTIES };
	CStatic m_txtInfo;
	CStatic m_txtToolTip;
	CStatic m_txtTimeOut;
	CStatic m_txtBalloonTitle;
	CStatic m_txtBalloonMsg;
	CStatic m_txtBalloonIcon;
	CEdit   m_editTooltip;
	CEdit   m_editTimeout;
	CEdit   m_editBalloonTitle;
	CComboBox   m_comboBalloonIcon;
	CButton m_chkShowIcon;
	CButton m_chkHideIcon;
	CButton m_chkAnimateIcon;
	CButton m_btnTooltip;
	CButton m_btnShowBalloon;
	CButton m_btnMinimizeToTray;
	CStatic m_txtBalloonTip;
	CButton m_gboxBalloonTip;
	BOOL    m_bAnimateIcon;
	BOOL    m_bHideIcon;
	BOOL    m_bShowIcon;
	CString m_strBalloonMsg;
	CString m_strBalloonTitle;
	int     m_iTimeOut;
	CString m_strToolTip;
	int     m_iBalloonIcon;
	//}}AFX_DATA
	CXTNoFlickerWnd <CEdit> m_editBalloonMsg;

	static void MinMaxWindow();
	static CPropertiesDialog *m_pInstance;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPropertiesDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL


// Implementation
protected:
	HICON m_hIcon;
	static bool  m_bMinimized;
	CWnd* m_pParentWnd;

	// Generated message map functions
	//{{AFX_MSG(CPropertiesDialog)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnChkShowicon();
	afx_msg void OnChkHideicon();
	afx_msg void OnChkAnimateicon();
	afx_msg void OnChangeEditTooltip();
	afx_msg void OnBtnTooltip();
	afx_msg void OnChangeEditBalloontitle();
	afx_msg void OnChangeEditTimeout();
	afx_msg void OnChangeEditBalloonmsg();
	afx_msg void OnSelendokComboBalloonicon();
	afx_msg void OnBtnShowBalloon();
	afx_msg void OnBtnMinimizetray();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	virtual INT_PTR DoModal(CWnd* pParentWnd);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__PROPERTIESDIALOG_H__)
