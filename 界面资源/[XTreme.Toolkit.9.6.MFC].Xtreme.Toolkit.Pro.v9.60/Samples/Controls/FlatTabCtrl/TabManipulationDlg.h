// TabManipulationDlg.h : implementation file
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

#if !defined(AFX_TABMANIPULATIONDLG_H__A603B61C_D2D3_4DCF_8CBD_36F9B66988FF__INCLUDED_)
#define AFX_TABMANIPULATIONDLG_H__A603B61C_D2D3_4DCF_8CBD_36F9B66988FF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CTabManipulationDlg dialog

class CXTFlatTabCtrl;

class CTabManipulationDlg : public CXTResizeDialog
{
// Construction
public:
	CTabManipulationDlg(CXTFlatTabCtrl *pFlatTabCtrl,
		CWnd* pParent = NULL);
	~CTabManipulationDlg();

// Dialog Data
	//{{AFX_DATA(CTabManipulationDlg)
	enum { IDD = IDD_MANIPULATE };
	UINT    m_iTabNum;
	CString m_sText;
	//}}AFX_DATA

	void UpdateDlgStatus();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTabManipulationDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTabManipulationDlg)
	afx_msg void OnAddTab();
	afx_msg void OnDeleteAll();
	afx_msg void OnDeleteTab();
	afx_msg void OnModifyTab();
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	CXTFlatTabCtrl *m_pFlatTabCtrl;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TABMANIPULATIONDLG_H__A603B61C_D2D3_4DCF_8CBD_36F9B66988FF__INCLUDED_)
