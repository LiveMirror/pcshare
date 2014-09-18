// HexEditDlg.h : header file
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

#if !defined(AFX_HEXEDITDLG_H__CF147E50_214B_11D2_B9F9_0000E829B15A__INCLUDED_)
#define AFX_HEXEDITDLG_H__CF147E50_214B_11D2_B9F9_0000E829B15A__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

/////////////////////////////////////////////////////////////////////////////
// CHexEditDlg dialog

class CHexEditDlg : public CXTResizeDialog
{
// Construction
public:
	CHexEditDlg(CWnd* pParent = NULL);  // standard constructor

// Dialog Data
	//{{AFX_DATA(CHexEditDlg)
	enum { IDD = IDD_HEXEDIT_DIALOG };
	CXTColorPicker  m_cpTextHiliteDisabled;
	CXTColorPicker  m_cpBackHiliteDisabled;
	CXTColorPicker  m_cpTextEnabledDisabled;
	CXTColorPicker  m_cpBackEnabledDisabled;
	CXTColorPicker  m_cpTextHiliteEnabled;
	CXTColorPicker  m_cpBackHiliteEnabled;
	CXTColorPicker  m_cpTextEnabled;
	CXTColorPicker  m_cpBackEnabled;
	CXTMaskEdit m_editBaseAddress;
	CSliderCtrl m_slider;
	CXTHexEdit  m_editHex;
	CStatic m_txtEnabled;
	CStatic m_txtDisabled;
	BOOL    m_bAddress;
	BOOL    m_bHex;
	BOOL    m_bASCII;
	BOOL    m_bWideAddress;
	BOOL    m_bEnable;
	BOOL    m_bCaret;
	COLORREF m_crBackEnabled;
	COLORREF m_crTextEnabled;
	COLORREF m_crBackHiliteEnabled;
	COLORREF m_crTextHiliteEnabled;
	COLORREF m_crBackDisabled;
	COLORREF m_crTextDisabled;
	COLORREF m_crBackHiliteDisabled;
	COLORREF m_crTextHiliteDisabled;
	BOOL    m_bAllowDel;
	BOOL    m_bReadOnly;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHexEditDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	LPBYTE  m_pData;

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CHexEditDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnChkAddress();
	afx_msg void OnChkHexCode();
	afx_msg void OnChkAscii();
	afx_msg void OnChkWideAddress();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnChangeBaseAddress();
	afx_msg void OnEnable();
	afx_msg void OnClrBackEnabled();
	afx_msg void OnClrTextEnabled();
	afx_msg void OnClrBackHiliteEnabled();
	afx_msg void OnClrTextHiliteEnabled();
	afx_msg void OnClrBackDisabled();
	afx_msg void OnClrTextDisabled();
	afx_msg void OnClrBackHiliteDisabled();
	afx_msg void OnClrTextHiliteDisabled();
	afx_msg void OnChkCaret();
	afx_msg void OnDestroy();
	afx_msg void OnChkAllowdel();
	afx_msg void OnChkReadonly();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HEXEDITDLG_H__CF147E50_214B_11D2_B9F9_0000E829B15A__INCLUDED_)
