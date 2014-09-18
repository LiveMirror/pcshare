// MaskEditDlg.h : header file
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

#if !defined(AFX_MASKEDITDLG_H__A7F197B5_94A9_43FD_B1A7_81E05B7A849D__INCLUDED_)
#define AFX_MASKEDITDLG_H__A7F197B5_94A9_43FD_B1A7_81E05B7A849D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MaskHelpDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CMaskEditDlg dialog

class CMaskEditDlg : public CXTResizeDialog
{
// Construction
public:
	CMaskEditDlg(CWnd* pParent = NULL); // standard constructor

// Dialog Data
	//{{AFX_DATA(CMaskEditDlg)
	enum { IDD = IDD_MASKEDIT_DIALOG };
	CEdit   m_editPrompt;
	CButton m_btnHelp;
	CXTEdit m_editLiteral;
	CXTEdit m_editMask;
	CXTEdit m_editDefault;
	CXTListBox  m_lboxMask;
	CXTMaskEdit m_editMaskEdit;
	CString m_strEditMask;
	CXTStringHelper m_strLiteral;
	CString m_strDefault;
	BOOL    m_bDefault;
	CString m_strPrompt;
	CXTResizeGroupBox m_grpProperties;
	//}}AFX_DATA

	CMaskHelpDlg m_dlg;

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMaskEditDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;
	CXTTipWindow m_tipWindow;
	void RemoveSpaces( CString& strIn );

	// Generated message map functions
	//{{AFX_MSG(CMaskEditDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnDestroy();
	afx_msg void OnSelchangeLboxMask();
	afx_msg void OnChkDefault();
	afx_msg void OnUpdate();
	afx_msg void OnHelp();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MASKEDITDLG_H__A7F197B5_94A9_43FD_B1A7_81E05B7A849D__INCLUDED_)
