// MDIChildWndEx.h : header file
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

#if !defined(AFX_MDICHILDWNDEX_H__8C7454C9_412A_4B16_B96F_04941C600B81__INCLUDED_)
#define AFX_MDICHILDWNDEX_H__8C7454C9_412A_4B16_B96F_04941C600B81__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CMDIChildWndEx frame

class CMDIChildWndEx : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CMDIChildWndEx)
protected:
	CMDIChildWndEx();           // protected constructor used by dynamic creation

// Attributes
public:
	int m_nColor;

// Operations
public:
	afx_msg LRESULT OnGetTabColor(WPARAM, LPARAM);

	afx_msg void OnUpdateSelectColor(CCmdUI* pCmdUI);
	afx_msg void OnSelectColor(UINT nColor);

	afx_msg void OnUpdateSelectColorNone(CCmdUI* pCmdUI);
	afx_msg void OnSelectColorNone();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMDIChildWndEx)
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CMDIChildWndEx();

	// Generated message map functions
	//{{AFX_MSG(CMDIChildWndEx)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MDICHILDWNDEX_H__8C7454C9_412A_4B16_B96F_04941C600B81__INCLUDED_)
