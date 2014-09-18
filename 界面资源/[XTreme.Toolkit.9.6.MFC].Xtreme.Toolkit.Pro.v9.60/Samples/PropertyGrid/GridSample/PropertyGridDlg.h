// PropertyGridDlg.h : header file
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

#if !defined(AFX_PropertyGridDLG_H__03B48B0F_47BD_4632_B28B_BCE982A01581__INCLUDED_)
#define AFX_PropertyGridDLG_H__03B48B0F_47BD_4632_B28B_BCE982A01581__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CPropertyGridDlg dialog

//#include "PropertyGridBox.h"

#ifdef _XTP_INCLUDE_CONTROLS
#define CPropertyGridDlgBase CXTResizeDialog
#else
#define CPropertyGridDlgBase CDialog
#endif

class CPropertyGridDlg : public CPropertyGridDlgBase
{
// Construction
public:
	CPropertyGridDlg(CWnd* pParent = NULL); // standard constructor

// Dialog Data
	//{{AFX_DATA(CPropertyGridDlg)
	enum { IDD = IDD_PROPERTYGRID_DIALOG };
	CComboBox   m_cmbTheme;
	CStatic m_wndPlaceHolder;
	int     m_nTheme;
	BOOL    m_bTabItems;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPropertyGridDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	HICON    m_hIcon;
	CXTPPropertyGrid m_wndPropertyGrid;
	BOOL m_bCollapse;
	BOOL m_bDoubleFont;
	BOOL m_bWinXPTheme;

	afx_msg void OnClickedChkToolbar();
	afx_msg void OnClickedChkHelp();
	afx_msg void OnClickedSort();
	afx_msg void OnClickedChkDouble();
	afx_msg void OnClickedWinXPTheme();
	afx_msg LRESULT OnGridNotify(WPARAM, LPARAM);

	BOOL m_bHelpVisible;
	BOOL m_bToolBarVisible;
	int m_nSort;
	afx_msg void OnClickedCustomcolors();
	BOOL m_bCustomColors;
protected:
	virtual void OnOK();


	// Generated message map functions
	//{{AFX_MSG(CPropertyGridDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnDestroy();
	afx_msg void OnButtonSwitchstate();
	afx_msg void OnSelchangeComboTheme();
	afx_msg void OnChkTabitems();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PropertyGridDLG_H__03B48B0F_47BD_4632_B28B_BCE982A01581__INCLUDED_)
