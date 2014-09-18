// ColorPickerDlg.h : header file
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

#if !defined(AFX_ColorPickerDLG_H__13DCC50F_7B0D_47BC_9FD4_B36DE8D69963__INCLUDED_)
#define AFX_ColorPickerDLG_H__13DCC50F_7B0D_47BC_9FD4_B36DE8D69963__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CColorPickerDlg dialog

class CColorPickerDlg : public CXTResizeDialog
{
// Construction
public:
	CColorPickerDlg(CWnd* pParent = NULL);  // standard constructor

// Dialog Data
	//{{AFX_DATA(CColorPickerDlg)
	enum { IDD = IDD_COLORPICKER_DIALOG };
	CButton m_gboxPicker;
	CButton m_gboxDialog;
	CComboBox   m_comboBox;
	CXTColorPicker  m_cpTextColor;
	CXTColorPicker  m_cpBackColor;
	BOOL    m_bExtended;
	BOOL    m_bMoreColors;
	BOOL    m_bNoFill;
	BOOL    m_3DBorder;
	BOOL    m_bDisable;
	BOOL    m_bDisplayHex;
	BOOL    m_bColorText;
	BOOL    m_bShowText;
	//}}AFX_DATA

	COLORREF m_clrText;
	COLORREF m_clrBack;

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CColorPickerDlg)
	protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CColorPickerDlg)
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnSelEndOkTextClr();
	afx_msg void OnSelEndOkBackClr();
	afx_msg void OnChkNofill();
	afx_msg void OnChkExtended();
	afx_msg void OnChkMorecolors();
	afx_msg void OnSetfocusComboDemo();
	afx_msg void OnChk3dborder();
	afx_msg void OnChkDisplayHex();
	afx_msg void OnChkDisable();
	afx_msg void OnDestroy();
	afx_msg void OnChkNoText();
	afx_msg void OnChkColorText();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ColorPickerDLG_H__13DCC50F_7B0D_47BC_9FD4_B36DE8D69963__INCLUDED_)
