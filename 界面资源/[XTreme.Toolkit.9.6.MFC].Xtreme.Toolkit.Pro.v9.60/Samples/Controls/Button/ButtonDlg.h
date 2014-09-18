// ButtonDlg.h : header file
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

#if !defined(AFX_BUTTONDLG_H__7838AF65_FBA8_45D4_B811_4979E8FE9761__INCLUDED_)
#define AFX_BUTTONDLG_H__7838AF65_FBA8_45D4_B811_4979E8FE9761__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CButtonDlg dialog

class CButtonDlg : public CXTResizeDialog
{
// Construction
public:
	CButtonDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CButtonDlg)
	enum { IDD = IDD_BUTTON_DIALOG };
	CButton m_chkAlpha;
	CButton m_btnWinXPThemes;
	CButton m_chkHilite;
	CButton m_radFlat;
	CButton m_radSemiFlat;
	CButton m_radNormal;
	CButton m_chkWordTheme;
	CXTResizeGroupBox   m_gboxTwoRows;
	CXTResizeGroupBox   m_gboxRegular;
	CXTButton   m_btnOK;
	CXTButton   m_btn4;
	CXTButton   m_btn3;
	CXTButton   m_btn2;
	CXTButton   m_btn1;
	BOOL    m_bEnable;
	BOOL    m_bFocus;
	BOOL    m_bHilite;
	int     m_nFlatState;
	int     m_nTextJustification;
	BOOL    m_bHideIcons;
	BOOL    m_bWinXPThemes;
	BOOL    m_bWordTheme;
	int     m_intTheme;
	BOOL    m_bAlpha;
	BOOL    m_bToggleButtons;
	//}}AFX_DATA

	void UpdateXTStyle();
	void ModifyBtnStyle(DWORD dwRemove, DWORD dwAdd);

	DWORD m_dwXTBtnStyle;

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CButtonDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	void ChangeTheme(XTThemeStyle enumTheme);
	void EnableButtons(XTThemeStyle enumTheme);
	void SetWinxpThemes();
	void SetWordTheme();

	void UpdateIcons(BOOL bAlpha);

// Implementation
protected:
	HICON m_hIcon;
	CBitmap m_bitmap;
	CImageList m_imageList;

	// Generated message map functions
	//{{AFX_MSG(CButtonDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnChkEnabled();
	afx_msg void OnChkCentered();
	afx_msg void OnChkFocus();
	afx_msg void OnSetBorder();
	afx_msg void OnChkHilite();
	afx_msg void OnUpdateTextJustification();
	afx_msg void OnHideicons();
	afx_msg void OnDestroy();
	afx_msg void OnRadClassic();
	afx_msg void OnRadOfficeXp();
	afx_msg void OnRadOffice2003();
	afx_msg void OnChkWinxpThemes();
	afx_msg void OnChkWordTheme();
	afx_msg void OnSysColorChange();
	afx_msg void OnChkAlpha();
	afx_msg void OnCheckToggle();
	afx_msg void OnButtonClicked(UINT nID);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BUTTONDLG_H__7838AF65_FBA8_45D4_B811_4979E8FE9761__INCLUDED_)
