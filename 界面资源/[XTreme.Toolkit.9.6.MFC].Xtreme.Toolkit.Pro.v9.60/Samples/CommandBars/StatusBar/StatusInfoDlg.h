// StatusInfoDlg.h : header file
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

#if !defined(AFX_STATUSINFODLG_H__ED213BBD_B2CC_4AEC_B0C2_041BF59546EC__INCLUDED_)
#define AFX_STATUSINFODLG_H__ED213BBD_B2CC_4AEC_B0C2_041BF59546EC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CStatusInfoDlg dialog

class CMainFrame;
class CStatusInfoDlg : public CDialog
{
// Construction
public:
	CStatusInfoDlg(
		CEdit& wndEditCtrl,
		CAnimateCtrl& wndAnimCtrl,
		CProgressCtrl& wndProgCtrl,
		CXTPStatusBarLogoPane& wndLogoPane,
		CXTPStatusBarIconPane& wndIconPane,
		CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CStatusInfoDlg)
	enum { IDD = IDD_STATUS_INFO };
	CButton m_chkPlayAnim;
	CButton m_chkEnableIcon;
	CButton m_btnTestProg;
	BOOL    m_bAnim;
	BOOL    m_bEdit;
	BOOL    m_bLogo;
	BOOL    m_bProgress;
	BOOL    m_bIcon;
	BOOL    m_bEnableIcon;
	BOOL    m_bPlayAnim;
	//}}AFX_DATA

	CEdit         &m_wndEditCtrl;
	CAnimateCtrl  &m_wndAnimCtrl;
	CProgressCtrl &m_wndProgCtrl;
	CXTPStatusBarLogoPane   &m_wndLogoPane;
	CXTPStatusBarIconPane   &m_wndIconPane;
	CMainFrame    *m_pMainFrame;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStatusInfoDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CStatusInfoDlg)
	afx_msg void OnChkAnim();
	afx_msg void OnChkEdit();
	afx_msg void OnChkLogo();
	afx_msg void OnChkProgress();
	afx_msg void OnBtnTesprog();
	virtual BOOL OnInitDialog();
	afx_msg void OnChkIcon();
	afx_msg void OnChkEnableicon();
	afx_msg void OnChkPlayanim();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STATUSINFODLG_H__ED213BBD_B2CC_4AEC_B0C2_041BF59546EC__INCLUDED_)
