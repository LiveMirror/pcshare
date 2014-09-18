// SimpleGridPage.h : header file
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

#if !defined(AFX_SIMPLEGRIDPAGE_H__DC1F2249_A32D_4390_ABD9_E4240F73AFD0__INCLUDED_)
#define AFX_SIMPLEGRIDPAGE_H__DC1F2249_A32D_4390_ABD9_E4240F73AFD0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CSimpleGridPage dialog

class CSimpleGridPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CSimpleGridPage)

// Construction
public:
	CSimpleGridPage();
	~CSimpleGridPage();

	void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);

// Dialog Data
	//{{AFX_DATA(CSimpleGridPage)
	enum { IDD = IDD_PAGE_SIMPLE };
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA

	CXTPPropertyGrid m_wndPropertyGrid;

// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CSimpleGridPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CSimpleGridPage)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	afx_msg LRESULT OnGridNotify(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SIMPLEGRIDPAGE_H__DC1F2249_A32D_4390_ABD9_E4240F73AFD0__INCLUDED_)
