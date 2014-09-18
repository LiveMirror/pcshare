// TrayIconDlgDlg.h : header file
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

#if !defined(AFX_TRAYICONDLGDLG_H__4D07F785_F2F4_4995_88EF_8503DAE9D596__INCLUDED_)
#define AFX_TRAYICONDLGDLG_H__4D07F785_F2F4_4995_88EF_8503DAE9D596__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CTrayIconDlgDlg dialog

class CTrayIconDlgDlg : public CXTResizeDialog
{
// Construction
public:
	void EnableControls();
	CTrayIconDlgDlg(CWnd* pParentWnd = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CTrayIconDlgDlg)
	enum { IDD = IDD_TRAYICONDLG_DIALOG };
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

	void MinMaxWindow();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTrayIconDlgDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON       m_hIcon;
	bool        m_bMinimized;
	CXTTrayIcon m_TrayIcon;

	// Generated message map functions
	//{{AFX_MSG(CTrayIconDlgDlg)
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
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
	afx_msg void OnRestore();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TRAYICONDLGDLG_H__4D07F785_F2F4_4995_88EF_8503DAE9D596__INCLUDED_)
