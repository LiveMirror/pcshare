// ListCtrlDlg.h : header file
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

#if !defined(AFX_LISTCTRLDLG_H__1141BD3E_360E_46A6_BB41_C9B238048BA3__INCLUDED_)
#define AFX_LISTCTRLDLG_H__1141BD3E_360E_46A6_BB41_C9B238048BA3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CListCtrlDlg dialog

class CListCtrlDlg : public CXTResizeDialog
{
// Construction
public:
	CListCtrlDlg(CWnd* pParent = NULL); // standard constructor

// Dialog Data
	//{{AFX_DATA(CListCtrlDlg)
	enum { IDD = IDD_LISTCTRL_DIALOG };
	int                 m_iTheme;
	int                 m_iDisableSizing;
	int                 m_iMinSize;
	BOOL                m_bBoldText;
	BOOL                m_bSortArrow;
	BOOL                m_bAutoSize;
	BOOL                m_bContextMenus;
	BOOL                m_bHotTracking;
	BOOL                m_bHeaderImages;
	BOOL                m_bWinTheme;
	BOOL                m_bDisableSizing;
	BOOL                m_bMinSize;
	BOOL                m_bSortColor;
	CEdit               m_editMinSize;
	CButton             m_btnWinTheme;
	CButton             m_btnHotTracking;
	CComboBox           m_comboThemes;
	CComboBox           m_comboDisableSize;
	BOOL    m_bFullRowSel;
	BOOL    m_bGridLines;
	BOOL    m_bCheckBoxes;
	BOOL    m_bDragDrop;
	BOOL    m_bListColor;
	BOOL    m_bRowColor;
	BOOL    m_bEnableList;
	//}}AFX_DATA
	CXTListCtrl         m_listCtrl;
	COLORREF            m_crSortText;
	COLORREF            m_crSortBack;
	COLORREF            m_crListText;
	COLORREF            m_crListBack;
	COLORREF            m_crRowText;
	COLORREF            m_crRowBack;
	CXTColorPicker      m_cpSortText;
	CXTColorPicker      m_cpSortBack;
	CXTColorPicker      m_cpListText;
	CXTColorPicker      m_cpListBack;
	CXTColorPicker      m_cpRowText;
	CXTColorPicker      m_cpRowBack;
	CXTResizeGroupBox   m_gboxList;
	CXTResizeGroupBox   m_gboxHeader;

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CListCtrlDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	//}}AFX_VIRTUAL

// Implementation
protected:

	HICON           m_hIcon;
	int             m_nSortedCol;
	bool            m_bAscending;
	CXTHeaderCtrl   m_header;

	void ClearSelection();
	void EnableControls(BOOL bRedraw=TRUE);
	void ModifyDrawStyle(UINT uFlag, BOOL bRemove);
	void SortColumn(int iCol, bool bAsc);

	// Generated message map functions
	//{{AFX_MSG(CListCtrlDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnChkBoldtext();
	afx_msg void OnChkSortarw();
	afx_msg void OnDestroy();
	afx_msg void OnSelendokComboThemes();
	afx_msg void OnSelendokComboDisablesize();
	afx_msg void OnChkWintheme();
	afx_msg void OnChkHottracking();
	afx_msg void OnChkImages();
	afx_msg void OnChkAutosize();
	afx_msg void OnChkContextmenus();
	afx_msg void OnChangeEditMinsize();
	afx_msg void OnChkDisablesizing();
	afx_msg void OnChkMinimumsize();
	afx_msg void OnChkSortcolor();
	afx_msg void OnChkFullRowSelect();
	afx_msg void OnChkGridLines();
	afx_msg void OnChkCheckBoxes();
	afx_msg void OnChkHeaderDragDrop();
	afx_msg void OnChkListcolor();
	afx_msg void OnChkRowcolor();
	afx_msg void OnBtnSizeToFit();
	afx_msg void OnChkEnable();
	afx_msg void OnBtnUpdate();
	afx_msg void OnBtnReset();
	//}}AFX_MSG
	afx_msg void OnSelEndOkSortBackColor();
	afx_msg void OnSelEndOkSortTextColor();
	afx_msg void OnSelEndOkListBackColor();
	afx_msg void OnSelEndOkListTextColor();
	afx_msg void OnSelEndOkRowBackColor();
	afx_msg void OnSelEndOkRowTextColor();
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LISTCTRLDLG_H__1141BD3E_360E_46A6_BB41_C9B238048BA3__INCLUDED_)
