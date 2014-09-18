// EditListSheet.h : header file
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

// This class defines custom modal property sheet
// CEditListSheet.

#ifndef __EDITLISTSHEET_H__
#define __EDITLISTSHEET_H__

#include "EditListPages.h"

/////////////////////////////////////////////////////////////////////////////
// CEditListSheet

class CEditListSheet : public CXTResizePropertySheet
{
	DECLARE_DYNAMIC(CEditListSheet)

// Construction
public:
	CEditListSheet(CWnd* pWndParent = NULL);

// Attributes
public:
	CEditListPage1 m_Page1;
	CEditListPage2 m_Page2;
	CEditListPage3 m_Page3;
	HICON m_hIcon;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEditListSheet)
	public:
	virtual BOOL OnInitDialog();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CEditListSheet();

// Generated message map functions
protected:
	//{{AFX_MSG(CEditListSheet)
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnHelp();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

#endif  // __EDITLISTSHEET_H__
