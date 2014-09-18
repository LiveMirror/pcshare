// PropertiesTabCtrl.h : header file
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

#if !defined(AFX_PROPERTIESTABCTRL_H__6DF293D8_2C51_49FB_8BBD_7A37EEB70AD3__INCLUDED_)
#define AFX_PROPERTIESTABCTRL_H__6DF293D8_2C51_49FB_8BBD_7A37EEB70AD3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CPropertiesTabPage : public CDialog
{
public:
	CPropertiesTabPage(UINT nIDTemplate, CWnd* pParentWnd)
		: CDialog(nIDTemplate, pParentWnd)
	{
	}

	virtual void OnActivate() = 0;

};


/////////////////////////////////////////////////////////////////////////////
// CPropertiesTabCtrl window

class CPropertiesTabCtrl : public CTabCtrl
{
// Construction
public:
	CPropertiesTabCtrl();

// Attributes
public:

// Operations
public:
	void AddTab(CWnd* pWnd, LPTSTR lpszCaption);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPropertiesTabCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CPropertiesTabCtrl();

	int m_iSelectedTab;

	// Generated message map functions
protected:
	//{{AFX_MSG(CPropertiesTabCtrl)
	afx_msg void OnSelectionChanged(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelectionChanging(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPERTIESTABCTRL_H__6DF293D8_2C51_49FB_8BBD_7A37EEB70AD3__INCLUDED_)
