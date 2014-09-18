// DialogNewControl.h : header file
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

#if !defined(AFX_DIALOGNEWCONTROL_H__0BBC727F_3E04_479F_8B0C_9843E2C3ECCD__INCLUDED_)
#define AFX_DIALOGNEWCONTROL_H__0BBC727F_3E04_479F_8B0C_9843E2C3ECCD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CDialogNewControl dialog

class CDialogNewControl : public CDialog
{
// Construction
public:
	CDialogNewControl(CWnd* pParent = NULL);   // standard constructor


// Dialog Data
	//{{AFX_DATA(CDialogNewControl)
	enum { IDD = IDD_DIALOG_CONTROLS_NEW };
	CComboBox   m_comboType;
	CComboBox   m_comboId;
	CComboBox   m_comboCategory;
	CString m_strCaption;
	CString m_strType;
	CString m_strId;
	CString m_strCategory;
	//}}AFX_DATA

	BOOL m_bGenerateId;

	void GenerateID();
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogNewControl)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDialogNewControl)
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeEditCaption();
	afx_msg void OnEditchangeComboCategory();
	afx_msg void OnSelchangeComboCategory();
	afx_msg void OnEditchangeComboId();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};




//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOGNEWCONTROL_H__0BBC727F_3E04_479F_8B0C_9843E2C3ECCD__INCLUDED_)
