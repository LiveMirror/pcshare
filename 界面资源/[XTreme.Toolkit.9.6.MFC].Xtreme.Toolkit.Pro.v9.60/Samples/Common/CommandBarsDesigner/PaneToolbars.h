// PaneToolbars.h : header file
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

#if !defined(AFX_PANETOOLBARS_H__94DE84EA_41AC_47FD_A540_082A4FC61E35__INCLUDED_)
#define AFX_PANETOOLBARS_H__94DE84EA_41AC_47FD_A540_082A4FC61E35__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CPaneToolbars window

class CPaneToolbars : public CWnd
{
	class CToolbarsTree : public CTreeCtrl
	{
		LRESULT WindowProc(UINT message,WPARAM wParam,LPARAM lParam);
	};
// Construction
public:
	CPaneToolbars();

// Attributes
public:
	CXTPToolBar m_wndToolBar;

	CToolbarsTree m_wndTreeCtrl;
	CImageList m_ilTreeIcons;

	BOOL m_bLockSelected;

	CXTPCommandBars* m_pActiveCommandBars;

// Operations
public:
	void Refresh();
	void RefreshItem(HTREEITEM hItem);

	int BuildToolbarsTree(HTREEITEM hItem, CXTPCommandBar* pCommandBar, BOOL bCheck);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPaneToolbars)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CPaneToolbars();

	// Generated message map functions
protected:
	//{{AFX_MSG(CPaneToolbars)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPanetoolbarsAdd();
	afx_msg void OnUpdatePanetoolbarsAdd(CCmdUI* pCmdUI);
	afx_msg void OnUpdatePanetoolbarsDelete(CCmdUI* pCmdUI);
	afx_msg void OnPanetoolbarsDelete();
	//}}AFX_MSG
	afx_msg void OnSelChanged(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSetFocus(NMHDR* pNMHDR, LRESULT* pResult);


	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PANETOOLBARS_H__94DE84EA_41AC_47FD_A540_082A4FC61E35__INCLUDED_)
