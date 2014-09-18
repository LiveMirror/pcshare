// ACDSeeTabCtrl.h : header file
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

#if !defined(AFX_ACDSEETABCTRL_H__60B90AFB_03D7_4DAC_8E8F_8E8C373AB472__INCLUDED_)
#define AFX_ACDSEETABCTRL_H__60B90AFB_03D7_4DAC_8E8F_8E8C373AB472__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CACDSeeTabCtrl window

class CACDSeeTabCtrl : public CTabCtrl
{
// Construction
public:
	CACDSeeTabCtrl();

// Attributes
public:

// Operations
public:
	DECLARE_DYNCREATE(CACDSeeTabCtrl)

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CACDSeeTabCtrl)
	//}}AFX_VIRTUAL
#if _MSC_VER < 1200 // MFC 5.0

	BOOL InsertItem(int nItem, LPCTSTR lpszItem, int nImage)
		{ ASSERT(::IsWindow(m_hWnd)); return InsertItem(TCIF_TEXT|TCIF_IMAGE, nItem, lpszItem, nImage, 0); }
	BOOL InsertItem(UINT nMask, int nItem, LPCTSTR lpszItem,
		int nImage, LPARAM lParam)
	{
		ASSERT(::IsWindow(m_hWnd));

		TCITEM item;
		item.mask = nMask;
		item.iImage = nImage;
		item.lParam = lParam;
		item.pszText = (LPTSTR) lpszItem;

		return (BOOL) ::SendMessage(m_hWnd, TCM_INSERTITEM, nItem, (LPARAM) &item);
	}
#endif


// Implementation
public:
	virtual ~CACDSeeTabCtrl();

	void DrawItem(LPDRAWITEMSTRUCT);

	// Generated message map functions
protected:
	//{{AFX_MSG(CACDSeeTabCtrl)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};



/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ACDSEETABCTRL_H__60B90AFB_03D7_4DAC_8E8F_8E8C373AB472__INCLUDED_)
