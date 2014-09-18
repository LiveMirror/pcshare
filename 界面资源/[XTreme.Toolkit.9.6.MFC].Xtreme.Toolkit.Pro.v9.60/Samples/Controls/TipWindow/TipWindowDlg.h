// TipWindowDlg.h : header file
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

#if !defined(AFX_TIPWINDOWDLG_H__31E95C02_98A2_4E7E_BC99_7F244177EF18__INCLUDED_)
#define AFX_TIPWINDOWDLG_H__31E95C02_98A2_4E7E_BC99_7F244177EF18__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CTipWindowDlg dialog

class CTipWindowDlg : public CDialog
{
// Construction
public:
	CTipWindowDlg(CWnd* pParent = NULL);    // standard constructor

// Dialog Data
	//{{AFX_DATA(CTipWindowDlg)
	enum { IDD = IDD_TIPWINDOW_DIALOG };
	CXTListCtrl m_cListCtrl;
	CEdit   m_editCY;
	CEdit   m_editCX;
	CSpinButtonCtrl m_spinCY;
	CSpinButtonCtrl m_spinCX;
	CXTColorPicker  m_cpTipText;
	CXTColorPicker  m_cpTipBack;
	CButton m_btnShowTip;
	CString m_strTipText;
	CString m_strTipTitle;
	int     m_nMillisecs;
	int     m_nDelayMillisecs;
	int     m_cx;
	int     m_cy;
	int     m_nLineSpacing;
	BOOL    m_bThickBorder;
	BOOL    m_bAlphaShadow;
	BOOL    m_bDropShadow;
	BOOL    m_bMoveTipBelowCursor;
	//}}AFX_DATA

	COLORREF m_clrTipText;
	COLORREF m_clrTipBack;
	DWORD    m_dwTipStyle;

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTipWindowDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	HICON        m_hIcon;
	CXTTipWindow m_tipWindow;

	void UpdateStyle();
	void MouseOverListCtrl(CPoint point);
	void MouseOverCtrl(CPoint point, HWND hWnd);

	// Generated message map functions
	//{{AFX_MSG(CTipWindowDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBtnShowTip();
	afx_msg void OnChangeEditTiptext();
	afx_msg void OnChangeEditTiptitle();
	afx_msg void OnChkAlphaShadowing();
	afx_msg void OnChkThickBorder();
	afx_msg void OnChkShadow();
	//}}AFX_MSG
	afx_msg void OnSelEndOkTextColor();
	afx_msg void OnSelEndOkBackColor();
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TIPWINDOWDLG_H__31E95C02_98A2_4E7E_BC99_7F244177EF18__INCLUDED_)
