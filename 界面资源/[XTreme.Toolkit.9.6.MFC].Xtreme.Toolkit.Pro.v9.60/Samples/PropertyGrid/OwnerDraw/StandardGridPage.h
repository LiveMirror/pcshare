// StandardGridPage.h : header file
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

#if !defined(AFX_STANDARDGRIDPAGE_H__68AF0522_40CB_4EFD_A9D2_4F504B02C828__INCLUDED_)
#define AFX_STANDARDGRIDPAGE_H__68AF0522_40CB_4EFD_A9D2_4F504B02C828__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CStandardGridPage dialog

class CStandardGridPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CStandardGridPage)

// Construction
public:
	CStandardGridPage();
	~CStandardGridPage();

// Dialog Data
	//{{AFX_DATA(CStandardGridPage)
	enum { IDD = IDD_PAGE_STANDARD };
	BOOL    m_bCustomColors;
	//}}AFX_DATA


	CXTPPropertyGrid m_wndPropertyGrid;

// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CStandardGridPage)
	public:
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CStandardGridPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnClickedCustomcolors();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STANDARDGRIDPAGE_H__68AF0522_40CB_4EFD_A9D2_4F504B02C828__INCLUDED_)
