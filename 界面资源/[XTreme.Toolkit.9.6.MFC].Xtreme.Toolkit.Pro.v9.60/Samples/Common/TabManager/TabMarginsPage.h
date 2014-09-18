// TabMarginsPage.h : header file
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

#if !defined(AFX_TABMARGINSPAGE_H__AD997199_7224_4972_B4AB_7C77E2E39748__INCLUDED_)
#define AFX_TABMARGINSPAGE_H__AD997199_7224_4972_B4AB_7C77E2E39748__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CTabMarginsPage dialog

class CTabMarginsPage : public CDialog
{
// Construction
public:
	CTabMarginsPage(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CTabMarginsPage)
	enum { IDD = IDD_TABCTRL_MARGINS };
	//}}AFX_DATA

	BOOL    m_bApply[4];
	CXTPTabPaintManager* GetPaintManager(int nIndex);
	CXTPTabPaintManager* GetPaintManager();

	CRect m_rcControlMargin;
	CRect m_rcHeaderMargin;
	CRect m_rcClientMargin;
	CRect m_rcButtonMargin;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTabMarginsPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	virtual void OnCancel();
	virtual void OnOK();
	virtual void Update();

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTabMarginsPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnCheckApply();
	afx_msg void OnChangeEditControl();
	afx_msg void OnChangeEditClient();
	afx_msg void OnChangeEditHeader();
	afx_msg void OnChangeEditButton();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TABMARGINSPAGE_H__AD997199_7224_4972_B4AB_7C77E2E39748__INCLUDED_)
