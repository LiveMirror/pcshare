// FlatTabCtrlDlg.h : header file
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

#if !defined(AFX_FLATTABCTRLDLG_H__2A2DDE27_AC18_4011_874A_D1D13DFBCC8E__INCLUDED_)
#define AFX_FLATTABCTRLDLG_H__2A2DDE27_AC18_4011_874A_D1D13DFBCC8E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ListCtrlForFlatTab.h"
#include "EditCtrlForFlatTab.h"
#include "TreeCtrlForFlatTab.h"
#include "SampleFormView.h"

/////////////////////////////////////////////////////////////////////////////
// CFlatTabCtrlDlg dialog

class CFlatTabCtrlDlg : public CXTResizeDialog
{
// Construction
public:
	CFlatTabCtrlDlg(CWnd* pParent = NULL);  // standard constructor
	~CFlatTabCtrlDlg();

// Dialog Data
	//{{AFX_DATA(CFlatTabCtrlDlg)
	enum { IDD = IDD_FLATTABCTRL_DIALOG };
	CStatic	m_staticBorder;
	CButton	m_chkHasArrows;
	BOOL    m_bIsBottom;
	BOOL    m_bHasArrows;
	BOOL    m_bHasHomeEnd;
	BOOL    m_bHasHScroll;
	//}}AFX_DATA

	CXTFlatTabCtrl  *m_pFlatTabCtrl;

	// use the flicker free template class, this will create the objects
	// making them flicker free!
	CXTNoFlickerWnd <CListCtrlForFlatTab> m_sheet1;
	CXTNoFlickerWnd <CEditCtrlForFlatTab> m_sheet2;
	CXTNoFlickerWnd <CTreeCtrlForFlatTab> m_sheet3;
	CXTNoFlickerWnd <CListBox>            m_sheet4;
	CXTNoFlickerWnd <CListBox>            m_sheet5;
	CXTNoFlickerWnd <CListBox>            m_sheet6;

	CSampleFormView *m_pSheet7;

	void SelectTabView(int nTab);

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFlatTabCtrlDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	//}}AFX_VIRTUAL

	void GetTabRect();
	
// Implementation
protected:
	HICON m_hIcon;

	void CreateTabs();
	int CreateViews();
	CRect m_rcTabCtrl;
	int m_dwTabStyle;

	class CTabManipulationDlg *m_pManipDlg;

	// Generated message map functions
	//{{AFX_MSG(CFlatTabCtrlDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void UpdateTabsStyle();
	afx_msg void OnManipulate();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FLATTABCTRLDLG_H__2A2DDE27_AC18_4011_874A_D1D13DFBCC8E__INCLUDED_)
